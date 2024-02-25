#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#ifndef MC_ESTIMATION_H
#define MC_ESTIMATION_H

void monte_carlo_simulation(const long long int *number_of_tosses, unsigned int *seed, long long int *number_in_circle);
#endif