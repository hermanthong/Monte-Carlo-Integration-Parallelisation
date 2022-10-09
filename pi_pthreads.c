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
#include <pthread.h>

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

typedef struct {
    size_t id;
    int samples;
    int result;
} thread_args;

void* thread_worker(void * args) {
    thread_args *t_args = (thread_args*) args;
    struct rand_gen rand = init_rand_pthreads(t_args->id);
    float randX, randY = 0.0;
    for (int i = 0; i < t_args->samples; ++i) {
            randX = next_rand(rand);
            randY = next_rand(rand);

            //0 to 1 inclusive
            if ((randX * randX) + (randY * randY) < 1)
                ++t_args->result;
        }
    
    free_rand(rand);
    return NULL;
}

double calculate_pi (int num_threads, int samples) {
    double pi;
    pthread_t tid[num_threads];
    thread_args **args = malloc(sizeof(thread_args*) * num_threads);
    int insideCircle = 0;
    //create the threads and make them work
    for (int i = 0; i < num_threads; i ++) {
        args[i] = malloc(sizeof(thread_args));

        args[i]->samples = samples/num_threads;
        args[i]->id = tid[i];
        args[i]->result = 0;
        pthread_create(&tid[i], NULL, thread_worker, (void*)args[i]);
    }

    //join the threads and get final summation
    for (int i = 0; i < num_threads; i ++) {
        pthread_join(tid[i], NULL);
        insideCircle += args[i]->result;
        free(args[i]);
    }
    free(args);
    //free(args);
    pi = 4.0 * insideCircle / samples;
    return pi;
}
