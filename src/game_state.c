#include <stdlib.h>

#include "level.h"
#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"

void game_state_init(GameState *game_state)
{
    game_state->delta = 0.0f;
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
    level->n_colliders = 1;
    level->colliders = malloc(level->n_colliders * sizeof(Polygon));

    Rect bbox = {
        .origin = {.x = SCREEN_WIDTH / 2 - 100, .y = 2.0f / 3.0f * SCREEN_HEIGHT},
        .width = 200.0f,
        .height = 30.0f};
    polygon_from_rect(level->colliders, &bbox);

    game_state_load_level(game_state, game_state->level);
    // END TODO
}

void game_state_update(GameState *game_state, float delta)
{
    game_state->delta = delta;

    // Update
    level_update(game_state->level, game_state);
    player_update(game_state->player, game_state);

    // Draw
    level_draw(game_state->level, game_state);
    player_draw(game_state->player, game_state);
}

void game_state_load_level(GameState *game_state, struct Level_st *level)
{
    game_state->level = level;
    game_state->player->entity.position = level->spawn_position;
}

Vec2 game_state_camera_transform(const GameState *game_state, const Vec2 *position)
{
    Vec2 origin = vec2_subtract(&(game_state->camera_focus), &(game_state->camera_half_extension));
    return vec2_subtract(position, &origin);
}
