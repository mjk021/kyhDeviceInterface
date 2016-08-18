/*
[]=========================================================================[]

	Copyright(C) 2000-2005, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	inifile.h

DESC:
	

REVISION:
	2005-01-12 [Apex Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __EPASS_INI_FILE_H__
#define __EPASS_INI_FILE_H__

#include <es/config.h>
#include <map>
#include "base.h"

#include <locale.h>
#if defined(linux)
#include <wchar.h>
#endif

//#ifndef ULONG
//typedef unsigned long		ULONG;
//#endif

namespace epass
{

#define INI_DEFAULT_STRING	_TEXT("DEFAULT_F94174A7C1D94A0E9D57739193CF18CC")
#define INI_DEFAULT_NUMBER	0xFFFFFFFF

typedef std::map<stdString,stdString> mapSection;	// One section
typedef mapSection::value_type mapSection_type;
/*

typedef std::map<stdString, mapSection*> iniFile;// 保存了整个文件的信息
typedef iniFile::value_type iniFile_type;
*/

// Section
class IniSection
{
public:
	IniSection(const stdString &strSectionName);
	~IniSection();

	stdString Name(void)
	{
		return m_strName;
	}

	bool AddKeyValue(const stdString &strKey, const stdString &strValue);
	bool AddKeyValue(ULONG ulKey, const stdString &strValue);

	stdString GetString(const stdString &strKey);
	stdString GetString(ULONG ulKey);

	//added for ini output
	long Output(char *buff);
	//if exists, set; else, add;
	bool SetString(const stdString &strKey,const stdString &strValue);

#ifdef _DEBUG
	void Dump(void);
#endif

private:
	stdString m_strName;

	mapSection m_section;
};

typedef std::map<stdString,IniSection*> mapIniFile;	// A ini file.
typedef mapIniFile::value_type mapIniFile_type;

// Ini file
class IniFile
{
public:
	enum PARSE_RV
	{
		PARSE_ERROR, PARSE_SECTION, PARSE_KEYVALUE, PARSE_COMMENT, PARSE_OTHER
	};

public:
	IniFile();
	~IniFile();

	// Read and parse special file.
	bool Load(const stdString &strFileName);
	// parse the special data.
	bool Load(const char *pszData);

	IniSection *CreateSection(const stdString &strSectionName);
	IniSection *GetSection(const stdString &strName);
	/*
		stdString GetCurrentSectionName(void);
		bool UseSection(const stdString& strSection, bool bCreateWhenNotFound = false);
	*/
#ifdef _DEBUG
	void Dump(void);
#endif
	void ClearUp(void);

	//added for ini output
	long Output(char *buff);	

private:
	static PARSE_RV _ParseLine(const stdString &strLine, stdString &strKey,
			stdString &strValue);

	bool _ProcessLine(const std::string strLineA, IniSection **pCurSection);

private:
	mapIniFile m_file;
protected:
	/*
		iniFile* m_pIFFile;
		mapSection* m_pCurSection;*/
};

#define RSC_BEGIN	0x01
#define RSC_END		0x02
#define RSC_ALL		RSC_BEGIN | RSC_END

// Remove white-space charactor at special location. (Space/TAB, etc.)
// Here special location maybe begin or/and end of a string.
void RemoveSpaceChar(stdString &strString, ULONG ulFlag = RSC_ALL);

// Replace all strFrom inside strLine to strTo.
void ReplaceString(stdString &strLine, const stdString &strFrom,
		const stdString &strTo);

// Convert between MultiByte and WideChar.
#if defined(WIN32)
inline std::string ToStringA(const std::string &strStr, UINT CodePage = CP_ACP)
{
	return strStr;
}
std::string ToStringA(const std::wstring &strStr, UINT CodePage = CP_ACP);

inline std::wstring ToStringW(const std::wstring &strStr,
		UINT CodePage = CP_ACP)
{
	return strStr;
}
std::wstring ToStringW(const std::string &strStr, UINT CodePage = CP_ACP);
#elif defined(linux)
inline std::string ToStringA(const std::string& strStr, unsigned int Init = 1)
{return strStr;}
std::string ToStringA(const std::wstring& strStr, unsigned int Init = 1);

inline std::wstring ToStringW(const std::wstring& strStr, unsigned int Init = 1)
{return strStr;}
std::wstring ToStringW(const std::string& strStr, unsigned int Init = 1);

#endif //#if defined(WIN32)
}	// namespace epass

#endif // __EPASS_INI_FILE_H__
