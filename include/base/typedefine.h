#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>

#include <cassert>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <typeinfo>
using namespace std;

#include "UserDefine.h"
#ifndef __TYPE_DEFINE__H
#define __TYPE_DEFINE__H
/*******************************************/
//		�����ļ�����
/*******************************************/
#if defined(WIN32)
	#include "windows.h"
#else
	#include "dlfcn.h"
	#include <errno.h>
	#include <unistd.h>
#endif
/*******************************************/
//		�����������Ͷ���
/*******************************************/
#if defined(WIN32)
	typedef SCARDHANDLE CARD_HANDLE;
	typedef SCARDHANDLE DEV_HANDLE;

#else
	//windows define
	typedef int BOOL;
	typedef unsigned long ULONG;
	typedef long LONG_PTR;
	typedef unsigned int UINT_PTR;
	typedef UINT_PTR            WPARAM;
	typedef LONG_PTR            LPARAM;
	typedef LONG_PTR            LRESULT;
	typedef unsigned long CK_SLOT_ID;
	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	typedef void VOID;
	typedef unsigned int        UINT;
	typedef unsigned char       BYTE;
	typedef void* DEV_HANDLE;
	typedef void* HWND;

	//pkcs11 define
	typedef unsigned long CK_RV;
	typedef unsigned long CK_ULONG;
	typedef unsigned char     CK_BYTE;
	typedef CK_BYTE           CK_CHAR;
	typedef CK_BYTE           CK_BBOOL;
	typedef CK_BYTE     *   CK_BYTE_PTR;
	typedef CK_ULONG    *   CK_ULONG_PTR;
	typedef CK_ULONG          CK_OBJECT_HANDLE;
	typedef CK_ULONG          CK_SLOT_ID;
	typedef CK_SLOT_ID * CK_SLOT_ID_PTR;
	typedef CK_ULONG          CK_FLAGS;
	typedef void        *   CK_VOID_PTR;
	typedef void *LPVOID;
#endif
/*******************************************/
//		�궨��
/*******************************************/
#if defined(WIN32)

#else
	//windows define
	#define IN
	#define OUT

	#define MAX_PATH	1500
	#define TRUE	1
	#define FALSE	0
	//pkcs11 define
	#define CK_ENTRY

	#define CKF_DONT_BLOCK     1

	#define CKR_OK                                0x00000000
	#define CKR_HOST_MEMORY                       0x00000002
	#define CKR_GENERAL_ERROR                     0x00000005
	#define CKR_ARGUMENTS_BAD                     0x00000007
	#define CKR_CANT_LOCK                         0x0000000A
	#define CKR_DEVICE_ERROR                      0x00000030
	#define CKR_TOKEN_NOT_RECOGNIZED              0x000000E1
	#define CKR_CRYPTOKI_NOT_INITIALIZED          0x00000190
	#define CKR_NO_EVENT                          0x00000008
	#define CKR_BUFFER_TOO_SMALL                  0x00000150
#endif



#define FLAG_CONTAINER_VALID_INDEX	31

#define FLAG_USER_PIN_CHANGED_INDEX		30
#define FLAG_ADMIN_PIN_CHANGED_INDEX		29
#define TOKEN_INFO_FILE_LEN	150
#define FILE_CONTAINER_FILE_LEN		65

#define PORT_COM1	1
#define PORT_COM2	2
#define PORT_USB1	3
#define PORT_USB2	4

#define CKO_DATA            0x00000000
#define CKO_CERTIFICATE     0x00000001
#define CKO_PUBLIC_KEY      0x00000002
#define CKO_PRIVATE_KEY     0x00000003
#define CKO_SECRET_KEY      0x00000004
#define CKO_INNER_DATA		0x000000FF


#define CKF_DATA            0x00000000
#define CKF_CERTIFICATE     0x00000001
#define CKF_PUBLIC_KEY      0x00000002
#define CKF_PRIVATE_KEY     0x00000003
#define CKF_SECRET_KEY      0x00000004

#define APP_USER 0x01
#define APP_ADMIN  0x02

#define AT_KEYEXCHANGE          1
#define AT_SIGNATURE            2

//key type
#define KEY_512		1
#define KEY_1024	2
//
#define NEW_KEY_COS	1

#define CKU_SO    0
#define CKU_USER  1
//���ܿ��������ݶ���


typedef unsigned short DEV_RES;

#define MAX_READER_MODULE_COUNT	5
#define MAX_COS_MODULE_COUNT	5

