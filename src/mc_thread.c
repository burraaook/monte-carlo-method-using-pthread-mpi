#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "../include/mc_estimation.h"

typedef struct {
    long long int number_in_circle;
    long long int number_of_tosses;
    unsigned int seed;
} thread_data_t;

/* define a thread pool */
typedef struct {
    pthread_t *threads;
    int number_of_threads;
    long long int number_of_tosses;
    long long int number_in_circle;
    pthread_mutex_t mutex;
    thread_data_t *thread_data;
} thread_pool_t;

thread_pool_t *thread_pool;

void *worker_thread (void *arg) {
    thread_data_t *data = (thread_data_t *) arg;
    monte_carlo_simulation(&data->number_of_tosses, &data->seed, &data->number_in_circle);
    /* increment number in circle */
    pthread_mutex_lock(&thread_pool->mutex);
    thread_pool->number_in_circle += data->number_in_circle;
    pthread_mutex_unlock(&thread_pool->mutex);
    return NULL;
}

void log_to_csv_file (int number_of_threads, const long long int *number_of_tosses, double pi_estimated, double elapsed);

void mc_thread_test (const long long int *total_number_of_tosses, int number_of_threads) {
    double pi_estimated;
    int thread;
    struct timespec start, finish;
    double elapsed;
    long long int number_of_tosses;

    /* allocate memory for threads and thread data */
    thread_pool = (thread_pool_t *) malloc(sizeof(thread_pool_t));
    thread_pool->number_of_threads = number_of_threads;
    thread_pool->threads = (pthread_t *) malloc(thread_pool->number_of_threads * sizeof(pthread_t));
    thread_pool->thread_data = (thread_data_t *) malloc(thread_pool->number_of_threads * sizeof(thread_data_t));
    thread_pool->number_in_circle = 0;

    /* create mutex */
    pthread_mutex_init(&thread_pool->mutex, NULL);

    /* starting time */
    clock_gettime(CLOCK_MONOTONIC, &start);

    number_of_tosses = *total_number_of_tosses / number_of_threads;

    /* create threads */
    for (thread = 0; thread < number_of_threads; thread++) {
        thread_pool->number_of_tosses = number_of_tosses;
        thread_pool->thread_data[thread].number_of_tosses = number_of_tosses;
        thread_pool->thread_data[thread].seed = time(NULL) + thread;
        pthread_create(&thread_pool->threads[thread], NULL, worker_thread, &thread_pool->thread_data[thread]);
    }

    /* join threads */
    for (thread = 0; thread < number_of_threads; thread++) {
        pthread_join(thread_pool->threads[thread], NULL);
    }
    
    /* estimate pi */
    pi_estimated = 4.0 * thread_pool->number_in_circle / *total_number_of_tosses;

    /* finishing time */
    clock_gettime(CLOCK_MONOTONIC, &finish);

    /* destroy mutex */
    pthread_mutex_destroy(&thread_pool->mutex);

    /* free memory */
    free(thread_pool->threads);
    free(thread_pool->thread_data);
    free(thread_pool);

    elapsed = (finish.tv_nsec - start.tv_nsec) / 1000000000.0 + (finish.tv_sec - start.tv_sec);
    
    /* set buffer to null */
    setbuf(stdout, NULL);

    fprintf(stdout, "Estimation of Pi with %d threads, %lld tosses: %f\n", number_of_threads, *total_number_of_tosses, pi_estimated);
    fprintf(stdout, "Elapsed time: %f\n", elapsed);

    log_to_csv_file(number_of_threads, total_number_of_tosses, pi_estimated, elapsed);
}

int main (int argc, char *argv[]) {

    long long int number_of_tosses;
    int number_of_threads = 1;
    /* get number of threads and number of tosses */
    /* <number of threads> <number of tosses> */
    if (argc == 1) {
        /* read it from stdin */
        fprintf(stdout, "number of threads: ");
        fscanf(stdin, "%d", &number_of_threads);
        fprintf(stdout, "number of tosses: ");
        fscanf(stdin, "%lld", &number_of_tosses);
    }
    else if (argc == 2) {
        /* read it from stdin */
        fprintf(stdout, "number of tosses: ");
        fscanf(stdin, "%lld", &number_of_tosses);

        number_of_threads = atoi(argv[1]);
    }
    else {
        number_of_threads = atoi(argv[1]);
        number_of_tosses = atoll(argv[2]);
    }

    mc_thread_test(&number_of_tosses, number_of_threads);
    return 0;
}

void log_to_csv_file (int number_of_threads, const long long int *number_of_tosses, double pi_estimated, double elapsed) {
    FILE *fp;
    fp = fopen("mc_thread.csv", "a");
    fprintf(fp, "%d;%lld;%f;%f\n", number_of_threads, *number_of_tosses, pi_estimated, elapsed);
    fclose(fp);
}