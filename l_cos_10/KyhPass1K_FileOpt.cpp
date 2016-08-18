/***
* KyhPass1KFileOpt.cpp:
*		definitions of apdu cmd api
* Purpose:
*       This file defines the structures, global values,and functions
*       used by KyhPass1KApdu.cpp
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-23 17:21:41
***/



#include "KyhPass1K_Define.h"
#include "KyhPass1K_Apdu.h"
#include "KyhPass1K_Glb.h"
#include "AlgDes.h"
#include "AlgDes1.h"
#include  <base/reader.h>
WORD KingCos_GetResponse(DEV_HANDLE CardHandle,WORD MaxSize,
						 WORD *p_ReturnLen,unsigned char *p_ReturnData)
{
	DEV_RES result;

	unsigned long len;
	unsigned long ReturnLen;
	unsigned char ReturnData[MAX_PATH];
	unsigned char Apdu_comm[5]={0x00,0xC0,0x00,0x00,0x00};
	Apdu_comm[4]=(unsigned char )MaxSize;

	len=5;

	result=P_SendApduComm(CardHandle,len,Apdu_comm,&ReturnLen,ReturnData);

	*p_ReturnLen=(unsigned char)ReturnLen;
	if(p_ReturnData!=NULL)
		memcpy(p_ReturnData,ReturnData,*p_ReturnLen);
	return result;
}

/*
this cmd is similar to CreateDF, CreateMF is a kind of CreateDF
*/
// RFU,this cmd do not use
WORD KingCos_CreateMF(DEV_HANDLE icd,
					 BYTE EC,
					 BYTE LockState,
					 BYTE LockAC,
					 BYTE CreateAC,
					 BYTE DeleteAC,
					 BYTE CLA,
					 unsigned char SM
					 )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = CLA;
	g_apdu.ins = 0xE0;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09;
	/*
	0x08 = 2 (FID)               +
				 1 (FType)          +
				 1 (EC)                +
				 1 (LockState)  +
				 1 (LockAC)       +
				 1 (CreateAC)      +
				 1 (DeleteAC)    +
				 1 (FnameLen)
					*/
	//  compatible to COS , using byte stream
	g_apdu.sendbuf[0] = 0x3F;         // FID[1]
	g_apdu.sendbuf[1] = 0x00;         // FID[0]
	// FType is a reserved byte 
	g_apdu.sendbuf[2] = FILETYPE_DF;	// FType(MF file attribute)
	g_apdu.sendbuf[3] = EC;						// EC(error count)                                                  
	g_apdu.sendbuf[4] = LockState;			// LockState
	g_apdu.sendbuf[5] = LockAC;				// LockAC(custom condition)
	g_apdu.sendbuf[6] = CreateAC;			// CreateAC(custom condition)
	g_apdu.sendbuf[7] = DeleteAC;			// DeleteAC(custom condition)

	unsigned char *pKey = NULL;
	unsigned char KeyLen = 0x00;
	unsigned char challenge[8] = {0};
	unsigned char *random = NULL;

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = ISSUE_KEY;
		KeyLen = 0x10;
		KingCos_GetChallenge(icd,0x08,challenge);
		random = challenge;
	}

	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
		random = NULL;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,KeyLen,SM,random);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}


/*
this cmd is to SelectMF, and use the FID to select
*/
WORD KingCos_SelectMF(DEV_HANDLE icd)
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x00;
	g_apdu.ins = 0xA4;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;
	// use the FID to Select MF
	g_apdu.lc = 0x02;
	g_apdu.sendbuf[0] = 0x3F;
	g_apdu.sendbuf[1] = 0x00;
	
	KingCos_BuildApdu(icd,&g_apdu,send,NULL,0,APDU_MODEL_PLAIN,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	if((g_sw1sw2&0xff00)==0x6100)
	{
		DWORD dwdst=0x82;
		BYTE rdata[MAX_PATH];
		g_sw1sw2=KingCos_GetResponse(icd,g_sw1sw2&0x00ff,(WORD*)&dwdst,rdata);
	}
		return g_sw1sw2;
}


