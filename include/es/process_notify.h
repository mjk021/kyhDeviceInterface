#ifndef __ES_PROCESS_NOTIFY_H__
#define __ES_PROCESS_NOTIFY_H__

#include <es/share_mem.h>
//nclude <es/process_mutex.h>
#include <es/process_event.h>
//#include <es/ipcchannel.h>
#include <es/thread.h>
#include <es/thread_event.h>

#include <list>
using namespace std;

#define NOTIFY_SHARE_MEMORY_SIZE	4096
#define NOTIFY_MESSAGE_MAX_SIZE		2048
#define NOTIFY_PROCESS_MAX_COUNT	500

#define FIFO_FILE_PATH			"/tmp/shuttle_fifo/"

//#pragma pack(push,1)
#pragma pack(1)
typedef struct NotifyMessage
{
	ES_UINT	ulNotifyType;
	ES_UINT ulMessageLen;
	ES_BYTE	 ubMessage[1];
}NotifyMessage;
#pragma pack()
//#pragma pack(pop)

//typedef list<ES_BYTE_PTR> NotifyMessageList;

typedef void (__stdcall* funcptr_notify_callback)(void* pArg, NotifyMessage* slotEvent);

class CProcessNotify
{
public:

	CProcessNotify();
	~CProcessNotify();
	
	// 函数输出接口
	// 初始化共享内存，和共享内存对应的互斥体，并创建接收队列和接收线程,函数成功返回0 
	ES_ULONG	proc_init(ES_CHAR_PTR pbName);//,funcptr_notify_callback notify_callback, void* pArg);
	// Unmap 共享内存，close 共享内存对应的互斥体，结束创建接收队列和接收线程，函数成功返回0 
	ES_ULONG	proc_Final();
	void proc_SetCallBack(funcptr_notify_callback notify_callback, void* pArg);
	ES_ULONG	proc_CreateEvent();
	// 发送消息pbData给 PID: pid = 0 ,广播； PID != 1 , 向特定的进程广播，函数成功返回0 ，其中pbDate编码方式为如下
	//	CK_ULONG	m_MessageType 
	//	CK_ULONG	m_MessageLength
	//				message Data
	ES_ULONG	proc_Send(ES_ULONG pid, ES_BYTE_PTR pbDate, ES_ULONG ulDataLen);
	ES_ULONG	proc_GetCurrentProcessCount(ES_ULONG& ulCurrentProcessCount);
//	// 取得当前进程接收队列的第一条消息的地址，从*ppData 中返回,假如队列中没有消息了,*ppbData = NULL，函数成功返回0 
//	ES_ULONG	proc_Recv(ES_BYTE_PTR *ppbData);
//	// 释放消息对列中的对应的数据包地址
//	ES_ULONG	proc_Free(ES_BYTE_PTR pData);
	// 注册当前进程的PID到PID共享内存列表
	ES_ULONG	proc_RegisterPid();
	// 注消当前进程的PID到PID共享内存列表
	ES_ULONG	proc_UnRegister(ES_ULONG ulProcessID = 0);
	ES_ULONG	proc_GetProcessCount();


private:

	funcptr_notify_callback m_NotifyCallback;
	void* m_pNotifyCallbackArg;

	// 检测进程是否都是激活的
	ES_BOOL IsProcActive(ES_ULONG ulProcessID);
	// 清除进程列表中僵死的进程
	ES_ULONG FlushProcess();


#if defined(WIN32)
	static unsigned __stdcall _GetNotifyMessage(void* pArg);
	static bool __stdcall _NotifyMessageCancel(void* pParam);

	ES_IPC_handle m_RecvMessageEvent;
	ES_IPC_handle m_SendMessageEvent;
	ES_IPC_handle m_threadCanceEvent;

#elif defined(linux) || defined(macintosh)
//	static void* _GetNotifyMessage(void* pArg);
	static void* _GetNotifyMessage(void* pArg);
	static bool _NotifyMessageCancel(void* pArg);

	int m_iFifoRead;
	int m_iFifoWrite;
#endif
	ES_BOOL	m_bTrue;

	ES_ULONG m_ulBufferSize;
	ES_ULONG m_ProcessID;
	Thread m_Thread;
	//CShareMemory m_ShareMemory;
	CShareMem m_ShareMemory;
	CProcessMutex m_ProcessMutex;
	ES_BYTE_PTR m_pData;
	ES_BOOL	m_IsStopThread;
	ES_BOOL	m_IsRegistered;
	stdString m_strBaseName;
};

#endif
