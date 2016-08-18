#ifndef __COS_INTERFACE__
#define __COS_INTERFACE__
#include "typedefine.h"
//PIN类型定义
#define CKU_SO    0
#define CKU_USER  1
//COS文件创建权限定义

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
//COS文件类型定义
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

//加载对称临时密钥
DEV_RES Z_LoadSymmetricKey(
    DEV_HANDLE devHandle, 
    BYTE alg, 
    BYTE *key,
    const BYTE *pAdminPin, 
    ULONG AdminPinLen
);
//对称算法计算
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
//创建文件系统
DEV_RES Z_CreateFileSys(DEV_HANDLE devHandle, const BYTE *pAdminPin, ULONG AdminPinLen);

//Z_SM4Calc
DEV_RES Z_SM4Calc(DEV_HANDLE devHandle,BOOL isEnc,BYTE* key,BYTE*pDataIn,ULONG ulDataInLen,BYTE*pDataOut,ULONG* pulDataOutLen);

//名称：Z_SelectMF
//功能: 选择主文件
//IN：DeviceHandle：设备句柄
//返回：错误代码
DEV_RES Z_SelectMF(DEV_HANDLE devHandle);
//名称：Z_SelectEF
//功能: 选择EF文件
//IN：DeviceHandle：设备句柄
//IN：FileID：文件标识
//返回：错误代码
DEV_RES Z_SelectEF(DEV_HANDLE devHandle,WORD FileID);
//名称：Z_SelectDF
//功能: 选择目录文件
//IN：DeviceHandle：设备句柄
//IN：FileID：文件标识
//返回：错误代码
DEV_RES Z_DeleteDF(DEV_HANDLE devHandle,WORD FileID);
//名称：Z_GetErrMsg
//功能: 获得错误代码对应的信息
//IN：code：错误代码
//OUT：msg：返回的信息,
//注：msg默认不超过256个字符
//返回：错误代码
void Z_GetErrMsg(WORD code,char *msg);
//名称：Z_CreateFile
//功能: 创建文件
//IN：DeviceHandle：设备句柄
//IN：FileType：文件类型
//IN：FileID：文件标识
//IN：Size：文件长度
//IN：ReadRight：文件读权限
//IN：WriteRight：文件写权限
//IN：UseRight：文件使用权限
//返回：错误代码
DEV_RES Z_CreateEF(DEV_HANDLE devHandle,ULONG FileType,WORD FileID,ULONG Size,ULONG Right);
//名称：Z_VerifyPin
//功能: 验证PIN
//IN：DeviceHandle：设备句柄
//IN：PinType：PIN类型,包括CKU_USER,CKU_SO
//IN：PinCode：PIN值
//IN：PinLen：PIN值长度
//返回：错误代码
DEV_RES Z_VerifyPin(DEV_HANDLE devHandle,ULONG PinType,const BYTE *PinCode,ULONG PinLen);
//名称：Z_ChangePin
//功能: 修改PIN
//IN：DeviceHandle：设备句柄
//IN：PinType：PIN类型,包括CKU_USER,CKU_SO
//IN：OldPin：旧PIN值
//IN：OldPinLen：旧PIN值长度
//IN：NewPin：新PIN值
//IN：NewPinLen：新PIN值长度
//返回：错误代码
DEV_RES Z_ChangePin(DEV_HANDLE devHandle,ULONG PinType,
					 const BYTE *OldPin,ULONG OldPinLen, 
					 const BYTE *NewPin,ULONG NewPinLen);

//名称：Z_UnblockPin
//功能: 解锁PIN
//IN：DeviceHandle：设备句柄
//IN：UnblockPin：解锁PIN
//IN：UnblockPinLen：解锁PIN值长度
//IN：NewPin：新用户PIN值
//IN：NewPinLen：新用户PIN值长度
//返回：错误代码
DEV_RES Z_UnblockPin(DEV_HANDLE devHandle,
					  const BYTE *UnblockPin,ULONG UnblockPinLen, 
					 const BYTE *NewPin,ULONG NewPinLen);
