#include <stdlib.h>

#include <pspiofilemgr.h>

#include "level.h"
#include "level_collider.h"
#include "draw_geometries.h"

bool_t level_load(Level *level, const char *path)
{
    int fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
    if (fd < 0)
    {
        return FALSE;
    }

    // Spawn
    sceIoRead(fd, &(level->spawn_position), sizeof(Vec2));

    // Goal
    float goal_x;
    float goal_top_y;
    float goal_bottom_y;
    sceIoRead(fd, &goal_x, sizeof(float));
    sceIoRead(fd, &goal_top_y, sizeof(float));
    sceIoRead(fd, &goal_bottom_y, sizeof(float));
    level_goal_init(&(level->goal), goal_x, goal_top_y, goal_bottom_y);

    // Objectives
    sceIoRead(fd, &(level->n_objectives), sizeof(uint32_t));
    level->objectives = malloc(level->n_objectives * sizeof(LevelObjective));
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        Vec2 objective_position;
        sceIoRead(fd, &objective_position, sizeof(Vec2));
        level_objective_init(level->objectives + i, &objective_position);
    }

    // Colliders
    sceIoRead(fd, &(level->n_colliders), sizeof(uint32_t));
    level->colliders = malloc(level->n_colliders * sizeof(Polygon));

    uint32_t max_vertices;
    sceIoRead(fd, &max_vertices, sizeof(uint32_t));
    Vec2 *tmp_vertices = malloc(max_vertices * sizeof(Vec2));
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        uint32_t ephemeral;
        sceIoRead(fd, &ephemeral, sizeof(uint32_t));
        level->colliders[i].ephemeral = ephemeral;

        size_t n_vertices;
        sceIoRead(fd, &n_vertices, sizeof(uint32_t));
        sceIoRead(fd, tmp_vertices, n_vertices * sizeof(Vec2));
        polygon_init(level->colliders + i, tmp_vertices, n_vertices);
    }

    free(tmp_vertices);

    return TRUE;
}

void level_free(Level *level)
{
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        Polygon *polygon = level->colliders + i;
        polygon_free(polygon);
    }

    free(level->objectives);
    free(level->colliders);
}

void level_update(Level *level, GameState *game_state)
{
    // Update objectives
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        LevelObjective *objective = level->objectives + i;
        level_objective_update(objective, game_state);
    }

    // Update goal
    level_goal_update(&(level->goal), game_state);
}

void level_draw(const Level *level, const GameState *game_state)
{
    // Draw colliders
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        level_collider_draw(level->colliders + i, game_state);
    }

    // Draw objectives
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        const LevelObjective *objective = level->objectives + i;
        level_objective_draw(objective, game_state);
    }

    // Draw goal
    level_goal_draw(&(level->goal), game_state);
}

void level_reset(Level *level)
{
    level->goal.active = FALSE;
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        LevelObjective *objective = level->objectives + i;
        objective->state = LEVEL_OBJECTIVE_STATE_DEFAULT;
    }
}
