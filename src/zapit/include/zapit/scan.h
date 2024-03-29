/*
        Neutrino-GUI  -   DBoxII-Project

        Copyright (C) 2011 CoolStream International Ltd

        License: GPLv2

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __scan_h__
#define __scan_h__

#include <linux/dvb/frontend.h>

#include <inttypes.h>
#include <map>
#include <list>
#include <string>

#include <zapit/femanager.h>
#include <zapit/getservices.h>
#include "bouquets.h"
#include <OpenThreads/Thread>

extern CBouquetManager* scanBouquetManager;

class CServiceScan : public OpenThreads::Thread
{
	public:
		typedef enum scan_type {
			SCAN_PROVIDER,
			SCAN_TRANSPONDER

		} scan_type_t;
		typedef enum scan_flags {
			SCAN_NIT		= 0x01,
			SCAN_BAT		= 0x02,
			SCAN_FTA		= 0x04,
			SCAN_RESET_NUMBERS	= 0x08,
			SCAN_LOGICAL_NUMBERS	= 0x10,
			SCAN_TV			= 0x20,
			SCAN_RADIO		= 0x40,
			SCAN_TVRADIO		= 0x60,
			SCAN_DATA		= 0x80,
			SCAN_ALL		= 0xE0,
			SCAN_LOGICAL_HD		= 0x100
		} scan_flags_t;

	private:
		bool started;
		bool running;
		bool abort_scan;
		scan_type_t scan_mode;
		int flags;
		void * scan_arg;
		bool satHaveChannels;

		uint32_t fake_tid, fake_nid;
		uint32_t found_transponders;
		uint32_t processed_transponders;
		uint32_t found_channels;
		uint32_t found_tv_chans;
		uint32_t found_radio_chans;
		uint32_t found_data_chans;
		uint32_t failed_transponders;
		unsigned short cable_nid;

		transponder_list_t scantransponders;   // list to scan
		transponder_list_t scanedtransponders; // successfully scanned
		transponder_list_t failedtransponders; // failed to tune
		transponder_list_t nittransponders;    // transponders from NIT
		std::map <t_channel_id, uint8_t> service_types;

		bool ScanProvider(t_satellite_position satellitePosition);
		void Cleanup(const bool success);
		bool tuneFrequency(FrontendParameters *feparams, t_satellite_position satellitePosition);
		void SendTransponderInfo(transponder &t);
		bool ReadNitSdt(t_satellite_position satellitePosition);
		bool AddFromNit();
		void FixServiceTypes();

		void CheckSatelliteChannels(t_satellite_position satellitePosition);
		bool ScanTransponder();
		bool ScanProviders();
		void SaveServices();
		void CleanAllMaps();
		//bool ReplaceTransponderParams(freq_id_t freq, t_satellite_position satellitePosition, FrontendParameters *feparams);

		void run();

		CFrontend * frontend;
		static CServiceScan * scan;
		CServiceScan();

	public:
		~CServiceScan();
		static CServiceScan * getInstance();

		bool Start(scan_type_t mode, void * arg);
		bool Stop();

		bool AddTransponder(transponder_id_t TsidOnid, FrontendParameters *feparams, bool fromnit = false);
		void ChannelFound(uint8_t service_type, std::string providerName, std::string serviceName);
		void AddServiceType(t_channel_id channel_id, uint8_t service_type);

		bool Scanning() { return running; };
		void Abort() { abort_scan = true; };
		bool Aborted() { return abort_scan; };

		bool SetFrontend(t_satellite_position satellitePosition);
		CFrontend * GetFrontend() { return frontend; };

		uint32_t & FoundTransponders() { return found_transponders; };
		uint32_t & FoundChannels() { return found_channels; };
		void SetCableNID(unsigned short nid) { cable_nid = nid; }
		bool isFtaOnly() { return flags & SCAN_FTA; }
		int GetFlags() { return flags; }
		bool SatHaveChannels() { return satHaveChannels; }
};
#endif /* __scan_h__ */
