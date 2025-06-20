/*
  Neutrino-GUI  -   DBoxII-Project

  Copyright (C) 2001 Steffen Hehn 'McClean'
  Homepage: http://dbox.cyberphoria.org/

  Copyright (C) 2007-2012 Stefan Seyfried

  Kommentar:

  Diese GUI wurde von Grund auf neu programmiert und sollte nun vom
  Aufbau und auch den Ausbaumoeglichkeiten gut aussehen. Neutrino basiert
  auf der Client-Server Idee, diese GUI ist also von der direkten DBox-
  Steuerung getrennt. Diese wird dann von Daemons uebernommen.


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

#ifndef __settings__
#define __settings__

#include "config.h"
#include <system/localize.h>
#include <configfile.h>
#include <zapit/client/zapitclient.h>
#include <zapit/client/zapittools.h>
#include <eitd/edvbstring.h> // UTF8

#include <hardware_caps.h>

#include <string>
#include <list>

struct SNeutrinoTheme
{
	unsigned char menu_Head_alpha;
	unsigned char menu_Head_red;
	unsigned char menu_Head_green;
	unsigned char menu_Head_blue;

	unsigned char menu_Head_Text_alpha;
	unsigned char menu_Head_Text_red;
	unsigned char menu_Head_Text_green;
	unsigned char menu_Head_Text_blue;

	int menu_Head_gradient;
	int menu_Head_gradient_direction;

	int menu_SubHead_gradient;
	int menu_SubHead_gradient_direction;

	int menu_Separator_gradient_enable;

	unsigned char menu_Content_alpha;
	unsigned char menu_Content_red;
	unsigned char menu_Content_green;
	unsigned char menu_Content_blue;

	unsigned char menu_Content_Text_alpha;
	unsigned char menu_Content_Text_red;
	unsigned char menu_Content_Text_green;
	unsigned char menu_Content_Text_blue;

	unsigned char menu_Content_Selected_alpha;
	unsigned char menu_Content_Selected_red;
	unsigned char menu_Content_Selected_green;
	unsigned char menu_Content_Selected_blue;

	unsigned char menu_Content_Selected_Text_alpha;
	unsigned char menu_Content_Selected_Text_red;
	unsigned char menu_Content_Selected_Text_green;
	unsigned char menu_Content_Selected_Text_blue;

	unsigned char menu_Content_inactive_alpha;
	unsigned char menu_Content_inactive_red;
	unsigned char menu_Content_inactive_green;
	unsigned char menu_Content_inactive_blue;

	unsigned char menu_Content_inactive_Text_alpha;
	unsigned char menu_Content_inactive_Text_red;
	unsigned char menu_Content_inactive_Text_green;
	unsigned char menu_Content_inactive_Text_blue;

	unsigned char menu_Foot_alpha;
	unsigned char menu_Foot_red;
	unsigned char menu_Foot_green;
	unsigned char menu_Foot_blue;

	unsigned char menu_Foot_Text_alpha;
	unsigned char menu_Foot_Text_red;
	unsigned char menu_Foot_Text_green;
	unsigned char menu_Foot_Text_blue;

	int menu_Hint_gradient;
	int menu_Hint_gradient_direction;
	int menu_ButtonBar_gradient;
	int menu_ButtonBar_gradient_direction;

	unsigned char infobar_alpha;
	unsigned char infobar_red;
	unsigned char infobar_green;
	unsigned char infobar_blue;

	unsigned char infobar_casystem_alpha;
	unsigned char infobar_casystem_red;
	unsigned char infobar_casystem_green;
	unsigned char infobar_casystem_blue;

	unsigned char infobar_Text_alpha;
	unsigned char infobar_Text_red;
	unsigned char infobar_Text_green;
	unsigned char infobar_Text_blue;

	int infobar_gradient_top;
	int infobar_gradient_top_direction;
	int infobar_gradient_body;
	int infobar_gradient_body_direction;
	int infobar_gradient_bottom;
	int infobar_gradient_bottom_direction;

	unsigned char colored_events_alpha;
	unsigned char colored_events_red;
	unsigned char colored_events_green;
	unsigned char colored_events_blue;

	int colored_events_channellist;
	int colored_events_infobar;

	unsigned char channellist_Description_Text_alpha;
	unsigned char channellist_Description_Text_red;
	unsigned char channellist_Description_Text_green;
	unsigned char channellist_Description_Text_blue;

	unsigned char clock_Digit_alpha;
	unsigned char clock_Digit_red;
	unsigned char clock_Digit_green;
	unsigned char clock_Digit_blue;

	int progressbar_design;
	int progressbar_design_channellist;
	int progressbar_gradient;
	int progressbar_timescale_red;
	int progressbar_timescale_green;
	int progressbar_timescale_yellow;
	int progressbar_timescale_invert;

	unsigned char shadow_alpha;
	unsigned char shadow_red;
	unsigned char shadow_green;
	unsigned char shadow_blue;

	//NI
	unsigned char progressbar_active_red;
	unsigned char progressbar_active_green;
	unsigned char progressbar_active_blue;
	unsigned char progressbar_passive_red;
	unsigned char progressbar_passive_green;
	unsigned char progressbar_passive_blue;

	int rounded_corners;
};

struct timer_remotebox_item
{
	unsigned int port;
	std::string user;
	std::string pass;
	std::string rbname;
	std::string rbaddress;
	bool online;
};

struct SNeutrinoSettings
{

	//video
	int video_Format;
	int video_Mode;
	int analog_mode1;
	int video_43mode;
#if HAVE_SH4_HARDWARE
	int hdmi_mode;
	uint32_t video_mixer_color;
#endif
	int psi_contrast;
	int psi_saturation;
	int psi_brightness;
	int psi_tint;
	int psi_step;

	char current_volume;
	int current_volume_step;
	int start_volume;
	int channel_mode;
	int channel_mode_radio;
	int channel_mode_initial;
	int channel_mode_initial_radio;

	//misc
	int shutdown_real;
	int shutdown_real_rcdelay;
	int shutdown_count;
	int shutdown_min;
	int sleeptimer_min;
	int record_safety_time_before;
	int record_safety_time_after;
	int zapto_pre_time;
#if HAVE_ARM_HARDWARE
	int zappingmode;
	int hdmi_colorimetry;
#endif
	int infobar_subchan_disp_pos;
	int infobar_buttons_usertitle;
	int infobar_analogclock;
#if BOXMODEL_UFS922
	int fan_speed;
#endif
	int infobar_show;
	int infobar_show_channellogo;
	int infobar_show_numbers;
	int infobar_casystem_display;
	int infobar_casystem_frame;
	int volume_pos;
	int volume_digits;
	int volume_size;
	int show_mute_icon;
	int menu_pos;
	int show_menu_hints;
	int infobar_show_sysfs_hdd;
	int infobar_show_res;
	int infobar_show_tuner;
	int infobar_show_dd_available;
	//audio
#if !BOXMODEL_E4HDULTRA && !BOXMODEL_BRE2ZE4K
	int audio_AnalogMode;
#endif
	int audio_DolbyDigital;
#if HAVE_SH4_HARDWARE
	int audio_mixer_volume_analog;
	int audio_mixer_volume_spdif;
	int audio_mixer_volume_hdmi;
#endif
	int auto_lang;
	int auto_subs;
	int srs_enable;
	int srs_algo;
	int srs_ref_volume;
	int srs_nmgr_enable;
#if HAVE_ARM_HARDWARE
	int ac3_pass;
	int dts_pass;
#else
	int hdmi_dd;
	int spdif_dd;
#endif // HAVE_ARM_HARDWARE
#if !BOXMODEL_E4HDULTRA && !BOXMODEL_BRE2ZE4K
	int analog_out;
#endif
	int audio_volume_percent_ac3;
	int audio_volume_percent_pcm;

	//video
	int hdmi_cec_mode;
	int hdmi_cec_view_on;
	int hdmi_cec_standby;
	int hdmi_cec_volume;
#if HAVE_SH4_HARDWARE
	int hdmi_cec_broadcast;
#endif
	int make_hd_list;
	int make_webtv_list;
	int make_webradio_list;
	int make_new_list;
	int make_removed_list;
	int keep_channel_numbers;
	int show_empty_favorites;
	int avsync;
	int ci_standby_reset;
	int ci_clock[4];
	int ci_op[4];
	int ci_ignore_messages[4];
	int ci_save_pincode[4];
	std::string ci_pincode[4];
	int ci_check_live;
	int ci_tuner;
#if ENABLE_RADIOTEXT
	int radiotext_enable;
#endif
	//screen saver
	int screensaver_delay;
	std::string screensaver_dir;
	int screensaver_timeout;
	int screensaver_random;
	int screensaver_mode;

	//language
	std::string language;
	std::string timezone;

	std::string pref_lang[3];
	std::string pref_subs[3];
	std::string subs_charset;

	// EPG
	int epg_save;
	int epg_save_standby;
	int epg_save_frequently;
	int epg_read;
	int epg_read_frequently;
	int epg_cache;
	int epg_old_events;
	int epg_max_events;
	int epg_extendedcache;
	std::string epg_dir;
	int epg_scan;
	int epg_scan_mode;
	int epg_scan_rescan;
	int epg_save_mode;
	int enable_sdt;

	int epg_search_history_size;
	int epg_search_history_max;
	std::list<std::string> epg_search_history;

	//network
	std::string network_ntpserver;
	std::string network_ntprefresh;
	int network_ntpenable;
	std::string ifname;

	std::list<std::string> webtv_xml;
	std::list<std::string> webradio_xml;
	std::list<std::string> xmltv_xml; // see http://wiki.xmltv.org/
	std::list<std::string> xmltv_xml_auto;

	//personalize
	enum PERSONALIZE_SETTINGS  //settings.h
	{
		P_MAIN_PINSTATUS,

		//user menu
		P_MAIN_BLUE_BUTTON,
		P_MAIN_YELLOW_BUTTON,
		P_MAIN_GREEN_BUTTON,
		P_MAIN_RED_BUTTON,

		//main menu
		P_MAIN_TV_MODE,
		P_MAIN_TV_RADIO_MODE, //togglemode
		P_MAIN_RADIO_MODE,
		P_MAIN_TIMER,
		P_MAIN_MEDIA,

		P_MAIN_GAMES,
		P_MAIN_TOOLS,
		P_MAIN_SCRIPTS,
		P_MAIN_LUA,
		P_MAIN_SETTINGS,
		P_MAIN_SERVICE,
		P_MAIN_SLEEPTIMER,
		P_MAIN_STANDBY,
		P_MAIN_REBOOT,
		P_MAIN_SHUTDOWN,
		P_MAIN_INFOMENU,
		P_MAIN_CISETTINGS,

		//settings menu
		P_MSET_SETTINGS_MANAGER,
		P_MSET_VIDEO,
		P_MSET_AUDIO,
		P_MSET_PARENTALLOCK,
		P_MSET_NETWORK,
		P_MSET_RECORDING,
		P_MSET_OSDLANG,
		P_MSET_OSD,
		P_MSET_VFD,
		P_MSET_DRIVES,
		P_MSET_CISETTINGS,
		P_MSET_KEYBINDING,
		P_MSET_MEDIAPLAYER,
		P_MSET_MISC,

		//service menu
		P_MSER_TUNER,
		P_MSER_SCANTS,
		P_MSER_RELOAD_CHANNELS,
		P_MSER_BOUQUET_EDIT,
		P_MSER_RESET_CHANNELS,
		P_MSER_RESTART,
		P_MSER_RESTART_TUNER,
		P_MSER_RELOAD_PLUGINS,
		P_MSER_SERVICE_INFOMENU,
		P_MSER_SOFTUPDATE,

		//media menu
		P_MEDIA_MENU,
		P_MEDIA_AUDIO,
		P_MEDIA_INETPLAY,
		P_MEDIA_PVIEWER,

		//movieplayer menu
		P_MPLAYER_MBROWSER,
		P_MPLAYER_FILEPLAY_VIDEO,
		P_MPLAYER_FILEPLAY_AUDIO,
		P_MPLAYER_GUI_MOUNT,

		//feature keys
		P_FEAT_KEY_FAVORIT,
		P_FEAT_KEY_TIMERLIST,
		P_FEAT_KEY_VTXT,
		P_FEAT_KEY_RC_LOCK,

		//user menu
		P_UMENU_SHOW_CANCEL,

		//plugins types
		P_UMENU_PLUGIN_TYPE_GAMES,
		P_UMENU_PLUGIN_TYPE_TOOLS,
		P_UMENU_PLUGIN_TYPE_SCRIPTS,
		P_UMENU_PLUGIN_TYPE_LUA,

		P_SETTINGS_MAX
	};

	int  personalize[P_SETTINGS_MAX];
	std::string personalize_pincode;

	//timing
	enum TIMING_SETTINGS
	{
		TIMING_MENU		= 0,
		TIMING_CHANLIST		= 1,
		TIMING_EPG		= 2,
		TIMING_INFOBAR		= 3,
		TIMING_INFOBAR_RADIO	= 4,
		TIMING_INFOBAR_MOVIE	= 5,
		TIMING_VOLUMEBAR	= 6,
		TIMING_FILEBROWSER	= 7,
		TIMING_NUMERICZAP	= 8,
		TIMING_POPUP_MESSAGES	= 9,
		TIMING_STATIC_MESSAGES	= 10,

		TIMING_SETTING_COUNT
	};

	int timing [TIMING_SETTING_COUNT];

	//widget settings
	int widget_fade;

	//theme/color options
	SNeutrinoTheme theme;

	//network
#define NETWORK_NFS_NR_OF_ENTRIES 8
	struct
	{
		std::string ip;
		std::string mac;
		std::string local_dir;
		std::string dir;
		int  automount;
		std::string mount_options1;
		std::string mount_options2;
		int  type;
		std::string username;
		std::string password;
	} network_nfs[NETWORK_NFS_NR_OF_ENTRIES];
	std::string network_nfs_audioplayerdir;
	std::string network_nfs_picturedir;
	std::string network_nfs_moviedir;
	std::string network_nfs_recordingdir;
	std::string timeshiftdir;
	std::string downloadcache_dir;
	std::string last_webtv_dir;
	std::string last_webradio_dir;

	//recording
	int  recording_stopsectionsd;
	unsigned char recording_audio_pids_default;
	int recording_audio_pids_std;
	int recording_audio_pids_alt;
	int recording_audio_pids_ac3;
	int recording_stream_vtxt_pid;
	int recording_stream_subtitle_pids;
	int recording_stream_pmt_pid;
	int record_dirsize;
	int recording_bufsize;
	int recording_bufsize_dmx;
	int recording_epg_for_filename;
	int recording_epg_for_end;
	int recording_save_in_channeldir;
	int recording_zap_on_announce;
	int recording_slow_warning;
	int recording_startstop_msg;
	int shutdown_timer_record_type;
	std::vector<timer_remotebox_item> timer_remotebox_ip;
	int timer_followscreenings;
	std::string recording_filename_template;
	int recording_already_found_check;

	// timeshift
	int timeshift_pause;
	int timeshift_auto;
	int timeshift_temp;
	int timeshift_delete;
	int timeshift_hours;

	int filesystem_is_utf8;

	int default_logo;
	std::string logo_hdd_dir;

	int load_m3u_logos;
	std::string m3u_logo_hdd_dir;

	std::string plugin_hdd_dir;

	std::string plugins_disabled;
	std::string plugins_game;
	std::string plugins_tool;
	std::string plugins_script;
	std::string plugins_lua;

	//key configuration
	int key_tvradio_mode;

	int key_pageup;
	int key_pagedown;

	int key_channelList_sort;
	int key_channelList_addrecord;
	int key_channelList_addremind;

	int key_playbutton;
	int key_quickzap_up;
	int key_quickzap_down;
	int key_bouquet_up;
	int key_bouquet_down;
	int key_subchannel_up;
	int key_subchannel_down;
	int key_zaphistory;
	int key_lastchannel;
	int key_list_start;
	int key_list_end;
	int key_power_off;
	int menu_left_exit;
	int audio_run_player;
	int record_hours;
	int key_record;
	int key_help;
	int key_volumeup;
	int key_volumedown;

	int mbkey_copy_onefile;
	int mbkey_copy_several;
	int mbkey_cut;
	int mbkey_truncate;
	int mbkey_cover;

	int mpkey_rewind;
	int mpkey_forward;
	int mpkey_pause;
	int mpkey_stop;
	int mpkey_play;
	int mpkey_audio;
	int mpkey_time;
	int mpkey_bookmark;
	int mpkey_next3dmode;
	int mpkey_goto;
	int mpkey_subtitle;
	int mpkey_next_repeat_mode;
	int key_timeshift;
	int key_plugin;

	int key_unlock;

	int key_screenshot;
	int screenshot_count;
	int screenshot_format;
	int screenshot_cover;
	int screenshot_plans;
	int auto_cover;
	std::string screenshot_dir;

	int key_current_transponder;

	int cacheTXT;
	int minimode;
	int mode_clock;

	enum MODE_LEFT_RIGHT_KEY_TV_SETTINGS
	{
		ZAP     = 0,
		VZAP    = 1,
		VOLUME  = 2,
		INFOBAR = 3
	};
	int mode_left_right_key_tv;

#if ENABLE_PIP
	int key_pip_close;
	int key_pip_setup;
	int key_pip_swap;

	int pip_width;
	int pip_height;
	int pip_x;
	int pip_y;
	int pip_radio_width;
	int pip_radio_height;
	int pip_radio_x;
	int pip_radio_y;
#endif
	int bigFonts;
	int window_size;
	int window_width;
	int window_height;
	int eventlist_additional;
	int eventlist_epgplus;

	enum CHANNELLIST_ADDITIONAL_MODES
	{
		CHANNELLIST_ADDITIONAL_MODE_OFF		= 0,
		CHANNELLIST_ADDITIONAL_MODE_EPG 	= 1,
		CHANNELLIST_ADDITIONAL_MODE_MINITV	= 2
	};
	int channellist_additional;
	int channellist_displaymode;
	bool channellist_descmode;
	int channellist_epgtext_align_right;
	int channellist_foot;
	int channellist_new_zap_mode;
	int channellist_sort_mode;
	int channellist_hdicon;
	int channellist_scrambleicon;
	int channellist_numeric_adjust;
	int channellist_show_channellogo;
	int channellist_show_infobox;
	int channellist_show_numbers;
	int repeat_blocker;
	int repeat_genericblocker;
#define LONGKEYPRESS_OFF 499
	int longkeypress_duration;
	int audiochannel_up_down_enable;

	//screen configuration
	int screen_StartX;
	int screen_StartY;
	int screen_EndX;
	int screen_EndY;
	int screen_StartX_a_0;
	int screen_StartY_a_0;
	int screen_EndX_a_0;
	int screen_EndY_a_0;
	int screen_StartX_a_1;
	int screen_StartY_a_1;
	int screen_EndX_a_1;
	int screen_EndY_a_1;
	int screen_StartX_b_0;
	int screen_StartY_b_0;
	int screen_EndX_b_0;
	int screen_EndY_b_0;
	int screen_StartX_b_1;
	int screen_StartY_b_1;
	int screen_EndX_b_1;
	int screen_EndY_b_1;
	int osd_resolution;
#if HAVE_SH4_HARDWARE
	int screen_StartX_int;
	int screen_StartY_int;
	int screen_EndX_int;
	int screen_EndY_int;
#endif
	int screen_preset;
	int screen_width;
	int screen_height;

	//Software-update
	int softupdate_autocheck;
	int softupdate_mode;
	std::string softupdate_proxypassword;
	std::string softupdate_proxyserver;
	std::string softupdate_proxyusername;
	std::string softupdate_url_file;
	std::string	backup_dir;
	std::string	update_dir;

	// parentallock
	int parentallock_prompt;
	int parentallock_lockage;
	int parentallock_defaultlocked;
	int parentallock_zaptime;
	std::string parentallock_pincode;


	// Font sizes
	enum FONT_TYPES
	{
		FONT_TYPE_MENU = 0,
		FONT_TYPE_MENU_TITLE,
		FONT_TYPE_MENU_INFO,
		FONT_TYPE_MENU_FOOT,
		FONT_TYPE_EPG_TITLE,
		FONT_TYPE_EPG_INFO1,
		FONT_TYPE_EPG_INFO2,
		FONT_TYPE_EPG_DATE,
		FONT_TYPE_EPGPLUS_ITEM,
		FONT_TYPE_EVENTLIST_TITLE,
		FONT_TYPE_EVENTLIST_ITEMLARGE,
		FONT_TYPE_EVENTLIST_ITEMSMALL,
		FONT_TYPE_EVENTLIST_DATETIME,
		FONT_TYPE_EVENTLIST_EVENT,
		FONT_TYPE_CHANNELLIST,
		FONT_TYPE_CHANNELLIST_DESCR,
		FONT_TYPE_CHANNELLIST_NUMBER,
		FONT_TYPE_CHANNELLIST_EVENT,
		FONT_TYPE_CHANNEL_NUM_ZAP,
		FONT_TYPE_INFOBAR_NUMBER,
		FONT_TYPE_INFOBAR_CHANNAME,
		FONT_TYPE_INFOBAR_INFO,
		FONT_TYPE_INFOBAR_SMALL,
		FONT_TYPE_FILEBROWSER_ITEM,
		FONT_TYPE_MENU_HINT,
		FONT_TYPE_MOVIEBROWSER_HEAD,
		FONT_TYPE_MOVIEBROWSER_LIST,
		FONT_TYPE_MOVIEBROWSER_INFO,
		FONT_TYPE_SUBTITLES,
		FONT_TYPE_MESSAGE_TEXT,
		FONT_TYPE_BUTTON_TEXT,
		FONT_TYPE_COUNT
	};

	enum FONT_TYPES_FIXED
	{
		FONT_TYPE_FIXED_30_BOLD = 0,
		FONT_TYPE_FIXED_30_REGULAR,
		FONT_TYPE_FIXED_30_ITALIC,
		FONT_TYPE_FIXED_20_BOLD,
		FONT_TYPE_FIXED_20_REGULAR,
		FONT_TYPE_FIXED_20_ITALIC,
		FONT_TYPE_FIXED_16_BOLD,
		FONT_TYPE_FIXED_16_REGULAR,
		FONT_TYPE_FIXED_16_ITALIC,
		FONT_TYPE_FIXED_COUNT
	};

	int infoClockFontSize;
	int infoClockSeconds;
	int infoClockBackground;

#ifdef ENABLE_GRAPHLCD
	// graphlcd
	int glcd_enable;
	uint32_t glcd_color_fg;
	uint32_t glcd_color_bg;
	uint32_t glcd_color_bar;
	std::string glcd_font;
	int glcd_percent_channel;
	int glcd_percent_epg;
	int glcd_percent_bar;
	int glcd_percent_time;
	int glcd_percent_time_standby;
	int glcd_percent_logo;
	int glcd_mirror_osd;
	int glcd_mirror_video;
	int glcd_time_in_standby;
	int glcd_show_logo;
	int glcd_brightness;
	int glcd_brightness_standby;
	int glcd_scroll;
	int glcd_scroll_speed;
	int glcd_selected_config;
#endif

#ifdef ENABLE_LCD4LINUX
	// lcd4linux
	int lcd4l_support;
	std::string lcd4l_logodir;
	int lcd4l_brightness;
	int lcd4l_brightness_standby;
	int lcd4l_dpf_type;
	int lcd4l_skin;
	int lcd4l_skin_radio;
	int lcd4l_convert;
#endif

	// lcdd
	enum LCD_SETTINGS
	{
		LCD_BRIGHTNESS         = 0,
		LCD_STANDBY_BRIGHTNESS,
		LCD_POWER,
		LCD_SHOW_VOLUME,
		LCD_DEEPSTANDBY_BRIGHTNESS,
		LCD_SETTING_COUNT
	};
	int lcd_setting[LCD_SETTING_COUNT];
	int lcd_info_line;
	std::string lcd_setting_dim_time;
	int lcd_setting_dim_brightness;
#if HAVE_SH4_HARDWARE
	int lcd_vfd_scroll;
#endif
	int lcd_scroll;
	int lcd_notify_rclock;

	//#define FILESYSTEM_ENCODING_TO_UTF8(a) (g_settings.filesystem_is_utf8 ? (a) : ZapitTools::Latin1_to_UTF8(a).c_str())
#define FILESYSTEM_ENCODING_TO_UTF8(a) (isUTF8(a) ? (a) : ZapitTools::Latin1_to_UTF8(a).c_str())
#define UTF8_TO_FILESYSTEM_ENCODING(a) (g_settings.filesystem_is_utf8 ? (a) : ZapitTools::UTF8_to_Latin1(a).c_str())
	//#define FILESYSTEM_ENCODING_TO_UTF8_STRING(a) (g_settings.filesystem_is_utf8 ? (a) : ZapitTools::Latin1_to_UTF8(a))
#define FILESYSTEM_ENCODING_TO_UTF8_STRING(a) (isUTF8(a) ? (a) : ZapitTools::Latin1_to_UTF8(a))

	// pictureviewer
	int picviewer_slide_time;
	int picviewer_scaling;

	//audioplayer
	int audioplayer_display;
	int audioplayer_follow;
	int audioplayer_highprio;
	int audioplayer_select_title_by_name;
	int audioplayer_repeat_on;
	int audioplayer_show_playlist;
	int audioplayer_enable_sc_metadata;
	std::string shoutcast_dev_id;
	int shoutcast_enabled;

	//Filebrowser
	int filebrowser_showrights;
	int filebrowser_sortmethod;
	int filebrowser_denydirectoryleave;
	int filebrowser_use_filter;

	//movieplayer
	int movieplayer_repeat_on;
	int movieplayer_display_playtime;
	std::string tmdb_api_key;
	int tmdb_enabled;
	int eof_cnt;

	//zapit setup
	std::string StartChannelTV;
	std::string StartChannelRadio;
	t_channel_id startchanneltv_id;
	t_channel_id startchannelradio_id;
	int uselastchannel;

	//adzap
	int adzap_zapBackPeriod;
	int adzap_writeData;
	int adzap_zapOnActivation;
	enum { ADZAP_ZAP_OFF, ADZAP_ZAP_TO_LAST, ADZAP_ZAP_TO_START };

	int	power_standby;
	int	hdd_sleep;
	int	hdd_noise;
	int	hdd_fs;
	enum { HDD_STATFS_OFF = 0, HDD_STATFS_ALWAYS, HDD_STATFS_RECORDING };
	int	hdd_statfs_mode;
	int	zap_cycle;
	int	sms_channel;
	int	sms_movie;

	std::string	font_file;
	std::string	ttx_font_file;
	std::string	sub_font_file;

	int font_scaling_x;
	int font_scaling_y;

	// NI
	int		show_menu_hints_line;

	int		livestreamResolution;
	std::string	livestreamScriptPath;

	// USERMENU
	typedef enum
	{
		// Do not change ordering of members, add new item just before BUTTON_MAX!!!
		BUTTON_RED = 0,
		BUTTON_GREEN = 1,
		BUTTON_YELLOW = 2,
		BUTTON_BLUE = 3,
		BUTTON_MAX // MUST be always the last in the list
	} USER_BUTTON;

	typedef enum
	{
		// Do not change ordering of members, add new item just before ITEM_MAX!!!
		ITEM_NONE = 0,
		ITEM_BAR = 1,
		ITEM_EPG_LIST = 2,
		ITEM_EPG_PLUS = 3,
		ITEM_EPG_INFO = 4,
		ITEM_EPG_MISC = 5,
		ITEM_AUDIO_SELECT = 6,
		ITEM_SUBCHANNEL = 7,
		ITEM_RECORD = 8,
		ITEM_MOVIEPLAYER_MB = 9,
		ITEM_TIMERLIST = 10,
		ITEM_VTXT = 11,
		ITEM_FAVORITS = 12,
		ITEM_TECHINFO = 13,
		ITEM_REMOTE = 14,
		ITEM_PLUGIN_TYPES = 15,
		ITEM_IMAGEINFO = 16,
		ITEM_BOXINFO = 17,
		ITEM_CAM = 18,
		ITEM_CLOCK = 19,
		ITEM_GAMES = 20,
		ITEM_SCRIPTS = 21,
		ITEM_FILEPLAY_VIDEO = 22,
		ITEM_TOOLS = 23,
		ITEM_LUA = 24,
		ITEM_HDDMENU = 25,
		ITEM_AUDIOPLAY = 26,
		ITEM_INETPLAY = 27,
		ITEM_NETSETTINGS = 28,
		ITEM_SWUPDATE = 29,
		ITEM_LIVESTREAM_RESOLUTION = 30,
		ITEM_ADZAP = 31,
		ITEM_TUNER_RESTART = 32,
		ITEM_THREE_D_MODE = 33,
		ITEM_FILEPLAY_AUDIO = 34,
		ITEM_MAX // MUST be always the last in the list
	} USER_ITEM;

	typedef struct
	{
		unsigned int key;
		std::string items;
		std::string title;
		std::string name;
	} usermenu_t;
	std::vector<usermenu_t *> usermenu;
};

extern const struct personalize_settings_t personalize_settings[SNeutrinoSettings::P_SETTINGS_MAX];

typedef struct time_settings_t
{
	const int default_timing;
	const neutrino_locale_t name;
	const neutrino_locale_t hint;
} time_settings_struct_t;

const time_settings_struct_t timing_setting[SNeutrinoSettings::TIMING_SETTING_COUNT] =
{
	{ 240,	LOCALE_TIMING_MENU,			LOCALE_MENU_HINT_OSD_TIMING},//TODO: add hint locales
	{ 60,	LOCALE_TIMING_CHANLIST,			LOCALE_MENU_HINT_OSD_TIMING},
	{ 240,	LOCALE_TIMING_EPG,			LOCALE_MENU_HINT_OSD_TIMING},
	{ 6,	LOCALE_TIMING_INFOBAR,			LOCALE_MENU_HINT_OSD_TIMING},
	{ 6,	LOCALE_TIMING_INFOBAR_RADIO,		LOCALE_MENU_HINT_OSD_TIMING},
	{ 6,	LOCALE_TIMING_INFOBAR_MOVIEPLAYER,	LOCALE_MENU_HINT_OSD_TIMING},
	{ 3,	LOCALE_TIMING_VOLUMEBAR,		LOCALE_MENU_HINT_OSD_TIMING},
	{ 60,	LOCALE_TIMING_FILEBROWSER,		LOCALE_MENU_HINT_OSD_TIMING},
	{ 3,	LOCALE_TIMING_NUMERICZAP,		LOCALE_MENU_HINT_OSD_TIMING},
	{ 6,	LOCALE_TIMING_POPUP_MESSAGES,		LOCALE_MENU_HINT_OSD_TIMING},
	{ 60,	LOCALE_TIMING_STATIC_MESSAGES,		LOCALE_MENU_HINT_TIMEOUTS_STATIC_MESSAGES}
};

// vfd , simple display
#define DEFAULT_VFD_BRIGHTNESS			4
#define DEFAULT_VFD_STANDBYBRIGHTNESS		4
#define DEFAULT_LCD_POWER			0x01
#define DEFAULT_LCD_SHOW_VOLUME			0x01

#define CORNER_RADIUS_LARGE	CFrameBuffer::getInstance()->scale2Res(11)
#define CORNER_RADIUS_MID	CFrameBuffer::getInstance()->scale2Res(7)
#define CORNER_RADIUS_SMALL	CFrameBuffer::getInstance()->scale2Res(5)
#define CORNER_RADIUS_MIN	CFrameBuffer::getInstance()->scale2Res(3)
#define CORNER_RADIUS_NONE	0

#define RADIUS_LARGE		(g_settings.theme.rounded_corners ? CORNER_RADIUS_LARGE : CORNER_RADIUS_NONE)
#define RADIUS_MID		(g_settings.theme.rounded_corners ? CORNER_RADIUS_MID   : CORNER_RADIUS_NONE)
#define RADIUS_SMALL		(g_settings.theme.rounded_corners ? CORNER_RADIUS_SMALL : CORNER_RADIUS_NONE)
#define RADIUS_MIN		(g_settings.theme.rounded_corners ? CORNER_RADIUS_MIN   : CORNER_RADIUS_NONE)
#define RADIUS_NONE		0

// offsets
#define OFFSET_SHADOW		CFrameBuffer::getInstance()->scale2Res(6)
#define OFFSET_INTER		CFrameBuffer::getInstance()->scale2Res(6)
#define OFFSET_INNER_LARGE	CFrameBuffer::getInstance()->scale2Res(20)
#define OFFSET_INNER_MID	CFrameBuffer::getInstance()->scale2Res(10)
#define OFFSET_INNER_SMALL	CFrameBuffer::getInstance()->scale2Res(5)
#define OFFSET_INNER_MIN	CFrameBuffer::getInstance()->scale2Res(2)
#define OFFSET_INNER_NONE	0

#define SCROLLBAR_WIDTH		(OFFSET_INNER_MID + 2*OFFSET_INNER_MIN)

#define FRAME_WIDTH_MIN		CFrameBuffer::getInstance()->scale2Res(2)
#define FRAME_WIDTH_NONE	0

#define DETAILSLINE_WIDTH	CFrameBuffer::getInstance()->scale2Res(16)

#define SIDEBAR_WIDTH		CFrameBuffer::getInstance()->scale2Res(40)

#define BIGFONT_FACTOR		1.5

struct SglobalInfo
{
	hw_caps_t *hw_caps;
};

const int PARENTALLOCK_PROMPT_NEVER          = 0;
const int PARENTALLOCK_PROMPT_ONSTART        = 1;
const int PARENTALLOCK_PROMPT_CHANGETOLOCKED = 2;
const int PARENTALLOCK_PROMPT_ONSIGNAL       = 3;

class CScanSettings
{
	public:
		CConfigFile	configfile;
		int		bouquetMode;
		int		scanType;

		int		scan_nit;
		int		scan_nit_manual;
		int		scan_bat;
		int		scan_fta_flag;
		int		scan_reset_numbers;
		int		scan_logical_numbers;
		int		scan_logical_hd;
		int		cable_nid;

		std::string	satName;
		int		sat_TP_fec;
		int		sat_TP_pol;
		std::string	sat_TP_freq;
		std::string	sat_TP_rate;
		int		sat_TP_delsys;
		int		sat_TP_mod;
		int		sat_TP_pilot;
		std::string	sat_TP_pli;
		std::string	sat_TP_plc;
		int		sat_TP_plm;

		std::string	cableName;
		int		cable_TP_mod;
		int		cable_TP_fec;
		std::string	cable_TP_freq;
		std::string	cable_TP_rate;
		int		cable_TP_delsys;

		std::string	terrestrialName;
		std::string	terrestrial_TP_freq;
		int		terrestrial_TP_constel;
		int		terrestrial_TP_bw;
		int		terrestrial_TP_coderate_HP;
		int		terrestrial_TP_coderate_LP;
		int		terrestrial_TP_guard;
		int		terrestrial_TP_hierarchy;
		int		terrestrial_TP_transmit_mode;
		int		terrestrial_TP_delsys;
		std::string	terrestrial_TP_pli;

		CScanSettings();

		bool loadSettings(const char *const fileName);
		bool saveSettings(const char *const fileName);
};

#endif
