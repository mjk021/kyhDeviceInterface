#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "typedefine.h"


#define READER_TYPE_UNKNOWN			0x00

#define READER_TYPE_KYH_HID			0x0D
#define READER_TYPE_UDISK			0x0E
#define READER_TYPE_KYH_CCID		0x0F

#ifndef __READER__H
#define __READER__H

#define MAX_COMMAND_BUFFER_LEN

//////////////////////////
#define ERR_OPEN_DEVIDE			8001
#define ERR_GET_ATTR			8002
#define ERR_COS_COMMAND			8003
#define ERR_GET_ATR				8004
#define ERR_READ				8005
#define ERR_WRITE				8006



//#define READER_TYPE_COUNT	3
//#define READER_DLL_NAME1	"zc100hid"
//#define READER_DLL_NAME2	"pcsc"
//#define READER_DLL_NAME3	"zc100_u"
#ifdef __cplusplus
extern "C" {
#endif
	BOOL P_Initialize(VOID *arg);
	void P_Finalize();
	BOOL P_GetSlotListName(char pReaderList[][MAX_NAME_LEN],ULONG *pCount);
    unsigned char P_GetCardStatus(DEV_HANDLE CardHandle);
	BOOL P_GetSlotList(DEV_HANDLE *pList,ULONG *pCount);
	BOOL P_GetSlotInfo(DEV_HANDLE DevHandle,S_SlotInfo *pInfo);
	BOOL P_Open(const char *ReaderName,DEV_HANDLE *pDevHandle);
	BOOL P_Close(DEV_HANDLE DevHandle);
	BOOL P_IsOpened(DEV_HANDLE DevHandle);
	DEV_RES P_SendApduComm(DEV_HANDLE DevHandle,
						unsigned long DataInLen,const unsigned char *DataIn,
						unsigned long *pDataOutLen,unsigned char *pDataOut);
	BOOL P_Reset(DEV_HANDLE DevHandle,unsigned char *ATR,ULONG *ATRLen);
	ULONG P_GetCosType(DEV_HANDLE CardHandle);
#ifdef __cplusplus
}
#endif

typedef BOOL (*F_P_Initialize)(VOID *arg);
typedef void (*F_P_Finalize)();
typedef BOOL (*F_P_GetSlotListName)(char pReaderList[][MAX_NAME_LEN],ULONG *pCount);
typedef BOOL (*F_P_Open)(const char *ReaderName,DEV_HANDLE *pCardHandle);
typedef BOOL (*F_P_GetSlotList)(DEV_HANDLE *pList,ULONG *pCount);
typedef BOOL (*F_P_GetSlotInfo)(DEV_HANDLE CardHandle,S_SlotInfo *pInfo);
typedef ULONG (*F_P_GetCosType)(DEV_HANDLE CardHandle);
typedef BOOL (*F_P_Close)(DEV_HANDLE CardHandle);
typedef BOOL (*F_P_IsOpened)(DEV_HANDLE CardHandle);
typedef DEV_RES (*F_P_SendApduComm)(DEV_HANDLE CardHandle,unsigned long DataInLen,const unsigned char *DataIn,unsigned long *pDataOutLen,unsigned char *pDataOut);
typedef BOOL (*F_P_Reset)(DEV_HANDLE CardHandle,unsigned char *ATR,ULONG *ATRLen);

typedef struct S_ReaderModule
{
	ULONG version;
	F_P_Initialize pP_Initialize;
	F_P_Finalize pP_Finalize;


	F_P_GetSlotListName	pP_GetSlotListName;
	F_P_Open	pP_Open;
	F_P_GetSlotList			pP_GetSlotList;
	F_P_GetSlotInfo		pP_GetSlotInfo;
	F_P_GetCosType		pP_GetCosType;
	F_P_Close		pP_Close;
	F_P_IsOpened		pP_IsOpened;
	F_P_SendApduComm	pP_SendApduComm;
	F_P_Reset	pP_Reset;
	char ModuleName[MAX_PATH];
#if defined(WIN32)
	HMODULE hModule;
#else
	void *hModule;
#endif
}S_ReaderModule;



#endif
