#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int 
main(int argc, char const *argv[])
{
    int i;
    pid_t cpid;

    for (i=0 ; i<3 ; i++) {
        cpid = fork();

        printf("[%d] PID(%d) PPID(%d)\n", i, getpid(), getppid());

#ifndef OMIT_SWITH
        switch (cpid) {
            case 0:         // Child Process
                pause();
                break;
            
            case -1:        // Error Case
                break;

            default:        // Parent Process
                break;
        }
#endif

    }

    wait(NULL);

    return 0;
}