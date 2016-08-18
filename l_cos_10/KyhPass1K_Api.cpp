/***
* KyhPass1KApi.cpp
*
* Purpose:
*       This file defines the api of apdu for CSP to call,
* and wrapped the api of KyhPass1KCos.cpp
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-22 16:37:21
***/


// import the api of KyhPass1KCos.cpp

#include "KyhPass1K_Define.h"
#include "KyhPass1K_Glb.h"
#include "KyhPass1K_Apdu.h"
#include  <base/reader.h>
#include  <base/cos.h>
#include "AlgDes.h"

struct S_Init{
	ULONG handle;
	bool init;
};


BOOL Z_Initialize(void *handle)
{
	return TRUE;
}

void Z_Finalize()
{
}


ULONG Z_GetCosType(DEV_HANDLE devHandle)
{
	return 5;
}

#define SM4_ID  0x00
DEV_RES Z_SM4Calc(DEV_HANDLE devHandle,BOOL isEnc,BYTE* key,BYTE*pDataIn,ULONG ulDataInLen,BYTE*pDataOut,ULONG* pulDataOutLen)
{
     BYTE algId = SM4_ID;
     g_SM = APDU_MODEL_ENC_MAC;
     g_sw1sw2 = KingCos_UpdateKey(devHandle, 0x0b, algId, 0x10, (BYTE *)key, MF_MKey, 0x10, g_SM);
     if(0x9000 != g_sw1sw2)
          return g_sw1sw2;
     g_sw1sw2 = KingCos_SymmetricAlg_Calc(devHandle, isEnc, algId, (BYTE *)pDataIn, ulDataInLen, pDataOut, pulDataOutLen);
     if(0x9000 != g_sw1sw2)
          return g_sw1sw2;
     return g_sw1sw2;  
}

void convert(BYTE* pData ,WORD len)
{
	int i = 0;
	BYTE tmp = 0;

	if(len % 4) return;

	i = 0;
	while(i < len)
	{
		tmp = pData[i];
		pData[i] = pData[i + 3];
		pData[i + 3] = tmp;
		tmp = pData[i + 1];
		pData[i + 1] = pData[i + 2];
		pData[i + 2] = tmp;
		i += 4;
	}
}
DEV_RES Z_LoadSymmetricKey(
    DEV_HANDLE devHandle, 
    BYTE alg, 
    BYTE *key,
    const BYTE *pAdminPin, 
    ULONG AdminPinLen
)
{
    unsigned short res;
    BYTE apdu[265];
    BYTE rdata[300];
    unsigned long revlen = 0;
    
    BYTE Challenge[100] = {0};
    BYTE adminKey[100] = {0};
	// get the challenge for InitValue
	res=KingCos_GetChallenge(devHandle,0x04,Challenge);
	if(res!=0x9000)
		return res;
		
	memcpy(adminKey, pAdminPin, AdminPinLen);
	adminKey[AdminPinLen]=0x80;

	memset(apdu, 0x0, 260);
	memcpy(apdu, "\x84\x40\x03\x00\x10",5);
	apdu[2] = alg;
	apdu[4] = 32 + 4;
	if(NULL == key) return 0x6985;
	memcpy(apdu + 5, key, 16);
    convert(apdu + 5, 16);
    apdu[5 + 16] = 0x80;
    TripleDesEnc(apdu + 5,apdu + 5, 32,adminKey);
    
    BYTE MAC[280]={0};
    BYTE InitValue[8]={0};
	memcpy(InitValue,Challenge,4);
	mac_3des(adminKey,InitValue,apdu,MAC,5 + 32);
	memcpy(apdu + 5 + 32,MAC,4);
	return P_SendApduComm(devHandle, 5 + apdu[4], (BYTE*)apdu, &revlen, rdata); 
}

