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

#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <wait.h>
//-------------------------------------------------------------------------------------
//   Common variable:
//-------------------------------------------------------------------------------------

#define PHILOSOPHER_NO 5
#define LEFT PHILOSOPHER_ID
#define RIGHT (PHILOSOPHER_ID + 1) % PHILOSOPHER_NO

int philosophers_pids[PHILOSOPHER_NO];        // Declare thread array
int philosophers_id;
static int SEMID;

void eat(int i);
void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void *philosopher();
void initializePhilosphers();
void childTermination(int i);

//-------------------------------------------------------------------------------------
//   main():
//-------------------------------------------------------------------------------------
int main(void) {
    initializePhilosphers();

    pid_t child_pid;
    int childCreated;
    for(childCreated = 0; childCreated < PHILOSOPHER_NO; childCreated++){
        child_pid = fork();                                                         //  Child creation

        if(child_pid == -1){                                                        // '-1' : return the parent-process, failed during creating child-process
            printf("\tFailed during creating child-process!\n");
            childTermination(childCreated);
            exit(1);
        }
        else if(child_pid == 0){                                                    // '0' : return the child-process
            philosophers_id = childCreated;
            philosophers_pids[childCreated] = getpid();
            philosopher(childCreated);
            exit(0);
        }
    }

    int status;
    for(childCreated = 0; childCreated < PHILOSOPHER_NO; childCreated++){
        child_pid = wait(&status);

        if(child_pid == -1) {                                                       // Expecting the child process id, wait until all child procces ends
            printf("\tNo more child procceses");
            break;
        }
        exit(0);
    }

    return 0;
}

//-------------------------------------------------------------------------------------
//   grab_forks():
//   sembuf:
//      sem_num - semaphore number
//		sem_op - semaphore operation:
//		sem_flg - operation flag
//
//      If sem_op is negative and sem_flag is 0, semop() will increment the semncnt associated with
//      the specified semaphore and suspend execution of the calling thread
//-------------------------------------------------------------------------------------
void grab_forks(int left_fork_id){
    int PHILOSOPHER_ID = left_fork_id;              // initialization for defined LEFT and RIGHT

    struct sembuf sem_operation[] = {               // function for toggling the semaphore, lock the semaphore
            { .sem_num = LEFT,  .sem_op = -1, .sem_flg = 0 },
            { .sem_num = RIGHT, .sem_op = -1, .sem_flg = 0 },
    };

    if(semop(SEMID, sem_operation, 2) == -1){       //  performs operations on the set of semaphores
        perror("grab_forks(): Failed during semop error!");
        exit(1);
    }
}
//-------------------------------------------------------------------------------------
//   eat():
//-------------------------------------------------------------------------------------
void eat(int i)
{
    int PHILOSOPHER_ID = i;                         // initialization for defined LEFT and RIGHT
    sleep(2);
    printf("Philosopher[%d] TAKES FORK %d and %d\n",
           RIGHT, LEFT, RIGHT);

    printf("Philosopher[%d] is EATING\n", RIGHT);
}

//-------------------------------------------------------------------------------------
//   put_away_forks():
//-------------------------------------------------------------------------------------
void put_away_forks( int left_fork_id ){
    int PHILOSOPHER_ID = left_fork_id;              // initialization for defined LEFT and RIGHT

    struct sembuf sem_operation[] = {               // function for toggling the semaphore, unlock the semaphore
            { .sem_num = LEFT,  .sem_op = 1, .sem_flg = 0 },
            { .sem_num = RIGHT, .sem_op = 1, .sem_flg = 0 },
    };

    if(semop(SEMID, sem_operation, 2) == -1){
        perror("put_away_forks(): Failed during semop error!");
        exit(1);
    }
    printf("Philosopher[%d] PUTS FORK [%d-%d] DOWN.\n", RIGHT, LEFT, RIGHT);
    printf("Philosopher[%d] is THINKING.\n", RIGHT);

}
/* -------------------------------------------------------------------------------------
    philosopher(void *num):
        Pseudocode:
        process P[i]
        while true do {
            THINK;
            PICKUP;
            EAT;
            PUTDOWN;
        }
------------------------------------------------------------------------------------- */
void *philosopher()
{
    //Set the ID of the current philosopher
    int PHILOSOPHER_ID = philosophers_id;

	while(1) {
        printf("Philosopher[%d] is THINKING\n", PHILOSOPHER_ID);
		sleep(1);
        grab_forks(PHILOSOPHER_ID);		            //Philosopher grabs forks
        eat(PHILOSOPHER_ID);
        sleep(1);	                                //Philosopher is eating
        put_away_forks(PHILOSOPHER_ID);	            //Philosopher puts forks down
	}

}

//-------------------------------------------------------------------------------------
//   initializePhilosphers():
//-------------------------------------------------------------------------------------
void initializePhilosphers(){

    SEMID = semget(IPC_PRIVATE, PHILOSOPHER_NO, 0666);              //  create new set semaphores with 0660 specifies explicit permissions
    if(SEMID < 0) {                                                 // checking if semaphores were created successfully, -1 is a failure
        perror("initializePhilosphers(): Error while getting the semaphores.\n");
        exit(1);
    }

    int i;
    for(i = 0; i < PHILOSOPHER_NO; i++){                            //  initialize all semaphores to 1 - all forks are free initially
        if(semctl(SEMID, i, SETVAL, 1) == -1){                      // semctl(int semid, int semnum, int cmd, ...); - Performs semaphore control operations.
            perror("initializePhilosphers(): Error while semctl!");
            exit(1);
        }
    }
}

//-------------------------------------------------------------------------------------
//   childTermination():
//      The already created child termiantion process:
//-------------------------------------------------------------------------------------
void childTermination(int i) {
    int j = 0;
    for(j = 0; j < i; j++) {
        kill(philosophers_pids[j], SIGTERM);                        // SIGTERM -  correct terminate a child processes
    }
}