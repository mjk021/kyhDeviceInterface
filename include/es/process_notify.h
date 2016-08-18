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
	
	// ��������ӿ�
	// ��ʼ�������ڴ棬�͹����ڴ��Ӧ�Ļ����壬���������ն��кͽ����߳�,�����ɹ�����0 
	ES_ULONG	proc_init(ES_CHAR_PTR pbName);//,funcptr_notify_callback notify_callback, void* pArg);
	// Unmap �����ڴ棬close �����ڴ��Ӧ�Ļ����壬�����������ն��кͽ����̣߳������ɹ�����0 
	ES_ULONG	proc_Final();
	void proc_SetCallBack(funcptr_notify_callback notify_callback, void* pArg);
	ES_ULONG	proc_CreateEvent();
	// ������ϢpbData�� PID: pid = 0 ,�㲥�� PID != 1 , ���ض��Ľ��̹㲥�������ɹ�����0 ������pbDate���뷽ʽΪ����
	//	CK_ULONG	m_MessageType 
	//	CK_ULONG	m_MessageLength
	//				message Data
	ES_ULONG	proc_Send(ES_ULONG pid, ES_BYTE_PTR pbDate, ES_ULONG ulDataLen);
	ES_ULONG	proc_GetCurrentProcessCount(ES_ULONG& ulCurrentProcessCount);
//	// ȡ�õ�ǰ���̽��ն��еĵ�һ����Ϣ�ĵ�ַ����*ppData �з���,���������û����Ϣ��,*ppbData = NULL�������ɹ�����0 
//	ES_ULONG	proc_Recv(ES_BYTE_PTR *ppbData);
//	// �ͷ���Ϣ�����еĶ�Ӧ�����ݰ���ַ
//	ES_ULONG	proc_Free(ES_BYTE_PTR pData);
	// ע�ᵱǰ���̵�PID��PID�����ڴ��б�
	ES_ULONG	proc_RegisterPid();
	// ע����ǰ���̵�PID��PID�����ڴ��б�
	ES_ULONG	proc_UnRegister(ES_ULONG ulProcessID = 0);
	ES_ULONG	proc_GetProcessCount();


private:

	funcptr_notify_callback m_NotifyCallback;
	void* m_pNotifyCallbackArg;

	// �������Ƿ��Ǽ����
	ES_BOOL IsProcActive(ES_ULONG ulProcessID);
	// ��������б��н����Ľ���
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
