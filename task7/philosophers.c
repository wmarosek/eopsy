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
 *
 * Implement in the C language again the dining philosophers program. Now,
 * instead of usage of processes and semaphores, use threads and mutexes from
 * threads library.
 *
 * ------------------------------------------------------------------------------------- */

//-------------------------------------------------------------------------------------
//   Includes:
//-------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------
//   Common variable:
//-------------------------------------------------------------------------------------

#define PHILOSOPHER_NO 5
#define DINING_NO 3
#define LEFT PHILOSOPHER_ID
#define RIGHT (PHILOSOPHER_ID + 1) % PHILOSOPHER_NO

#define THINKING 0
#define HUNGRY 1
#define EATING 2


//-------------------------------------------------------------------------------------
//   Mutexes and shared variables:
//-------------------------------------------------------------------------------------
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s[PHILOSOPHER_NO] = PTHREAD_MUTEX_INITIALIZER;
int state[PHILOSOPHER_NO];

//-------------------------------------------------------------------------------------
//   Definition of function:
//-------------------------------------------------------------------------------------
void eat(int i, int meal);
void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void initializePhilosphers();
void* philospher(void* num);
void test(int i);

//-------------------------------------------------------------------------------------
//   main():
//-------------------------------------------------------------------------------------
int main(){
    initializePhilosphers();

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
void *philosopher(void *num) {
    //Set the ID of the current philosopher
    int PHILOSOPHER_ID = *(int *) num;

    for (int i = 0; i < DINING_NO; i++) {
            printf("Philosopher[%d] is THINKING\n", PHILOSOPHER_ID);
            sleep(1);
            grab_forks(PHILOSOPHER_ID);                    //Philosopher grabs forks
            eat(PHILOSOPHER_ID, i+1);                            //Philosopher is eating
            put_away_forks(PHILOSOPHER_ID);                //Philosopher puts forks down
        }

        return NULL;

}

//-------------------------------------------------------------------------------------
//   initializePhilosphers():
//-------------------------------------------------------------------------------------
void initializePhilosphers(){
    int i;
    // Init mutexes - no eating
    for (i = 0; i < PHILOSOPHER_NO; i++)
    {
        pthread_mutex_lock(s + i);
    }

    pthread_t threadIds[PHILOSOPHER_NO];        // Declare thread array
    int philosophersIds[PHILOSOPHER_NO];
    // Thread Creation
    for (i = 0; i < PHILOSOPHER_NO; i++)
    {
        philosophersIds[i] = i;
        pthread_create(&threadIds[i], NULL, philosopher, &philosophersIds[i]);
    }

    sleep(30);

    // Wait equivalent
    // Join all threads
    for (i = 0; i < PHILOSOPHER_NO; i++)
    {
        pthread_join(threadIds[i], NULL);
    }

//    pthread_mutex_destroy(&m);

    for (i = 0; i < PHILOSOPHER_NO; i++)
    {
        pthread_mutex_destroy(&threadIds[i]);
    }

    exit(0);

}

void grab_forks(int left_fork_id){
    int PHILOSOPHER_ID = left_fork_id;

    pthread_mutex_lock(&m); //   Lock mutex, Begin Critical Section
    state[PHILOSOPHER_ID] = HUNGRY;

    printf("Philosopher[%d] TRY TO %d GRAB FORK\n",PHILOSOPHER_ID, PHILOSOPHER_ID);
    test(PHILOSOPHER_ID);

    pthread_mutex_unlock(&m);   // Unlock mutex, end of ciritical section

    pthread_mutex_lock(&s[PHILOSOPHER_ID]); //take forks

    sleep(1);
}

void test(int i){
    int PHILOSOPHER_ID = i;

    if( state[PHILOSOPHER_ID] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING )
    {
        state[PHILOSOPHER_ID] = EATING;
        pthread_mutex_unlock(&s[PHILOSOPHER_ID]);
    }
}

void put_away_forks(int i)
{
    int PHILOSOPHER_ID = i;

    pthread_mutex_lock(&m);             //Begin Critical Section
    state[PHILOSOPHER_ID] = THINKING;  //Set state to the philosopher to THINKING

    printf("Philosopher[%d] PUTS FORK [%d-%d] DOWN.\n", RIGHT, LEFT, RIGHT);
    printf("Philosopher[%d] is THINKING.\n", RIGHT);

    test(LEFT);		                    //Test eating conditions for the left philosopher
    test(RIGHT);	                    //Test eating conditions for the right philosopher
    pthread_mutex_unlock(&m); 		    //End Critical Section


}

//-------------------------------------------------------------------------------------
//   eat():
//-------------------------------------------------------------------------------------
void eat(int i, int meal)
{
    int PHILOSOPHER_ID = i;                         // initialization for defined LEFT and RIGHT
    sleep(2);
    printf("Philosopher[%d] TAKES FORK %d and %d\n",
           RIGHT, RIGHT, LEFT);

    printf("Philosopher[%d] is EATING [%d] meal\n", RIGHT, meal);
}