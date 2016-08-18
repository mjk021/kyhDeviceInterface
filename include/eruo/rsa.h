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

int RSAPublicEncrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
    R_RSA_PUBLIC_KEY *, R_RANDOM_STRUCT *));
int RSAPrivateEncrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
    R_RSA_PRIVATE_KEY *));
int RSAPublicDecrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
    R_RSA_PUBLIC_KEY *));
int RSAPrivateDecrypt PROTO_LIST ((unsigned char *, unsigned int *, unsigned char *, unsigned int,
    R_RSA_PRIVATE_KEY *));


int VerifyKeyPair1(int BitLen,unsigned char *pModulus,unsigned char *pExponent,
				   unsigned char *pPriExponent,unsigned char *pCoefficient,
				   unsigned char *pPrime1,unsigned char *pPrime2,
				   unsigned char *pExponent1,unsigned char *pExponent2);
int VerifyKeyPair(int BitLen,unsigned char *pModulus,unsigned char *pExponent,
				   unsigned char *pPriExponent,unsigned char *pCoefficient,
				   unsigned char *pPrime1,unsigned char *pPrime2,
				   unsigned char *pExponent1,unsigned char *pExponent2);
int PrivateKeyEncry(int BitLen,unsigned char *pModulus,unsigned char *pPrime1,unsigned char *pPrime2,
				  unsigned char *pExponent1,unsigned char *pExponent2,
				  unsigned char *pCoefficient,

				  unsigned char *pSourceData,unsigned int SrcDataLen,
				  unsigned char *pEncryResult,unsigned int *pEncryResultLen);
int PrivateKeyDecry(int BitLen,unsigned char *pModulus,unsigned char *pPrime1,unsigned char *pPrime2,
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
		char *pText64,			// in: Base64�ַ���
		unsigned long dwText64Len,		// in: ���봮����
		unsigned char  *pbData,			// out: �����������
		unsigned long *pdwDataLen);		// in/out:���������������
unsigned int Base64ToInt(unsigned int c);
int BytesToBase64Str(
					BYTE *pbData,			//in �������ֽ�����
					unsigned long dwDataLen,		//in �������ֽ������ֽ���
					char *pText64,			//out Base64�ַ���
					unsigned long *pdwText64Len);	//in/out Base64�ַ�������
void ConvertPriKey(int BitLen,unsigned char *Prime1,unsigned char *Prime2,
				   unsigned char *pModulus,unsigned char *pPubExponent,
				   unsigned char *pPriExponent,
				   unsigned char *pExponent1,unsigned char *pExponent2,
				   unsigned char *pCoefficient1,unsigned char *pCoefficient2);


#ifdef __cplusplus
}
#endif
