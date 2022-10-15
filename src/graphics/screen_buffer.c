#include <pspdisplay.h>

#include "screen_buffer.h"

#define DISPLAY_BUFFER_ADDRESS 0x4000000
#define DRAW_BUFFER_ADDRESS 0x4300000

Color *display_buffer;
bool_t screen_buffer_initialized = FALSE;

void screen_buffer_init()
{
    if (screen_buffer_initialized)
    {
        return;
    }

    screen_buffer_initialized = TRUE;
    display_buffer = (Color *)DISPLAY_BUFFER_ADDRESS;
    draw_buffer = (Color *)DRAW_BUFFER_ADDRESS;

    sceDisplaySetMode(0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceDisplaySetFrameBuf(display_buffer, SCREEN_BUFFER_WIDTH,
                          PSP_DISPLAY_PIXEL_FORMAT_8888,
                          PSP_DISPLAY_SETBUF_IMMEDIATE);
}

void screen_buffer_paint(size_t buffer_idx, const Color *color)
{
    Color *in_buffer = draw_buffer + buffer_idx;
    Color blend = color_interpolate(in_buffer, color, color->alpha);

    in_buffer->red = blend.red;
    in_buffer->green = blend.green;
    in_buffer->blue = blend.blue;
    in_buffer->alpha = color->alpha;
}

void screen_buffer_clear(const Color *color)
{
    for (size_t i = 0; i < SCREEN_BUFFER_SIZE; ++i)
    {
        draw_buffer[i] = *color;
    }
}

void screen_buffer_swap()
{
    Color *tmp = display_buffer;
    display_buffer = draw_buffer;
    draw_buffer = tmp;

    sceDisplaySetFrameBuf(display_buffer, SCREEN_BUFFER_WIDTH,
                          PSP_DISPLAY_PIXEL_FORMAT_8888,
                          PSP_DISPLAY_SETBUF_NEXTFRAME);
}
