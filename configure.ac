define(ver_major, 4)
define(ver_minor, 0)
define(ver_micro, 0)

AC_INIT(Frankenstone-Neutrino, ver_major.ver_minor.ver_micro)
AM_INIT_AUTOMAKE([nostdinc foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

AC_DEFINE(RELEASE_CYCLE, "ver_major.0", [Release cycle information; used in update code])

TUXBOX_APPS
TUXBOX_APPS_DIRECTORY
TUXBOX_APPS_PKGCONFIG
TUXBOX_BOXTYPE

AC_PROG_CC
AC_PROG_CXX
AC_DISABLE_STATIC
AM_PROG_LIBTOOL

# Add build information to local_build_config.h.tmp
# -------------------------------------------------
rm -f local_build_config.h.tmp
cat > local_build_config.h.tmp << EOF
/* Build system under which the program was compiled on. */
#define USED_BUILD "$build"
/* Define to the used CXXFLAGS to compile this package. */
#define USED_CXXFLAGS "$CXXFLAGS"
EOF
if COMPILER=`$CC --version | head -n 1`; then
	cat >> local_build_config.h.tmp <<-EOF
	/* Define to name and version of used compiler */
	#define USED_COMPILER "$COMPILER"
	EOF
fi
# only update header if it differs to avoid unnecessary rebuilds
if ! diff local_build_config.h.tmp local_build_config.h >/dev/null 2>&1; then
	rm -f local_build_config.h
	mv local_build_config.h.tmp local_build_config.h
else
	rm -f local_build_config.h.tmp
fi

AC_ARG_ENABLE(ffmpegdec,
	AS_HELP_STRING([--enable-ffmpegdec], [enable ffmpeg decoder support @<:@default=no@:>@]),
	AC_DEFINE(ENABLE_FFMPEGDEC, 1, [include ffmpeg decoder support]))
AM_CONDITIONAL(ENABLE_FFMPEGDEC, test "$enable_ffmpegdec" = "yes")

if test "$enable_ffmpegdec" != "yes"; then

AC_ARG_WITH(tremor,
	AS_HELP_STRING([--with-tremor], [use libvorbisidec instead of libogg/libvorbis @<:@default=no@:>@]),
	[TREMOR="$withval"],
	[TREMOR=no])

AC_ARG_WITH(tremor-static,
	AS_HELP_STRING([--with-tremor-static], [use statically linked libvorbisidec instead of libogg/libvorbis @<:@default=no@:>@]),
	[TREMOR_STATIC="$withval"],
	[TREMOR_STATIC=no])

if test "$TREMOR_STATIC" = "yes"; then
	TREMOR=yes
fi

if test "$TREMOR" = "yes"; then
	TUXBOX_APPS_LIB_PKGCONFIG(VORBISIDEC,vorbisidec)
	AC_DEFINE(USE_TREMOR, 1, use libvorbisidec/tremor library)
fi

if test "$TREMOR_STATIC" = "yes"; then
	# hack to get the static lib location from the pkg-config data
	VORBISIDEC_LIBS="$(echo $VORBISIDEC_LIBS | sed 's@-L@@; s@ -l.*@/libvorbisidec.a@;')"
fi

TUXBOX_APPS_LIB_PKGCONFIG_CHECK(ID3TAG,libid3tag)
if test x"$ID3TAG_EXISTS" != xyes; then
	TUXBOX_APPS_LIB_PKGCONFIG(ID3TAG,id3tag)
fi

TUXBOX_APPS_LIB_PKGCONFIG_CHECK(MAD,libmad)
if test x"$MAD_EXISTS" != xyes; then
	TUXBOX_APPS_LIB_PKGCONFIG(MAD,mad)
fi

TUXBOX_APPS_LIB_PKGCONFIG_CHECK(OGG,ogg)
if test -z "$OGG_CFLAGS" ; then
	TUXBOX_APPS_LIB_PKGCONFIG(OGG,ogg)
fi

fi # test "$enable_ffmpegdec"

TUXBOX_APPS_LIB_PKGCONFIG(SWRESAMPLE,libswresample)

AM_CONDITIONAL(USE_TREMOR, test "$TREMOR" = "yes")

# TUXBOX_APPS_LIB_PKGCONFIG(OPENSSL,openssl)
TUXBOX_APPS_LIB_PKGCONFIG(CURL,libcurl)

## For the check you must use the corresponding libtool number as version number,
## see freetype source code docs/VERSIONS.TXT
PKG_CHECK_MODULES([FREETYPE], [freetype2 >= 16.2.10], [
	echo "freetype2 >= 2.5.0 found"
], [
	echo "freetype2 >= 2.5.0 not found, use alternative search method with freetype-config"
	#AC_MSG_ERROR([freetype2 >= 2.5.0 not found])
])

# fallback to curl-config (which is ugly for cross-compilation)
if test -z "$CURL_LIBS" -a -z "$CURL_CFLAGS"; then
	TUXBOX_APPS_LIB_CONFIG(CURL,curl-config)
fi

# fallback to freetype-config (which is ugly for cross-compilation)
if test -z "$FREETYPE_LIBS" -a -z "$FREETYPE_CFLAGS"; then
	TUXBOX_APPS_LIB_CONFIG(FREETYPE,freetype-config)
	CPPFLAGS+=" $FREETYPE_CFLAGS"
	AC_MSG_CHECKING([whether FreeType version is 2.5.0 or higher])
	AC_TRY_CPP([
		#include <ft2build.h>
		#include FT_FREETYPE_H
		#if FREETYPE_MAJOR < 2 || (FREETYPE_MAJOR == 2 && FREETYPE_MINOR < 5)
		#error Freetype version too low.
		#endif
	], [
		AC_MSG_RESULT(yes)
	], [
		AC_MSG_ERROR([Need FreeType library version 2.5.0 or higher])
	])
fi

TUXBOX_APPS_LIB_PKGCONFIG(PNG,libpng)
TUXBOX_APPS_LIB_PKGCONFIG(AVFORMAT,libavformat)
TUXBOX_APPS_LIB_PKGCONFIG(AVCODEC,libavcodec)
TUXBOX_APPS_LIB_PKGCONFIG(AVUTIL,libavutil)
TUXBOX_APPS_LIB_PKGCONFIG(SIGC,sigc++-2.0)
TUXBOX_APPS_LIB_PKGCONFIG(SWRESAMPLE,libswresample)
TUXBOX_APPS_LIB_PKGCONFIG(SWSCALE,libswscale)

if test "$BOXTYPE" = "duckbox" ||
   test "$BOXTYPE" = "armbox" ; then
	AC_DEFINE(SCREENSHOT, 1, [Define to 1 if the platform supports screenshots])
fi

AC_ARG_ENABLE(graphlcd,
	AS_HELP_STRING([--enable-graphlcd], [enable GraphLCD support]),
	,[enable_graphlcd=no])

AM_CONDITIONAL(ENABLE_GRAPHLCD, test "$enable_graphlcd" = "yes")
if test "$enable_graphlcd" = "yes"; then
	AC_DEFINE(ENABLE_GRAPHLCD, 1, [include GraphLCD support])
fi

AC_ARG_ENABLE(lcd4linux,
	AS_HELP_STRING([--enable-lcd4linux], [enable Lcd4Linux support]),
	,[enable_lcd4linux=no])

AM_CONDITIONAL(ENABLE_LCD4LINUX, test "$enable_lcd4linux" = "yes")
if test "$enable_lcd4linux" = "yes"; then
	AC_DEFINE(ENABLE_LCD4LINUX, 1, [include Lcd4Linux support])
fi

AC_ARG_ENABLE(radiotext,
	AS_HELP_STRING([--enable-radiotext], [enable RadioText support @<:@default=no@:>@]),
	AC_DEFINE(ENABLE_RADIOTEXT, 1, [enable RadioText support]))
AM_CONDITIONAL(ENABLE_RADIOTEXT, test "$enable_radiotext" = "yes")

AC_ARG_ENABLE(keyboard-no-rc,
	AS_HELP_STRING([--enable-keyboard-no-rc], [enable keyboard control, disable rc control @<:@default=no@:>@]),
	AC_DEFINE(KEYBOARD_INSTEAD_OF_REMOTE_CONTROL, 1, [enable keyboard control, disable rc control]))

AC_ARG_ENABLE(mdev,
	AS_HELP_STRING([--enable-mdev], [use mdev instead of broken neutrino mount hack @<:@default=no@:>@]),
	AC_DEFINE(ASSUME_MDEV, 1, [use mdev instead of broken neutrino mount hack]))

AC_ARG_ENABLE(giflib,
	AS_HELP_STRING([--enable-giflib], [use giflib instead of libungif @<:@default=no@:>@]),
	AC_DEFINE(ENABLE_GIFLIB, 1, [use giflib instead of libungif]))
AM_CONDITIONAL(ENABLE_GIFLIB, test "$enable_giflib" = "yes")

AC_ARG_ENABLE(pugixml,
	AS_HELP_STRING([--enable-pugixml], [use pugixml instead of xmltree @<:@default=no@:>@]),
	AC_DEFINE(USE_PUGIXML, 1, [use pugixml instead of xmltree]))
AM_CONDITIONAL(USE_PUGIXML, test "$enable_pugixml" = "yes")

AC_ARG_ENABLE(flac,
	AS_HELP_STRING([--enable-flac], [enable FLAC support @<:@default=no@:>@]),
	AC_DEFINE(ENABLE_FLAC, 1, [enable FLAC support]))
AM_CONDITIONAL(ENABLE_FLAC, test "$enable_flac" = "yes")

AC_ARG_ENABLE(webif,
	AS_HELP_STRING(--disable-webif,disable webif),
	,[enable_webif=yes])

AM_CONDITIONAL(ENABLE_WEBIF,test "$enable_webif" = "yes")
if test "$enable_webif" = "yes"; then
	AC_DEFINE(ENABLE_WEBIF,1,[include webif])
fi

AC_ARG_ENABLE(dynamicdemux,
	AS_HELP_STRING(--enable-dynamicdemux,use dynamic demux),
	,[enable_dynamicdemux=no])

AM_CONDITIONAL(DYNAMIC_DEMUX,test "$enable_dynamicdemux" = "yes")
if test "$enable_dynamicdemux" = "yes"; then
	AC_DEFINE(DYNAMIC_DEMUX,1,[use dynamic demux])
fi

AC_ARG_ENABLE(arm-acc,
	AS_HELP_STRING(--disable-arm-acc,disable arm hardware acceleration),
	,[enable_arm_acc=yes])

AM_CONDITIONAL(ENABLE_ARM_ACC,test "$enable_arm_acc" = "yes")
if test "$enable_arm_acc" = "yes"; then
	AC_DEFINE(ENABLE_ARM_ACC,1,[enable arm hardware acceleration])
fi

AC_ARG_ENABLE(lua,
	AS_HELP_STRING([--enable-lua], [enable Lua support @<:@default=yes@:>@]),
	[enable_lua="$enableval"])

if test "$enable_lua" = "yes"; then
	AC_DEFINE(ENABLE_LUA, 1, [include Lua support])
	PKG_CHECK_MODULES(LUA, lua, LLUA="yes", LLUA="no")
	PKG_CHECK_MODULES(luajit, luajit, LLUAJIT="yes", LLUAJIT="no")
		if test "x$LLUAJIT" = "xyes"; then
			LUA_LIBS="-lluajit-5.1"
			AC_DEFINE(LUA_COMPAT_5_2, 1, [needed for build with lua 5.1])
		elif test "x$LLUA" = "xyes"; then
			LUA_LIBS="-llua"
			AC_DEFINE(LUA_COMPAT_5_2, 0, [needed for build with lua 5.1])
		else
			echo "lualib not found, assuming static lua in linker path..."
			LUA_LIBS="-llua -ldl"
			AC_DEFINE(STATIC_LUAPOSIX, 1, [Define to 1 for static lua build.])
			AC_DEFINE(LUA_COMPAT_5_2, 0, [needed for build with lua 5.1])
		fi
fi
AM_CONDITIONAL(ENABLE_LUA, test "$enable_lua" = "yes")

AC_ARG_ENABLE(cleanup,
	AS_HELP_STRING([--enable-cleanup], [enable cleanup on exit @<:@default=no@:>@]),
	AC_DEFINE(EXIT_CLEANUP, 1, [enable cleanup on exit]))

AC_ARG_ENABLE(pip,
	AS_HELP_STRING([--enable-pip], [enable picture in picture support @<:@default=no@:>@]),
	AC_DEFINE(ENABLE_PIP, 1, [enable picture in picture support]))
AM_CONDITIONAL(ENABLE_PIP, test "$enable_pip" = "yes")

AC_ARG_ENABLE(fribidi,
	AS_HELP_STRING([--enable-fribidi], [enable fribidi support @<:@default=no@:>@]),
	AC_DEFINE(ENABLE_FRIBIDI, 1, [enable fribidi support]))
AM_CONDITIONAL(ENABLE_FRIBIDI, test "$enable_fribidi" = "yes")

AC_ARG_WITH(stb-hal-includes,
	AS_HELP_STRING([--with-stb-hal-includes=PATH], [path for libstb-hal includes [[NONE]]]),
	[STB_HAL_INC="$withval"],
	[STB_HAL_INC=""])

AC_ARG_WITH(stb-hal-build,
	AS_HELP_STRING([--with-stb-hal-build=PATH], [path where libstb-hal is built [[NONE]]]),
	[STB_HAL_LIB="-L$withval"],
	[STB_HAL_LIB=""])

	AC_DEFINE(USE_STB_HAL, 1, [use libstb-hal])
	if test "$STB_HAL_INC" = ""; then
		AC_MSG_ERROR([need libstb-hal includes path with --with-stb-hal-includes=...])
	fi
	HWLIB_CFLAGS="-I$STB_HAL_INC"

AM_CONDITIONAL(USE_STB_HAL,
	test "$BOXTYPE" = "duckbox"   ||
	test "$BOXTYPE" = "armbox")

# hack, so that framebuffer.h does not need to be included everywhere...
AC_DEFINE(fb_pixel_t, uint32_t, [hack, so that framebuffer.h does not need to be included everywhere...])

# hack to define a short filename also for out-of-tree build
if test `dirname $0` = `pwd` || test "$0" = ./configure; then
	HWLIB_CFLAGS="$HWLIB_CFLAGS "'-D__file__=__FILE__'
	HWLIB_CFLAGS="$HWLIB_CFLAGS "'-D__path_file__=__FILE__'
else
	HWLIB_CFLAGS="$HWLIB_CFLAGS "'-D__file__="\"$(subst $(srcdir)/,,$(abspath $<))\""'
	HWLIB_CFLAGS="$HWLIB_CFLAGS "'-D__path_file__="\"$(subst $(top_srcdir)/,,$(abspath $<))\""'
fi

AC_CHECK_PROG(AR, ar, , ar)
dnl Set default for ARFLAGS, since autoconf does not have a macro for it.
dnl This allows people to set it when running configure or make
test -n "$ARFLAGS" || ARFLAGS="cr"

AC_SUBST(AR)
AC_SUBST(ARFLAGS)
AC_SUBST(HWLIB_CFLAGS)
AC_SUBST(FREETYPE_CFLAGS)
AC_SUBST(FREETYPE_LIBS)
AC_SUBST(VORBISIDEC_CFLAGS)
AC_SUBST(VORBISIDEC_LIBS)
AC_SUBST(STB_HAL_INC)
AC_SUBST(STB_HAL_LIB)
AC_SUBST(LUA_CFLAGS)
AC_SUBST(LUA_LIBS)
AC_SUBST(BLURAY_LIBS)

AC_OUTPUT([
Makefile
data/Makefile
data/config/Makefile
data/control/Makefile
data/control-arm/Makefile
data/fonts/Makefile
data/icons/Makefile
data/icons/buttons/Makefile
data/icons/filetypes/Makefile
data/icons/headers/Makefile
data/icons/hints/Makefile
data/icons/locale/Makefile
data/icons/logo/Makefile
data/icons/movieplayer/Makefile
data/icons/radar/Makefile
data/icons/slider/Makefile
data/icons/status/Makefile
data/icons/status/ca/Makefile
data/icons/status/camd/Makefile
data/icons/status/channel/Makefile
data/icons/status/markers/Makefile
data/icons/status/various/Makefile
data/icons/various/Makefile
data/iso-codes/Makefile
data/license/Makefile
data/locale/Makefile
data/pictures/Makefile
data/pictures/backgrounds/Makefile
data/pictures/screensaver/Makefile
data/scripts/Makefile
data/webchannels/Makefile
data/themes/Makefile
data/zapit/Makefile
data/y-web/Makefile
data/y-web/images/Makefile
data/y-web/languages/Makefile
data/y-web/scripts/Makefile
data/y-web/styles/Makefile
lib/Makefile
lib/connection/Makefile
lib/jsoncpp/Makefile
lib/libconfigfile/Makefile
lib/libdvbsub/Makefile
lib/libeventserver/Makefile
lib/libmd5sum/Makefile
lib/libnet/Makefile
lib/libtuxtxt/Makefile
lib/luaclient/Makefile
lib/sectionsdclient/Makefile
lib/timerdclient/Makefile
lib/xmltree/Makefile
src/Makefile
src/daemonc/Makefile
src/driver/Makefile
src/driver/audiodec/Makefile
src/driver/pictureviewer/Makefile
src/eitd/Makefile
src/gui/Makefile
src/gui/bedit/Makefile
src/gui/components/Makefile
src/gui/lua/Makefile
src/gui/moviebrowser/Makefile
src/gui/widget/Makefile
src/nhttpd/Makefile
src/nhttpd/nhttpd.conf
src/nhttpd/tuxboxapi/Makefile
src/nhttpd/yhttpd_core/Makefile
src/nhttpd/yhttpd_mods/Makefile
src/system/Makefile
src/timerd/Makefile
src/zapit/Makefile
src/zapit/lib/Makefile
src/zapit/src/Makefile
])
