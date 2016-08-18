#include <base/typedefine.h>
#include "stdio.h"
#include <base/reader.h>
#include <errno.h>
#include <asm/types.h>
//#include <linux/hiddev.h>
#include <sys/ioctl.h>
#include <usb.h>

#include <sys/time.h>
#include <pthread.h>

#include <sys/shm.h>  
 
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h>  

#include "map"
#include "string"
#include "vector"
using namespace std;
#define ESLOG_INF(func) printf
#define ESLOG_ERR(func) printf
typedef unsigned long CK_RV;
typedef unsigned char* CK_BYTE_PTR;
typedef unsigned char CK_BYTE;
typedef unsigned long* CK_ULONG_PTR;
typedef unsigned long CK_ULONG;
typedef void* CK_VOID_PTR;

#define CKR_DEVICE_ERROR    0x30
#define CKR_GENERAL_ERROR	0x31
#define CKR_BUFFER_TOO_SMALL	0x32
#define CKR_RET_ERROR			0x33
#define CKR_OK				0x00
#define INVALID_HANDLE_VALUE 0xFFFFFFFF
#define SW_WORD_DEVICE_ERROR				0xFFFF
#define SW_WORD_ARGUMENT_BAD				0xFFFE
#define SW_WORD_LENGTH_WRONG				0xFFFD
#define SW_WORD_POINTER_WRONG			  0xFFFC
#define READ_OVERLAPPED_TIMEOUT		10000
#include "scsi/ScsiOp.h"


  
#define TEXT_SZ 2048  
 

#define KEY_STATUS_CLOSE   0
#define KEY_STATUS_OPENED  1 
#define KEY_STATUS_RUN_OK  2
#define KEY_STATUS_RUN_ERR 3   
static unsigned char keyStatus = KEY_STATUS_CLOSE;
 

ULONG g_ErrCode=0;
char g_ErrMsg[MAX_NAME_LEN];

map <DEV_HANDLE, string,less<DEV_HANDLE> > g_OpenMap;
map <string,string,less<string> > g_DeviceMap;

#define LOCKTIMEOUT    5000       
pthread_mutex_t g_Mutex;

typedef signed   int    INT32;

static int commCnt = 0;


INT32 MutexLock();
INT32 MutexUnLock();

 
union semun  
{  
    int val;  
    struct semid_ds *buf;  
    unsigned short *arry;  
};  
  
static int sem_id = 0;  
  
static int set_semvalue();  
static void del_semvalue();  
static int semaphore_p();  
static int semaphore_v();  

static int set_semvalue()  
{  
    //用于初始化信号量，在使用信号量前必须这样做  
    union semun sem_union;  
  
    sem_union.val = 1;  
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1)  
        return 0;  
    return 1;  
}  
  
static void del_semvalue()  
{  
    //删除信号量  
    union semun sem_union;  
  
    if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)  
        fprintf(stderr, "Failed to delete semaphore\n");  
}  
  
static int semaphore_p()  
{  
    //对信号量做减1操作，即等待P（sv）  
    struct sembuf sem_b;  
    sem_b.sem_num = 0;  
    sem_b.sem_op = -1;//P()  
    sem_b.sem_flg = SEM_UNDO;  
    if(semop(sem_id, &sem_b, 1) == -1)  
    {  
        fprintf(stderr, "semaphore_p failed\n");  
        return 0;  
    }  
    return 1;  
}  
  
static int semaphore_v()  
{  
    //这是一个释放操作，它使信号量变为可用，即发送信号V（sv）  
    struct sembuf sem_b;  
    sem_b.sem_num = 0;  
    sem_b.sem_op = 1;//V()  
    sem_b.sem_flg = SEM_UNDO;  
    if(semop(sem_id, &sem_b, 1) == -1)  
    {  
        fprintf(stderr, "semaphore_v failed\n");  
        return 0;  
    }  
    return 1;  
} 
  

