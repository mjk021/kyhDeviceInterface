#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include "typedefine.h"
#ifndef __EXPORT_APP_H
#define __EXPORT_APP_H

//extern unsigned long D_Initialize(void *   pInitArgs);
//typedef unsigned long (* CK_D_Initialize)(void *   pInitArgs  );
//#include "base_head.h"
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************/
//					��������Ϣ�ӿ�
/*****************************************************/
	extern BOOL D_Initialize(void *   pInitArgs);





	extern void D_Finalize();

	extern BOOL D_GetSlotList(OUT DEV_HANDLE *pDevList,IN OUT ULONG *pCount);
	extern BOOL D_GetSlotListName(char pName[][MAX_PATH],IN OUT ULONG *pCount);
//����: ��ѯ�豸�б�,������
//OUT��pName���豸�б����ƣ�
//IN OUT��pCount������ΪOUTʱ�����豸�б����������ΪINʱ��������豸����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
//˵��������豸�Ѿ���,ֻ�ǰѾ������
//�÷�1��
//	DEV_HANDLE  *pName[2];
//	ULONG Count=2;//����޶�2
//	GetSlotListName(pName,&Count);
	extern BOOL D_OpenSlot(const char *slotName,DEV_HANDLE *pDevHandle);
//����: �������������豸������
//IN:slotName�豸����
//OUT��pDevHandle���豸���
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_CloseSlot(DEV_HANDLE DevHandle);
//����˵����		�Ͽ����������豸������
//IN��DeviceHandle���豸���
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_GetSlotInfo(DEV_HANDLE DevHandle,S_SlotInfo *pSlotInfo);
//����˵������ȡ�۵��йس��ҺͰ汾��Ϣ.
//IN��DeviceHandle���豸���
//OUT:pSlotInfo:�豸Slot��Ϣ
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern long D_GetErrMsg(DEV_HANDLE DevHandle,char * value);
//������		HS_GetErrMsg
//����˵����		�õ����Կ������Ĵ�����Ϣ
//IN��DeviceHandle���豸���
//OUT��value ������Ϣ,Ĭ�ϴ�С������256
//���أ��������

	extern BOOL D_IsOpened(DEV_HANDLE DevHandle);
//����˵����		�ж϶��������豸�Ƿ���������
//IN��DeviceHandle���豸���
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��

	extern BOOL D_GetTokenInfo(DEV_HANDLE DevHandle,S_TokenInfo *pInfo);
//����˵�������豸��CardInfo File�ж������еĸ��豸�Ĺ���Ӧ����Ϣ
//����˵�������豸��CardInfo File�ж������еĸ��豸���ļ�������Ϣ������Ϣ�ǹ̶��ģ�ֻ���ġ�
//IN��DeviceHandle���豸���
//OUT��pInfo���豸��Ӧ����Ϣ
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��



/*****************************************************/
//Pin��Token��Applition��Ϣ
/*****************************************************/
	extern BOOL D_IsLogin(DEV_HANDLE DevHandle,ULONG usertype);
//����˵������֪Pin�Ƿ��Ѿ�����Login״̬��������Ҫ�����Ի������û�����Pin
//IN��DeviceHandle���豸���
//IN:	usertype;1=�û�pin,2=����ԱPin
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_IsInitPin(DEV_HANDLE DevHandle,ULONG usertype);
//����˵������֪Pin�Ƿ��Ѿ���ʼ��������P11�������C_InitPin�ӿ�
//IN��DeviceHandle���豸���
//IN:	usertype;1=�û�pin,2=����ԱPin
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_InitPin(DEV_HANDLE DevHandle,ULONG usertype,const BYTE * pPin,ULONG  ulPinLen);
//����˵������ʼ��Token��Pin����
//IN��DeviceHandle���豸���
//IN:	usertype;1=�û�pin,2=����ԱPin
//IN:pPin���û�Pin
//IN��ulPinLen���û�Pin����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_Login(DEV_HANDLE DevHandle,ULONG usertype,const BYTE *pCodeValue,ULONG CodeLen);
//������		HS_Login
//���ܣ�		У���豸�û�Pin
//IN��DeviceHandle���豸���
//IN��usertype���û�����
//IN��pCodeValue�û�Pin
//IN��CodeLen�û�Pin����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_Logout(DEV_HANDLE DevHandle,ULONG usertype);
//����˵�����û�ע��
//IN��DeviceHandle���豸���
//IN��usertype���û�����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��

	extern BOOL D_ChangePin(DEV_HANDLE DevHandle,ULONG usertype,const BYTE *pOldPin,ULONG OldPinLen,
						   const BYTE *pNewPin,ULONG NewPinLen);
