/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	share_mem.h

DESC:
	封装对共享内存的操作。
	注意：为了在另外一个进程能够知道该共享内存的大小，我们在创建共享内存的
		时候多分配一个UNLONG来保存共享内存的尺寸，但是客户获取数据时却不包
		含这个ES_ULONG。详见Create()/GetSize()/GetData()。

REVISION:
	[2006-06-20 Apex Liu] Created.
[]=========================================================================[]
*/

#ifndef __ES_SHARE_MEMORY_H__
#define __ES_SHARE_MEMORY_H__

#include <es/base_type.h>
#include <es/es_typedef.h>
#include <es/share_memory.h>
#include <es/process_mutex.h>
#define TOKEN_COUNT_MAX		10

#if defined(WIN32)
#include <WtsApi32.h>

typedef BOOL (WINAPI* funcWTSEnumSessions)(IN HANDLE hServer, IN DWORD Reserved, IN DWORD Version, OUT PWTS_SESSION_INFOA* ppSessionInfo, OUT DWORD * pCount);
typedef VOID (WINAPI* funcWTSFreeMemory)(IN PVOID pMemory);
typedef BOOL (WINAPI* funcProcessIdToSessionId)(DWORD dwProcessId, DWORD* pSessionId);

#elif defined(linux) || defined(macintosh)
#include <string.h>
#include <stdlib.h>
#endif//#if defined(WIN32)
static const ES_ULONG SHM_OK				= 0x00;
static const ES_ULONG SHM_ALREADY_EXISTS	= 0x01;
static const ES_ULONG SHM_NOT_EXISTS		= 0x02;
static const ES_ULONG SHM_ARGUMENTS_BAD	= 0x03;
static const ES_ULONG SHM_USE_GETLASTERROR	= 0x04;	// 请使用系统的相关函数获取错误值
static const ES_ULONG SHM_NOT_OTHER_SESSION = 0x05;	// 没有其他session的共享内存供更新

class CShareMemory
{
friend class LockShareMemoryHolder;

public:
	CShareMemory();
	virtual ~CShareMemory();

	// Create()用指定的名字创建一个共享内存区
	// 如果创建时该共享内存区已经存在，就返回成功
	ES_ULONG Create(const stdString& strName, ES_ULONG ulSize,bool IsFristCreate = false);

	// Open()附着到一个指定名称的共享内存区
	// 如果附着时该共享内存区尚未存在,就返回错误.
	// 可以多次执行该函数
	ES_ULONG Open(stdString &strName,bool IsFristCreate = false, DWORD dwSessionID = 0xFFFFFFFF);

	// CreateOpen()的反操作
	ES_ULONG Close(void);
	ES_ULONG Destroy(void);
	ES_ULONG Delete();
	ES_ULONG ClearMemoryValue();

	ES_ULONG GetSize(void)
	{
		//return (static_cast<ES_ULONG>(m_pData[0]));
		return m_ulSize;
	}
	ES_ULONG GetIncCount(void)
	{
		//return m_ulIncCount;
		return GET32(&((ES_UINT*)m_pRealData)[2]);
	}
	ES_ULONG UpdateIncCount(void)
	{
		m_ulIncCount = GET32(&(++((ES_UINT*)m_pRealData)[2]));
		return m_ulIncCount;
	}
	ES_ULONG ResetIncCount(void)
	{
		//((ES_ULONG*)m_pRealData)[2] = 0;
		memset(&((ES_UINT*)m_pRealData)[2], 0, sizeof(ES_UINT));
		m_ulIncCount = 0;
		return m_ulIncCount;
	}



	stdString& GetName(void){return m_strBaseName;}

	ES_ULONG Lock(void);
	ES_ULONG Unlock(void);
	//ES_BOOL IsValid(void){return ((NULL != m_pData) && (0 != m_strBaseName.length()));}
	ES_BOOL IsValid(void){return ((NULL != m_pData) && (NULL != m_pRealData) && (0 != m_strBaseName.length()));}
	
	ES_ULONG RefreshOtherSessions(ES_BOOL bSaveAndValidate);
	ES_ULONG RefreshSessions();
	ES_ULONG GetLatestSMSessID(DWORD *pdwUpdateCount);	
	ES_BOOL IsNeedUptSMFromSess();

private:
	// 不允许 赋值 和 拷贝 操作
	CShareMemory(const CShareMemory &rhs);
	CShareMemory &operator=(const CShareMemory &rhs);

	// 可以直接使用的共享内存指针
	ES_BYTE* _GetData(void)
	{
		return m_pData;
	}
	ES_ULONG LoadWTSFunc(void);

	ES_ULONG RefreshOneSession(DWORD dwSessionID);



private:
	CProcessMutex	m_ProcMutex;
	CProcessMutex	m_ALLMemMutex;
	//CShareMem		m_ProcMem;

	stdString m_strBaseName;
	HANDLE m_hFileMapping;
 //	HANDLE m_hMutex;
	ES_BYTE* m_pRealData;
	ES_BYTE* m_pData;
	ES_ULONG m_ulSize;
	ES_ULONG m_ulIncCount;
	ES_BYTE* m_pSlotData;
	ES_ULONG m_ulSlotIndex;
	ES_ULONG m_ulStorage;
	ES_ULONG m_ulTotalSize;

#if defined(WIN32)
	bool m_IsVista;
	HMODULE m_hWtsDll;
	HMODULE m_hKerDll;
	funcWTSEnumSessions m_pFuncEnumSessions;
	funcWTSFreeMemory m_pFuncFreeMemory;
	funcProcessIdToSessionId m_pFuncProcessIdToSessionId;
	DWORD	m_dwCurrentSessionId;
#endif //#if defined(WIN32)
};

class LockShareMemoryHolder
{
public:
	LockShareMemoryHolder(CShareMemory* pShareMemory)
	{
		m_pShareMemory = pShareMemory;
		//m_pShareMemory->Lock();
	}

	ES_BYTE* AcquireDataPtr(void)
	{
		if (NULL != m_pShareMemory)
			return m_pShareMemory->_GetData();
		else
			return NULL;
	}

	~LockShareMemoryHolder()
	{
		if (NULL != m_pShareMemory)
		m_pShareMemory->Unlock();
	}

protected:
	CShareMemory* m_pShareMemory;
};

class ShareMemoryHandleHolder
{
public:
	ShareMemoryHandleHolder(CShareMemory* pShareMemory) :
		m_pShareMemory(pShareMemory),
	  	//m_bHold(true)
		m_bHold(false) // now when create it hold it.
	{
	}
	void ReleaseHolder(void)
	{
		m_bHold = false;
	}
	~ShareMemoryHandleHolder(void)
	{
		if (m_bHold && NULL != m_pShareMemory)
		{
			m_pShareMemory->Close();
			m_pShareMemory = NULL;
		}
	}
	
protected:
	CShareMemory* m_pShareMemory;
	bool m_bHold;
};
#endif // __NG_SHARE_MEMORY_H__