//名称：Z_GenKeyPair
//功能: 创建RSA密钥对
//IN：DeviceHandle：设备句柄
//IN：PriKeyId：私钥文件标识
//IN：PubKeyId：公钥文件标识

//返回：错误代码
DEV_RES Z_GenKeyPair(DEV_HANDLE devHandle,WORD PriKeyId,WORD PubKeyId);
//名称：Z_KeyEncrypt
//功能: RSA签名
//IN：DeviceHandle：设备句柄
//IN：PriKeyId：私钥文件标识
//IN：pDataIn：输入数据
//IN：pDataOut：输出数据
//返回：错误代码
DEV_RES Z_KeyEncrypt(DEV_HANDLE devHandle,WORD PriKeyId,const BYTE *pDataIn,ULONG DataInLen, BYTE *pDataOut,ULONG *pDataOutLen);
//名称：Z_KeyDecrypt
//功能: RSA解密
//IN：DeviceHandle：设备句柄
//IN：PriKeyId：私钥文件标识
//IN：pDataIn：输入数据
//IN：pDataOut：输出数据
//返回：错误代码
DEV_RES Z_KeyDecrypt(DEV_HANDLE devHandle,WORD PriKeyId,const BYTE *pDataIn,ULONG DataInLen, BYTE *pDataOut,ULONG *pDataOutLen);
//名称：Z_ReadBin
//功能: 读当前选择的二进制文件数据
//IN：DeviceHandle：设备句柄
//IN：Offset：偏移
//IN：DataLen：数据长度
//IN：pDataOut：输出数据
//返回：错误代码
DEV_RES Z_ReadBin(DEV_HANDLE devHandle,WORD FileId,ULONG Offset,ULONG DataLen,BYTE *pDataOut);
//名称：Z_UpdateBin
//功能: 写当前选择的二进制文件数据
//IN：DeviceHandle：设备句柄
//IN：Offset：偏移
//IN：DataLen：数据长度
//IN：pDataOut：输出数据
//返回：错误代码
DEV_RES Z_UpdateBin(DEV_HANDLE devHandle,WORD FileId,ULONG Offset,ULONG DataLen,const BYTE *pDataIn);

//名称：Z_WritePubKey
//功能：写当前选择的公钥
//IN：DeviceHandle：设备句柄
//IN：KeyLen：公钥长度，固定为1024
//IN：Modulus 模，长度=128
//IN：Exponent 指数，长度=3
DEV_RES Z_WritePubKey(DEV_HANDLE devHandle,WORD FileId,S_KeyBase *pKey);
//名称：Z_ReadPubKey
//功能：读当前选择的公钥
//IN：DeviceHandle：设备句柄
//IN：KeyLen：公钥长度，固定为1024
//OUT：pModulus 模，长度=128
//OUT：pExponent 指数，长度=3
DEV_RES Z_ReadPubKey(DEV_HANDLE devHandle,WORD FileId,S_KeyBase *pKey);

//名称：Z_WritePriKey
//功能：写当前选择的私钥
//IN：DeviceHandle：设备句柄
//IN：KeyLen：公钥长度，固定为1024
//IN：Prime1 模，长度=64，私钥元素的P参数
//IN：Prime2 模，长度=64，私钥元素的Q参数
//IN：Exponent1 模，长度=64，私钥元素的p(d mod(p-1))参数
//IN：Exponent2 模，长度=64，私钥元素的q(d mod(q-1))参数
//IN：Coefficient 模，长度=64，私钥元素的I(inverse of q)mod p参数
DEV_RES Z_WritePriKey(DEV_HANDLE devHandle,WORD FileId,S_KeyBase *pKey);

#ifdef __cplusplus
}
#endif
#endif
