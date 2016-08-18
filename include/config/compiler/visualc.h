/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	visualc.h

DESC:
	Microsoft Visual C++ compiler setup. (Stolen from boost.)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#define NG_MSVC _MSC_VER

// turn off the warnings before we #include anything
#pragma warning( disable : 4503 ) // warning: decorated name length exceeded
#pragma warning( disable : 4786 ) // ident trunc to '255' chars in debug info

//   
// check for exception handling support:   
#ifndef _CPPUNWIND   
#  define NG_NO_EXCEPTIONS   
#endif 

#if _MSC_VER == 1200
#	define NG_COMPILER_VERSION 6.0
#elif _MSC_VER == 1300
#	define NG_COMPILER_VERSION 7.0
#elif _MSC_VER == 1310
#	define NG_COMPILER_VERSION 7.1
#else
#	define NG_COMPILER_VERSION _MSC_VER
#endif

#define NG_COMPILER "Microsoft Visual C++ version " NG_STRINGIZE(NG_COMPILER_VERSION)

//
// versions check:
// we don't support Visual C++ prior to version 6:
#if _MSC_VER < 1200
#error "Compiler not supported or configured - please reconfigure"
#endif

//
// last known and checked version is 1310:
#if (_MSC_VER > 1500)
#	error "Unknown compiler version - please report the results to Euphen Liu <yufeng@mail.page>"
#endif