//������		D_ChangePin
//���ܣ�		�޸��豸�û�Pin
//IN��DeviceHandle���豸���
//IN��usertype���û�����
//IN��pOldPin���û�Pin
//IN��OldPinLen���û�Pin����
//IN��pNewPin���û�Pin
//IN��NewPinLen���û�Pin����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_UnblockPin(DEV_HANDLE DevHandle,const BYTE *pAdmPin,ULONG AdmPinLen,const BYTE *pUserPin,ULONG UserPinLen);
//������		D_UnblockPin
//���ܣ�		�����û�PIN
//IN��DeviceHandle���豸���
//IN��pAdmPin����ԱPIN
//IN��AdmPinLen����ԱPIN����
//IN��pUserPin���û�Pin
//IN��UserPinLen���û�Pin����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_IsChangedPin(DEV_HANDLE DevHandle,ULONG usertype);
/*****************************************************/
//�������
/*****************************************************/
/*****************************************************/
//���Ҷ�����趨������Ч�Բ���
/*****************************************************/

	extern BOOL D_FindObjects(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG right,ULONG ObjType,ULONG *pCount,ULONG ObjID[]);
//����˵�������Ҷ�����󣬷��ض�����
//IN��DeviceHandle���豸���
//IN��ContainerId����Id
//IN��ObjType���������ͣ��ֱ�Ϊ���ݶ���֤����󣬹�Կ����˽Կ����
//OUT��pObjID�����ض���ID��
//OUT��pCount���������
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_FindObject(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,ULONG nAlg,ULONG *pObjID);
//����˵��������һ�����󣬷��ض�����
//IN��DeviceHandle���豸���
//IN��ContainerId����ID
//IN��ObjType���������ͣ��ֱ�Ϊ���ݶ���֤����󣬹�Կ����˽Կ����
//OUT��ObjID�����ض���ID��
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_FindObjectEx(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,const char *name,ULONG *pObjID);
//����˵����ͨ�����Ʋ���һ�����󣬷��ض�����
//IN��DeviceHandle���豸���
//IN��ContainerId����ID
//IN��ObjType���������ͣ��ֱ�Ϊ���ݶ���֤����󣬹�Կ����˽Կ����
//IN��name��������
//OUT��ObjID�����ض���ID��
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
/*****************************************************/
//���󴴽�����
/*****************************************************/
	extern BOOL D_CreateKeyPair(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG nAlg,ULONG KeyLen,ULONG PubKeyAttrLen,ULONG PriKeyAttrLenLen,ULONG *pPriKeyID,ULONG *pPubKeyID);
//����˵����������Կ�Զ����ļ�
//IN��DeviceHandle���豸���
//IN��ContainerId����Id
//IN��nAlg��������;��ʶ,����ΪAT_KEYEXCHANGE��AT_SIGNATURE
//OUT��pPriKeyID��˽Կ����ID
//OUT��pPubKeyID����Կ����ID
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_CreateObjectEx(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,char *Label,ULONG right,ULONG len,ULONG AttrLen,ULONG *pObjID);
//����˵�������������ļ���ͨ�����ƣ�
//IN��DeviceHandle���豸���
//IN��ObjType���������ͣ��ֱ�Ϊ���ݶ���֤����󣬹�Կ����˽Կ����
//IN��Label����������
//IN��ContainerId����Id
//IN��len���󳤶�
//OUT��pObjID������ID
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_CreateObject(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,ULONG nAlg,ULONG right,ULONG len,ULONG AttrLen,ULONG *pObjID);
//����˵�������������ļ�
//IN��DeviceHandle���豸���
//IN��ObjType���������ͣ��ֱ�Ϊ���ݶ���֤����󣬹�Կ����˽Կ����
//IN��nAlg��������;��ʶ,����ΪAT_KEYEXCHANGE��AT_SIGNATURE
//IN��ContainerId����Id
//IN��len���󳤶�
//OUT��pObjID������ID
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
/*****************************************************/
//�������ٲ���
/*****************************************************/
	extern BOOL D_DestroyAllObject(DEV_HANDLE DevHandle,ULONG ContainerId);