INT32 MutexLock()
{
    struct timeval  tCurrentTime;
    struct timespec tTimeout;

    INT32 iRetCode = 0;
 
    if(!semaphore_p())  
       return -1;  
    return 0;
}

INT32 MutexUnLock()
{
    if(!semaphore_v())  
       return -1;  
    return 0; 
}



bool ConnectToken(char *AccessPath,void** pTokenHandle)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	usb_dev_handle *udev;
	char cTemp[2*PATH_MAX + 1];
	CK_RV rv;
	usb_find_busses();
	usb_find_devices();
	for(bus = usb_busses; bus; bus = bus->next)
	{
		for(dev = bus->devices; dev; dev = dev->next)
		{
			memset(cTemp,0,2*PATH_MAX + 1);
			sprintf(cTemp,"%s:%s",dev->filename,dev->bus->dirname);
			ESLOG_INF(("filename=%s;dirname=%s;m_strLinkName=%s",dev->filename,dev->bus->dirname,AccessPath));
			if(strcmp(cTemp,AccessPath) == 0)
			{
				udev = usb_open(dev);
				if(udev)
				{
					usb_reset(udev);
					
					usb_detach_kernel_driver_np(udev,0);
					if(pTokenHandle)
						*pTokenHandle =(void*) udev;

					ESLOG_INF(("usb_open ok m_TokenHandle=%x",udev));
					usb_claim_interface(udev,0);
					return true;
				}
				else
				{
    				    printf("dev is null\r\n");
				}
				break;
			}
		}

	}
	return false;
}

bool DisConnectToken(void *TokenHandle)
{
	if(TokenHandle != (void*)INVALID_HANDLE_VALUE)
	{
		usb_dev_handle *udev = (usb_dev_handle *)TokenHandle;
		usb_close(udev);
	}
	TokenHandle = (void*)INVALID_HANDLE_VALUE;
	return true;
}