DEV_RES Z_SymmetricAlgCalc(
    DEV_HANDLE devHandle, 
    bool isEnc,
    BYTE alg, 
    BYTE mode, 
    BYTE *pIv, 
    BYTE *pDataIn, 
    ULONG ulDataInLen,
    BYTE*pDataOut,
    ULONG* pulDataOutLen
    )
{
    unsigned short res;
    unsigned long revlen = MAX_PATH;
    unsigned long dataLen; 
    BYTE apdu[1600];
    BYTE rdata[1600];
	
	memset(apdu, 0x0, sizeof(apdu));
	memcpy(apdu, "\x80\x50\x03\x00\x00",5);
	apdu[2] = alg;
	if(ulDataInLen > 1489) return 0x6985;
        if(!isEnc) apdu[3] = 1;
	 
	if(ECB_MODE == mode)
	{
        dataLen = 1 + ulDataInLen;
        memcpy(apdu + 8, pDataIn, ulDataInLen);	    
	}
	else if(CBC_MODE == mode)
	{
        dataLen = 1 + 16 + ulDataInLen;
        memcpy(apdu + 8,pIv ,16); 
        memcpy(apdu + 8 + 16, pDataIn, ulDataInLen);
	}
	else
	{
	    return 0x6985;
	}
    apdu[5] = (dataLen>>8) & 0xff;
    apdu[6] = dataLen & 0xff;
    apdu[7] = mode;
    convert(apdu + 8, dataLen - 1);
    res = P_SendApduComm(devHandle, 5 + 2 + dataLen, (BYTE*)apdu, &revlen, rdata);
	if(res !=0x9000)
		return res;
	
	revlen = ulDataInLen;
	memcpy(apdu, "\x00\x25\x00\x00\x00",5);
	apdu[2] = (ulDataInLen>>8) & 0xff;
	apdu[3] = ulDataInLen & 0xff;
	res = P_SendApduComm(devHandle, 5, (BYTE*)apdu, &revlen, rdata);
	if(res != 0x9000){
		return res;
	}
	if(revlen != ulDataInLen) 
	    return 0x6985;
	*pulDataOutLen = revlen;
	convert(rdata, revlen);
	memcpy(pDataOut,rdata,revlen);
	return 0x9000;
}

DEV_RES Z_Test(
    DEV_HANDLE devHandle, 
    bool isEnc,
    BYTE alg, 
    BYTE mode, 
    BYTE *pIv, 
    BYTE *pDataIn, 
    ULONG ulDataInLen,
    BYTE*pDataOut,
    ULONG* pulDataOutLen
    )
{
    unsigned short res;
    unsigned long revlen = MAX_PATH;
    unsigned long dataLen; 
    BYTE apdu[1600];
    BYTE rdata[1600];
	
	memset(apdu, 0x0, sizeof(apdu));
	memcpy(apdu, "\x80\x51\x03\x00\x00",5);
	apdu[2] = alg;
	if(ulDataInLen > 1489) return 0x6985;
        if(!isEnc) apdu[3] = 1;
	 
	if(ECB_MODE == mode)
	{
        dataLen = 1 + ulDataInLen;
        memcpy(apdu + 8, pDataIn, ulDataInLen);	    
	}
	else if(CBC_MODE == mode)
	{
        dataLen = 1 + 16 + ulDataInLen;
        memcpy(apdu + 8,pIv ,16); 
        memcpy(apdu + 8 + 16, pDataIn, ulDataInLen);
	}
	else
	{
	    return 0x6985;
	}
    apdu[5] = (dataLen>>8) & 0xff;
    apdu[6] = dataLen & 0xff;
    apdu[7] = mode;
    convert(apdu + 8, dataLen - 1);
    res = P_SendApduComm(devHandle, 5 + 3 + dataLen, (BYTE*)apdu, &revlen, rdata);
	if(res !=0x9000)
		return res;
	
	revlen = ulDataInLen;
	memcpy(apdu, "\x00\x25\x00\x00\x00",5);
	apdu[2] = (ulDataInLen>>8) & 0xff;
	apdu[3] = ulDataInLen & 0xff;
	res = P_SendApduComm(devHandle, 5, (BYTE*)apdu, &revlen, rdata);
	if(res != 0x9000){
		return res;
	}
	if(revlen != ulDataInLen) 
	    return 0x6985;
	*pulDataOutLen = revlen;
	convert(rdata, revlen);
	memcpy(pDataOut,rdata,revlen);
	return 0x9000;
}	
			


