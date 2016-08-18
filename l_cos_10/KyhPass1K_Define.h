/***
* KyhPass1K_Define.h
*		
* Purpose:
*       This file defines macro of Key variables and Key variables
*
* Copyright (c) KingYH Corporation. All rights reserved.
*
* Author : hffan
* Date : 2013-07-19 11:05:02
***/

#ifndef __KyhPass1K_Define__H
#define __KyhPass1K_Define__H


#define APDU_MODEL_PLAIN					  0x00
#define APDU_MODEL_PLAIN_MAC		  0x01
#define APDU_MODEL_ENC_MAC             0x22
#define APDU_MODEL_ENC						  0x03



#define MAX_BIN_APDU_LEN					  192

#define RIGHT_FREE										 0x00
#define RIGHT_READ_USER_PIN				 0x01
#define RIGHT_WRITE_USER_PIN			 0x02
#define RIGHT_USE_USER_PIN					 0x04
#define RIGHT_READ_FORBIDDEN			 0x08


#define USER_PIN_RIGHT_STATUS			0x55
#define ADMIN_PIN_RIGHT_STATUS		0x66


#define PIN_HASH											 0x00
#define PIN_RANDOM_HASH					 0x01
#define PIN_RANDOM_DES						 0x70





#define TYPE_MKEY													    0x01
#define TYPE_PIN_CODE												0x02
#define TYPE_PIN_UNBLOCK_RELOAD				    0x03

//#define TYPE_PIN_RELOAD                   0x04
#define TYPE_EXTH														0x05
#define TYPE_INTH														0x06

#define TYPE_KEY_UNBLOCK               0x07
#define TYPE_KEY_RELOAD                  0x08
#define TYPE_APP_LOCK                       0x09
#define TYPE_DATA_MT                        0x0a
#define TYPE_DATA_ENC                       0x0b
#define TYPE_DATA_DEC                       0x0c
#define TYPE_INIT_SYS						    0x0d
#define TYPE_UKEY_ISSUE					   0x0e
#define TYPE_SESSION_KEY				   0x0f
#define TYPE_TMP_KEY_PAIR             0x10
#define TYPE_TMP_KEY                         0x11


// define the pointer of MKey
#define DATAKEY				(BYTE *)"0102030405060708"
#define DF_APPKey			(BYTE *)"8888888888888888"
#define INIT_KEY				(BYTE *)"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
#define  ISSUE_KEY			(BYTE *)"KyhPass1K1234567"

#define MF_MKey				(BYTE *)"2222222222222222"
#define DF_MKey				(BYTE *)"2222222222222222"

#define RELOAD_KEY		(BYTE *)"jyh12345jyh12345"


// define the pin type 
#define TYPE_ADMIN_PIN			0x00
#define TYPE_USER_PIN				0x01

// define use which algorithm to encrypt or decrypt
#define ALG_SHA1				0x00
#define ALG_SHA256			0x10
#define ALG_SHA384			0x20
#define ALG_SHA512			0x30
#define ALG_SM3				0x40
#define ALG_MD5				0x50
#define ALG_NONE			0x60

// define use which algorithm to generate key pair
#define ALG_RSA1024	0x00
#define ALG_RSA2048	0x01



// define the TLV of public key
#define TLV_E					0x45
#define TLV_N					0x4E

// the key for symmetrical algorithm 
#define SYM_TDES				0x00
#define SYM_DES					0x01
#define SYM_SM4					0x02
#define SYM_SCB2					0x03


// define use the pin as the key of which algorithm
#define PIN_SHA1							0x00
#define PIN_SHA256						0x01
#define PIN_SHA384						0x02
#define PIN_SHA512						0x03
#define PIN_SM3								0x04
#define PIN_MD5							0x05
#define PIN_SM4								0x06
#define PIN_3DES							0x07
#define PIN_RAND_HASH			0x20
#define PIN_RAND_DES_3DES	0x10
#define PIN_OTHER						0x60


#define MF_FID							0x3F00
#define KEY_FID							0x0000


#define FILETYPE_DF									0x00
#define FILETYPE_KEY									0x20
#define FILETYPE_BIN									0x10
#define FILETYPE_LINEAR_RECORD		0x21
#define FILETYPE_VAR_RECORD				0x22
#define FILETYPE_RECY_RECORD			0x23
#define FILETYPE_PUBKEY_RSA1024		0x11
#define FILETYPE_PRIKEY_RSA1024		0x12
#define FILETYPE_PUBKEY_RSA2048		0x13
#define FILETYPE_PRIKEY_RSA2048		0x14
#define FILETYPE_PUBKEY_SM2				0x15
#define FILETYPE_PRIKEY_SM2				0x16

#define PID_ADMIN  0x00
#define PID_USER      0x01




// define all global constant Keys



#endif 
