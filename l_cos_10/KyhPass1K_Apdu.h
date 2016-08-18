/***
* KyhPass1K_Apdu.h
*	 
* Purpose:
*       This file defines the user-defined data type ,
*  and the basic api for KyhPass1K_Api.cpp to call
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-19 11:05:02
***/

#ifndef __KyhPass1K_APDU__H
#define __KyhPass1K_APDU__H

#include <base/typedefine.h>


// usb key basic data definition


//define the struct of the APDU cmd
typedef struct APDUSt
{
	BYTE cla;
	BYTE ins;
	BYTE p1;
	BYTE p2;
	BYTE lc;
	BYTE sendbuf[MAX_PATH];
	BYTE le;
	BYTE recvbuf[MAX_PATH];
} APDUSt ;



WORD KingCos_EraseMF(DEV_HANDLE icd);


WORD KingCos_UpdateKey(DEV_HANDLE icd,
					 BYTE KeyUsage,
					 BYTE KeyIndex,
					 BYTE KeyLen,
					 unsigned char *pKey,
					 unsigned char *pMKey,
					 unsigned char MKeyLen,
					 unsigned char SM);

WORD KingCos_SymmetricAlg_Calc
                           (DEV_HANDLE devHandle,
	                    BOOL isEnc,
			    BYTE algId,
			    const BYTE* pDataIn,			
		            ULONG ulDataInLen,					
			    BYTE* pDataOut,					
			    ULONG* pulDataOutLen);


// the declaration of KyhPass1KCos.cpp api
int KingCos_BuildApdu(DEV_HANDLE icd,
					  APDUSt *papdu,
					  unsigned char *sendCmd,
					  unsigned char *key,
					  int kLen,
					  unsigned char iType,
					  unsigned char *random // basic random as first 4byte InitValue for mac calculate 
					  );

// the declaration of KyhPass1KSysServ.cpp api

WORD KingCos_SelectMF(DEV_HANDLE icd);

WORD KingCos_CreateDF(DEV_HANDLE icd,
					 WORD FID,
					 BYTE EC,
					 BYTE LockState,
					 BYTE LockAC,
					 BYTE CreateAC,
					 BYTE DeleteAC,
					 const char *pFName,						    // the name of DF
					 unsigned char FNameLen,		    		// the length of DF's name
					 unsigned char *pMKey,
					 unsigned char MKeyLen,
					 BYTE CLA,
					 unsigned char SM
					 );

WORD KingCos_SelectDF(DEV_HANDLE icd,
					  WORD FID);
			
WORD KingCos_CreateKeyFile(DEV_HANDLE icd,
						  WORD FID,
						  BYTE FType,
						  BYTE Usage,
						  WORD FSize,
						  BYTE ReadAC,
						  BYTE WriteAC,
						  BYTE SM,
						  unsigned char *pMKey,
						  unsigned char MKeyLen,
						  BYTE CLA
						  );

WORD KingCos_CreatePinFile(DEV_HANDLE icd,
						  WORD FID,
						  BYTE FType,
						  BYTE Usage,
						  WORD FSize,
						  BYTE ReadAC,
						  BYTE SM,
						  unsigned char CLA
						  );
WORD KingCos_CreatePubKeyFile(DEV_HANDLE icd,
						  WORD FID,
						  BYTE FType,
						  BYTE Usage,
						  WORD FSize,
						  BYTE ReadAC,
						  BYTE WriteAC,
						  BYTE SM,
						  unsigned char *pMKey,
						  unsigned char MKeyLen,
						  BYTE CLA
						  );
WORD KingCos_CreatePriKeyFile(DEV_HANDLE icd,
						  WORD FID,
						  BYTE FType,
						  BYTE Usage,
						  WORD FSize,
						  BYTE ReadAC,
						  BYTE WriteAC,
						  BYTE SM,
						  unsigned char *pMKey,
						  unsigned char MKeyLen,
						  BYTE CLA
						  );
WORD KingCos_CreateBin(DEV_HANDLE icd,
					   WORD FID,
					   BYTE FType,															  // binary file
					   BYTE Usage,																// without limiting the usage
					   WORD FSize,
					   BYTE ReadAC,
					   BYTE WriteAC,
					   BYTE SM,
					   unsigned char *pMKey,
					   unsigned char MKeyLen,
					   BYTE CLA
					   );

WORD KingCos_SelectEF(DEV_HANDLE icd,
					  WORD FID
					  );

WORD KingCos_DeleteDF(DEV_HANDLE icd,
					  DWORD FID
					  );
WORD KingCos_DeleteMF(DEV_HANDLE icd);


WORD KingCos_DeleteEF(DEV_HANDLE icd,
					  DWORD FID
					  );


// the declaration of KyhPass1KAuthOprt.cpp api
WORD KingCos_GetChallenge(DEV_HANDLE icd,
						  unsigned char Number,                      // input number
						  unsigned char *pChallenge               // output receive dataBuffer
						  );



WORD KingCos_GenKeyPair(DEV_HANDLE icd,
						unsigned char Alg,								 // use which algorithm to generate key pair
						unsigned short PubFID,
						unsigned short PriFID
						);