CK_RV im_SendToCOS(void *handle,const CK_BYTE* pSendApdu, CK_ULONG ulSendLen,
				   CK_BYTE* pRecvBuff, CK_ULONG* pulRecLen, CK_ULONG timeout)
{
	if(handle == (void*)INVALID_HANDLE_VALUE)
	{
		printf("%d error!\n",__LINE__);
		return CKR_GENERAL_ERROR;
	}


	int ret;
	usb_dev_handle *udev = (usb_dev_handle *)handle;

	UMS_SCSI_CBW  	blksend ={0}; 
	UMS_SCSI_CSW    blkRev ={0}; 

	if((udev == NULL) || (pRecvBuff == NULL))
	{
		printf("%d error!\n",__LINE__);
		return CKR_GENERAL_ERROR;
	}
	try
	{	
		memset(&blksend, 0, sizeof(blksend));		
		blksend.dCBWSignature = 0x43425355;
		blksend.dCBWTag = 0x11223344;

		if(ulSendLen<=5)
		{
			//send CBW
			blksend.dCBWDataTransferLength = *pulRecLen + 2;	
			blksend.bmCBWFlags = 0x80;
			blksend.bCBWLUN = 0x00;
			blksend.bCBWCBLength = 0x07;		
			blksend.CBWCB[0] = 0xFE;		
			blksend.CBWCB[1] = 0x00;			
			memcpy(&blksend.CBWCB[2], pSendApdu, 5);		
			ret = usb_bulk_write(udev, BLUK_OUT, (char *)(&blksend), 31, timeout);
			//printf("ret1: %d", ret);	
			if(ret!=31)
			{
				printf("%d error!\n",__LINE__);
				return CKR_RET_ERROR;		
			}
			//read DATA
			ret = usb_bulk_read(udev, BLUK_IN, (char *)pRecvBuff, blksend.dCBWDataTransferLength, timeout);
			//printf("ret2: %d", ret);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)pRecvBuff, blksend.dCBWDataTransferLength, timeout);
			}	
			*pulRecLen = ret;	//µÃµœÊµŒÊœÓÊÕ³€¶È			
			//read CSW
			ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			//printf("ret3: %d", ret);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			}					
		}
		else
		{
			//send CBW
			blksend.dCBWDataTransferLength = ulSendLen-5;	
			blksend.bmCBWFlags = 0x00;
			blksend.bCBWLUN = 0x00;
			blksend.bCBWCBLength = 0x07;		
			blksend.CBWCB[0] = 0xFE;		
			blksend.CBWCB[1] = 0x00;			
			memcpy(&blksend.CBWCB[2], pSendApdu, 5);		
			ret = usb_bulk_write(udev, BLUK_OUT, (char *)(&blksend), 31, timeout);
			//printf("rets1: %d", ret);	
			if(ret!=31)
			{
				printf("%d error!%x\n",__LINE__,ret);
				return CKR_RET_ERROR;		
			}
			//send DATA
			ret = usb_bulk_write(udev, BLUK_OUT, (char *)(pSendApdu+5), blksend.dCBWDataTransferLength, timeout);
			//printf("rets2: %d", ret);	
			if(ret!=blksend.dCBWDataTransferLength)
			{
				printf("%d error!\n",__LINE__);
				return CKR_RET_ERROR;		
			}
			//read CSW	
			ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			//printf("rets3: %d", ret);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			}	
			//send CBW
			blksend.dCBWDataTransferLength = 2;	
			blksend.bmCBWFlags = 0x80;
			blksend.bCBWLUN = 0x00;
			blksend.bCBWCBLength = 0x02;	
			memset(&blksend.CBWCB[0], 0, 16);						
			blksend.CBWCB[0] = 0xFE;		
			blksend.CBWCB[1] = 0x01;				
			ret = usb_bulk_write(udev, BLUK_OUT, (char *)(&blksend), 31, timeout);
			//printf("rets1: %d", ret);	
			if(ret!=31)	
			{
				printf("%d error!\n",__LINE__);
				return CKR_RET_ERROR;		
			}
			//read DATA
			ret = usb_bulk_read(udev, BLUK_IN, (char *)pRecvBuff, blksend.dCBWDataTransferLength, timeout);
			//printf("retr2: %d", ret);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)pRecvBuff, blksend.dCBWDataTransferLength, timeout);
			}	
			*pulRecLen = ret;	//µÃµœÊµŒÊœÓÊÕ³€¶È					
			//read CSW
			ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			//printf("rets3: %d", ret);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			}						
		}
		//if 61 XX
		if((pRecvBuff[0]==0x61)&&(*pulRecLen==2))
		{	
			printf("%02x%02x(%d)",pRecvBuff[0],pRecvBuff[1],*pulRecLen);
			//send CBW(00 C0)
			if(pRecvBuff[1]==0x00)	blksend.dCBWDataTransferLength = 258;
			else blksend.dCBWDataTransferLength = pRecvBuff[1]+2;	
			blksend.bmCBWFlags = 0x80;
			blksend.bCBWLUN = 0x00;
			blksend.bCBWCBLength = 0x07;	
			memset(&blksend.CBWCB[0], 0, 16);						
			blksend.CBWCB[0] = 0xFE;		
			blksend.CBWCB[1] = 0x00;			
			blksend.CBWCB[2] = 0x00;
			blksend.CBWCB[3] = 0xC0;
			blksend.CBWCB[4] = 0x00;
			blksend.CBWCB[5] = 0x00;
			blksend.CBWCB[6] = pRecvBuff[1];	

			ret = usb_bulk_write(udev, BLUK_OUT, (char *)(&blksend), 31, timeout);	
			if(ret!=31)
			{
				printf("%d error!\n",__LINE__);	
				return CKR_RET_ERROR;		
			}
			//read DATA 
			ret = usb_bulk_read(udev, BLUK_IN, (char *)pRecvBuff, blksend.dCBWDataTransferLength, timeout);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)pRecvBuff, blksend.dCBWDataTransferLength, timeout);
			}	
			printf("ret=%x\n",ret);
			*pulRecLen = ret;	//µÃµœÊµŒÊœÓÊÕ³€¶È					
			//read CSW	
			ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			//printf("rets3: %d", ret);
			while(-EPIPE == ret)
			{
				while(-EPIPE == ret)
				{
					ret = usb_clear_halt(udev,BLUK_IN);
				}
				ret = usb_bulk_read(udev, BLUK_IN, (char *)&blkRev, 13, timeout);
			}						
		}			
	}
	catch (...)
	{
		ESLOG_ERR(("im_TxAPDU catch error : %d", errno));
		*pulRecLen = 0;
		return CKR_GENERAL_ERROR;
	}
	return CKR_OK;

}

