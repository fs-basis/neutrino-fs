/*
	Framebuffer acceleration hardware abstraction functions.
	The hardware dependent framebuffer acceleration functions for STi chips
	are represented in this class.

	(C) 2017 Stefan Seyfried

	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <driver/fb_generic.h>
#include <driver/fb_accel.h>
#ifdef ENABLE_GRAPHLCD
#include <driver/nglcd.h>
#endif

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <memory.h>
#include <math.h>
#include <limits.h>

#include <linux/kd.h>

#include <stdlib.h>

#include <linux/stmfb.h>
#include <bpamem.h>

#include <driver/abstime.h>
#include <system/set_threadname.h>
#include <global.h>
#include <cs_api.h>

/* note that it is *not* enough to just change those values */
#define DEFAULT_XRES 1280
#define DEFAULT_YRES 720
#define DEFAULT_BPP  32

#define LOGTAG "[fb_accel_sti_noauto] "
static int bpafd = -1;
static size_t lbb_sz = 1920 * 1080;	/* offset from fb start in 'pixels' */
static size_t lbb_off = lbb_sz * sizeof(fb_pixel_t);	/* same in bytes */
static int backbuf_sz = 0;

inline fb_pixel_t mergeColor(fb_pixel_t oc, int ol, fb_pixel_t ic, int il)
{
	return   ((( (oc >> 24)         * ol) + ( (ic >> 24)         * il)) >> 8) << 24
		|(((((oc >> 16) & 0xff) * ol) + (((ic >> 16) & 0xff) * il)) >> 8) << 16
		|(((((oc >> 8)  & 0xff) * ol) + (((ic >>  8) & 0xff) * il)) >> 8) << 8
		|(((( oc        & 0xff) * ol) + (( ic        & 0xff) * il)) >> 8);
}

void CFbAccelSTi::waitForIdle(const char *)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> m_lock(blit_mutex);
	ioctl(fd, STMFBIO_SYNC_BLITTER);
}

CFbAccelSTi::CFbAccelSTi()
{
	fb_name = "STx7xxx framebuffer (no auto blit)";
}

void CFbAccelSTi::init(const char * const)
{
	CFrameBuffer::init();
	if (lfb == NULL) {
		printf(LOGTAG "CFrameBuffer::init() failed.\n");
		return; /* too bad... */
	}
	available = fix.smem_len;
	printf(LOGTAG "%dk video mem\n", available / 1024);
	memset(lfb, 0, available);

	lbb = lfb;	/* the memory area to draw to... */
	if (available < 12*1024*1024)
	{
		/* for old installations that did not upgrade their module config
		 * it will still work good enough to display the message below */
		fprintf(stderr, "[neutrino] WARNING: not enough framebuffer memory available!\n");
		fprintf(stderr, "[neutrino]          I need at least 12MB.\n");
		FILE *f = fopen("/tmp/infobar.txt", "w");
		if (f) {
			fprintf(f, "NOT ENOUGH FRAMEBUFFER MEMORY!");
			fclose(f);
		}
		lbb_sz = 0;
		lbb_off = 0;
	}
	lbb = lfb + lbb_sz;
	bpafd = open("/dev/bpamem0", O_RDWR | O_CLOEXEC);
	if (bpafd < 0)
	{
		fprintf(stderr, "[neutrino] FB: cannot open /dev/bpamem0: %m\n");
		return;
	}
	backbuf_sz = 1280 * 720 * sizeof(fb_pixel_t);
	BPAMemAllocMemData bpa_data;
#if BOXMODEL_OCTAGON1008 || BOXMODEL_FORTIS_HDBOX || BOXMODEL_CUBEREVO || BOXMODEL_CUBEREVO_MINI || BOXMODEL_CUBEREVO_MINI2 || BOXMODEL_CUBEREVO_250HD || BOXMODEL_CUBEREVO_2000HD || BOXMODEL_CUBEREVO_3000HD || BOXMODEL_IPBOX9900 || BOXMODEL_IPBOX99 || BOXMODEL_IPBOX55 || BOXMODEL_TF7700 || BOXMODEL_HL101
	bpa_data.bpa_part = (char *)"LMI_SYS";
#else
	bpa_data.bpa_part = (char *)"LMI_VID";
#endif
	bpa_data.mem_size = backbuf_sz;
	int res;
	res = ioctl(bpafd, BPAMEMIO_ALLOCMEM, &bpa_data);
	if (res)
	{
		fprintf(stderr, "[neutrino] FB: cannot allocate from bpamem: %m\n");
		fprintf(stderr, "backbuf_sz: %d\n", backbuf_sz);
		close(bpafd);
		bpafd = -1;
		return;
	}
	close(bpafd);

	char bpa_mem_device[30];
	sprintf(bpa_mem_device, "/dev/bpamem%d", bpa_data.device_num);
	bpafd = open(bpa_mem_device, O_RDWR | O_CLOEXEC);
	if (bpafd < 0)
	{
		fprintf(stderr, "[neutrino] FB: cannot open secondary %s: %m\n", bpa_mem_device);
		return;
	}

	backbuffer = (fb_pixel_t *)mmap(0, bpa_data.mem_size, PROT_WRITE|PROT_READ, MAP_SHARED, bpafd, 0);
	if (backbuffer == MAP_FAILED)
	{
		fprintf(stderr, "[neutrino] FB: cannot map from bpamem: %m\n");
		ioctl(bpafd, BPAMEMIO_FREEMEM);
		close(bpafd);
		bpafd = -1;
		return;
	}
	startX = 0;
	startY = 0;
	endX = DEFAULT_XRES - 1;
	endY = DEFAULT_YRES - 1;
	borderColor = 0;
	borderColorOld = 0x01010101;
	resChange();
	setMixerColor(g_settings.video_mixer_color);
};

