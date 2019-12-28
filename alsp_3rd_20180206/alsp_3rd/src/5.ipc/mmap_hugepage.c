#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
typedef struct elem_data {
    int num; /* 4B, 4B */
    char str[28]; /* 28B, 32B */
} ELEM_DATA;
int n_elem;
ELEM_DATA *p_array; /* base address of linear array */
void print_elem(const ELEM_DATA *dest);
char *get_deltatimespec_str(struct timespec *ts_now, struct timespec * const ts_prev);
struct timespec diff_timespec(struct timespec ts_now, struct timespec ts_prev);
int main(int argc, char *argv[])
{
    int i, idx, ret;
    long n_array;
    int cnt_randomwalking = 800000000; /* 800 million */
    if (argc != 3) {
        printf("%s < huge | normal > <# MiB>\n", argv[0]);
        exit(1);
    }

    int flags;
    if (argv[1][0] == 'h') {
        flags = MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE|MAP_HUGETLB;
    } else {
        flags = MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE;
    }
    n_array = atoi(argv[2])*(1024*(1024/sizeof(ELEM_DATA)));

    /* get a cpu clock */
    struct timespec ts0, ts1, ts2;
    clockid_t clock_cpu;
    if ((ret = clock_getcpuclockid(0, &clock_cpu)) != 0) {
        return EXIT_FAILURE;
    }
    clock_gettime(clock_cpu, &ts0);

    printf("# of Array=%ld, Tot. size of mmap=%ld, # of test-count=%d \n",
            n_array, n_array * sizeof(ELEM_DATA), cnt_randomwalking);

    size_t sz_map = n_array * sizeof(ELEM_DATA);
    p_array = mmap(NULL, sz_map, PROT_READ|PROT_WRITE, flags, -1, 0);
    if (p_array == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* input data */
    for (i=0; i<n_array; i++) {
        p_array[i].num = i;
        stpncpy(p_array[i].str, "test", 4);
    }
    printf("[Phase 1] init mmap. elapsed cpu time = %s\n", get_deltatimespec_str(&ts1, &ts0));

    printf(">> Random walking : %d iterations\n", cnt_randomwalking);

    srandom(time(NULL));
    for (i=0; i<cnt_randomwalking; i++) {
        idx = rand() % n_array;
        p_array[idx].num += 2; /* update */
    }

    printf("[Phase 2] elapsed cpu time = %s\n", get_deltatimespec_str(&ts2, &ts1));
    printf("Press any key to exit.\n");
    getchar();

    return 0;
}

char *get_deltatimespec_str(struct timespec *ts_now, struct timespec * const ts_prev)
{
    static char str_ts[16];
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, ts_now);
    if (ts_prev != NULL) {
        struct timespec ts_diff = diff_timespec(*ts_now, *ts_prev);
        snprintf(str_ts, sizeof(str_ts), "%ld.%09ld", ts_diff.tv_sec, ts_diff.tv_nsec);
    }
    return str_ts;
}

struct timespec diff_timespec(struct timespec ts_now, struct timespec ts_prev)
{
    struct timespec t;
    t.tv_sec = ts_now.tv_sec - ts_prev.tv_sec;
    t.tv_nsec = ts_now.tv_nsec - ts_prev.tv_nsec;
    if (t.tv_nsec < 0) {
        t.tv_sec--;
        t.tv_nsec += 1000000000;
    }
    return t;
}
