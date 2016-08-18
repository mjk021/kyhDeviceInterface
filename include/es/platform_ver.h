/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	platform_ver.h

DESC:
	

REVISION:
	[2006-06-24 Apex Liu] Created.

[]=========================================================================[]
*/

#ifndef __PLATFORM_VERSION_H__
#define __PLATFORM_VERSION_H__

#if defined(WIN32)

#include <es/base_type.h>
#include <windows.h>

class OSVersion
{
public:
	enum enumPlatform
	{
		pf_Unknown,
		pf_Win31,
		pf_WinNT4,
		pf_Win95,
		pf_Win97,
		pf_Win98,
		pf_Win98SE,
		pf_WinME,
		pf_Win2K,
		pf_WinXP,
		pf_Win2K3,
		pf_WinVista,
		pf_Windows7
	};

public:
	OSVersion();
	virtual ~OSVersion();

	const stdString& Name(void);
	const stdString& Version(void);

	bool IsRemote(void);
	bool IsX64(void);

	bool IsWin9x(void);
	bool IsWin95(void);
	bool IsWin97(void);

	bool IsWin98(void);
	bool IsWin98SE(void);
	bool IsWinME(void);

	bool IsWinNT4(void);
	bool IsWin2K(void);
	bool IsWinXP(void);
	bool IsWin2K3(void);
	bool IsWinVista(void);
	bool IsWindows7(void);
	bool IsAndAboveVista(void);

	stdString GetShareMemoryNameSpace(void);
	stdString GetOtherKernelObjNameSpace(void);

private:
	void Parse(void);
	
private:
    OSVERSIONINFOEX m_osVer;
	bool m_bIsEX;
	bool m_bInfoViable;

	enumPlatform m_platform;
	bool m_bX64;

	HMODULE m_hKerDll;

	typedef BOOL (WINAPI* funcProcessIdToSessionId)(DWORD dwProcessId, DWORD* pSessionId);
	funcProcessIdToSessionId m_pFuncProcessIdToSessionId;
	// Readable/Printable OS (platform) name
	stdString m_strName;

	// Readable/Printable version number.
	stdString m_strVersion;
};

extern OSVersion g_osver;

#endif  // #if defined(WIN32)

#endif // __PLATFORM_VERSION_H__