DEV_RES Z_CreateFileSys(DEV_HANDLE devHandle, const BYTE *pAdminPin, ULONG AdminPinLen)
{
	g_SM = APDU_MODEL_ENC_MAC;
	g_sw1sw2 = KingCos_EraseMF(devHandle);
	if(0x9000 != g_sw1sw2)
	    return g_sw1sw2;
	g_sw1sw2 =  KingCos_InitDevice(devHandle, ISSUE_KEY, 0x10);


	g_sw1sw2 =  KingCos_CreateDF(devHandle, 0x3F00, 0x30, 0x00, 0x07, 0x07, 0x07, NULL, 0x0, ISSUE_KEY, 0x10, 0x84, g_SM);
	//because on every DF directory , must have a Key file , so at CreateDF,we should Create Key file on its directory
	if(0x9000 != g_sw1sw2)
		return g_sw1sw2;
	// Create a Key file on the MF
	g_sw1sw2 = KingCos_CreateKeyFile( devHandle, KEY_FID, FILETYPE_KEY, 0x00, 0x1410, 0x00, 0x05, g_SM, ISSUE_KEY, 0x10, 0x84 );
	if(0x9000 != g_sw1sw2)
		return g_sw1sw2;
	// Load a MKey in the Key file on the MF
	g_sw1sw2 =  KingCos_LoadKey( devHandle, TYPE_MKEY, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, MF_MKey, ISSUE_KEY, 0x10, g_SM );
	if(0x9000 != g_sw1sw2)
		return g_sw1sw2;
	// load a DataKey in the Key file on the MF
	g_sw1sw2 =  KingCos_LoadKey( devHandle, TYPE_DATA_MT, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, DATAKEY, MF_MKey, 0x10, g_SM );
	if (0x9000 != g_sw1sw2)
		return g_sw1sw2;
	// load a ReloadKey in the Key file on the MF
	g_sw1sw2 =  KingCos_LoadKey( devHandle, TYPE_PIN_UNBLOCK_RELOAD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, RELOAD_KEY, MF_MKey, 0x10, g_SM );
	if (0x9000 != g_sw1sw2)
		return g_sw1sw2;
    //load SymmetricAlg 
    g_sw1sw2 =  KingCos_LoadKey( devHandle, 0x0b, SM4_ID, SM4_ALG, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, (BYTE *)"2222222222222222", MF_MKey, 0x10, g_SM );
	if (0x9000 != g_sw1sw2)
		return g_sw1sw2;
	g_sw1sw2 =  KingCos_LoadPin( devHandle, TYPE_PIN_CODE, PID_ADMIN, PIN_RAND_DES_3DES, 0x00, 0x0f, 0x66, 0x06, 0x77,AdminPinLen,pAdminPin,MF_MKey,0x10,g_SM);		
    if (0x9000 != g_sw1sw2)
        return g_sw1sw2;
	return g_sw1sw2;  
}


DEV_RES Z_CreateEF(DEV_HANDLE devHandle,
					ULONG FType,
					WORD FileId,
					ULONG Size,
					ULONG Right
					)
{
	/* 
	this variable is a switch to determine 
	which type of the cmd to transfer
	*/ 
#define FILE_TYPE_BIN			1
#define FILE_TYPE_PRI_KEY	2
#define FILE_TYPE_PUB_KEY	3

	g_SM = APDU_MODEL_ENC_MAC;
	unsigned char cla = 0x00;
	BYTE ReadAC = 0x0f;
	BYTE WriteAC = 0x0f;

	if ( APDU_MODEL_ENC_MAC == g_SM)
		cla = 0x84;
	else if ( APDU_MODEL_PLAIN == g_SM)
		cla = 0x80;

	// read AC
	if((Right&RIGHT_READ_USER_PIN)||(Right&RIGHT_USE_USER_PIN))
		ReadAC = USER_PIN_RIGHT_STATUS;
	else if (Right&RIGHT_READ_FORBIDDEN)
		ReadAC = 0xFE;
	// write AC
	if(Right&RIGHT_WRITE_USER_PIN)
		WriteAC = USER_PIN_RIGHT_STATUS;


	switch(FType)
	{
	case FILETYPE_KEY:
		return KingCos_CreateKeyFile(devHandle,FileId,FType,0x00,0x1410,ReadAC,WriteAC,g_SM,MF_MKey,0x10,cla);
	case FILE_TYPE_PUB_KEY:
		return	KingCos_CreatePubKeyFile(devHandle, FileId, FILETYPE_PUBKEY_RSA1024, 0x00, 0x0000, ReadAC, WriteAC, g_SM, MF_MKey, 0x10,cla );
	case FILE_TYPE_PRI_KEY:
		return	KingCos_CreatePriKeyFile(devHandle, FileId, FILETYPE_PRIKEY_RSA1024, 0x00, 0x0000, ReadAC, WriteAC, g_SM, MF_MKey, 0x10,cla);
		// FILETYPE_BIN | FILETYPE_VAR_RECORD
	case FILE_TYPE_BIN: 
		return	KingCos_CreateBin( devHandle, FileId,FILETYPE_BIN , 0x00, Size, ReadAC, WriteAC, g_SM, MF_MKey, 0x10, cla);
		// case FILE_TYPE_VAR_RECORD , RFU
	default:
		/*do not have this default type file on the EF, so this cmd is invalid*/
		return 0x6901;                           
	}
}


