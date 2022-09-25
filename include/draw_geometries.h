#ifndef DRAW_GEOMETRIES_H
#define DRAW_GEOMETRIES_H

#include "vec2.h"
#include "types.h"
#include "polygon.h"

void draw_line(const Vec2 *p, const Vec2 *q, rgb_t color);
void draw_rect(const Rect *rect, rgb_t color);
void draw_polygon(const Polygon *polygon, rgb_t color);

#endif