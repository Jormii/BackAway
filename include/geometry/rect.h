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

Vec2 rect_center(const Rect *rect);
bool_t rect_contains_point(const Rect *rect, const Vec2 *p);
bool_t rect_within_rect(const Rect *r1, const Rect *r2);
void rect_given_center(Rect *rect, const Vec2 *center, int width, int height);

#endif
