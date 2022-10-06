#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"
#include "sprite.h"

typedef struct GameState_st
{
    float delta;
    Vec2 camera_focus;
    Vec2 camera_half_extension;

    size_t n_sprites;
    Sprite *loaded_sprites;
} GameState;

void game_state_init(GameState *game_state);
void game_state_update(GameState *game_state, float delta);

#endif
