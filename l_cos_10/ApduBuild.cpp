/***
* ApduBuild.cpp:
*		 basic function for KingCos to call	
* Purpose:
*       definitions of basic apdu function of different algorithm mode
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-23 17:41:24
***/




#include "KyhPass1K_Apdu.h"
#include "KyhPass1K_Define.h"
#include "AlgDes.h"
#include "KyhPass1K_Glb.h"
#include  <base/reader.h>



/*
Create the different apdu function model that whether
use the key to calculate the mac for instance such as :
										                             1. APDU_MODEL_PLAIN
										                             2. APDU_MODEL_PLAIN_MAC
										                             3. APDU_MODEL_ENC_MAC
										                             4. APDU_MODEL_ENC
*/
int KingCos_BuildApdu(DEV_HANDLE icd,
					   APDUSt *papdu,
					   unsigned char *sendCmd,
					   unsigned char *key,
					   int kLen,
					   unsigned char iType,
					   unsigned char *random // basic random as first 4byte InitValue for mac calculate 
					   )
{
	DWORD len = 0;
	
	unsigned char InitValude[8] = {0};
	unsigned char EncryptData[280] = {0};
	unsigned char padLen = 0;
	unsigned char macSrc[280] = {0};
	unsigned char mac[4] = {0};
	int macSrcLen = 0;
	unsigned char plainLen = 0;
	unsigned char encryptLen = 0;
	unsigned char sendDataLen = 0;


	memset(sendCmd,0,256);
	
	sendCmd[0] = papdu->cla;
	sendCmd[1] = papdu->ins;
	sendCmd[2] = papdu->p1;
	sendCmd[3] = papdu->p2;
	switch(iType)
	{
	// plain text
	case APDU_MODEL_PLAIN:
		//APDU struct have the lc ,data
		if (papdu->lc != 0)
			{
				sendCmd[4] = papdu->lc;
				//copy the data of APDU body
				memcpy(sendCmd+5,papdu->sendbuf,papdu->lc);
			}
		//explain why copy the "\x00\x84\x00\x00\x08" to the &8bRand is correct
		if (papdu->le != 0)
			{
				if (0 == papdu->lc)
					*(sendCmd+4) = papdu->le;
				else 
					// cla + ins + p1 + p2 + lc + data
					*(sendCmd+4+1+papdu->lc) = papdu->le;
				//function exited abnormally
				break;
				//return 5;
			}

		//else 
		memcpy(papdu,sendCmd,sendCmd[4]+5);
		break;
		//return sendCmd[4] + 5;
		// encrypt ,this case for External Authority 


	case APDU_MODEL_ENC:
		//APDU struct have the lc ,data
		if (papdu->lc != 0)
		{
			sendCmd[4] = papdu->lc;
			//copy the data of APDU body
			memcpy(sendCmd+5,papdu->sendbuf,papdu->lc);
		}
		//explain why copy the "\x00\x84\x00\x00\x08" to the &bRand is correct
		if (papdu->le != 0)
		{
			if (0 == papdu->lc)
				*(sendCmd+4) = papdu->le;
			else 
				// cla + ins + p1 + p2 + lc + data
				*(sendCmd+4+1+papdu->lc) = papdu->le;
		}
		// before encrypt data should pad the source data
		padLen =  8-(papdu->lc%8);
		encryptLen = papdu->lc + padLen;
		memcpy(sendCmd+5+papdu->lc,"\x80",1);

		if (kLen == 8)
			DesEnc(EncryptData,sendCmd+5,encryptLen,key);
		if (kLen == 16)
			//TripleDesEnc(sendCmd+5,sendCmd+4,papdu->lc+1,key);
			TripleDesEnc(EncryptData,sendCmd+5,encryptLen,key);
		sendCmd[4] = encryptLen; 

		memcpy(sendCmd+5,EncryptData,encryptLen);
		papdu->lc = encryptLen;
		return papdu->lc;



	// plainText + Mac
	case APDU_MODEL_PLAIN_MAC:
		//APDU struct have the lc ,data
		if (papdu->lc != 0)
		{
			sendCmd[4] = papdu->lc;
			//copy the data of APDU body
			memcpy(sendCmd+5,papdu->sendbuf,papdu->lc);
			plainLen = papdu->lc;
		}
		else
		{
			*(sendCmd+4) = papdu->le;
			plainLen = 0;
		}

		macSrcLen = plainLen +  4 + 1;
		//memcpy(sendCmd+5+papdu->lc,"\x80",1);

		memcpy(macSrc,sendCmd,4);
		macSrc[4] = plainLen + 4;										
	

		// get InitValue from out random
		memcpy(InitValude,random,4);
		if (kLen == 8)
			mac_des(key,InitValude,macSrc,mac,macSrcLen);
		if (kLen == 16)
			mac_3des(key,InitValude,macSrc,mac,macSrcLen);


		sendDataLen = 4;

		//papdu->lc = sendDataLen;

		//memcpy(sendCmd+5,EncryptData,encryptLen);
		memcpy(sendCmd+5,mac,4);

		//memcpy(papdu->sendbuf,sendCmd,sendDataLen);
		sendCmd[4] = sendDataLen;
		//sendCmd[9] = papdu->le;

		// for outer function to call
		papdu->lc = sendDataLen;
		return 0;



	// encryptData + Mac
	case APDU_MODEL_ENC_MAC:
		//APDU struct have the lc ,data
		if (papdu->lc != 0)
			{
				sendCmd[4] = papdu->lc;
				//copy the data of APDU body
				memcpy(sendCmd+5,papdu->sendbuf,papdu->lc);
			}
		//explain why copy the "\x00\x84\x00\x00\x08" to the &8bRand is correct
		if (papdu->le != 0)
		{
			if (0 == papdu->lc)
				*(sendCmd+4) = papdu->le;
			else 
				// cla + ins + p1 + p2 + lc + data
				*(sendCmd+4+1+papdu->lc) = papdu->le;
		}

		// before encrypt data should pad the source data
		padLen =  8-(papdu->lc%8);
		encryptLen = papdu->lc + padLen;
		macSrcLen = encryptLen + 4 + 1;
		memcpy(sendCmd+5+papdu->lc,"\x80",1);

		if (kLen == 8)
			DesEnc(EncryptData,sendCmd+5,encryptLen,key);
		if (kLen == 16)
			TripleDesEnc(EncryptData,sendCmd+5,encryptLen,key);

		// pad length for mac calculate
		//sendCmd[4] = papdu->lc += 8-(papdu->lc%8);
		memcpy(macSrc,sendCmd,4);
		macSrc[4] = encryptLen+4;												// mac is 0x24
		memcpy(macSrc+5,EncryptData,encryptLen);
		//macSrc[4+encryptLen] = 0x80;

		// get InitValue from out random
		memcpy(InitValude,random,4);
		if (kLen == 8)
			mac_des(key,InitValude,macSrc,mac,macSrcLen);
		if (kLen == 16)
			mac_3des(key,InitValude,macSrc,mac,macSrcLen);
		

		sendDataLen = encryptLen + 4;

		//papdu->lc = sendDataLen;

		memcpy(sendCmd+5,EncryptData,encryptLen);
		memcpy(sendCmd+5+encryptLen,mac,4);

		//memcpy(papdu->sendbuf,sendCmd,sendDataLen);
		sendCmd[4] = sendDataLen;

		// for outer function to call
		papdu->lc = sendDataLen;
		return papdu->lc;

	default:
		break;
	}
	/* 
	not for transfer data  , just use the sendbuf of sendCmd 's address
	for P_SendApduComm to call
	*/
	return 0;
}