/*
this cmd is to CreateDF, different from CreateMF is that CreateDF needs FName in addition
*/
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
					 )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = CLA;
	g_apdu.ins = 0xE0;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09 + (BYTE)FNameLen;
	/*
	0x09 + (BYTE)FNameLen = 
													2 (FID)               +
													1 (FType)          +
													1 (EC)                +
													1 (LockState)  +
													1 (LockAC)       +
													1 (CreateAC)      +
													1 (DeleteAC)    +
													1 (FNameLen) +        
													FNameLen     
													*/

	// pointer to WORD convert to pointer to BYTE
	/*
							|	|	
	|   | WORD  ¡ú  |	| BYTE           
	
	*/
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);  //FID[1]
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);  //FID[0]
	
	// FType is a reserved byte 
	g_apdu.sendbuf[2] = 0x00;  // FType(DF file attribute)

	/*
	0xEC  =	|		0		0		|		0		0		0		|		0		0		0		|
					|	  bit8  bit7   |    bit6  bit5  bit4     |     bit3  bit2 bit0     |
	*/
	g_apdu.sendbuf[3] = EC;										   // EC(error count)
	g_apdu.sendbuf[4] = LockState;							   // LockState
	g_apdu.sendbuf[5] = LockAC;								   // LockAC(custom condition)
	g_apdu.sendbuf[6] = CreateAC;							   // CreateAC(custom condition)
	g_apdu.sendbuf[7] = DeleteAC;							   // DeleteAC(custom condition)
	g_apdu.sendbuf[8] = FNameLen;							   // FNameLen

	memcpy(g_apdu.sendbuf+9, pFName, FNameLen);
	unsigned char *pKey;
	unsigned char KeyLen;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if ( APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,	KeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);

	return g_sw1sw2;

}


					 
/*
this cmd is to SelectDF, use the FID to select the directory
*/
WORD KingCos_SelectDF(DEV_HANDLE icd,
					 WORD FID
					 )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x00;
	g_apdu.ins = 0xA4;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;

	// use the FID to Select DF
	g_apdu.lc = 0x02;
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);  //FID[1]
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);  //FID[0]
	KingCos_BuildApdu(icd,&g_apdu,send,NULL,0,APDU_MODEL_PLAIN,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);

	if((g_sw1sw2&0xff00)==0x6100)
	{
		DWORD dwdst=0x82;
		BYTE rdata[MAX_PATH];
		g_sw1sw2=KingCos_GetResponse(icd,g_sw1sw2&0x00ff,(WORD*)&dwdst,rdata);
	}

	return g_sw1sw2;
}


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
					   )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	BYTE sm = 0x22;

	g_apdu.cla = CLA;
	g_apdu.ins = 0xE0;
	g_apdu.p1 = 0x01;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09;

	/*
	0x09 = 
				2 (FID)               +
				1 (FType)          +
				1 (Usage)			 +
				2 (FSize)           +
				1 (ReadAC)		 +
				1 (WriteAC)      +
				1 (SM)               

	*/
	/*
	pointer to WORD convert to pointer to BYTE

	|	|	
	|   | WORD  ¡ú  |	| BYTE           

	*/
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);								//FID[1]
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);								//FID[0]

	// FType is a reserved byte 
	g_apdu.sendbuf[2] = FType;																	// FType (EF file attribute)
	g_apdu.sendbuf[3] = Usage;																	// Usage (this apdu do not detect this byte)
	g_apdu.sendbuf[4] =*((unsigned char *)&FSize+1);						// FSize[1]
	g_apdu.sendbuf[5] =*((unsigned char *)&FSize+0);						// FSize[0]
	g_apdu.sendbuf[6] = ReadAC;																// ReadAC(do not detect this byte,any value)
	g_apdu.sendbuf[7] = WriteAC;																// WriteAC(custom condition)
	g_apdu.sendbuf[8] = sm;																	    // SM(Security Message )
	unsigned char *pKey;
	unsigned char KeyLen;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,	KeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}



