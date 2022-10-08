#include <stdlib.h>

#include "level.h"
#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"

#include "state_menu.h"
#include "state_level.h"

GameStateCallbacks state_cbs[_GAME_STATE_N_STATES_] = {
    {.init_cb = menu_state_init, .update_cb = menu_state_update, .draw_cb = menu_state_draw},
    {.init_cb = level_state_init, .update_cb = level_state_update, .draw_cb = level_state_draw},
};

void game_state_init(GameState *game_state, GameStateID initial_state)
{
    game_state->delta = 0.0f;
    game_state->state_id = initial_state;
    game_state->camera_focus.x = 0.0f;
    game_state->camera_focus.y = 0.0f;
    game_state->camera_half_extension.x = 0.5 * SCREEN_WIDTH;
    game_state->camera_half_extension.y = 0.5 * SCREEN_HEIGHT;

    for (size_t i = 0; i < _GAME_STATE_N_STATES_; ++i)
    {
        state_cbs[i].init_cb(game_state);
    }
}

void game_state_update(GameState *game_state, float delta)
{
    game_state->delta = delta;

    GameStateCallbacks cbs = state_cbs[game_state->state_id];
    cbs.update_cb(game_state);
    cbs.draw_cb(game_state);
}

Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position)
{
    Vec2 origin = vec2_subtract(&(game_state->camera_focus), &(game_state->camera_half_extension));
    return vec2_subtract(position, &origin);
}
