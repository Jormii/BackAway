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

    // TODO: Initialize level somewhere else
    int grid_size = 30;
    game_state->level = malloc(sizeof(Level));

    Level *level = game_state->level;
    level->spawn_position.x = 2.5f * (float)grid_size;
    level->spawn_position.y = 2.5f * (float)grid_size;

    // Initialize goal
    float goal_x = 7 * grid_size;
    float goal_top_y = 1 * grid_size;
    float goal_bottom_y = 4 * grid_size;
    level_goal_init(&(level->goal), goal_x, goal_top_y, goal_bottom_y);

    // Initialize objectives
    level->n_objectives = 2;
    level->objectives = malloc(level->n_objectives * sizeof(LevelObjective));

    Vec2 obj_pos_1 = {.x = 8.5f * grid_size, .y = 4 * grid_size};
    level_objective_init(level->objectives, &obj_pos_1);

    Vec2 obj_pos_2 = {.x = 12.5f * grid_size, .y = 10 * grid_size};
    level_objective_init(level->objectives + 1, &obj_pos_2);

    // Initialize colliders
    level->n_colliders = 4;
    level->colliders = malloc(level->n_colliders * sizeof(Polygon));

    Rect coll_rect_1 = {
        .origin = {.x = 0, .y = 0},
        .width = 16 * grid_size,
        .height = 14 * grid_size};
    polygon_from_rect(level->colliders, &coll_rect_1);
    level->colliders[0].ephemeral = FALSE;

    Vec2 coll_vert_2[12] = {
        {.x = 1 * grid_size, .y = 1 * grid_size},
        {.x = 1 * grid_size, .y = 8 * grid_size},
        {.x = 5 * grid_size, .y = 8 * grid_size},
        {.x = 5 * grid_size, .y = 13 * grid_size},
        {.x = 9 * grid_size, .y = 13 * grid_size},
        {.x = 9 * grid_size, .y = 10 * grid_size},
        {.x = 15 * grid_size, .y = 10 * grid_size},
        {.x = 15 * grid_size, .y = 7 * grid_size},
        {.x = 14 * grid_size, .y = 7 * grid_size},
        {.x = 14 * grid_size, .y = 6 * grid_size},
        {.x = 15 * grid_size, .y = 6 * grid_size},
        {.x = 15 * grid_size, .y = 1 * grid_size}};
    polygon_init(level->colliders + 1, coll_vert_2, 12);
    level->colliders[1].ephemeral = FALSE;

    Vec2 coll_vert_3[6] = {
        {.x = 6 * grid_size, .y = 1 * grid_size},
        {.x = 7 * grid_size, .y = 1 * grid_size},
        {.x = 7 * grid_size, .y = 4 * grid_size},
        {.x = 12 * grid_size, .y = 4 * grid_size},
        {.x = 12 * grid_size, .y = 6 * grid_size},
        {.x = 6 * grid_size, .y = 6 * grid_size}};
    polygon_init(level->colliders + 2, coll_vert_3, 6);
    level->colliders[2].ephemeral = FALSE;

    Rect coll_rect_4 = {
        .origin = {.x = 10 * grid_size, .y = 1 * grid_size},
        .width = grid_size,
        .height = 3 * grid_size};
    polygon_from_rect(level->colliders + 3, &coll_rect_4);
    level->colliders[3].ephemeral = TRUE;

    game_state->restart_level = TRUE; // Force in order to place everything correctly
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
