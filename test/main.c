#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>  
#include <malloc.h>
#include <asm/types.h>
#include  <base/_reader.h>
#include  <base/_cos.h>
#include  <base/_typedefine.h>
#include  <sys/time.h>
#include <stdlib.h>  
#include <semaphore.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>  

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

 

#define INVALID_HANDLE_VALUE 0xFFFFFFFF
#define TEST_LEN   1488


const BYTE adminPin[6] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36};
BYTE iv[16] = {0x00};
 BYTE key[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    BYTE dataIn[1500] = 
    {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    };

DEV_HANDLE CardHandle = NULL;
void close_device(void)
{
    P_Close(CardHandle);
}
bool open_device(void)
{

    P_GetSlotList(&CardHandle,NULL);
    if (CardHandle==0)
    {
 
      return FALSE;
    } 

    return TRUE;
}

bool load_key(void)
{
     bool result=FALSE;
    DEV_RES res = 0x0;
    unsigned long revlen;
    BYTE apdu[300];
    BYTE rdata[300];
    int i = 0;
    int j = 10;
    BYTE dataOut[1500];
    ULONG len;
    unsigned char alg = SM1_ALG;

    revlen = 5;
    memcpy(apdu, "\x00\x84\x00\x00\x04", 5);
    res = P_SendApduComm(CardHandle,5,apdu,&revlen,rdata);
    if(res != 0x9000)
    {
        printf("get random response Error! %d \n", res);
	    P_Close(CardHandle);
        return FALSE;
    }
 
    res = Z_CreateFileSys(CardHandle, adminPin, 6);
    if(res != 0x9000)
    {
        printf("Creat File Sys Error!\n");
	    P_Close(CardHandle);
        return FALSE;
    }
    res = Z_LoadSymmetricKey(CardHandle, alg, key, adminPin, 6);
    if(res != 0x9000)
    {
        printf("Load Key Error!\n");
	    P_Close(CardHandle);
        return FALSE;
    }  
 
    printf("\n");   

}

void test_sm1_cbc(void)
{

    struct timeval start;
    struct timeval end;
    unsigned long timer;

    bool result=FALSE;
    DEV_RES res = 0x0;
    unsigned long revlen;
    BYTE apdu[300];
    BYTE rdata[300];
    int i = 0;
    int j = 100;
       BYTE dataOut[1500];
    ULONG len;
       unsigned char alg = SM1_ALG;


    printf("///////////////////////////////////////////////////////\n");
         printf("=================>  TEST SM1 CBC \n");
    printf("///////////////////////////////////////////////////////\n");
 
  
    
    if(0== CardHandle)
    {
       printf("handle err\r\n");
       return;
    }
    while(j--)
    {
        
        printf("\r\n=================>  TEST SM1 CBC %d===========\r\n", j);
        gettimeofday(&start, NULL); 
        memset(dataOut, 0x00, sizeof(dataOut));
        gettimeofday(&start, NULL); 
        res = Z_SymmetricAlgCalc(CardHandle, true, alg, CBC_MODE, iv, dataIn, TEST_LEN, dataOut, &len);
        gettimeofday(&end, NULL); 
        if(res != 0x9000)
        {
            printf("CBC ENC Error!\n");
	    P_Close(CardHandle);
            return;
        }
        else
        {
            timer = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            printf("timer = %ld us\n", timer);
            printf("cbc Enc Result: \n");
 
           printf("\n");           
        } 

        memset(dataIn, 0x00, sizeof(dataIn));
        gettimeofday(&start, NULL); 
        res = Z_SymmetricAlgCalc(CardHandle, false, alg, CBC_MODE, iv, dataOut, TEST_LEN, dataIn, &len);
        gettimeofday(&end, NULL); 
        if(res != 0x9000)
        {
            printf("CBC Dec Error!\n");
	    P_Close(CardHandle);
            return;
        }
        else
        {
            timer = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            printf("timer = %ld us\n", timer);
            printf("cbc dec Result: \n");
 
           printf("\n");           
        }
 
        sleep(2);
    }

}



void test_sm1_ecb(void)
{

    struct timeval start;
    struct timeval end;
    unsigned long timer;

    bool result=FALSE;
    DEV_RES res = 0x0;
    unsigned long revlen;
    BYTE apdu[300];
    BYTE rdata[300];
    int i = 0;
    int j = 100;
       BYTE dataOut[1500];
    ULONG len;

    unsigned char alg = SM1_ALG;
    printf("///////////////////////////////////////////////////////\n");
         printf("=================>  TEST SM1 \n");
    printf("///////////////////////////////////////////////////////\n");
 
    
    if(0== CardHandle)
    {
       printf("handle err\r\n");
       return;
    }
    while(j--)
    {
   
         printf("\r\n=================>  TEST SM1 ECB %d===========\r\n", j);
        gettimeofday(&start, NULL); 
        res = Z_SymmetricAlgCalc(CardHandle, true, alg, ECB_MODE, NULL, dataIn, TEST_LEN, dataOut, &len);
        gettimeofday(&end, NULL); 
        if(res != 0x9000)
        {
            printf("ECB ENC Error!\n");
	    P_Close(CardHandle);
            return;
        }
        else
        {
            timer = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            printf("timer = %ld us\n", timer);
            printf("sm1 ecb Enc Result: \n");
 
            printf("\n");    
        }
        
        memset(dataIn, 0x00, sizeof(dataIn));
        gettimeofday(&start, NULL); 
        res = Z_SymmetricAlgCalc(CardHandle, false, alg, ECB_MODE, NULL, dataOut, TEST_LEN, dataIn, &len);
        gettimeofday(&end, NULL); 
        if(res != 0x9000)
        {
            printf("ECB DEC Error!\n");
	    P_Close(CardHandle);
            return;
        }
        else
        {
            timer = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            printf("timer = %ld us\n", timer);
            printf("ecb dec Result: \n");
 
            printf("\n");    
        }
        sleep(1);
    }

}
 

void *thread_test_SM1_ecb(void *arg)
{
     
    test_sm1_ecb();
}

void *thread_test_SM1_cbc(void *arg)
{
    test_sm1_cbc();
}
 

   

int shmid;  
void *shm = NULL;//分配的共享内存的原始首地址  
int shmKey = 2897;

struct shared_st  
{  
   unsigned char flag;  //1: get status; 0: finish
   unsigned char status; //0: CLOSE ; 1: OPENED ; 2: RUN_OK; 3: RUN_ERR;
};   
struct shared_st  *shared = NULL;//指向shm  
int create_shm(void)
{
 
   //创建共享内存  
   shmid = shmget((key_t)shmKey, sizeof(struct shared_st), 0666|IPC_CREAT);  
   if(shmid == -1)  
   {  
       fprintf(stderr, "shmget failed\n");  
          return -1;  
   }  
  //将共享内存连接到当前进程的地址空间  
   shm = shmat(shmid, (void*)0, 0);  
   if(shm == (void*)-1)  
   {  
        fprintf(stderr, "shmat failed\n");  
          return -1;  
    }  
    printf("Memory attached at %X\n", (int)shm);  
   //设置共享内存  
   shared = (struct shared_st *)shm;  
   return 0;
}

void delete_shm(void)
{
    //把共享内存从当前进程中分离  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
         return ;//exit(EXIT_FAILURE);  
    }  
    //删除共享内存  
    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        return ;//exit(EXIT_FAILURE);  
    } 
    shared = NULL;

}


 
union semun  
{  
    int val;  
    struct semid_ds *buf;  
    unsigned short *arry;  
};  
  
