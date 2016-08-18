/*
[]======================================================================[]

	Copyright(C), Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	oem_cfg.h

DESC:
	This is the const definition header file for EnterSafe OEM.

REVISION:
	2004-04-26 [Skybird Le]
		Created.
[]======================================================================[]
*/

#ifndef __ES_OEM_CFG_H__
#define __ES_OEM_CFG_H__

#include <es/base_type.h>
#include <es/es_typedef.h>
//=========================================================================//
// Window Class
//=========================================================================//
// #define CERTD_WNDCLASS					"CERTD_8E767E7FEDD84EB78252B9F815AEA984"
//#define BALLOON_WNDCLASS				"BALLOON_EB6C8E40DCD9419E96B0B0C271FA38CB"
//
//#define INITTOOL_WNDCLASS				"INIT_1A255CC42D1F4D7D9E82408C14C93A1E"
// #define EPASS_NG_MGR_GUID				"4417ABFD-640E-4120-ADCF-CC17692E4F3E"

//=========================================================================//
// PKCS#11 oem related constants.
//=========================================================================//
//#if defined(PKCS11_MODULE)

// The manufacturer's ID, padding with space (' '). Here is
// "Feitian Technologies Co., Ltd."
//static const CK_UTF8CHAR g_ManufactureID[32] =
//{
//	'F','e','i','t','i','a','n',' ',
//	'T','e','c','h','n','o','l','o',
//	'g','i','e','s',' ','C','o','.',
//	',',' ','L','t','d','.',' ',' '
//};
//
//// The description of the PKCS#11 library, padding with space (' '). Here is
//// "ePassNG PKCS#11 Library."
//static const CK_UTF8CHAR g_LibDesc[32] =
//{
//	'e','P','a','s','s','N','G',' ',
//	'P','K','C','S','#','1','1',' ',
//	'L','i','b','r','a','r','y',' ',
//	' ',' ',' ',' ',' ',' ',' ',' '
// };


// for ftsafe ePass3000
// here we do not use CK_ULONG to avoid the byte sequence in macintosh
//static const CK_BYTE g_OEMID[4] = {0x00, 0x00, 0x00, 0x00};

//#endif // defined(PKCS11_MODULE)

//=========================================================================//
// CSP oem related constants.
//=========================================================================//
//#if defined(CSP_MODULE) || defined(CERTSP_MODULE)

//#define CSP_PROVIDER_NAME		"FEITIAN ePassNG RSA Cryptographic Service Provider"
//  #define WCSP_PROVIDER_NAME		L"FEITIAN ePassNG RSA Cryptographic Service Provider"
//const char PKCS11_LIBRARY_FILENAME[]	= "ngp11v211.dll";

//const char CONST_GUID_COMPANY[]	= "6C3D95F6-8868-4F7B-A390-897A6265C497_FEITIAN";

//const char CSP_LANGUAGE_FILE[]		="lang\\ngcsp";
// const char CERTD_LANGUAGE_FILE[]	="lang\\certd";
// const char CERTD_FILE_NAME[]		="epsng_certd";
//const char MGR_LANGUAGE_FILE[]		="lang\\ngmgr";


// e-Pass3000
//#if defined(HID)
//	static const char cardName1[] = "ePass3000ND";
//#else
//	static const char cardName1[] = "ePass3000";
//  #endif
//	static const unsigned char atr1[] =
//	{
//		0x3B, 0x0F, 0x00, 0x65, 0x46, 0x53, 0x05, 0x00, 0x05, 0x71, 0xDF,
//		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
// 	};
//	static const unsigned char atr_mask1[] =
//	{
//		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
//		0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00
// 	};

//#if defined(HID) || defined(CCID_HID)
//	#define TSP_DESCRIPTION			"ePass3000ND TSP"
//#elif defined(SMARTCARD)
//	#define TSP_DESCRIPTION			"ePass3000 smart card TSP"
// #endif

// The manufacturer's ID, padding with space (' '). Here is
// "Feitian Technologies Co., Ltd."
static const unsigned char ucManufacturerID[32] =
{
	'F', 'e', 'i', 't', 'i', 'a', 'n', ' ', 'T', 'e', 'c', 'h', 'n', 'o', 'l',
	'o', 'g', 'i', 'e', 's', ' ', 'C', 'o', '.', ',', ' ', 'L', 't', 'd', '.',
	' ', ' '
};

// common definition
#define SSF33_BLOCK_LEN			16
#define SSF33_KEY_LEN			16
#define SSF33_KEY_ID			10

