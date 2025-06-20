/*
	$port: mediaplayer_setup.cpp,v 1.5 2010/12/06 21:00:15 tuxbox-cvs Exp $

	Neutrino-GUI  -   DBoxII-Project

	media player setup implementation - Neutrino-GUI

	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

	Copyright (C) 2011 T. Graf 'dbt'
	Homepage: http://www.dbox2-tuning.net/

	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mediaplayer_setup.h"

#include <global.h>
#include <neutrino.h>
#include <mymenu.h>

#include <gui/audioplayer_setup.h>
#include <gui/filebrowser.h>
#include <gui/moviebrowser/mb.h>
#include <gui/pictureviewer_setup.h>
#include <gui/webradio_setup.h>
#include <gui/webtv_setup.h>
#include <gui/widget/icons.h>
#include <gui/widget/stringinput.h>
#include <gui/xmltv_setup.h>

#include <driver/screen_max.h>

#include <system/debug.h>

CMediaPlayerSetup::CMediaPlayerSetup()
{
	width = 40;
	selected = -1;
}

CMediaPlayerSetup::~CMediaPlayerSetup()
{
}

#define MEDIA_FILESYSTEM_IS_UTF8_OPTION_COUNT 2
const CMenuOptionChooser::keyval MEDIA_FILESYSTEM_IS_UTF8_OPTIONS[MEDIA_FILESYSTEM_IS_UTF8_OPTION_COUNT] =
{
	{ 0, LOCALE_FILESYSTEM_IS_UTF8_OPTION_ISO8859_1 },
	{ 1, LOCALE_FILESYSTEM_IS_UTF8_OPTION_UTF8      }
};

int CMediaPlayerSetup::exec(CMenuTarget* parent, const std::string & actionKey)
{
	dprintf(DEBUG_DEBUG, "init mediaplayer setup menu\n");
	int   res = menu_return::RETURN_REPAINT;

	if (parent)
		parent->hide();

	if(actionKey == "filebrowserdir")
	{
		const char *action_str = "filebrowserdir";
		chooserDir(g_settings.network_nfs_moviedir, true, action_str);
		return menu_return::RETURN_REPAINT;
	}

	res = showMediaPlayerSetup();

	return res;
}

/*shows media setup menue entries*/
int CMediaPlayerSetup::showMediaPlayerSetup()
{
	CMenuWidget* mediaSetup = new CMenuWidget(LOCALE_MAINMENU_SETTINGS, NEUTRINO_ICON_SETTINGS, width);
	mediaSetup->setSelected(selected);

	// intros
	mediaSetup->addIntroItems(LOCALE_MAINSETTINGS_MULTIMEDIA);

	CMenuForwarder *mf;

	CWebRadioSetup rsetup;
	mf = new CMenuForwarder(LOCALE_WEBRADIO_HEAD, true, NULL, &rsetup, "show_menu", CRCInput::RC_red);
	mf->setHint(NEUTRINO_ICON_HINT_RADIOMODE /* FIXME */, LOCALE_MENU_HINT_WEBRADIO_SETUP);
	mediaSetup->addItem(mf);

	CWebTVSetup wsetup;
	mf = new CMenuForwarder(LOCALE_WEBTV_HEAD, true, NULL, &wsetup, "show_menu", CRCInput::RC_green);
	mf->setHint(NEUTRINO_ICON_HINT_TVMODE /* FIXME */, LOCALE_MENU_HINT_WEBTV_SETUP);
	mediaSetup->addItem(mf);

	CXMLTVSetup xmltvsetup;
	mf = new CMenuForwarder(LOCALE_XMLTV_HEAD, true, NULL, &xmltvsetup, "show_menu", CRCInput::RC_yellow);
	mf->setHint(NEUTRINO_ICON_HINT_TVMODE /* FIXME */, LOCALE_MENU_HINT_XMLTV_SETUP);
	mediaSetup->addItem(mf);

	mediaSetup->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_AUDIO_PICTURE_HEAD));

	int shortcut = 0;
	CAudioPlayerSetup asetup;
	mf = new CMenuForwarder(LOCALE_AUDIOPLAYER_NAME, true, NULL, &asetup, "", CRCInput::convertDigitToKey(shortcut++));
	mf->setHint(NEUTRINO_ICON_HINT_APLAY, LOCALE_MENU_HINT_APLAY_SETUP);
	mediaSetup->addItem(mf);

	CPictureViewerSetup psetup;
	mf = new CMenuForwarder(LOCALE_PICTUREVIEWER_HEAD, true, NULL, &psetup, "", CRCInput::convertDigitToKey(shortcut++));
	mf->setHint(NEUTRINO_ICON_HINT_PICVIEW, LOCALE_MENU_HINT_PICTUREVIEWER_SETUP);
	mediaSetup->addItem(mf);

	mediaSetup->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MOVIEPLAYER_FILEPLAYBACK_VIDEO));

	CMenuForwarder* fileDir = new CMenuForwarder(LOCALE_FILEBROWSER_START_DIR, true, g_settings.network_nfs_moviedir, this, "filebrowserdir", CRCInput::convertDigitToKey(shortcut++));
	fileDir->setHint("", LOCALE_MENU_HINT_FILEBROWSER_STARTDIR);
	mediaSetup->addItem(fileDir);

	mediaSetup->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_FILEBROWSER_HEAD));

	//filebrowser settings
	CMenuWidget menue_fbrowser(LOCALE_MAINMENU_SETTINGS, NEUTRINO_ICON_SETTINGS, width);
	showFileBrowserSetup(&menue_fbrowser);
	mf = new CMenuForwarder(LOCALE_FILEBROWSER_HEAD, true, NULL, &menue_fbrowser, NULL, CRCInput::convertDigitToKey(shortcut++));
	mf->setHint("", LOCALE_MENU_HINT_MISC_FILEBROWSER);
	mediaSetup->addItem(mf);

	mediaSetup->addItem(new CMenuSeparator(CMenuSeparator::LINE | CMenuSeparator::STRING, LOCALE_MAINMENU_MOVIEPLAYER));

	CMovieBrowser msetup;
	mf = new CMenuForwarder(LOCALE_MOVIEBROWSER_HEAD, true, NULL, &msetup, "show_menu", CRCInput::convertDigitToKey(shortcut++));
	mf->setHint(NEUTRINO_ICON_HINT_MB, LOCALE_MENU_HINT_MOVIEBROWSER_SETUP);
	mediaSetup->addItem(mf);

	CMenuOptionChooser *mc;
	mc = new CMenuOptionChooser(LOCALE_MOVIEPLAYER_DISPLAY_PLAYTIME, &g_settings.movieplayer_display_playtime, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, g_info.hw_caps->display_xres >= 8);
	mc->setHint("", LOCALE_MENU_HINT_MOVIEPLAYER_DISPLAY_PLAYTIME);
	mediaSetup->addItem(mc);

	mediaSetup->addItem(GenericMenuSeparatorLine);

	CMenuOptionNumberChooser * ef = new CMenuOptionNumberChooser(LOCALE_MOVIEPLAYER_EOF_CNT, &g_settings.eof_cnt, true, 0, 25, NULL);
