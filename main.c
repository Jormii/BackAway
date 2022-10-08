#include <time.h>

#include <pspkernel.h>
#include <pspdisplay.h>
#include <oslib/oslib.h>

#include "input.h"
#include "callbacks.h"
#include "game_state.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("BackAway", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main()
{
    setup_callbacks();
    input_init();
    screen_buffer_init();
    oslInit(0);
    oslInitAudioME(OSL_FMT_MP3);

    GameState game_state;
    game_state_init(&game_state, GAME_STATE_LEVEL);

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
        game_state_update(&game_state, delta);
        screen_buffer_swap();
        sceDisplayWaitVblankStart();
        oslAudioVSync();

        frame_end = clock();
    }

    sceKernelExitGame();
    return 0;
}
