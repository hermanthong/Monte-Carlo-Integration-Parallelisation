/*
============================================================================
Filename    : pi.c
Author      : Your names goes here
SCIPER		: Your SCIPER numbers
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

double calculate_pi (int num_threads, int samples);

int main (int argc, const char *argv[]) {

    int num_threads, num_samples;
    double pi;

    if (argc != 3) {
		printf("Invalid input! Usage: ./pi <num_threads> <num_samples> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
	}

    set_clock();
    pi = calculate_pi (num_threads, num_samples);

    printf("- Using %d threads: pi = %.15g computed in %.4gs.\n", num_threads, pi, elapsed_time());

    return 0;
}


double calculate_pi (int num_threads, int samples) {
   
    omp_set_num_threads(num_threads);
    int insideCircle = 0;
    double randX, randY = 0.0;

    #pragma omp parallel private(randX, randY) reduction(+: insideCircle) 
    {
        struct rand_gen rand = init_rand();
        for (int i = 0; i < samples/num_threads; ++i) {
            randX = next_rand(rand);
            randY = next_rand(rand);

            //0 to 1 inclusive
            if ((randX * randX) + (randY * randY) < 1)
                ++insideCircle;
        }
        free_rand(rand);
    }
    
    
    //insideCircle += insideCircle_priv;
    
    
    double pi = 4.0 * insideCircle / samples;

    return pi;
}
