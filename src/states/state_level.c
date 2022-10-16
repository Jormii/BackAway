#include <stdlib.h>

#include "input.h"
#include "level.h"
#include "player.h"
#include "state_level.h"
#include "screen_buffer.h"
#include "level_collider.h"

void level_state_reset(GameState *game_state);

void level_state_init(GameState *game_state)
{
    game_state->slow_motion = FALSE;
    game_state->restart_level = FALSE;

    game_state->player = malloc(sizeof(Player));
    player_init(game_state->player);
}

void level_state_update(GameState *game_state)
{
    if (game_state->restart_level)
    {
        level_state_reset(game_state);
        return;
    }

    if (input_button_held(INPUT_BUTTON_LEFT_TRIGGER))
    {
        game_state->slow_motion = TRUE;
        game_state->delta = 0.001f; // Force delta
    }
    else
    {
        game_state->slow_motion = FALSE;
    }

    level_update(game_state->level, game_state);
    player_update(game_state->player, game_state);
}

void level_state_draw(const GameState *game_state)
{
    player_draw(game_state->player, game_state);
    level_draw(game_state->level, game_state);
}

void level_state_end_level(GameState *game_state)
{
}

void level_state_reset(GameState *game_state)
{
    game_state->restart_level = FALSE;
    player_reset(game_state->player, game_state);
    level_reset(game_state->level);
}
