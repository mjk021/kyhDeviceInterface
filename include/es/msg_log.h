/*
[]=========================================================================[]

	Copyright(C) 2000-2003, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	msg_log.h

DESC:
	Èç¹û¶¨ÒåÁËESLOG_NOT_LOG£¬ÄÇÃ´ËùÓÐµÄÐÅÏ¢¶¼²»»á¼ÇÂ¼¡£Êµ¼ÊÉÏ£¬Õâ¸öµ÷ÊÔÐÅÏ¢
	Ä£¿é¸ù±¾¾Í²»»á±»±àÒëÁ¬½Ó¡£
	Èç¹û¶¨ÒåÁËESLOG_NOT_LOG_BIN£¬ÄÇÃ´ËùÓÐµÄ¶þ½øÖÆÐÅÏ¢¶¼²»»á±»¼ÇÂ¼¡£ÔÚÕýÊ½·¢
	ÐÐÊ±(¼´£¬¶¨ÒåÁËESLOG_RELEASE)£¬Õâ¸öÖµ×Ô¶¯±»¶¨Òå

	ÔÚÕýÊ½·¢ÐÐÊ±£¬ÇëÔÚ°üº¬msg_log.hÎÄ¼þÖ®Ç°¶¨ÒåESLOG_RELEASEÖµ¡£
	µ«ÊÇ£¬²»ÒªÔÚpch.hÍ·ÎÄ¼þÖÐ¶¨Òå£¬ÇëÖ±½ÓÔÚmakeÎÄ¼þ(Æä·Çwin32µÄOS)»òÏîÄ¿ÅäÖÃÎÄ¼þ(dsp-win32)ÖÐ¶¨Òå

	ÓÃ·¨Ê¾Àý(Òª×¢ÒâµÄÊÇ£¬Ö§³Ö±ä³¤²ÎÊýµÄ²¿·ÖÐèÒªÓÃÀ¨ºÅÀ¨ÆðÀ´)£º
		ESLOG_INF(("Information string, %d, %s", 1, "abc"));
		ESLOG_WRN(("Warning information, %d, %s", 1, "abc"));
		ESLOG_ERR(("Error !! (0x%08X)", GetLastError()));

	ÔÚº¯ÊýÈë¿Ú´¦Ê¹ÓÃFUNC_INºê£¬¿ÉÒÔÔÚº¯Êý½øÈëºÍÍË³öÊ±¼ÇÂ¼ÐÅÏ¢£¬ÀýÈç£º
		void foo(void)
		{
			
		}
	ÄÇÃ´²»¹ÜÕâ¸öº¯ÊýÓÐ¶àÉÙ¸ö³ö¿Úµã£¬Ö»ÒªÀë¿ªÕâ¸öº¯Êý£¬¾Í»á¼ÇÂ¼ÏàÓ¦ÐÅÏ¢¡£

REVISION:
	[2006-06-20 Apex Liu] Created.

[]=========================================================================[]
*/

#ifndef __MSG_LOG_H__
#define __MSG_LOG_H__

#ifndef _DEBUG
#	if !( defined(ESLOG_NOT_LOG) ||  defined(ESLOG_RELEASE) || defined(ESLOG_RELEASE_WITHLOG) )
#		error When build release version, you should define 'ESLOG_NOT_LOG' or 'ESLOG_RELEASE' or 'ESLOG_RELEASE_WITHLOG' in project settings.
#	endif
#endif

#ifndef ESLOG_NOT_LOG

#include <es/base_type.h>
#include <es/es_typedef.h>
#include <es/thread_mutex.h>
 
// release end
#if defined(ESLOG_RELEASE)
	#define ESLOG_NOT_LOG_BIN
	#define ESLOG_NOT_LOG_INF
	#define ESLOG_NOT_LOG_WRN
	#define ESLOG_NOT_LOG_FUNC
	#define ESLOG_NOT_LOG_ERR

#elif defined(ESLOG_RELEASE_WITHLOG)
// Log all message.
#endif

#if defined(WIN32)
#	pragma warning( disable : 4786)
#	include <windows.h>
#elif defined(linux) || defined(macintosh)
#	include <errno.h>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	using namespace std;
#else
#	error "Only support WIN32 / LINUX / Mac OS X."
#endif

//#include <map>
//#include <string>
//using namespace std;

// If you want to use our message log system, you must specify which level
// message you want to log:

#define ESLOG_LEVEL_NONE						0x1000
#define ESLOG_LEVEL_ERROR						0x0100
#define ESLOG_LEVEL_WARNING						0x0010
#define ESLOG_LEVEL_INFO						0x0001
#define ESLOG_LEVEL_UNKNOWN						0x0000

