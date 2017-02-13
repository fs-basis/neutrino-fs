/*
	Copyright (C) 2007-2013 Stefan Seyfried

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

	private functions for the fbaccel class (only used in CFrameBuffer)
*/


#ifndef __fbaccel__
#define __fbaccel__
#include <config.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Thread>
#include <OpenThreads/Condition>
#include "fb_generic.h"

#if HAVE_SPARK_HARDWARE || HAVE_DUCKBOX_HARDWARE
#define PARTIAL_BLIT 1
#endif

class CFbAccel
	: public CFrameBuffer
{
	public:
		CFbAccel();
		~CFbAccel();
		void paintBoxRel(const int x, const int y, const int dx, const int dy, const fb_pixel_t col, int radius, int type);
		virtual void paintRect(const int x, const int y, const int dx, const int dy, const fb_pixel_t col);
};

class CFbAccelSTi
	: public OpenThreads::Thread, public CFbAccel
{
	private:
		void run(void);
		void blit(void);
		void _blit(void);
		bool blit_thread;
		bool blit_pending;
		OpenThreads::Condition blit_cond;
		OpenThreads::Mutex blit_mutex;
		fb_pixel_t *backbuffer;
#ifdef PARTIAL_BLIT
		struct {
			int xs;
			int ys;
			int xe;
			int ye;
		} to_blit;
		uint32_t last_xres;
#endif
	public:
		CFbAccelSTi();
		~CFbAccelSTi();
		void init(const char * const);
		int setMode(unsigned int xRes, unsigned int yRes, unsigned int bpp);
		void paintRect(const int x, const int y, const int dx, const int dy, const fb_pixel_t col);
		void blit2FB(void *fbbuff, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff, uint32_t xp, uint32_t yp, bool transp);
		void waitForIdle(const char *func = NULL);
		void mark(int x, int y, int dx, int dy);
		fb_pixel_t * getBackBufferPointer() const;
		void setBlendMode(uint8_t);
		void setBlendLevel(int);
		void resChange(void);
		void setBorder(int sx, int sy, int ex, int ey);
		void setBorderColor(fb_pixel_t col);
		void ClearFB(void);
		void getBorder(int &sx, int &sy, int &ex, int &ey);
		fb_pixel_t getBorderColor(void);
		void blitArea(int src_width, int src_height, int fb_x, int fb_y, int width, int height);
		void set3DMode(Mode3D);
		Mode3D get3DMode(void);
#if HAVE_SPARK_HARDWARE || HAVE_DUCKBOX_HARDWARE
		void paintBoxRel(const int x, const int y, const int dx, const int dy, const fb_pixel_t col, int radius, int type);
		void blitBB2FB(int fx0, int fy0, int fx1, int fy1, int tx0, int ty0, int tx1, int ty2);
		void blitFB2FB(int fx0, int fy0, int fx1, int fy1, int tx0, int ty0, int tx1, int ty2);
		void blitBPA2FB(unsigned char *mem, SURF_FMT fmt, int w, int h, int x = 0, int y = 0, int pan_x = -1, int pan_y = -1, int fb_x = -1, int fb_y = -1, int fb_w = -1, int fb_h = -1, bool transp = false);
		void blitBoxFB(int x0, int y0, int x1, int y1, fb_pixel_t color);
		void setMixerColor(uint32_t mixer_background);
#endif
};