CFbAccelSTi::~CFbAccelSTi()
{
	if (backbuffer)
	{
		fprintf(stderr, LOGTAG "unmap backbuffer\n");
		munmap(backbuffer, backbuf_sz);
	}
	if (bpafd != -1)
	{
		fprintf(stderr, LOGTAG "BPAMEMIO_FREEMEM\n");
		ioctl(bpafd, BPAMEMIO_FREEMEM);
		close(bpafd);
	}
	if (lfb)
		munmap(lfb, available);
	if (fd > -1)
		close(fd);
}

void CFbAccelSTi::paintBoxRel(const int x, const int y, const int dx, const int dy, const fb_pixel_t col, int radius, int type)
{
	/* draw a filled rectangle (with additional round corners) */

	if (!getActive())
		return;

	if (dx == 0 || dy == 0) {
		//dprintf(DEBUG_NORMAL, "[CFrameBuffer] [%s - %d]: radius %d, start x %d y %d end x %d y %d\n", __FUNCTION__, __LINE__, radius, x, y, x+dx, y+dy);
		return;
	}

	checkFbArea(x, y, dx, dy, true);

	if (radius > 0) { // if radius = 0 there is no round corner --tango
	// hack: don't paint round corners unless these are actually visible --martii
	fb_pixel_t *f = lbb + y * stride/sizeof(fb_pixel_t) + x;
	if ((col == *f)
	 && (col == *(f + dx - 1))
	 && (col == *(f + (dy - 1) * stride/sizeof(fb_pixel_t)))
	 && (col == *(f + (dy - 1) * stride/sizeof(fb_pixel_t) + dx - 1)))
		type = 0;
	}

	if (!type || !radius)
	{
		paintRect(x, y, dx, dy, col);
		checkFbArea(x, y, dx, dy, false);
		return;
	}

	/* limit the radius */
	radius = limitRadius(dx, dy, radius);
	if (radius < 1)		/* dx or dy = 0... */
		radius = 1;	/* avoid div by zero below */

	int line = 0;
	while (line < dy) {
		int ofl, ofr;
		int level;
		if (calcCorners(NULL, &ofl, &ofr, dy, line, radius,
				corner_tl, corner_tr, corner_bl, corner_br, level))
		{
			int height = dy - ((corner_tl || corner_tr)?radius: 0 ) - ((corner_bl || corner_br) ? radius : 0);
			paintRect(x, y + line, dx, height, col);
			line += height;
			continue;
		}
		if (dx - ofr - ofl < 1) {
			line++;
			continue;
		}

		fb_pixel_t *p = lbb + (y + line) * stride/sizeof(fb_pixel_t) + x + ofl;
		*p = mergeColor(*p, 255 - level, col, level);

		p = lbb + (y + line) * stride/sizeof(fb_pixel_t) + x + dx - ofr - 1;
		*p = mergeColor(*p, 255 - level, col, level);

		paintLine(x + ofl, y + line, x + dx - ofr, y + line, col);
		line++;
	}
	checkFbArea(x, y, dx, dy, false);
}

