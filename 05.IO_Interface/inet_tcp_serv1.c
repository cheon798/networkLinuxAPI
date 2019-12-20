#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
//#include <netinet/ip.h>

#define print_msg(io, msgtype, arg...) \
        flockfile(io); \
        fprintf(io, "["#msgtype"][%s/%s:%03d]", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(io, arg); \
        fputc('\n', io); \
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
    //char *sip = "192.168.0.100";

    if (argc > 2) {
        printf("%s [port number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        port = (short)atoi((char *)argv[1]);    /* char -> char * -> int -> short */
    } else {
        port = 0;   /* landom port */
    }

    fd_linstener = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (fd_linstener == -1) {
        pr_err("[TCP server] : Fail : socket()");
        exit(EXIT_FAILURE);
    }

    saddr_s.sin_family = AF_INET;           /* IPv4 */
    saddr_s.sin_addr.s_addr = INADDR_ANY;   /* 0.0.0.0 */
    saddr_s.sin_port = htons(port);         /* landom port, little -> big endian */

    rtn = bind(fd_linstener, (struct sockaddr *)&saddr_s, sizeof(saddr_s));
    if (rtn == -1) {
        pr_err("[TCP server] : Fail : bind()");
        exit(EXIT_FAILURE);
    }

    if (port == 0) {
        len_saddr = sizeof(saddr_s);
        getsockname(fd_linstener, (struct sockaddr *)&saddr_s, &len_saddr);
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

    for(;;) pause();

    return 0;
}

void
start_child(int sfd, int idx)
{
    int cfd, ret_len;
    char buf[40];           /* small buffer */  
    socklen_t len_saddr;
    struct sockaddr_storage saddr_c;

    for(;;) {
        len_saddr = sizeof(saddr_c);
        cfd = accept(sfd, (struct sockaddr *)&saddr_c, &len_saddr);
        if (cfd == -1) {
            pr_err("[Child] Fail : accept()");
            close(cfd);
            continue;
        }

        if (saddr_c.ss_family == AF_INET) {
            pr_out("[Child:%d] accept (ip:port) (%s:%d)", 
            idx,
            inet_ntoa(((struct sockaddr_in *)&saddr_c)->sin_addr),
            ntohs(((struct sockaddr_in *)&saddr_c)->sin_port));
        }

        for (;;) {
            ret_len = recv(cfd, buf, sizeof(buf), 0);
            if (ret_len == -1) {
                if (errno == EINTR) continue;
                pr_err("[Child:%d] F1ail : recv() : %s", idx, strerror(errno));
                break;
            }

            if (ret_len == 0) {
                pr_err("[Child:%d] Session closed", idx);
                close(cfd);
                break;
            }

            pr_out("[Child:%d] RECV(%.*s)", idx, ret_len, buf);

            if (send(cfd, buf, ret_len, 0) == -1) {
                pr_err("[Child:%d] Fail : send() to socket(%d)", idx, cfd);
                close(cfd);
            }

            sleep(1);
        }
    }  
}