SUBDIRS = images scripts languages styles

installdir = $(PRIVATE_HTTPDDIR)

install_DATA =	favicon.ico \
		index.html \
		prototype.js \
		robots.txt \
		lcd4linux.html \
		Y_About.yhtm \
		Y_Baselib.js \
		Y_blank.htm \
		Y_Blocks.txt \
		Y_Boxcontrol_Bouquet_Editor_Edit.yhtm \
		Y_Boxcontrol_Bouquet_Editor_Main.yhtm \
		Y_Boxcontrol_Bouquetlist.yhtm \
		Y_Boxcontrol_Bouquets.yhtm \
		Y_Boxcontrol_Bouquets_popup.yhtm \
		Y_Boxcontrol_Channellist.yhtm \
		Y_Boxcontrol_Menue.yhtm \
		Y_Boxcontrol_Messages.yhtm \
		Y_Dist.css \
		Y_Dyn_Pages.yhtm \
		Y_EPG.js \
		Y_EPG_Plus.yhtm \
		Y_Info_Help.yhtm \
		Y_Info_Menue.yhtm \
		Y_Live_EPG_Info.yhtm \
		Y_Live_EPG.yhtm \
		Y_Main.css \
		Y_Menue.yhtm \
		Y_neutrino_Blocks.txt \
		Y_Refresh.yhtm \
		Y_Settings_buttons.yhtm \
		Y_Settings_Menue.yhtm \
		Y_Settings_mount_liste.yhtm \
		Y_Settings_mount.yhtm \
		Y_Settings_neutrino_forms.yhtm \
		Y_Settings_nhttpd.yhtm \
		Y_Settings_umount_liste.yhtm \
		Y_Settings_wol.yhtm \
		Y_Settings_xmltv.yhtm \
		Y_Settings_yWeb.yhtm \
		Y_Settings_zapit.yhtm \
		Y_StreamInfo.yhtm \
		Y_Timer_Edit.yhtm \
		Y_Timer_List.yhtm \
		Y_Tools_Boxcontrol.yhtm \
		Y_Tools_Check_Install.yhtm \
		Y_Tools_Glcd_Screenshot.yhtm \
		Y_Tools_Info_Menue.yhtm \
		Y_Tools_m3uConvert.yhtm \
		Y_Tools_Menue.yhtm \
		Y_Tools_Rcsim.yhtm \
		Y_Tools_Screenshot.yhtm  \
		Y_Tools_tvshot.yhtm \
		Y_User.css \
		Y_Version.txt \
		ywidget.css \
		ywidget.js \
		Y_yweb.js

# file manager
install_DATA +=	accordion.js \
		effects.js \
		Y_Filemgr.yhtm \
		Y_Filemgr_Edit.yhtm \
		Y_Filemgr_blocks.txt

install-data-hook:
	@# Badass hack, I know. If anyone knows a better way -- please send patch!
	find $(DESTDIR)$(PRIVATE_HTTPDDIR)/ -type f -print0 | xargs --no-run-if-empty -0 \
		sed -i \
			-e 's|%(CONFIGDIR)|$(CONFIGDIR)|g' \
			-e 's|%(ZAPITDIR)|$(ZAPITDIR)|g' \
			-e 's|%(PLUGINDIR)|$(PLUGINDIR)|g' \
			-e 's|%(PRIVATE_HTTPDDIR)|$(PRIVATE_HTTPDDIR)|g' \
	;