void CFbAccelSTi::paintRect(const int x,const int y,const int dx,const int dy,const fb_pixel_t col)
{
	if (dx <= 0 || dy <= 0)
		return;

	// The STM blitter introduces considerable overhead probably worth for small areas.  --martii
	if (dx * dy < DEFAULT_XRES * 4) {
		waitForIdle();
		fb_pixel_t *fbs = getFrameBufferPointer() + (DEFAULT_XRES * y) + x;
		fb_pixel_t *fbe = fbs + DEFAULT_XRES * (dy - 1) + dx;
		int off = DEFAULT_XRES - dx;
		while (fbs < fbe) {
			fb_pixel_t *ex = fbs + dx;
			while (fbs < ex)
				*fbs++ = col;
			fbs += off;
		}
		return;
	}

	/* function has const parameters, so copy them here... */
	int width = dx;
	int height = dy;
	int xx = x;
	int yy = y;
	/* maybe we should just return instead of fixing this up... */
	if (x < 0) {
		fprintf(stderr, "[neutrino] fb::%s: x < 0 (%d)\n", __func__, x);
		width += x;
		if (width <= 0)
			return;
		xx = 0;
	}

	if (y < 0) {
		fprintf(stderr, "[neutrino] fb::%s: y < 0 (%d)\n", __func__, y);
		height += y;
		if (height <= 0)
			return;
		yy = 0;
	}

	int right = xx + width;
	int bottom = yy + height;

	if (right > (int)xRes) {
		if (xx >= (int)xRes) {
			fprintf(stderr, "[neutrino] fb::%s: x >= xRes (%d > %d)\n", __func__, xx, xRes);
			return;
		}
		fprintf(stderr, "[neutrino] fb::%s: x+w > xRes! (%d+%d > %d)\n", __func__, xx, width, xRes);
		right = xRes;
	}
	if (bottom > (int)yRes) {
		if (yy >= (int)yRes) {
			fprintf(stderr, "[neutrino] fb::%s: y >= yRes (%d > %d)\n", __func__, yy, yRes);
			return;
		}
		fprintf(stderr, "[neutrino] fb::%s: y+h > yRes! (%d+%d > %d)\n", __func__, yy, height, yRes);
		bottom = yRes;
	}

	STMFBIO_BLT_DATA bltData;
	memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));

	bltData.operation  = BLT_OP_FILL;
	bltData.dstOffset  = lbb_off;
	bltData.dstPitch   = stride;

	bltData.dst_left   = xx;
	bltData.dst_top    = yy;
	bltData.dst_right  = right;
	bltData.dst_bottom = bottom;

	bltData.dstFormat  = SURF_ARGB8888;
	bltData.srcFormat  = SURF_ARGB8888;
	bltData.dstMemBase = STMFBGP_FRAMEBUFFER;
	bltData.srcMemBase = STMFBGP_FRAMEBUFFER;
	bltData.colour     = col;

	OpenThreads::ScopedLock<OpenThreads::Mutex> m_lock(blit_mutex);
	if (ioctl(fd, STMFBIO_BLT, &bltData ) < 0)
		fprintf(stderr, "blitRect FBIO_BLIT: %m x:%d y:%d w:%d h:%d s:%d\n", xx,yy,width,height,stride);
}

