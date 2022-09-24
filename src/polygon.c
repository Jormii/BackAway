#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "macros.h"
#include "polygon.h"
#include "screen_buffer.h"

void draw_line(int x0, int y0, int xf, int yf, rgb_t color);
void draw_vertical_line(int x, int y0, int yf, rgb_t color);

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

void polygon_draw(const Polygon *polygon, rgb_t color)
{
    rect_draw(&(polygon->bbox), 0x00AAFFAA);

    for (size_t i = 0; i < polygon->n_vertices; ++i)
    {
        const Vec2 *p0 = polygon->vertices + i;
        const Vec2 *pf = polygon->vertices + ((i + 1) % polygon->n_vertices);
        draw_line(p0->x, p0->y, pf->x, pf->y, color);

        Vec2 pm_0 = {
            .x = (p0->x + pf->x) / 2,
            .y = (p0->y + pf->y) / 2};
        Vec2 pm_f = {
            .x = pm_0.x + 10 * polygon->normals[i].x,
            .y = pm_0.y + 10 * polygon->normals[i].y};

        draw_line(pm_0.x, pm_0.y, pm_f.x, pm_f.y, 0x00CCCCCC);
    }
}

void draw_line(int x0, int y0, int xf, int yf, rgb_t color)
{
    if (x0 > xf)
    {
        draw_line(xf, yf, x0, y0, color);
    }
    else if (x0 == xf)
    {
        draw_vertical_line(x0, y0, yf, color);
    }
    else
    {
        int dx = xf - x0;
        int dy = yf - y0;

        x0 = MAX(x0, 0);
        xf = MIN(xf, SCREEN_WIDTH);
        for (int x = x0; x <= xf; ++x)
        {
            int y = y0 + dy * (x - x0) / dx;

            size_t idx = SCREEN_BUFFER_INDEX(x, y);
            draw_buffer[idx] = color;
        }
    }
}

void draw_vertical_line(int x, int y0, int yf, rgb_t color)
{
    int _y0 = MIN(y0, yf);
    int _yf = MAX(y0, yf);

    _y0 = MAX(_y0, 0);
    _yf = MIN(_yf, SCREEN_HEIGHT);
    for (int y = _y0; y <= _yf; ++y)
    {
        size_t idx = SCREEN_BUFFER_INDEX(x, y);
        draw_buffer[idx] = color;
    }
}