#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* passengerOne(void* argument){
    int *passengerDestPointer = (int *)argument;
    printf("Passenger function. \n");

    for (int i = 1; i <= *passengerDestPointer; i++){
        printf("Passenger 1 is on floor %d.\n", i);
        if (i == *passengerDestPointer){
            printf("Passenger 1 has arrived at there destination, floor %d.\n", i);
        }
        
    }
    return NULL;
}

void passengerTwo(void* argument){
    int *passengerDestPointer = (int *)argument;
    printf("Passenger function. \n");

    for (int i = 1; i <= *passengerDestPointer; i++){
        printf("Passenger 2 is on floor %d.\n", i);
        if (i == *passengerDestPointer){
            printf("Passenger 2 has arrived at there destination, floor %d.\n", i);
        }
        
    }
}

void elevatorFunction(){
    printf("Elevator function. \n");
}

int main(int argc, char const *argv[]){
    pthread_t passengerThread;
    int floor_1 = 5;
    int floor_2 = 3;

    pthread_create(&passengerThread, NULL, passengerOne, &floor_1);
    passengerTwo(&floor_2);
    pthread_join(passengerThread, NULL);

    return 0;
}