void CFbAccelSTi::blit2FB(void *fbbuff, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff, uint32_t xp, uint32_t yp, bool transp)
{
	int x, y, dw, dh;
	x = xoff;
	y = yoff;
	dw = width - xp;
	dh = height - yp;

	size_t mem_sz = width * height * sizeof(fb_pixel_t);
	unsigned long ulFlags = 0;
	if (!transp) /* transp == false (default): use transparency from source alphachannel */
		ulFlags = BLT_OP_FLAGS_BLEND_SRC_ALPHA|BLT_OP_FLAGS_BLEND_DST_MEMORY; // we need alpha blending

	STMFBIO_BLT_EXTERN_DATA blt_data;
	memset(&blt_data, 0, sizeof(STMFBIO_BLT_EXTERN_DATA));
	blt_data.operation  = BLT_OP_COPY;
	blt_data.ulFlags    = ulFlags;
	blt_data.srcOffset  = 0;
	blt_data.srcPitch   = width * 4;
	blt_data.dstOffset  = lbb_off;
	blt_data.dstPitch   = stride;
	blt_data.src_left   = xp;
	blt_data.src_top    = yp;
	blt_data.src_right  = width;
	blt_data.src_bottom = height;
	blt_data.dst_left   = x;
	blt_data.dst_top    = y;
	blt_data.dst_right  = x + dw;
	blt_data.dst_bottom = y + dh;
	blt_data.srcFormat  = SURF_ARGB8888;
	blt_data.dstFormat  = SURF_ARGB8888;
	blt_data.srcMemBase = (char *)backbuffer;
	blt_data.dstMemBase = (char *)lfb;
	blt_data.srcMemSize = mem_sz;
	blt_data.dstMemSize = stride * yRes + lbb_off;

	OpenThreads::ScopedLock<OpenThreads::Mutex> m_lock(blit_mutex);
#if 0
	ioctl(fd, STMFBIO_SYNC_BLITTER);
#endif
	if (fbbuff != backbuffer)
		memmove(backbuffer, fbbuff, mem_sz);
	// icons are so small that they will still be in cache
	msync(backbuffer, backbuf_sz, MS_SYNC);

	if (ioctl(fd, STMFBIO_BLT_EXTERN, &blt_data) < 0)
		perror(LOGTAG "blit2FB STMFBIO_BLT_EXTERN");
	return;
}

void CFbAccelSTi::blit()
{
#ifdef ENABLE_GRAPHLCD
	nGLCD::Blit();
#endif
	msync(lbb, DEFAULT_XRES * 4 * DEFAULT_YRES, MS_SYNC);

	if (borderColor != borderColorOld || (borderColor != 0x00000000 && borderColor != 0xFF000000)) {
		borderColorOld = borderColor;
		switch(mode3D) {
		case CFrameBuffer::Mode3D_off:
		default:
			blitBoxFB(0, 0, screeninfo.xres, sY, borderColor);		// top
			blitBoxFB(0, 0, sX, screeninfo.yres, borderColor);	// left
			blitBoxFB(eX, 0, screeninfo.xres, screeninfo.yres, borderColor);	// right
			blitBoxFB(0, eY, screeninfo.xres, screeninfo.yres, borderColor);	// bottom
			break;
		case CFrameBuffer::Mode3D_SideBySide:
			blitBoxFB(0, 0, screeninfo.xres, sY, borderColor);			// top
			blitBoxFB(0, 0, sX/2, screeninfo.yres, borderColor);			// left
			blitBoxFB(eX/2 + 1, 0, screeninfo.xres/2 + sX/2, screeninfo.yres, borderColor);	// middle
			blitBoxFB(screeninfo.xres/2 + eX/2 + 1, 0, screeninfo.xres, screeninfo.yres, borderColor);	// right
			blitBoxFB(0, eY, screeninfo.xres, screeninfo.yres, borderColor);			// bottom
			break;
		case CFrameBuffer::Mode3D_TopAndBottom:
			blitBoxFB(0, 0, screeninfo.xres, sY/2, borderColor); 			// top
			blitBoxFB(0, eY/2 + 1, screeninfo.xres, screeninfo.yres/2 + sY/2, borderColor); 	// middle
			blitBoxFB(0, screeninfo.yres/2 + eY/2 + 1, screeninfo.xres, screeninfo.yres, borderColor); // bottom
			blitBoxFB(0, 0, sX, screeninfo.yres, borderColor);			// left
			blitBoxFB(eX, 0, screeninfo.xres, screeninfo.yres, borderColor);			// right
			break;
		case CFrameBuffer::Mode3D_Tile:
			blitBoxFB(0, 0, (screeninfo.xres * 2)/3, (sY * 2)/3, borderColor);		// top
			blitBoxFB(0, 0, (sX * 2)/3, (screeninfo.yres * 2)/3, borderColor);		// left
			blitBoxFB((eX * 2)/3, 0, (screeninfo.xres * 2)/3, (screeninfo.yres * 2)/3, borderColor);	// right
			blitBoxFB(0, (eY * 2)/3, (screeninfo.xres * 2)/3, (screeninfo.yres * 2)/3, borderColor);	// bottom
			break;
		}
	}
	switch(mode3D) {
	case CFrameBuffer::Mode3D_off:
	default:
		blitBB2FB(0, 0, DEFAULT_XRES, DEFAULT_YRES, sX, sY, eX, eY);
		break;
	case CFrameBuffer::Mode3D_SideBySide:
		blitBB2FB(0, 0, DEFAULT_XRES, DEFAULT_YRES, sX/2, sY, eX/2, eY);
		blitBB2FB(0, 0, DEFAULT_XRES, DEFAULT_YRES, screeninfo.xres/2 + sX/2, sY, screeninfo.xres/2 + eX/2, eY);
		break;
	case CFrameBuffer::Mode3D_TopAndBottom:
		blitBB2FB(0, 0, DEFAULT_XRES, DEFAULT_YRES, sX, sY/2, eX, eY/2);
		blitBB2FB(0, 0, DEFAULT_XRES, DEFAULT_YRES, sX, screeninfo.yres/2 + sY/2, eX, screeninfo.yres/2 + eY/2);
		break;
	case CFrameBuffer::Mode3D_Tile:
		blitBB2FB(0, 0, DEFAULT_XRES, DEFAULT_YRES, (sX * 2)/3, (sY * 2)/3, (eX * 2)/3, (eY * 2)/3);
		blitFB2FB(0, 0, screeninfo.xres/3, (screeninfo.yres * 2)/3, (screeninfo.xres * 2)/3, 0, screeninfo.xres, (screeninfo.yres * 2)/3);
		blitFB2FB(screeninfo.xres/3, 0, (screeninfo.xres * 2)/3, screeninfo.yres/3, 0, (screeninfo.yres * 2)/3, screeninfo.xres/3, screeninfo.yres);
		blitFB2FB(screeninfo.xres/3, screeninfo.yres/3, (screeninfo.xres * 2)/3, (screeninfo.yres * 2)/3, screeninfo.xres/3, (screeninfo.yres * 2)/3, (screeninfo.xres * 2)/3, screeninfo.yres);
		break;
	}
	OpenThreads::ScopedLock<OpenThreads::Mutex> m_lock(blit_mutex);
	if(ioctl(fd, STMFBIO_SYNC_BLITTER) < 0)
		perror(LOGTAG "blit2FB STMFBIO_SYNC_BLITTER");
}

