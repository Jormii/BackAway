#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "macros.h"
#include "polygon.h"
#include "screen_buffer.h"

void polygon_create(const Vec2 *vertices, size_t n_vertices, Polygon *out_polygon)
{
    assert(n_vertices >= 3);

    // Allocate
    out_polygon->n_vertices = n_vertices;
    out_polygon->vertices = malloc(n_vertices * sizeof(Vec2));
    out_polygon->normals = malloc(n_vertices * sizeof(Vec2));

    // Compute fields
    Vec2 min_corner = {.x = INFINITY, .y = INFINITY};
    Vec2 max_corner = {.x = -INFINITY, .y = -INFINITY};
    for (size_t i = 0; i < n_vertices; ++i)
    {
        const Vec2 *p0 = vertices + i;
        const Vec2 *pf = vertices + ((i + 1) % n_vertices);

        // Vertices
        out_polygon->vertices[i] = *p0;

        // Normals
        Vec2 *normal = out_polygon->normals + i;
        normal->x = pf->y - p0->y;
        normal->y = -(pf->x - p0->x);
        vec2_normalize(normal);

        // Bbox
        min_corner.x = MIN(p0->x, min_corner.x);
        min_corner.y = MIN(p0->y, min_corner.y);
        max_corner.x = MAX(p0->x, max_corner.x);
        max_corner.y = MAX(p0->y, max_corner.y);
    }

    // Last calculations
    out_polygon->bbox.origin = min_corner;
    out_polygon->bbox.width = max_corner.x - min_corner.x;
    out_polygon->bbox.height = max_corner.y - min_corner.y;
}

void polygon_from_rect(const Rect *rect, Polygon *out_polygon)
{
    Vec2 corners[4];
    for (size_t i = 0; i < 4; ++i)
    {
        corners[i] = rect->origin;
    }

    corners[1].x += rect->width;
    corners[2].x += rect->width;
    corners[2].y += rect->height;
    corners[3].y += rect->height;

    polygon_create(corners, 4, out_polygon);
}
