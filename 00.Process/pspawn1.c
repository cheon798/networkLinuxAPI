#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <spawn.h>
#include <wait.h>

int 
main(int argc, char const *argv[])
{
    int rtn, wstatus;
    pid_t cpid, wpid;
    char buf_err[64];

    posix_spawn_file_actions_t posix_faction;

    char *argv_exec[] = {"forkexec_child", (char *)NULL};
    printf("Parent[%d] : Start\n", getpid());

    rtn = posix_spawn_file_actions_init(&posix_faction);
    if (rtn != 0) {
        strerror_r(rtn, buf_err, sizeof(buf_err));
        fprintf(stderr, "Fail : file_actions_init : %s\n", buf_err);
        exit(EXIT_FAILURE);
    }

    rtn = posix_spawn_file_actions_addopen(&posix_faction, 456, "pspawn.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
    if (rtn != 0) {
        strerror_r(rtn, buf_err, sizeof(buf_err));
        fprintf(stderr, "Fail : file_actions_addopen : %s\n", buf_err);
        exit(EXIT_FAILURE);
    }

    rtn = posix_spawn(&cpid, argv_exec[0], &posix_faction, NULL, argv_exec, NULL);
    // int 
    // posix_spawn(pid_t *restrict pid,                             &cpid,
    //             const char *restrict path,                       argv_exec[0],
    //             const posix_spawn_file_actions_t *file_actions,  &posix_faction,
    //             const posix_spawnattr_t *restrict attrp,         NULL,
    //             char *const argv[restrict],                      argv_exec,
    //             char *const envp[restrict])                      NULL)
    
    rtn = posix_spawn_file_actions_destroy(&posix_faction);
     if (rtn != 0) {
        strerror_r(rtn, buf_err, sizeof(buf_err));
        fprintf(stderr, "Fail : file_actions_destroy : %s\n", buf_err);
        exit(EXIT_FAILURE);
    }   

    printf("Parent[%d] : Wait for child(%d)\n", getpid(), (int)cpid);

    wpid = wait(&wstatus);
    if (wpid == -1) {
        perror("wait() \n");
    } else {
        if (WIFEXITED(wstatus)) {
            printf("[COMPLETE END]  SIG : %d \n", WEXITSTATUS(wstatus));
            printf("[COMPLETE END] CPID : %d \n", wpid);
        } else if (WIFSIGNALED(wstatus)) {
            printf("[KILLED BY SIG]  SIG : %d \n", WTERMSIG(wstatus));
            printf("[KILLED BY SIG] CPID : %d \n", wpid);
        } else if (WIFSTOPPED(wstatus)) {
            printf("[STOPPED BY SIG]  SIG : %d \n", WTERMSIG(wstatus));
            printf("[STOPPED BY SIG] CPID : %d \n", wpid);
        } else if (WIFCONTINUED(wstatus)) {
            printf("[CONTINUED] CPID : %d \n", wpid);
        }
    }

    printf("Parent[%d] : Exit\n", getpid());

    return 0;
}