/* not really used yet */
void CFbAccelSTi::mark(int, int, int, int)
{
}

void CFbAccelSTi::blitBB2FB(int fx0, int fy0, int fx1, int fy1, int tx0, int ty0, int tx1, int ty1)
{
	STMFBIO_BLT_DATA  bltData;
	memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));
	bltData.operation  = BLT_OP_COPY;
	bltData.srcOffset  = lbb_off;
	bltData.srcPitch   = stride;
	bltData.src_left   = fx0;
	bltData.src_top    = fy0;
	bltData.src_right  = fx1;
	bltData.src_bottom = fy1;
	bltData.srcFormat = SURF_BGRA8888;
	bltData.srcMemBase = STMFBGP_FRAMEBUFFER;
	bltData.dstPitch   = screeninfo.xres * 4;
	bltData.dstFormat = SURF_BGRA8888;
	bltData.dstMemBase = STMFBGP_FRAMEBUFFER;
	bltData.dst_left   = tx0;
	bltData.dst_top    = ty0;
	bltData.dst_right  = tx1;
	bltData.dst_bottom = ty1;
	blit_mutex.lock();
	if (ioctl(fd, STMFBIO_BLT, &bltData ) < 0)
		perror(LOGTAG "STMFBIO_BLT_BB");
	blit_mutex.unlock();
}

