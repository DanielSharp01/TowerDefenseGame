#include "Interval.h"

bool Interval_intersects(double min_a, double max_a, double min_b, double max_b)
{
    return (min_b <= max_a && min_a <= min_b) || (min_a <= max_b && min_b <= min_a);
}

bool Interval_contains(double min_a, double max_a, double min_b, double max_b)
{
    return min_a <= min_b && max_a >= max_b;
}