//	ef->setHint("", LOCALE_MENU_HINT_MOVIEPLAYER_EOF_CNT);
	mediaSetup->addItem(ef);

	int res = mediaSetup->exec (NULL, "");
	selected = mediaSetup->getSelected();
	delete mediaSetup;
	return res;
}

//filebrowser settings
void CMediaPlayerSetup::showFileBrowserSetup(CMenuWidget *mp_fbrowser)
{
	mp_fbrowser->addIntroItems(LOCALE_FILEBROWSER_HEAD);

	CMenuOptionChooser * mc;
	mc = new CMenuOptionChooser(LOCALE_FILESYSTEM_IS_UTF8            , &g_settings.filesystem_is_utf8            , MEDIA_FILESYSTEM_IS_UTF8_OPTIONS, MEDIA_FILESYSTEM_IS_UTF8_OPTION_COUNT, true );
	mc->setHint("", LOCALE_MENU_HINT_FILESYSTEM_IS_UTF8);
	mp_fbrowser->addItem(mc);

	mc = new CMenuOptionChooser(LOCALE_FILEBROWSER_SHOWRIGHTS        , &g_settings.filebrowser_showrights        , MESSAGEBOX_NO_YES_OPTIONS              , MESSAGEBOX_NO_YES_OPTION_COUNT              , true );
	mc->setHint("", LOCALE_MENU_HINT_FILEBROWSER_SHOWRIGHTS);
	mp_fbrowser->addItem(mc);

	mc = new CMenuOptionChooser(LOCALE_FILEBROWSER_DENYDIRECTORYLEAVE, &g_settings.filebrowser_denydirectoryleave, MESSAGEBOX_NO_YES_OPTIONS              , MESSAGEBOX_NO_YES_OPTION_COUNT              , true );
	mc->setHint("", LOCALE_MENU_HINT_FILEBROWSER_DENYDIRECTORYLEAVE);
	mp_fbrowser->addItem(mc);
#if 0
	CMenuForwarder* fileDir = new CMenuForwarder(LOCALE_FILEBROWSER_START_DIR, true, g_settings.network_nfs_moviedir, this, "filebrowserdir");
	fileDir->setHint("", LOCALE_MENU_HINT_FILEBROWSER_STARTDIR);
	mp_fbrowser->addItem(fileDir);
#endif
}
