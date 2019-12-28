/* vim: set ts=4 sw=4: */
/* Filename    : rt_timer_cputime.c
 * Description : RT timer
 * Author      : Sun Young Kim <sunyzero@gmail.com>
 * Notes       : 
 */
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define GET_TIME0(a)	get_time0(a, sizeof(a)) == NULL ? "error" : a
char * get_time0(char *buf, size_t sz_buf);

int inst_timer(void);
void sa_sigaction_rtmin(int signum, siginfo_t *si, void *sv);

int main()
{
	if (inst_timer() == -1) {
		return EXIT_FAILURE;
	}
	while (1) {
        printf("\tsleep(1)\n");
        sleep(1);
        int x;
        for(int i=0; i<32*1024*1024; i++) { /* increase cpu time */
            x += i % 10;
        }
	}

	return 0;
}

/* Name : inst_timer
 * Desc : install timer handler with RT signal
 * Argv : None
 * Ret  : 0 (if success), -1 (fail to convert)
 */
int inst_timer(void)
{
	struct sigaction   sa_rt1;
	struct sigevent    sigev;	/* signal event */
	timer_t	   rt_timer;		/* timer id */
	struct itimerspec  rt_itspec;
	char	ts_now[20];

	memset(&sa_rt1, 0, sizeof(sa_rt1));
	sigemptyset(&sa_rt1.sa_mask);
	sa_rt1.sa_sigaction = sa_sigaction_rtmin; /* handler function */
	sa_rt1.sa_flags = SA_SIGINFO;

	if (sigaction(SIGRTMIN, &sa_rt1, NULL) == -1) {
		perror("FAIL: sigaction()");
		return -1;
	}
	sigev.sigev_notify = SIGEV_SIGNAL; 
	sigev.sigev_signo = SIGRTMIN;

	/* create timer */
	if (timer_create(CLOCK_PROCESS_CPUTIME_ID, &sigev, &rt_timer) == -1) {
		perror("FAIL: timer_create()");
		return -1;
	}

	/* interval timer setting */
	printf("Enable timer at %s\n", GET_TIME0(ts_now));
	rt_itspec.it_value.tv_sec = 1; 
	rt_itspec.it_value.tv_nsec = 0;
	rt_itspec.it_interval.tv_sec = 1; /* periodic timer with 4 sec. */
	rt_itspec.it_interval.tv_nsec = 0;
	if (timer_settime(rt_timer, 0, &rt_itspec, NULL) == -1) {
		perror("FAIL: timer_settime()");
		return -1;
	}

	return 0;
}


/* Name : sa_sigaction_rtmin
 * Desc : handler for RT timer
 * Argv : None
 * Ret  : None
 */
void sa_sigaction_rtmin(int signum, siginfo_t *si, void *sv)
{
	char	ts_now[20];
	printf("[%d] -> RT timer expiration at %s\n", (int)getpid(), GET_TIME0(ts_now));
}


/* Name : get_time0
 * Desc : print current time with UNIX time stamp
 * Argv : None
 * Ret  : Not null(if success) NULL(fail to convert)
 */
char * get_time0(char *buf, size_t sz_buf)
{
/* #define STR_TIME_FORMAT		"%y-%m-%d/%H:%M:%S" */
#define STR_TIME_FORMAT		"%H:%M:%S"
	struct timespec	tspec;
	struct tm	tm_now;
    size_t  sz_ret;

	if (buf == NULL) return NULL;
	if (clock_gettime(CLOCK_MONOTONIC, &tspec) == -1) {
		return NULL;
	}
	localtime_r((time_t *)&tspec.tv_sec, &tm_now);

	if ((sz_ret = strftime(buf, sz_buf, STR_TIME_FORMAT, &tm_now)) == 0) {
		return NULL;
	}
    snprintf(buf + sz_ret, sizeof(buf), ".%09d", (int)tspec.tv_nsec);
	return buf;
}

