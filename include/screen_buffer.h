#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H

#include "types.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define SCREEN_BUFFER_WIDTH 512
#define SCREEN_BUFFER_SIZE (SCREEN_BUFFER_WIDTH * SCREEN_HEIGHT)

#define SCREEN_BUFFER_INDEX(x, y) ((x) + (y)*SCREEN_BUFFER_WIDTH)

rgb_t *draw_buffer;

void screen_buffer_init();
void screen_buffer_clear(rgb_t color);
void screen_buffer_swap();

#endif
