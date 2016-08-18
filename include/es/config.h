/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	config.h

DESC:
	NG configuration header file. (Stolen from boost.)

REVISION:
	2004-02-19 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __ES_CONFIG_PRELOAD_H__
#define __ES_CONFIG_PRELOAD_H__

#define ES_STRINGIZE(X) ES_DO_STRINGIZE(X)
#define ES_DO_STRINGIZE(X) #X

//=========================================================================//
// Detect which compiler we are using.
#if !defined(ES_COMPILER_CONFIG)	// && !defined(NG_NO_COMPILER_CONFIG) && !defined(NG_NO_CONFIG)
#	include "config/select_compiler_config.h"	//<boost/config/select_compiler_config.hpp>
#endif
// if we have a compiler config, include it now:
#ifdef ES_COMPILER_CONFIG
#	include ES_COMPILER_CONFIG
#endif

//=========================================================================//
// Detect which platform we are running.
// if we don't have a platform config set, try and find one:
#if !defined(ES_PLATFORM_CONFIG)	// && !defined(BOOST_NO_PLATFORM_CONFIG) && !defined(BOOST_NO_CONFIG)
#  include "config/select_platform_config.h"
#endif
// if we have a platform config, include it now:
#ifdef ES_PLATFORM_CONFIG
#  include ES_PLATFORM_CONFIG
#endif

//-------------------------------------------------------------------------//
// 显示我们编译的环境
//-------------------------------------------------------------------------//
// TODO: Could not use "#pragma message" under g++. Who can help me?
//#define NG_DEVELOPMENT_ENV		"Build on " NG_PLATFORM " with " NG_COMPILER
//#pragma message(NG_DEVELOPMENT_ENV)

#endif // __ES_CONFIG_PRELOAD_H__
