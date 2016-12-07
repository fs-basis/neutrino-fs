SUBDIRS = \
	fonts \
	icons \
	inetradio \
	iso-codes \
	license \
	locale \
	neutrino-scripts \
	pictures \
	scripts \
	themes

if BOXTYPE_TRIPLE
SUBDIRS += lcd
endif

configdir = $(CONFIGDIR)
config_DATA = cables.xml satellites.xml encoding.conf tobackup.conf providermap.xml settingsupdate.conf terrestrial.xml

install-data-hook:
	$(INSTALL) -d $(DESTDIR)/$(CONFIGDIR)/zapit
