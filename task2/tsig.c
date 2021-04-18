//-------------------------------------------------------------------------------------
//   Includes:
//-------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

//-------------------------------------------------------------------------------------
//   Common variable:
//-------------------------------------------------------------------------------------

#define NUM_CHILD 10
#define WITH_SIGNALS
#define INFINITE_LOOP for(;;)

void printLine();
void printHeader();

void childProcess();
void childTermination();

#ifdef WITH_SIGNALS
bool interupted = false;

void intHandler();
void termHandler();
#endif

//-------------------------------------------------------------------------------------
//   main():
//-------------------------------------------------------------------------------------

int main(){

    #ifdef WITH_SIGNALS
    
    for(int i = 1; i < _NSIG; i++)
        signal(i, SIG_IGN);
    
    signal(SIGCHLD, SIG_DFL);
    signal(SIGINT, intHandler);

    #endif

    pid_t pid;
    pid_t child[NUM_CHILD];

    printHeader();
    printf("\n\tParent PID: %d\n", getpid());


//-------------------------------------------------------------------------------------
//   The main (parent) process algorithm:
//      Three possibale cases:
//          - '0' : return the child-process 
//          - '-1' : return the parent-process, failed during creating child-process
//          - 'positive no' : return the parent-process, the pid is equal to child-process
//-------------------------------------------------------------------------------------

    int i =  0;
    for(i = 0; i < NUM_CHILD; ++i){
        switch (pid = fork())
        {
        case 0:
            childProcess();
            return 0;
        
        case -1:
            printf("\tFailed during creating child-process!\n");
            childTermination(i, child);
            return -1;

        default:
            child[i] = pid;
            if(!interupted){
                printf("\tParent[%d]: with new Child[%d]\n", getpid(), pid);
            } else {
                printf("\tFailed during creating child-process! - interupted\n");
                childTermination(i, child);
            }
        
        }

        sleep(1);
    }

    int status, num_zero = 0, num_one = 0;

    INFINITE_LOOP{

        pid = wait(&status);
        if(pid == -1) // Stopping loop  // wait() returns PID of the child
            break;                      // if -1 is returned error occurs -> we are in parent
        if(WIFEXITED(status) == true)
            printf("\tChild[%d]: Exit status: %d\n", pid, WEXITSTATUS(status));
        
        if(WEXITSTATUS(status) == 0)
            num_zero++;
        else
            num_one++;
    }

    printf("\n\tNo more children for Parent[%d].\n", getpid());
    printf("\tNumber of recieved 0 exit codes: %d\n", num_zero);
    printf("\tNumber of recieved 1 exit codes: %d\n\n", num_one);





	#ifdef WITH_SIGNALS
        for(int i=1; i<_NSIG; i++)
            signal(i, SIG_DFL);
    #endif

    return 0;
}

//-------------------------------------------------------------------------------------
//   The child process algorithm:
//-------------------------------------------------------------------------------------
void childProcess() {

    #ifdef WITH_SIGNALS
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, termHandler);
    #endif

    printf("\t[C] Parent[%d]: with new Child[%d]\n", getppid(), getpid());
    sleep(10);
    printf("\t[C] Execution completed, Child[%d]\n", getpid());

}
//-------------------------------------------------------------------------------------
//   The already created child termiantion process:
//-------------------------------------------------------------------------------------
void childTermination(int i, pid_t pids[]) {
    int j = 0;
    for(j = 0; j < i; j++) {
        kill(pids[j], SIGTERM);
        printf("\t[T] Termineted by SIGTERM, Child[%d]\n", getpid());
    }
}


#ifdef WITH_SIGNALS
void intHandler() {
    printf("\tParent[%d]: Interrupt triggered\n", getpid());
    interupted = true;
}


void termHandler() {
    printf("\tParent[%d]: Termineted by SIGTERM\n", getpid());
    interupted = true;
}
#endif




//-------------------------------------------------------------------------------------
//   Printing funcitons:
//-------------------------------------------------------------------------------------

void printLine() {
    printf("\n-------------------------------------------------------------------------------------");
}

void printHeader() {
    printLine();
    printf("\n\t\t\t [EOPSY] Processes and Signals ");
    printLine();
    
}