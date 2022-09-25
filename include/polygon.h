#ifndef POLYGON_H
#define POLYGON_H

#include "vec2.h"
#include "rect.h"
#include "types.h"

typedef struct Polygon_st
{
    size_t n_vertices;
    Vec2 *vertices; // Clockwise order
    Vec2 *normals;
    Rect bbox;
} Polygon;

void polygon_create(const Vec2 *vertices, size_t n_vertices, Polygon *dst_polygon);
void polygon_from_rect(const Rect *rect, Polygon *dst_polygon);

#endif