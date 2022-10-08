#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"
#include "sprite.h"
#include "physics.h"

typedef enum GameStateID_en
{
    GAME_STATE_MAIN_MENU,
    GAME_STATE_LEVEL,
    _GAME_STATE_N_STATES_
} GameStateID;

struct Level_st;
struct Player_st;
typedef struct GameState_st
{
    float delta;
    GameStateID state_id;
    Vec2 camera_focus;
    Vec2 camera_half_extension;

    struct Level_st *level;
    struct Player_st *player;
} GameState;

void game_state_init(GameState *game_state, GameStateID initial_state);
void game_state_update(GameState *game_state, float delta);

void game_state_load_level(GameState *game_state, struct Level_st *level);
void game_state_end_of_level(GameState *game_state);

Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position);

#endif
