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
    dprintf(STDOUT_FILENO, "Child[%d] : Start\n", getpid());
    dprintf(456, "Child[%d] : fd(456) : Test fd.\n", getpid());
    close(456);
    dprintf(STDOUT_FILENO, "Child[%d] : Exit\n", getpid());

    return 0;
}
