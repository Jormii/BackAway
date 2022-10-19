#include <stdlib.h>

#include "animated_sprite.h"

void animated_sprite_on_tick(Timer *timer);

void animated_sprite_init(AnimatedSprite *anim_sprite, size_t n_frames, int fps)
{
    anim_sprite->n_frames = n_frames;
    anim_sprite->frames = malloc(n_frames * sizeof(Sprite *));
    anim_sprite->rendered_frame_idx = 0;

    anim_sprite->frame_tick.cycle = TRUE;
    anim_sprite->frame_tick.countdown = 1.0f / (float)fps;
    anim_sprite->frame_tick.trigger_cb = animated_sprite_on_tick;
    anim_sprite->frame_tick.trigger_cb_ptr = anim_sprite;
    timer_start(&anim_sprite->frame_tick);
}

void animated_sprite_free(AnimatedSprite *anim_sprite)
{
    free(anim_sprite->frames);
}

void animated_sprite_update(AnimatedSprite *anim_sprite, float delta)
{
    timer_update(&(anim_sprite->frame_tick), delta);
}

const Sprite *animated_sprite_get(const AnimatedSprite *anim_sprite)
{
    return anim_sprite->frames[anim_sprite->rendered_frame_idx];
}

void animated_sprite_on_tick(Timer *timer)
{
    AnimatedSprite *anim_sprite = (AnimatedSprite *)(timer->trigger_cb_ptr);
    size_t *idx = &(anim_sprite->rendered_frame_idx);
    *idx = (*idx + 1) % anim_sprite->n_frames;
}
