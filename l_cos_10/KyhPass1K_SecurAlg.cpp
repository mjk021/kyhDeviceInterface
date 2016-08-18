/***
* KyhPass1KSecurAlg.cpp:
*		definitions of apdu cmd api
* Purpose:
*       This file defines the structures, global values,and functions
*       used by KyhPass1KApdu.cpp
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-23 17:15:02
***/




#include "KyhPass1K_Define.h"
#include "KyhPass1K_Apdu.h"
#include "KyhPass1K_Glb.h"
#include "AlgDes1.h"
#include <base/reader.h>





/* 
this cmd for COS to Generate a key pair
*/

						
WORD KingCos_GenKeyPair(DEV_HANDLE icd,
						unsigned char Alg,								 // use which algorithm to generate key pair
						unsigned short PubFID,
						unsigned short PriFID
						)
{

	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = 0x80;
	g_apdu.ins = 0x89;
	g_apdu.p1 = Alg;													// use which algorithm to generate KeyPairs
	g_apdu.p2 = 0x00;											    // different SFI ,determine which type of KeyPairs  
	g_apdu.lc = 0x04; 
	g_apdu.sendbuf[0] = *((unsigned char *)&PubFID + 1);										 // PubFID[1]
	g_apdu.sendbuf[1] = *((unsigned char *)&PubFID + 0);										 // PubFID[0] 
	g_apdu.sendbuf[2] = *((unsigned char *)&PriFID + 1);										 // PriFID[1]
	g_apdu.sendbuf[3] = *((unsigned char *)&PriFID + 0);										 // PriFID[0] 

	//this cmd only transfer the data of send address
	// this cmd only have plain text transfer
	KingCos_BuildApdu(icd,&g_apdu,send,NULL,0,	APDU_MODEL_PLAIN,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	
	return g_sw1sw2;
}




/* 
this cmd for COS to Import  a key pair , and only for import Public Key
*/
WORD KingCos_ImportPubKey(DEV_HANDLE icd,
						unsigned short PubFID,															// now only for RSA file,not for SM2
						const unsigned char *Modulus,
						const unsigned char *Exponent,
						int KeyLen,																			    // public key length ,default is 1024
						BYTE CLA,
						BYTE SM,
						unsigned char *pDataKey,
						unsigned long DataKeyLen
						)
{
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = 0x84;
	g_apdu.ins = 0x8B;
	g_apdu.p1 = *((unsigned char *)&PubFID + 1);									// PubFID[1]
	g_apdu.p2 = *((unsigned char *)&PubFID + 0);									// PubFID[0] 

	g_apdu.lc = 1+1+4+1+1+128;
	
	g_apdu.sendbuf[0] = 0x45;																			// type of import element
	g_apdu.sendbuf[1] = 0x04;		
	g_apdu.sendbuf[2] = 0x00;	// the len of exponent
	memcpy(g_apdu.sendbuf + 3,Exponent,0x03);										// copy the exponent to the sendbuf
	g_apdu.sendbuf[6] = 0x4E;
	g_apdu.sendbuf[7] = 128;
	memcpy(g_apdu.sendbuf+1+1+4+1+1,Modulus,128);

	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);


	KingCos_BuildApdu(icd,&g_apdu,send,pDataKey,DataKeyLen,	SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}






/* 
this cmd for COS to Import  a key pair , and only for import Private Key
*/

WORD KingCos_ImportPriKey(DEV_HANDLE icd,
						  unsigned short PriFID,
						  const unsigned char *Prime1,
						  const unsigned char *Prime2,
						  const unsigned char *Exponent1,
						  const unsigned char *Exponent2,
						  const unsigned char *Coefficient,
						  const BYTE *pPriExponent,
						  const BYTE *pPubModulus,
						  const BYTE *pPubExponent,
						  BYTE CLA,
						  unsigned char SM,
						  BYTE *pRouteKey,
						  int RouteKeyLen
						  )
{
	
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = 0x94;
	g_apdu.ins = 0x8B;
	g_apdu.p1 = *((unsigned char *)&PriFID + 1);													// PriFID[1]
	g_apdu.p2 = *((unsigned char *)&PriFID + 0);													// PriFID[0] 

	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);
	
	/*
	the cos do not detect which position of the data
	*/
	int offset=0;
	g_apdu.sendbuf[offset] = 0x50;offset++;
	g_apdu.sendbuf[offset] = 64;offset++;
	memcpy( g_apdu.sendbuf+offset,Prime1,64 );offset+=64;

	g_apdu.sendbuf[offset] = 0x51;offset++;
	g_apdu.sendbuf[offset] = 64;offset++;
	memcpy( g_apdu.sendbuf+offset,Prime2,64 );offset+=64;

	g_apdu.lc = offset;
	KingCos_BuildApdu(icd,&g_apdu,send,pRouteKey,RouteKeyLen,APDU_MODEL_ENC_MAC,challenge);

	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	if(g_sw1sw2!=0x9000)
		return g_sw1sw2;

	KingCos_GetChallenge( icd,0x04,challenge);
	g_apdu.cla = 0x84;
	offset=0;
	g_apdu.sendbuf[offset] = 0x70;offset++;
	g_apdu.sendbuf[offset] = 64;offset++;
	memcpy( g_apdu.sendbuf+offset,Exponent1,64 );offset+=64;

	g_apdu.sendbuf[offset] = 0x71;offset++;
	g_apdu.sendbuf[offset] = 64;offset++;
	memcpy( g_apdu.sendbuf+offset,Exponent2,64 );offset+=64;

	g_apdu.sendbuf[offset] = 0x56;offset++;
	g_apdu.sendbuf[offset] = 64;offset++;
	memcpy( g_apdu.sendbuf+offset,Coefficient,64 );offset+=64;

	g_apdu.lc = offset;
	KingCos_BuildApdu(icd,&g_apdu,send,pRouteKey,RouteKeyLen,APDU_MODEL_ENC_MAC,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	if(g_sw1sw2!=0x9000)
		return g_sw1sw2;

	return g_sw1sw2;
}






/* 
this cmd for COS to export  a key pair , and only for export a Public Key,
and the Private Key is forbidden to export
*/
WORD KingCos_ExportPubKey(DEV_HANDLE icd,
						unsigned short PubFID,
						BYTE CLA,
						int KeyLen,
						unsigned char *pModulus,
						unsigned char *pExponent,
						BYTE SM
						)
{

	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = CLA;
	g_apdu.ins = 0x8A;
	g_apdu.p1 = *((unsigned char *)&PubFID + 1);										 // PriFID[1]
	g_apdu.p2 = *((unsigned char *)&PubFID + 0);										 // PriFID[0] 

	g_apdu.lc = 1+1;																								// E(1byte),N(1byte)
						

	g_apdu.sendbuf[0] = 0x45;																			// type of import element
	g_apdu.sendbuf[1] = 0x4E;																			// the len of exponent
	//  Exponent+Modulus =  flag+len+value + flag+len+value
	g_apdu.le = 1+1+4+1+1+128;


	unsigned char *pKey = NULL;
	unsigned char pKeyLen = 0;
	unsigned char challenge[8] = {0};
	unsigned char *random = NULL;


	//int detect;
	BYTE TempDataOutMac[280] = {0};
	BYTE TempDataOutPad[280] = {0};
	BYTE TempDataOutPadBlock[8] = {0};
	BYTE TempDataOut[280] = {0};

	BYTE TempDataOutBlock[8] = {0};
	unsigned long cnt = 0;
	int blockLen = 8;

	if ( APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		pKeyLen = 0;
		KingCos_GetChallenge( icd,0x04,challenge);
		random = challenge;
	}
	else if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = DATAKEY;
		pKeyLen = 0x10;
		KingCos_GetChallenge( icd,0x04,challenge);
		random = challenge;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,pKeyLen,SM,random);

	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	// get the data of PubKey from recvbuf
	// Exponent is on the first

		if(g_sw1sw2==0x9000)
		{
			memcpy(TempDataOutMac,g_apdu.recvbuf,recvLen);
			recvLen-= 4;
			memcpy(TempDataOutPad,TempDataOutMac,recvLen);
			for ( cnt = 0; cnt < recvLen/8; cnt++)
				{

					memcpy(TempDataOutPadBlock, TempDataOutPad+8*cnt, 8);
					// decrypt the output data , KEY is dataKey

					TripleDESDecipher((int *)&blockLen, TempDataOutPadBlock, TempDataOutBlock, DATAKEY);
					memcpy(TempDataOut + cnt*8, TempDataOutBlock, 8);

				}
			if (TLV_E == TempDataOut [0])
				{
					memcpy( pExponent,&TempDataOut[3],3);
					memcpy(pModulus,&TempDataOut[8],128);
				}
			// Modulus is on the first 
			else if (TLV_N == TempDataOut [0])
				{
					memcpy( pModulus,&TempDataOut[2],128);
					memcpy(pExponent,&TempDataOut[133],3);
				}
		}
	return g_sw1sw2;

}




