#include <stdlib.h>

#include "macros.h"
#include "sprite.h"
#include "screen_buffer.h"

Sprite *sprite_allocate(int width, int height)
{
    Sprite tmp_sprite;
    void *start = &tmp_sprite;
    void *end = &(tmp_sprite.bitmap);
    size_t offset = (size_t)(end) - (size_t)(start);

    size_t bitmap_dimensions = width * height;
    size_t bytes = offset + sizeof(tmp_sprite.bitmap) * bitmap_dimensions;
    Sprite *sprite = malloc(bytes);

    sprite->origin.x = 0;
    sprite->origin.y = 0;
    sprite->width = width;
    sprite->height = height;
    sprite->bitmap = (rgb_t *)(sprite + offset);

    return sprite;
}

void sprite_draw(const Sprite *sprite)
{
    int x = sprite->origin.x;
    int y = sprite->origin.y;
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