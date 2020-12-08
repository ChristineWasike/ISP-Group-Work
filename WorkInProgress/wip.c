#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_PEOPLE 10 
#define MAX_FLOORS 8

//globals
int elevator_state; // -1 (stop), 0 (up), 1 (down)
int current_floor; 
int floor_numbers[MAX_PEOPLE]; // array for people's destination floors
int i, j, k; 
pthread_t person_tid[MAX_PEOPLE]; // thread ids for each person stored in an array
pthread_t elevator_tid; // thread id for elevetor
pthread_mutex_t lock;

// elevator simulation
void * elevator() {

    pthread_mutex_lock(&lock);

    // to get random floor numbers
    time_t t;
    srand((unsigned) time(&t));

    while (1) { // infinite loop

        if (k == 10 || k == 0) { // when either no person has started moving down or all people moving down have arrived, start moving up

            /**************************************************************/
            /* This section simulates elevator movement from the bottom to the top*/
            /**************************************************************/

            // initializing elevator state to start at floor 1, current state is 'stop'
            elevator_state = -1;
            current_floor = 1;

            // generating random destinations for people
            for (i = 0; i < MAX_PEOPLE; i++) {
                int temp;
                temp = rand() % (MAX_FLOORS - 1); // gets random numbers between 0 and 6
                floor_numbers[i] = temp + 2; // adjusts random nums to (2 - 8)
            }

            // elevator movement from floor 1 to top
            while (current_floor <= MAX_FLOORS) {
                
                printf("\nThe elevator is at Floor %d", current_floor);
                printf("\no-------o\n");
                printf("|   %d   |\n", current_floor);
                printf("o-------o\n\n");
                elevator_state = 0; // start moving up
                sleep(3); // simulate elevator movement

                for (i = 0; i < MAX_PEOPLE; i++) { // loop through people's floor numbers
                    if (floor_numbers[i] == current_floor) { // if person's destination is reached
                        elevator_state = -1; // stop moving
                        sleep(2); // simulate time taken for a person to get off the elevator
                        printf("Person %d with thread id %ld has arrived.\n", ++j, person_tid[i]); // offload passenger
                    }
                    elevator_state = 0; // continue moving up
                }
                current_floor++; // increment floor numbers
            }
            elevator_state = -1; // when floor with the passenger with the highest floor is reached, stop elevator
            printf("\nThe elevator simulation has reached the top! All passengers have arrived!\n");
            printf("Remember to press 'CTRL+C' to quit!\n");
        }

        else if (j == 10) { // only start moving down if the last person gets off the elevator
 
            /*********************************************************************/
            /* This section simulates elevator movement from top floor to bottom */
            /*********************************************************************/

            int k; // person identifier
            current_floor-=2; // we want to start from the next floor from the top

            // generating random destinations for people
            for (i = 0; i < MAX_PEOPLE; i++) {
                int temp;
                temp = rand() % (MAX_FLOORS - 1); // gets random numbers between 0 and 6
                floor_numbers[i] = temp + 1; // adjusts random nums to (1 - 7)
            }

            // elevator movement from top to bottom
            while (current_floor >= 1) {

                printf("\nThe elevator is at Floor %d", current_floor);
                printf("\no-------o\n");
                printf("|   %d   |\n", current_floor);
                printf("o-------o\n\n");
                elevator_state = 1; // start moving down
                sleep(3); // simulate elevator movement

                for (i = 9; i >= 0; i--) { // loop through people's floor numbers
                    if (floor_numbers[i] == current_floor) { // if person's destination is reached
                        elevator_state = -1; // stop moving
                        sleep(2); // simulate time taken for a person to get off the elevator
                        printf("Person %d with thread id %ld has arrived.\n", ++k, person_tid[i]); // offload passenger
                    }
                    elevator_state = 1; // continue moving down
                }
                current_floor--; // decrement floor numbers
            }
            elevator_state = -1; // when floor with the passenger with the lowest floor is reached, stop elevator
            printf("\nThe elevator simulation has reached the bottom!\n");
            printf("Remember to press 'CTRL+C' to quit!\n");
        }

        // reset person identifiers when all people going up/down have arrived
        else if (j == 10 && k == 10) {
            j = 0;
            k = 0;
        }
    }

    pthread_mutex_unlock(&lock);

    pthread_exit(NULL); // exit calling thread

}

int main() {
    int err;

    printf("\nGuidelines:\n");
    printf("\n1. The simulation is of 10 people using the lift to arrive at their floor of a 8-story building.");
    printf("\n2. For the simulation we will expect people to start at floor 1 and exit at their destination.");
    printf("\n3. Please press 'CTRL+C' to quit!!\n");

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