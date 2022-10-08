#ifndef MACROS_H
#define MACROS_H

#include <math.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(x, min, max) (MIN((max), MAX((x), (min))))
#define EQUAL_EPSILON(x, y, eps) (fabs((x - (y))) <= (eps))

#endif
