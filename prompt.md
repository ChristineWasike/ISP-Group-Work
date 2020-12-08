## Question Two
Write a program in C to simulate an elevator.

### Guidelines:
The simulation is of people using the lift to arrive at their floor of a 8-story building. For the simulation we will expect people to start at floor 1 and exit at their destination. When all people have arrived at their floor, the simulation ends.

#### Passenger:
1) 10 people will use the elevator (1 thread per person created at the start of simulation).
2) Each person begins at floor 1 waiting for an elevator.
3) Each person randomly picks a floor from 2 to 8.
4) A person will wait for an elevator to arrive.
5) A person will enter into the elevator only if there is room.
6) A person will only board the elevator going to their floor. ** 
7) Once at the destination floor, the person exits the elevator.

#### Elevator:
1) There is only 1 elevator (1 thread for this elevator).
2) Elevator can only hold 10 people.
3) The elevator starts on floor 1.
4) Elevator thread sleep 1 second per floor to simulate travel time.

#### Other Guidelines:
1) Each activity of each thread should be printed to the screen.


// The Logic
- Add hyphens or (=====) to illustrate the floors