unsigned short im_Transmit(void* handle,const CK_BYTE* pbApdu, CK_ULONG ulSendLen, CK_BYTE* pRetData, CK_ULONG* pulRetLen, CK_ULONG ulMiliSecondTimeout)
{
	CK_ULONG ulRetLen = *pulRetLen;	
 
	//·µ»ØŽíÎó×ŽÌ¬×Ö
	if ((NULL == pbApdu) || (NULL == pRetData) || (NULL == pulRetLen))
	{
		printf("%d error!\n",__LINE__);
		return SW_WORD_POINTER_WRONG;
	}

	if ((ulSendLen<5) || (ulRetLen > MAX_PATH))
	{
		printf("%d error!\n",__LINE__);
		return SW_WORD_LENGTH_WRONG;
	}

	CK_RV rv = im_SendToCOS(handle, pbApdu, ulSendLen, pRetData, &ulRetLen, ulMiliSecondTimeout);

	while((rv==CKR_OK)&&(ulRetLen==2)&&(pRetData[0]==0x51)&&(pRetData[1]==0x15))
	{
		usleep(200);	
		ulRetLen = 0;	
		rv = im_SendToCOS(handle, pbApdu, ulSendLen, pRetData, &ulRetLen, ulMiliSecondTimeout);		
	}	

	if (CKR_OK != rv)
	{
		printf("%d error!\n",__LINE__);
		return SW_WORD_DEVICE_ERROR;
	}

	//ÒªœÓÊÕµÄ³€¶ÈÐ¡ÓÚÊµŒÊ³€¶È
	if (*pulRetLen < (ulRetLen - 2))
	{
		printf("%d error!(%d<%d)%x\n",__LINE__,*pulRetLen,ulRetLen - 2,rv);
		*pulRetLen = ulRetLen - 2;
		return SW_WORD_LENGTH_WRONG;
	}

	*pulRetLen = ulRetLen - 2;


	return pRetData[ulRetLen - 2] * 0x100 + pRetData[ulRetLen - 1];
}

void P_SetErrMsg(ULONG code,char *msg)
{
	g_ErrCode=code;
	strcpy(g_ErrMsg,msg);
}

unsigned char P_GetCardStatus(DEV_HANDLE CardHandle)
{
  	BYTE resp[MAX_PATH]; 
    ULONG respLen=0x22;

    if (keyStatus == KEY_STATUS_CLOSE)
    {
        return keyStatus;
    }
    else  if ((keyStatus == KEY_STATUS_OPENED) || (keyStatus == KEY_STATUS_RUN_OK))
    {
            
        WORD res=P_SendApduComm(CardHandle,5,(BYTE*)"\x80\x10\x00\x00\x10",&respLen,resp);
        if(res!=0x9000)
	    {
            keyStatus = KEY_STATUS_RUN_ERR;
		    return FALSE;
	    }
        keyStatus = KEY_STATUS_RUN_OK;
	    return keyStatus;             
    }
}

