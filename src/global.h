/*
 * global.hpp
 *
 *  Created on: May 24, 2013
 *      Author: soroush
 */

#ifndef GLOBAL_HPP_
#define GLOBAL_HPP_

#include <iostream>
#include <ctime>
#include <stdio.h>

#define LOGGING

#define _ERROR(format, args...)  \
	{ \
	time_t t = time(0);\
    struct tm * now = localtime( & t );\
    fprintf(stderr, "[ERROR %d-%d-%d %d:%d:%d]: ",now->tm_year + 1900,\
            now->tm_mon + 1, now->tm_mday,\
            now->tm_hour, now->tm_min, now->tm_sec); \
    fprintf(stderr, format , ## args);\
    printf("\n");\
    fflush(stdout);\
	}

#define _INFO(format, args...)  \
	{ \
    time_t t = time(0);\
    struct tm * now = localtime( & t );\
    fprintf(stdout, "[INFO  %d-%d-%d %d:%d:%d]: ",now->tm_year + 1900,\
            now->tm_mon + 1, now->tm_mday,\
            now->tm_hour, now->tm_min, now->tm_sec); \
    fprintf(stdout, format , ## args);\
    printf("\n"); \
    fflush(stderr);\
	}

#endif /* GLOBAL_HPP_ */