/* 
this cmd for COS to use asymmetric Private Key to signature,
and can use the Private KEY to encrypt data
*/
WORD KingCos_RsaDigitalSig(DEV_HANDLE icd,
						unsigned short PriFID,									// use asymmmetric Private Key
						const unsigned char *pSigData, 
						unsigned short SigDataLen,
						unsigned char *pSignResult,
						unsigned char Alg,
						BYTE CLA
						)
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280]={0};
	g_apdu.cla=CLA;
	g_apdu.ins=0x8C;
	g_apdu.p1=*((unsigned char *)&PriFID+1);					// PriFID[1]
	g_apdu.p2=*((unsigned char *)&PriFID+0);					// PriFID[0]
	g_apdu.lc=(BYTE)SigDataLen;
	// list the data 
	memcpy(g_apdu.sendbuf , pSigData, SigDataLen);         // the Signature data of Rsa is 128bit

	KingCos_BuildApdu(icd,&g_apdu, send,NULL, 0,APDU_MODEL_PLAIN,NULL);

	g_sw1sw2=P_SendApduComm(icd,  g_apdu.lc+5,send,&recvLen, g_apdu.recvbuf);

	memcpy(pSignResult,g_apdu.recvbuf,SigDataLen);
	return g_sw1sw2;
}



