/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	utilities.h

DESC:
	

REVISION:
	2004-03-04 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __NG_IPC_UTILITIES_H__
#define __NG_IPC_UTILITIES_H__

#include <es/base_type.h>
#include <es/config.h>
//#include <string>
//using namespace std;

//namespace our_test_namespace
//{

#if !defined(WIN32)
#include <sys/sem.h>
#define ES_IPC_INVALID_KEY				-1

// Get a key_t (for mutex and share memory, etc) from a string.
// Use CRC32 the string, and use the result as the key_t.
// If strName is empty, this function return -1 (Invalid key_t value).
// key_t is a unsigned long.
key_t StringToKey(const stdString& strName);

#endif // #if defined(NG_HAS_PTHREADS)

//} // namespace our_test_namespace

#endif // __NG_IPC_UTILITIES_H__
