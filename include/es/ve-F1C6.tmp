/*

[]=========================================================================[]



	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.

	All rights reserved.



FILE:

	process_mutex.h



DESC:

	这是一个可以跨越进程的Mutex实现。



REVISION:

	2004-03-04 [Euphen Liu]

		Created.

[]=========================================================================[]

*/

#ifndef	__ES_PROC_MUTEX_H__
#define __ES_PROC_MUTEX_H__
#include <es/base_type.h>
#include <es/ipc_base_type.h>

class CProcessMutex
{
public:
	CProcessMutex();
	virtual ~CProcessMutex();

	ES_ULONG Create(const stdString& strName);
	ES_ULONG Destroy(void);

	ES_ULONG ReMove(const stdString& strName);//强制启动时清除内核对象
	ES_ULONG Open(const stdString& strName);
	ES_ULONG Close(void);

	ES_ULONG Lock(void);
	ES_ULONG TryLock(void);
	ES_ULONG Unlock(void);

	const ES_IPC_handle GetMutexHandle(){return m_hMutex;};

private:
	CProcessMutex(const CProcessMutex& rhs);
	CProcessMutex& operator=(const CProcessMutex& rhs);

private:
	ES_IPC_handle	m_hMutex;
#if defined(WIN32)
	OSVersion g_osver;
#else
	ES_ULONG m_threadid;
	ES_ULONG m_LockedCounts;;
#endif
	bool m_bIsByCreate;
};

class LockProcessMutexHolder
{
public:
	LockProcessMutexHolder(CProcessMutex* pProcessMutex)
	{
		m_pProcessMutex = pProcessMutex;
	}
	~LockProcessMutexHolder()
	{
		if(NULL != m_pProcessMutex)
		{
			m_pProcessMutex->Unlock();
		}
	}

protected:
	CProcessMutex* m_pProcessMutex;
};

ES_BOOL InterlockedIncrement(const stdString& ref_name, ES_LONG_PTR ref);
ES_BOOL InterlockedDecrement(const stdString& ref_name, ES_LONG_PTR ref);

#endif // __ES_PROC_MUTEX_H__
