#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int 
main(int argc, char const *argv[])
{
    int fd;
    pid_t cpid;

    printf("Parent[%d] : Start\n", getpid());

    fd = open("forkexec.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open() fail");
        exit(EXIT_FAILURE);
    }

    dprintf(fd, "Parent[%d] : Open log file(fd=%d)\n", getpid(), fd);

    #ifdef APPLY_FD_CLOEXEC
        int ret;
        if ((ret = fcntl(fd, F_SETFD, FD_CLOEXEC)) == -1) {
            perror("fcntl() fail");
            exit(EXIT_FAILURE);
        }
    #endif

    char *argv_exec[] = {"forkexec_child", (char *)NULL};

    cpid = fork();
    switch (cpid) {
        case 0 :
            execv(argv_exec[0], argv_exec);
            break;

        case -1 :
            perror("fork() : fail");
            break;

        default :
            wait(NULL);
            break;
    }

    printf("Parent[%d] : Exit\n", getpid());

    return 0;
}
