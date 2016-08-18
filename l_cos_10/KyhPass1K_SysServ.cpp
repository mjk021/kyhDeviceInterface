/***
* KyhPass1KSysServ.cpp:
*		definitions of apdu cmd api
* Purpose:
*       This file defines the structures, global values,and functions
*       used by KyhPass1KApdu.cpp
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-19 11:05:02
***/



#include "KyhPass1K_Define.h"
#include "KyhPass1K_Apdu.h"
#include "KyhPass1K_Glb.h"
#include <base/reader.h>

WORD KingCos_EraseMF(DEV_HANDLE icd)
{
	unsigned long recvLen=MAX_PATH;

	BYTE send[280] = {0};
	g_apdu.cla = 0x80;
	g_apdu.ins = 0xDF;
	g_apdu.p1 = 0x00;                                 // code reset
	g_apdu.p2 = 0x00;
	g_apdu.le = 0x00;
	KingCos_BuildApdu(icd,&g_apdu,send,NULL,0,APDU_MODEL_PLAIN	,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;    
}

/* 
this cmd for COS is download into the Card,
but in non-Initial status
*/
WORD KingCos_InitDevice(DEV_HANDLE icd,
						unsigned char *pIssueKey,
						unsigned char IssueKeyLen
						)
{

	BYTE send[280] = {0};
	int apduLen = 0;
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = 0x84;
	g_apdu.ins = 0x12;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;

	/*0x1b = 1   (EraseMF_AC)                  + 
					1   (IssueKeyErrCnt)            + 
					1   (IssueKeyLength)           + 
					16 (IssueKey)                       +
					8   (Random)
					*/

	/*this number do not include Mac ,
	and this lc is not the final lc
	*/
	g_apdu.lc = 0x1b;               
	g_apdu.le = 0x00;  
	// security permission of delete MF 
	g_apdu.sendbuf[0] = 0x07;
	// times of error count of CreateMF
	g_apdu.sendbuf[1] = 0x03;
	// length of IssueKey
	g_apdu.sendbuf[2] = IssueKeyLen;
	/*before InitDevice , get a 8 byte randoms as the Key
	for Mac
	*/
	BYTE Challenge[MAX_PATH] = {0};
	KingCos_GetChallenge( icd,0x08,Challenge);


	// copy the IssueKey from sendbuf[3] to sendbuf[18]
	memcpy(g_apdu.sendbuf+3,pIssueKey,IssueKeyLen);
	
	// copy the 8 byte randoms to the sendbuf behind the IssueKey
	memcpy(g_apdu.sendbuf+3+IssueKeyLen,Challenge,0x08);

	/* 
	this cmd including send a random cmd for Des
	encrypt to get 4bytes Mac
	*/
	KingCos_BuildApdu(icd,&g_apdu,send,INIT_KEY,0x10, APDU_MODEL_ENC_MAC,Challenge);
	
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc + 4 +1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}




/*
this cmd for Reset the device
*/
WORD KingCos_ResetDevice(DEV_HANDLE icd)
{
	unsigned long recvLen=MAX_PATH;

	BYTE send[280] = {0};
	g_apdu.cla = 0x80;
	g_apdu.ins = 0x10;
	g_apdu.p1 = 0x00;                                 // code reset
	g_apdu.p2 = 0x00;
	g_apdu.le = 0x00;
	KingCos_BuildApdu(icd,&g_apdu,send,NULL,0,APDU_MODEL_PLAIN	,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;

}

WORD KingCos_UpdateKey(DEV_HANDLE icd,
					 BYTE KeyUsage,
					 BYTE KeyIndex,
					 BYTE KeyLen,
					 unsigned char *pKey,
					 unsigned char *pMKey,
					 unsigned char MKeyLen,
					 unsigned char SM)
{

	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x84;
	g_apdu.ins = 0x16;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x03 + KeyLen;

	g_apdu.sendbuf[0] = KeyUsage;
	g_apdu.sendbuf[1] = KeyIndex;
	g_apdu.sendbuf[2] = KeyLen;

	memcpy(g_apdu.sendbuf+3, pKey, KeyLen);
	g_apdu.le = 0x00;
	unsigned char *tKey;
	unsigned char tKeyLen;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		tKey = pMKey;
		tKeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN_MAC == SM)
	{
		tKey = pMKey;
		tKeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		tKey = NULL;
		tKeyLen = 0x00;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,tKey,	tKeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}

/*
this cmd for load Key to the KeyFile
*/
WORD KingCos_LoadKey(DEV_HANDLE icd,
					 BYTE KeyUsage,
					 BYTE KeyIndex,
					 BYTE KeyAlg,
					 BYTE KeyVer,
					 BYTE UsageAC,
					 BYTE UpdateAC,
					 BYTE RFUStatus,
					 BYTE ErrCnt,
					 BYTE KeyLen,
					 unsigned char *pKey,
					 unsigned char *pMKey,
					 unsigned char MKeyLen,
					 unsigned char SM

					 )
{

	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x84;
	g_apdu.ins = 0x15;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09 + KeyLen;

	g_apdu.sendbuf[0] = KeyUsage;
	g_apdu.sendbuf[1] = KeyIndex;
	g_apdu.sendbuf[2] = KeyAlg;
	g_apdu.sendbuf[3] = KeyVer;
	g_apdu.sendbuf[4] = UsageAC;
	g_apdu.sendbuf[5] = UpdateAC;
	g_apdu.sendbuf[6] = RFUStatus;
	g_apdu.sendbuf[7] = ErrCnt;
	g_apdu.sendbuf[8] = KeyLen;

	memcpy(g_apdu.sendbuf+9, pKey, KeyLen);
	g_apdu.le = 0x00;
	unsigned char *tKey;
	unsigned char tKeyLen;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		tKey = pMKey;
		tKeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN_MAC == SM)
	{
		tKey = pMKey;
		tKeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		tKey = NULL;
		tKeyLen = 0x00;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,tKey,	tKeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	
	return g_sw1sw2;

}


/*
this cmd for load Pin to the KeyFile
*/
WORD KingCos_LoadPin(DEV_HANDLE icd,
					 BYTE PinUsage,
					 BYTE PinIndex,
					 BYTE PinAlgandMode,                 // different from LoadKey on this byte
					 BYTE PinVer,
					 BYTE UsageAC,
					 BYTE UpdateAC,
					 BYTE RFUStatus,
					 BYTE ErrCnt,
					 BYTE PinLen,
					 const unsigned char *pPin,
					 unsigned char *pMKey,
					 unsigned char MKeyLen,
					 int SM
					 )
{

	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x84;
	g_apdu.ins = 0x15;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09 + PinLen;

	g_apdu.sendbuf[0] = PinUsage;
	g_apdu.sendbuf[1] = PinIndex;
	g_apdu.sendbuf[2] = PinAlgandMode;
	g_apdu.sendbuf[3] = PinVer;
	g_apdu.sendbuf[4] = UsageAC;
	g_apdu.sendbuf[5] = UpdateAC;
	g_apdu.sendbuf[6] = RFUStatus;
	g_apdu.sendbuf[7] = ErrCnt;
	g_apdu.sendbuf[8] = PinLen;

	memcpy(g_apdu.sendbuf+9, pPin, PinLen);
	unsigned char *pKey;
	unsigned char KeyLen;
	unsigned char challenge[MAX_PATH] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}
	else if (APDU_MODEL_PLAIN_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}

	KingCos_BuildApdu(icd,&g_apdu,send,pKey,	KeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;

}
