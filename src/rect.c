#include "rect.h"
#include "macros.h"
#include "screen_buffer.h"

bool_t rect_within_rect(const Rect *r1, const Rect *r2)
{
    return r1->origin.x + r1->width >= r2->origin.x &&  // r1 right edge past r2 left
           r1->origin.x <= r2->origin.x + r2->width &&  // r1 left edge past r2 right
           r1->origin.y + r1->height >= r2->origin.y && // r1 top edge past r2 bottom
           r1->origin.y <= r2->origin.y + r2->height;   // r1 bottom edge past r2 top
}

void rect_draw(const Rect *rect, rgb_t color)
{
    int x = rect->origin.x - 1;
    int x_end = rect->origin.x + rect->width;
    int y = rect->origin.y - 1;
    int y_end = rect->origin.y + rect->height;

    int px = MAX(x, 0);
    int px_end = MIN(x_end, SCREEN_WIDTH);
    if (y >= 0 && y < SCREEN_HEIGHT)
    {
        for (int xx = px; xx < px_end; ++xx)
        {
            draw_buffer[SCREEN_BUFFER_INDEX(xx, y)] = color;
        }
    }
    if (y_end >= 0 && y_end < SCREEN_HEIGHT)
    {
        for (int xx = px; xx < px_end; ++xx)
        {
            draw_buffer[SCREEN_BUFFER_INDEX(xx, y_end)] = color;
        }
    }

    int py = MAX(y, 0);
    int py_end = MIN(y_end, SCREEN_HEIGHT);
    if (x >= 0 && x < SCREEN_WIDTH)
    {
        for (int yy = py; yy < py_end; ++yy)
        {
            draw_buffer[SCREEN_BUFFER_INDEX(x, yy)] = color;
        }
    }
    if (x_end >= 0 && x_end < SCREEN_WIDTH)
    {
        for (int yy = py; yy < py_end; ++yy)
        {
            draw_buffer[SCREEN_BUFFER_INDEX(x_end, yy)] = color;
        }
    }
}