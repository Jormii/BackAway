#include <pspkernel.h>

#include "callbacks.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("BackAway", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main()
{
    setup_callbacks();
    screen_buffer_init();

    while (running())
    {
        screen_buffer_clear(0x00444444);

        int x = 0;
        int y = 0;
        int width = 10;
        int height = 10;
        for (int _y = 0; _y < (y + height); ++_y)
        {
            for (int _x = 0; _x < (x + width); ++_x)
            {
                size_t idx = SCREEN_BUFFER_INDEX(_x, _y);
                draw_buffer[idx] = 0x00FFFFFF;
            }
        }

        screen_buffer_swap();
    }

    sceKernelExitGame();
    return 0;
}