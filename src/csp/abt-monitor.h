/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
 Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>

 AIT is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 See the file COPYING included with this distribution for more
 information.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef C_ABT_MONITOR_H_
#define C_ABT_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

struct C_ABT_Monitor;
typedef struct C_ABT_Monitor C_ABT_Monitor;
C_ABT_Monitor* abt_monitor_create(char* host, const unsigned short responserPort,
        const unsigned short publisherPort, char* xcsp);
void abt_monitor_destroy(C_ABT_Monitor* m);
void abt_monitor_start(C_ABT_Monitor* m);

#ifdef __cplusplus
}
#endif

#endif /* C_ABT_MONITOR_H_ */
