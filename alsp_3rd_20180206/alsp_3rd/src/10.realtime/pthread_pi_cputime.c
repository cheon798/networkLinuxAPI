/* vim: set ts=4 sw=4: */
/* Filename    : pthread_pi_cputime.c
 * Description : calculate pi
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : numerical integration method
 *               ALSP Chapter.10 Realtime Extensions
 */
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#define MY_SCHED_PRIO   10
#define NUM_THREADS		3
static int num_steps=1000000000;
double pi, step;
struct start_arg {
	int		idx;
	int     i_start;
	int     i_end;
	double  sum;
} start_arg[NUM_THREADS];

struct timespec diff_ts(struct timespec t1, struct timespec t2);

void *start_func(void *arg)
{
	int     i;
	double  x, sum = 0.0;
	struct start_arg *p_arg = (struct start_arg *) arg;
#ifdef _POSIX_THREAD_CPUTIME
	int		ret;
	struct timespec     ts1, ts2, ts_diff;
	clockid_t	clock_cpu;

	if ((ret = pthread_getcpuclockid(pthread_self(), &clock_cpu)) != 0) {
        return NULL;
    }
	clock_gettime(clock_cpu, &ts1);
	printf("[T%d] 1: clock_gettime = %ld.%09ld\n", p_arg->idx, ts1.tv_sec, ts1.tv_nsec);
#endif

	for (i=p_arg->i_start; i<p_arg->i_end; i++) {
		x = (i+0.5) * step;
		sum += 4.0/(1.0 + x*x);
	}
	p_arg->sum = sum;

#ifdef _POSIX_THREAD_CPUTIME
	clock_gettime(clock_cpu, &ts2);
	ts_diff = diff_ts(ts1, ts2);
	printf("[T%d] 2: elapsed cpu time = %ld.%09ld\n", p_arg->idx, ts_diff.tv_sec, ts_diff.tv_nsec);
#endif
	return NULL;
}


int sched_set_options(int policy, int prio)
{
	struct sched_param 	schedp = { .sched_priority = prio };
	if (sched_setscheduler(0, policy, &schedp) == -1) {
		perror("sched_setscheduler");
        return -1;
	}
    printf("sched_setscheduler : %d\n", policy);
    return 0;
}

void sched_print_options() 
{
	struct sched_param 	schedp;

	printf("* current sched_getscheduler = %d\n", sched_getscheduler(0));

	sched_getparam(0, &schedp);
	printf("* current sched_getparam = %d\n", schedp.sched_priority);
}

int main(int argc, char *argv[])
{
	int     i;
	double  sum;
	pthread_t   pt_id[2];

	step = 1.0/(double) num_steps;
	start_arg[0].i_start = 0;
	start_arg[0].i_end   = num_steps>>1;
	start_arg[1].i_start = start_arg[0].i_end + 1;
	start_arg[1].i_end   = start_arg[1].i_start + (num_steps>>2);
	start_arg[2].i_start = start_arg[1].i_end + 1;
	start_arg[2].i_end   = num_steps;

    sched_print_options();
    if (argc != 2) {
        printf("%s <1 | 2 | 3 | 5>\n", argv[0]);
        printf("1:SCHED_FIFO, 2:SCHED_RR, 3:SCHED_BATCH, 5:SCHED_IDLE\n");
        exit(0);
    }
    switch((char)atoi(argv[1])) {
        case 1:
            sched_set_options(SCHED_FIFO, 10);
            break;
        case 2:
            sched_set_options(SCHED_RR, 10);
            break;
        case 3:
            sched_set_options(SCHED_BATCH, 10);
            break;
        case 5:
            sched_set_options(SCHED_IDLE, 10);
            break;
       default:
            break;
    }

	printf("%d~%d, %d~%d, %d~%d\n",
			start_arg[0].i_start, start_arg[0].i_end ,
			start_arg[1].i_start, start_arg[1].i_end ,
			start_arg[2].i_start, start_arg[2].i_end );

	for (i=3; i--; ) { /* create pthread */
		start_arg[i].idx = i;
		if (pthread_create(&pt_id[i], NULL, start_func, &start_arg[i])){
			perror("pthread_create");
			return 1;
		}
	}
	for (i=3; i--; ) { /* join pthread : explicit barrier */
		if (pthread_join(pt_id[i], NULL)){
			perror("pthread_join");
			return 1;
		}
	}
	sum = start_arg[0].sum + start_arg[1].sum + start_arg[2].sum;
	printf("PI = %.8f (sum = %.8f)\n", step*sum, sum);
	return EXIT_SUCCESS;
}

struct timespec diff_ts(struct timespec t1, struct timespec t2)
{
	struct timespec t;
	t.tv_sec = t2.tv_sec - t1.tv_sec;
	t.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	if (t.tv_nsec < 0) {
		t.tv_sec--;
		t.tv_nsec += 1000000000;
	}
	return t;
}

