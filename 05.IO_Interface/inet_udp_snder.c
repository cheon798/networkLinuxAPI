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

int 
main(int argc, char const *argv[])
{
    int sfd, rc_gai;

    if (argc != 4) {
        printf("%s <address> <port> <message>\n", argv[0]);
        exit(EXIT_FAILURE);    
    }

    struct addrinfo ai;

    


    return 0;
}
