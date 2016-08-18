/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	thread_event.h

DESC:
	event thought threads and inside a process	

REVISION:
	2005-07-04 [Skybird Le]
		Created.
[]=========================================================================[]
*/
#ifndef __NG_THREAD_EVENT_H__
#define __NG_THREAD_EVENT_H__
#include <es/base_type.h>
#include <es/ipc_base_type.h>

#if defined(WIN32)
#	include <windows.h>
#elif defined(macintosh) || defined(linux)
#	include <semaphore.h>
#	include <errno.h>
#endif
//namespace our_test_namespace
//{
static const ES_ULONG TEVENT_OK						= 0x00;
static const ES_ULONG TEVENT_WAITING				= 0x01; // 有一个线程正在等待
static const ES_ULONG TEVENT_USE_GETLASTERROR		= 0x04;	// 请使用系统的相关函数获取错误值


class esCThreadEvent
{
public:
	esCThreadEvent();
	~esCThreadEvent();

public:
	ES_ULONG Create();
	ES_ULONG Close();
	ES_ULONG WaitForEvent();
	ES_ULONG SetEvent();
#if defined(WIN32)
	const HANDLE	 GetEventHandle(){return m_hEvent;}
#endif

private:
#if defined(WIN32)
	HANDLE m_hEvent;
#elif defined(macintosh)
	sem_t* m_sem;
#else
    sem_t m_sem; 
#endif
};

//}

#endif //__NG_THREAD_EVENT_H__
