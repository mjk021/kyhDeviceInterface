/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	linux.h

DESC:
	Linux specific config options. (Stolen from boost)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/


#define NG_PLATFORM "linux"

#ifndef linux
#	define linux
#endif

// 我们暂时规定，在Linux平台上只允许使用GCC来编译.
//#if !defined(__GNUC__)
#if !defined(NG_GNUC)
#	error "On Linux platform, you must use GCC compiler."
#endif

#define NG_HAS_PTHREADS
#define NG_HAS_PTHREAD_YIELD
#define NG_HAS_GETTIMEOFDAY
