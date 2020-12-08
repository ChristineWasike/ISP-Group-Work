#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX_PEOPLE 10 
#define MAX_FLOORS 8

//globals
bool elevator_state; // false (stop), true(up)
int current_floor; 
int floor_numbers[MAX_PEOPLE]; // array for people's destination floors
int i; 
pthread_t person_tid[MAX_PEOPLE]; // thread ids for each person stored in an array
pthread_t elevator_tid; // thread id for elevetor
pthread_mutex_t lock;

// to sort floor numbers in ascending order
int cmpfunc(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

// elevator simulation
void * elevator() {

    pthread_mutex_lock(&lock);

    // to get random floor numbers
    time_t t;
    srand((unsigned) time(&t));

    // initializing elevator state to start at floor 1, current state is 'stop'
    elevator_state = false;
    current_floor = 1;

    // generating random destinations for people
    for (i = 0; i < MAX_PEOPLE; i++) {
        int temp;
        temp = rand() % (MAX_FLOORS - 1); // gets random numbers between 0 and 6
        floor_numbers[i] = temp + 2; // adjusts random nums to (2 - 8)
    }

    // sorting floor numbers to ascending order
    qsort(floor_numbers, 10, sizeof(int), cmpfunc);

    // elevator movement from floor 1 to 8
    while (current_floor <= MAX_FLOORS) {

        printf("\nThe elevator is at floor %d", current_floor);

        printf("\no-------o\n");
        printf("|   %d   |\n", current_floor);
        printf("o-------o\n\n");
        elevator_state = true; // start moving
        sleep(3); // simulate elevator movement

        for (i = 0; i < MAX_PEOPLE; i++) { // loop through people's floor numbers
            if (floor_numbers[i] == current_floor) { // if person's destination is reached
                elevator_state = false; // stop moving
                sleep(2); // simulate time taken for a person to get off the elevator
                printf("Person %d with thread id %ld has arrived.\n", i + 1, person_tid[i]); // offload passenger
            }
            elevator_state = true; // continue moving
        }
        current_floor++; // increment floor numbers
    }
    elevator_state = false; // when 8th floor is reached, end simulation
    printf("\nThe simulation has ended. Goodbye!\n");

    pthread_mutex_unlock(&lock);

    pthread_exit(NULL); // exit calling thread

}

int main() {
    int err;

    printf("\nGuidelines:\n");
    printf("\n1. The simulation is of 10 people using the lift to arrive at their floor of a 8-story building.");
    printf("\n2. For the simulation we will expect people to start at floor 1 and exit at their destination.");
    printf("\n3. When all people have arrived at their floor, the simulation ends.\n");

    printf("\nThe simulation has started!\n");

    if (pthread_mutex_init(&lock, NULL) != 0) { // initializing lock
        printf("\nMutex init has failed\n");
        return -1;
    }

    pthread_create(&elevator_tid, NULL, elevator, NULL); // creating elevator thread

    for (int i = 0; i < 10; i++) { 
        err = pthread_create(&person_tid[i], NULL, elevator, NULL); // creating person's threads
        if (err != 0)
            printf("\nCannot create thread: [%s]", strerror(err)); // check if thread creation is successful
    }

    for (int i = 0; i < 10; i++) { // wait for all person's thread to complete runing
        pthread_join(person_tid[i], NULL);
    }

    pthread_join(elevator_tid, NULL); // wait for elevator thread to complete runing

    pthread_mutex_destroy(&lock);

    pthread_exit(NULL); // exit calling thread

    return 0;
    
}