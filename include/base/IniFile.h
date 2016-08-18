#include "typedefine.h"
#ifndef __INIFILE__
#define __INIFILE__
class CIniFileEx
{
public:
	FILE *m_fp;
	int m_fileLen;
	char *m_pBuf;
public:
	CIniFileEx(){
		m_fp=NULL;
		m_fileLen=0;
		m_pBuf=NULL;
	};
	~CIniFileEx(){
		if(m_fp)
		{
			fclose(m_fp);
			m_fp=NULL;
		}
		if(m_pBuf)
		{
			free(m_pBuf);
			m_pBuf=NULL;
		}
		m_fileLen=0;
	};
	BOOL LoadFile(const char *name,const char *mode);
	BOOL GetString(const char *title,const char *key,char *pOut,int maxlen);
	BOOL GetInt(const char *title,const char *key,int *pOut);


};
#endif