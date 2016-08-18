/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	sharemem.h

DESC:
	��װ�Թ����ڴ�Ĳ�����
	ע�⣺Ϊ��������һ�������ܹ�֪���ù����ڴ�Ĵ�С�������ڴ��������ڴ��
		ʱ������һ��UNLONG�����湲���ڴ�ĳߴ磬���ǿͻ���ȡ����ʱȴ����
		�����ULONG�����Create()/GetSize()/GetData()��

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

	// Create()��ָ�������ִ���һ�������ڴ���
	// �������ʱ�ù����ڴ����Ѿ����ڣ��ͷ��ش���
	ES_ULONG Create(const stdString strName, ES_ULONG ulSize);
	ES_ULONG Destroy(void);

	// Open()���ŵ�һ��ָ�����ƵĹ����ڴ���
	// �������ʱ�ù����ڴ�����δ����,�ͷ��ش���.
	ES_ULONG Open(stdString strName, DWORD dwSessionID = 0xFFFFFFFF);

	// Create()/Open()�ķ�����
	ES_ULONG Close(void);

	// ����ֱ��ʹ�õĹ����ڴ�ָ��
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
