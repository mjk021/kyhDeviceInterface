/*
[]=========================================================================[]

	Copyright(C) 2000-2005, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	base.h

DESC:
	

REVISION:
	2005-01-12 [Apex Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __EPASS_BASE_H__
#define __EPASS_BASE_H__

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#elif defined(linux) || defined(macintosh)
#include <wchar.h>
#include <unistd.h>
	#if defined(UNICODE) || defined(_UNICODE)
		#define TCHAR		wchar_t
	#else
		#define TCHAR		char
	#endif

	#ifndef ULONG
		#define ULONG unsigned long
	#endif

	#ifndef UINT
		#define UINT unsigned int
	#endif

	#ifndef _TEXT
		#define _TEXT(x) x
	#endif
	
	#ifndef MAX_PATH
		#define MAX_PATH PATH_MAX
	#endif

	#define GetModuleFileName(x,y,z) getcwd(y,z)
#endif

// 屏蔽变量名称过长的警告
#pragma warning (disable:4786)

#include <string>
#include <iostream>
#include <sstream>

#if !defined(stdString)
#	if defined(UNICODE) || defined(_UNICODE)
#		define stdString			std::wstring
#	else
#		define stdString			std::string
#	endif
#endif

#if !defined(stdOStrStream)
#	if defined(UNICODE) || defined(_UNICODE)
#		define stdOStrStream			std::wostringstream
#	else
#		define stdOStrStream			std::ostringstream
#	endif
#endif

#if !defined(tcout)
#	if defined(UNICODE) || defined(_UNICODE)
#		define tcout		std::wcout
#	else
#		define tcout		std::cout
#	endif
#endif

#endif // __EPASS_BASE_H__

