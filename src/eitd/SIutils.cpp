/*
 * SIutils.cpp, utility functions for the SI-classes (dbox-II-project)
 *
 * (C) 2001 by fnbrd (fnbrd@gmx.de)
 *
 * Copyright (C) 2011-2012 CoolStream International Ltd
 *
 * License: GPLv2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <time.h>
#include <string.h>

#include "SIutils.hpp"

// Thanks to kwon
time_t changeUTCtoCtime(const unsigned char *buffer, int local_time)
{
	int year, month, day, y_, m_, k, hour, minutes, seconds, mjd;

	if (!memcmp(buffer, "\xff\xff\xff\xff\xff", 5))
		return 0; // keine Uhrzeit

	mjd = (buffer[0] << 8) | buffer[1];
	hour = buffer[2];
	minutes = buffer[3];
	seconds = buffer[4];

	y_   = (int)((mjd - 15078.2) / 365.25);
	m_   = (int)((mjd - 14956.1 - (int)(y_ * 365.25)) / 30.6001);
	day  = mjd - 14956 - (int)(y_ * 365.25) - (int)(m_ * 30.60001);

	k = !!((m_ == 14) || (m_ == 15));

	year  = y_ + k + 1900;
	month = m_ - 1 - k * 12;

	struct tm time;
	memset(&time, 0, sizeof(struct tm));

	time.tm_mday = day;
	time.tm_mon = month - 1;
	time.tm_year = year - 1900;
	time.tm_hour = (hour >> 4) * 10 + (hour & 0x0f);
	time.tm_min = (minutes >> 4) * 10 + (minutes & 0x0f);
	time.tm_sec = (seconds >> 4) * 10 + (seconds & 0x0f);

	return mktime(&time) + (local_time ? -timezone : 0);
}

time_t parseDVBtime(uint16_t mjd, uint32_t bcd, bool local_time)
{
	int year, month, day, y_, m_, k, hour, minutes, seconds;

	y_   = (int)((mjd - 15078.2) / 365.25);
	m_   = (int)((mjd - 14956.1 - (int)(y_ * 365.25)) / 30.6001);
	day  = mjd - 14956 - (int)(y_ * 365.25) - (int)(m_ * 30.60001);

	hour = (bcd >> 16) & 0xFF;
	minutes = (bcd >> 8) & 0xFF;
	seconds = bcd & 0xFF;

	k = !!((m_ == 14) || (m_ == 15));

	year  = y_ + k + 1900;
	month = m_ - 1 - k * 12;

	struct tm time;
	memset(&time, 0, sizeof(struct tm));

	time.tm_mday = day;
	time.tm_mon = month - 1;
	time.tm_year = year - 1900;
	time.tm_hour = (hour >> 4) * 10 + (hour & 0x0f);
	time.tm_min = (minutes >> 4) * 10 + (minutes & 0x0f);
	time.tm_sec = (seconds >> 4) * 10 + (seconds & 0x0f);

	return mktime(&time) + (local_time ? -timezone : 0);
}

// Thanks to tmbinc
int saveStringToXMLfile(FILE *out, const char *c, int /*withControlCodes*/)
{
	if (!c)
		return 1;
	// Die Umlaute sind ISO-8859-9 [5]
	/*
	  char buf[6000];
	  int inlen=strlen(c);
	  int outlen=sizeof(buf);
	//  UTF8Toisolat1((unsigned char *)buf, &outlen, (const unsigned char *)c, &inlen);
	  isolat1ToUTF8((unsigned char *)buf, &outlen, (const unsigned char *)c, &inlen);
	  buf[outlen]=0;
	  c=buf;
	*/
	for (; *c; c++)
	{
		switch ((unsigned char)*c)
		{
			case '<':
				fprintf(out, "&lt;");
				break;
			case '>':
				fprintf(out, "&gt;");
				break;
			case '&':
				fprintf(out, "&amp;");
				break;
			case '\"':
				fprintf(out, "&quot;");
				break;
			case '\'':
				fprintf(out, "&apos;");
				break;
			case 0x0a:
				fprintf(out, "&#x0a;");
				break;
			case 0x0d:
				fprintf(out, "&#x0d;");
				break;
			default:
				if ((unsigned char)*c < 32)
					break;
				fprintf(out, "%c", *c);
		} // case

	} // for
	return 0;
}

// Entfernt die ControlCodes aus dem String (-> String wird evtl. kuerzer)
void removeControlCodes(char *string)
{
	if (!string)
		return;
	for (; *string;)
		if (!((*string >= 32) && (((unsigned char)*string) < 128)))
			memmove(string, string + 1, strlen(string + 1) + 1);
		else
			string++;
	return ;
}
