#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "common.h"

int main(int argc,char* argv[]){
    //printf("MANAGER\n");
    int value1,value2,type,value3,value4;
    struct my_shared_mem *shared_mem;
    if(strcmp(argv[1],"-s")==0){
        shared_mem = shmat(atoi(argv[2]),NULL,0); 
    }
    sem_wait(&shared_mem->mutex);
    bool loop=shared_mem->status;
    sem_post(&shared_mem->mutex);
    while(loop){
        sem_wait(&shared_mem->mutex);
        sem_getvalue(&shared_mem->enter,&value1);
        sem_getvalue(&shared_mem->exit,&value2);
        sem_getvalue(&shared_mem->mantime_enter,&value3);
        sem_getvalue(&shared_mem->mantime_exit,&value4);
        if((value1 +value3)==0){
            sem_post(&shared_mem->enter);
            sem_post(&shared_mem->mantime_enter);
        }
        if((value2+ value4)==0){
            sem_post(&shared_mem->exit);
            sem_post(&shared_mem->mantime_exit);
        }
        loop=shared_mem->status;
        sem_post(&shared_mem->mutex);
    }
    shmdt(shared_mem);
}
