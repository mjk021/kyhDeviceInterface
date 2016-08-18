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
//					连接与信息接口
/*****************************************************/
	extern BOOL D_Initialize(void *   pInitArgs);





	extern void D_Finalize();

	extern BOOL D_GetSlotList(OUT DEV_HANDLE *pDevList,IN OUT ULONG *pCount);
	extern BOOL D_GetSlotListName(char pName[][MAX_PATH],IN OUT ULONG *pCount);
//功能: 查询设备列表,但不打开
//OUT：pName：设备列表名称；
//IN OUT：pCount：当作为OUT时返回设备列表个数；当作为IN时输入最大设备数量
//返回：成功为TRUE，失败为FALSE；
//说明：如果设备已经打开,只是把句柄返回
//用法1：
//	DEV_HANDLE  *pName[2];
//	ULONG Count=2;//最大限定2
//	GetSlotListName(pName,&Count);
	extern BOOL D_OpenSlot(const char *slotName,DEV_HANDLE *pDevHandle);
//功能: 建立读卡器与设备的连接
//IN:slotName设备名称
//OUT：pDevHandle：设备句柄
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_CloseSlot(DEV_HANDLE DevHandle);
//功能说明：		断开读卡器与设备的连接
//IN：DeviceHandle：设备句柄
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_GetSlotInfo(DEV_HANDLE DevHandle,S_SlotInfo *pSlotInfo);
//功能说明：获取槽的有关厂家和版本信息.
//IN：DeviceHandle：设备句柄
//OUT:pSlotInfo:设备Slot信息
//返回：成功为TRUE，失败为FALSE；
	extern long D_GetErrMsg(DEV_HANDLE DevHandle,char * value);
//函数名		HS_GetErrMsg
//功能说明：		得到最后对卡操作的错误信息
//IN：DeviceHandle：设备句柄
//OUT：value 错误信息,默认大小不大于256
//返回：错误代码

	extern BOOL D_IsOpened(DEV_HANDLE DevHandle);
//功能说明：		判断读卡器与设备是否连接正常
//IN：DeviceHandle：设备句柄
//返回：成功为TRUE，失败为FALSE；

	extern BOOL D_GetTokenInfo(DEV_HANDLE DevHandle,S_TokenInfo *pInfo);
//功能说明：从设备的CardInfo File中读出所有的该设备的公共应用信息
//功能说明：从设备的CardInfo File中读出所有的该设备的文件对象信息，该信息是固定的，只读的。
//IN：DeviceHandle：设备句柄
//OUT：pInfo：设备的应用信息
//返回：成功为TRUE，失败为FALSE；



/*****************************************************/
//Pin，Token，Applition信息
/*****************************************************/
	extern BOOL D_IsLogin(DEV_HANDLE DevHandle,ULONG usertype);
//功能说明：告知Pin是否已经处于Login状态，否则需要弹出对话框让用户输入Pin
//IN：DeviceHandle：设备句柄
//IN:	usertype;1=用户pin,2=管理员Pin
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_IsInitPin(DEV_HANDLE DevHandle,ULONG usertype);
//功能说明：告知Pin是否已经初始化，否则，P11将会调用C_InitPin接口
//IN：DeviceHandle：设备句柄
//IN:	usertype;1=用户pin,2=管理员Pin
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_InitPin(DEV_HANDLE DevHandle,ULONG usertype,const BYTE * pPin,ULONG  ulPinLen);
//功能说明：初始化Token的Pin参数
//IN：DeviceHandle：设备句柄
//IN:	usertype;1=用户pin,2=管理员Pin
//IN:pPin：用户Pin
//IN：ulPinLen：用户Pin长度
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_Login(DEV_HANDLE DevHandle,ULONG usertype,const BYTE *pCodeValue,ULONG CodeLen);
//函数名		HS_Login
//功能：		校验设备用户Pin
//IN：DeviceHandle：设备句柄
//IN：usertype：用户类型
//IN：pCodeValue用户Pin
//IN：CodeLen用户Pin长度
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_Logout(DEV_HANDLE DevHandle,ULONG usertype);
//功能说明：用户注销
//IN：DeviceHandle：设备句柄
//IN：usertype：用户类型
//返回：成功为TRUE，失败为FALSE；

	extern BOOL D_ChangePin(DEV_HANDLE DevHandle,ULONG usertype,const BYTE *pOldPin,ULONG OldPinLen,
						   const BYTE *pNewPin,ULONG NewPinLen);
