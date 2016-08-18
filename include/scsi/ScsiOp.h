#ifndef _SCSIOP_H_
#define _SCSIOP_H_

#ifdef WIN32
#include <setupapi.h>
#include <winioctl.h>
#include "scsi.h"

// �洢�豸����������
// #ifndef COM
// 	typedef enum _STORAGE_BUS_TYPE {
// 		BusTypeUnknown = 0x00,
// 		BusTypeScsi,
// 		BusTypeAtapi,
// 		BusTypeAta,
// 		BusType1394,
// 		BusTypeSsa,
// 		BusTypeFibre,
// 		BusTypeUsb,
// 		BusTypeRAID,
// 		BusTypeMaxReserved = 0x7F
// 	} STORAGE_BUS_TYPE, *PSTORAGE_BUS_TYPE;
// #endif

// ��ѯ�洢�豸���Ե�����
#if _MSC_VER < 1500
typedef enum _STORAGE_QUERY_TYPE {
	PropertyStandardQuery = 0,          // ��ȡ����
	PropertyExistsQuery,                // �����Ƿ�֧��
	PropertyMaskQuery,                  // ��ȡָ��������
	PropertyQueryMaxDefined             // ��֤����
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;
  
// ��ѯ�洢�豸��������������
typedef enum _STORAGE_PROPERTY_ID {
	StorageDeviceProperty = 0,          // ��ѯ�豸����
	StorageAdapterProperty              // ��ѯ����������
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;
  
// ��ѯ������������ݽṹ
typedef struct _STORAGE_PROPERTY_QUERY {
	STORAGE_PROPERTY_ID PropertyId;     // �豸/������
	STORAGE_QUERY_TYPE QueryType;       // ��ѯ���� 
	UCHAR AdditionalParameters[1];      // ���������(�������������Ե�1���ֽ�)
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;
  
// ��ѯ������������ݽṹ
typedef struct _STORAGE_DEVICE_DESCRIPTOR {
	ULONG Version;                    // �汾
	ULONG Size;                       // �ṹ��С
	UCHAR DeviceType;                 // �豸����
	UCHAR DeviceTypeModifier;         // SCSI-2������豸����
	BOOLEAN RemovableMedia;           // �Ƿ���ƶ�
	BOOLEAN CommandQueueing;          // �Ƿ�֧���������
	ULONG VendorIdOffset;             // �����趨ֵ��ƫ��
	ULONG ProductIdOffset;            // ��ƷID��ƫ��
	ULONG ProductRevisionOffset;      // ��Ʒ�汾��ƫ��
	ULONG SerialNumberOffset;         // ���кŵ�ƫ��
	STORAGE_BUS_TYPE BusType;         // ��������
	ULONG RawPropertiesLength;        // ������������ݳ���
	UCHAR RawDeviceProperties[1];     // �������������(�������������Ե�1���ֽ�)
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;
#endif
#if defined(_WIN64)
	typedef unsigned __int64 ULONG_PTR;
#else
	typedef unsigned long ULONG_PTR;
#endif

typedef struct _SCSI_PASS_THROUGH {
	USHORT		Length;
	UCHAR		ScsiStatus;
	UCHAR		PathId;
	UCHAR		TargetId;
	UCHAR		Lun;
	UCHAR		CdbLength;
	UCHAR		SenseInfoLength;
	UCHAR		DataIn;
	ULONG		DataTransferLength;
	ULONG		TimeOutValue;
	ULONG_PTR	DataBufferOffset;
	ULONG		SenseInfoOffset;
	UCHAR		Cdb[16];
} SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;

typedef struct _SCSI_PASS_THROUGH_WITH_BUFFERS {
    SCSI_PASS_THROUGH Spt;
    ULONG             Filler;      // realign buffers to double word boundary
    UCHAR             SenseBuf[32];
    //UCHAR             DataBuf[4096+512];
	UCHAR             DataBuf[16384+512];
} SCSI_PASS_THROUGH_WITH_BUFFERS;

typedef struct {
	USHORT Length;
	UCHAR  ScsiStatus;
	UCHAR  PathId;
	UCHAR  TargetId;
	UCHAR  Lun;
	UCHAR  CdbLength;
	UCHAR  SenseInfoLength;
	UCHAR  DataIn;
	ULONG  DataTransferLength;
	ULONG  TimeOutValue;
	PVOID  DataBuffer;
	ULONG  SenseInfoOffset;
	UCHAR  Cdb[16];
} SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;

typedef struct {
	SCSI_PASS_THROUGH_DIRECT Spt;
	UCHAR ucSenseBuf[32];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

#define CDB6GENERIC_LENGTH				6
#define CDB10GENERIC_LENGTH				10
#define CDB12GENERIC_LENGTH				12

#define SCSI_IOCTL_DATA_OUT				0
#define SCSI_IOCTL_DATA_IN				1
#define SCSI_IOCTL_DATA_UNSPECIFIED		2

#if _MSC_VER < 1500
#define offsetof(s,m) (size_t)&(((s *)0)->m)
#endif

// IOCTL������
#define IOCTL_SCSI_BASE                 FILE_DEVICE_CONTROLLER
#define IOCTL_SCSI_PASS_THROUGH         CTL_CODE(IOCTL_SCSI_BASE, 0x0401, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_MINIPORT             CTL_CODE(IOCTL_SCSI_BASE, 0x0402, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_INQUIRY_DATA     CTL_CODE(IOCTL_SCSI_BASE, 0x0403, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_GET_CAPABILITIES     CTL_CODE(IOCTL_SCSI_BASE, 0x0404, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_PASS_THROUGH_DIRECT  CTL_CODE(IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_ADDRESS          CTL_CODE(IOCTL_SCSI_BASE, 0x0406, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_RESCAN_BUS           CTL_CODE(IOCTL_SCSI_BASE, 0x0407, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_GET_DUMP_POINTERS    CTL_CODE(IOCTL_SCSI_BASE, 0x0408, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_FREE_DUMP_POINTERS   CTL_CODE(IOCTL_SCSI_BASE, 0x0409, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IDE_PASS_THROUGH          CTL_CODE(IOCTL_SCSI_BASE, 0x040a, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_ATA_PASS_THROUGH          CTL_CODE(IOCTL_SCSI_BASE, 0x040b, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_ATA_PASS_THROUGH_DIRECT   CTL_CODE(IOCTL_SCSI_BASE, 0x040c, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_STORAGE_QUERY_PROPERTY	CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

#elif defined(linux) || defined(macintosh)

#define BLUK_IN      0x81
#define BLUK_OUT     0x02
#define PACKETSIZE   2048 

#pragma pack(1)
//command pack
typedef struct UMS_SCSI_CBW
{
	ES_UINT		dCBWSignature;		// 0x43425355
	ES_UINT		dCBWTag;		// random, same as receive 
	ES_UINT		dCBWDataTransferLength;	// send length or want receive length
	CK_BYTE		bmCBWFlags;		// 00 send, 80 receive
	CK_BYTE		bCBWLUN;		// 00 
	CK_BYTE		bCBWCBLength;		// length of CBWCB 
	CK_BYTE		CBWCB[49];		// command data
} UMS_SCSI_CBW/* __attribute__((packed))*/;

//receive pack
typedef struct UMS_SCSI_CSW
{
	ES_UINT		dCSWSignature;		// 0x53425355
	ES_UINT		dCSWTag;		// random, same as send
	ES_UINT		dCSWDataResidue;	// 00
	CK_BYTE		bCSWStatus;		// the status of command, 0 success, 1 fault
} UMS_SCSI_CSW;
#pragma pack()

#endif //WIN32
#endif //_SCSIOP_H_

