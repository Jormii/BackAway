#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"
#include "sprite.h"
#include "physics.h"
#include "ui_button.h"

typedef enum GameStateID_en
{
    GAME_STATE_LOAD,
    GAME_STATE_MAIN_MENU,
    GAME_STATE_LEVEL,
    _GAME_STATE_N_STATES_
} GameStateID;

struct GameState_st;
typedef void (*GameStateInit_fp)(struct GameState_st *game_state);
typedef void (*GameStateUpdate_fp)(struct GameState_st *game_state);
typedef void (*GameStateDraw_fp)(const struct GameState_st *game_state);

typedef struct GameStateCallbacks_st
{
    GameStateInit_fp init_cb;
    GameStateUpdate_fp update_cb;
    GameStateDraw_fp draw_cb;
} GameStateCallbacks;

struct Level_st;
struct Player_st;
typedef struct GameState_st
{
    // -- Shared across states --
    float delta;
    GameStateID state_id;
    Vec2 camera_focus;
    Vec2 camera_half_extension;

    // -- GAME_STATE_MAIN_MENU --
    UIButtonCollection button_collection;

    // -- GAME_STATE_LEVEL --
    bool_t slow_motion;
    struct Level_st *level;
    struct Player_st *player;
} GameState;

void game_state_init(GameState *game_state, GameStateID initial_state);
void game_state_update(GameState *game_state, float delta);

Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position);

#endif
