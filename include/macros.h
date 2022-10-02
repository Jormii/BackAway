#ifndef MACROS_H
#define MACROS_H

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(x, min, max) (MIN((max), MAX((x), (min))))

#endif
