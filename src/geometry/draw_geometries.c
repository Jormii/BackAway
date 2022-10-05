#include "macros.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

void draw_vertical_line(int x, int y0, int yf, const Color *color);

void draw_point(const Vec2 *point, const Color *color)
{
    if (point->x >= 0 && point->x < SCREEN_WIDTH && point->y >= 0 && point->y < SCREEN_HEIGHT)
    {
        size_t idx = SCREEN_BUFFER_INDEX((int)point->x, (int)point->y);
        draw_buffer[idx] = *color;
    }
}

void draw_line(const Vec2 *p, const Vec2 *q, const Color *color)
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
                draw_buffer[idx] = *color;
            }
        }
    }
}

void draw_vertical_line(int x, int y0, int yf, const Color *color)
{
    if (x < 0 || x >= SCREEN_WIDTH)
    {
        return;
    }

    int _y0 = MIN(y0, yf);
    int _yf = MAX(y0, yf);

    _y0 = MAX(_y0, 0);
    _yf = MIN(_yf, SCREEN_HEIGHT);
    for (int y = _y0; y <= _yf; ++y)
    {
        size_t idx = SCREEN_BUFFER_INDEX(x, y);
        draw_buffer[idx] = *color;
    }
}

void draw_rect(const Rect *rect, const Color *color)
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