//函数名		D_ChangePin
//功能：		修改设备用户Pin
//IN：DeviceHandle：设备句柄
//IN：usertype：用户类型
//IN：pOldPin旧用户Pin
//IN：OldPinLen旧用户Pin长度
//IN：pNewPin新用户Pin
//IN：NewPinLen新用户Pin长度
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_UnblockPin(DEV_HANDLE DevHandle,const BYTE *pAdmPin,ULONG AdmPinLen,const BYTE *pUserPin,ULONG UserPinLen);
//函数名		D_UnblockPin
//功能：		解锁用户PIN
//IN：DeviceHandle：设备句柄
//IN：pAdmPin管理员PIN
//IN：AdmPinLen管理员PIN长度
//IN：pUserPin新用户Pin
//IN：UserPinLen新用户Pin长度
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_IsChangedPin(DEV_HANDLE DevHandle,ULONG usertype);
/*****************************************************/
//对象操作
/*****************************************************/
/*****************************************************/
//查找对象和设定对象有效性操作
/*****************************************************/

	extern BOOL D_FindObjects(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG right,ULONG ObjType,ULONG *pCount,ULONG ObjID[]);
//功能说明：查找多个对象，返回对象句柄
//IN：DeviceHandle：设备句柄
//IN：ContainerId容器Id
//IN：ObjType：对象类型，分别为数据对象，证书对象，公钥对象，私钥对象
//OUT：pObjID：返回对象ID。
//OUT：pCount：对象个数
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_FindObject(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,ULONG nAlg,ULONG *pObjID);
//功能说明：查找一个对象，返回对象句柄
//IN：DeviceHandle：设备句柄
//IN：ContainerId容器ID
//IN：ObjType：对象类型，分别为数据对象，证书对象，公钥对象，私钥对象
//OUT：ObjID：返回对象ID。
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_FindObjectEx(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,const char *name,ULONG *pObjID);
//功能说明：通过名称查找一个对象，返回对象句柄
//IN：DeviceHandle：设备句柄
//IN：ContainerId容器ID
//IN：ObjType：对象类型，分别为数据对象，证书对象，公钥对象，私钥对象
//IN：name对象名称
//OUT：ObjID：返回对象ID。
//返回：成功为TRUE，失败为FALSE；
/*****************************************************/
//对象创建操作
/*****************************************************/
	extern BOOL D_CreateKeyPair(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG nAlg,ULONG KeyLen,ULONG PubKeyAttrLen,ULONG PriKeyAttrLenLen,ULONG *pPriKeyID,ULONG *pPubKeyID);
//功能说明：创建密钥对对象文件
//IN：DeviceHandle：设备句柄
//IN：ContainerId容器Id
//IN：nAlg：对象用途标识,可以为AT_KEYEXCHANGE、AT_SIGNATURE
//OUT：pPriKeyID：私钥对象ID
//OUT：pPubKeyID：公钥对象ID
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_CreateObjectEx(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,char *Label,ULONG right,ULONG len,ULONG AttrLen,ULONG *pObjID);
//功能说明：创建对象文件（通过名称）
//IN：DeviceHandle：设备句柄
//IN：ObjType：对象类型，分别为数据对象，证书对象，公钥对象，私钥对象
//IN：Label：对象名称
//IN：ContainerId容器Id
//IN：len对象长度
//OUT：pObjID：对象ID
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_CreateObject(DEV_HANDLE DevHandle,ULONG ContainerId,ULONG ObjType,ULONG nAlg,ULONG right,ULONG len,ULONG AttrLen,ULONG *pObjID);
//功能说明：创建对象文件
//IN：DeviceHandle：设备句柄
//IN：ObjType：对象类型，分别为数据对象，证书对象，公钥对象，私钥对象
//IN：nAlg：对象用途标识,可以为AT_KEYEXCHANGE、AT_SIGNATURE
//IN：ContainerId容器Id
//IN：len对象长度
//OUT：pObjID：对象ID
//返回：成功为TRUE，失败为FALSE；
/*****************************************************/
//对象销毁操作
/*****************************************************/
	extern BOOL D_DestroyAllObject(DEV_HANDLE DevHandle,ULONG ContainerId);
