#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

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
/*
    struct addrinfo {
        int              ai_flags;      // input flag (AI_*const)
        int              ai_family;     // AF_INET, AF_INET6, ...
        int              ai_socktype;   // SOCK_STREAM, SOCK_DGRAM, ...
        int              ai_protocol;   // IPPROTO_TCP, IPPROT_UDP, IPPROTO_IP, ...
        socklen_t        ai_addrlen;    // sock address length
        struct sockaddr *ai_addr;       // sock address structure pointer
        char            *ai_canonname;  // host canonical name
        struct addrinfo *ai_next;       // address information structure pointer
    };
*/
int 
main(int argc, char const *argv[])
{
    int sfd, rc_gai;
    struct addrinfo ai;    // ai = {0,};  // ai struct member 변수를 모두 0으로 초기화 
    struct addrinfo *ai_ret; 

    if (argc != 4) {
        printf("%s [ip address] [port number] [message]\n", argv[0]);
        exit(EXIT_FAILURE);    
    }

    memset(&ai, 0x00, sizeof(struct addrinfo));

    ai.ai_flags = AI_ADDRCONFIG;
    ai.ai_family = AF_INET;
    ai.ai_socktype = SOCK_DGRAM;
    ai.ai_protocol = IPPROTO_UDP;
    

    rc_gai = getaddrinfo(argv[1], argv[2], &ai, &ai_ret);
    if (rc_gai != 0) {
        pr_err("[UDP Sender] : Fail : getaddrinfo() : %s", gai_strerror(rc_gai));
        exit(EXIT_FAILURE);
    }

    sfd = socket(ai_ret->ai_family, ai_ret->ai_socktype, ai_ret->ai_protocol);
    if (sfd == -1) {
        pr_err("[UDP Sender] : Fail : socket() : %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int ret_snd, ret_rcv;
    char *buf_snd = strdup(argv[3]);
    char  buf_rcv[64];
    size_t len_snd = strlen(buf_snd);
    socklen_t len_saddr;
    struct sockaddr_in saddr_c;

    while (1) {
        ret_snd = sendto(sfd, buf_snd, len_snd, 0, 
                    (struct sockaddr *)ai_ret->ai_addr, ai_ret->ai_addrlen);
        if (ret_snd == -1) {
            pr_err("[UDP Sender] : Fail : sendto() : %s", strerror(errno));
        }

        pr_out("[sendto  ] (%zd:%s)", len_snd, buf_snd);

        len_saddr = sizeof(saddr_c);

        ret_rcv = recvfrom(sfd, buf_rcv, sizeof(buf_rcv), 0, (struct sockaddr *)&saddr_c, &len_saddr);
        if (ret_rcv == -1) {
            pr_err("[UDP Sender] : Fail : recvfrom() : %s", strerror(errno));
        }

        pr_out("[recvfrom] (%d:%.*s)", ret_rcv, ret_rcv, buf_rcv);
    }
    
    return 0;
}