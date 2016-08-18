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
#include <sys/shm.h>  
#include <sys/shm.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h>  
#include <sys/time.h>
#include <time.h>
 
 
 
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
#define SEM_TIME_OUT_SEC   10  
static int sem_id = 0;  
static int sem_key = 9876;  


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
    int timeOutSec = SEM_TIME_OUT_SEC;
    struct timespec timeOut;

    clock_gettime(CLOCK_REALTIME, &timeOut);
 
    timeOut.tv_sec = timeOutSec;
    if (semtimedop(sem_id, &sem_b, 1, &timeOut))
    {  
        fprintf(stderr, "semaphore_p timout\n");  
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
  


int get_device_status(void)
{
    int timeOut = 10;
    unsigned char status = 0;
    int res = -1;

    sem_id = semget((key_t)sem_key, 1, 0666 | IPC_CREAT);  
    if (-1 == create_shm())
    {
       return res;  
    }

    if(!semaphore_p())  
    {
        return res;  
    } 
    shared->flag = 1;

    if(!semaphore_v())  
    {
       return res;  
    }       
    while((timeOut--))
    {
        if(!semaphore_p())  
        {
           break;  
        }
        if (shared->flag == 1)
        {
            sleep(1);   
        }
        else if (shared->flag == 0)
        { 
            res = shared->status;
            if(!semaphore_v())  
            {
                return -1;      
            }
            break;
        }    
        if(!semaphore_v())  
        {
            return -1;              
        }
    }

    return res;
}


int main(int argc, char* argv[])
{ 
    int res;  
    int j = 1;
 
    res = get_device_status();
    if (res == -1)    
        printf("device process have stoped\r\n");
    else if (res == 0)
        printf("device status : close\r\n");
    else if (res == 1)
        printf("device status : open\r\n");  
    else if (res == 2)
        printf("device status : run ok\r\n");  
    else if (res == 3)
        printf("device status : run err\r\n");  
    else 
        printf("device status : other err\r\n");  

}
