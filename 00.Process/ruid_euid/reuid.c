#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
 
int main(int argc, char **argv)
{
    printf("RUID : %d\n", getuid());
    printf("EUID : %d\n", geteuid());

    FILE *fp = fopen("dummy", "w");
    if(fp) {
        fprintf(fp, "Hello\n");
        fclose(fp);
    } else {
        printf("fopen error\n");
    }

    return 0;
}
