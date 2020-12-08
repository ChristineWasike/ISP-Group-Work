#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_PEOPLE 10
#define MAX_FLOORS 8

//globals
int elevator_state;            // -1 (stop), 0(up), 1(down)
int no_of_floors;              // current floor
int floor_numbers[MAX_PEOPLE]; // array for people's destination floors
int i;
pthread_t person_tid[MAX_PEOPLE];
pthread_t elevator_tid;
pthread_mutex_t lock;

// to sort floor numbers
int cmpfunc(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}

// elevator simulation
void *elevator(){

    pthread_mutex_lock(&lock);

    // to get random floor numbers
    time_t t;
    srand((unsigned)time(&t));

    // initializing elevator state to start at floor 1
    elevator_state = -1;
    no_of_floors = 1;

    // generating random destinations for people
    for (i = 0; i < MAX_PEOPLE; i++){
        int temp;
        temp = rand() % (MAX_FLOORS - 1); // gets random numbers between 0 and 6
        floor_numbers[i] = temp + 2;      // adjusts random nums to (2 - 8)
    }

    // sorting floor numbers to ascending order
    qsort(floor_numbers, 10, sizeof(int), cmpfunc);

    // elevator movement
    while (no_of_floors <= MAX_FLOORS){

        printf("\nThe elevator is at floor %d.\n", no_of_floors);
        elevator_state = 0; // start moving

        for (i = 0; i < MAX_PEOPLE; i++)
        {
            if (floor_numbers[i] == no_of_floors)
            {                        // if person's destination is reached
                elevator_state = -1; // stop moving
                sleep(1);
                printf("Person %d with thread id %ld has arrived.\n", i + 1, person_tid[i]); // offload passenger
                // pthread_exit(&person_tid[i]);
            }
        }
        elevator_state = 0; // keep moving
        no_of_floors++;
    }
    elevator_state = -1; // if 8th floor is reached, end simulation
    printf("\nThe simulation has ended!\n");

    pthread_exit(0);
    int value = pthread_mutex_unlock(&lock);
    printf("Unlock done: %d\n", value);
    
}

int main(){
    int err;

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\nMutex init has failed\n");
        return -1;
    }

    // elevator();
    pthread_create(&elevator_tid, NULL, elevator, NULL);


    for (int i = 0; i < 10; i++){
        err = pthread_create(&person_tid[i], NULL, elevator, NULL);
        if (err != 0)
            printf("\nCannot create thread: [%s]", strerror(err));
    }

    for (int i = 0; i < 10; i++){
        pthread_join(person_tid[i], NULL);
    }
    pthread_join(elevator_tid, NULL);
    
    pthread_mutex_destroy(&lock);
    pthread_exit(0);
    return 0;
}