#define MAX_SLOT_COUNT	32
#define MAX_TOKEN_COUNT		256
#define MIN_NAME_LEN	32
#define MAX_OBJ_MAPPING_COUNT	100


#define MAX_NAME_LEN		MAX_PATH



#define MAX_USER_PIN_LEN				16
#define MAX_SERIAL_NUMBER		16
////////////////////////////////////////////////
//COS������Ͷ���

#define COS_ZC100	1
#define COS_Z2000	2
#define COS_JAVA	3
#define COS_FEI_TIAN_2		4
#define COS_KING_COS_1		5
//�ڲ����ݶ����㷨ID����
#define AT_CONTAINER_INFO_DATA	1
#define AT_FLAG_INFO_DATA		2
#define AT_FILE_INDEX_INFO_DATA	3

#define AT_TOKEN_INFO_DATA		4
#define AT_SIGNATRUE_INFO_DATA	5

//�����������ļ������ʽ����

#define FORMAT_TLV1		1
#define FORMAT_TLV2		2
#define FORMAT_TLV4		4




typedef struct {
	unsigned char UintLen;
	unsigned char UintCount;
}S_INFO_HEAD;

typedef struct {
	double m_Version;
	UINT m_Type;
	char m_Name[MAX_NAME_LEN];
	ULONG m_DFFileID;
	BOOL m_IsEmpty;
	BOOL m_IsDeleted;
}S_ContainerInfo;

#define CON_TYPE_CSP	1
#define CON_TYPE_P11	2
#define CON_TYPE_MAIN	4
#define CON_TYPE_KOAL	8




typedef struct {
	BYTE m_Valid;
	BYTE m_Type;
	WORD m_DFFileID;
	WORD m_Size;
	char m_Name[MAX_NAME_LEN];
}S_HD_CONTAINER_V3;

#define TKN_APP_TYPE	1
#define TKN_VERSION		2
#define TKN_RSA_KEY_TYPE	3
#define TKN_MAX_ERR_TIMES	4
#define TKN_DEVICE_SERIAL_NUMBER	5
#define TKN_APP_SERIAL_NUMBER	6
#define TKN_CREATE_TIME			7
#define TKN_USER_NAME			8
#define TKN_URL					9
#define TKN_LABEL				10
#define TKN_EMAIL				11
#define TKN_MAF_NAME			12
#define TKN_ATR					13


//������ϢTag
#define TKN_CONTAINER_TYPE		20
#define TKN_CONTAINER_NAME		21
//�������ļ���Ϣ
#define TKN_CREATE_FILE			22
typedef struct {
	char m_SoPin[MAX_USER_PIN_LEN+1];
	char m_UserPin[MAX_USER_PIN_LEN+1];
	unsigned char m_AppType;//ͬһ��Ӧ����Ŀ����ͬ�Ŀ�Ƭ���ͣ��������Ա��Ƭ���û���Ƭ
	unsigned char m_KeyCodeErrTime;//�û�Pin���������
	char m_AppNumber[MAX_SERIAL_NUMBER+1];
	char m_UserName[MIN_NAME_LEN];//��Ƭ�û�����
	char m_Label[MIN_NAME_LEN];
	char m_Url[MIN_NAME_LEN];//
	char m_Email[MIN_NAME_LEN];
	char m_PriKeyFile[MAX_NAME_LEN];
}S_Format;
typedef struct {
	unsigned char m_KeyCodeLevel;//0��ʾ�û�Pin��MF�£�1��ʾ�û�Pin��DF��
	unsigned char m_CardType;//ʶ��ͬ�ĳ��̿�Ƭ��Key
	unsigned char m_AppType;//ͬһ��Ӧ����Ŀ����ͬ�Ŀ�Ƭ���ͣ��������Ա��Ƭ���û���Ƭ
	unsigned char m_Version;//��Ƭ�ṹ�汾
	unsigned char m_KeyType;//�ǶԳ�RSA��Կ����0=512λ��Կ��1=1024λ��Կ��2=2048��Կ
	unsigned char m_KeyCodeErrTime;//�û�Pin���������
	unsigned char m_Reserve[2];

	unsigned char m_SerialNumber[MAX_SERIAL_NUMBER];//�ÿ�ƬKey�������к�
	char m_AppNumber[MAX_SERIAL_NUMBER+1];
	unsigned short m_Year;//����ʱ��
	unsigned char m_Month; //����ʱ��
	unsigned char m_Day; //����ʱ��
	unsigned char m_Hour; //����ʱ��
	unsigned char m_Minute; //����ʱ��
	char m_UserName[MIN_NAME_LEN];//��Ƭ�û�����
	char m_Label[MIN_NAME_LEN];
	char m_MafID[MIN_NAME_LEN];
	char m_Url[MIN_NAME_LEN];//
	char m_Email[MIN_NAME_LEN];
}S_TokenInfo;

