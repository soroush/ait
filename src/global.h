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

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream>
#include <ctime>
#include <stdio.h>

#define LOGGING
/**
 * Library Export Symbols.
 */
#ifdef _WIN32
#    ifdef DLL_EXPORT
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#endif
#ifndef _WIN32
#    define LIBRARY_API
#endif

/**
 * Error Logging Backend
 */
#define _ERROR(format, args...)  \
	{ \
	time_t t = time(0);\
    struct tm * now = localtime( & t );\
    fprintf(stderr, "[ERROR %d-%d-%d %d:%d:%d]: ",now->tm_year + 1900,\
            now->tm_mon + 1, now->tm_mday,\
            now->tm_hour, now->tm_min, now->tm_sec); \
    fprintf(stderr, format , ## args);\
    printf("\n");\
    fflush(stderr);\
	}

/**
 * Information Logging Backend
 */
#define _INFO(format, args...)  \
	{ \
    time_t t = time(0);\
    struct tm * now = localtime( & t );\
    fprintf(stdout, "[INFO  %d-%d-%d %d:%d:%d]: ",now->tm_year + 1900,\
            now->tm_mon + 1, now->tm_mday,\
            now->tm_hour, now->tm_min, now->tm_sec); \
    fprintf(stdout, format , ## args);\
    printf("\n"); \
    fflush(stdout);\
	}

#endif /* GLOBAL_H_ */
