#include <stdlib.h>

#include "macros.h"
#include "polygon.h"
#include "screen_buffer.h"

void draw_line(int x0, int y0, int xf, int yf, rgb_t color);
void draw_vertical_line(int x, int y0, int yf, rgb_t color);

Polygon *polygon_allocate(size_t n_vertices)
{
    Polygon *polygon = malloc(sizeof(Polygon));
    polygon->n_vertices = n_vertices;
    polygon->vertices = malloc(n_vertices * sizeof(Vec2));
    polygon->normals = malloc(n_vertices * sizeof(Vec2));

    return polygon;
}

void polygon_compute_normals(Polygon *polygon)
{
    for (size_t i = 0; i < polygon->n_vertices; ++i)
    {
        const Vec2 *p0 = polygon->vertices + i;
        const Vec2 *pf = polygon->vertices + ((i + 1) % polygon->n_vertices);

        Vec2 *normal = polygon->normals + i;
        normal->x = pf->y - p0->y;
        normal->y = -(pf->x - p0->x);
        vec2_normalize(normal);
    }
}

void polygon_draw(const Polygon *polygon, rgb_t color)
{
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

    for (int y = _y0; y <= _yf; ++y)
    {
        size_t idx = SCREEN_BUFFER_INDEX(x, y);
        draw_buffer[idx] = color;
    }
}