typedef struct {
	unsigned long m_Port;//�˿����
	unsigned long m_PortType;//�˿�����
	unsigned long m_devType;
	char m_guid[65];
	char m_Path[MAX_NAME_LEN];
	char m_ReaderName[MAX_NAME_LEN];//����������
	char m_ReaderMafID[MIN_NAME_LEN];//����������
	char m_ReaderVer[MIN_NAME_LEN];//�������汾
	char m_CardName[MIN_NAME_LEN];//��Ƭ����
	char m_CardMafID[MIN_NAME_LEN];//��Ƭ����
	char m_Label[MIN_NAME_LEN];//�豸��ʶ����
	BYTE m_Serial[16];
}S_SlotInfo;

typedef struct {
	ULONG m_ContainerId;
	ULONG m_ContainerType;
	long m_ValidFlagIndex;
	unsigned long m_ObjClass;//�������������ݶ���֤����󣬹�Կ����˽Կ����Pin��
	unsigned long m_nAlg;

	ULONG m_Right;//���ļ��Ƿ�Pin����
	BOOL m_bExtData;//�ļ���Ŷ���λ��
	unsigned long m_FileId;//���ļ�ID
	unsigned long m_Offset;
	unsigned long m_FileLen;//�ļ�Ϊ����׼���˶��ٿռ�
	BOOL m_bRegisted;
	char m_Label[MIN_NAME_LEN];
}S_ObjFileStore;
/*
#define MAX_EXT_DATA_COUNT		25

struct BITSTRUCT
{
int m_Class:8;//�������������ݶ���֤����󣬹�Կ����˽Կ����Pin��
int m_nAlg:8;
int m_ValidFlagIndex:8;
int m_bExtData:1;
int m_Reserve:6;
int m_Private:1;
};

typedef struct {
WORD m_FileId;//���ļ�ID
WORD m_FileLen;//�ļ�Ϊ����׼���˶��ٿռ�
BITSTRUCT	 m_Bit;
char m_Label[MIN_NAME_LEN];
}S_CreateFileIndex;
*/
typedef struct {
	ULONG m_Version;
	vector <S_ObjFileStore *> m_storeList;
//	S_ObjFileStore *m_pObjFileConfigs;
//	int m_ObjFileConfigCount;
}S_FileStruct;

#define READ_PIN_PROTECT	1
#define WRITE_PIN_PROTECT	2
#define USE_PIN_PROTECT		4

#define READ_FORBIDDEN		8
#define WRITE_FORBIDDEN		16


#define OBJ_DATA_TYPE_MAIN	0
#define OBJ_DATA_TYPE_ATTR	1



#define PRI_KEY_FILE_LEN	0x24E
#define PUB_KEY_FILE_LEN	0x10E
#define CERT_FILE_LEN		0x6A4

#define MAX_CONTAINER_COUNT	64

typedef struct  
{
	ULONG keyType;
	ULONG keyLen;
}S_KeyBase;
typedef struct:public S_KeyBase  
{
	BYTE Modulus[0x80];
	BYTE Exponent[0x03];
}S_RsaPubKey1024;
typedef struct:public S_KeyBase
{
	BYTE Prime1[0x40];
	BYTE Prime2[0x40];
	BYTE Exponent1[0x40];
	BYTE Exponent2[0x40];
	BYTE Coefficient[0x40];
	BYTE PriExponent[0x80];
	BYTE PubModulus[0x80];
	BYTE PubExponent[0x03];
}S_RsaPriKey1024;

typedef struct {
	char m_Reader[MAX_PATH];//������ͬ
	BOOL m_IsLogin;//������ͬ
	int m_OpenedCount;//������
}S_SHARE_SLOT_INFO;

typedef struct {
	S_SHARE_SLOT_INFO m_SlotInfo[MAX_SLOT_COUNT];
#if defined(WIN32)
		HWND m_hNotifyWnd;
#endif
}S_SHARE_INFO;

#endif



