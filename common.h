#include<semaphore.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

struct my_shared_mem {
    sem_t enter;    //check if a bus is on the entry
    sem_t exit;     //chech for exit
    sem_t mutex;    //mutex to check shared memory 
    sem_t mantime_enter;    //mantime for enter
    sem_t mantime_exit;     //mantime for exit
    bool status;    //to terminated station manager
    bool status_controller; //to terminated controller
    bool file_checker;      //bool checker to know if we have to write or append in file

    int buses;  //how many buses in parking area
    int sum_of_buses;   //all buses
    long int time_VOR;  //time of waiting for vor's
    long int time_ASK;  //time of waiting for ask's
    long int time_PEL;  //time of waiting for pel's
    int VOR_buses;      //number of buses vor's
    int ASK_buses;  //number of buses ask's
    int PEL_buses;      //number of buses pel's
    long int sum_time;  //how many time
    int park_VOR;       //num of VOR's seats
    int park_ASK;       //num of ASK's seats
    int park_PEL;       //num of PEL's seats
    int people_in_station;  //people went to station
    int people_left;        //people got off the station 
    int current_people_station;    //people get off the current buses ringth know
    int complete_buses; //buses that are served in the total
};

void itoa(int n, char s[]);
void reverse(char s[]);