/*
this cmd is to CreateKeyFile, Create a key file , just one kind of CreateEF
*/
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
						  )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = CLA;
	g_apdu.ins = 0xE0;
	g_apdu.p1 = 0x01;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09;
	/*
	0x09 = 
				2 (FID)               +
				1 (FType)          +
				1 (Usage)			 +
				2 (FSize)           +
				1 (ReadAC)		 +
				1 (WriteAC)      +
				1 (SM)               

	*/

	/*
	pointer to WORD convert to pointer to BYTE
	|	|	
	|   | WORD  ¡ú  |	| BYTE           
	*/
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);								//FID[1]
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);								//FID[0]
	// FType is a reserved byte 
	g_apdu.sendbuf[2] = FType;																		// FType (DF file attribute)
	g_apdu.sendbuf[3] = Usage;																		// Usage (this apdu do not detect this byte)
	g_apdu.sendbuf[4] =*((unsigned char *)&FSize+1);							// FSize[1]
	g_apdu.sendbuf[5] =*((unsigned char *)&FSize+0);							// FSize[0]
	g_apdu.sendbuf[6] = ReadAC;																	// ReadAC(do not detect this byte,any value)
	g_apdu.sendbuf[7] = WriteAC;																	// WriteAC(custom condition)
	g_apdu.sendbuf[8] = SM;																			// SM(Security Message )
	unsigned char *pKey = NULL;
	unsigned char KeyLen = 0;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}

	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,KeyLen,	SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}






/*
this cmd is to CreatePubKeyFile, Create a public key file , just one kind of CreateEF
*/
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
						  )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	BYTE sm = 0x22;
	g_apdu.cla = CLA;
	g_apdu.ins = 0xE0;
	g_apdu.p1 = 0x01;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09;
	/*
	0x09 = 
				2 (FID)               +
				1 (FType)          +
				1 (Usage)		     +
				2 (FSize)           +
				1 (ReadAC)	     +
				1 (WriteAC)      +
				1 (SM)               

	*/
	/*
	pointer to WORD convert to pointer to BYTE

	|	|	
	|   | WORD  ¡ú  |	| BYTE           
	*/
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);							//FID[1] 
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);							//FID[0] 
	// FType is a reserved byte 
	g_apdu.sendbuf[2] = FType;																		// FType (DF file attribute)
	g_apdu.sendbuf[3] = Usage;																		// Usage (custom condition)
	g_apdu.sendbuf[4] =*((unsigned char *)&FSize+1);							// FSize[1] (do not detect this byte)
	g_apdu.sendbuf[5] =*((unsigned char *)&FSize+0);							// FSize[0] (do not detect this byte)
	g_apdu.sendbuf[6] = ReadAC;																	// ReadAC (custom condition)
	g_apdu.sendbuf[7] = WriteAC;																	// WriteAC (custom condition)
	g_apdu.sendbuf[8] = sm;																		    // SM (Security Message )
	unsigned char *pKey;
	unsigned char KeyLen;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,	KeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}



/*
this cmd is to CreatePriKeyFile, Create a private key file , just one kind of CreateEF
*/
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
						  )
{

	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	BYTE sm = 0x22;

	g_apdu.cla = CLA;
	g_apdu.ins = 0xE0;
	g_apdu.p1 = 0x01;
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x09;
	/*
	0x09 = 
				2 (FID)               +
				1 (FType)          +
				1 (Usage)		  +
				2 (FSize)           +
				1 (ReadAC)	  +
				1 (WriteAC)      +
				1 (SM)               

	*/


	/*
	pointer to WORD convert to pointer to BYTE

	|	|	
	|   | WORD  ¡ú  |	| BYTE           

	*/
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);							//FID[1] 
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);							//FID[0] 

	// FType is a reserved byte 
	g_apdu.sendbuf[2] = FType;																	// FType (DF file attribute)
	g_apdu.sendbuf[3] = Usage;																	// Usage (custom condition)
	g_apdu.sendbuf[4] =*((unsigned char *)&FSize+1);						// FSize[1] (do not detect this byte)
	g_apdu.sendbuf[5] =*((unsigned char *)&FSize+0);						// FSize[0] (do not detect this byte)
	g_apdu.sendbuf[6] = ReadAC;																// ReadAC (custom condition)
	g_apdu.sendbuf[7] = WriteAC;																// WriteAC (custom condition)
	g_apdu.sendbuf[8] = sm;																		// SM (Security Message )
	unsigned char *pKey;
	unsigned char KeyLen;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,	KeyLen,SM,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);

	return g_sw1sw2;

}




