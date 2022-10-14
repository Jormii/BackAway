#include "level.h"
#include "assets.h"
#include "level_goal.h"

void level_goal_init(LevelGoal *goal, float x, float top_y, float bottom_y)
{
    goal->x = x;
    goal->top_y = top_y;
    goal->bottom_y = bottom_y;
    goal->active = FALSE;

    goal->inactive_top_sprite = all_sprites + SPRITE_ID_GOAL_INACTIVE_TOP;
    goal->inactive_bottom_sprite = all_sprites + SPRITE_ID_GOAL_INACTIVE_BOTTOM;
    goal->active_top_sprite = all_sprites + SPRITE_ID_GOAL_ACTIVE_TOP;
    goal->active_bottom_sprite = all_sprites + SPRITE_ID_GOAL_ACTIVE_BOTTOM;

    // BBOX's initialization depends on sprites
    const Sprite *sprite = goal->inactive_top_sprite; // All sprites have the same size
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
    const Sprite *top_sprite = goal->inactive_top_sprite;
    const Sprite *bottom_sprite = goal->inactive_bottom_sprite;
    if (goal->active)
    {
        top_sprite = goal->active_top_sprite;
        bottom_sprite = goal->active_bottom_sprite;
    }

    // TODO: Procedural in-between
    sprite_draw(top_sprite, goal->x, goal->top_y, FALSE, FALSE);
    sprite_draw(bottom_sprite, goal->x, goal->bottom_y, FALSE, FALSE);
}
