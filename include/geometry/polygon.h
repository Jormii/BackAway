#ifndef POLYGON_H
#define POLYGON_H

#include "rect.h"
#include "vec2.h"

typedef struct Polygon_st
{
    size_t n_vertices;
    Vec2 *vertices;
    Vec2 *normals;
    Vec2 *vertices_normals;
    Rect bbox;
} Polygon;

void polygon_init(Polygon *polygon, const Vec2 *vertices, size_t n_vertices);

void polygon_move(Polygon *polygon, const Vec2 *movement);
void polygon_from_rect(Polygon *polygon, const Rect *rect);

#endif
