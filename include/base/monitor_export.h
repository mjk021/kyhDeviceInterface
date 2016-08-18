#include "base/typedefine.h"

#ifndef __ES_MONITOR_EXPORT_H__
#define __ES_MONITOR_EXPORT_H__
typedef struct S_SlotEvent
{
	ULONG slotId;
	ULONG eventType;
	ULONG extraData;
	ULONG ulReaderType;
	char accessPath[MAX_PATH];
	char strSlotName[33];		// Reader name (maybe virtually)
}S_SlotEvent;
#ifdef __cplusplus
extern "C" {
#endif
extern BOOL M_WaitForSlotEvent(bool bBlock, S_SlotEvent *pEvent);
extern BOOL M_Initialize(void *);
extern void M_PostEvent(S_SlotEvent *pEvent);

#ifdef __cplusplus
}
#endif
#endif
