#include <stdio.h>
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
    game_state->skip_frame = FALSE;
    game_state->slow_motion = FALSE;
    game_state->restart_level = FALSE;

    game_state->level_id = _LEVEL_ID_COUNT_;
    game_state->level = NULL;
    game_state->player = malloc(sizeof(Player));
    player_init(game_state->player);
}

bool_t level_state_update(GameState *game_state)
{
    game_state->skip_frame = FALSE;

    if (game_state->restart_level)
    {
        level_state_reset(game_state);
        return TRUE;
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

    return game_state->skip_frame;
}

void level_state_draw(const GameState *game_state)
{
    player_draw(game_state->player, game_state);
    level_draw(game_state->level, game_state);
}

void level_state_load_level(GameState *game_state, LevelID level_id)
{
    if (game_state->level != NULL)
    {
        level_free(game_state->level);
    }
    else
    {
        game_state->level = malloc(sizeof(Level));
    }

    const char *path = all_leveles[level_id].path;
    if (!level_load(game_state->level, path))
    {
        printf("Level %s couldn't be loaded\n", path);
    }

    game_state->restart_level = TRUE;
    game_state->level_id = level_id;
}

void level_state_end_level(GameState *game_state)
{
    LevelID next = game_state->level_id + 1;
    if (next < _LEVEL_ID_COUNT_)
    {
        game_state->skip_frame = TRUE;
        level_state_load_level(game_state, next);
    }
}

void level_state_reset(GameState *game_state)
{
    game_state->restart_level = FALSE;
    player_reset(game_state->player, game_state);
    level_reset(game_state->level);
}
