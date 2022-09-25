#include <stdlib.h>

#include "macros.h"
#include "sprite.h"
#include "screen_buffer.h"

void sprite_create(int width, int height, Sprite *dst_sprite)
{
    dst_sprite->width = width;
    dst_sprite->height = height;
    dst_sprite->bitmap = malloc(width * height * sizeof(rgb_t));
}

void sprite_draw(int x, int y, const Sprite *sprite)
{
    int x_end = x + sprite->width;
    int y_end = y + sprite->height;

    size_t i = 0;
    for (int py = y; py < y_end; ++py)
    {
        if (py >= SCREEN_HEIGHT)
        {
            break;
        }
        else if (py >= 0)
        {
            for (int px = x; px < x_end; ++px)
            {
                if (px >= 0 && px < SCREEN_WIDTH)
                {
                    size_t idx = SCREEN_BUFFER_INDEX(px, py);
                    draw_buffer[idx] = sprite->bitmap[i];
                }

                ++i;
            }
        }
        else
        {
            i += sprite->width;
        }
    }
}