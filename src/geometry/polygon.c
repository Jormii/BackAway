#include <math.h>
#include <stdlib.h>

#include "macros.h"
#include "polygon.h"

void polygon_init(Polygon *polygon, const Vec2 *vertices, size_t n_vertices)
{
    polygon->n_vertices = n_vertices;
    polygon->vertices = malloc(n_vertices * sizeof(Vec2));
    polygon->normals = malloc(n_vertices * sizeof(Vec2));
    polygon->vertices_normals = malloc(n_vertices * sizeof(Vec2));

    Vec2 min_corner = {.x = INFINITY, .y = INFINITY};
    Vec2 max_corner = {.x = -INFINITY, .y = -INFINITY};
    for (size_t i = 0; i < n_vertices; ++i)
    {
        const Vec2 *p0 = vertices + i;
        const Vec2 *pf = vertices + ((i + 1) % n_vertices);
        Vec2 v = vec2_subtract(pf, p0);

        // Vertex and normal
        Vec2 *vertex = polygon->vertices + i;
        Vec2 *normal = polygon->normals + i;

        *vertex = *p0;
        normal->x = v.y;
        normal->y = -v.x;
        vec2_normalize(normal);

        // BBOX
        min_corner.x = MIN(p0->x, min_corner.x);
        min_corner.y = MIN(p0->y, min_corner.y);
        max_corner.x = MAX(p0->x, max_corner.x);
        max_corner.y = MAX(p0->y, max_corner.y);
    }

    // Vertices normals
    for (size_t i = 0; i < n_vertices; ++i)
    {
        size_t prev_i = (i == 0) ? (n_vertices - 1) : (i - 1);

        const Vec2 *normal = polygon->normals + i;
        const Vec2 *prev_normal = polygon->normals + prev_i;
        polygon->vertices_normals[i] = vec2_add(normal, prev_normal);
        vec2_normalize(polygon->vertices_normals + i);
    }

    // BBOX
    Rect *bbox = &(polygon->bbox);
    bbox->origin = min_corner;
    bbox->width = max_corner.x - min_corner.x;
    bbox->height = max_corner.y - min_corner.y;
}

void polygon_free(Polygon *polygon)
{
    free(polygon->vertices);
    free(polygon->normals);
    free(polygon->vertices_normals);
}

void polygon_place_at(Polygon *polygon, const Vec2 *position)
{
    Vec2 v = vec2_subtract(position, polygon->vertices);
    polygon_move(polygon, &v);
}

void polygon_move(Polygon *polygon, const Vec2 *movement)
{
    for (size_t i = 0; i < polygon->n_vertices; ++i)
    {
        Vec2 *vertex = polygon->vertices + i;
        *vertex = vec2_add(vertex, movement);
    }

    polygon->bbox.origin = vec2_add(&(polygon->bbox.origin), movement);
}

void polygon_from_rect(Polygon *polygon, const Rect *rect)
{
    Vec2 vertices[] = {
        rect->origin,
        rect->origin,
        rect->origin,
        rect->origin};

    vertices[1].x += rect->width;
    vertices[2].x += rect->width;
    vertices[2].y += rect->height;
    vertices[3].y += rect->height;

    polygon_init(polygon, vertices, 4);
}