/*
this cmd is for Update binary file or private/public file
*/

WORD KingCos_UpdateBinary(DEV_HANDLE icd,
						  int Offset,
						  unsigned int DataLen,
						  const BYTE *pDataIn,												// input data
						  unsigned char CLA,
						  unsigned char SM,
						  BYTE *pMKey,
						  unsigned char MKeyLen
						  )
			
{
	switch(SM){
		case APDU_MODEL_ENC_MAC:
			if (DataLen < MAX_BIN_APDU_LEN){
				g_sw1sw2 = KingCos_UpdateBinaryCell(icd,Offset,DataLen,pDataIn,SM,CLA,pMKey,MKeyLen);
			}
			else{
				unsigned int count = 0;
				while (count < DataLen) {
					if (count+MAX_BIN_APDU_LEN > DataLen)
						g_sw1sw2 = KingCos_UpdateBinaryCell(icd,count+Offset,DataLen-count,pDataIn+count,SM,	CLA,pMKey,MKeyLen);
					else
						g_sw1sw2 = KingCos_UpdateBinaryCell(icd,count+Offset,MAX_BIN_APDU_LEN,pDataIn+count,SM,CLA,pMKey,MKeyLen);
					if (g_sw1sw2 != 0x9000)
						return g_sw1sw2;
					count+=MAX_BIN_APDU_LEN;
				}
			}
	
			break;
		case APDU_MODEL_PLAIN:
				if (DataLen < MAX_BIN_APDU_LEN){
				g_sw1sw2 = KingCos_UpdateBinaryCell(icd,Offset,DataLen,pDataIn,SM,CLA,NULL,0x10);                          
			}
			else{
				unsigned int count = 0;
				while (count < DataLen) {
					if (count+MAX_BIN_APDU_LEN > DataLen)
						g_sw1sw2 = KingCos_UpdateBinaryCell(icd,count+Offset,DataLen-count,pDataIn+count,SM,	CLA,NULL,0);
					else
						g_sw1sw2 = KingCos_UpdateBinaryCell(icd,count+Offset,MAX_BIN_APDU_LEN,pDataIn+count,SM,CLA,NULL,0);
					if (g_sw1sw2 != 0x9000)
						return g_sw1sw2;
					count+=MAX_BIN_APDU_LEN;
				}
			}
			break;
		default:
			return 0x6901;
	}
	return g_sw1sw2;
}




/*
this cmd is Update binary file cell for UpdateBinary to call
or private/public file
*/

WORD KingCos_UpdateBinaryCell(DEV_HANDLE icd,
							  int Offset,                               // use the offset 1byte , not 2bytes
							  unsigned int DataLen,
							  const BYTE *pDataIn,
							  unsigned char SM,
							  unsigned char CLA,
							  unsigned char *pMKey,
							  unsigned char MKeyLen
							   )
{
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = CLA;
	g_apdu.ins = 0xD6;

	g_apdu.p1=*((unsigned char *)&Offset+1);
	g_apdu.p2=*((unsigned char *)&Offset+0);

	g_apdu.lc = DataLen;

	memcpy(g_apdu.sendbuf,pDataIn,DataLen);
	unsigned char *pKey;
	unsigned char KeyLen;
	unsigned char challenge[8] = {0};
	unsigned char *random = NULL;
	

	if (APDU_MODEL_ENC_MAC == SM)
	{
		pKey = pMKey;
		KeyLen = MKeyLen;
		KingCos_GetChallenge( icd,0x04,challenge);
		random = challenge;
	}
	if (APDU_MODEL_PLAIN == SM)
	{
		pKey = NULL;
		KeyLen = 0x00;
		random = NULL;
	}
	KingCos_BuildApdu(icd,&g_apdu,send,pKey,	KeyLen,SM,random);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);

	return g_sw1sw2;

}





/*
this cmd is  ReadBinary to read binary file or  private/public file
*/