WORD KingCos_ExterAuth(DEV_HANDLE icd,
					   unsigned char KeyIndex,
					   unsigned char *pExAuKey                  // external authority Key
					   );

WORD KingCos_InterAuth(DEV_HANDLE icd,
					   unsigned char KeyIndex,
					   unsigned char *pInAuKey					// internal authority Key
					   );

WORD KingCos_ImportPubKey(DEV_HANDLE icd,
						unsigned short PubFID,															// now only for RSA file,not for SM2
						const unsigned char *Modulus,
						const unsigned char *Exponent,
						int KeyLen,																			    // public key length ,default is 1024
						BYTE CLA,
						BYTE SM,
						unsigned char *pDataKey,
						unsigned long DataKeyLen
						);


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
						  );

WORD KingCos_ExportPubKey(DEV_HANDLE icd,
						unsigned short PubFID,
						BYTE CLA,
						int KeyLen,
						unsigned char *pModulus,
						unsigned char *pExponent,
						BYTE SM
						);

// this cmd is used as private key encrypt
WORD KingCos_RsaDigitalSig(DEV_HANDLE icd,
						   unsigned short PriFID,									// use asymmmetric Private Key
						   const unsigned char *pSigData, 
						   unsigned short SigDataLen,
						   unsigned char *pSignResult,
						   unsigned char Alg,
						   BYTE CLA
						   );
				

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
					 );

				
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
					 );

WORD KingCos_UpdateBinary(DEV_HANDLE icd,
						   int Offset,
						  unsigned int DataLen,
						  const BYTE *pDataIn,										// input data
						  unsigned char CLA,
						  unsigned char SM,
						  BYTE *pMKey,
						  unsigned char MKeyLen
						  );


WORD KingCos_UpdateBinaryCell(DEV_HANDLE icd,
							  int Offset,													 // use the offset 1byte , not 2bytes
							  unsigned int DataLen,
							  const BYTE *pDataIn,								// input data
							  unsigned char SM,
							  unsigned char CLA,
							  unsigned char *pMKey,
							  unsigned char MKeyLen
							   );

WORD KingCos_ReadBinary(DEV_HANDLE icd,
						int Offset,                               // use the offset 1byte , not 2bytes
						unsigned int DataLen,
						BYTE *pDataOut,						  // output data
						unsigned char SM,
						unsigned char CLA,
						unsigned char *pDataKey,					 // Data maintenance key
						unsigned char DataKeyLen
						);
WORD KingCos_ReadBinaryCell(DEV_HANDLE icd,
							  int Offset,                               // use the offset 1byte , not 2bytes
							  unsigned int DataLen,
							  BYTE *pDataOut,								// output data
							  unsigned char SM,
							  unsigned char CLA,
							  unsigned char *pDataKey,						// Data maintenance key
							  unsigned char DataKeyLen
							  );




WORD KingCos_ReloadPin(DEV_HANDLE icd,
					   unsigned char PinIndex,
					   const unsigned char *pNewPin,
					   unsigned long NewPinLen,
					   BYTE *pReloadKey,
					   unsigned char ReloadKeyLen
					   );

WORD KingCos_VerifyPin(DEV_HANDLE icd,
					   unsigned int usertype,
					   const unsigned char *pPin,
					   unsigned long PinLen
					   );

WORD KingCos_ChangePin(DEV_HANDLE icd,
					   unsigned char PID,
					   const BYTE *pNewPin,           // must not be modify this variable
					   ULONG NewPinLen,
					   const BYTE *pOldPin,
					   ULONG OldPinLen,
					   unsigned char SM
					   );


WORD KingCos_UpdatePin(DEV_HANDLE icd,
					   unsigned char PinUsage,
					   unsigned char PinIndex,
					   unsigned long UpdateDataLen,
					   const unsigned char *UpdateData,
					   unsigned char *pDataKey,							 // Data maintenance key
					   unsigned int DataKeyLen,
					   unsigned char SM
					   );



WORD KingCos_UnblockPin(DEV_HANDLE devHandle,
						BYTE *PinReloadKey,
						int PinReloadKeyLen, 
						BYTE *NewPin,
						int NewPinLen
						);




WORD KingCos_PriKeyDecrypt(DEV_HANDLE icd,
						unsigned short PriFID,									// use asymmmetric Private Key
						const unsigned char *pEncryData, 
						ULONG EncryDataLen,
						unsigned char *pPlainData,
						 ULONG *pDataOutLen,
						BYTE CLA
						);


WORD KingCos_InitDevice(DEV_HANDLE icd,
						unsigned char *pIssueKey,
						unsigned char IssueKeyLen
						);




WORD KingCos_ResetDevice(DEV_HANDLE icd);

WORD KingCos_CreateMF(DEV_HANDLE icd,
					 BYTE EC,
					 BYTE LockState,
					 BYTE LockAC,
					 BYTE CreateAC,
					 BYTE DeleteAC,
					 unsigned char CLA,
					 unsigned char SM
					 );



#endif