class CFbAccelCSHD1
	: public CFbAccel
{
	private:
		fb_pixel_t lastcol;
		int		  devmem_fd;	/* to access the GXA register we use /dev/mem */
		unsigned int	  smem_start;	/* as aquired from the fbdev, the framebuffers physical start address */
		volatile uint8_t *gxa_base;	/* base address for the GXA's register access */
		void setColor(fb_pixel_t col);
		void run(void);
		fb_pixel_t *backbuffer;
	public:
		CFbAccelCSHD1();
		~CFbAccelCSHD1();
		void init(const char * const);
		int setMode(unsigned int xRes, unsigned int yRes, unsigned int bpp);
		void paintPixel(int x, int y, const fb_pixel_t col);
		void paintRect(const int x, const int y, const int dx, const int dy, const fb_pixel_t col);
		void paintLine(int xa, int ya, int xb, int yb, const fb_pixel_t col);
		inline void paintHLineRel(int x, int dx, int y, const fb_pixel_t col) { paintLine(x, y, x+dx, y, col); };
		inline void paintVLineRel(int x, int y, int dy, const fb_pixel_t col) { paintLine(x, y, x, y+dy, col); };
		void paintBoxRel(const int x, const int y, const int dx, const int dy, const fb_pixel_t col, int radius = 0, int type = CORNER_ALL);
		void blit2FB(void *fbbuff, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff, uint32_t xp = 0, uint32_t yp = 0, bool transp = false);
		void blitBox2FB(const fb_pixel_t* boxBuf, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff);
		void waitForIdle(const char *func = NULL);
		fb_pixel_t * getBackBufferPointer() const;
		void setBlendMode(uint8_t);
		void setBlendLevel(int);
		void add_gxa_sync_marker(void);
		void setupGXA(void);
};

class CFbAccelCSHD2
	: public CFbAccel
{
	private:
		fb_pixel_t *backbuffer;

	public:
		CFbAccelCSHD2();
//		~CFbAccelCSHD2();
		int setMode(unsigned int xRes, unsigned int yRes, unsigned int bpp);
		void paintHLineRel(int x, int dx, int y, const fb_pixel_t col);
		void paintVLineRel(int x, int y, int dy, const fb_pixel_t col);
		void paintBoxRel(const int x, const int y, const int dx, const int dy, const fb_pixel_t col, int radius = 0, int type = CORNER_ALL);
		void blit2FB(void *fbbuff, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff, uint32_t xp = 0, uint32_t yp = 0, bool transp = false);
		void blitBox2FB(const fb_pixel_t* boxBuf, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff);
		void setBlendMode(uint8_t);
		void setBlendLevel(int);
};

class CFbAccelGLFB
	: public OpenThreads::Thread, public CFbAccel
{
	private:
		void run(void);
		void blit(void);
		void _blit(void);
		bool blit_thread;
		bool blit_pending;
		OpenThreads::Condition blit_cond;
		OpenThreads::Mutex blit_mutex;
		fb_pixel_t *backbuffer;
	public:
		CFbAccelGLFB();
		~CFbAccelGLFB();
		void init(const char * const);
		int setMode(unsigned int xRes, unsigned int yRes, unsigned int bpp);
		void blit2FB(void *fbbuff, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff, uint32_t xp, uint32_t yp, bool transp);
		fb_pixel_t * getBackBufferPointer() const;
};

class CFbAccelTD
	: public CFbAccel
{
	private:
		fb_pixel_t lastcol;
		void setColor(fb_pixel_t col);
		fb_pixel_t *backbuffer;
	public:
		CFbAccelTD();
		~CFbAccelTD();
		void init(const char * const);
		int setMode(unsigned int xRes, unsigned int yRes, unsigned int bpp);
		void paintPixel(int x, int y, const fb_pixel_t col);
		void paintRect(const int x, const int y, const int dx, const int dy, const fb_pixel_t col);
		void paintHLineRel(int x, int dx, int y, const fb_pixel_t col) { paintLine(x, y, x + dx, y, col); };
		void paintVLineRel(int x, int y, int dy, const fb_pixel_t col) { paintLine(x, y, x, y + dy, col); };
		void paintLine(int xa, int ya, int xb, int yb, const fb_pixel_t col);
		void blit2FB(void *fbbuff, uint32_t width, uint32_t height, uint32_t xoff, uint32_t yoff, uint32_t xp, uint32_t yp, bool transp);
		void waitForIdle(const char *func = NULL);
		void setBlendMode(uint8_t);
		void setBlendLevel(int);
};

#endif
