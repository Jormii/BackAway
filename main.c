#include <pspkernel.h>
#include <pspdisplay.h>

#include "time.h"
#include "callbacks.h"
#include "game_state.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("BackAway", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main()
{
    setup_callbacks();
    screen_buffer_init();
    game_state_initialize();

    clock_t frame_start = clock();
    clock_t frame_end = clock();
    while (running())
    {
        clock_t ellapsed = frame_end - frame_start;
        float delta = ((float)ellapsed) / CLOCKS_PER_SEC;
        frame_start = clock();

        screen_buffer_clear(0x00444444);
        game_state_update(delta);
        screen_buffer_swap();
        sceDisplayWaitVblankStart();

        frame_end = clock();
    }

    sceKernelExitGame();
    return 0;
}