//功能说明：销毁说有密钥对象，证书对象，数据对象。
//IN：DeviceHandle：设备句柄
//IN：ContainerId容器Id
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_DestroyObject(DEV_HANDLE DevHandle,ULONG ObjID);
//功能说明：销毁对象
//IN：DeviceHandle：设备句柄
//IN：pObjID：对象ID
//返回：成功为TRUE，失败为FALSE；

/****************************************************************/
//		RSA公司钥操作类
/****************************************************************/

	extern BOOL D_GenerateKeyPair(DEV_HANDLE DevHandle,ULONG PriID,ULONG PubID);
//函数名		HS_GenerateKeyPair
//功能：		产生密钥对
//IN：DeviceHandle：设备句柄
//IN：PriID 私钥ID
//IN：PubID 公钥ID
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_KeySign(DEV_HANDLE DevHandle,ULONG ObjID, DWORD flag,const BYTE *pDataIn,ULONG DataInLen,BYTE *pSignResult,ULONG *pSignResultLen);
//函数名		D_KeyEncrypt
//功能：		私钥签名功能
//IN：DeviceHandle：设备句柄
//IN：PriID 私钥ID
//IN:签名算法例如=CALG_SHA1
//IN：pDataIn 输入数据
//IN：DataInLen 输入数据长
//OUT：pDataOut 输出数据
//OUT：pDataOutLen 输出数据长
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_KeyEncrypt(DEV_HANDLE DevHandle,ULONG ObjID, const BYTE *pDataIn,ULONG DataInLen,BYTE *pSignResult,ULONG *pSignResultLen);
//函数名		D_KeyEncrypt
//功能：		私钥加密签名功能
//IN：DeviceHandle：设备句柄
//IN：PriID 私钥ID
//IN：pDataIn 输入数据
//IN：DataInLen 输入数据长
//OUT：pDataOut 输出数据
//OUT：pDataOutLen 输出数据长
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_KeyDecrypt(DEV_HANDLE DevHandle,ULONG ObjID,const BYTE *pDataIn,ULONG DataInLen, BYTE *pDataOut,ULONG *pDataOutLen);
//函数名		D_KeyDecrypt
//功能：		私钥解密功能
//IN：DeviceHandle：设备句柄
//IN：PriID 私钥ID
//IN：pDataIn 输入数据
//IN：DataInLen 输入数据长
//OUT：pDataOut 输出数据
//OUT：pDataOutLen 输出数据长
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_WritePubKey(DEV_HANDLE DevHandle,ULONG PubID,S_KeyBase *pKey);
//函数名		HS_WritePubKey
//功能：		写公钥
//IN：DeviceHandle：设备句柄
//IN：PubID 私钥ID
//IN：Modulus 模
//IN：ModulusLen 模长，应=128
//IN：Exponent 指数
//IN：ExponentLen 指数长，应=3
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_WritePriKey(DEV_HANDLE DevHandle,ULONG PriID,S_KeyBase *pKey);
//函数名		HS_WritePriKey
//功能：		写私钥
//IN：DeviceHandle：设备句柄
//IN：PriID 私钥ID
//IN：KeyLen 密钥长，应等于1024
//IN：Prime1	--------------p
//IN：Prime2----------------q
//IN：Exponent1------------------p(d mod(p-1))
//IN：Exponent2------------------q(d mod(q-1))
//IN：Coefficient----------------I(inverse of q)mod p
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_ReadPubKey(DEV_HANDLE DevHandle,ULONG PubID,S_KeyBase *pKey);
//函数名		HS_ReadPubKey
//功能：		读公钥
//IN：DeviceHandle：设备句柄
//IN：PubID 私钥ID
//OUT：pModulus 模
//OUT：pModulusLen 模长，应=128
//OUT：pExponent 指数
//OUT：pExponentLen 指数长，应=3
//返回：成功为TRUE，失败为FALSE；

	extern BOOL D_SetAttributeValue(DEV_HANDLE DevHandle,ULONG ObjID,BYTE Format,const BYTE *value,ULONG valueLen);