//����˵��������˵����Կ����֤��������ݶ���
//IN��DeviceHandle���豸���
//IN��ContainerId����Id
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_DestroyObject(DEV_HANDLE DevHandle,ULONG ObjID);
//����˵�������ٶ���
//IN��DeviceHandle���豸���
//IN��pObjID������ID
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��

/****************************************************************/
//		RSA��˾Կ������
/****************************************************************/

	extern BOOL D_GenerateKeyPair(DEV_HANDLE DevHandle,ULONG PriID,ULONG PubID);
//������		HS_GenerateKeyPair
//���ܣ�		������Կ��
//IN��DeviceHandle���豸���
//IN��PriID ˽ԿID
//IN��PubID ��ԿID
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_KeySign(DEV_HANDLE DevHandle,ULONG ObjID, DWORD flag,const BYTE *pDataIn,ULONG DataInLen,BYTE *pSignResult,ULONG *pSignResultLen);
//������		D_KeyEncrypt
//���ܣ�		˽Կǩ������
//IN��DeviceHandle���豸���
//IN��PriID ˽ԿID
//IN:ǩ���㷨����=CALG_SHA1
//IN��pDataIn ��������
//IN��DataInLen �������ݳ�
//OUT��pDataOut �������
//OUT��pDataOutLen ������ݳ�
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_KeyEncrypt(DEV_HANDLE DevHandle,ULONG ObjID, const BYTE *pDataIn,ULONG DataInLen,BYTE *pSignResult,ULONG *pSignResultLen);
//������		D_KeyEncrypt
//���ܣ�		˽Կ����ǩ������
//IN��DeviceHandle���豸���
//IN��PriID ˽ԿID
//IN��pDataIn ��������
//IN��DataInLen �������ݳ�
//OUT��pDataOut �������
//OUT��pDataOutLen ������ݳ�
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_KeyDecrypt(DEV_HANDLE DevHandle,ULONG ObjID,const BYTE *pDataIn,ULONG DataInLen, BYTE *pDataOut,ULONG *pDataOutLen);
//������		D_KeyDecrypt
//���ܣ�		˽Կ���ܹ���
//IN��DeviceHandle���豸���
//IN��PriID ˽ԿID
//IN��pDataIn ��������
//IN��DataInLen �������ݳ�
//OUT��pDataOut �������
//OUT��pDataOutLen ������ݳ�
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_WritePubKey(DEV_HANDLE DevHandle,ULONG PubID,S_KeyBase *pKey);
//������		HS_WritePubKey
//���ܣ�		д��Կ
//IN��DeviceHandle���豸���
//IN��PubID ˽ԿID
//IN��Modulus ģ
//IN��ModulusLen ģ����Ӧ=128
//IN��Exponent ָ��
//IN��ExponentLen ָ������Ӧ=3
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_WritePriKey(DEV_HANDLE DevHandle,ULONG PriID,S_KeyBase *pKey);
//������		HS_WritePriKey
//���ܣ�		д˽Կ
//IN��DeviceHandle���豸���
//IN��PriID ˽ԿID
//IN��KeyLen ��Կ����Ӧ����1024
//IN��Prime1	--------------p
//IN��Prime2----------------q
//IN��Exponent1------------------p(d mod(p-1))
//IN��Exponent2------------------q(d mod(q-1))
//IN��Coefficient----------------I(inverse of q)mod p
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_ReadPubKey(DEV_HANDLE DevHandle,ULONG PubID,S_KeyBase *pKey);
//������		HS_ReadPubKey
//���ܣ�		����Կ
//IN��DeviceHandle���豸���
//IN��PubID ˽ԿID
//OUT��pModulus ģ
//OUT��pModulusLen ģ����Ӧ=128
//OUT��pExponent ָ��
//OUT��pExponentLen ָ������Ӧ=3
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��

	extern BOOL D_SetAttributeValue(DEV_HANDLE DevHandle,ULONG ObjID,BYTE Format,const BYTE *value,ULONG valueLen);
