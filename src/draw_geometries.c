#include "macros.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

void draw_vertical_line(int x, int y0, int yf, rgb_t color);

void draw_line(const Vec2 *p, const Vec2 *q, rgb_t color)
{
    if (p->x > q->x)
    {
        draw_line(q, p, color);
    }
    else if (p->x == q->x)
    {
        draw_vertical_line(p->x, p->y, q->y, color);
    }
    else
    {
        int dx = q->x - p->x;
        int dy = q->y - p->y;

        int x0 = MAX(p->x, 0);
        int xf = MIN(q->x, SCREEN_WIDTH);
        for (int x = x0; x <= xf; ++x)
        {
            int y = p->y + dy * (x - x0) / dx;
            if (y >= 0 && y < SCREEN_HEIGHT)
            {
                size_t idx = SCREEN_BUFFER_INDEX(x, y);
                draw_buffer[idx] = color;
            }
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

void draw_rect(const Rect *rect, rgb_t color)
{
    Vec2 c1 = rect->origin;
    Vec2 c2 = {
        .x = rect->origin.x + rect->width,
        .y = rect->origin.y};
    Vec2 c3 = {
        .x = rect->origin.x + rect->width,
        .y = rect->origin.y + rect->height};
    Vec2 c4 = {
        .x = rect->origin.x,
        .y = rect->origin.y + rect->height};

    draw_line(&c1, &c2, color);
    draw_line(&c2, &c3, color);
    draw_line(&c3, &c4, color);
    draw_line(&c4, &c1, color);
}

void draw_polygon(const Polygon *polygon, rgb_t color)
{
    for (size_t i = 0; i < polygon->n_vertices; ++i)
    {
        const Vec2 *p0 = polygon->vertices + i;
        const Vec2 *pf = polygon->vertices + ((i + 1) % polygon->n_vertices);
        draw_line(p0, pf, color);

        Vec2 pm_0 = {
            .x = (p0->x + pf->x) / 2,
            .y = (p0->y + pf->y) / 2};
        Vec2 pm_f = {
            .x = pm_0.x + 10 * polygon->normals[i].x,
            .y = pm_0.y + 10 * polygon->normals[i].y};

        int r = 255 * ((1 + polygon->normals[i].x) / 2);
        int g = 255 * ((1 + polygon->normals[i].y) / 2);
        draw_line(&pm_0, &pm_f, RGB(r, g, 0));
    }
}
