#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>

bool Interval_intersects(double min_a, double max_a, double min_b, double max_b);
bool Interval_contains(double min_a, double max_a, double min_b, double max_b);

#endif