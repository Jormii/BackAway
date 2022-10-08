#include <stdlib.h>

#include "level.h"
#include "player.h"
#include "state_level.h"
#include "screen_buffer.h"

void level_state_init(GameState *game_state)
{
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
    // END TODO
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

void level_state_end_level(GameState *game_state)
{
    // TODO
}
