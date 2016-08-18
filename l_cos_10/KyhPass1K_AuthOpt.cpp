/***
* KyhPass1KAuth_Opt.cpp:
*		definitions of api of apdu cmd 
* Purpose:
*       This file define the cos api used by KyhPass1KApdu.cpp
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-23 17:19:40
***/



#include "KyhPass1K_Define.h"
#include "KyhPass1K_Apdu.h"
#include "KyhPass1K_Glb.h"
#include "AlgDes.h"

#include  <base/reader.h>



/*
this cmd is to get the Challenge from recvbuf or not
*/
WORD KingCos_GetChallenge(DEV_HANDLE icd,
						  unsigned char Number,                      // input number
						  unsigned char *pChallenge               // output receive dataBuffer
						  )
{
	// define the temporary apdu  ,do not use g_apdu, because of flush apdu when other KingCos to call this function
	unsigned  short sw1sw2	;
	APDUSt apdu;
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;
	apdu.cla = 0x00;
	apdu.ins = 0x84;
	apdu.p1 = 0x00;
	apdu.p2 = 0x00;
	apdu.le = Number;
	apdu.lc = 0;
	KingCos_BuildApdu( icd,	&apdu, send,NULL,0,APDU_MODEL_PLAIN,NULL);
	sw1sw2 = P_SendApduComm( icd, apdu.lc+4+1, send, &recvLen, apdu.recvbuf );
	/* sometimes we do not need the Challenges get from the recvbuf , 
	and give the pChallenge point to NULL , also can return 0x9000
	*/
	if(NULL != pChallenge)
		memcpy(pChallenge,apdu.recvbuf,Number);
	return sw1sw2;
}


/*
this cmd is for the UKey verify whether the device is  valid 
*/
WORD KingCos_ExterAuth(DEV_HANDLE icd,
					   unsigned char KeyIndex,
					   unsigned char *pExAuKey                     // external authority Key
					   )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x00;
	g_apdu.ins = 0x82;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = KeyIndex;                                              // external authority Key index
	g_apdu.lc = 0x08;
	//before send external authority cmd , get a 8 byte randoms as the sendbuf
	BYTE Challenge[MAX_PATH] = {0};
	BYTE Random[8] = {0};                                            // for store the Challenge get from the UKey
	KingCos_GetChallenge(icd, 0x08, Challenge);
	// copy the 8 byte randoms to the sendbuf 
	memcpy(Random,Challenge,0x08);
	// use the  external authority Key to encrypt the Random
	TripleDesEnc(g_apdu.sendbuf, Random, 8, pExAuKey);
	// use the plain 
	KingCos_BuildApdu(icd, &g_apdu, send, NULL, 0, APDU_MODEL_PLAIN,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}


/*
this cmd is for the Device verify whether the UKey is  valid 
*/
WORD KingCos_InterAuth(DEV_HANDLE icd,
					   unsigned char KeyIndex,
					   unsigned char *pInAuKey                     // internal authority Key
					   )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x00;
	g_apdu.ins = 0x88;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = KeyIndex;                                             // internal authority Key index
	g_apdu.lc = 0x08;
	//before send internal authority cmd , get a 8 byte randoms as the sendbuf
	BYTE Challenge[MAX_PATH] = {0};
	KingCos_GetChallenge(icd,0x08,Challenge);
	// copy the 8 byte randoms to the sendbuf 
	memcpy(g_apdu.sendbuf,Challenge,0x08);
	// use the plain 
	KingCos_BuildApdu(icd, &g_apdu, send, NULL, 0, APDU_MODEL_PLAIN,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;

} 


