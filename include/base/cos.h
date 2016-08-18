#ifndef __COS_INTERFACE__
#define __COS_INTERFACE__
#include "typedefine.h"
//PIN���Ͷ���
#define CKU_SO    0
#define CKU_USER  1
//COS�ļ�����Ȩ�޶���

#define RIGHT_FREE					0x00
#define RIGHT_READ_USER_PIN			0x01
#define RIGHT_WRITE_USER_PIN		0x02
#define RIGHT_USE_USER_PIN			0x04
#define RIGHT_READ_FORBIDDEN		0x08



/*
#define RIGHT_FREE			0
#define RIGHT_PIN_PROTECT	1
#define RIGHT_FORBIDDEN		2
*/
//COS�ļ����Ͷ���
#define FILE_TYPE_BIN	1
#define FILE_TYPE_PRI_KEY	2
#define FILE_TYPE_PUB_KEY	3
#define FILE_TYPE_PIN	4

//#define COS_TYPE_COUNT			2

//#define COS_TYPE_DLL_NAME1		"zc100"
//#define COS_TYPE_DLL_NAME2		"z2000"


#ifdef __cplusplus
extern "C" {
#endif
#define SM4_ALG_ID  0x0
#define ECB_MODE	0
#define CBC_MODE	1

#define ENCRYPTION  0
#define DECRYPTION  1

#define SM4_ALG 0x02
#define SM1_ALG 0x03

DEV_RES Z_Test(
    DEV_HANDLE devHandle, 
    bool isEnc,
    BYTE alg, 
    BYTE mode, 
    BYTE *pIv, 
    BYTE *pDataIn, 
    ULONG ulDataInLen,
    BYTE*pDataOut,
    ULONG* pulDataOutLen
    );

//���ضԳ���ʱ��Կ
DEV_RES Z_LoadSymmetricKey(
    DEV_HANDLE devHandle, 
    BYTE alg, 
    BYTE *key,
    const BYTE *pAdminPin, 
    ULONG AdminPinLen
);
//�Գ��㷨����
DEV_RES Z_SymmetricAlgCalc(
    DEV_HANDLE devHandle, 
    bool isEnc,
    BYTE alg, 
    BYTE mode, 
    BYTE *pIv, 
    BYTE *pDataIn, 
    ULONG ulDataInLen,
    BYTE*pDataOut,
    ULONG* pulDataOutLen
    );

//Z_CreateFileSys
//�����ļ�ϵͳ
DEV_RES Z_CreateFileSys(DEV_HANDLE devHandle, const BYTE *pAdminPin, ULONG AdminPinLen);

//Z_SM4Calc
DEV_RES Z_SM4Calc(DEV_HANDLE devHandle,BOOL isEnc,BYTE* key,BYTE*pDataIn,ULONG ulDataInLen,BYTE*pDataOut,ULONG* pulDataOutLen);

//���ƣ�Z_SelectMF
//����: ѡ�����ļ�
//IN��DeviceHandle���豸���
//���أ��������
DEV_RES Z_SelectMF(DEV_HANDLE devHandle);
//���ƣ�Z_SelectEF
//����: ѡ��EF�ļ�
//IN��DeviceHandle���豸���
//IN��FileID���ļ���ʶ
//���أ��������
DEV_RES Z_SelectEF(DEV_HANDLE devHandle,WORD FileID);
//���ƣ�Z_SelectDF
//����: ѡ��Ŀ¼�ļ�
//IN��DeviceHandle���豸���
//IN��FileID���ļ���ʶ
//���أ��������
DEV_RES Z_DeleteDF(DEV_HANDLE devHandle,WORD FileID);
//���ƣ�Z_GetErrMsg
//����: ��ô�������Ӧ����Ϣ
//IN��code���������
//OUT��msg�����ص���Ϣ,
//ע��msgĬ�ϲ�����256���ַ�
//���أ��������
void Z_GetErrMsg(WORD code,char *msg);
//���ƣ�Z_CreateFile
//����: �����ļ�
//IN��DeviceHandle���豸���
//IN��FileType���ļ�����
//IN��FileID���ļ���ʶ
//IN��Size���ļ�����
//IN��ReadRight���ļ���Ȩ��
//IN��WriteRight���ļ�дȨ��
//IN��UseRight���ļ�ʹ��Ȩ��
//���أ��������
DEV_RES Z_CreateEF(DEV_HANDLE devHandle,ULONG FileType,WORD FileID,ULONG Size,ULONG Right);
//���ƣ�Z_VerifyPin
//����: ��֤PIN
//IN��DeviceHandle���豸���
//IN��PinType��PIN����,����CKU_USER,CKU_SO
//IN��PinCode��PINֵ
//IN��PinLen��PINֵ����
//���أ��������
DEV_RES Z_VerifyPin(DEV_HANDLE devHandle,ULONG PinType,const BYTE *PinCode,ULONG PinLen);
//���ƣ�Z_ChangePin
//����: �޸�PIN
//IN��DeviceHandle���豸���
//IN��PinType��PIN����,����CKU_USER,CKU_SO
//IN��OldPin����PINֵ
//IN��OldPinLen����PINֵ����
//IN��NewPin����PINֵ
//IN��NewPinLen����PINֵ����
//���أ��������
DEV_RES Z_ChangePin(DEV_HANDLE devHandle,ULONG PinType,
					 const BYTE *OldPin,ULONG OldPinLen, 
					 const BYTE *NewPin,ULONG NewPinLen);

//���ƣ�Z_UnblockPin
//����: ����PIN
//IN��DeviceHandle���豸���
//IN��UnblockPin������PIN
//IN��UnblockPinLen������PINֵ����
//IN��NewPin�����û�PINֵ
//IN��NewPinLen�����û�PINֵ����
//���أ��������
DEV_RES Z_UnblockPin(DEV_HANDLE devHandle,
					  const BYTE *UnblockPin,ULONG UnblockPinLen, 
					 const BYTE *NewPin,ULONG NewPinLen);
//���ƣ�Z_GenKeyPair
//����: ����RSA��Կ��
//IN��DeviceHandle���豸���
//IN��PriKeyId��˽Կ�ļ���ʶ
//IN��PubKeyId����Կ�ļ���ʶ

//���أ��������
DEV_RES Z_GenKeyPair(DEV_HANDLE devHandle,WORD PriKeyId,WORD PubKeyId);
//���ƣ�Z_KeyEncrypt
//����: RSAǩ��
//IN��DeviceHandle���豸���
//IN��PriKeyId��˽Կ�ļ���ʶ
//IN��pDataIn����������
//IN��pDataOut���������
//���أ��������
DEV_RES Z_KeyEncrypt(DEV_HANDLE devHandle,WORD PriKeyId,const BYTE *pDataIn,ULONG DataInLen, BYTE *pDataOut,ULONG *pDataOutLen);
//���ƣ�Z_KeyDecrypt
//����: RSA����
//IN��DeviceHandle���豸���
//IN��PriKeyId��˽Կ�ļ���ʶ
//IN��pDataIn����������
//IN��pDataOut���������
//���أ��������
DEV_RES Z_KeyDecrypt(DEV_HANDLE devHandle,WORD PriKeyId,const BYTE *pDataIn,ULONG DataInLen, BYTE *pDataOut,ULONG *pDataOutLen);
//���ƣ�Z_ReadBin
//����: ����ǰѡ��Ķ������ļ�����
//IN��DeviceHandle���豸���
//IN��Offset��ƫ��
//IN��DataLen�����ݳ���
//IN��pDataOut���������
//���أ��������
DEV_RES Z_ReadBin(DEV_HANDLE devHandle,WORD FileId,ULONG Offset,ULONG DataLen,BYTE *pDataOut);
//���ƣ�Z_UpdateBin
//����: д��ǰѡ��Ķ������ļ�����
//IN��DeviceHandle���豸���
//IN��Offset��ƫ��
//IN��DataLen�����ݳ���
//IN��pDataOut���������
//���أ��������
DEV_RES Z_UpdateBin(DEV_HANDLE devHandle,WORD FileId,ULONG Offset,ULONG DataLen,const BYTE *pDataIn);

//���ƣ�Z_WritePubKey
//���ܣ�д��ǰѡ��Ĺ�Կ
//IN��DeviceHandle���豸���
//IN��KeyLen����Կ���ȣ��̶�Ϊ1024
//IN��Modulus ģ������=128
//IN��Exponent ָ��������=3
DEV_RES Z_WritePubKey(DEV_HANDLE devHandle,WORD FileId,S_KeyBase *pKey);
//���ƣ�Z_ReadPubKey
//���ܣ�����ǰѡ��Ĺ�Կ
//IN��DeviceHandle���豸���
//IN��KeyLen����Կ���ȣ��̶�Ϊ1024
//OUT��pModulus ģ������=128
//OUT��pExponent ָ��������=3
DEV_RES Z_ReadPubKey(DEV_HANDLE devHandle,WORD FileId,S_KeyBase *pKey);

//���ƣ�Z_WritePriKey
//���ܣ�д��ǰѡ���˽Կ
//IN��DeviceHandle���豸���
//IN��KeyLen����Կ���ȣ��̶�Ϊ1024
//IN��Prime1 ģ������=64��˽ԿԪ�ص�P����
//IN��Prime2 ģ������=64��˽ԿԪ�ص�Q����
//IN��Exponent1 ģ������=64��˽ԿԪ�ص�p(d mod(p-1))����
//IN��Exponent2 ģ������=64��˽ԿԪ�ص�q(d mod(q-1))����
//IN��Coefficient ģ������=64��˽ԿԪ�ص�I(inverse of q)mod p����
DEV_RES Z_WritePriKey(DEV_HANDLE devHandle,WORD FileId,S_KeyBase *pKey);

#ifdef __cplusplus
}
#endif
#endif
