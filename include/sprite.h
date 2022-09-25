#ifndef SPRITE_H
#define SPRITE_H

#include "vec2.h"
#include "types.h"

typedef struct Sprite_st
{
    int width;
    int height;
    rgb_t *bitmap;
} Sprite;

void sprite_create(int width, int height, Sprite *dst_sprite);
void sprite_draw(int x, int y, const Sprite *sprite);

#endif