#define ESLOG_MAX_BUFFER_SIZE					512



#define OUT_MODE_TO_FILE	1
#define OUT_MODE_TO_CONSOLE	2
#define OUT_MODE_TO_NEW_CONSOLE	4
#define OUT_MODE_TO_DBG_VIEW	8



#define OUT_FLAG_PROCESS_THREAD_ID		1
#define OUT_FLAG_TITLE			2
#define OUT_FLAG_TIME			4
#define OUT_FLAG_COUNT			8

#ifndef  INTERLOCK
#define INTERLOCK 
ES_LONG InterlockedIncrement(ES_LONG_PTR ref);
#endif

// global variable declared in msg_log.h
// this variable switch the output
// if it is true, output can be seen if they meet the level in configuration fileno
// if it is false, all the output are closed , include error message.
// if it not assigned again, the default value is false
// In daemon service, if you close the 0(std-in), 1(std-out), 2(std-err), you should not
// assign this value to true, otherwise the service will end with "Broken pipe" error.

class MessageLogger
{
// Ê¹ÓÃÕâÖÖ·½Ê½À´¼òµ¥ÊµÏÖ'µ¥¼þÄ£Ê½'
friend MessageLogger& get_msg_logger(void);

public:
	MessageLogger();
	virtual ~MessageLogger();

	//void SetTagName(const char* szTagName){m_strTagName = szTagName;}
	
	void LogString(const char* szFormat, ...);
	void LogBinary(const char* msg, const unsigned char* pBuf, unsigned long ulSize);
	void SetLevel(unsigned long ulLevel){m_ulLevel = ulLevel;}
	void SetPosition(const char* szFileName, unsigned long ulLineNumber);

	// ÔËÐÐÊ±ÆÁ±Î±ê×¼Êä³ö
	void LogToStd(bool bEnable = true){m_bLogToStd = bEnable;}
	//void _DoLog(const char* szLog);
	void _DoLog(const char* szLog,...);

private:
	//void _DoLog(const stdString& strLog);
	//void _DoLog(const char* szLog,...);

//	void _Lock(void){EnterCriticalSection(&m_threadLocker);}
//	void _Unlock(void){LeaveCriticalSection(&m_threadLocker);}

	unsigned long m_ulLevel;
	char m_PostionString[MAX_PATH];
	char m_szBuf[ESLOG_MAX_BUFFER_SIZE];

	HANDLE m_hStdErr;
	bool m_bLogToStd;

	bool m_bLogToDbgView;
public:
	char m_LogFileName[MAX_PATH];
	DWORD m_flag;
	long m_Count;
	long m_tabCount;
	DWORD m_outMode;
	bool m_bComment;
//	CRITICAL_SECTION m_threadLocker;
};


MessageLogger& get_msg_logger(void);

#define ESLOG_MAX_FUNCNAME_SIZE		128

class MessageLoggerFuncInOut
{
public:
	MessageLoggerFuncInOut(const char* pszFuncName, bool IsService = false);
	virtual ~MessageLoggerFuncInOut();
private:
	long m_level;
	unsigned long m_ulStartTime;
	char m_szFuncName[ESLOG_MAX_FUNCNAME_SIZE];
	bool m_IsService;
	ThreadMutex m_tMutex;
	
};

#define ESLOG_DISABLE_OUTPUT                                        \
	{                                                               \
		MessageLogger& message_logger = get_msg_logger();           \
		message_logger.EnableOutput(false);                         \
	}
#define ESLOG_ENABLE_OUTPUT                                         \
	{                                                               \
		MessageLogger& message_logger = get_msg_logger();           \
		message_logger.EnableOutput(true);                          \
	}


#if defined(WIN32)
#	define ESLOG_LOG_BEGIN		{do{                                \
		DWORD dwErrorCode = GetLastError();

#	define ESLOG_LOG_END		                                    \
			SetLastError(dwErrorCode);                              \
		}while(0);                                                  \
	}

#elif defined(linux) || defined(macintosh)

#	define ESLOG_LOG_BEGIN		{do{\
 	int error_no = errno;
#	define ESLOG_LOG_END		errno = error_no;}while(0);}

#endif

#ifndef ESLOG_NOT_LOG_FUNC
#define ESLOG_FUNC(fmt)                                            \
	ESLOG_LOG_BEGIN                                                \
		MessageLogger& message_logger = get_msg_logger();          \
		message_logger.SetLevel(ESLOG_LEVEL_INFO);                 \
		message_logger.LogString fmt;                              \
	ESLOG_LOG_END
