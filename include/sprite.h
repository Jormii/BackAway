#ifndef SPRITE_H
#define SPRITE_H

#include "vec2.h"
#include "types.h"

typedef struct Sprite_st
{
    Vec2 origin;
    int width;
    int height;

    rgb_t *bitmap;
} Sprite;

Sprite *sprite_allocate(int width, int height);
void sprite_draw(const Sprite *sprite);

#endif