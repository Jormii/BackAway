#include "level.h"
#include "draw_geometries.h"

void level_update(Level *level, GameState *game_state)
{
    if (!level->goal.active)
    {
        level->goal.active = TRUE;
        for (size_t i = 0; i < level->n_objectives && level->goal.active; ++i)
        {
            const LevelObjective *objective = level->objectives + i;
            level->goal.active = level->goal.active && objective->hit;
        }
    }
}

void level_draw(const Level *level, const GameState *game_state)
{
    Color black = {0, 0, 0, 0};
    Color green = {128, 255, 128, 0};
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        draw_polygon(level->colliders + i, &green, &black);
    }

    const Color *goal_color = (level->goal.active) ? &green : &black;
    draw_rect(&(level->goal.boundary), goal_color);

    Rect objective_rect; // TODO: Sprite
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        const LevelObjective *objective = level->objectives + i;
        const Color *objective_color = (objective->hit) ? &green : &black;
        rect_given_center(&objective_rect, &(objective->position), 10.0f, 10.0f);

        draw_rect(&objective_rect, objective_color);
    }
}
