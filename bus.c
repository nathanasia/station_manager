#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/time.h>
#include<semaphore.h>
#include <time.h>
#include"common.h"
//./bus -t type -n incpassengers -c capacity -p parkperiod -m mantime -s shmid
int main(int argc,char* argv[]){
    srand(time(NULL));
    char* type=malloc(4*sizeof(char));
    struct timeval stop, start;
    struct my_shared_mem *shared_mem;
    int bus_type,tp,num_pas,capacity,mantime,parkperiod;
    num_pas=rand()%60;
    capacity=rand()%60;
    mantime=rand()%3;
    parkperiod=rand()%4;
    for(int k=0;k<argc;k++){
        if(strcmp(argv[k],"-s")==0){
            shared_mem = shmat(atoi(argv[k+1]),NULL,0); 
        }else if(strcmp(argv[k],"-t")==0){
            strcpy(type,argv[k+1]);
        }else if(strcmp(argv[k],"-n")==0){
            num_pas=atoi(argv[k+1]);
        }else if(strcmp(argv[k],"-c")==0){
            capacity=atoi(argv[k+1]);
        }else if(strcmp(argv[k],"-p")==0){
            parkperiod=atoi(argv[k+1]);
        }else if(strcmp(argv[k],"-m")==0){
            mantime=atoi(argv[k+1]);
        }
    }
    if(strcmp(type,"VOR")==0){
        bus_type=0;
    }else if(strcmp(type,"ASK")==0){
        bus_type=1;
    }else if(strcmp(type,"PEL")==0){
        bus_type=2;
    }
    FILE *fo;
    if(shared_mem->file_checker==0){
        fo=fopen("public_ledger.txt","w");
        if(fo==NULL){
            perror("Problem to open file!\n");
            return 1;
        }
        shared_mem->file_checker=1;
    }else{
        fo=fopen("public_ledger.txt","a");
        if(fo==NULL){
            perror("Problem to open file!\n");
            return 1;
        }
    }
    sem_wait(&shared_mem->mutex);
    fprintf(fo,"Bus with id=%d,type=%s ask permission to enter\n",getpid(),type);
    fclose(fo);
    sem_post(&shared_mem->mutex);
    sem_wait(&shared_mem->enter);
    sem_wait(&shared_mem->mutex);
    fo=fopen("public_ledger.txt","a");
    if(fo==NULL){
        perror("Problem to open file!\n");
        return 1;
    }
    fprintf(fo,"Bus with id=%d and type=%s is on the way to the parking area\n",getpid(),type);
    fclose(fo);
    sem_post(&shared_mem->mutex);
    sleep(mantime);
    sem_wait(&shared_mem->mantime_enter);
    gettimeofday(&start, NULL);
    sem_wait(&shared_mem->mutex);
    shared_mem->sum_of_buses++;
    if(bus_type==0){
        shared_mem->VOR_buses++;
        if(shared_mem->park_VOR>0){
            shared_mem->park_VOR--;
            tp=0;
        }else if(shared_mem->park_PEL>0){
            shared_mem->park_PEL--;
            tp=2;
        }
    }
    if(bus_type==1){
        shared_mem->ASK_buses++;
        if(shared_mem->park_ASK>0){
            shared_mem->park_ASK--;
            tp=1;
        }else if(shared_mem->park_PEL>0){
            shared_mem->park_PEL--;
            tp=2;
        }
    }
    if(bus_type==2){
        shared_mem->PEL_buses++;
        if(shared_mem->park_PEL>0){
            shared_mem->park_PEL--;
            tp=2;
        }
    }
    fo=fopen("public_ledger.txt","a");
    if(fo==NULL){
        perror("Problem to open file!\n");
        return 1;
    }
    if(tp==0){
        fprintf(fo,"Bus with id=%d and type=%s finally is parked in a median VOR's type\n",getpid(),type);
    }else if(tp==1){
        fprintf(fo,"Bus with id=%d and type=%s finally is parked in a median ASK's type\n",getpid(),type);
    }else{
        fprintf(fo,"Bus with id=%d and type=%s finally is parked in a median PEL's type\n",getpid(),type);
    }
    fclose(fo);
    shared_mem->buses ++;
    int in_people=rand()%capacity;
    shared_mem->people_left=shared_mem->people_left+ in_people;
    fo=fopen("public_ledger.txt","a");
    if(fo==NULL){
        perror("Problem to open file!\n");
        return 1;
    }
    fprintf(fo,"Bus with id=%d and type=%s %d passengers get in this bus\n",getpid(),type,in_people);
    fclose(fo);
    shared_mem->people_in_station=shared_mem->people_in_station+num_pas;
    shared_mem->current_people_station=shared_mem->current_people_station+num_pas;
    sem_post(&shared_mem->mutex);
    sleep(parkperiod);
    fo=fopen("public_ledger.txt","a");
    if(fo==NULL){
        perror("Problem to open file!\n");
        return 1;
    }
    fprintf(fo,"Bus with id=%d,type=%s ask permission to exit\n",getpid(),type);
    fclose(fo);
    sem_wait(&shared_mem->exit);
    gettimeofday(&stop, NULL);
    long int t=(stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec ;
    sem_wait(&shared_mem->mutex);
    shared_mem->sum_time=shared_mem->sum_time +t;
    if(tp==0){
        shared_mem->time_VOR=shared_mem->time_VOR + t;;
    }else if(tp==1){
        shared_mem->time_ASK=shared_mem->time_ASK + t;
    }else if(tp==2){
        shared_mem->time_PEL=shared_mem->time_PEL + t;
    }
    sem_post(&shared_mem->mutex);
    fo=fopen("public_ledger.txt","a");
    if(fo==NULL){
        perror("Problem to open file!\n");
        return 1;
    }
    fprintf(fo,"Bus with id=%d and type=%s is on the way to exit\n",getpid(),type);
    fclose(fo);
    sem_post(&shared_mem->mutex);
    sleep(mantime);
    sem_wait(&shared_mem->mutex);
    fo=fopen("public_ledger.txt","a");
    if(fo==NULL){
        perror("Problem to open file!\n");
        return 1;
    }
    sem_wait(&shared_mem->mantime_exit);
    fprintf(fo,"Bus with id=%d,type=%s finally is out of bus station\n",getpid(),type);
    fclose(fo);
    sem_post(&shared_mem->mutex);
    //printf("EXIT BUS ID=%d \n",getpid());
    shared_mem->complete_buses++;
    shared_mem->current_people_station=shared_mem->people_in_station-num_pas;
    sem_wait(&shared_mem->mutex);
    if(tp==0){
        shared_mem->park_VOR++;
    }else if(tp==1){
        shared_mem->park_ASK++;
    }else if(tp==2){
        shared_mem->park_PEL++;
    }
    shared_mem->buses --;
    sem_post(&shared_mem->mutex);
    shmdt(shared_mem);
}