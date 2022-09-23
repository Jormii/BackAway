#include <pspkernel.h>

#include "sprite.h"
#include "callbacks.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("BackAway", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main()
{
    setup_callbacks();
    screen_buffer_init();

    Sprite *sprite = sprite_allocate(30, 30);
    size_t idx = 0;
    for (int y = 0; y < sprite->height; ++y)
    {
        for (int x = 0; x < sprite->width; ++x)
        {
            rgb_t color = 0;
            switch (y % 3)
            {
            case 0:
                color = 0x000000FF;
                break;
            case 1:
                color = 0x0000FF00;
                break;
            case 2:
                color = 0x00FF0000;
            default:
                break;
            }

            sprite->bitmap[idx] = color;
            ++idx;
        }
    }

    float *sx = &(sprite->origin.x);
    float *sy = &(sprite->origin.y);
    *sy = -15;

    while (running())
    {
        screen_buffer_clear(0x00444444);

        sprite_draw(sprite);

        *sx += 1;
        if ((*sx) >= SCREEN_WIDTH)
        {
            *sx = 0;
            *sy += 1;
        }

        if ((*sy) >= SCREEN_HEIGHT)
        {
            *sy = 0;
        }

        screen_buffer_swap();
    }

    sceKernelExitGame();
    return 0;
}