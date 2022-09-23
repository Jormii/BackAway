#ifndef RECT_H
#define RECT_H

#include "vec2.h"
#include "types.h"

typedef struct Rect_st
{
    Vec2 origin;
    int width;
    int height;
} Rect;

bool_t rect_within_rect(const Rect *r1, const Rect *r2);

void rect_draw(const Rect *rect, rgb_t color);

#endif