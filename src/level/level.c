#include "level.h"
#include "level_collider.h"
#include "draw_geometries.h"

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
        level_collider_draw(level->colliders + i);
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
