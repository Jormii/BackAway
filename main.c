#include <time.h>

#include <pspkernel.h>
#include <pspdisplay.h>

#include "input.h"
#include "callbacks.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("BackAway", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main()
{
    setup_callbacks();
    input_init();
    screen_buffer_init();

    clock_t frame_start = clock();
    clock_t frame_end = clock();
    Color clear_color = {.red = 255, .green = 128, .blue = 128, .alpha = 0};
    while (running())
    {
        clock_t ellapsed = frame_end - frame_start;
        float delta = ((float)ellapsed) / CLOCKS_PER_SEC;
        frame_start = clock();

        screen_buffer_clear(&clear_color);
        input_update();

        screen_buffer_swap();
        sceDisplayWaitVblankStart();

        frame_end = clock();
    }

    sceKernelExitGame();
    return 0;
}