/* 
this cmd for COS to use asymmetric Private Key to signature
*/

WORD KingCos_PriKeyDecrypt(DEV_HANDLE icd,
						unsigned short PriFID,									// use asymmetric Private Key
						const unsigned char *pEncryData, 
						unsigned long EncryDataLen,
						unsigned char *pPlainData,
						unsigned long *pDataOutLen,
						BYTE CLA
						)
{

	unsigned long recvLen=MAX_PATH;
	BYTE send[280]={0};
	g_apdu.cla=CLA;
	g_apdu.ins=0x8E;
	g_apdu.p1=*((unsigned char *)&PriFID+1);								// PriFID[1]
	g_apdu.p2=*((unsigned char *)&PriFID+0);								// PriFID[0]
	g_apdu.lc=(BYTE)EncryDataLen;


	// list the data 
	memcpy(g_apdu.sendbuf, pEncryData, EncryDataLen);         // the Signature data of Rsa is 128bit

	KingCos_BuildApdu(icd,&g_apdu, send,NULL, 0,APDU_MODEL_PLAIN,NULL);

	g_sw1sw2=P_SendApduComm(icd,  g_apdu.lc+5,send,&recvLen, g_apdu.recvbuf);

	if(pDataOutLen)*pDataOutLen=EncryDataLen;
	memcpy(pPlainData,g_apdu.recvbuf,EncryDataLen);
	return g_sw1sw2;
}


WORD KingCos_SymmetricAlg_Calc
                           (DEV_HANDLE icd,
	                       BOOL isEnc,
						   BYTE algId,
						   const BYTE* pDataIn,			
						   ULONG ulDataInLen,					
					       BYTE* pDataOut,					
					       ULONG* pulDataOutLen	
						   )
{

	unsigned long recvLen=MAX_PATH;
	BYTE send[280]={0};
	g_apdu.cla= 0x80;
	g_apdu.ins = 0xF1;
	
	g_apdu.p1= 0xC0;
	if(isEnc)
	{
		g_apdu.p1= 0x40;
	}								
	g_apdu.p2= algId;
	g_apdu.lc = (BYTE)ulDataInLen;
	memcpy(g_apdu.sendbuf,pDataIn,(BYTE)ulDataInLen);

	KingCos_BuildApdu(icd,&g_apdu, send,NULL, 0,APDU_MODEL_PLAIN,NULL);

	g_sw1sw2=P_SendApduComm(icd,  g_apdu.lc+5,send,&recvLen, g_apdu.recvbuf);

	if(pulDataOutLen) *pulDataOutLen = recvLen;

	if(pDataOut) 
	{
		memcpy(pDataOut, g_apdu.recvbuf, recvLen);
	}
	return g_sw1sw2;
}


