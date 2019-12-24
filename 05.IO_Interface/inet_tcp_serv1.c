#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define print_msg(io, msgtype, arg...) \
        flockfile(io); \
        fprintf(io, "["#msgtype"][%s/%s:%03d]", __FILE__, __FUNCTION__, __LINE__); \    
        fprintf(io, arg); \
        fputc('\n', io); \
        funlockfile(io);
    /*
        void flockfile(FILE *filehandle)
        void funlockfile(FILE *filehandle)
        int fprintf(FILE *stream, const char *format, ...)
        int fputc(int c, FILE *stream)
    */
#define pr_err(arg...) print_msg(stderr, ERR, arg)
#define pr_out(arg...) print_msg(stdout, REP, arg)

#define LISTEN_BACKLOG  20
#define MAX_POOL        3

/*
    int atoi(const char *nptr)                      type convert : nptr --> int
    int socket(int domain, int type, int protocol)  AF_INET SOCK_STEAM  IPPROTO_IP
    ----------------------------------------------------------------------
    struct sockaddr_in {                struct sockaddr_in6 {
        sa_familty_t    sin_family;         sa_family_t     sin6_family;
        in_port_t       sin_port;           in_port_t       sin6_port;
        struct in_addr  sin_addr;           uint32_t        sin6_flowinfo;
            -----------------------         struct in6_addr sin6_addr;
            struct in_addr {                    --------------------------
                 in_addr_t  s_addr;             struct in6_addr {
            };                                      uint8_t   s6_addr[16];
            -----------------------             };
        char            sin_zero[8];            --------------------------
    };                                      uint32_t        sin6_cope_id;
                                        };
    ----------------------------------------------------------------------
    uint16_t htons(uint16_t hostshort)  port type convert : host(litten) byte --> network(bic) byte
    int bind(int sockfd, const strcut sockaddr *addr, socklen_t addrlen)
    int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
*/

int fd_linstener;                       // Socket Filedescripter
void start_child(int fd, int idx);      // Fucntion that call accept() for connect() in child process

int 
main(int argc, char const *argv[])
{
    int i, rtn;
    short port;                         // Server Port
    socklen_t len_saddr;                // IPv4(AF_INET) sockaddr_* structure length
    struct sockaddr_in saddr_s ={};     // IPv4(AF_INET) sockaddr_* structure
    pid_t pid;                          // process ID for fork()

    if (argc > 2) { 
        printf("%s [port number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {                            // if user insert port number
        port = (short)atoi((char *)argv[1]);    // char -> char * -> int -> short
    } else {
        port = 0;                               // if user don't insert port number
    }                                           // , this API set landom port

    fd_linstener = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);    // IPPROTO_IP --> 0 or IPPROTO_TCP
    if (fd_linstener == -1) {
        pr_err("[TCP server] : Fail : socket()");
        exit(EXIT_FAILURE);
    }

    saddr_s.sin_family = AF_INET;               // IPv4 Domain
    saddr_s.sin_addr.s_addr = INADDR_ANY;       // Multi NIC 동시 지원, 이식성
    saddr_s.sin_port = htons(port);             // port type convert : little endian -> big endian

    rtn = bind(fd_linstener, (struct sockaddr *)&saddr_s, sizeof(saddr_s));
    if (rtn == -1) {
        pr_err("[TCP server] : Fail : bind()");
        exit(EXIT_FAILURE);
    }

    if (port == 0) {                            // if user don't insert port number
        len_saddr = sizeof(saddr_s);            // , to obtain fd_linstener's informantion(port number)
        getsockname(fd_linstener, (struct sockaddr *)&saddr_s, &len_saddr);
    }   // returns the current address to which the socket sockfd is bound, in the buffer pointed to by addr.

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