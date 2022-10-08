#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"
#include "sprite.h"
#include "physics.h"

struct Level_st;
struct Player_st;
typedef struct GameState_st
{
    float delta;
    Vec2 camera_focus;
    Vec2 camera_half_extension;

    struct Level_st *level;
    struct Player_st *player;
} GameState;

void game_state_init(GameState *game_state);
void game_state_update(GameState *game_state, float delta);

void game_state_load_level(GameState *game_state, struct Level_st *level);
Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position);

#endif
