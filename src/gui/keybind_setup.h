/*
	$port: keybind_setup.h,v 1.2 2010/09/07 09:22:36 tuxbox-cvs Exp $

	keybindings setup implementation - Neutrino-GUI

	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

	Copyright (C) 2010 T. Graf 'dbt'
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

#ifndef __keybind_setup__
#define __keybind_setup__

#include <gui/widget/menue.h>
#include <gui/widget/icons.h>
#include <gui/widget/keychooser.h>

#include <system/setting_helpers.h>

#include <string>

class CKeybindSetup : public CMenuTarget, public CChangeObserver
{
	public:
		enum keynames
		{
			NKEY_TV_RADIO_MODE,
			NKEY_POWER_OFF,
			NKEY_PAGE_UP,
			NKEY_PAGE_DOWN,
			NKEY_VOLUME_UP,
			NKEY_VOLUME_DOWN,
			NKEY_LIST_START,
			NKEY_LIST_END,
			NKEY_SORT,
			NKEY_ADD_RECORD,
			NKEY_ADD_REMIND,
			NKEY_BOUQUET_UP,
			NKEY_BOUQUET_DOWN,
			NKEY_CURRENT_TRANSPONDER,
			NKEY_CHANNEL_UP,
			NKEY_CHANNEL_DOWN,
			NKEY_SUBCHANNEL_UP,
			NKEY_SUBCHANNEL_DOWN,
			NKEY_ZAP_HISTORY,
			NKEY_LASTCHANNEL,
			MPKEY_PLAY,
			MPKEY_PAUSE,
			MPKEY_STOP,
			MPKEY_FORWARD,
			MPKEY_REWIND,
			MPKEY_AUDIO,
			MPKEY_SUBTITLE,
			MPKEY_TIME,
			MPKEY_BOOKMARK,
			MPKEY_GOTO,
			MPKEY_NEXT3DMODE,
			MPKEY_NEXT_REPEAT_MODE,
			NKEY_TIMESHIFT,
			NKEY_UNLOCK,
			NKEY_HELP,
			NKEY_SCREENSHOT,
#if ENABLE_PIP
			NKEY_PIP_CLOSE,
			NKEY_PIP_SETUP,
			NKEY_PIP_SWAP,
#endif
			NKEY_RECORD,
			MBKEY_COPY_ONEFILE,
			MBKEY_COPY_SEVERAL,
			MBKEY_CUT,
			MBKEY_TRUNCATE,
			MBKEY_COVER,

			KEYBINDS_COUNT
		};

	private:
		CKeyChooser *keychooser[KEYBINDS_COUNT];
		int width;
		int remote_code;
		int remote_code_old;

		int showKeySetup();
		void showKeyBindSetup(CMenuWidget *bindSettings);
		void showKeyBindModeSetup(CMenuWidget *bindSettings_modes);
		void showKeyBindChannellistSetup(CMenuWidget *bindSettings_chlist);
		void showKeyBindQuickzapSetup(CMenuWidget *bindSettings_qzap);
		void showKeyBindMovieplayerSetup(CMenuWidget *bindSettings_mplayer);
		void showKeyBindMoviebrowserSetup(CMenuWidget *bindSettings_mbrowser);
		int getRemoteCode();
		bool setRemoteCode(int code);

	public:
		CKeybindSetup();
		~CKeybindSetup();
		int exec(CMenuTarget *parent, const std::string &actionKey);
		bool changeNotify(const neutrino_locale_t OptionName, void *data);
		static const char *getMoviePlayerButtonName(const neutrino_msg_t key, bool &active, bool return_title = false);
};

#endif

