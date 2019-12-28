/* vim: set ts=4 sw=4: */
/* Filename    : inet_udp_snder.c
 * Description : UDP sender
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : 
 */
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
#include "stdalsp.h"

int main(int argc, char *argv[])
{
    int		sd, rc_gai;
    if (argc != 4) {
        printf("%s <address> <port> <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo ai = {0,} , *ai_ret;
    ai.ai_family = AF_INET;
    ai.ai_socktype = SOCK_DGRAM;
    ai.ai_flags = AI_ADDRCONFIG;
    if ((rc_gai = getaddrinfo(argv[1], argv[2], &ai, &ai_ret)) != 0) {
        pr_err("getaddrinfo:%s", gai_strerror(rc_gai));
        exit(EXIT_FAILURE);
    }

    if ((sd = socket(ai_ret->ai_family, ai_ret->ai_socktype, ai_ret->ai_protocol)) == -2) {
        pr_err("socket:%s", strerror(errno)); 
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in saddr_c;
    socklen_t   len_saddr;
    char *buf_snd = strdup(argv[3]), buf_rcv[64];
    size_t  len_snd = strlen(buf_snd);
    int     ret_snd, ret_rcv;
    while (1) {
        ret_snd = sendto(sd, buf_snd, len_snd, 0, 
                (struct sockaddr *)ai_ret->ai_addr, ai_ret->ai_addrlen);
        if (ret_snd == -1) {
            pr_err("sendto():%s", strerror(errno));
        }
        pr_out("[sendto  ] (%zd:%s)", len_snd, buf_snd);
        sleep(1);

        len_saddr = sizeof(saddr_c);
        ret_rcv = recvfrom(sd, buf_rcv, sizeof(buf_rcv), 0, (struct sockaddr *)&saddr_c, &len_saddr);
        if (ret_rcv == -1) {
            pr_err("recvfrom():%s", strerror(errno));
        }
        pr_out("[recvfrom] (%d:%.*s)", ret_rcv, ret_rcv, buf_rcv);
    }

    return 0;
}