//����˵�������ö�����չ����
//IN��ObjType ��������
//IN��ObjID ����ID
//IN��pAttr ����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_GetAttributeValue(DEV_HANDLE DevHandle,ULONG ObjID,BYTE *pFormat,BYTE *value,ULONG *pValueLen);

//����˵������ö�����չ����
//IN��ObjType ��������
//IN��ObjID ����ID
//IN��pAttr ����
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_ReadCert(DEV_HANDLE DevHandle,ULONG ObjID,BYTE *pValue,ULONG *pLen);
//������		HS_ReadCert
//���ܣ�		��֤��
//IN��DeviceHandle���豸���
//IN��ObjID ����ID
//OUT��pData ֤������
//OUT��pLen  ֤�鳤��
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_WriteCert(DEV_HANDLE DevHandle,ULONG ObjID,const BYTE *pValue,ULONG DataLen);
//������		HS_WriteCert
//���ܣ�		д֤��
//IN��DeviceHandle���豸���
//IN��ObjID ����ID
//IN��pData ֤������
//IN��DataLen  ֤�鳤��
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��

	extern BOOL D_ReadData(DEV_HANDLE DevHandle,ULONG ObjID,BYTE *pFormat,BYTE *pData,ULONG *pLen);
//����˵������ָ����ŵ����ݶ���
//IN��DeviceHandle���豸���
//IN��ObjID ����ID
//IN��pData ��������
//IN��DataLen  ���ݳ���
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_WriteData(DEV_HANDLE DevHandle,ULONG ObjID,BYTE Format,const BYTE *Value,ULONG ValueLen);
//����˵����дָ����ŵ����ݶ���
//IN��DeviceHandle���豸���
//IN��ObjID ����ID
//IN��pData ��������
//IN��DataLen  ���ݳ���
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��


/*******************************************************/
//��������
/*******************************************************/

	extern BOOL D_GetContainerCount(DEV_HANDLE DevHandle,ULONG *pIds,ULONG *pCount);
//����˵�����������������������������
//IN��DeviceHandle���豸���
//OUT��pIds ����Id����
//OUT��pCount���������
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_CreateContainer(DEV_HANDLE DevHandle,WORD type,const char * Name,ULONG *pId);
//����˵������������
//IN��DeviceHandle���豸���
//IN��Name ��������
//OUT:pId,����Id
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_DeleteContainer(DEV_HANDLE DevHandle,ULONG ContainerId);
//����˵����ɾ������
//IN��DeviceHandle���豸���
//IN��ContainerId ����Id
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_GetContainerInfo(DEV_HANDLE DevHandle,ULONG Id,S_ContainerInfo * pInfo);
//����˵�������������Ϣ
//IN��DeviceHandle���豸���
//IN��Id ����Id
//OUT��pInfo ���ص�������Ϣ
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��
	extern BOOL D_ReFormat(DEV_HANDLE DevHandle,int nReTry,const BYTE* pSoPin,int SoPinLen,const BYTE* pUserPin,int UserPinLen);
	extern BOOL D_Format(DEV_HANDLE DevHandle,S_Format *pFormat);
//BOOL D_Format(IN ULONG slotID,IN BYTE nReTry,IN LPSTR pSoPin,IN LPSTR pUserPin,LPSTR pLabel,LPSTR pUserName,char *AppNumber,BYTE *pSignatrue);

//����˵������ʽ���豸
//IN��DeviceHandle���豸���
//IN��nReTry��PIN���������������PIN
//IN��pSuPwd����ԱPIN
//IN��pUserPwd�û�PIN
//IN��pLabel�豸��ʶ
//���أ��ɹ�ΪTRUE��ʧ��ΪFALSE��

#ifdef __cplusplus
}
#endif
#endif
