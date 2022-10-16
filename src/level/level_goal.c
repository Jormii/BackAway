#include "level.h"
#include "assets.h"
#include "macros.h"
#include "level_goal.h"
#include "screen_buffer.h"

void level_goal_draw_effect(const LevelGoal *goal, const GameState *game_state);

void level_goal_init(LevelGoal *goal, float x, float top_y, float bottom_y)
{
    // Load sprites
    goal->inactive_sprite = all_sprites + SPRITE_ID_GOAL_INACTIVE;
    goal->active_sprite = all_sprites + SPRITE_ID_GOAL_ACTIVE;

    // Positions initialization depends on sprites
    goal->x = x;
    goal->top_y = top_y;
    goal->bottom_y = bottom_y - goal->inactive_sprite->meta.height;
    goal->active = FALSE;

    const Sprite *sprite = goal->inactive_sprite; // All sprites have the same size
    goal->bbox.origin.x = x;
    goal->bbox.origin.y = top_y;
    goal->bbox.width = sprite->meta.width;
    goal->bbox.height = bottom_y - top_y;

    // Load sounds
    goal->hit_sfx = all_sounds + SOUND_ID_CHIME_HIT;
    goal->active_sfx = all_sounds + SOUND_ID_CHIME_ALL;
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
        goal->active = objective->state == LEVEL_OBJECTIVE_STATE_ACTIVE;
    }

    sound_play(goal->active_sfx);
}

void level_goal_draw(const LevelGoal *goal, const GameState *game_state)
{
    level_goal_draw_effect(goal, game_state);

    const Sprite *sprite = (goal->active) ? goal->active_sprite : goal->inactive_sprite;

    Vec2 pixel = {.x = goal->x, goal->top_y};
    pixel = game_state_camera_transform(game_state, &pixel);
    sprite_draw(sprite, pixel.x, pixel.y, FALSE, TRUE);

    pixel.x = goal->x;
    pixel.y = goal->bottom_y;
    pixel = game_state_camera_transform(game_state, &pixel);
    sprite_draw(sprite, pixel.x, pixel.y, FALSE, FALSE);
}

void level_goal_draw_effect(const LevelGoal *goal, const GameState *game_state)
{
    int x_offset = 4;
    int y_offset = 3;
    const Rect *bbox = &(goal->bbox);
    Vec2 origin = game_state_camera_transform(game_state, &(bbox->origin));
    int x0 = MAX(0, origin.x + x_offset);
    int y0 = MAX(0, origin.y + y_offset);
    int xf = MIN(origin.x + bbox->width - x_offset, SCREEN_WIDTH);
    int yf = MIN(origin.y + bbox->height - y_offset, SCREEN_HEIGHT);

    Color color = {11, 1, 25, 255};
    if (goal->active)
    {
        color.red = 216;
        color.green = 143;
        color.blue = 60;
        color.alpha = 255;
    }

    int y_displacement = y0 - origin.y;
    for (int py = y0; py < yf; ++py)
    {
        size_t draw_buffer_idx = SCREEN_BUFFER_INDEX(x0, py);

        // TODO
        float alpha_x = ((float)(py - y0) + y_displacement) / (float)bbox->height;
        alpha_x = 2.0f * alpha_x - 1.0f;
        color.alpha = (int)(255.0f * (0.1f + 0.75f * alpha_x * alpha_x));

        for (int px = x0; px < xf; ++px)
        {
            screen_buffer_paint(draw_buffer_idx, &color);
            draw_buffer_idx += 1;
        }
    }
}
