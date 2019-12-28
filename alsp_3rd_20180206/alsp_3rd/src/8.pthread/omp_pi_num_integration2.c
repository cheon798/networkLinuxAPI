/* vim: set ts=4 sw=4: */
/* Filename    : omp_pi_num_integration2.c
 * Description : calculate pi
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : numerical integration method
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
const int num_steps=800000000; /* integration 횟수: 8억번 (너무 많으면 줄이자.) */
int main()
{
	double step, sum = 0.0;
	step = 1.0/(double) num_steps;
    printf("%d, %d\n", num_steps<<1, num_steps>>1);
#pragma omp parallel sections reduction(+:sum)
    {
#pragma omp section
        {
            double x1;
            int num_steps1=num_steps>>1;
            for (int i=0; i<num_steps1; i++) {
                x1 = (i+0.5) * step;
                sum += 4.0/(1.0 + x1*x1);
            }
        }
#pragma omp section
        {
            double x2;
            for (int i = num_steps>>1; i<num_steps; i++) {
                x2 = (i+0.5) * step;
                sum += 4.0/(1.0 + x2*x2);
            }
        }
    }
    printf("pi = %.8f (sum = %.8f), 4*atan(1) = %.8f\n", 
            step*sum, sum, atan(1)*4);
	return EXIT_SUCCESS;
}
