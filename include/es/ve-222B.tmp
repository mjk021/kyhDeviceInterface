/*
[]=========================================================================[]

	Copyright(C) 2000-2006, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	general_func.h

DESC:
	general common functions

REVISION:
	2006-06-26 [Skybird Le]
		Created.
[]=========================================================================[]
*/


#if !defined(__GENERAL_FUNC_H__)
#define __GENERAL_FUNC_H__

#if defined(WIN32)
#include <windows.h>
#include "pkcs11/cryptoki.h"
#include "pkcs11/auxiliary.h"

// to create or open the named mutex under win98 && win2k && super
HANDLE ObtainAccessableMutex( BOOL bInitialOwner, const char* szName );
#endif

bool Bytes2String(const unsigned char* pBytes, int length, char* pStr, int* pOutLen);

void InvertBuffer(unsigned char* pBuffer, unsigned long ulBufferLen);

#if defined(WIN32)
int Utf8StringToAppString(const char * utf8Str_in, int iInLength, char* appStr_out);
int AppStringToUtf8String(const char * appStr_in, int iInLength, char* utf8Str_out);
int Utf8AnsiConvert(const char * Str_in, int iInLength, int iInType/*CP_UTF8,CP_ACP*/, char* Str_out);
CK_RV RunC_InitToken(CK_FUNCTION_LIST_PTR p11Func,
					 CK_SLOT_ID		slotID,		// ID of the token's slot
					 CK_CHAR_PTR		pPin,		// the SO's initial PIN
					 CK_ULONG		ulPinLen,	// length in bytes of the PIN
					 CK_CHAR_PTR		pLabel		// 32-byte token label (blank padded)
					 );
CK_RV RunC_InitPIN(CK_FUNCTION_LIST_PTR p11Func,
				   CK_SESSION_HANDLE	hSession,	// the session's handle
				   CK_CHAR_PTR			pPin,		// the normal user's PIN
				   CK_ULONG			ulPinLen	// length in bytes of the PIN
				   );
CK_RV RunC_SetPIN(CK_FUNCTION_LIST_PTR p11Func,
				  CK_SESSION_HANDLE	hSession,	// the session's handle
				  CK_CHAR_PTR			pOldPin,	// the old PIN
				  CK_ULONG			ulOldLen,	// length of the old PIN
				  CK_CHAR_PTR			pNewPin,	// the new PIN
				  CK_ULONG			ulNewLen	// length of the new PIN)
				  );
CK_RV RunC_Login(CK_FUNCTION_LIST_PTR p11Func,
				 CK_SESSION_HANDLE	hSession,	// the session's handle
				 CK_USER_TYPE		userType,	// the user type
				 CK_CHAR_PTR			pPin,		// the user's PIN
				 CK_ULONG			ulPinLen	// the length of the PIN)
				 );
CK_RV RunSetTokenLabel(AUX_FUNC_LIST_PTR pAuxFunc,
						CK_SLOT_ID		slotID,		// ID of the token's slot
						CK_USER_TYPE	userType,	// the user type (now reserved)
						CK_CHAR_PTR		pPin,		// the user pin (now reserved)
						CK_ULONG		ulPinLen,	// length in bytes of the PIN (now reserved)
						CK_CHAR_PTR		pLabel		// 32-byte token label (space-char [" "] padded)
						);

//为了转化一些空格填充的定长数组(blank padded),for example token label
bool UTF8BytesToANSI(char * pArray, int iLength);
#endif //defined(WIN32)
#endif // __GENERAL_FUNC_H__
