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
#include <stdbool.h>
#include <time.h>
#include "common.h"

int main(int argc,char* argv[]){
    struct my_shared_mem *shared_mem;
    srand(time(NULL));
    int stattime=rand()%9;
    int times=rand()%7;
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"-d")==0){
            times=atoi(argv[i+1]);
        }else if(strcmp(argv[i],"-t")==0){
            stattime=atoi(argv[i+1]);
        }else if(strcmp(argv[i],"-s")==0){
            shared_mem = shmat(atoi(argv[i+1]),NULL,0);
        }
    }
    sem_wait(&shared_mem->mutex);
    int timer=0;
    bool loop=shared_mem->status_controller;
    sem_post(&shared_mem->mutex);
    while(loop){
        sleep(1);   //metraei deyterolepta(eikoniko xronometro)
        sem_wait(&shared_mem->mutex);
        timer++;
        if(timer%times==0){
            printf("---------CURRENT DATA---------\n");
            printf("NUMBER OF BUSES IN PARKING AREA: %d\n",shared_mem->buses);
            printf("NUMBER OF PASSENGERS IN STATION BY CURRENT BUSES:%d\n",shared_mem->current_people_station);
            printf("EMPTY SEATS IN VOR'S MEDIAN:%d\n",shared_mem->park_VOR);
            printf("EMPTY SEATS IN ASK'S MEDIAN:%d\n",shared_mem->park_ASK);
            printf("EMPTY SEATS IN PEL'S MEDIAN:%d\n",shared_mem->park_PEL);
            printf("------------------------------\n");
        }
        if(timer%stattime==0){
            printf("---------STATISTICS---------\n");
            printf("AVERAGE TIME OF WAITING: %f \n",(float)shared_mem->sum_time/(float)(shared_mem->sum_of_buses));
            printf("AVERAGE WAITING TIME FOR VOR'S BUSES:%f\n",(float)shared_mem->time_VOR/(float)(shared_mem->VOR_buses));
            printf("AVERAGE WAITING TIME FOR ASK'S BUSES:%f\n",(float)shared_mem->time_ASK/(float)(shared_mem->ASK_buses));
            printf("AVERAGE WAITING TIME FOR PEL'S BUSES:%f\n",(float)shared_mem->time_PEL/(float)(shared_mem->PEL_buses));
            printf("NUMBER OF PASSENGERS ALREADY LAND ON THE STATION: %d\n",shared_mem->people_in_station);
            printf("NUMBER OF PASSENGERS ALREADY GET OFF THE STATION: %d\n",shared_mem->people_left);
            printf("TOTAL NUMBER OF BUSES:%d\n",shared_mem->sum_of_buses);
            printf("TOTAL NUMBER OF BUSES THAT HAVE ALREADY COMPLETE THE MISSION:%d\n",shared_mem->complete_buses);
            printf("----------------------------\n");
        }
        loop=shared_mem->status_controller;
        sem_post(&shared_mem->mutex);
    }
    shmdt(shared_mem);
}