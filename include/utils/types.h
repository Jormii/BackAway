#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

#define FALSE 0
#define TRUE 1
#define RGB(r, g, b) ((r)&0x000000FF) + (((g)&0x000000FF) << 8) + (((b)&0x000000FF) << 16)

typedef uint8_t bool_t;
typedef uint8_t u8_t;

typedef uint32_t rgb_t;

#endif
