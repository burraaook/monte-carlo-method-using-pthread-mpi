#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "../include/mc_estimation.h"

void log_to_csv_file (int number_of_processes, long long int number_of_tosses, double pi_estimated, double elapsed) {
    FILE *fp;
    fp = fopen("mc_mpi.csv", "a");
    fprintf(fp, "%d; %lld; %f; %f\n", number_of_processes, number_of_tosses, pi_estimated, elapsed);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    long long int number_of_tosses;
    long long int number_in_circle = 0;
    double pi_estimated;
    int my_rank, comm_sz;
    long long int local_number_of_tosses, local_number_in_circle;
    unsigned int seed;
    double start, finish, elapsed;

    /* initialize MPI */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* process 0 reads the number of tosses */
    if (my_rank == 0) {
        if (argc != 2) {
            /* read it from stdin */
            fprintf(stdout, "number of tosses: ");
            fscanf(stdin, "%lld", &number_of_tosses);
        }
        else {
            number_of_tosses = atoll(argv[1]);
        }
        /* starting time */
        start = MPI_Wtime();
    }

    /* broadcast the number of tosses */
    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    /* calculate the number of tosses for each process */
    local_number_of_tosses = number_of_tosses / comm_sz;

    /* find the number in circle */
    seed = time(NULL) + my_rank;
    monte_carlo_simulation(&local_number_of_tosses, &seed, &local_number_in_circle);

    /* reduce the number in circle */
    MPI_Reduce(&local_number_in_circle, &number_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* estimate pi */
    if (my_rank == 0) {

        pi_estimated = 4.0 * number_in_circle / number_of_tosses;
        finish = MPI_Wtime();
        elapsed = finish - start;

        /* set buffer to null */
        setbuf(stdout, NULL);

        fprintf(stdout, "Estimation of Pi with %d procesess, %lld tosses: %f\n", comm_sz, number_of_tosses, pi_estimated);
        fprintf(stdout, "Elapsed time = %f seconds\n", elapsed);

        /* log to csv file */
        log_to_csv_file(comm_sz, number_of_tosses, pi_estimated, elapsed);
    }

    /* finalize MPI */
    MPI_Finalize();

    return 0;
}