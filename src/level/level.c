#include "level.h"
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
    Color black = {0, 0, 0, 0};
    Color green = {128, 255, 128, 0};
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        const Polygon *collider = level->colliders + i;
        const Color *edge_color = (!collider->ephemeral) ? &black : &green;
        const Color *normal_color = (!collider->ephemeral) ? &green : &black;

        draw_polygon(collider, edge_color, normal_color);
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
