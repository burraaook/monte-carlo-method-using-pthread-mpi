#include "../include/mc_estimation.h"

void monte_carlo_simulation(const long long int *number_of_tosses, unsigned int *seed, long long int *number_in_circle) {
    double x, y, distance_squared;
    long long int toss;
 
    *number_in_circle = 0;

    for (toss = 0; toss < *number_of_tosses; toss++) {
        /* generate random point */
        x = 2 * ((double) rand_r(seed) / RAND_MAX) - 1;
        y = 2 * ((double) rand_r(seed) / RAND_MAX) - 1;

        /* check if point is in circle */
        distance_squared = x * x + y * y;
        if (distance_squared <= 1) {
            (*number_in_circle)++;
        }
    }
}