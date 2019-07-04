SUBDIRS = \
	config \
	fonts \
	icons \
	inetradio \
	iso-codes \
	license \
	locale \
	pictures \
	scripts \
	themes \
	zapit

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
