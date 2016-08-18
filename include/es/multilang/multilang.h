/*
[]=========================================================================[]

	Copyright(C) 2000-2005, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	multilang.h

DESC:
	

REVISION:
	2005-01-12 [Apex Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __MULTI_LANGUAGE_H__
#define __MULTI_LANGUAGE_H__

#include "inifile.h"


namespace epass
{

#define INVALID_CODE_PAGE		0x1FFFFFFF

// 内置使用英语
#define BUILTIN_CODE_PAGE		1033
#define BUILTIN_LANG_NAME		_TEXT("English")
#define BUILTIN_FONT_FACE		_TEXT("Tahoma")
#define BUILTIN_FONT_HEIGHT		8

class MultiLanguage
{
public:
	MultiLanguage();
	virtual ~MultiLanguage();

	bool Initialize(void);

//	bool IsValid(void){return m_bValid;}
	bool IsInitialized(void){return (m_bValid || m_bInitialized);}
	bool IsUseBuiltin(void){return m_bUseBuiltin;}

	// 指定语言文件所在路径
	void SetPath(const stdString& strPath);
	void SetCodePage(ULONG ulCodePage){m_ulCodePage = ulCodePage;}
	void SetPrefix(const stdString& strPrefix){m_strPrefix = strPrefix;}

	// Load from a file.
	bool Load(const stdString& strFileName);

	bool Load(const stdString& strPrefix, ULONG ulLangId);

	bool LoadData(const char *pszData);

	// GetString() will get special string from current using section.
	// If not found, search 'strings' section.
	stdString GetString(const stdString& strId, const stdString& strDefault = INI_DEFAULT_STRING);
	stdString GetString(ULONG ulId, const stdString& strDefault = INI_DEFAULT_STRING);

	ULONG GetNumber(const stdString& strId, ULONG ulDefault = INI_DEFAULT_NUMBER);
	ULONG GetNumber(ULONG ulId, ULONG ulDefault = INI_DEFAULT_NUMBER);

	ULONG GetCodePage(void){return m_ulCodePage;}
	stdString GetLanguageName(void){return m_strLanguageName;}
	ULONG GetFontHeight(void){return m_ulFontHeight;}
	stdString GetFontFaceName(void){return m_strFontFaceName;}

#ifdef _DEBUG
	void Dump(void){m_ini.Dump();}
#endif

protected:
	// This function must support twice call.
	//virtual bool _LoadDefault(unsigned char* pBuf, unsigned long* pulSize) = 0;

	bool _UseSection(const stdString& strSectionName);

	bool _PostLoad(void);

protected:
	IniFile m_ini;
	IniSection* m_pCurSection;

	bool m_bUseBuiltin;		// 是否使用内置语言资源

	bool m_bInitialized;
	bool m_bValid;

	stdString m_strPath;
	stdString m_strPrefix;

	ULONG m_ulCodePage;
	stdString m_strLanguageName;

	ULONG m_ulFontHeight;
	stdString m_strFontFaceName;
};

// 设置语言文件的路径，如果没有指定，就是可执行程序文件的所在路径
void ml_setPath(const stdString& strPath);

// 设置语言文件的前缀，如果没有指定，就是可执行程序文件的文件名(不含扩展名)
void ml_setPrefix(const stdString& strPrefix);

// 设置/获取使用的语言的代码(10进制)，例如简体中文(2052)，英文(1033)等。
void ml_setCodePage(ULONG ulCodePage);
ULONG ml_getCodePage(void);

stdString ml_getLanguageName(void);

// 加载指定的语言文件，例如，当前用户设定为简体中文环境，加载的文件是
// "prefix_2052.lng"，这里2052(0x0804)是简体中文的LanguageID。如果加
// 载失败，就会使用内置语言。
// 如果没有调用，就使用内置语言。
bool ml_load(const stdString& strLangFile);

// 加载指定的语言资源，如果加载失败，就会使用内置语言。
// 如果没有调用，就使用内置语言。
bool ml_loadData(const char *pszData);

// 获取设置的字体名称
stdString ml_getFontFaceName(void);

// 获取设置的字体高度
ULONG ml_getFontHeight(void);

stdString __(ULONG ulId, const TCHAR* szDefault);
stdString ___(const stdString& strId, const TCHAR* szDefault);

MultiLanguage& ml_getInstance(void);
#ifdef WIN32
stdString __(ULONG ulId);

void ml_loadDlgStr(HWND hDlg, UINT nIDTemplete);

void ml_loadMenuStr(const stdString& strSection, HMENU hMenu);

#endif

ULONG SetLang(char *reg_name,char *product_name);
}	// namespace epass

#endif // __MULTI_LANGUAGE_H__