void CFbAccelSTi::blitFB2FB(int fx0, int fy0, int fx1, int fy1, int tx0, int ty0, int tx1, int ty1)
{
	STMFBIO_BLT_DATA  bltData;
	memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));
	bltData.operation  = BLT_OP_COPY;
	bltData.srcPitch   = screeninfo.xres * 4;
	bltData.src_left   = fx0;
	bltData.src_top    = fy0;
	bltData.src_right  = fx1;
	bltData.src_bottom = fy1;
	bltData.srcFormat = SURF_BGRA8888;
	bltData.srcMemBase = STMFBGP_FRAMEBUFFER;
	bltData.dstPitch   = bltData.srcPitch;
	bltData.dstFormat = SURF_BGRA8888;
	bltData.dstMemBase = STMFBGP_FRAMEBUFFER;
	bltData.dst_left   = tx0;
	bltData.dst_top    = ty0;
	bltData.dst_right  = tx1;
	bltData.dst_bottom = ty1;
	blit_mutex.lock();
	if (ioctl(fd, STMFBIO_BLT, &bltData ) < 0)
		perror(LOGTAG "STMFBIO_BLT_FB");
	blit_mutex.unlock();
}

fb_pixel_t *CFbAccelSTi::getBackBufferPointer() const
{
	return backbuffer;
}

/* original interfaceL: 1 == pixel alpha, 2 == global alpha premultiplied */
void CFbAccelSTi::setBlendMode(uint8_t mode)
{
	/* mode = 1 => reset to no extra transparency */
	if (mode == 1)
		setBlendLevel(0);
}

/* level = 100 -> transparent, level = 0 -> nontransperent */
void CFbAccelSTi::setBlendLevel(int level)
{
	struct stmfbio_var_screeninfo_ex v;
	memset(&v, 0, sizeof(v));
	/* set to 0 already...
	 v.layerid = 0;
	 v.activate = STMFBIO_ACTIVATE_IMMEDIATE; // == 0
	 v.premultiplied_alpha = 0;
	*/
	v.caps = STMFBIO_VAR_CAPS_OPACITY | STMFBIO_VAR_CAPS_PREMULTIPLIED;
	v.opacity = 0xff - (level * 0xff / 100);
	if (ioctl(fd, STMFBIO_SET_VAR_SCREENINFO_EX, &v) < 0)
		perror(LOGTAG "setBlendLevel STMFBIO");
}

fb_pixel_t CFbAccelSTi::getBorderColor(void)
{
	return borderColor; 
}

void CFbAccelSTi::getBorder(int &sx, int &sy, int &ex, int &ey)
{
	sx = startX;
	sy = startY;
	ex = endX;
	ey = endY;
}

void CFbAccelSTi::ClearFB(void)
{
	blitBoxFB(0, 0, screeninfo.xres, screeninfo.yres, 0);
}

void CFbAccelSTi::blitBoxFB(int x0, int y0, int x1, int y1, fb_pixel_t color)
{
	if (x0 > -1 && y0 > -1 && x0 < x1 && y0 < y1) {
		STMFBIO_BLT_DATA  bltData;
		memset(&bltData, 0, sizeof(STMFBIO_BLT_DATA));
		bltData.operation  = BLT_OP_FILL;
		bltData.dstPitch   = screeninfo.xres * 4;
		bltData.dstFormat  = SURF_ARGB8888;
		bltData.srcFormat  = SURF_ARGB8888;
		bltData.dstMemBase = STMFBGP_FRAMEBUFFER;
		bltData.srcMemBase = STMFBGP_FRAMEBUFFER;
		bltData.colour     = color;
		bltData.dst_left   = x0;
		bltData.dst_top    = y0;
		bltData.dst_right  = x1;
		bltData.dst_bottom = y1;
		if (ioctl(fd, STMFBIO_BLT, &bltData ) < 0)
			perror("STMFBIO_BLT");
	}
}

