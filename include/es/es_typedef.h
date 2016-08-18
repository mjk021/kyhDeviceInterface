/*
[]======================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	ngtype.h

DESC:
	Base type define. Used in all ePass ES projects.

REVISION:
	2002-12-20 [Euphen Liu]
		Created.
[]======================================================================[]
*/

#ifndef __ES_ES_TYPE_H__
#define __ES_ES_TYPE_H__

#include <es/config.h>

// Signed 8-bit value, -127 to 127.
typedef char					ES_CHAR;

// Unsigned 8-bit value, 0 to 255.
typedef unsigned char			ES_BYTE;

// Signed int
typedef int						ES_INT;
typedef unsigned int					ES_UINT;

// Unsigned 16-bit value, 0 to 65535.
typedef unsigned short			ES_WORD;

// Signed 32-bit value.
typedef long					ES_LONG;

// Unsigned 32-bit value, 0 to 4G.
typedef unsigned long int ES_ULONG;

typedef void					ES_VOID;

// Boolean
typedef ES_BYTE					ES_BOOL;
typedef ES_VOID*				ES_VOID_PTR;
//typedef void*					ES_VOID_PTR;
typedef ES_BYTE*				ES_BYTE_PTR;
typedef ES_CHAR*				ES_CHAR_PTR;
typedef ES_LONG*				ES_LONG_PTR;
typedef ES_ULONG*				ES_ULONG_PTR;
typedef ES_UINT*				ES_UINT_PTR;
typedef ES_BOOL*				ES_BOOL_PTR;
typedef ES_WORD*				ES_WORD_PTR;

typedef ES_ULONG				ES_FLAG;
typedef ES_FLAG*				ES_FLAG_PTR;

typedef ES_ULONG				ES_RV;

#if !defined(stdString)
#	if defined(UNICODE) || defined(_UNICODE)
#		define stdString			std::wstring
#	else
#		define stdString			std::string
#	endif
#endif

// the following typedef is only for linux ngpkcs11 cast 
// we can remove and change the definition in sources if free
#ifndef FALSE
	#define FALSE				0
#endif

#ifndef TRUE
	#define TRUE				1
#endif

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif

#ifndef MAX_PATH
	#define MAX_PATH 			260
#endif

#if defined(linux) || defined(macintosh)

//// define the platform cast functions
#ifndef ZeroMemory
#	define ZeroMemory(x,y)			memset(x,0,y)
#endif

#ifndef __stdcall
	#define __stdcall
#endif

#ifndef CopyMemory
#	define CopyMemory 				memcpy
#endif

#if defined _T
#undef _T
#endif

#ifndef _T
#	define 	_T(X)			X
#endif

////{{ linux has defined them
#ifndef DWORD
typedef ES_UINT					DWORD;
#endif
typedef ES_BYTE					UCHAR;
typedef ES_WORD					USHORT;
typedef ES_WORD					WORD;
typedef ES_BYTE					BYTE;
typedef ES_LONG					LONG;
typedef ES_ULONG_PTR			DWORD_PTR;


#	ifndef LPTSTR
#		define LPTSTR			ES_CHAR*
#	endif

typedef unsigned long			HANDLE;

#define INFINITE				0xFFFFFFFF
#define INVALID_HANDLE_VALUE	0xFFFFFFFF
#define INVALID_FILE_SIZE		0xFFFFFFFF		


#define CREATE_NEW				1
#define CREATE_ALWAYS			2
#define OPEN_EXISTING			3
#define OPEN_ALWAYS				4
#define TRUNCATE_EXISTING		5

#define FILE_BEGIN				0
#define FILE_CURRENT			1
#define FILE_END				2

#define MAKEWORD(a, b)      	((WORD)(((BYTE)((DWORD)(a) & 0xff)) | ((WORD)((BYTE)((DWORD)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      	((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
#define LOWORD(l)           	((WORD)((DWORD)(l) & 0xFFFF))
#define HIWORD(l)           	((WORD)((DWORD)(l) >> 16))
#define LOBYTE(w)           	((BYTE)((DWORD)(w) & 0xFF))
#define HIBYTE(w)           	((BYTE)((DWORD)(w) >> 8))

#endif // linux|macintosh

////define the ENDIAN of platform
//// macintosh: big-endian->little-endian
//// other: little-endian
unsigned short GET16(unsigned short *s);
unsigned int GET32(unsigned int *i);
unsigned short MAKE16(unsigned short *s);
unsigned int MAKE32(unsigned int *i);

#if defined(linux) || defined(macintosh)
#define GetLastError()			errno
#endif
#endif // __ES_ES_TYPE_H__
