/* -------------------------------------------------------------------------------------
 * The Dining Philosophers problem is a classic multi-process synchronization
 * problem. The problem consists of five philosophers sitting at a table who do
 * nothing but think and eat. Between each philosopher, there is a single fork
 * In order to eat, a philosopher must have both forks. A problem can arise if
 * each philosopher grabs the fork on the right, then waits for the fork on the
 * left. In this case a deadlock has occurred, and all philosophers will starve.
 * Also, the philosophers should be fair. Each philosopher should be able to eat
 * as much as the rest.
 *
 * The Dining Philosophers
 * ------------------------------------------------------------------------------------- */

//-------------------------------------------------------------------------------------
//   Includes:
//-------------------------------------------------------------------------------------
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------
//   Common variable:
//-------------------------------------------------------------------------------------

#define philosopherNo 5
#define LEFT (PHILOSOPHER_ID + philosopherNo - 1) % philosopherNo
#define RIGHT (PHILOSOPHER_ID + 1) % philosopherNo

#define THINKING 2
#define HUNGRY 1
#define EATING 0

//-------------------------------------------------------------------------------------
//   Definition of function:
//-------------------------------------------------------------------------------------

sem_t table;
sem_t chopstick[philosopherNo];             // Binary Semaphores to keep track of Philosopher

pthread_t threadIds[philosopherNo];        // Declare thread array
int philosophersIds[philosopherNo];
int philosophersState[philosopherNo];


void eat(int i);
void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void initializePhilosphers();
void initializeThreads();
void* philospher(void* num);


//-------------------------------------------------------------------------------------
//   main():
//-------------------------------------------------------------------------------------
int main(){
    initializePhilosphers();
    initializeThreads();

    return 0;
}
/* -------------------------------------------------------------------------------------
    Pseudocode:
    process P[i]
    while true do {
        THINK;
        PICKUP(CHOPSTICK[i], CHOPSTICK[i+1 mod 5]);
        EAT;
        PUTDOWN(CHOPSTICK[i], CHOPSTICK[i+1 mod 5])
    }
------------------------------------------------------------------------------------- */
void *philosopher(void *num)
{
    //Set the ID of the current philosopher
    int PHILOSOPHER_ID = *(int *)num;

    philosophersState[PHILOSOPHER_ID] = THINKING;
    printf("Philosopher[%d] is THINKING\n", PHILOSOPHER_ID);

    while(1)
    {
        sleep(1);
        grab_forks(PHILOSOPHER_ID);		            //Philosopher grabs forks
        sleep(1);	            //Philosopher is eating
        put_away_forks(PHILOSOPHER_ID);	            //Philosopher puts forks down
    }
}

//-------------------------------------------------------------------------------------
//   initializePhilosphers():
//-------------------------------------------------------------------------------------
void initializePhilosphers(){
    int i;

    for(i = 0; i < philosopherNo; i++){
        philosophersIds[i] = i;                             // setting ids
        printf("Philosopher[%d] is sitting around the table...\n", i);

    }

    sem_init(&table, 0, 1);

    for(i = 0; i < philosopherNo; i++){
        sem_init(&chopstick[i], 0, 0);         // setting the state of all Chopsticks as 0
    }
}

//-------------------------------------------------------------------------------------
//   initializeThreads():
//-------------------------------------------------------------------------------------
void initializeThreads(){
    int i;

    // Thread Creation
    for (i = 0; i < philosopherNo; i++)
    {
        pthread_create(&threadIds[i], NULL, philosopher, &philosophersIds[i]);
    }

    // Wait equivalent
    // Join all threads
    for (i = 0; i < philosopherNo; i++)
    {
        pthread_join(threadIds[i], NULL);
    }
}



void eat(int i)
{
    int PHILOSOPHER_ID = i;

    if (philosophersState[PHILOSOPHER_ID] == HUNGRY && philosophersState[LEFT] != EATING && philosophersState[RIGHT] != EATING) {
        philosophersState[PHILOSOPHER_ID] = EATING;                      // state that eating

        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n",
               PHILOSOPHER_ID + 1, LEFT + 1, PHILOSOPHER_ID + 1);

        printf("Philosopher %d is Eating\n", PHILOSOPHER_ID + 1);

        // sem_post(&chopstick[PHILOSOPHER_ID]) has no effect
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&chopstick[PHILOSOPHER_ID]);
    }
}

void grab_forks(int left_fork_id){
    int PHILOSOPHER_ID = left_fork_id;

    sem_wait(&table);                                           // critical section
    philosophersState[PHILOSOPHER_ID] = HUNGRY;
    printf("\nPhilosopher %d grab fork",PHILOSOPHER_ID);

    eat(PHILOSOPHER_ID);

    sem_post(&table);
    sem_wait(&chopstick[PHILOSOPHER_ID]);
    sleep(1);
}

void put_away_forks(int i)
{
    int PHILOSOPHER_ID = i;

    sem_wait(&table); //Begin Critical Section
    philosophersState[PHILOSOPHER_ID] = THINKING; //Set state to the philosopher to THINKING

    printf("Philosopher[%d] puts fork [%d-%d] down.\n", PHILOSOPHER_ID, LEFT, PHILOSOPHER_ID);
    printf("Philosopher[%d] is THINKING.\n", PHILOSOPHER_ID);

    eat(LEFT);		//Test eating conditions for the left philosopher
    eat(RIGHT);	//Test eating conditions for the right philosopher
    sem_post(&table); 		//End Critical Section
}


