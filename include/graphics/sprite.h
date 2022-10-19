#ifndef SPRITE_H
#define SPRITE_H

#include "color.h"

typedef struct SpriteMeta_st
{
    int width;
    int height;
} SpriteMeta;

typedef struct Sprite_st
{
    SpriteMeta meta;
    Color *bitmap;
} Sprite;

bool_t sprite_load(Sprite *sprite, const char *path);
void sprite_draw(const Sprite *sprite, int x, int y, float alpha_factor, bool_t flip_x, bool_t flip_y);

#endif
