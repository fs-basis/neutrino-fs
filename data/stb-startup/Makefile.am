installdir = $(PLUGINDIR_VAR)

install_DATA = \
	stb-startup.cfg \
	stb-startup.lua \
	stb-startup_hint.png

install-exec-local:
	install -d $(DESTDIR)/$(PLUGINDIR_VAR)
	install -m 0644 $(srcdir)/stb-startup.cfg $(DESTDIR)/$(PLUGINDIR_VAR)
	install -m 0644 $(srcdir)/stb-startup.lua $(DESTDIR)/$(PLUGINDIR_VAR)
	install -m 0644 $(srcdir)/stb-startup_hint.png $(DESTDIR)/$(PLUGINDIR_VAR)
