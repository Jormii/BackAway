#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"
#include "assets.h"
#include "sprite.h"
#include "physics.h"
#include "ui_button.h"

typedef enum GameStateID_en
{
    GAME_STATE_LOAD,
    GAME_STATE_MENU,
    GAME_STATE_LEVEL,
    _GAME_STATE_N_STATES_
} GameStateID;

struct GameState_st;
typedef void (*GameStateInit_fp)(struct GameState_st *game_state);
typedef void (*GameStateUpdate_fp)(struct GameState_st *game_state); // Returns if frame should be skipped
typedef void (*GameStateDraw_fp)(const struct GameState_st *game_state);

typedef struct GameStateCallbacks_st
{
    GameStateInit_fp init_cb;
    GameStateUpdate_fp update_cb;
    GameStateDraw_fp draw_cb;
} GameStateCallbacks;

struct Level_st;
struct Player_st;
struct MenuButtonData_st;
typedef struct GameState_st
{
    // -- Shared across states --
    float delta;
    bool_t skip_frame;
    GameStateID state_id;
    Vec2 camera_focus;
    Vec2 camera_half_extension;

    // -- GAME_STATE_MENU --
    UIButtonCollection buttons;
    struct MenuButtonData_st *buttons_data;

    // -- GAME_STATE_LEVEL --
    bool_t paused;
    bool_t slow_motion;
    bool_t restart_level;
    LevelID level_id;
    struct Level_st *level;
    struct Player_st *player;
} GameState;

void game_state_init(GameState *game_state, GameStateID initial_state);
void game_state_update(GameState *game_state, float delta);

Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position);

#endif
