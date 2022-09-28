/*
============================================================================
Filename    : integral.c
Author      : Your names goes here
SCIPER		: Your SCIPER numbers
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "function.c"

double integrate (int num_threads, int samples, int a, int b, double (*f)(double));

int main (int argc, const char *argv[]) {

    int num_threads, num_samples, a, b;
    double integral;

    if (argc != 5) {
		printf("Invalid input! Usage: ./integral <num_threads> <num_samples> <a> <b>\n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
        a = atoi(argv[3]);
        b = atoi(argv[4]);
	}

    set_clock();

    /* You can use your self-defined funtions by replacing identity_f. */
    integral = integrate (num_threads, num_samples, a, b, identity_f);

    printf("- Using %d threads: integral on [%d,%d] = %.15g computed in %.4gs.\n", num_threads, a, b, integral, elapsed_time());

    return 0;
}


double integrate (int num_threads, int samples, int a, int b, double (*f)(double)) {
    double integral;

    omp_set_num_threads(num_threads);
    double totalY = 0;
    double randX = 0.0;

    #pragma omp parallel private(randX) reduction(+: totalY) 
    {
        struct rand_gen rand = init_rand();
        for (int i = 0; i < samples/num_threads; ++i) {
            randX = next_rand(rand) * (b-a) + a;
            totalY += f(randX);
        }
        free_rand(rand);
    }    
    
    integral = (totalY * (b-a)) / samples;

    return integral;
}
