/* vim: set ts=4 sw=4: */
/* Filename    : epoll_timerfd.c
 * Description : 
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <netdb.h>
#include "stdalsp.h"

#define LISTEN_BACKLOG	256
#define FDSESSION_DEFAULT_TIMEOUT 30

#define ADD_EV(a, b, c)	if (add_ev(a, b, c) == -1) { pr_err("Fail: add_ev"); exit(EXIT_FAILURE); }
#define DEL_EV(a, b)	if (del_ev(a, b) == -1) { pr_err("Fail: del_ev"); exit(EXIT_FAILURE); }

typedef struct fdsession {
    int     socketfd, timerfd, timeout;
} FDSESSION;
typedef struct fds_list {
    int     n_allocated, n_used;
    FDSESSION   *list;
} FDSESSIONLIST;

FDSESSIONLIST fds_list;    /* fd session list */

const int	max_ep_events	= 32;
int	epollfd;		/* epoll fd */

#define GET_TIME0(a)    get_time0(a, sizeof(a)) == NULL ? "error" : a
char * get_time0(char *buf, size_t sz_buf);
int fcntl_setnb(int fd);		/* set non-blocking mode */
int setsockopt_linger(int fd);
int add_ev(int efd, int fd, int timeout);	
int del_ev(int efd, int fd); 
int get_epollevent(int fd_listener, struct epoll_event *ep_events, int ret_poll);
int fds_new_timerfd(FDSESSION **fds_, int socketfd, int timeout);
int fds_set_timerfd(FDSESSION *fds_);
int fds_deletebytimerfd(int timerfd);
int fds_findbysocketfd(FDSESSION **ret_fds, int fd);
int fds_findbytimerfd(FDSESSION **ret_fds, int fd);


