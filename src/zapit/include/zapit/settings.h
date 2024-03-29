/*
 * $Header: /cvs/tuxbox/apps/dvb/zapit/include/zapit/settings.h,v 1.8.2.6 2003/06/14 06:46:22 digi_casi Exp $
 *
 * zapit's settings - d-box2 linux project
 *
 * (C) 2002 by thegoodguy <thegoodguy@berlios.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __zapit__settings_h__
#define __zapit__settings_h__


#ifdef HAVE_CONFIG_H
#include                "config.h"
#else
#define CONFIGDIR       "/var/tuxbox/config"
#endif

#define BOUQUETS_TMP	"/tmp/bouquets.tmp"
#define BOUQUETS_XML	ZAPITDIR "/bouquets.xml"
#define SERVICES_TMP	"/tmp/services.tmp"
#define SERVICES_XML	ZAPITDIR "/services.xml"
#define UBOUQUETS_XML	ZAPITDIR "/ubouquets.xml"
#define ZAPITCONFIGFILE	ZAPITDIR "/zapit.conf"

#define CURRENTSERVICES_TMP	"/tmp/currentservices.tmp"
#define CURRENTSERVICES_XML	"/tmp/currentservices.xml"

#define CABLES_XML	CONFIGDIR "/cables.xml"
#define SATELLITES_XML	CONFIGDIR "/satellites.xml"
#define TERRESTRIAL_XML	CONFIGDIR "/terrestrial.xml"
#define WEBRADIO_XML	WEBCHANNELS "/webradio.xml"
#define WEBTV_XML	WEBCHANNELS "/webtv.xml"

#define CAMD_UDS_NAME	"/tmp/camd.socket"


#endif /* __zapit__settings_h__ */
