#include <stdlib.h>

#include "level.h"

void level_one_load(Level *level)
{
    int grid_size = 30;
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
}