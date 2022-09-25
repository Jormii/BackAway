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

bool_t rect_contains_point(const Rect *rect, const Vec2 *p);
bool_t rect_within_rect(const Rect *r1, const Rect *r2);

#endif
