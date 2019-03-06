SUBDIRS = \
	fonts \
	icons \
	inetradio \
	iso-codes \
	license \
	locale \
	pictures \
	scripts \
	themes

if BOXTYPE_ARMBOX
SUBDIRS += neutrino-scripts-arm
else
SUBDIRS += neutrino-scripts
endif

if ENABLE_WEBIF
SUBDIRS += y-web
endif

if BOXTYPE_TRIPLE
SUBDIRS += lcd
endif

configdir = $(CONFIGDIR)
config_DATA = cables.xml satellites.xml encoding.conf tobackup.conf providermap.xml settingsupdate.conf terrestrial.xml

config_DATA += epglanguages.conf webtv_usr.xml webradio_usr.xml
install-data-hook:
	$(INSTALL) -d $(DESTDIR)/$(CONFIGDIR)/zapit