void CFbAccelSTi::blitBPA2FB(unsigned char *mem, SURF_FMT fmt, int w, int h, int x, int y, int pan_x, int pan_y, int fb_x, int fb_y, int fb_w, int fb_h, bool transp)
{
	if (w < 1 || h < 1)
		return;
	if (fb_x < 0)
		fb_x = x;
	if (fb_y < 0)
		fb_y = y;
	if (pan_x < 0 || pan_x > w - x)
		pan_x = w - x;
	if (pan_y < 0 || pan_y > h - y)
		pan_y = h - y;
	if (fb_w < 0)
		fb_w = pan_x;
	if (fb_h < 0)
		fb_h = pan_y;

	STMFBIO_BLT_EXTERN_DATA blt_data;
	memset(&blt_data, 0, sizeof(STMFBIO_BLT_EXTERN_DATA));
	blt_data.operation  = BLT_OP_COPY;
	if (!transp) /* transp == false (default): use transparency from source alphachannel */
		blt_data.ulFlags = BLT_OP_FLAGS_BLEND_SRC_ALPHA|BLT_OP_FLAGS_BLEND_DST_MEMORY; // we need alpha blending
//	blt_data.srcOffset  = 0;
	switch (fmt) {
	case SURF_RGB888:
	case SURF_BGR888:
		blt_data.srcPitch   = w * 3;
		break;
	default: // FIXME, this is wrong for quite a couple of formats which are currently not in use
		blt_data.srcPitch   = w * 4;
	}
	blt_data.dstOffset  = lbb_off;
	blt_data.dstPitch   = stride;
	blt_data.src_left   = x;
	blt_data.src_top    = y;
	blt_data.src_right  = x + pan_x;
	blt_data.src_bottom = y + pan_y;
	blt_data.dst_left   = fb_x;
	blt_data.dst_top    = fb_y;
	blt_data.dst_right  = fb_x + fb_w;
	blt_data.dst_bottom = fb_y + fb_h;
	blt_data.srcFormat  = fmt;
	blt_data.dstFormat  = SURF_ARGB8888;
	blt_data.srcMemBase = (char *)mem;
	blt_data.dstMemBase = (char *)lfb;
	blt_data.srcMemSize = blt_data.srcPitch * h;
	blt_data.dstMemSize = stride * DEFAULT_YRES + lbb_off;

	msync(mem, blt_data.srcPitch * h, MS_SYNC);

	if(ioctl(fd, STMFBIO_BLT_EXTERN, &blt_data) < 0)
		perror("blitBPA2FB FBIO_BLIT");
}

CFrameBuffer::Mode3D CFbAccelSTi::get3DMode()
{
	return mode3D;
}

void CFbAccelSTi::set3DMode(Mode3D m)
{
	if (mode3D != m) {
		ClearFB();
		mode3D = m;
		borderColorOld = 0x01010101;
		blit();
	}
}

void CFbAccelSTi::blitArea(int src_width, int src_height, int fb_x, int fb_y, int width, int height)
{
	if (!src_width || !src_height)
		return;
	STMFBIO_BLT_EXTERN_DATA blt_data;
	memset(&blt_data, 0, sizeof(STMFBIO_BLT_EXTERN_DATA));
	blt_data.operation  = BLT_OP_COPY;
	blt_data.ulFlags    = BLT_OP_FLAGS_BLEND_SRC_ALPHA | BLT_OP_FLAGS_BLEND_DST_MEMORY;	// we need alpha blending
//	blt_data.srcOffset  = 0;
	blt_data.srcPitch   = src_width * 4;
	blt_data.dstOffset  = lbb_off;
	blt_data.dstPitch   = stride;
//	blt_data.src_top    = 0;
//	blt_data.src_left   = 0;
	blt_data.src_right  = src_width;
	blt_data.src_bottom = src_height;
	blt_data.dst_left   = fb_x;
	blt_data.dst_top    = fb_y;
	blt_data.dst_right  = fb_x + width;
	blt_data.dst_bottom = fb_y + height;
	blt_data.srcFormat  = SURF_ARGB8888;
	blt_data.dstFormat  = SURF_ARGB8888;
	blt_data.srcMemBase = (char *)backbuffer;
	blt_data.dstMemBase = (char *)lfb;
	blt_data.srcMemSize = backbuf_sz;
	blt_data.dstMemSize = stride * DEFAULT_YRES + lbb_off;

	msync(backbuffer, blt_data.srcPitch * src_height, MS_SYNC);

	if(ioctl(fd, STMFBIO_BLT_EXTERN, &blt_data) < 0)
		perror("blitArea FBIO_BLIT");
}

void CFbAccelSTi::resChange(void)
{
	if (ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo) == -1)
		perror("frameBuffer <FBIOGET_VSCREENINFO>");

	sX = (startX * screeninfo.xres)/DEFAULT_XRES;
	sY = (startY * screeninfo.yres)/DEFAULT_YRES;
	eX = (endX * screeninfo.xres)/DEFAULT_XRES;
	eY = (endY * screeninfo.yres)/DEFAULT_YRES;
	borderColorOld = 0x01010101;
}