DEV_RES Z_SelectMF(DEV_HANDLE devHandle)
{
	return KingCos_SelectMF(devHandle);
}


DEV_RES Z_SelectDF(DEV_HANDLE devHandle,
						   WORD FID)
{
	return KingCos_SelectDF(devHandle,FID);
}

DEV_RES Z_DeleteDF(DEV_HANDLE devHandle,
						   WORD fileId
						   )
{
	return KingCos_DeleteDF(devHandle,fileId);
}


DEV_RES Z_DeleteEF(DEV_HANDLE devHandle,
						   WORD fileId
						   )
{
	return KingCos_DeleteEF(devHandle,fileId);
}


/*
this api is for InitPin in the MF
*/
DEV_RES Z_InitPin(DEV_HANDLE devHandle,
						  ULONG nReTry,
						  const BYTE *pUserPin,
						  ULONG UserPinLen,
						  const BYTE *pAdminPin,
						  ULONG AdminPinLen
						  )
{
    g_SM = APDU_MODEL_ENC_MAC;
    // first load the Administrator pin
    // UsageAC = 0x00 ,because first use user pin to verify pin 
    // UpdateAC =0xfe,X>Y ,the admin pin is forbidden to update
    g_sw1sw2 =  KingCos_LoadPin( devHandle, TYPE_PIN_CODE, PID_ADMIN, PIN_RAND_DES_3DES, 0x00, 0x0f, 0x66, 0x06, 0x77,AdminPinLen,pAdminPin,MF_MKey,0x10,g_SM);		
    if (0x9000 != g_sw1sw2)
        return g_sw1sw2;
    // second load User pin
    // UpdateAC = 0x05 < RFUStatus(admin pin) = 0x07, so use admin pin to update user pin
    g_sw1sw2 =  KingCos_LoadPin( devHandle, TYPE_PIN_CODE, PID_USER, PIN_RAND_DES_3DES,0x00, 0x0f, 0x56, 0x05,0x77,UserPinLen,pUserPin,MF_MKey, 0x10, g_SM );
    // the final return value ,when load the userPIN
    if (0x9000 != g_sw1sw2)
        return g_sw1sw2;
    
    ULONG retLen=0;
    BYTE ret[MAX_PATH];
    g_sw1sw2=P_SendApduComm(devHandle,5,(BYTE*)"\x80\x13\x00\x00\x00",&retLen,ret);
    return g_sw1sw2;
}


DEV_RES Z_VerifyPin(DEV_HANDLE devHandle,
							ULONG usertype,							 // the user Pin or administrator Pin
							const unsigned char *pCodeValue,
							ULONG CodeLen
							)
{
	//(TYPE_USER_PIN == usertype) , (TYPE_ADMIN_PIN == usertype) 
	return KingCos_VerifyPin( devHandle, usertype, pCodeValue, CodeLen );
}



DEV_RES Z_GetChallenge(DEV_HANDLE icd,
							   unsigned char Number,                      // input number
							   unsigned char *pChallenge               // output receive dataBuffer
							   )
{
	return KingCos_GetChallenge(icd,Number,pChallenge);
}



DEV_RES Z_GenKeyPair(DEV_HANDLE devHandle,
							 unsigned short PriKeyId,
							 unsigned short PubKeyId
							 )
{
	g_ALG = ALG_RSA1024;
	return KingCos_GenKeyPair( devHandle, g_ALG, PubKeyId, PriKeyId );
}



