/*
 * rtl-sdr, turns your Realtek RTL2832 based DVB dongle into a SDR receiver
 * Copyright (C) 2012 by Hoernchen <la@tfc-server.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SDRPLAY_EXPORT_H
#define SDRPLAY_EXPORT_H

 

#  define __SDR_EXPORT     __declspec(dllexport)
#  define __SDR_IMPORT     __declspec(dllimport)
 
#ifndef rtlsdr_STATIC
#	ifdef rtlsdr_EXPORTS
#	define MIRISDR_API __SDR_EXPORT
#	else
#	define SDRPLAY_API __SDR_IMPORT
#	endif
#else
#define RTLSDR_API
#endif


#endif /* SDRPLAY_EXPORT_H */
