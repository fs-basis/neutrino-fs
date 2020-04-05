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
SUBDIRS += control-arm
else
SUBDIRS += control
endif

if ENABLE_WEBIF
SUBDIRS += y-web
endif

if BOXTYPE_TRIPLE
SUBDIRS += lcd
endif
