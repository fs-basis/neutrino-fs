AM_CPPFLAGS = -fno-rtti -fno-exceptions -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -std=gnu++0x

BUILT_SOURCES = version.h
version.h:
	@rm -f $@; \
	echo '#define BUILT_DATE "'`date`'"' > $@
	@if test -d $(top_srcdir)/.git ; then \
		pushd $(top_srcdir) ; \
		GITBRANCH=$$(git rev-parse --abbrev-ref HEAD) ; \
		GITDESCRIBE=$$(git describe --always --tags --dirty || echo 'VCS failed') ; \
		popd ; \
		echo '#define VCS "'$${GITDESCRIBE} [$${GITBRANCH}]'"' >> $@ ; \
	fi
noinst_HEADERS = version.h
.PHONY: version.h

SUBDIRS = bedit components moviebrowser widget

if ENABLE_LUA
SUBDIRS += lua
endif

AM_CPPFLAGS += \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/zapit/include \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/libeventserver \
	-I$(top_srcdir)/lib/libnet \
	-I$(top_srcdir)/lib/libconfigfile \
	-I$(top_srcdir)/lib/connection \
	-I$(top_srcdir)/lib/xmltree \
	-I$(top_srcdir)/lib/jsoncpp \
	@SIGC_CFLAGS@ \
	@CURL_CFLAGS@ \
	@LUA_CFLAGS@ \
	@FREETYPE_CFLAGS@ \
	@AVFORMAT_CFLAGS@ \
	@LUA_CFLAGS@ \
	@HWLIB_CFLAGS@

noinst_LIBRARIES = libtimerlist.a libneutrino_gui.a libneutrino_gui2.a

libneutrino_gui_a_SOURCES = \
	adzap.cpp \
	audio_select.cpp \
	audio_setup.cpp \
	audiomute.cpp \
	audioplayer.cpp \
	audioplayer_setup.cpp\
	bookmarkmanager.cpp \
	bouquetlist.cpp \
	buildinfo.cpp \
	channellist.cpp \
	cec_setup.cpp \
	dboxinfo.cpp \
	epgplus.cpp \
	epgview.cpp \
	eventlist.cpp \
	favorites.cpp \
	filebrowser.cpp \
	followscreenings.cpp \
	imageinfo.cpp \
	info_menue.cpp \
	infoviewer.cpp \
	infoviewer_bb.cpp \
	keybind_setup.cpp \
	mediaplayer.cpp \
	mediaplayer_setup.cpp \
	miscsettings_menu.cpp \
	movieplayer.cpp \
	network_service.cpp \
	network_setup.cpp \
	nfs.cpp \
	osd_helpers.cpp \
	osd_progressbar_setup.cpp \
	osd_setup.cpp \
	osdlang_setup.cpp \
	parentallock_setup.cpp \
	personalize.cpp \
	pictureviewer.cpp \
	pictureviewer_setup.cpp \
	pluginlist.cpp \
	plugins.cpp \
	proxyserver_setup.cpp \
	rc_lock.cpp \
	record_setup.cpp \
	scan.cpp \
	scan_setup.cpp \
	screensaver.cpp \
	screensetup.cpp \
	settings_manager.cpp \
	sleeptimer.cpp \
	streaminfo.cpp \
	subchannel_select.cpp \
	themes.cpp \
	timeosd.cpp \
	tmdb.cpp \
	update.cpp \
	update_menue.cpp \
	update_settings.cpp \
	user_menue.cpp \
	user_menue_setup.cpp \
	vfd_setup.cpp \
	videosettings.cpp \
	volumebar.cpp \
	webradio_setup.cpp \
	webtv_setup.cpp \
	xmltv_setup.cpp \
	zapit_setup.cpp

if ENABLE_PIP
libneutrino_gui_a_SOURCES += \
	pipsetup.cpp
endif

if BOXTYPE_DUCKBOX
libneutrino_gui_a_SOURCES += \
	3dsetup.cpp \
	psisetup.cpp
endif

if BOXTYPE_ARMBOX
libneutrino_gui_a_SOURCES += \
	3dsetup.cpp \
	psisetup.cpp
endif

if ENABLE_LCD4LINUX
libneutrino_gui_a_SOURCES += \
	lcd4l_setup.cpp
endif

if ENABLE_GRAPHLCD
libneutrino_gui_a_SOURCES += \
	glcdsetup.cpp
endif

libneutrino_gui2_a_SOURCES = \
	color.cpp \
	hdd_menu.cpp \
	infoclock.cpp \
	kerneloptions.cpp \
	motorcontrol.cpp

libneutrino_gui2_a_SOURCES += \
	cam_menu.cpp

libtimerlist_a_SOURCES = \
	timerlist.cpp
