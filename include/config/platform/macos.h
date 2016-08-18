/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	macos.h

DESC:
	Mac OS specific config options. (Stolen from boost)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#define NG_PLATFORM "Mac OS"

#ifndef macintosh
#	define macintosh
#endif

// 我们暂时规定，在Mac OS X平台上只允许使用GCC来编译.
#if !defined(NG_GNUC)
#	error "On MacOS X platform, you must use GCC compiler."
#endif

#define NG_HAS_PTHREADS
#define NG_HAS_SCHED_YIELD
#define NG_HAS_GETTIMEOFDAY

/*
#if __MACH__ && !defined(_MSL_USING_MSL_C)

#warning "Using the Mac OS X system BSD-style C library."

// Using the Mac OS X system BSD-style C library.

#  define NG_NO_CTYPE_FUNCTIONS
#  define NG_NO_CWCHAR

//
// BSD runtime has pthreads, sigaction, sched_yield and gettimeofday,
// of these only pthreads are advertised in <unistd.h>, so set the 
// other options explicitly:
//
#  define NG_HAS_SCHED_YIELD
#  define NG_HAS_GETTIMEOFDAY
#  define NG_HAS_SIGACTION

#  if (__GNUC__ < 3) && !defined( __APPLE_CC__)

// GCC strange "ignore std" mode works better if you pretend everything
// is in the std namespace, for the most part.

#    define NG_NO_STDC_NAMESPACE
#  endif

//#else
//#warning "Using the MSL C library."
//// Using the MSL C library.
//// We will eventually support threads in non-Carbon builds, but we do
//// not support this yet.
//#  if TARGET_CARBON
//#    define NG_HAS_MPTASKS
//// The MP task implementation of Boost Threads aims to replace MP-unsafe
//// parts of the MSL, so we turn on threads unconditionally.
//#    define NG_HAS_THREADS
//// The remote call manager depends on this.
//#    define NG_BIND_ENABLE_PASCAL
//#  endif

#elif
// Now we does not support other comiler.
#	error "Sorry, this compiler does not support on Mac OS X now."

#endif

*/