int main(int argc, char *argv[])
{
	int		ret, ret_poll, fd_listener;
	char	*port;
	struct epoll_event	*ep_events;
	/* logical var 			*/

	if (argc > 2) {
		printf("%s [port number]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 2) {
		port = strdup(argv[1]);
	} else {
		port = strdup("0");	/* random port */
	}

	struct addrinfo ai, *ai_ret;
	int     rc_gai;
	memset(&ai, 0, sizeof(ai));
	ai.ai_family = AF_INET;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

	if ((rc_gai = getaddrinfo(NULL, port, &ai, &ai_ret)) != 0) {
		pr_err("getaddrinfo():%s", gai_strerror(rc_gai));
		exit(EXIT_FAILURE);
	}

	if ((fd_listener = socket(
					ai_ret->ai_family,
					ai_ret->ai_socktype,
					ai_ret->ai_protocol)) == -1) {
		pr_err("socket()");
		exit(EXIT_FAILURE);
	}

	fcntl_setnb(fd_listener); /* set nonblock flag */

	if (bind(fd_listener, ai_ret->ai_addr, ai_ret->ai_addrlen) == -1) {
		pr_err("bind()");
		exit(EXIT_FAILURE);
	}

	if (!strncmp(port, "0", strlen(port))) { /* port 번호를 알아낸다. */
		struct sockaddr_storage saddr_s;
        socklen_t	len_saddr = sizeof(saddr_s);
		getsockname(fd_listener, (struct sockaddr *)&saddr_s, &len_saddr);
		if (saddr_s.ss_family == AF_INET) {
			pr_out("bind : IPv4 Port : #%d",
					ntohs(((struct sockaddr_in *)&saddr_s)->sin_port));
		} else  if (saddr_s.ss_family == AF_INET6) {
			pr_out("bind : IPv6 Port : #%d",
					ntohs(((struct sockaddr_in6 *)&saddr_s)->sin6_port));
		} else {
			pr_out("getsockname : ss_family=%d", saddr_s.ss_family);
		}
	} else {
		pr_out("bind : %s", port);
	}
	listen(fd_listener, LISTEN_BACKLOG);

	if ((epollfd = epoll_create(1)) == -1) {
		/* error */
		exit(EXIT_FAILURE);
	}
	if ((ep_events = calloc(max_ep_events, sizeof(struct epoll_event))) == NULL) {
		/* error */
		exit(EXIT_FAILURE);
	}

	ADD_EV(epollfd, fd_listener, 0);
    char buf_time[32];
	while (1) {
		pr_out("[%s] epoll waiting ...", GET_TIME0(buf_time));
		if ((ret_poll = epoll_wait(epollfd, ep_events, max_ep_events, -1)) == -1) {
			/* error */
		}
		pr_out("epoll_wait = %d", ret_poll);
        ret = get_epollevent(fd_listener, ep_events, ret_poll);
        if (ret != 0) {
            pr_err("get_epollevent()");
        }
	}

	return 0;
}

int get_epollevent(int fd_listener, struct epoll_event *ep_events, int ret_poll) 
{
	int		i, ret, ret_recv, fd;
	char	buf[1024], buf_time[32];

    for (i=0; i<ret_poll; i++) {
        if (ep_events[i].events & EPOLLIN) {
            /* New connection ? */
            if (ep_events[i].data.fd == fd_listener) {
                while(1) {
                    if ((fd = accept(fd_listener, NULL, NULL)) == -1) {
                        if (errno == EAGAIN) { /* no more new connection */
                            break;
                        }
                        pr_err("accept()");
                        /* need error handling */
                        break;
                    }
                    fcntl_setnb(fd);
                    ADD_EV(epollfd, fd, FDSESSION_DEFAULT_TIMEOUT);
                    pr_out("accept : add socket = %d", fd);
                } /* end : while */
                //return 0;
            } else {
                /* connection in fdsession? */
                FDSESSION *ret_fds;
                if (fds_findbytimerfd(&ret_fds, ep_events[i].data.fd) == 0) {
                    /* If the event has been received by timerfd, 
                     * the timerfd and the socketfd in this session will be removed. */
                    pr_out("timeout[%s] expired/closed (sfd/tfd=%d/%d)", GET_TIME0(buf_time), 
                            ret_fds->socketfd, ret_fds->timerfd);
                    setsockopt_linger(ret_fds->socketfd);
                    DEL_EV(epollfd, ret_fds->socketfd);
                } else {
                    /* if socketfd */
                    if ((ret_recv = recv(ep_events[i].data.fd, buf, sizeof(buf), 0)) == -1) {
                        /* error */
                        pr_err("recv()");
                    } else {
                        if (ret_recv == 0) {
                            /* closed */
                            pr_out("closed by foreign host (sfd=%d)", ep_events[i].data.fd);
                            DEL_EV(epollfd, ep_events[i].data.fd);
                        } else {
                            /* receive a normal msg */
                            pr_out("recv[%s] (fd=%d,n=%d)=%.*s", GET_TIME0(buf_time),
                                    ep_events[i].data.fd, ret_recv, ret_recv, buf);

                            /* set timer */
                            if ((ret = fds_findbysocketfd(&ret_fds, ep_events[i].data.fd)) != 0) {
                                return ret;
                            }
                            if ((ret = fds_set_timerfd(ret_fds)) != 0) {
                                return ret;
                            }
                        }
                    } /* end : recv() */
                } /* end : if-else (timerfd?) */
            } /* end : if-else (fd_listener?) */
        } else if (ep_events[i].events & EPOLLERR) {
            /* error */
        } else {
            pr_out("fd(%d) epoll event(%d) err(%s)", 
                    ep_events[i].data.fd, ep_events[i].events, strerror(errno));
        }
    } /* end : for(i) */
    return 0;
}

/* Name : add_ev
 * Desc : add socket descriptor to epoll
 * Argv : int efd, int fd, int timeout
 * Ret  : zero(if success), negative(if fail)
 */
int add_ev(int efd, int fd, int timeout)
{
	struct epoll_event ev;

	ev.events = EPOLLIN ;
	ev.data.fd = fd;
	if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		pr_err("fd(%d) EPOLL_CTL_ADD  Error(%d:%s)", fd, errno, strerror(errno));
		return -1;
	}

    if (timeout != 0) {
        int     ret;
        FDSESSION *ret_fds;
        if ((ret = fds_new_timerfd(&ret_fds, fd, timeout)) != 0) {
            return ret;
        }
        if ((ret = fds_set_timerfd(ret_fds)) != 0) {
            return ret;
        }
        ev.events = EPOLLIN;
        ev.data.fd = ret_fds->timerfd;
        if (epoll_ctl(efd, EPOLL_CTL_ADD, ret_fds->timerfd, &ev) == -1) {
            pr_err("fd(%d) EPOLL_CTL_ADD  Error(%d:%s)", fd, errno, strerror(errno));
            return -1;
        }
    }

	return 0;
}

/* Name : del_ev
 * Desc : delete socket descriptor from epoll
 * Argv : int efd, int fd
 * Ret  : zero(if success), negative(if fail)
 */
int del_ev(int efd, int fd)
{
	if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		pr_err("fd(%d) EPOLL_CTL_DEL Error(%d:%s)", fd, errno, strerror(errno));
		return errno;
	}
	close(fd);

    int     ret;
    FDSESSION   *ret_fds;
    if ((ret = fds_findbysocketfd(&ret_fds, fd)) != 0) {
        /* 에러처리필요 */
        return ret;
    }
	if (epoll_ctl(efd, EPOLL_CTL_DEL, ret_fds->timerfd, NULL) == -1) {
		pr_err("fd(%d) EPOLL_CTL_DEL Error(%d:%s)", ret_fds->timerfd, errno, strerror(errno));
        /* 에러처리필요 */
		return errno;
	}
    close(ret_fds->timerfd);
    if ((ret = fds_deletebytimerfd(ret_fds->timerfd)) != 0) {
        /* 에러처리필요 */
        return ret;
    }
	return 0;
}


