#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "level.h"
#include "player.h"
#include "state_level.h"
#include "screen_buffer.h"
#include "level_collider.h"

void level_state_reset(GameState *game_state);
void level_state_update_pause(GameState *game_state);
void level_state_update_running(GameState *game_state);

void end_level_trigger(Timer *timer);

void level_state_init(GameState *game_state)
{
    game_state->paused = FALSE;
    game_state->slow_motion = FALSE;
    game_state->restart_level = FALSE;

    Timer *timer = &(game_state->end_level_timer);
    timer->cycle = FALSE;
    timer->countdown = 1.5f;
    timer->trigger_cb = end_level_trigger;
    timer->trigger_cb_ptr = game_state;
    timer->running = FALSE;

    game_state->level_id = _LEVEL_ID_COUNT_;
    game_state->level = NULL;
    game_state->player = malloc(sizeof(Player));
    player_init(game_state->player);
}

void level_state_update(GameState *game_state)
{
    if (game_state->restart_level || input_button_pressed(INPUT_BUTTON_SELECT))
    {
        level_state_reset(game_state);
        game_state->skip_frame = TRUE;
        return;
    }

    if (game_state->paused)
    {
        level_state_update_pause(game_state);
    }
    else
    {
        level_state_update_running(game_state);
    }

    timer_update(&(game_state->end_level_timer), game_state->delta);
}

void level_state_draw(const GameState *game_state)
{
    player_draw(game_state->player, game_state);
    level_draw(game_state->level, game_state);

    const Timer *timer = &(game_state->end_level_timer);
    if (game_state->paused || timer->running)
    {
        Color pause_color = {11, 1, 25, 128}; // Default color
        if (timer->running)
        {
            float completion_ratio = 1.0f - (timer->left / timer->countdown);
            pause_color.alpha = (u8_t)(completion_ratio * 255.0f);
        }

        for (size_t i = 0; i < SCREEN_BUFFER_SIZE; ++i)
        {
            screen_buffer_paint(i, &pause_color);
        }
    }
    if (game_state->end_level_timer.running)
    {
        Color pause_color = {11, 1, 25, 128};
        for (size_t i = 0; i < SCREEN_BUFFER_SIZE; ++i)
        {
            screen_buffer_paint(i, &pause_color);
        }
    }
}

void level_state_load_level(GameState *game_state, LevelID level_id)
{
    if (game_state->level != NULL)
    {
        level_free(game_state->level);
    }
    else
    {
        game_state->level = malloc(sizeof(Level));
    }

    const char *path = all_leveles[level_id].path;
    if (!level_load(game_state->level, path))
    {
        printf("Level %s couldn't be loaded\n", path);
    }

    game_state->restart_level = TRUE;
    game_state->level_id = level_id;
}

void level_state_end_level(GameState *game_state)
{
    Timer *timer = &(game_state->end_level_timer);
    if (!timer->running)
    {
        timer_reset(timer);
        timer_start(timer);
    }
}

void level_state_reset(GameState *game_state)
{
    game_state->paused = FALSE;
    game_state->skip_frame = TRUE; // Only exception
    game_state->restart_level = FALSE;
    timer_stop(&(game_state->end_level_timer));
    player_reset(game_state->player, &(game_state->level->spawn_position));
    level_reset(game_state->level);
}

void level_state_update_pause(GameState *game_state)
{
    if (input_button_pressed(INPUT_BUTTON_START))
    {
        game_state->paused = FALSE;
        game_state->skip_frame = TRUE;
        return;
    }
    else if (input_button_pressed(INPUT_BUTTON_CIRCLE))
    {
        game_state->state_id = GAME_STATE_MENU;
        game_state->skip_frame = TRUE;
    }
}

void level_state_update_running(GameState *game_state)
{
    if (!(game_state->end_level_timer.running))
    {
        if (input_button_pressed(INPUT_BUTTON_START))
        {
            game_state->paused = TRUE;
            game_state->skip_frame = TRUE;
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
    }

    level_update(game_state->level, game_state);
    player_update(game_state->player, game_state);
}

void end_level_trigger(Timer *timer)
{
    GameState *game_state = (GameState *)(timer->trigger_cb_ptr);
    game_state->state_id = GAME_STATE_MENU;
}
