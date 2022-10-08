#include <stdlib.h>

#include "level.h"
#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"

typedef void (*GameStateUpdate_fp)(GameState *game_state);
typedef void (*GameStateDraw_fp)(const GameState *game_state);

typedef struct GameStateCallbacks_st
{
    GameStateUpdate_fp update_cb;
    GameStateDraw_fp draw_cb;
} GameStateCallbacks;

void menu_state_update(GameState *game_state);
void menu_state_draw(const GameState *game_state);

void level_state_update(GameState *game_state);
void level_state_draw(const GameState *game_state);

GameStateCallbacks state_cbs[_GAME_STATE_N_STATES_] = {
    {.update_cb = menu_state_update, .draw_cb = menu_state_draw},
    {.update_cb = level_state_update, .draw_cb = level_state_draw},
};

void game_state_init(GameState *game_state, GameStateID initial_state)
{
    game_state->delta = 0.0f;
    game_state->state_id = initial_state;
    game_state->camera_focus.x = 0.0f;
    game_state->camera_focus.y = 0.0f;
    game_state->camera_half_extension.x = 0.5 * SCREEN_WIDTH;
    game_state->camera_half_extension.y = 0.5 * SCREEN_HEIGHT;

    game_state->player = malloc(sizeof(Player));
    player_init(game_state->player);

    // TODO: Initialize level somewhere else
    game_state->level = malloc(sizeof(Level));

    Level *level = game_state->level;
    level->spawn_position.x = 0;
    level->spawn_position.y = 0;

    level->goal.active = FALSE;
    level->goal.boundary.origin.x = SCREEN_WIDTH - 100.0f;
    level->goal.boundary.origin.y = 0.0f;
    level->goal.boundary.width = 30.0f;
    level->goal.boundary.height = SCREEN_HEIGHT;

    level->n_objectives = 1;
    level->objectives = malloc(level->n_objectives * sizeof(LevelObjective));

    level->objectives->hit = FALSE;
    level->objectives->position.x = 100.0f;
    level->objectives->position.y = 100.0f;

    level->n_colliders = 1;
    level->colliders = malloc(level->n_colliders * sizeof(Polygon));

    Rect bbox = {
        .origin = {.x = SCREEN_WIDTH / 2 - 200, .y = 2.0f / 3.0f * SCREEN_HEIGHT},
        .width = 400.0f,
        .height = 30.0f};
    polygon_from_rect(level->colliders, &bbox);

    game_state_load_level(game_state, game_state->level);
    // END TODO
}

void game_state_update(GameState *game_state, float delta)
{
    game_state->delta = delta;

    GameStateCallbacks cbs = state_cbs[game_state->state_id];
    cbs.update_cb(game_state);
    cbs.draw_cb(game_state);
}

void game_state_load_level(GameState *game_state, struct Level_st *level)
{
    game_state->level = level;
    game_state->player->entity.position = level->spawn_position;
}

void game_state_end_of_level(GameState *game_state)
{
    // TODO
}

Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position)
{
    Vec2 origin = vec2_subtract(&(game_state->camera_focus), &(game_state->camera_half_extension));
    return vec2_subtract(position, &origin);
}

void menu_state_update(GameState *game_state)
{
}

void menu_state_draw(const GameState *game_state)
{
}

void level_state_update(GameState *game_state)
{
    level_update(game_state->level, game_state);
    player_update(game_state->player, game_state);
}

void level_state_draw(const GameState *game_state)
{
    level_draw(game_state->level, game_state);
    player_draw(game_state->player, game_state);
}
