/*
	RSA.H - header file for RSA.C

    Copyright (c) J.S.A.Kapp 1994 - 1996.

	RSAEURO - RSA Library compatible with RSAREF 2.0.

	All functions prototypes are the Same as for RSAREF.
	To aid compatiblity the source and the files follow the
	same naming comventions that RSAREF uses.  This should aid
        direct importing to your applications.

	This library is legal everywhere outside the US.  And should
	NOT be imported to the US and used there.

	RSA Routines Header File.

	Revision 1.00 - JSAK.
*/

#ifdef __cplusplus
extern "C" {
#endif



int VerifyKeyPair1(int BitLen,unsigned char *pModulus,unsigned char *pExponent,
				   unsigned char *pPriExponent,unsigned char *pCoefficient,
				   unsigned char *pPrime1,unsigned char *pPrime2,
				   unsigned char *pExponent1,unsigned char *pExponent2);
int VerifyKeyPair(int BitLen,unsigned char *pModulus,unsigned char *pExponent,
				   unsigned char *pPriExponent,unsigned char *pCoefficient,
				   unsigned char *pPrime1,unsigned char *pPrime2,
				   unsigned char *pExponent1,unsigned char *pExponent2);
int PrivateKeyEncry(int BitLen,unsigned char *pPrime1,unsigned char *pPrime2,
				  unsigned char *pExponent1,unsigned char *pExponent2,
				  unsigned char *pCoefficient,

				  unsigned char *pSourceData,unsigned int SrcDataLen,
				  unsigned char *pEncryResult,unsigned int *pEncryResultLen);
int PrivateKeyDecry(int BitLen,unsigned char *pPrime1,unsigned char *pPrime2,
				  unsigned char *pExponent1,unsigned char *pExponent2,
				  unsigned char *pCoefficient,
				  unsigned char *pSourceData,unsigned int SrcDataLen,
				  unsigned char *pEncryResult,unsigned int *pEncryResultLen);
int PublicKeyEncry(int BitLen,unsigned char *pModulus,unsigned char *pExponent,
				  unsigned char *pSourceData,unsigned int SrcDataLen,
				  unsigned char *pEncryResult,unsigned int *pEncryResultLen);
int PublicKeyDecry(int BitLen,unsigned char *pModulus,unsigned char *pExponent,
				  unsigned char *pSourceData,unsigned int SrcDataLen,
				  unsigned char *pEncryResult,unsigned int *pEncryResultLen);
int GenKeyPair(int BitLen,R_RSA_PUBLIC_KEY *pubKey,	R_RSA_PRIVATE_KEY *priKey);
int Base64StrToBytes(	
		char *pText64,			// in: Base64字符串
		unsigned long dwText64Len,		// in: 输入串长度
		unsigned char  *pbData,			// out: 二进制输出串
		unsigned long *pdwDataLen);		// in/out:二进制输出串长度
unsigned int Base64ToInt(unsigned int c);
int __stdcall BytesToBase64Str(
					BYTE *pbData,			//in 二进制字节数组
					unsigned long dwDataLen,		//in 二进制字节数组字节数
					char *pText64,			//out Base64字符串
					unsigned long *pdwText64Len);	//in/out Base64字符串长度
#ifdef __cplusplus
}
#endif
