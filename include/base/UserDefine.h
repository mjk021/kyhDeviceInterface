#ifndef _USERDef_H
#define _USERDef_H
/******************����ʱʹ�ã�ͨ���û�����************/

#define KEY_TYPE_BUTTON_KEY		//CSP���壬�Ƿ�֧�ְ�ťkey����

#define ZCKT_FILE_SUFFIX_NAME	""
//CSP ����
#define	ZCKT_CSP_NAME		L"JYH Key CSP"//�û����ߣ�CSP����ʱʹ��
//�û����ƶ���
#define	ZCKT_USER_NAME 		"Public User"	//APPģ��,����ģ��ʹ��
//��Կ���ļ����ƶ���
#define ZCKT_USER_VERSION_NAME	L"ͨ�ò��԰汾"//�û�����ʹ��
#define ZCKT_VERSION_NO		L"3.8"
//URL����
#define ZCKT_URL		""		//����ģ��ʹ��
#define	ZCKT_PRI_KEY_FILE_NAME 	"PriKey.key"//����ģ��ʹ��
//��Կ����
#define ZCKT_USER_RSA_PUB_KEY_MODULUS	(BYTE*)"\xBD\xFE\xA9\xAB\xD2\xA9\x2C\x19\xD7\x21\xB6\x43\xCA\x49\x90\xD3\xB6\xCC\xF9\x59\xE0\xE6\x1B\x21\x2A\xCE\x9E\x13\x69\xF9\xC3\x95\xAC\x2A\x55\x2D\x89\xF2\x42\x8C\x82\xC4\xB\xB0\x15\x29\x48\x50\x9C\x2F\x10\x17\x16\xAB\x11\x7E\xA7\x78\xE3\x23\xC4\x80\xA2\xA0\x2A\xF2\x49\xD5\x93\x59\xCD\xC1\x4F\x52\xB6\x72\x5F\xE9\x44\xAA\xB9\x8A\x1F\x3C\xF4\x7F\xEA\xF2\xDE\xE3\xE5\x68\x84\xD6\xB\x4D\x11\x7\x73\xA5\x7E\x16\xC6\x5F\xBF\xA5\x83\x19\x92\x7C\xF4\xB1\xAD\xB4\xA8\x3C\xD7\x92\x7C\xC1\x6F\x11\xA3\xB1\xC4\xAC\x8A\x17"
#define ZCKT_USER_RSA_PUB_KEY_EXPONENT	(BYTE*)"\x01\x00\x01"
//unsigned char g_Modulus[128]={0xBD,0xFE,0xA9,0xAB,0xD2,0xA9,0x2C,0x19,0xD7,0x21,0xB6,0x43,0xCA,0x49,0x90,0xD3,0xB6,0xCC,0xF9,0x59,0xE0,0xE6,0x1B,0x21,0x2A,0xCE,0x9E,0x13,0x69,0xF9,0xC3,0x95,0xAC,0x2A,0x55,0x2D,0x89,0xF2,0x42,0x8C,0x82,0xC4,0xB,0xB0,0x15,0x29,0x48,0x50,0x9C,0x2F,0x10,0x17,0x16,0xAB,0x11,0x7E,0xA7,0x78,0xE3,0x23,0xC4,0x80,0xA2,0xA0,0x2A,0xF2,0x49,0xD5,0x93,0x59,0xCD,0xC1,0x4F,0x52,0xB6,0x72,0x5F,0xE9,0x44,0xAA,0xB9,0x8A,0x1F,0x3C,0xF4,0x7F,0xEA,0xF2,0xDE,0xE3,0xE5,0x68,0x84,0xD6,0xB,0x4D,0x11,0x7,0x73,0xA5,0x7E,0x16,0xC6,0x5F,0xBF,0xA5,0x83,0x19,0x92,0x7C,0xF4,0xB1,0xAD,0xB4,0xA8,0x3C,0xD7,0x92,0x7C,0xC1,0x6F,0x11,0xA3,0xB1,0xC4,0xAC,0x8A,0x17};
//unsigned char g_Exponent[3]={0x1,0x0,0x1};//APPģ��ʹ��

/**********��װʱʹ��*******/
//��װĬ��·��
#define ZCKT_CUSTOMNAME          "USBKEY��ȫ�׼�"//��װʱʹ��
#define ZCKT_CUSTOMPADSTR        "����Ա���û�����"//��װʱʹ��
//��̬������
//#define ZCKT_DLL_CSP_NAME		"Csp.dll"//��װ,����ʱʹ��
//#define ZCKT_DLL_P11_NAME		"P11.dll"//��װ,����ʱʹ��
//#define ZCKT_DLL_APP_NAME		"App.dll"//��װ,����ʱʹ��
//#define ZCKT_DLL_READER_NAME		"Reader.dll"//��װ,����ʱʹ��
//#define ZCKT_DLL_COS_NAME		"Cos.dll"//��װ,����ʱʹ��

//������ģ�����������ƶ���
//#define ZCKT_READER_TYPE_COUNT	4//��װ,����ʱʹ��
//#define ZCKT_READER_DLL_NAME1	"zc100H"//��װ,����ʱʹ��
//#define ZCKT_READER_DLL_NAME2	"pcsc"//��װ,����ʱʹ��
//#define ZCKT_READER_DLL_NAME3	"zc100U"//��װ,����ʱʹ��
//#define ZCKT_READER_DLL_NAME4	"zc100HB"//��װ,����ʱʹ��

//COSģ�����������ƶ���
//#define ZCKT_COS_TYPE_COUNT			3//��װ,����ʱʹ��

//#define ZCKT_COS_TYPE_DLL_NAME1		"zc100"//��װ,����ʱʹ��
//#define ZCKT_COS_TYPE_DLL_NAME2		"z2000"//��װ,����ʱʹ��
//#define ZCKT_COS_TYPE_DLL_NAME3		"JCos"//��װ,����ʱʹ��




//�����ڴ�����
#define ZCKT_SHARE_MEMORY_NAME	"{4F9EE966-B1FD-454d-903B-9682C8BF8476}_shm1"//APPģ��ʹ��
//�����ڴ���������ź�����
#define ZCKT_SHARE_MEMORY_OPERATE_MUTEX_NAME	"{4F9EE966-B1FD-454d-903B-9682C8BF8476}_mtx1"//APPģ��ʹ��
#define ZCKT_SHARE_DEV_OPERATE_MUTEX_NAME	"{4F9EE966-B1FD-454d-903B-9682C8BF8476}_mtx2"//APPģ��ʹ��

#endif