#else
#define ESLOG_FUNC(fmt)
#endif // ESLOG_NOT_LOG_FUNC

#ifndef ESLOG_NOT_LOG_INF
#define ESLOG_INF(fmt)                                             \
	ESLOG_LOG_BEGIN                                                \
		MessageLogger& message_logger = get_msg_logger();          \
		message_logger.SetLevel(ESLOG_LEVEL_INFO);                 \
		message_logger.LogString fmt;                              \
	ESLOG_LOG_END
#else // #ifndef ESLOG_NOT_LOG_INF
	#define ESLOG_INF(fmt)
#endif // #ifndef ESLOG_NOT_LOG_INF

#ifndef ESLOG_NOT_LOG_WRN
#define ESLOG_WRN(fmt)                                             \
	ESLOG_LOG_BEGIN                                                \
		MessageLogger& message_logger = get_msg_logger();          \
		message_logger.SetLevel(ESLOG_LEVEL_WARNING);              \
		message_logger.LogString fmt;                              \
	ESLOG_LOG_END
#else // #ifndef ESLOG_NOT_LOG_WRN
#define ESLOG_WRN(fmt)
#endif // #ifndef ESLOG_NOT_LOG_WRN


// ×¢Òâ£ºÔÚµ÷ÊÔ°æ°üº¬ÁË³ö´íµÄµØ·½µÄÎÄ¼þÃûºÍÐÐºÅ£¬µ«ÊÇÔÚÕýÊ½·¢ÐÐÊ±£¬¼´Ê¹ÔÊÐí
// ¼ÇÂ¼µ÷ÊÔÐÅÏ¢£¬Ò²²»Ó¦¸Ã¼ÇÂ¼ÎÄ¼þÃûºÍÐÐºÅ¡£ËùÒÔÕâÀï×÷ÌØ±ð´¦Àí
#ifndef ESLOG_NOT_LOG_ERR

#ifdef ESLOG_RELEASE
// ÕýÊ½·¢ÐÐ°æ²»¼ÇÂ¼ÎÄ¼þÃûºÍÐÐºÅ
#define ESLOG_ERR(fmt)                                           \
	ESLOG_LOG_BEGIN                                              \
		MessageLogger& message_logger = get_msg_logger();        \
		message_logger.SetLevel(ESLOG_LEVEL_ERROR);              \
		message_logger.LogString fmt;                            \
	ESLOG_LOG_END
#else // #ifdef ESLOG_RELEASE
#define ESLOG_ERR(fmt)                                           \
	ESLOG_LOG_BEGIN                                              \
		MessageLogger& message_logger = get_msg_logger();        \
		message_logger.SetLevel(ESLOG_LEVEL_ERROR);              \
		message_logger.SetPosition(__FILE__, __LINE__);           \
		message_logger.LogString fmt;                            \
	ESLOG_LOG_END
#endif // #ifdef ESLOG_RELEASE

#else // #ifndef ESLOG_NOT_LOG_ERR	
	#define ESLOG_ERR(fmt)
#endif // #ifndef ESLOG_NOT_LOG_ERR

#ifndef ESLOG_NOT_LOG_BIN
#define ESLOG_BIN(msg, buf, size)                                       \
	ESLOG_LOG_BEGIN                                                \
		MessageLogger& message_logger = get_msg_logger();          \
		message_logger.SetLevel(ESLOG_LEVEL_INFO);                 \
		message_logger.LogBinary(msg, buf, size);                       \
	ESLOG_LOG_END
#else // #ifndef ESLOG_NOT_LOG_BIN
	#define ESLOG_BIN(msg, buf, size)
#endif // #ifndef ESLOG_NOT_LOG_BIN

//#ifndef ESLOG_NOT_LOG_FUNC
//#define FUNC_IN(func)	MessageLoggerFuncInOut msgloggerinout_##func(#func)
//#else
//#define FUNC_IN(func)	
//#endif

#ifndef ESLOG_NOT_LOG_FUNC
#define FUNC_IN(func)	MessageLoggerFuncInOut msgloggerinout(func)
#else
#define FUNC_IN(func)	
#endif

#else // #ifdef ESLOG_DO_LOG

	#define FUNC_IN(sz)
	#define ESLOG_BIN(msg, buf, size)
	#define ESLOG_INF(fmt)
	#define ESLOG_WRN(fmt)
	#define ESLOG_ERR(fmt)
				
	#define ESLOG_DISABLE_OUTPUT
	#define ESLOG_ENABLE_OUTPUT

#endif // #ifndef ESLOG_NOT_LOG

#endif // __MSG_LOG_H__

// EOF
