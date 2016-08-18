/*
[]=========================================================================[]

	Copyright(C), Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	debug.h

DESC:
	Some important macro for debug.

REVISION:
	[2006-06-15 Apex Liu] Created.
[]=========================================================================[]
*/

#ifndef __ES_DEBUG_H__
#define __ES_DEBUG_H__

#include <es/base_type.h>

#if defined(WIN32)
#	include <windows.h>
#endif

#ifndef _DEBUG
#	if !defined(ES_CSP11_CATCH_EXCEPTION)
#		error When build release version, you should define 'ES_CSP11_CATCH_EXCEPTION'.
#	endif
#endif

#ifndef ES_CSP11_CATCH_EXCEPTION
	#define ES_TRY {for(;;){
	#define ES_CATCH_ALL break;} if(false){
	#define ES_LEAVE break;
	#define ES_ENDTRY }}
#else
	#define ES_TRY {try{for(;;){
	#define ES_CATCH_ALL break;}}catch(...){
	#define ES_LEAVE break;
	#define ES_ENDTRY }}
#endif // ES_CSP11_CATCH_EXCEPTION


#if 0

ES_TRY
{
}
ES_CATCH
{
}

{
	for(;;)
	{

		// Main code here.

		break;
	}
	if(false)
	{
	}
}
#endif






// #ifdef ES_CSP11_TEST_TIME
// class TestTimeUse
// {
// public:
// 	TestTimeUse(const stdString& strInfo)
// 	{
// 		m_strInfo = strInfo;
// 		m_ulStartTime = GetTickCount();
// 	}
// 	~TestTimeUse()
// 	{
// 		stdOStrStream oss;
// 		oss << "function " << m_strInfo << "() execute within "
// 			<< GetTickCount() - m_ulStartTime << " ms";
// 		ESLOG_INF((oss.str().c_str()));
// 	}
// 
// private:
// 	stdString m_strInfo;
// 	unsigned long m_ulStartTime;
// };
// 
// #define TEST_TIME_USE(func)	TestTimeUse testtimeuse_##func(#func);
// 
// #else
// 
// #define TEST_TIME_USE(func)	
// 
// #endif






#endif // __ES_DEBUG_H__