#define GENERIC_SECRET_KEY_ID   12
#define GENERIC_SECRET_BLOCK_LEN 64


//#pragma pack(push,1)
#pragma pack(1)
#if defined(linux) || defined(macintosh)
	#if defined(UNICODE) || defined(_UNICODE)
		#define TCHAR		wchar_t
	#else
		#define TCHAR		char
	#endif
#endif
typedef struct _ES_CONFIG_DATA
{
	TCHAR Loc_flag[40];

	DWORD ulStructSize;
	unsigned short usStructVer;				// Version of this struct.
	unsigned char ucMagicCode[16];			// XOR the following data.

	TCHAR szGUID[40];						// An GUID to build more exclusive string.
	TCHAR szCompanyGuid[60];					// GUID to identify a company.

	//CSP
	TCHAR szCspName[MAX_PATH];				// CSP name (Can not empty, e.g. "EnterSafe RSA Cryptographic Service Provider")
	//char szCspFileName[MAX_PATH];			// CSP file name (Not include file extention, can not empty, e.g. "escsp")
	TCHAR szP11FileName[MAX_PATH];			// PKCS#11 library file name (Can not empty, e.g. "esp11v211.dll")
// 	char szFTStoreName[MAX_PATH];			//
// 	char szDevName[MAX_PATH];
	DWORD ulPinCacheTime;					// User-PIN Cache time (in ms, 0 = never expire)
	BYTE ucPinCacheType;                    // User-PIN Cache type: (0 = Whole system) / (1 = Inside process)

	//PKCS#11 (Add one byte for NULL end string)
	TCHAR szP11LibDesc[33];					// CK_INFO.libraryDescription (Can not empty)
	TCHAR szP11LibManuId[33];				// CK_INFO.manufacturerID (Can not empty)

	TCHAR szSlotDesc[65];					// CK_SLOT_INFO.slotDescription (Empty = use reader name)
	TCHAR szSlotManuId[33];					// CK_SLOT_INFO.manufacturerID (Can not empty)

	TCHAR szTokenName[33];					// CK_TOKEN_INFO.label (Default token name)
	TCHAR szTokenModel[17];					// CK_TOKEN_INFO.model (Empty = use model return by TSP)

	DWORD ulUserPinMin;						//the min length of user pin (0 = use value return by TSP)
	DWORD ulUserPinMax;						//the max length of user pin (0 = use value return by TSP)
	DWORD ulSoPinMin;						//the min length of soPIN (0 = use value return by TSP)
	DWORD ulSoPinMax;						//the max length of soPIN (0 = use value return by TSP)
	DWORD ulUserPinTypeLimit;				// User-PIN type: 0 = No limit | 1 = Number | 2 = Alphanumeric | 4 = Case-blend
	DWORD ulSoPinTypeLimit;					// So-PIN type: 0 = No limit | 1 = Number | 2 = Alphanumeric | 4 = Case-blend

	DWORD ulTrayType;						// Show tray icon: (0x0 = not show | 0x1 = show)

	//other
	unsigned char ucOemId[4];				// Product OEM ID
	TCHAR szRegPath[MAX_PATH];				// Mgr Config 
	TCHAR szLangCsp[MAX_PATH];				// CSP language files location and filename prefix.
	TCHAR szLangCertd[MAX_PATH];				// Certd language files location and filename prefix.
	TCHAR szLangMgr[MAX_PATH];				// Manager tool language files location and filename prefix.
	TCHAR szPublicConfigFile[MAX_PATH];		// Public configuration file path.

//	unsigned char szCertedWndClass[60];		// certed guid CERTD_WNDCLASS
//	unsigned char szBallWndClass[60];		// BALLOON guid BALLOON_WNDCLASS

//	unsigned char szInitToolWndClass[60];	// InitTool guid INITTOOL_WNDCLASS
//	unsigned char szNgMgrWndClass[60];		// NG_MGR guid EPASS_NG_MGR_GUID

//const char CERTD_FILE_NAME[]		="epsng_certd";	
//	TCHAR szCertdName[60];//CERTD_FILE_NAME[]
	//TSP DES
//	TCHAR szTspDes[60];//TSP_DESCRIPTION
	//atr
//	unsigned char szAtr[30];//atr1
//	DWORD atrlen;
	
//	unsigned char szAtrMask[30];//atr_mask1
//	DWORD atrMasklen;
}ES_CONFIG_DATA;
//#pragma pack(pop)

#pragma pack()


extern ES_CONFIG_DATA g_Config;

#endif // __ES_OEM_CFG_H__

//EOF
