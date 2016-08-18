/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	gcc.h

DESC:
	GNU C++ compiler setup. (Stolen from boost.)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

//  GNU C++ compiler setup:

#define NG_GNUC __GNUC__

// versions check:
// 我们要求GCC的编译器版本在2.95及以上。
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 95)
#	error "Your GCC compiler too old. Use v2.95 or later and try again."
#endif

#define NG_COMPILER "GNU C++ version " __VERSION__

//
// last known and checked version is 4.1:
//#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 0))
#if (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ > 1))
#	error "Unknown compiler version - please report the results to Apex Liu <yufeng@mail.page>"
#endif

