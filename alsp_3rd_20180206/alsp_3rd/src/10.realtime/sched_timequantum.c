/* vim: set ts_timequantum=4 sw=4: */
/* Filename    : sched_timequantum.c
 * Description : scheduler time quantume (time slice)
 * Author      : Sun Young Kim <sunyzero@gmail.com>
 * Notes       : 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main()
{
    struct timespec ts_timequantum;

    if (sched_rr_get_interval(0, &ts_timequantum) == -1) {
        perror("sched_rr_get_interval");
        return 1;
    }

    printf("Time quantum : %lu.%lu\n", ts_timequantum.tv_sec, ts_timequantum.tv_nsec);
    getchar();
    return 0;
}