/*
this cmd for verify the pin ,compare the pin get from the keyboard with the pin
stored in the UKey
*/
WORD KingCos_VerifyPin(DEV_HANDLE icd,
					   unsigned int usertype,
					   const unsigned char *pPin,
					   unsigned long PinLen
					   )
{
	unsigned long recvLen=MAX_PATH;
	unsigned char PinIndex;
	if (usertype == TYPE_ADMIN_PIN )
		PinIndex = PID_ADMIN;
	else if (usertype == TYPE_USER_PIN )
		PinIndex = PID_USER;
	BYTE send[280] = {0};
	g_apdu.cla = 0x00;
	g_apdu.ins = 0x20;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = PinIndex;
	g_apdu.lc = 0x08;																									// the length of random

	BYTE Challenge[MAX_PATH] = {0};
	// define temporary PIN to pad the input pin as the Key for 3DES to verify pin
	unsigned char PinPad[16] = {0};
	memcpy(PinPad,pPin,PinLen);
	memcpy(PinPad + PinLen,"\x80",1);																	// pad the 1st byte with 0x80
	KingCos_GetChallenge(icd,0x08,Challenge	);
	memcpy(g_apdu.sendbuf,Challenge,0x08);

	//memcpy(g_apdu.sendbuf + 8,"\x80\x00\x00\x00\x00\x00\x00\x00",0x08); // pad random with 80 00 00 00 00 00 00 00 
	// use the PIN as the Key,should pad the PIN to 16byte
	KingCos_BuildApdu(icd,&g_apdu,send,PinPad,0x10,	APDU_MODEL_ENC,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}



/*
this cmd for unblock pin and reload a new pin into the UKey
*/
WORD KingCos_ReloadPin(DEV_HANDLE icd,
					   unsigned char PinIndex,
					   const unsigned char *pNewPin,
					   unsigned long NewPinLen,
					   BYTE *pReloadKey,
					   unsigned char ReloadKeyLen
					   )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	int sendLen=0;
	BYTE recvBuf[MAX_PATH];
	BYTE plainAfterPad[24]={0};
	int plainAfterPadLen=0;

	if(NewPinLen>16)
		NewPinLen=16;

	BYTE Challenge[MAX_PATH] = {0};
	// get the challenge for InitValue
	g_sw1sw2=KingCos_GetChallenge(icd,0x04,Challenge);
	if(g_sw1sw2!=0x9000)
		return g_sw1sw2;

	memcpy(plainAfterPad,pNewPin,NewPinLen);
	plainAfterPad[NewPinLen]=0x80;
	plainAfterPadLen=(8-(NewPinLen%8)) + NewPinLen;


	BYTE DataAfterEnc[280];
	TripleDesEnc(DataAfterEnc,plainAfterPad,plainAfterPadLen,pReloadKey);

	BYTE MacSrc[280]={0};
	int     MacSrcLen=0;
	int MacSrcPadLen = 0;
	MacSrc[0] = 0x84;
	MacSrc [1]= 0x2B;
	MacSrc [2]= 0x00;																// common verify pin mode
	MacSrc [3]= PinIndex;
	MacSrc [4]= plainAfterPadLen+4;
	
	memcpy(MacSrc+5,DataAfterEnc,plainAfterPadLen);

	MacSrc[plainAfterPadLen + 5]=0x80;
	// zc100 mac calculate
	MacSrcLen = plainAfterPadLen + 5;

	BYTE InitValue[8]={0};
	memcpy(InitValue,Challenge,4);


	BYTE MAC[280]={0};
	mac_3des(pReloadKey,InitValue,MacSrc,MAC,MacSrcLen);
	// after MAC , should delete the MacSrcPadLen

	memcpy(send,MacSrc,MacSrcLen);
	sendLen+=MacSrcLen;
	memcpy(send+sendLen,MAC,4);
	sendLen+=4;

	g_sw1sw2 = P_SendApduComm(icd,sendLen,send,&recvLen,recvBuf);
	return g_sw1sw2;
}



/*
this cmd for change the pin 
*/
 WORD KingCos_ChangePin(DEV_HANDLE icd,
					   unsigned char PID,
					   const BYTE *pNewPin,           // must not be modify this variable
					   ULONG NewPinLen,
					   const BYTE *pOldPin,
					   ULONG OldPinLen,
					   unsigned char SM
					   )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	int sendLen=0;
	BYTE recvBuf[MAX_PATH];
	BYTE plainAfterPad[32]={0};
	int plainAfterPadLen=0;

	BYTE EncryKey[16]={0};


	if(NewPinLen>16)
		NewPinLen=16;


	if(OldPinLen<16)
	{
		memcpy(EncryKey,pOldPin,OldPinLen); // use Old pin as encrypt Key
		EncryKey[OldPinLen]=0x80;
	}else
	{	
		memcpy(EncryKey,pOldPin,16);
	}
	BYTE Challenge[MAX_PATH] = {0};
	g_sw1sw2=KingCos_GetChallenge(icd,0x08,Challenge);
	if(g_sw1sw2!=0x9000)
		return g_sw1sw2;


	
	memcpy(plainAfterPad,Challenge,8);
	plainAfterPadLen+=8;
	memcpy(plainAfterPad+plainAfterPadLen,pNewPin,NewPinLen);
	plainAfterPadLen+=NewPinLen;
	plainAfterPad[plainAfterPadLen]=0x80;
	plainAfterPadLen++;
	plainAfterPadLen+=(8-(plainAfterPadLen%8));

	BYTE DataAfterEnc[280];
	TripleDesEnc(DataAfterEnc,plainAfterPad,plainAfterPadLen,EncryKey);

	BYTE MacSrc[280]={0};
	int     MacSrcLen=0;
	MacSrc[0] = 0x04;
	MacSrc [1]= 0x24;
	MacSrc [2]= 0x01;								// common verify pin mode
	MacSrc [3]= PID;
	MacSrc [4]= plainAfterPadLen+4; // need 4byte mac length
	memcpy(MacSrc+5,DataAfterEnc,plainAfterPadLen);
	

	MacSrc[plainAfterPadLen + 5]=0x80;
	MacSrcLen = plainAfterPadLen + 5;


	BYTE InitValue[8]={0};
	memcpy(InitValue,Challenge,4);

	BYTE MAC[280]={0};
	//mac_3des(MacKey,(BYTE*)"\x00\x00\x00\x00\x00\x00\x00\x00",MacSrc,MAC,MacSrcLen);
	mac_3des(EncryKey,InitValue,MacSrc,MAC,MacSrcLen);

	memcpy(send,MacSrc,MacSrcLen);
	sendLen+=MacSrcLen;
	memcpy(send+sendLen,MAC,4);
	sendLen+=4;

	g_sw1sw2 = P_SendApduComm(icd,sendLen,send,&recvLen,recvBuf);
	return g_sw1sw2;
 }