BOOL P_GetSlotList(DEV_HANDLE *pList,ULONG *pCount)
{
	struct usb_bus *busses;
	int iDevices=0,iBusses=0;
	unsigned short response=0x9000;
	unsigned short i = 0;
	int Count=0;

	if(pCount)
            *pCount=0;
	usb_init();
	iBusses=usb_find_busses();
	iDevices=usb_find_devices();
	char AccessPath[2*PATH_MAX + 1];
	void *handle=(void*)INVALID_HANDLE_VALUE;
	while((iBusses == 0) && (iDevices == 0) && (i < 50))
	{
		i++;
		usleep(1000);
		iBusses = usb_find_busses();
		iDevices = usb_find_devices();
	}
	busses=usb_get_busses();
	struct usb_bus *bus;
	int c,a;
	for (bus=busses; bus; bus=bus->next) 
	{
		struct usb_device *dev;
		for (dev=bus->devices; dev; dev=dev->next) 
		{
			printf("usb_find Device1 %s %x %x\n",dev->filename,dev->descriptor.idVendor,dev->descriptor.idProduct);
			if((dev->descriptor.idVendor==0x5948)&&(dev->descriptor.idProduct==0x1001))
			{
 
                //printf("find kyh key, connect key\r\n");
			    sprintf(AccessPath,"%s:%s",dev->filename,dev->bus->dirname);
                //创建信号量  
                sem_id = semget((key_t)5678, 1, 0666 | IPC_CREAT);  
               
               if(!set_semvalue())  
               {  
                    fprintf(stderr, "Failed to initialize semaphore\n");  
                    return FALSE;  
               } 
               MutexLock();
               if(!ConnectToken(AccessPath,&handle)) 
			   {
                    printf("connect false, continue\r\n");
				    continue;
			   }
			   if(pList)
				   pList[Count]=handle;
 
                keyStatus = KEY_STATUS_OPENED;
                MutexUnLock();
 				Count++;
                goto suc;
			}
		}
	}

suc:
	if(pCount)
		*pCount=Count;
	return TRUE;
} 

DEV_RES P_SendApduComm(DEV_HANDLE CardHandle,unsigned long sendlen,const unsigned char *sendbuf,
					   unsigned long *resplen,unsigned char *respbuf)
{
  
	MutexLock();
    commCnt++;
	DEV_RES res= im_Transmit(CardHandle,sendbuf,sendlen,respbuf,resplen,READ_OVERLAPPED_TIMEOUT);
	MutexUnLock();
        return res;
}



BOOL P_Reset(DEV_HANDLE CardHandle,unsigned char *ATR,ULONG *ATRLen)
{
	BYTE resp[MAX_PATH]; ULONG respLen=0x22;
	
        WORD res=P_SendApduComm(CardHandle,5,(BYTE*)"\x80\x10\x00\x00\x10",&respLen,resp);
        if(res!=0x9000)
	{
		return FALSE;
	}

	if(ATRLen)*ATRLen=respLen;
	if(ATR)memcpy(ATR,resp,respLen);
	return TRUE;
} 