void CFbAccelSTi::setBorder(int sx, int sy, int ex, int ey)
{
	startX = sx;
	startY = sy;
	endX = ex;
	endY = ey;
	sX = (startX * screeninfo.xres)/DEFAULT_XRES;
	sY = (startY * screeninfo.yres)/DEFAULT_YRES;
	eX = (endX * screeninfo.xres)/DEFAULT_XRES;
	eY = (endY * screeninfo.yres)/DEFAULT_YRES;
	borderColorOld = 0x01010101;
}

void CFbAccelSTi::setBorderColor(fb_pixel_t col)
{
	if (!col && borderColor)
		blitBoxFB(0, 0, screeninfo.xres, screeninfo.yres, 0);
	borderColor = col;
}

void CFbAccelSTi::setMixerColor(uint32_t mixer_background)
{
	struct stmfbio_output_configuration outputConfig;
	memset(&outputConfig, 0, sizeof(outputConfig));
	outputConfig.outputid = STMFBIO_OUTPUTID_MAIN;
	outputConfig.activate = STMFBIO_ACTIVATE_IMMEDIATE;
	outputConfig.caps = STMFBIO_OUTPUT_CAPS_MIXER_BACKGROUND;
	outputConfig.mixer_background = mixer_background;
	if(ioctl(fd, STMFBIO_SET_OUTPUT_CONFIG, &outputConfig) < 0)
		perror("setting output configuration failed");
}

/* wrong name... */
int CFbAccelSTi::setMode(unsigned int, unsigned int, unsigned int)
{
	/* it's all fake... :-) */
	xRes = screeninfo.xres = screeninfo.xres_virtual = DEFAULT_XRES;
	yRes = screeninfo.yres = screeninfo.yres_virtual = DEFAULT_YRES;
	bpp  = screeninfo.bits_per_pixel = DEFAULT_BPP;
	stride = screeninfo.xres * screeninfo.bits_per_pixel / 8;
	return 0;
}

void CFbAccelSTi::run(void) {};
void CFbAccelSTi::_blit(void) {};

#if 0
/* this is not accelerated... */
void CFbAccelSTi::paintPixel(const int x, const int y, const fb_pixel_t col)
{
	fb_pixel_t *pos = getFrameBufferPointer();
	pos += (stride / sizeof(fb_pixel_t)) * y;
	pos += x;
	*pos = col;
}

/* unused, because horizontal and vertical line are not acceleratedn in paintRect anyway
 * and everything else is identical to fb_generic code */
void CFbAccelSTi::paintLine(int xa, int ya, int xb, int yb, const fb_pixel_t col)
{
	int dx = abs (xa - xb);
	int dy = abs (ya - yb);
	if (dy == 0) /* horizontal line */
	{
		/* paintRect actually is 1 pixel short to the right,
		 * but that's bug-compatibility with the GXA code */
		paintRect(xa, ya, xb - xa, 1, col);
		return;
	}
	if (dx == 0) /* vertical line */
	{
		paintRect(xa, ya, 1, yb - ya, col);
		return;
	}
	int x;
	int y;
	int End;
	int step;

	if (dx > dy)
	{
		int p = 2 * dy - dx;
		int twoDy = 2 * dy;
		int twoDyDx = 2 * (dy-dx);

		if (xa > xb)
		{
			x = xb;
			y = yb;
			End = xa;
			step = ya < yb ? -1 : 1;
		}
		else
		{
			x = xa;
			y = ya;
			End = xb;
			step = yb < ya ? -1 : 1;
		}

		paintPixel(x, y, col);

		while (x < End)
		{
			x++;
			if (p < 0)
				p += twoDy;
			else
			{
				y += step;
				p += twoDyDx;
			}
			paintPixel(x, y, col);
		}
	}
	else
	{
		int p = 2 * dx - dy;
		int twoDx = 2 * dx;
		int twoDxDy = 2 * (dx-dy);

		if (ya > yb)
		{
			x = xb;
			y = yb;
			End = ya;
			step = xa < xb ? -1 : 1;
		}
		else
		{
			x = xa;
			y = ya;
			End = yb;
			step = xb < xa ? -1 : 1;
		}

		paintPixel(x, y, col);

		while (y < End)
		{
			y++;
			if (p < 0)
				p += twoDx;
			else
			{
				x += step;
				p += twoDxDy;
			}
			paintPixel(x, y, col);
		}
	}
	mark(xa, ya, xb, yb);
	blit();
}
#endif
