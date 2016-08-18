/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	ng_crc32.h

DESC:
	

REVISION:
	2004-03-04 [Euphen Liu]
		Created.
[]=========================================================================[]
*/

#ifndef __NG_CRC32_H__
#define __NG_CRC32_H__

#include <es/base_type.h>
//#include <string>

//namespace our_test_namespace
//{

//using std::string;

class CCRC32
{
public:
	CCRC32();
	virtual ~CCRC32();

	unsigned int GetCRC32(const stdString& strBuf);

//#ifdef NGCRC32_TEST
//	NG_ULONG_PTR Test_GetTable(void){return m_table;};
//#endif

private:
	void _InitTable(void);  // Builds lookup table array 
	unsigned int _Reflect(unsigned int ulRef, unsigned char ch);  // Reflects CRC bits in the lookup table

private:
	unsigned int m_table[256];  // Lookup table array 
};

//} // namespace our_test_namespace

#endif // __NG_CRC32_H__

