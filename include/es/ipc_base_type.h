#ifndef	__ES_IPC_BASE_H__
#define __ES_IPC_BASE_H__
#include <es/config.h>
#include <es/es_typedef.h>

#if defined(WIN32)
#	include <es/security_attr.h>
typedef	HANDLE	ES_IPC_handle;
#define ES_IPC_INVALUE_HANDLE NULL	
#elif defined(linux) || defined(macintosh)
typedef	long 	ES_IPC_handle;
#define ES_IPC_INVALUE_HANDLE -1
#define MAX_TRIES	100

#ifdef linux
#ifdef semun
#undef semun
#endif
union semun
{
	int					val;
	struct semid_ds		*buf;
	unsigned short int 	*array;
	struct seminfo 		*_buf;
};
#endif //linux

#endif

static const ES_ULONG PIPC_OK						= 0x00;
static const ES_ULONG PIPC_ALREADY_EXISTS			= 0x01;
static const ES_ULONG PIPC_NOT_EXISTS				= 0x02;
static const ES_ULONG PIPC_ARGUMENTS_BAD			= 0x03;
static const ES_ULONG PIPC_BAD_CALL					= 0x04;
static const ES_ULONG PIPC_USE_GETLASTERROR			= 0x05;	// 请使用系统的相关函数获取错误值
static const ES_ULONG PIPC_PROCESS_NUMBER_LIMIT		= 0x06;	// 请使用系统的相关函数获取错误值
static const ES_ULONG PIPC_SUPPORT_ERROR			= 0x07;	// 请使用系统的相关函数获取错误值
static const ES_ULONG PIPC_CANCEL					= 0x08;	// 请使用系统的相关函数获取错误值
static const ES_ULONG PIPC_BUFFER_TOO_SMALL			= 0x09;	// 请使用系统的相关函数获取错误值

#endif//__ES_IPC_BASE_H__
