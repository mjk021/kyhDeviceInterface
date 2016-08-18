/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	xtime.h

DESC:
	

REVISION:
	2004-02-24 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __NG_XTIME_H__
#define __NG_XTIME_H__

#include <es/config.h>

//namespace our_test_namespace
//{

enum
{
    TIME_UTC=1,
    TIME_TAI,
    TIME_MONOTONIC,
    TIME_PROCESS,
    TIME_THREAD,
    TIME_LOCAL,
    TIME_SYNC,
    TIME_RESOLUTION
};

struct xtime
{
//#if defined(BOOST_NO_INT64_T)
//    int_fast32_t sec;
//#else
//    int_fast64_t sec;
//#endif
//    int_fast32_t nsec;
#if defined(__GNUC__)
	long long sec;
#else
	__int64 sec;
#endif

	long nsec;
};

int xtime_get(struct xtime* xtp, int clock_type);
inline int xtime_cmp(const xtime& xt1, const xtime& xt2)
{
    int res = (int)(xt1.sec - xt2.sec);
    if (res == 0)
        res = (int)(xt1.nsec - xt2.nsec);
    return res;
}

//} // namespace our_test_namespace

#endif // __NG_XTIME_H__

