/** 
* @file        interval.h
* @brief    测试数据转发的正确性
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef INTERVAL_H
#define INTERVAL_H
#include "compile_type.h"
#include "m_type.h"
#ifndef __RT_INCLUDE
#ifdef __SPARC_GCC_MMU
#include <time.h>
#include <fsu_pthread.h>
#elif __GCC_C99
#include "unistd.h"
#endif
#else
#include "unistd.h"
#endif

#define msec_t uint_32

/** 
 * 统一使用自定义的sleep_ms
 * @param[in]   msec    毫秒
 */ 
static inline int sleep_ms(msec_t msec){
#ifdef __SPARC_GCC_MMU
    struct timespec tv;
    uint_32 sec=msec/1000;
    uint_64 nsec=(msec%1000)*1000000;
    tv.tv_sec=sec;
    tv.tv_nsec=nsec;
    return nanosleep(&tv,NULL);
#elif __GCC_C99
    uint_64 usec=msec*1000;
    return usleep(usec);
#endif
}

#endif