int fds_new_timerfd(FDSESSION **fds, int socketfd, int timeout)
{
    if (fds_list.n_allocated == fds_list.n_used) {
        int n_allocated = fds_list.n_allocated + 16; /* make a additional memory for fds_ */
        FDSESSION *p_fds = realloc(fds_list.list, n_allocated * sizeof(FDSESSION));
        if (p_fds == NULL) {
            return errno;
        }
        fds_list.list = p_fds;   /* reallocated address */
        fds_list.n_allocated = n_allocated;    /* number of allocated elements */
    }

    int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK|TFD_CLOEXEC);
    if (fd == -1) {
        perror("timerfd_craete");
        /* 에러처리 */
        return errno;
    }

    fds_list.list[fds_list.n_used].socketfd = socketfd;
    fds_list.list[fds_list.n_used].timerfd = fd;
    fds_list.list[fds_list.n_used].timeout = timeout;
    *fds = &fds_list.list[fds_list.n_used];
    ++fds_list.n_used;

    return 0;
}

int fds_set_timerfd(FDSESSION *fds)
{
    struct itimerspec rt_tspec = { .it_value.tv_sec = fds->timeout };
    if (timerfd_settime(fds->timerfd, 0, &rt_tspec, NULL) == -1) {
        pr_err("timerfd_settime");
        /* 에러처리 */
        return errno;
    }
    char buf_time[32];
    pr_out("timeout[%s] start (sfd/tfd=%d/%d, timeout=%d)", GET_TIME0(buf_time), 
            fds->socketfd, fds->timerfd, fds->timeout);

    return 0;
}

int fds_deletebytimerfd(int timerfd)
{
    int  i, flag = 0; /*  1:found, 0:not found */
    FDSESSION *iter_fds;
    for (i=0; i<fds_list.n_used; i++) {
        iter_fds = &fds_list.list[i];
        if (iter_fds->timerfd == timerfd) {
            if (i != (fds_list.n_used-1)) {
                fds_list.list[i] = fds_list.list[fds_list.n_used-1];
                /*  replace the fds_ to last one */
            }
            fds_list.list[fds_list.n_used-1].socketfd = -1;
            fds_list.list[fds_list.n_used-1].timerfd = -1;
            flag = 1;
            break;
        }
    } /*  loop: for(i) */

    if (flag == 0) {
        return ENOENT;
    }
    --fds_list.n_used;
    return 0;
}

int fds_findbysocketfd(FDSESSION **ret_fds, int fd)
{
    int i, flag = 0;
    FDSESSION *iter_fds;
    for (i=0; i<fds_list.n_used; i++) {
        iter_fds = &fds_list.list[i];
        if (iter_fds->socketfd == fd) {
            *ret_fds = iter_fds;
            flag = 1;
            break;
        }
    } /*  loop: for(i) */

    if (flag == 0) {
        return -1;
    }

    return 0;
}

int fds_findbytimerfd(FDSESSION **ret_fds, int fd)
{
    int i, flag = 0;
    FDSESSION *iter_fds;
    for (i=0; i<fds_list.n_used; i++) {
        iter_fds = &fds_list.list[i];
        if (iter_fds->timerfd == fd) {
            *ret_fds = iter_fds;
            flag = 1;
            break;
        }
    } /*  loop: for(i) */

    if (flag == 0) {
        return -1;
    }

    return 0;
}

/* Name : fcntl_setnb
 * Desc : set non-blocking mode
 * Argv : int fd
 * Ret  : zero(if success), otherwise(if fail)
 */
int fcntl_setnb(int fd)
{
	/* only influence about O_ASYNC, O_APPEND, O_NONBLOCK on Linux-specific */
	if (fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL)) == -1) {	
		return errno;
	}
	return 0;
}

/* Name : setsockopt_linger
 * Desc : set linger timeout to zero
 * Argv : int fd
 * Ret  : zero(if success), otherwise(if fail)
 */
int setsockopt_linger(int fd)
{
    struct linger so_linger = { .l_onoff=1, .l_linger=0 };
    if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) == -1) {
        return errno;
    }
    return 0;
}


/* Name : get_time0
 * Desc : print current time with UNIX time stamp
 * Argv : None
 * Ret  : Not null(if success) NULL(fail to convert)
 */
char * get_time0(char *buf, size_t sz_buf)
{
/* #define STR_TIME_FORMAT      "%y-%m-%d/%H:%M:%S" */
#define STR_TIME_FORMAT     "%H:%M:%S"
    struct timespec tspec;
    struct tm   tm_now;
    size_t  sz_ret;

    if (buf == NULL || sz_buf < 20) return NULL;
    if (clock_gettime(CLOCK_MONOTONIC, &tspec) == -1) {
        return NULL;
    }
    localtime_r((time_t *)&tspec.tv_sec, &tm_now);

    if ((sz_ret = strftime(buf, sz_buf, STR_TIME_FORMAT, &tm_now)) == 0) {
        return NULL;
    }
    return buf;
}
