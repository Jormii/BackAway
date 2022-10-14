#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "timer.h"
#include "types.h"
#include "sprite.h"

typedef struct AnimatedSprite_st
{
    size_t n_frames;
    const Sprite **frames;
    size_t rendered_frame_idx;
    Timer frame_tick;
} AnimatedSprite;

void animated_sprite_init(AnimatedSprite *anim_sprite, size_t n_frames, int fps);
void animated_sprite_update(AnimatedSprite *anim_sprite, float delta);

const Sprite *animated_sprite_get(const AnimatedSprite *anim_sprite);

#endif
