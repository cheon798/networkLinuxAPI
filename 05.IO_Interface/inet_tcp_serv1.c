#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <netinet/ip.h>

#define print_msg(io, msgtype, arg...) \
        flockfile(io); \
        fprintf(io, "["#msgtype"][%s/%s:%03d]", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(io, arg); \
        fput('\n', io); \
        funlockfile(io);

#define pr_err(arg...) print_msg(stderr, ERR, arg)
#define pr_out(arg...) print_msg(stdout, REP, arg)

#define LISTEN_BACKLOG  20
#define MAX_POOL    3

int fd_linstener;
void start_child(int fd, int idx);

int 
main(int argc, char const *argv[])
{
    int i, rtn;
    short port;
    socklen_t len_saddr;
    struct sockaddr_in saddr_s ={};
    pid_t pid;

    if (argc > 2) {
        printf("%s [port number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        port = (short)atoi((char *)argv[1]);
    } else {
        port = 0;   /* landom port */
    }

    fd_linstener = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (fd_linstener == -1) {
        pr_err("[TCP server] : Fail : socket()");
        exit(EXIT_FAILURE);
    }

    saddr_s.sin_family = AF_INET;
    saddr_s.sin_addr.s_addr = INADDR_ANY;
    saddr_s.sin_port = htons(port);

    rtn = bind(fd_linstener, (struct sockaddr *)&saddr_s, sizeof(saddr_s));
    if (rtn == -1) {
        pr_err("[TCP server] : Fail : bind()");
        exit(EXIT_FAILURE);
    }

    if (port == 0) {
        len_saddr = sizeof(saddr_s);
        getsockname(fd_linstener, (struct sockaddr *)&saddr_s, sizeof(saddr_s));
    }

    pr_out("[TCP server] Port : #%d", ntohs(saddr_s.sin_port));

    rtn = listen(fd_linstener, LISTEN_BACKLOG);
    if (rtn == -1) {
        pr_err("[TCP server] : Fail : listen()");
        exit(EXIT_FAILURE);
    }

    for (i=0 ; i<MAX_POOL ; i++) {
        switch (pid = fork())
        {
        case 0 :    /* child process*/
            start_child(fd_linstener, i);
            exit(EXIT_SUCCESS);
            break;
            
        case -1 :   /* error case */
            pr_err("[TCP server] : Fail : fork()");
            break;

        default:    /* parent process */
            pr_out("[TCP server] Making child process No.%d", i);
            break;
        }
    }

    return 0;
}
