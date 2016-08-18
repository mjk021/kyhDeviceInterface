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

#ifndef	__ES_PROC_EVENT_H__
#define __ES_PROC_EVENT_H__
#include <es/base_type.h>
#include <es/ipc_base_type.h>

ES_BOOL InterlockedIncrement(const stdString& ref_name, ES_LONG_PTR ref);
ES_BOOL InterlockedDecrement(const stdString& ref_name, ES_LONG_PTR ref);

ES_IPC_handle ES_Event_Create(const stdString& strEventName);
ES_ULONG ES_Event_Destroy(ES_IPC_handle  hevent);

ES_IPC_handle ES_Event_Open(const stdString& strEventName);
ES_ULONG ES_Event_Close(ES_IPC_handle  hevent);

ES_ULONG ES_SetEvent(ES_IPC_handle  hevent);
ES_ULONG ES_WaitForSingleObject(ES_IPC_handle  hevent);
ES_ULONG ES_WaitForMultipleObjects(ES_IPC_handle * phevent,ES_ULONG ulCount);

#endif // __ES_PROC_EVENT_H__
