#ifndef __CFG_H_
#define __CFG_H_

#define CIPHER_KEY "EnterSafe@2007+Feitian = 0"

#if defined MGR_CONFIG	
#pragma pack(1)
typedef struct
{
	long m_displayMode;
	bool m_bEnableSameCert;
	bool m_badmDele;
	bool m_badmExport;
	bool m_badmImport;
	bool m_badmInit;
	bool m_badmSoPin;	
	bool m_badmTokenName;
	bool m_badmUnlockPin;
	bool m_badmUserPin;
	bool m_badmViewCert;
	bool m_bUserDele;
	bool m_bUserExport;
	bool m_bUserImport;
	bool m_bUserTokenName;
	bool m_bUserUserPin;
	bool m_bUserViewCert;
	bool m_bSoftKeyboard;
	bool m_bSecDesktop;
	long m_langID;		
	long m_log;
	bool m_bPinIntensityCheck;
	bool m_bChangeTokenNameReqPin;
	bool m_bDelReqPin;
	bool m_bEnumBeforeLogin;
	long m_certValid;
	bool m_bCertChain;
}ConfigCentre;
#pragma pack()

#elif defined MIDWARE
#pragma pack(1)
typedef struct
{
	long m_langID;					// ÓïÑÔÎÄ¼þ£É£Ä
	bool m_bSSLdisconnect;			// ssl¡¡disconnect
	bool m_bSignCanExchange;		// sign key can exchange
	long m_TimeOut;				// time out
	long m_log;						// 
	long m_MaxSlotNum;              // 
	bool m_bSoftKeyboard;
	bool m_bSecDesktop;
}ConfigCentre;
#pragma pack()
	
#elif defined CERTD

#include <string>
using namespace std;

#pragma pack(1)
typedef struct
{
	bool m_bCloseBrowser;
	bool m_bVisitWebsite;
	stdString m_strUrl;
	long m_certValid;
	bool m_bStartMgr;
	long m_langID;
	long m_log;
	bool m_bShowLogo;
	bool m_bShowAlways;
	bool m_bShowMsg;
	bool m_bDefaultPinCheck;
	bool m_bCheckNoCancel;
}ConfigCentre;
#pragma pack()
#endif

#endif