//功能说明：设置对象扩展属性
//IN：ObjType 对象类型
//IN：ObjID 对象ID
//IN：pAttr 属性
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_GetAttributeValue(DEV_HANDLE DevHandle,ULONG ObjID,BYTE *pFormat,BYTE *value,ULONG *pValueLen);

//功能说明：获得对象扩展属性
//IN：ObjType 对象类型
//IN：ObjID 对象ID
//IN：pAttr 属性
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_ReadCert(DEV_HANDLE DevHandle,ULONG ObjID,BYTE *pValue,ULONG *pLen);
//函数名		HS_ReadCert
//功能：		读证书
//IN：DeviceHandle：设备句柄
//IN：ObjID 对象ID
//OUT：pData 证书内容
//OUT：pLen  证书长度
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_WriteCert(DEV_HANDLE DevHandle,ULONG ObjID,const BYTE *pValue,ULONG DataLen);
//函数名		HS_WriteCert
//功能：		写证书
//IN：DeviceHandle：设备句柄
//IN：ObjID 对象ID
//IN：pData 证书内容
//IN：DataLen  证书长度
//返回：成功为TRUE，失败为FALSE；

	extern BOOL D_ReadData(DEV_HANDLE DevHandle,ULONG ObjID,BYTE *pFormat,BYTE *pData,ULONG *pLen);
//功能说明：读指定序号的数据对象
//IN：DeviceHandle：设备句柄
//IN：ObjID 对象ID
//IN：pData 数据内容
//IN：DataLen  数据长度
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_WriteData(DEV_HANDLE DevHandle,ULONG ObjID,BYTE Format,const BYTE *Value,ULONG ValueLen);
//功能说明：写指定序号的数据对象
//IN：DeviceHandle：设备句柄
//IN：ObjID 对象ID
//IN：pData 数据内容
//IN：DataLen  数据长度
//返回：成功为TRUE，失败为FALSE；


/*******************************************************/
//容器操作
/*******************************************************/

	extern BOOL D_GetContainerCount(DEV_HANDLE DevHandle,ULONG *pIds,ULONG *pCount);
//功能说明：获得容器个数，返回容器名称
//IN：DeviceHandle：设备句柄
//OUT：pIds 容器Id数组
//OUT：pCount：对象个数
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_CreateContainer(DEV_HANDLE DevHandle,WORD type,const char * Name,ULONG *pId);
//功能说明：创建容器
//IN：DeviceHandle：设备句柄
//IN：Name 容器名称
//OUT:pId,容器Id
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_DeleteContainer(DEV_HANDLE DevHandle,ULONG ContainerId);
//功能说明：删除容器
//IN：DeviceHandle：设备句柄
//IN：ContainerId 容器Id
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_GetContainerInfo(DEV_HANDLE DevHandle,ULONG Id,S_ContainerInfo * pInfo);
//功能说明：获得容器信息
//IN：DeviceHandle：设备句柄
//IN：Id 容器Id
//OUT：pInfo 返回的容器信息
//返回：成功为TRUE，失败为FALSE；
	extern BOOL D_ReFormat(DEV_HANDLE DevHandle,int nReTry,const BYTE* pSoPin,int SoPinLen,const BYTE* pUserPin,int UserPinLen);
	extern BOOL D_Format(DEV_HANDLE DevHandle,S_Format *pFormat);
//BOOL D_Format(IN ULONG slotID,IN BYTE nReTry,IN LPSTR pSoPin,IN LPSTR pUserPin,LPSTR pLabel,LPSTR pUserName,char *AppNumber,BYTE *pSignatrue);

//功能说明：格式化设备
//IN：DeviceHandle：设备句柄
//IN：nReTry：PIN最大错误次数，锁定PIN
//IN：pSuPwd管理员PIN
//IN：pUserPwd用户PIN
//IN：pLabel设备标识
//返回：成功为TRUE，失败为FALSE；

#ifdef __cplusplus
}
#endif
#endif
