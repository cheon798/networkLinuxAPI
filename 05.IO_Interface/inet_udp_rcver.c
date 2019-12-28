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

#define MAX_POOL    4

/*
-----------------------------------------------------------------------
struct sockaddr_in {               | struct sockaddr_in6 {
    sa_familty_t    sin_family;    |     sa_family_t     sin6_family;
    in_port_t       sin_port;      |     in_port_t       sin6_port;
    struct in_addr  sin_addr;      |     uint32_t        sin6_flowinfo;
        ------------------------   |     struct in6_addr sin6_addr;
        struct in_addr {           |         --------------------------
               in_addr_t  s_addr   |         struct in6_addr {
        };                         |             uint8_t   s6_addr[16];
        ------------------------   |         };
    char            sin_zero[8];   |         --------------------------
};                                 |     uint32_t        sin6_cope_id;
                                   | };
------------------------------------------------------------------------
struct sockaddr {             | struct sockaddr_storage {
    sa_family_t sa_family;    |     sa_family_t       ss_family;
    char        sa_data[14];  |     char              __ss_padding[118];
};                            |     unsigned long int __ss_align;
                              | };
------------------------------------------------------------------------
*/
void start_child(int rfd, int idx);

int 
main(int argc, char const *argv[])
{
    int i, rfd, rtn;
    short port;
    socklen_t len_saddr;
    struct sockaddr_in saddr_s;
    pid_t pid;

    memset(&saddr_s, 0x00, sizeof(saddr_s));

    if (argc > 2) { 
        printf("%s [port number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {                            // if user insert port number
        port = (short)atoi((char *)argv[1]);    // char -> char * -> int -> short
    } else {
        port = 0;                               // if user don't insert port number
    }                                           // , this API set landom port

    rfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (rfd == -1) {
        pr_err("[UDP Receiver] : Fail : socket()");
        exit(EXIT_FAILURE);
    }

    saddr_s.sin_family = AF_INET;               // IPv4 Domain
    saddr_s.sin_addr.s_addr = INADDR_ANY;       // Multi NIC 동시 지원, 이식성
    saddr_s.sin_port = htons(port);             // port type convert : little endian -> big endian

    rtn = bind(rfd, (struct sockaddr *)&saddr_s, sizeof(saddr_s));
    if (rtn == -1) {
        pr_err("[UDP Receiver] : Fail : bind()"); 
        exit(EXIT_FAILURE);
    }

    if (port == 0) {                            // if user don't insert port number
        len_saddr = sizeof(saddr_s);            // , to obtain fd_linstener's informantion(port number)
        getsockname(rfd, (struct sockaddr *)&saddr_s, &len_saddr);
    }   // returns the current address to which the socket sockfd is bound, in the buffer pointed to by addr.

    pr_out("[UDP Receiver] Port : #%d", ntohs(saddr_s.sin_port));

    for (i=0 ; i<MAX_POOL ; i++) {
        switch (pid = fork())
        {
            case 0 : /* child process */
                start_child(rfd, i);
                exit(EXIT_SUCCESS);
                break;
            
            case -1 : /* error case */
                pr_err("[UDP Receiver] : Fail : fork()");
                break;

            default : /* parent process */
                pr_out("[UDP Receiver] Making child process No.%d", i);
                break;
        }
    }

    for(;;) pause();

    return 0;
}

void
start_child(int rfd, int idx)
{
    int len_rcv, len_snd;
    char rbuf[40];
    socklen_t len_saddr;
    struct sockaddr_in saddr_c;

    memset(&saddr_c, 0x00, sizeof(saddr_c));

    while (1) {
        len_saddr = sizeof(saddr_c);
        len_rcv = recvfrom(rfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&saddr_c, &len_saddr);
        if (len_rcv == -1) {
            if (errno == EINTR) continue;
            pr_err("[UDP Receiver] : Fail : recvfrom() : %s", strerror(errno));
            break;
        }

        if (len_rcv == 0) {             // Active Close : client request to connection exit
            pr_out("[UDP Receiver] Session closed");
            break;
        }

        pr_out("[recvfrom] (%d byte) RECV(%.*s)", len_rcv, rbuf[len_rcv-1] == '\n' ? len_rcv-1 : len_rcv, rbuf);
        pr_out("[sendto  ] (%s:%d)", inet_ntoa(saddr_c.sin_addr), ntohs(saddr_c.sin_port));

        len_snd = sendto(rfd, rbuf, len_rcv, 0, (struct sockaddr *)&saddr_c, sizeof(saddr_c));
        if (len_snd == -1) {
            pr_err("[UDP Receiver] : Fail : sendto()");
        }
    }
}