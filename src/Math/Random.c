#include "Random.h"

#include <stdlib.h>

int Random_int(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

double Random_double(double min, double max)
{
    return (double)rand() / RAND_MAX * (max - min) + min;
}