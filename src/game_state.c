#include "game_state.h"
#include "screen_buffer.h"

void game_state_init(GameState *game_state)
{
    game_state->delta = 0.0f;
    game_state->camera_focus.x = 0.0f;
    game_state->camera_focus.y = 0.0f;
    game_state->camera_half_extension.x = 0.5 * SCREEN_WIDTH;
    game_state->camera_half_extension.y = 0.5 * SCREEN_HEIGHT;

    // TODO: Load sprites
    game_state->n_sprites = 0;
    game_state->loaded_sprites = NULL;
}

void game_state_update(GameState *game_state, float delta)
{
    game_state->delta = delta;
}
