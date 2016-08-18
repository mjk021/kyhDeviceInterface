/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	sharemem.h

DESC:
	封装对共享内存的操作。
	注意：为了在另外一个进程能够知道该共享内存的大小，我们在创建共享内存的
		时候多分配一个UNLONG来保存共享内存的尺寸，但是客户获取数据时却不包
		含这个ULONG。详见Create()/GetSize()/GetData()。

REVISION:
	2004-03-02 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __NG_SHARE_MEMORY_H__
#define __NG_SHARE_MEMORY_H__

#include <es/base_type.h>
#include <es/ipc_base_type.h>


#if defined(linux) || defined(macintosh)

#	if defined(linux)
#		define SVSHM_MODE	(S_IRUSR | S_IRGRP  | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH)
#	elif defined(macintosh)
#		define SVSHM_MODE	(SHM_R | SHM_W | SHM_R >> 3 | SHM_R >> 6 | SHM_W >> 3 | SHM_W >> 6)
#	endif

#endif

class CShareMem //: private noncopyable
{
public:
	CShareMem();
	virtual ~CShareMem();

	// Create()用指定的名字创建一个共享内存区
	// 如果创建时该共享内存区已经存在，就返回错误
	ES_ULONG Create(const stdString strName, ES_ULONG ulSize);
	ES_ULONG Destroy(void);

	// Open()附着到一个指定名称的共享内存区
	// 如果附着时该共享内存区尚未存在,就返回错误.
	ES_ULONG Open(stdString strName, DWORD dwSessionID = 0xFFFFFFFF);

	// Create()/Open()的反操作
	ES_ULONG Close(void);

	// 可以直接使用的共享内存指针
	ES_BYTE_PTR GetData(void)
	{
		return m_pData;
	}

//	ES_ULONG GetSize(void)
//	{
//		return (static_cast<ES_ULONG>(m_pData[0]));
//	}

private:
	CShareMem(const CShareMem &rhs);
	CShareMem &operator=(const CShareMem &rhs);

private:
	ES_IPC_handle m_hFileMapping;
	ES_BYTE_PTR m_pData;
};

//} // namespace our_test_namespace

#endif // __NG_SHARE_MEMORY_H__
