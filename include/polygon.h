#ifndef POLYGON_H
#define POLYGON_H

#include "vec2.h"
#include "types.h"

typedef struct Polygon_st
{
    size_t n_vertices;
    Vec2 *vertices; // Clockwise order
    Vec2 *normals;
} Polygon;

Polygon *polygon_allocate(size_t n_vertices);
void polygon_compute_normals(Polygon *polygon);

void polygon_draw(const Polygon *polygon, rgb_t color);

#endif