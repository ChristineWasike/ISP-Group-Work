#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_PEOPLE 10 
#define MAX_FLOORS 8

//globals
int elevator_state; // -1 (stop), 0 (up), 1 (down)
int i, j, current_floor, current_people, waiting_list; 
int floor_numbers[MAX_PEOPLE]; // array for people's destination floors
int new_floor_number1, new_floor_number2, new_floor_numbers[MAX_PEOPLE]; // array for the destination floors of new people who are boarding the elevator
pthread_t person_tid[MAX_PEOPLE]; // thread ids for each person stored in an array
pthread_t elevator_tid; // thread id for elevetor
pthread_mutex_t lock;

// elevator simulation
void * elevator() {

    pthread_mutex_lock(&lock);

    // to get random floor numbers
    time_t t;
    srand((unsigned) time(&t));

    /**********************************************************************/
    /* This section simulates elevator movement from the bottom to the top*/
    /**********************************************************************/

    // initializing elevator state to start at floor 1, current state is 'stop'
    elevator_state = -1;
    current_floor = 1;
    current_people = 10;

    // generating random destinations for people
    for (i = 0; i < MAX_PEOPLE; i++) {
        int temp;
        temp = rand() % (MAX_FLOORS - 1); // gets random numbers between 0 and 6
        floor_numbers[i] = temp + 2; // adjusts random nums to (2 - 8)
    }

    // elevator movement from floor 1 to top
    while (current_floor <= MAX_FLOORS) {
        
        printf("\no------o\n");
        printf("|  F%d  |\n", current_floor);
        printf("o------o\n\n");
        elevator_state = 0; // start moving up
        sleep(1); // to simulate elevator movement

        printf("The number of people in the elevator is %d\n\n", current_people);

        for (i = 0; i < MAX_PEOPLE; i++) { // loop through people's floor numbers
            if (floor_numbers[i] == current_floor) { // if person's destination is reached
                elevator_state = -1; // stop moving
                printf("  => Person %d with thread id %ld has arrived.\n", ++j, person_tid[i]); // offload passenger
                sleep(2); // simulate time taken for a person to get off the elevator
                current_people -= 1;

                int temp2; // generate random destination floor for new person who now has space to board the elevator
                temp2 = rand() % (MAX_FLOORS); // gets random numbers between 0 and 7
                new_floor_number1 = temp2 + 1; // adjust random numbers to between 1 and 8

                if (new_floor_number1 > current_floor) { // check if generated floor number is greater than the current floor to determine if the person can board or wait for lift to come down
                    floor_numbers[i] = new_floor_number1; // update our floor numbers list with this person's floor number so that they can be taken to their destination
                    printf("\nJust a minute! We are picking up a new passenger heading to Floor %d!\n\n", new_floor_number1); // status update
                    sleep(2); // simulate time taken for a person to get on the elevator
                    current_people += 1;
                }
                else if (new_floor_number1 < current_floor) { // this is to exclude passengers who are already on their floors!
                    new_floor_numbers[i] = new_floor_number1; // if passenger's new floor is less than the current floor, he has to wait for the elevator to come down
                    waiting_list+=1;
                }
            }
            elevator_state = 0; // continue moving up
        }
        current_floor++; // increment floor numbers
    }

    elevator_state = -1; // when floor with the passenger with the highest floor is reached, stop elevator
    printf("\nThe elevator simulation has reached the top! We are now moving down!\n");

    /*********************************************************************************************/
    /* This section simulates elevator movement from top floor to bottom */
    /* The elevator is now going to drop off people who were waiting for it to go to lower floors*/
    /*********************************************************************************************/

    current_floor-=2; // we want to start from the next floor from the top

    // elevator movement from top to bottom
    while (current_floor >= 1) {

        printf("\no------o\n");
        printf("|  F%d  |\n", current_floor);
        printf("o------o\n\n");
        elevator_state = 1; // start moving down
        sleep(1); // simulate elevator movement

        printf("The number of people in the elevator is %d\n\n", waiting_list);

        for (i = MAX_PEOPLE; i > 0; i--) { // loop through people's floor numbers
            if (new_floor_numbers[i] == current_floor) { // if person's destination is reached
                elevator_state = -1; // stop moving
                printf("  => Person %d with thread id %ld has arrived.\n", ++j, person_tid[i]); // offload passenger
                sleep(2); // simulate time taken for a person to get off the elevator
                waiting_list -= 1;

                int temp3; // generate random destination floor for new person who now has space to board the elevator
                temp3 = rand() % (MAX_FLOORS); // gets random numbers between 0 and 7
                new_floor_number2 = temp3 + 1; // adjust random numbers to between 1 and 8

                if (new_floor_number2 < current_floor) { // check if generated floor number is lesser than the current floor to determine if the person can board the lift
                    new_floor_numbers[i] = new_floor_number2; // update our floor numbers list with this person's floor number so that they can be taken to their destination
                    printf("\nJust a minute! We are picking up a new passenger heading to Floor %d!\n\n", new_floor_number2); // status update
                    sleep(2); // simulate time taken for a person to get on the elevator
                    waiting_list += 1;
                }
            }
            elevator_state = 1; // continue moving down
        }
        current_floor--; // decrement floor numbers
    }

    elevator_state = -1; // when floor with the passenger with the lowest floor is reached, stop elevator
    printf("\nThe elevator simulation has reached the bottom! All people have arrived!\n");
    printf("\nRemember to press 'CTRL+C' to quit!!\n");

    pthread_mutex_unlock(&lock);

    pthread_exit(NULL); // exit calling thread

}

int main() {
    int err;

    printf("\nGuidelines:\n");
    printf("\n1. The simulation is of a maximum of 10 people using the lift to arrive at their floor of a 8-story building.");
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