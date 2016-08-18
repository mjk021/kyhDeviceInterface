/*
[]======================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	security_attr.h

DESC:
	

REVISION:
	[2006-06-20 Apex Liu] Created.

[]======================================================================[]
*/

#ifndef __SECURITY_ATTRIBUTES_HOLDER_H__
#define __SECURITY_ATTRIBUTES_HOLDER_H__

// #include <entersafe/config.h>
// #include <entersafe/noncopyable.h>

#if defined(WIN32)

#include <es/platform_ver.h>
#include <windows.h>

//namespace our_test_namespace
//{

class SecurityAttributes
{
public:
	SecurityAttributes();
	virtual ~SecurityAttributes();

	operator SECURITY_ATTRIBUTES*();

public:
	PACL GetSACL(){return m_pSACL;}
private:
	SecurityAttributes(const SecurityAttributes& rhs);
	SecurityAttributes& operator=(const SecurityAttributes& rhs);

protected:
	PSECURITY_DESCRIPTOR m_pTempSD;
	char m_pSD[SECURITY_DESCRIPTOR_MIN_LENGTH];
	SECURITY_ATTRIBUTES	m_sa;
	HMODULE m_hAdv32Dll;
	PACL m_pSACL;
	//OSVersion m_WinVer;
};

//} // namespace our_test_namespace

//#else
//#	error "This class support Windows Platform only !!"
//#endif	// #if defined(WIN32)
extern SecurityAttributes g_SecAttr;
#endif //if defined(WIN32)
#endif // __SECURITY_ATTRIBUTES_HOLDER_H__
