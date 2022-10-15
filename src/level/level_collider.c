#include <math.h>
#include <stdlib.h>

#include "color.h"
#include "macros.h"
#include "screen_buffer.h"
#include "level_collider.h"
#include "draw_geometries.h"

float random_zero_to_one();
void level_collider_draw_ephemeral(const Polygon *collider);
void level_collider_draw_non_ephemeral(const Polygon *collider);

void level_collider_draw(const Polygon *collider)
{
    if (collider->ephemeral)
    {
        level_collider_draw_ephemeral(collider);
    }
    else
    {
        level_collider_draw_non_ephemeral(collider);
    }
}

float random_zero_to_one()
{
    return rand() / (float)RAND_MAX;
}

void level_collider_draw_ephemeral(const Polygon *collider)
{
    // TODO: Slow
    const Rect *bbox = &(collider->bbox);

    int x0 = MAX(0, bbox->origin.x);
    int y0 = MAX(0, bbox->origin.y);
    int xf = MIN(bbox->origin.x + bbox->width, SCREEN_WIDTH);
    int yf = MIN(bbox->origin.y + bbox->height, SCREEN_HEIGHT);
    for (int py = y0; py < yf; ++py)
    {
        float r = random_zero_to_one();
        float f = 0.5f * (r + 1);
        f = f * f;

        if (f > (SCREEN_HEIGHT / bbox->height))
        {
            continue;
        }

        int n = (int)(255.0f * r);
        n = CLAMP(n, 0, 255);
        Color color = {n, n, n, 255};

        size_t draw_buffer_index = SCREEN_BUFFER_INDEX(x0, py);
        for (int px = x0; px < xf; ++px)
        {
            screen_buffer_paint(draw_buffer_index, &color);
            draw_buffer_index += 1;
        }
    }

    Color black = {0, 0, 0, 255};
    draw_rect(bbox, &black);
}

void level_collider_draw_non_ephemeral(const Polygon *collider)
{
    int x_prime = 17;
    int y_prime = 13;
    Color base = {0, 0, 0, 255};
    Color highlight = {255, 255, 255, 255};
    const Rect *bbox = &(collider->bbox);

    int x0 = MAX(0, bbox->origin.x);
    int y0 = MAX(0, bbox->origin.y);
    int xf = MIN(bbox->origin.x + bbox->width, SCREEN_WIDTH);
    int yf = MIN(bbox->origin.y + bbox->height, SCREEN_HEIGHT);
    for (int py = y0; py < yf; ++py)
    {
        bool_t row_draw = (py % y_prime) == 0;
        size_t draw_buffer_index = SCREEN_BUFFER_INDEX(x0, py);
        for (int px = x0; px < xf; ++px)
        {
            bool_t pixel_draw = row_draw && (px % x_prime) == 0;
            const Color *color = (pixel_draw) ? &highlight : &base;
            screen_buffer_paint(draw_buffer_index, color);

            draw_buffer_index += 1;
        }
    }

    Color black = {0, 0, 0, 255};
    draw_rect(&(collider->bbox), &black);
}
