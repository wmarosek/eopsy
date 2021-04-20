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

#define NUM_CHILD 15
// #define WITH_SIGNALS

//-------------------------------------------------------------------------------------
//   Definition of funcion:
//-------------------------------------------------------------------------------------
void childProcess();
void childTermination();

#ifdef WITH_SIGNALS
bool interupted = false;                            //  global variable which will notify about the fact of  keyboard interrupt occurance  

void intHandler();
void termHandler();
#endif

void printLine();
void printHeader();

//-------------------------------------------------------------------------------------
//   main():
//-------------------------------------------------------------------------------------

int main(){

    
    #ifdef WITH_SIGNALS                             // Force ignoring of all signals with the signal() and restore the default handler for SIGCHLD signal

        for(int i = 1; i < _NSIG; i++)              // Ignore all singals
            signal(i, SIG_IGN);
        
        signal(SIGCHLD, SIG_DFL);                   // Default signal handling - SIGCHLD inform that child creation process ended
        signal(SIGINT, intHandler);                 // Set interaption signal handling | ctrl+c

    #endif

    printHeader();
    printf("\tParent PID: %d\n", getpid());         // Print Parent Process ID
    printLine();


//-------------------------------------------------------------------------------------
//   The main (parent) process algorithm:
//      Three possibale cases:
//          - '0' : return the child-process 
//          - '-1' : return the parent-process, failed during creating child-process
//          - 'positive no' : return the parent-process, the pid is equal to child-process
//
//      inspirined on https://docs.oracle.com/cd/E19455-01/806-4750/signals-7/index.html 
//-------------------------------------------------------------------------------------
  
    pid_t childArray[NUM_CHILD];
    pid_t child_pid;
    
    int child_created;
    for(child_created = 0; child_created < NUM_CHILD; child_created++){
        child_pid = fork();                                                         //  Child creation
        
        if(child_pid == -1){                                                        // '-1' : return the parent-process, failed during creating child-process
            printf("\tFailed during creating child-process!\n");
            childTermination(child_created, childArray);
            return 1;
        }
        else if(child_pid == 0){                                                    // '0' : return the child-process 
            childProcess();
            return 0;
        }
        else if(child_pid > 0){                                                     // 'positive no' : return the parent-process, the pid is equal to child-process
            childArray[child_created] = child_pid;

            #ifdef WITH_SIGNALS
            if(!interupted){                                                        //   check the mark which may be set by the keyboard  interrupt handler. 
                printf("\tParent[%d]: with new Child[%d]\n", getpid(), child_pid);
            } else {
                printf("\tParent[%d]: Keyboard interrupt signal [ctrl+c] during creation new Child[%d]\n", getpid(), child_pid);
                childTermination(child_created, childArray);
            }
            #endif

            #ifndef WITH_SIGNALS
                printf("\tParent[%d]: with new Child[%d]\n", getpid(), child_pid);
            #endif

        }

        sleep(1);
    }

    int exitStatus[NUM_CHILD][2];
    int status;
    for(child_created = 0; child_created < NUM_CHILD; child_created++){
        child_pid = wait(&status);
        
        if(child_pid == -1)                                                        // Expecting the child process id, wait until all child procces ends
            printf("\tNo more child procceses");

        exitStatus[child_created][0] = child_pid;
        exitStatus[child_created][1] = WEXITSTATUS(status);
        printf("\tChild[%d]: Exit status: %d\n", exitStatus[child_created][0], exitStatus[child_created][1]);

    }

	#ifdef WITH_SIGNALS      
                           
    for(int i=1; i<_NSIG; i++)                                                      // Default signals
        signal(i, SIG_DFL);
    
    #endif

//-------------------------------------------------------------------------------------
//   Print Summary
//-------------------------------------------------------------------------------------
    printLine();
    printf("\t\t\t\t Summary \n");
    printLine();

    int one_cntr = 0;
    int zero_cntr = 0;
    for(child_created = 0; child_created < NUM_CHILD; child_created++){
        if(exitStatus[child_created][1] ==  1)
            one_cntr++;
        else
            zero_cntr++;
    }

    printf("\tProcesses with 0 exit status: %d\n", zero_cntr);
    printf("\tProcesses with 1 exit status: %d\n", one_cntr);
    printLine();
    
    return 0;
}

//-------------------------------------------------------------------------------------
//   childProcess(): 
//      The child process algorithm:
//-------------------------------------------------------------------------------------
void childProcess() {

    #ifdef WITH_SIGNALS

    signal(SIGINT, SIG_IGN);                                                        // Ignore handling of the keyboard interrupt signal  
    signal(SIGTERM, termHandler);                                                   // Own handler of the SIGTERM signal

    #endif

    printf("\tChild[%d]: Create new child from Parent[%d]\n", getpid(), getppid());
    sleep(10);
    printf("\tChild[%d]: Execution completed\n", getpid());
}

//-------------------------------------------------------------------------------------
//   childTermination(): 
//      The already created child termiantion process:
//-------------------------------------------------------------------------------------
void childTermination(int i, pid_t childArray[]) {
    int j = 0;
    for(j = 0; j < i; j++) {
        kill(childArray[j], SIGTERM);                                               // SIGTERM -  correct terminate a child processes
    }
}

//-------------------------------------------------------------------------------------
//   Handlers funcitons:
//-------------------------------------------------------------------------------------
#ifdef WITH_SIGNALS

void intHandler() {                                                                 // Keyboard interrupt signal handler [ctrl +c]
    printf("\tParent[%d]: Keyboard interrupt signal [ctrl+c] \n", getpid());
    interupted = true;
}


void termHandler() {                                                                // Own handler of the SIGTERM signal
    printf("\tParent[%d]: Termineted by SIGTERM\n", getpid());
    interupted = true;
}

#endif


//-------------------------------------------------------------------------------------
//   Printing helpers funcitons:
//-------------------------------------------------------------------------------------
void printLine() {
    printf("-------------------------------------------------------------------------------------\n");
}

void printHeader() {
    printLine();
    printf("\t\t\t [EOPSY] Processes and Signals \n");
    printLine();
}