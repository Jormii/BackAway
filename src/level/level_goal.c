#include "level.h"
#include "assets.h"
#include "macros.h"
#include "level_goal.h"
#include "screen_buffer.h"

void level_goal_draw_checkered_flag(const LevelGoal *goal);

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
    if (goal->active)
    {
        level_goal_draw_checkered_flag(goal);
    }

    const Sprite *sprite = (goal->active) ? goal->active_sprite : goal->inactive_sprite;
    sprite_draw(sprite, goal->x, goal->top_y, FALSE, TRUE);
    sprite_draw(sprite, goal->x, goal->bottom_y, FALSE, FALSE);
}

void level_goal_draw_checkered_flag(const LevelGoal *goal)
{
    int x_offset = 3;
    int y_offset = 3;
    const Rect *bbox = &(goal->bbox);
    int x0 = MAX(0, bbox->origin.x + x_offset);
    int y0 = MAX(0, bbox->origin.y + y_offset);
    int xf = MIN(bbox->origin.x + bbox->width - x_offset, SCREEN_WIDTH);
    int yf = MIN(bbox->origin.y + bbox->height - y_offset, SCREEN_HEIGHT);

    Color color = {255, 0, 0, 255};
    Vec2 checker_size = {.x = 5.0f, .y = 10.0f}; // A field or something
    for (int py = y0; py < yf; ++py)
    {
        size_t draw_buffer_idx = SCREEN_BUFFER_INDEX(x0, py);
        int checker_y_idx = (int)(((float)(py - y0)) / checker_size.y);

        float alpha_x = ((float)(py - y0)) / (float)bbox->height;
        alpha_x = 2.0f * alpha_x - 1.0f;
        color.alpha = (int)(255.0f * (0.1f + 0.75f * alpha_x * alpha_x));

        for (int px = x0; px < xf; ++px)
        {
            int checker_x_idx = (int)(((float)(px - x0)) / checker_size.x);
            int checker_idx_sum = checker_x_idx + checker_y_idx;
            if ((checker_idx_sum % 2) == 0)
            {
                screen_buffer_paint(draw_buffer_idx, &color);
            }

            draw_buffer_idx += 1;
        }
    }
}
