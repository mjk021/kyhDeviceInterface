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

// ����ʹ��Ӣ��
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

	// ָ�������ļ�����·��
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

	bool m_bUseBuiltin;		// �Ƿ�ʹ������������Դ

	bool m_bInitialized;
	bool m_bValid;

	stdString m_strPath;
	stdString m_strPrefix;

	ULONG m_ulCodePage;
	stdString m_strLanguageName;

	ULONG m_ulFontHeight;
	stdString m_strFontFaceName;
};

// ���������ļ���·�������û��ָ�������ǿ�ִ�г����ļ�������·��
void ml_setPath(const stdString& strPath);

// ���������ļ���ǰ׺�����û��ָ�������ǿ�ִ�г����ļ����ļ���(������չ��)
void ml_setPrefix(const stdString& strPrefix);

// ����/��ȡʹ�õ����ԵĴ���(10����)�������������(2052)��Ӣ��(1033)�ȡ�
void ml_setCodePage(ULONG ulCodePage);
ULONG ml_getCodePage(void);

stdString ml_getLanguageName(void);

// ����ָ���������ļ������磬��ǰ�û��趨Ϊ�������Ļ��������ص��ļ���
// "prefix_2052.lng"������2052(0x0804)�Ǽ������ĵ�LanguageID�������
// ��ʧ�ܣ��ͻ�ʹ���������ԡ�
// ���û�е��ã���ʹ���������ԡ�
bool ml_load(const stdString& strLangFile);

// ����ָ����������Դ���������ʧ�ܣ��ͻ�ʹ���������ԡ�
// ���û�е��ã���ʹ���������ԡ�
bool ml_loadData(const char *pszData);

// ��ȡ���õ���������
stdString ml_getFontFaceName(void);

// ��ȡ���õ�����߶�
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
