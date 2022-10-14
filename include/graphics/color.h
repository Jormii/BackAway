#ifndef COLOR_H
#define COLOR_H

#include "types.h"

typedef struct Color_st
{
    u8_t red;
    u8_t green;
    u8_t blue;
    u8_t alpha;
} Color;

Color color_interpolate(const Color *c1, const Color *c2, float t);

#endif
