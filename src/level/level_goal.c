#include "level.h"
#include "assets.h"
#include "level_goal.h"

void level_goal_init(LevelGoal *goal, float x, float top_y, float bottom_y)
{
    goal->x = x;
    goal->top_y = top_y;
    goal->bottom_y = bottom_y;
    goal->active = FALSE;

    goal->inactive_sprite = all_sprites + SPRITE_ID_GOAL_INACTIVE;
    goal->active_sprite = all_sprites + SPRITE_ID_GOAL_ACTIVE;

    // BBOX's initialization depends on sprites
    const Sprite *sprite = goal->inactive_sprite; // All sprites have the same size
    goal->bbox.origin.x = x;
    goal->bbox.origin.y = top_y;
    goal->bbox.width = sprite->meta.width;
    goal->bbox.height = bottom_y - top_y + sprite->meta.height;
}

void level_goal_update(LevelGoal *goal, GameState *game_state)
{
    if (goal->active)
    {
        return;
    }

    goal->active = TRUE;
    const Level *level = game_state->level;
    for (size_t i = 0; i < level->n_objectives && goal->active; ++i)
    {
        const LevelObjective *objective = level->objectives + i;
        goal->active = objective->active;
    }
}

void level_goal_draw(const LevelGoal *goal, const GameState *game_state)
{
    // TODO: Procedural in-between
    const Sprite *sprite = (goal->active) ? goal->active_sprite : goal->inactive_sprite;
    sprite_draw(sprite, goal->x, goal->top_y, FALSE, TRUE);
    sprite_draw(sprite, goal->x, goal->bottom_y, FALSE, FALSE);
}
