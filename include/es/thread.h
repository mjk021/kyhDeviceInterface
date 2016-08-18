
#ifndef __ES_THREAD_H__
#define __ES_THREAD_H__

#include <es/base_type.h>
#include <es/es_typedef.h>

#if defined(WIN32)
#include <windows.h>
#endif

#include <list>
using namespace std;

class Thread;
typedef struct ThreadParam
{
//	bool bCheckPointNeedStop;
//	bool bRunning;
//
//	ES_ULONG ulResult;

	Thread* pThread;
	void* pExtParam;
}ThreadParam;


#if defined(WIN32)
typedef unsigned (__stdcall *esThreadProcFunc)(void*);
#else
typedef void* (__stdcall *esThreadProcFunc)(void*);
#endif
typedef bool (__stdcall *esThreadCancelFunc)(void*);


//#include <entersafe/ng_typedef.h>

//#define NG_THREAD_INFINITE				0xFFFFFFFF  // Infinite timeout
//#define NG_TIME_OUT						0x00000001


#define ES_THREAD_HANDLE		HANDLE
#define ES_THREAD_ID			DWORD

#define ES_THREAD_SUCCESS					0x0000
#define ES_THREAD_E_GENERAL_ERROR			0x0001
#define ES_THREAD_E_INVALID_PARAM			0x0002

#define ES_THREAD_BAD_EXIT_CODE				0xFFFFFFFF

class Thread
{
public:
	Thread();
	virtual ~Thread();

	ES_ULONG Create(esThreadProcFunc pProcFunc, esThreadCancelFunc pCancelFunc, void* pParam);


	void BeginThreadProc(void);
	void ExitThreadProc(ES_ULONG ulExitCode);

	// Stop the thread, if it not stop in one second, TerminateThread() should
	// be called.
	//void Stop(void);
	bool WaitStop(ES_ULONG ulTimeOut = INFINITE);
	void Stop(ES_ULONG ulTimeOut = INFINITE);
	void Kill(void);

	// 等待这个线程结束运行(死等)
	//void WaitStop(void);

	// 获得这个线程的id
	ES_ULONG GetId(ES_THREAD_ID* pId);

	ES_ULONG GetExitCode(void){return m_ulExitCode;}

	bool IsRunning(void);
	bool IsNeedStop(void){return m_bCheckPointNeedStop;}

	static bool IsSameHandle(ES_THREAD_HANDLE hThread1, ES_THREAD_HANDLE hThread2);
	static void ZeroHandle(ES_THREAD_HANDLE hThread);
	static bool IsNullHandle(ES_THREAD_HANDLE hThread);

private:
	Thread(const Thread& rhs);
	Thread& operator=(const Thread& rhs);

private:
	ES_THREAD_HANDLE m_ThreadHandle;
	ES_THREAD_ID m_ThreadID;

	ES_ULONG m_ulExitCode;

	esThreadCancelFunc m_cancelFunc;
	ThreadParam m_threadParam;

	bool m_bCheckPointNeedStop;
	bool m_bRunning;
	ES_ULONG m_ulResult;

	//void* m_pThreadProcArg;
};

typedef list<Thread*> thread_pool;


#endif // __ES_THREAD_H__

// EOF