BOOL P_GetSlotListName(char pReaderList[][MAX_PATH], ULONG *pCount)
{
	struct usb_bus *busses;
	int iDevices=0,iBusses=0;
	unsigned short response=0x9000;
	unsigned short i;
	int Count=0;
	MutexLock();
        usb_init();
	iBusses=usb_find_busses();
	iDevices=usb_find_devices();
	char AccessPath[PATH_MAX];
	void *handle=(void*)INVALID_HANDLE_VALUE;
	if(pCount)
		*pCount=0;
	while((iBusses == 0) && (iDevices == 0) && (i < 50))
	{
		i++;
		usleep(80*1000);
		iBusses = usb_find_busses();
		iDevices = usb_find_devices();
	}
	busses=usb_get_busses();
	//ÔÚÕâÖ®ºó£¬ÓŠÓÃÓŠžÃÊÖ¶¯ÂÖÑ¯ËùÓÐ×ÜÏßºÍÉè±ž£¬Æ¥ÅäÆäËùÒªµÄ:
	struct usb_bus *bus;
	int c,a;
	for (bus=busses; bus; bus=bus->next) 
	{
		struct usb_device *dev;
		for (dev=bus->devices; dev; dev=dev->next) 
		{
			printf("usb_find Device1 %s %x %x\n",dev->filename,dev->descriptor.idVendor,dev->descriptor.idProduct);
			if((dev->descriptor.idVendor==0x5948)&&(dev->descriptor.idProduct==0x1001))
			{
				sprintf(AccessPath,"%s:%s",dev->filename,dev->bus->dirname);
				strcpy(pReaderList[Count],AccessPath);
				char SizeStr[36];
				sprintf(SizeStr,"%d",g_DeviceMap.size());
				string name=string("linux_usbkey")+string(" ")+string(SizeStr);
				g_DeviceMap.insert(pair<string,string>(AccessPath,name));
				Count++;
			}
		}
	}
	if(pCount)
		*pCount=Count;
        MutexUnLock();
	return TRUE;
}
BOOL P_GetSlotInfo(DEV_HANDLE CardHandle, S_SlotInfo *pInfo)
{
	map <DEV_HANDLE, string,less<DEV_HANDLE> >::iterator it=g_OpenMap.find(CardHandle);
	if(it==g_OpenMap.end())
		return FALSE;

	map <string, string,less<string> >::iterator it1=g_DeviceMap.find(it->second);
	if(it1==g_DeviceMap.end())
		return FALSE;
	BYTE Atr[MAX_PATH];
	ULONG AtrLen=MAX_PATH;
        
	if(P_SendApduComm(CardHandle,5,(BYTE*)"\x80\x10\x00\x01\x10",&AtrLen,Atr)!=0x9000)
	{
               
		return FALSE;
	}
        
	strcpy(pInfo->m_ReaderName,it1->second.data());
	strcpy(pInfo->m_Path,it1->first.data());

	memcpy(pInfo->m_Serial,Atr,sizeof(pInfo->m_Serial));
	pInfo->m_devType=READER_TYPE_UDISK;
	strcpy(pInfo->m_ReaderMafID,"kingyh Credit");
	strcpy(pInfo->m_CardMafID,"kingyh Credit");
	strcpy(pInfo->m_CardName,"kingyh-1000-U");
	return TRUE;
}
BOOL P_Open(const char *ReaderName,DEV_HANDLE *pCardHandle)
{
	char buffer[100];
	DEV_HANDLE handle=NULL;
	memset(buffer,0,100);
	
	MutexLock();
        bool ret = ConnectToken((char*)ReaderName,&handle);
	MutexUnLock();
        if((ret)&&(handle !=(DEV_HANDLE) INVALID_HANDLE_VALUE))
	{
		if(pCardHandle)*pCardHandle=handle;
		map <string, string,less<string> >::iterator it=g_DeviceMap.find(ReaderName);
		if(it!=g_DeviceMap.end())
			g_OpenMap.insert(pair<DEV_HANDLE,string>((DEV_HANDLE)handle,it->first));
		return TRUE;
	}
	return FALSE;
}


BOOL P_Close(DEV_HANDLE CardHandle)
{
/*
	map <DEV_HANDLE, string,less<DEV_HANDLE> >::iterator it=g_OpenMap.find(CardHandle);
	if(it!=g_OpenMap.end())
	{
		g_OpenMap.erase(it);
	}
*/
        
	MutexLock();
	DisConnectToken((DEV_HANDLE)CardHandle);
 
    keyStatus = KEY_STATUS_CLOSE;
	MutexUnLock();
    del_semvalue();
	return TRUE;
}


BOOL P_IsOpened(DEV_HANDLE CardHandle)
{
	return TRUE;
}

ULONG P_GetErrMsg(char *msg)
{
	strcpy(g_ErrMsg,msg);
	return g_ErrCode;
}
ULONG P_GetCosType(DEV_HANDLE CardHandle)
{
	return 5;
}
BOOL P_Initialize(VOID *arg)
{
	return TRUE;
}
void P_Finalize()
{

}
