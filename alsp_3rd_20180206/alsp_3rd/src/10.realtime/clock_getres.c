/* vim: set ts=4 sw=4: */
/* Filename    : clock_getres.c
 * Description : 
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : 
 * Update      : 
 */
#include <stdio.h>
#include <time.h>

int main()
{
	struct timespec ts;
	clock_getres(CLOCK_REALTIME, &ts);
	printf("clock precision = %ld.%09ld\n",
			ts.tv_sec, ts.tv_nsec);
	return 0;
}
