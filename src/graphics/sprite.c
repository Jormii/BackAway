#include <stdlib.h>

#include <pspiofilemgr.h>

#include "path.h"
#include "macros.h"
#include "sprite.h"
#include "screen_buffer.h"

bool_t sprite_load(Sprite *sprite, const char *path)
{
    int fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
    if (fd < 0)
    {
        sprite->meta.width = 0;
        sprite->meta.height = 0;
        return FALSE;
    }

    // Read first two ints to get width and height
    sceIoRead(fd, &(sprite->meta), 2 * sizeof(u32));

    // Allocate and read
    // TODO: Consecutive in memory
    size_t bitmap_size = (sprite->meta.width * sprite->meta.height) * sizeof(Color);
    sprite->bitmap = malloc(bitmap_size);
    sceIoRead(fd, sprite->bitmap, bitmap_size);

    sceIoClose(fd);
    return TRUE;
}

void sprite_draw(const Sprite *sprite, int x, int y)
{
    const SpriteMeta *meta = &(sprite->meta);
    int x0 = MAX(0, x);
    int xf = MIN(x + meta->width, SCREEN_WIDTH);
    int y0 = MAX(0, y);
    int yf = MIN(y + meta->height, SCREEN_HEIGHT);

    size_t bitmap_x_offset = x0 - x;
    size_t bitmap_y_offset = y0 - y;
    for (int py = y0; py < yf; ++py)
    {
        size_t bitmap_idx = bitmap_x_offset + bitmap_y_offset * meta->width;
        size_t drawbuffer_idx = SCREEN_BUFFER_INDEX(x0, py);

        for (int px = x0; px < xf; ++px)
        {
            const Color *bitmap_pixel = sprite->bitmap + bitmap_idx;
            Color *screen_pixel = draw_buffer + drawbuffer_idx;
            *screen_pixel = *bitmap_pixel;

            bitmap_idx += 1;
            drawbuffer_idx += 1;
        }

        bitmap_y_offset += 1;
    }
}
