AM_CXXFLAGS = -fno-rtti -fno-exceptions -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

SUBDIRS = pictureviewer audiodec

AM_CPPFLAGS = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/zapit/include \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/libnet \
	-I$(top_srcdir)/lib/libconfigfile \
	-I$(top_srcdir)/lib/connection \
	-I$(top_srcdir)/lib/libeventserver \
	@SIGC_CFLAGS@ \
	@FREETYPE_CFLAGS@ \
	@AVFORMAT_CFLAGS@ \
	@LUA_CFLAGS@ \
	@HWLIB_CFLAGS@

noinst_LIBRARIES = libneutrino_driver.a libneutrino_driver_netfile.a

libneutrino_driver_a_SOURCES = \
	abstime.c \
	audiofile.cpp \
	audiometadata.cpp \
	audioplay.cpp \
	colorgradient.cpp \
	fade.cpp \
	fb_window.cpp \
	file.cpp \
	fontrenderer.cpp \
	genpsi.cpp \
	moviecut.cpp \
	movieinfo.cpp \
	neutrinofonts.cpp
if ENABLE_RADIOTEXT
libneutrino_driver_a_SOURCES += \
	radiotext.cpp \
	radiotools.cpp
endif
libneutrino_driver_a_SOURCES += \
	rcinput.cpp \
	record.cpp \
	scanepg.cpp \
	screen_max.cpp \
	screenshot.cpp \
	shutdown_count.cpp \
	streamts.cpp \
	volume.cpp

if ENABLE_LCD4LINUX
libneutrino_driver_a_SOURCES += \
	lcd4l.cpp
endif

if ENABLE_GRAPHLCD
libneutrino_driver_a_SOURCES += \
	nglcd.cpp
endif

if BOXTYPE_DUCKBOX
libneutrino_driver_a_SOURCES += \
	fb_generic_sti_ddt.cpp \
	fb_accel_sti_ddt.cpp \
	vfd.cpp
endif

if BOXTYPE_ARMBOX
libneutrino_driver_a_SOURCES += \
	fb_generic.cpp \
	fb_accel.cpp \
	fb_accel_arm.cpp \
	simple_display.cpp
endif

libneutrino_driver_netfile_a_SOURCES = netfile.cpp
