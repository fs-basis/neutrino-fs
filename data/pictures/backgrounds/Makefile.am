installdir = $(BACKGROUNDS)

resolution = 1280x720

install_DATA = \
	$(resolution)/background.jpg \
	$(resolution)/bootlogo.mvi



install-data-hook:
	ln -sf $(BACKGROUNDS)/background.jpg $(DESTDIR)$(ICONSDIR)/mp3.jpg
	ln -sf $(BACKGROUNDS)/background.jpg $(DESTDIR)$(ICONSDIR)/radiomode.jpg
	ln -sf $(BACKGROUNDS)/background.jpg $(DESTDIR)$(ICONSDIR)/scan.jpg
	ln -sf $(BACKGROUNDS)/background.jpg $(DESTDIR)$(ICONSDIR)/shutdown.jpg
	ln -sf $(BACKGROUNDS)/background.jpg $(DESTDIR)$(ICONSDIR)/start.jpg