DEV_RES Z_WritePubKey(DEV_HANDLE devHandle,
					   WORD FileId,
					   S_KeyBase *pKey
					   )
{
	g_SM = APDU_MODEL_ENC_MAC;
	BYTE *pRouteKey=NULL;
	S_RsaPubKey1024 *pPubKey=(S_RsaPubKey1024 *)pKey;
	int KeyLen=0;
	if (APDU_MODEL_PLAIN == g_SM)
	{
		g_CLA = 0x90;
		pRouteKey = NULL;				 // give the DataKey array address to pointer 
		KeyLen = 0x00;
	}
	else if (APDU_MODEL_ENC_MAC == g_SM)
	{
		g_CLA = 0x94;
		pRouteKey = DATAKEY;		 // give the DataKey array address to pointer 
		KeyLen = 0x10;
	}
	return KingCos_ImportPubKey( devHandle, FileId, pPubKey->Modulus, pPubKey->Exponent, 1024,	g_CLA, g_SM, pRouteKey, KeyLen );
}


DEV_RES Z_WritePriKey(DEV_HANDLE devHandle,
							  WORD FileId,
								S_KeyBase *pKey
							  )
{
	g_SM = APDU_MODEL_ENC_MAC;
	BYTE *pRouteKey=NULL;
	int KeyLen=0;
	S_RsaPriKey1024 *pPriKey=(S_RsaPriKey1024 *)pKey;
	if (APDU_MODEL_ENC_MAC == g_SM)
	{
		g_CLA = 0x94;
		pRouteKey = DATAKEY;	 // give the DataKey array address to pointer 
		KeyLen = 0x10;
	}
	if (APDU_MODEL_PLAIN == g_SM)
	{
		g_CLA = 0x90;
		pRouteKey = NULL;			// give the DataKey array address to pointer 
		KeyLen = 0x00;
	}
	return KingCos_ImportPriKey( devHandle, FileId, pPriKey->Prime1, pPriKey->Prime2, pPriKey->Exponent1, pPriKey->Exponent2, pPriKey->Coefficient,pPriKey->PriExponent,pPriKey->PubModulus,pPriKey->PubExponent, g_CLA, g_SM, pRouteKey, KeyLen );
}





DEV_RES Z_ReadPubKey(DEV_HANDLE devHandle,
							 WORD FileId,
							 S_KeyBase *pKey
							 )
{
	g_SM = APDU_MODEL_PLAIN;
	S_RsaPubKey1024 *pPubKey=(S_RsaPubKey1024*)pKey;
	pPubKey->keyLen=1024;
	pPubKey->keyType=CKO_PUBLIC_KEY;
	unsigned char cla = 0x80;
	if(g_SM==APDU_MODEL_ENC_MAC)
		cla = 0x84;
	return KingCos_ExportPubKey(devHandle, FileId, cla,1024, pPubKey->Modulus, pPubKey->Exponent, g_SM );
}




DEV_RES Z_KeyEncrypt(DEV_HANDLE devHandle,
						   WORD PriKeyId,
						   const BYTE *pDataIn,
						   ULONG DataInLen,
						   BYTE *pDataOut,
						   ULONG *pDataOutLen
						   )
{
	g_ALG =	ALG_SHA1; 
	BYTE Temp[128];
	BYTE cla = 0x80;
	if(pDataOutLen)*pDataOutLen=128;
	if(DataInLen>=128)
	{
		memcpy(Temp,pDataIn,DataInLen);
	}else if(DataInLen<128-4)
	{
		Temp[0]=0x00;
		Temp[1]=0x01;
		unsigned short i=0;
		for(i=2;i<128-DataInLen-1;i++)
			Temp[i]=0xFF;
		Temp[i++]=0x00;
		memcpy(Temp+i,pDataIn,DataInLen);
	}else
	{
		return 0x6901;
	}
	return KingCos_RsaDigitalSig(devHandle, PriKeyId, Temp, 128, pDataOut, g_ALG, cla );
}



DEV_RES Z_KeyDecrypt(DEV_HANDLE devHandle,
						   WORD PriKeyId,
						   const BYTE *pDataIn,
						   ULONG DataInLen,
						   BYTE *pDataOut,
						    ULONG *pDataOutLen
						   )
{
	BYTE cla = 0x80;
	return KingCos_PriKeyDecrypt( devHandle, PriKeyId, pDataIn, DataInLen, pDataOut,pDataOutLen, cla );
}


DEV_RES Z_UnblockPin(DEV_HANDLE devHandle,
					  const BYTE *pAdmPin,		// use administrator pin to unblock the user pin
					  ULONG AdmPinLen,
					  const BYTE *pUserPin,		// new user pin
					  ULONG UserPinLen
					  )
			