WORD KingCos_ReadBinary(DEV_HANDLE icd,
							 int Offset,												    // use the offset 1byte , not 2bytes
							  unsigned int DataLen,
							  BYTE *pDataOut,								    // output data
							  unsigned char SM,
							  unsigned char CLA,
							  unsigned char *pDataKey,					 // Data maintenance key
							  unsigned char DataKeyLen
							  )
	{
	switch(SM)
	{
		case APDU_MODEL_PLAIN_MAC:
			if (DataLen < MAX_BIN_APDU_LEN)
			{
				g_sw1sw2 = KingCos_ReadBinaryCell(icd,Offset,DataLen,pDataOut,SM,CLA,pDataKey,DataKeyLen);
			}
			else
			{
				unsigned int count = 0;
				while (count < DataLen) 
				{
					if (count+MAX_BIN_APDU_LEN > DataLen)
						
						g_sw1sw2 = KingCos_ReadBinaryCell(icd,count+Offset,DataLen-count,pDataOut+count,SM,	CLA,pDataKey,DataKeyLen);
					else
						g_sw1sw2 = KingCos_ReadBinaryCell(icd,count+Offset,MAX_BIN_APDU_LEN,pDataOut+count,SM,	CLA,pDataKey,DataKeyLen);
					if (g_sw1sw2 != 0x9000)
						return g_sw1sw2;
					count+=MAX_BIN_APDU_LEN;
				}
			}
			break;
		case APDU_MODEL_PLAIN:
			if (DataLen < MAX_BIN_APDU_LEN)
			{
				g_sw1sw2 = KingCos_ReadBinaryCell(icd,Offset,DataLen,pDataOut,SM,CLA,NULL,0x00);
			}
			else
			{
				unsigned int count = 0;
				while (count < DataLen) 
				{
					if (count+MAX_BIN_APDU_LEN > DataLen)
						g_sw1sw2 = KingCos_ReadBinaryCell(icd,count+Offset,DataLen-count,pDataOut+count,SM,CLA,NULL,0x00);
					else
						g_sw1sw2 = KingCos_ReadBinaryCell(icd,count+Offset,MAX_BIN_APDU_LEN,pDataOut+count,SM,CLA,NULL,0x00);
					if (g_sw1sw2 != 0x9000)
						return g_sw1sw2;
					count+=MAX_BIN_APDU_LEN;
				}
			}
			break;
			
		
		default:
			break;
	}
	return g_sw1sw2;
}




/*
this cmd is Read binary file cell for ReadBinary to call
or private/public file
*/

WORD KingCos_ReadBinaryCell(DEV_HANDLE icd,
							  int Offset,												// use the offset 1byte , not 2bytes
							  unsigned int DataLen,
							   BYTE *pDataOut,								// output data
							  unsigned char SM,
							  unsigned char CLA,
							  unsigned char *pDataKey,				// Data maintenance key
							  unsigned char DataKeyLen
							  )
{
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = CLA;
	g_apdu.ins = 0xB0;
	g_apdu.p1=*((unsigned char *)&Offset+1);
	g_apdu.p2=*((unsigned char *)&Offset+0);
	g_apdu.lc = 0x00;
	g_apdu.le = DataLen;
	int detect;
	BYTE TempDataOutMac[280] = {0};
	BYTE TempDataOutPad[280] = {0};
	BYTE TempDataOutPadBlock[8] = {0};
	BYTE TempDataOut[280] = {0};

	BYTE TempDataOutBlock[8] = {0};
	unsigned long cnt = 0;
	int blockLen = 8;


	unsigned char challenge[8] = {0};
	unsigned char *random = NULL;
	if (SM == APDU_MODEL_PLAIN)
	{
		KingCos_GetChallenge( icd,0x04,challenge);
		random = challenge;
	}
	else if (SM == APDU_MODEL_ENC_MAC)
	{
		KingCos_GetChallenge( icd,0x04,challenge);
		random = challenge;
	}
	

	KingCos_BuildApdu(icd,&g_apdu,send,pDataKey,DataKeyLen,SM,random);
	// le 
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc + 1 + 4,send,&recvLen,g_apdu.recvbuf);
	if(g_sw1sw2==0x9000)
	{
		memcpy(TempDataOutMac,g_apdu.recvbuf,recvLen);
		recvLen-= 4;
		memcpy(TempDataOutPad,TempDataOutMac,recvLen);
		for ( cnt = 0; cnt < recvLen/8; cnt++)
		{
			
			memcpy(TempDataOutPadBlock, TempDataOutPad+8*cnt, 8);
			// decrypt the output data , KEY is dataKey
			//TripleDESDecipher((int *)&recvLen,TempDataOutPad,TempDataOut,DATAKEY);

			TripleDESDecipher((int *)&blockLen, TempDataOutPadBlock, TempDataOutBlock, DATAKEY);
			memcpy(TempDataOut + cnt*8, TempDataOutBlock, 8);

		}
		for(detect = 0;detect<8;detect++)
		{
			if (0x80 == TempDataOut[recvLen - 1 - detect])
				memcpy(pDataOut, TempDataOut,  recvLen - 1 - detect);
		}
	}
	return g_sw1sw2;
}