static int sem_id = 0;  
static int sem_key = 9876;
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

#define GET_CARD_STATUS_FINISH  0  
#define GET_CARD_STATUS_START   1


void *thread_check_device_status(void *arg)
{
    int times = 10;
    unsigned char status = 0;
  //创建信号量  
    sem_id = semget((key_t)sem_key, 1, 0666 | IPC_CREAT);  
    if(!set_semvalue())  
    {  
        fprintf(stderr, "Failed to initialize semaphore\n");  
        return FALSE;  
    } 
    create_shm();
    while(1)
    {
        if(!semaphore_p())  
        {
           printf("semaphore_p err\r\n") ; 
           break;  
        }
        if (shared->flag == GET_CARD_STATUS_START)   
        {     
            status = P_GetCardStatus(CardHandle); 
            shared->status = status;
            shared->flag = GET_CARD_STATUS_FINISH;
            sleep(1);   
        }
 //       else if (shared->flag == 2)
 //       {
 //           if(!semaphore_v())  
 //           {
 //              printf("semaphore_v err\r\n") ;   
 //           }
 //           break;
 //       }
        if(!semaphore_v())  
        {
           printf("semaphore_p err\r\n") ;  
           break; 
        }   
        sleep(1);
    }
    delete_shm();
    del_semvalue();
 
}

int main(int argc, char* argv[])
{
  
    int time = 15;
     pthread_t threadIdSM1;  
    pthread_t threadIdSM4;  
    pthread_t threadIdCardStatus;  
    char buf[80]; 
    void *thread_result; 
    int res;  
    
    FILE * out_file; 
    if (FALSE == open_device())
    {
        printf("open device err\r\n");
        return 1;
    }

    if (FALSE == load_key())
    {
        printf("load key err\r\n");
        return 1;
    }
     res = pthread_create(&threadIdSM1, NULL,  thread_test_SM1_cbc,NULL);  
    if (res != 0)  
    {  
        perror("Thread SM1 cbc failed!");  
        exit(EXIT_FAILURE);  
    }
    
    res = pthread_create(&threadIdSM4, NULL, thread_test_SM1_ecb, NULL);  
    if (res != 0)  
    {  
        perror("Thread SM1 ecb failed!");  
        exit(EXIT_FAILURE);  
    }
  

    res = pthread_create(&threadIdCardStatus, NULL, thread_check_device_status, NULL);  
    if (res != 0)  
    {  
        perror("Thread thread_check_device_status failed!");  
        exit(EXIT_FAILURE);  
    }
 
    while(time--)
    {
        sleep(1);    
    }
    close_device();
    //test 
}
