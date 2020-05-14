#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include "common.h"


int main(int argc,char* argv[]){
    srand(time(NULL));
    int num_pas,type,capacity,parkperiod;
    int shmID,pid,cpid,value1,value2;
    FILE* fp;
    char input[60];
    struct my_shared_mem *shared_mem;
    shmID  = shmget(IPC_PRIVATE, sizeof(*shared_mem), IPC_CREAT | 0666);
    if (shmID == -1) {
        perror("Shared memory");
        return 1;
    }
    printf("ALLOCATED MEMORY ID:%d\n\n",shmID);
    shared_mem = shmat(shmID,NULL,0);
    if (shared_mem == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }
    int island=1;
    shared_mem->park_VOR=island*3;
    shared_mem->park_ASK=island*3;
    shared_mem->park_PEL=island*3;
    shared_mem->buses=0;
    shared_mem->sum_time=0;
    shared_mem->status=1;
    shared_mem->sum_of_buses=0;
    shared_mem->time_ASK=0;
    shared_mem->current_people_station=0;
    shared_mem->time_PEL=0;
    shared_mem->time_VOR=0;
    shared_mem->PEL_buses=0;
    shared_mem->VOR_buses=0;
    shared_mem->ASK_buses=0;
    shared_mem->people_in_station=0;
    shared_mem->status_controller=1;
    shared_mem->complete_buses=0;
    shared_mem->file_checker=0;
    sem_init(&shared_mem->enter, 1, 0);
    sem_init(&shared_mem->exit, 1, 0);
    sem_init(&shared_mem->mantime_enter, 1, 0);
    sem_init(&shared_mem->mantime_exit, 1, 0);
    sem_init(&shared_mem->mutex, 1, 1);
    int conf[5];
    if(argc==3){
        if(strcmp(argv[1],"-l")==0){
            fp=fopen(argv[2],"r");
            if (fp==NULL) {
                printf("Cannot open file\n");
                return 1;
            }
            int k=0;
            for(int i=0;i<9;i++){
                fgets(input,60,fp);
                conf[k]=input[0]-48;
                k++;
            }
            int island=conf[5];
            shared_mem->park_VOR=island*conf[6];
            shared_mem->park_ASK=island*conf[7];
            shared_mem->park_PEL=island*conf[8];
            parkperiod=conf[1];
            int mantime=conf[2];
            int time=conf[3];
            int stattime=conf[4];
            pid_t st=fork();
            if(st==0){
                int pid1=fork();
                if(pid1==0){
                    char* args1[4];
                    args1[0]="./s_manager";
                    args1[1]="-s";
                    char* a2=malloc(20*sizeof(char));
                    itoa((int)shmID,a2);
                    args1[2]=a2;
                    args1[3]=NULL;
                    execvp(args1[0],args1);
                }else{
                    char* args1[8];
                    args1[0]="./comptroller";
                    args1[1]="-d";
                    char* num=malloc(3*sizeof(char));
                    itoa(time,num);
                    args1[2]=num;
                    args1[3]="-t";
                    char* num1=malloc(3*sizeof(char));
                    itoa(stattime,num1);
                    args1[4]=num1;
                    args1[5]="-s";
                    char* a2=malloc(20*sizeof(char));
                    itoa((int)shmID,a2);
                    args1[6]=a2;
                    args1[7]=NULL;
                    execvp(args1[0],args1);
                }
            }else{
                sleep(1);
                for(int i=0;i<conf[0];i++){
                    pid=fork();
                    type=rand()%3;
                    char* tp=malloc(4*sizeof(char));
                    if(type==0){
                        strcpy(tp,"VOR");
                    }else if(type==1){
                        strcpy(tp,"ASK");
                    }else{
                        strcpy(tp,"PEL");
                    }
                    num_pas=rand()%60;
                    capacity=rand()%60;
                    if(pid==0){
                        cpid=pid;
                        char* args[14];
                        args[0]="./bus";
                        args[1]="-t";
                        args[2]=tp;
                        args[3]="-n";
                        char* num=malloc(3*sizeof(char));
                        itoa(num_pas,num);
                        args[4]=num;
                        args[5]="-c";
                        char* num1=malloc(3*sizeof(char));
                        itoa(capacity,num1);
                        args[6]=num1;
                        args[7]="-p";
                        char* num2=malloc(3*sizeof(char));
                        itoa(parkperiod,num2);
                        args[8]=num2;
                        args[9]="-m";
                        char* num3=malloc(3*sizeof(char));
                        itoa(mantime,num3);
                        args[10]=num3;
                        args[11]="-s";
                        char* a1=malloc(20*sizeof(char));
                        itoa((int)shmID,a1);
                        args[12]=a1;
                        args[13]=NULL;
                        execvp(args[0],args);
                    }
                }
            }   
            getchar();
            shared_mem->status=0;
            shared_mem->status_controller=0;
            sem_destroy(&shared_mem->enter);
            sem_destroy(&shared_mem->exit);
            if (shmdt(shared_mem) == -1) {
                perror("shmdt");
                return 1;
            }
            if (shmctl(shmID, IPC_RMID, 0) == -1) {
                perror("shmctl");
                return 1;
            }
            fclose(fp);
        }
    }else if(argc==1){
        getchar();
        shared_mem->status=0;
        shared_mem->status_controller=0;
        sem_destroy(&shared_mem->enter);
        sem_destroy(&shared_mem->exit);
        if (shmdt(shared_mem) == -1) {
            perror("shmdt");
            return 1;
        }
        if (shmctl(shmID, IPC_RMID, 0) == -1) {
            perror("shmctl");
            return 1;
        }
        fclose(fp);
    }
    
}
