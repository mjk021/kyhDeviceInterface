/***
* AlgDes.h
*		
* Purpose:
*       This file defines api of des and 3des
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-24 09:08:09
***/

#ifndef __ALGDES__H
#define __ALGDES__H

 void  arr_inttohex(unsigned long int num , unsigned char arr_hex[] , int arr_len) ;
 unsigned long int   arr_hextoint(unsigned char arr_hexin[], int arrlen) ;
 unsigned char   asc_to_hex(char   ic_cmd[], unsigned char resp[], int len );
 BOOL   hex_to_asc(unsigned char hex_in[],  char data_back[], int len);
 int   TripleDesEnc(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
 int   TripleDesDec(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
 BOOL   GetDriverKey(const unsigned char MasterKey[], unsigned char ASN[] ,unsigned char DriverKey[] )	;
 void   DesDec(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
 void    DesEnc(unsigned char *outdata, unsigned char *indata, int datalen, const unsigned char *key);
 BOOL   mac_des(unsigned char *byKey, unsigned char *initvalue, unsigned char *datain, unsigned char *MAC, int byLen);
 BOOL   mac_3des(unsigned char byKey[], unsigned char initvalue[], unsigned char datain[], unsigned char MAC[],int byLen);
 BOOL   arr_HexCompare(unsigned char hex_a[] , unsigned char hex_b[] , int arr_len);
 void   KeyXors(unsigned char *drivekey, unsigned char *key_out );
 void   TurnString(unsigned char *pIn, int iInLen, int iLen);
 void   arr_int2TyteArr( int num , unsigned char bArr[], int iArrLen, int iType, bool bFlag);
 int   arr_TyteArr2int(unsigned char bArr[], int iArrLen, int iType, bool bFlag);
 int   pad80(unsigned char *DataIn , int *iDataLen);

#endif
