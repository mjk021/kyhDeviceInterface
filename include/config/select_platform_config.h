/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	select_platform_config.h

DESC:
	Boost compiler configuration selection header file
	(Stolen from boost)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

// locate which platform we are on and define NG_PLATFORM_CONFIG as needed.
// Note that we define the headers to include using "header_name" not
// <header_name> in order to prevent macro expansion within the header
// name (for example "linux" is a macro on linux systems).

#if defined(linux) || defined(__linux) || defined(__linux__)
// linux:
#  define ES_PLATFORM_CONFIG "config/platform/linux.h"

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define ES_PLATFORM_CONFIG "config/platform/win32.h"

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define ES_PLATFORM_CONFIG "config/platform/macos.h"

#else
// this must come last - generate an error if we don't
// recognise the platform:
#	error "Unknown platform - please report the results to Euphen Liu <yufeng@mail.page>"

#endif



