#include <math.h>
#include <stdlib.h>

#include "macros.h"
#include "polygon.h"

void polygon_init(Polygon *polygon, const Vec2 *vertices, size_t n_vertices)
{
    polygon->n_vertices = n_vertices;
    polygon->vertices = malloc(n_vertices * sizeof(Vec2));
    polygon->tangents = malloc(n_vertices * sizeof(Vec2));
    polygon->normals = malloc(n_vertices * sizeof(Vec2));

    Vec2 min_corner = {.x = INFINITY, .y = INFINITY};
    Vec2 max_corner = {.x = -INFINITY, .y = -INFINITY};
    for (size_t i = 0; i < n_vertices; ++i)
    {
        const Vec2 *p0 = vertices + i;
        const Vec2 *pf = vertices + ((i + 1) % n_vertices);

        // Vertices and edges
        Vec2 *vertex = polygon->vertices + i;
        Vec2 *tangent = polygon->tangents + i;
        Vec2 *normal = polygon->normals + i;

        *vertex = *p0;
        *tangent = vec2_subtract(pf, p0);
        vec2_normalize(tangent);

        normal->x = tangent->y;
        normal->y = -tangent->x;

        // BBOX
        min_corner.x = MIN(p0->x, min_corner.x);
        min_corner.y = MIN(p0->y, min_corner.y);
        max_corner.x = MAX(p0->x, max_corner.x);
        max_corner.y = MAX(p0->y, max_corner.y);
    }

    // Update BBOX
    Rect *bbox = &(polygon->bbox);
    bbox->origin = min_corner;
    bbox->width = max_corner.x - min_corner.x;
    bbox->height = max_corner.y - min_corner.y;
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
