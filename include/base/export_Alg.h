//////////////////////////////////////////////////////////////////////////
// export_Alg.h   

#if !defined(_EXPORT_ALG_KINGYH_H_)
#define _EXPORT_ALG_KINGYH_H_

#ifndef ALG_API
#if defined(WIN32)
	#define ALG_API __declspec(dllexport)
#else
	#define ALG_API extern
#endif
#endif

//#ifndef CK_MECHANISM
//	/* CK_MECHANISM is a structure that specifies a particular
//	 * mechanism  */
//	typedef struct CK_MECHANISM {
//	  ULONG			mechanism;
//	  void*			pParameter;
//
//	  /* ulParameterLen was changed from CK_USHORT to CK_ULONG for
//	   * v2.0 */
//	  ULONG          ulParameterLen;  /* in bytes */
//	} CK_MECHANISM;
//	typedef CK_MECHANISM * CK_MECHANISM_PTR;
//#endif

typedef struct CRYPT_KEYINFO {
	unsigned char*	pKeyValue;
	ULONG			ulKeyLen;
} CRYPT_KEYINFO;
typedef CRYPT_KEYINFO * CRYPT_KEYINFO_PTR;


//key pair info
typedef struct CRYPT_KEYPAIRINFO {
	int				nKeyPairLen;
	unsigned char	pKey_N[257];
	int				nKey_NLen;
	unsigned char	pKey_E[4];
	int				nKey_ELen;
	unsigned char	pKey_D[257];
	int				nKey_DLen;
	unsigned char	pKey_P[257];
	int				nKey_PLen;
	unsigned char	pKey_Q[257];
	int				nKey_QLen;
	unsigned char	pKey_DP[257];
	int				nKey_DPLen;
	unsigned char	pKey_DQ[257];
	int				nKey_DQLen;
	unsigned char	pKey_DU[257];
	int				nKey_DULen;
} CRYPT_KEYPAIRINFO;
typedef CRYPT_KEYPAIRINFO * CRYPT_KEYPAIRINFO_PTR;

#ifdef __cplusplus
extern "C"
{
#endif

ALG_API ULONG Ex_GenKey(IN CK_MECHANISM_PTR pMech,OUT ULONG *phKey, IN ULONG ulKeyPairLen);
ALG_API ULONG Ex_DelKey(IN ULONG hKey);

ALG_API ULONG Ex_SetKey(IN ULONG hKey, IN CRYPT_KEYINFO_PTR pKeyInfo);
ALG_API ULONG Ex_GetKey(IN ULONG hKey, OUT CRYPT_KEYINFO_PTR pKeyInfo);

ALG_API ULONG Ex_GetKeyPair(IN ULONG hKey, OUT CRYPT_KEYPAIRINFO_PTR pKeyPairInfo);
ALG_API ULONG Ex_SetKeyPair(IN CRYPT_KEYPAIRINFO_PTR pKeyPairInfo, IN OUT ULONG *phKey);

ALG_API ULONG Ex_EncryptInit(IN CK_MECHANISM_PTR pMech, IN ULONG hKey);
ALG_API ULONG Ex_Encrypt(IN ULONG hKey, IN BYTE* pData, IN ULONG ulDataLen, OUT BYTE* pEncryptedData, OUT ULONG* pulEncryptedDataLen);
ALG_API ULONG Ex_EncryptUpdate(IN ULONG hKey, IN BYTE* pPart, IN ULONG ulPartLen, OUT BYTE* pEncryptedPart, OUT ULONG * pulEncryptedPartLen);
ALG_API ULONG Ex_EncryptFinal(IN ULONG hKey, OUT BYTE * pLastEncryptedPart, OUT ULONG * pulLastEncryptedPartLen);

ALG_API ULONG Ex_DecryptInit(IN CK_MECHANISM_PTR pMech, IN ULONG hKey);
ALG_API ULONG Ex_Decrypt(IN ULONG hKey, IN BYTE* pEncryptedData, IN ULONG ulEncryptedDataLen, OUT BYTE* pData, OUT ULONG* pulDataLen);
ALG_API ULONG Ex_DecryptUpdate(IN ULONG hKey, IN BYTE* pEncryptedPart, IN ULONG ulEncryptedPartLen, OUT BYTE* pPart, OUT ULONG* pulPartLen);
ALG_API ULONG Ex_DecryptFinal(IN ULONG hKey, OUT BYTE* pLastPart, OUT ULONG* pulLastPartLen);

ALG_API ULONG Ex_DigestInit(IN CK_MECHANISM_PTR pMech, OUT ULONG *phDig);
ALG_API ULONG Ex_Digest(IN ULONG hDig, IN BYTE* pData, IN ULONG ulDataLen, OUT BYTE* pDigest, OUT ULONG* pulDigestLen);
ALG_API ULONG Ex_DigestUpdate(IN ULONG hDig, IN BYTE* pPart, IN ULONG ulPartLen);
ALG_API ULONG Ex_DigestFinal(IN ULONG hDig, OUT BYTE* pDigest, OUT ULONG* pulDigestLen);
ALG_API ULONG Ex_DestroyDigest(IN ULONG hDig);

ALG_API ULONG Ex_SignInit(IN CK_MECHANISM_PTR pMech, IN ULONG hKey);
ALG_API ULONG Ex_Sign(IN ULONG hKey, IN BYTE* pData, IN ULONG ulDataLen, OUT BYTE* pSignature, OUT ULONG* pulSignatureLen);
ALG_API ULONG Ex_SignUpdate(IN ULONG hKey, IN BYTE* pPart, IN ULONG ulPartLen);
ALG_API ULONG Ex_SignFinal(IN ULONG hKey, OUT BYTE* pSignature, OUT ULONG* pulSignatureLen);

ALG_API ULONG Ex_VerifyInit(IN CK_MECHANISM_PTR pMech, IN ULONG hKey);
ALG_API ULONG Ex_Verify(IN ULONG hKey, IN BYTE* pData, IN ULONG ulDataLen, IN BYTE* pSignature, IN ULONG ulSignatureLen);
ALG_API ULONG Ex_VerifyUpdate(IN ULONG hKey, IN BYTE* pPart, IN ULONG ulPartLen);
ALG_API ULONG Ex_VerifyFinal(IN ULONG hKey, IN BYTE* pSignature, IN ULONG ulSignatureLen);

ALG_API ULONG Ex_GenRandomBytes(IN BYTE* pRand, ULONG ulRandLen);

#ifdef __cplusplus
}
#endif

#endif	// !defined(_EXPORT_ALG_KINGYH_H_)