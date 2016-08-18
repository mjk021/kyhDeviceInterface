/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	select_compiler_config.hpp

DESC:
	NG compiler configuration selection header file.
	(Stolen from boost.)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

// locate which compiler we are using and define NG_COMPILER_CONFIG
// as needed: 

#if defined __GNUC__
//  GNU C++:
#	define ES_COMPILER_CONFIG "config/compiler/gcc.h"

#elif defined _MSC_VER
//  Microsoft Visual C++
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER
#	define ES_COMPILER_CONFIG "config/compiler/visualc.h"

#else
// this must come last - generate an error if we don't
// recognise the compiler:
#	error "Unknown compiler - please report results to Euphen Liu <yufeng@mail.page> "

#endif
