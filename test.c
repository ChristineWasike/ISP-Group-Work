#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_PEOPLE 10
#define MAX_FLOORS 8

//globals
int elevator_state; // -1 (stop), 0(up), 1(down)
int floor_number; // current floor
int floor_numbers[10] = {2, 2, 4, 5, 6, 7}; // array for people's destination floors
int i; 
pthread_t tid[10];

// to sort floor numbers
int cmpfunc(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

// elevator simulation
int elevator() {
    // int * floor_number = (int*) calloc(1, sizeof(int));

    // to get random floor numbers ***LOOK IT UP***
    time_t t;
    srand((unsigned) time(&t));

    // initializing elevator state to start at floor 1
    elevator_state = -1;
    floor_number = 1;

    // loading people - all starting at floor 1
    for (i = 0; i < MAX_PEOPLE; i++) {
        floor_numbers[i] = 1;
    }

    // generating random destinations for people
    for (i = 0; i < MAX_PEOPLE; i++) {
        int temp;
        temp = rand() % (MAX_FLOORS - 1); // gets random numbers between 0 and 6
        floor_numbers[i] = temp + 2; // adjusts random nums to (2 - 8)
    }

    // sorting floor numbers to ascending order
    qsort(floor_numbers, 10, sizeof(int), cmpfunc);

    // elevator movement
    while (floor_number <= MAX_FLOORS) {

        printf("\nThe elevator is at floor %d.\n", floor_number);
        elevator_state = 0; // start moving

        // [2, 2, 4, 5, 6, 7]
        // Possible use of threads
        for (i = 0; i < MAX_PEOPLE; i++) {
            if (floor_numbers[i] == floor_number) { // if person's destination is reached
                elevator_state = -1; // stop moving
                printf("Person %d has arrived.\n", i + 1); // offload passenger
                
            }
        }
        sleep(1);
        elevator_state = 0; // keep moving
        floor_number++;
    }

    elevator_state = -1; // if 8th floor is reached, end simulation
    printf("\nThe simulation has ended!\n");

}

int main() {

    // for (int i = 1; i <= 10; i++) {
    //     pthread_create(&tid[i], NULL, loadPassengers, NULL);
    // }
    // for (int i = 1; i <= 10; i++)
        // pthread_join(tid[i], NULL);

    elevator();
    
}