{
	g_sw1sw2 = Z_VerifyPin(devHandle,TYPE_ADMIN_PIN,pAdmPin,AdmPinLen);
	if(0x9000 != g_sw1sw2 )
		return g_sw1sw2;
	g_sw1sw2 = KingCos_ReloadPin(devHandle,PID_USER,pUserPin,UserPinLen,RELOAD_KEY,0x10);
	return g_sw1sw2;
}


DEV_RES Z_ReadBin(DEV_HANDLE devHandle,
				   WORD fileId,
				   ULONG Offset,      // use the offset 1byte , not 2bytes
				   ULONG DataLen,
				   BYTE *pDataOut							
				   )
{
	g_SM = APDU_MODEL_PLAIN;
	BYTE *pKey = NULL;
	BYTE KeyLen = 0;
	BYTE cla = 0;
	g_sw1sw2 = KingCos_SelectEF(devHandle,fileId);
	if (0x9000 != g_sw1sw2)
		return g_sw1sw2;
	if (APDU_MODEL_PLAIN == g_SM)
	{
		pKey = NULL;
		KeyLen = 0;
		cla = 0x00;
	}
	else if (APDU_MODEL_PLAIN_MAC == g_SM)
	{
		pKey = DATAKEY;
		KeyLen = 0x10;
		cla = 0x00;//only have 0x00
	}
	
	return KingCos_ReadBinary( devHandle, Offset, DataLen, pDataOut, g_SM, cla, pKey, KeyLen );
}

DEV_RES Z_UpdateBin(DEV_HANDLE devHandle,
					 WORD FileId,
					 ULONG Offset,
					 ULONG DataLen,
					 const BYTE *pDataIn																	  // input data
					 )
{
	g_SM = APDU_MODEL_ENC_MAC;
	BYTE *pKey = NULL;
	BYTE KeyLen = 0;
	BYTE cla = 0;
	if (APDU_MODEL_ENC_MAC == g_SM)
	{
		pKey = DATAKEY;
		KeyLen = 0x10;
		cla = 0x04;
	}	
	else if (APDU_MODEL_PLAIN == g_SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
		cla = 0x00;
	}

	switch(g_SM)
	{
		case APDU_MODEL_ENC_MAC:
			g_sw1sw2 = KingCos_SelectEF(devHandle,FileId);
			if (0x9000 != g_sw1sw2)
				return g_sw1sw2;
			g_sw1sw2 = KingCos_UpdateBinary( devHandle, Offset, DataLen, pDataIn, cla, g_SM, pKey, KeyLen );
				return g_sw1sw2;
		case APDU_MODEL_PLAIN:
			g_sw1sw2 = KingCos_SelectEF(devHandle,FileId);
			if (0x9000 != g_sw1sw2)
				return g_sw1sw2;
			g_sw1sw2 =  KingCos_UpdateBinary(devHandle, Offset, DataLen, pDataIn, cla, g_SM, NULL, 0x00 );
			return g_sw1sw2;
		default:
			return 0x6901;
	}
}


DEV_RES Z_ChangePin(DEV_HANDLE devHandle,
					 ULONG usertype,
					 const BYTE *pOldPin,
					 ULONG OldPinLen,
					 const BYTE *pNewPin,
					 ULONG NewPinLen
					 )
{
	g_SM = APDU_MODEL_ENC_MAC;
	unsigned char PID;
	// first write the unknown variable
	if (TYPE_USER_PIN == usertype)
		PID = PID_USER;
	else if (TYPE_ADMIN_PIN == usertype)
		PID = PID_ADMIN;
	return KingCos_ChangePin( devHandle, PID, pNewPin, NewPinLen,pOldPin ,OldPinLen,g_SM );
}



// the cmd below is RFU

DEV_RES Z_ClearDevice(DEV_HANDLE devHandle)
{
     return 0x6901;
}



DEV_RES Z_CreateMF(DEV_HANDLE devHandle)
{
	// use the plain text to transfer data
	g_SM = APDU_MODEL_ENC_MAC;
	if (APDU_MODEL_ENC_MAC == g_SM)
	{
		g_CLA = 0x84;
	}

	else if (APDU_MODEL_PLAIN == g_SM)
	{
		g_CLA = 0x80;
	}
	return KingCos_CreateMF(devHandle,0x30,0x00,0x07,0x07,0x07,g_CLA,g_SM);
}


DEV_RES Z_SelectEF(DEV_HANDLE devHandle,
						   WORD FID
						   )
{
	return KingCos_SelectEF(devHandle,FID);
}		  

