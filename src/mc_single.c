#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/mc_estimation.h"

void log_to_csv_file (long long int number_of_tosses, double pi_estimated, double elapsed) {
    FILE *fp;
    fp = fopen("mc_single.csv", "a");
    fprintf(fp, "%lld; %f; %f\n", number_of_tosses, pi_estimated, elapsed);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    long long int number_of_tosses;
    long long int number_in_circle = 0;
    double pi_estimated;
    unsigned int seed = time(NULL);
    /* starting time */
    double elapsed;
    struct timespec start, finish;

    /* get number of tosses */
    if (argc != 2) {
        /* read it from stdin */
        fprintf(stdout, "number of tosses: ");
        fscanf(stdin, "%lld", &number_of_tosses);
    }
    else {
        number_of_tosses = atoll(argv[1]);
    }

    /* starting time */
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* find the number in circle */
    monte_carlo_simulation(&number_of_tosses, &seed, &number_in_circle);

    /* estimate pi */
    pi_estimated = 4.0 * number_in_circle / number_of_tosses;

    /* set buffer to null */
    setbuf(stdout, NULL);

    /* finishing time */
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_nsec - start.tv_nsec) / 1000000000.0 + (finish.tv_sec - start.tv_sec);

    fprintf(stdout, "Estimation of Pi with single thread, %lld tosses: %f\n", number_of_tosses, pi_estimated);
    fprintf(stdout, "Elapsed time: %f\n", elapsed);

    log_to_csv_file(number_of_tosses, pi_estimated, elapsed);
    return 0;
}