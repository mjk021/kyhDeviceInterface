#ifndef __COS_INTERFACE__
#define __COS_INTERFACE__
#include "_typedefine.h"
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



#ifdef __cplusplus
}
#endif
#endif