/*
this cmd for delete the DF directory or MF directory
*/

WORD KingCos_DeleteDF(DEV_HANDLE icd,
					  DWORD FID)
{

	if (MF_FID == FID)
	{
		WORD res=KingCos_DeleteMF(icd);
		if(res!=0x9000)
			return res;
	}else
	{
		return 0x6901;
	}
	
	return 0x9000;
}



/*
this cmd for delete the DF directory
*/

WORD KingCos_DeleteMF(DEV_HANDLE icd)
{
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = 0x84;
	g_apdu.ins = 0xE4;
	g_apdu.p1 = 0x03;															 // P1 = 0x03 , by FID to delete the MF
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x00;
	g_apdu.le = 0x00;
	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);
	// use IssueKey to delete the MF
	KingCos_BuildApdu(icd,&g_apdu,send,ISSUE_KEY,0x10,APDU_MODEL_PLAIN_MAC,challenge);

	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	return g_sw1sw2;
}







/*
this cmd for delete the EF directory
*/


WORD KingCos_DeleteEF(DEV_HANDLE icd,
						   DWORD FID
						   )
{
	BYTE send[280] = {0};
	unsigned long recvLen=MAX_PATH;

	g_apdu.cla = 0x84;
	g_apdu.ins = 0xE4;
	g_apdu.p1 = 0x00;																						 // P1 = 0x00 , by FID to delete the DF
	g_apdu.p2 = 0x00;
	g_apdu.lc = 0x02;
	g_apdu.le = 0x00;

	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);							// FID[1] 
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);							// FID[0] 

	unsigned char challenge[8] = {0};
	KingCos_GetChallenge( icd,0x04,challenge);

	KingCos_BuildApdu(icd,&g_apdu,send,DF_MKey,0x10,APDU_MODEL_ENC_MAC,challenge);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);

	return g_sw1sw2;
}




/*

this cmd for select the EF directory , use the FID to select the directory
*/
WORD KingCos_SelectEF(DEV_HANDLE icd,
					 WORD FID
					 )
{
	unsigned long recvLen=MAX_PATH;
	BYTE send[280] = {0};
	g_apdu.cla = 0x00;
	g_apdu.ins = 0xA4;
	g_apdu.p1 = 0x00;
	g_apdu.p2 = 0x00;

	// use the FID to Select DF
	g_apdu.lc = 0x02;
	g_apdu.sendbuf[0] = *((unsigned char *)&FID + 1);  //FID[1]
	g_apdu.sendbuf[1] = *((unsigned char *)&FID + 0);  //FID[0]
	KingCos_BuildApdu(icd,&g_apdu,send,NULL,0,APDU_MODEL_PLAIN,NULL);
	g_sw1sw2 = P_SendApduComm(icd,g_apdu.lc+4+1,send,&recvLen,g_apdu.recvbuf);
	if((g_sw1sw2&0xff00)==0x6100)
	{
		DWORD dwdst=0x82;
		BYTE rdata[MAX_PATH];
		g_sw1sw2=KingCos_GetResponse(icd,g_sw1sw2&0x00ff,(WORD*)&dwdst,rdata);
	}
	return g_sw1sw2;


}
