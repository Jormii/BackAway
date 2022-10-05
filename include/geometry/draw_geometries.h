#ifndef DRAW_GEOMETRIES_H
#define DRAW_GEOMETRIES_H

#include "vec2.h"
#include "rect.h"
#include "types.h"
#include "polygon.h"

void draw_point(const Vec2 *point, const Color *color);
void draw_line(const Vec2 *p, const Vec2 *q, const Color *color);
void draw_rect(const Rect *rect, const Color *color);
void draw_polygon(const Polygon *polygon, const Color *edge_color, const Color *normal_color);

#endif
