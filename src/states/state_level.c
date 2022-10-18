#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "level.h"
#include "player.h"
#include "state_level.h"
#include "screen_buffer.h"
#include "level_collider.h"

void level_state_reset(GameState *game_state);
bool_t level_state_update_pause(GameState *game_state);
bool_t level_state_update_running(GameState *game_state);

void on_button_highlight(UIButton *button);
void on_continue_pressed(UIButton *button);
void on_restart_pressed(UIButton *button);
Vec2 button_position(const UIButton *button, const GameState *game_state);

void level_state_init(GameState *game_state)
{
    game_state->paused = FALSE;
    game_state->skip_frame = FALSE;
    game_state->slow_motion = FALSE;
    game_state->restart_level = FALSE;

    game_state->level_id = _LEVEL_ID_COUNT_;
    game_state->level = NULL;
    game_state->player = malloc(sizeof(Player));
    player_init(game_state->player);

    UIButtonCollection *buttons = &(game_state->pause_buttons);
    ui_button_collection_init(buttons, 2);

    UIButton *continue_button = buttons->buttons;
    continue_button->position.x = SCREEN_WIDTH;
    continue_button->position.y = 0.33f * SCREEN_HEIGHT;
    continue_button->sprite = all_sprites + SPRITE_ID_MENU_CONTINUE;
    continue_button->on_press_cb = on_continue_pressed;
    continue_button->on_highlighted_cb = NULL;
    continue_button->cb_ptr = game_state;

    UIButton *restart_button = buttons->buttons + 1;
    restart_button->position.x = SCREEN_WIDTH;
    restart_button->position.y = 0.66f * SCREEN_HEIGHT;
    restart_button->sprite = all_sprites + SPRITE_ID_MENU_RESTART;
    restart_button->on_press_cb = on_restart_pressed;
    restart_button->on_highlighted_cb = NULL;
    restart_button->cb_ptr = game_state;
}

bool_t level_state_update(GameState *game_state)
{
    game_state->skip_frame = FALSE;

    if (game_state->restart_level)
    {
        level_state_reset(game_state);
        return TRUE;
    }

    if (game_state->paused)
    {
        return level_state_update_pause(game_state);
    }
    else
    {
        return level_state_update_running(game_state);
    }
}

void level_state_draw(const GameState *game_state)
{
    player_draw(game_state->player, game_state);
    level_draw(game_state->level, game_state);

    if (game_state->paused)
    {
        Color pause_color = {0, 0, 0, 128};
        for (size_t i = 0; i < SCREEN_BUFFER_SIZE; ++i)
        {
            screen_buffer_paint(i, &pause_color);
        }

        const UIButton *button = game_state->pause_buttons.buttons + game_state->pause_buttons.highlighted_idx;
        Vec2 position = button_position(button, game_state);
        sprite_draw(button->sprite, position.x, position.y, FALSE, FALSE);
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
    LevelID next = game_state->level_id + 1;
    if (next < _LEVEL_ID_COUNT_)
    {
        game_state->skip_frame = TRUE;
        level_state_load_level(game_state, next);
    }
}

void level_state_reset(GameState *game_state)
{
    game_state->paused = FALSE;
    game_state->skip_frame = TRUE; // Only exception
    game_state->restart_level = FALSE;
    player_reset(game_state->player, &(game_state->level->spawn_position));
    level_reset(game_state->level);
}

bool_t level_state_update_pause(GameState *game_state)
{
    if (input_button_pressed(INPUT_BUTTON_START))
    {
        game_state->paused = FALSE;
        return TRUE;
    }

    ui_button_collection_update(&(game_state->pause_buttons));

    return game_state->skip_frame;
}

bool_t level_state_update_running(GameState *game_state)
{
    if (input_button_pressed(INPUT_BUTTON_START))
    {
        game_state->paused = TRUE;
        game_state->pause_buttons.highlighted_idx = 0;
        return TRUE;
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

    level_update(game_state->level, game_state);
    player_update(game_state->player, game_state);

    return game_state->skip_frame;
}

void on_continue_pressed(UIButton *button)
{
    GameState *game_state = (GameState *)(button->cb_ptr);

    game_state->paused = FALSE;
    game_state->skip_frame = TRUE;
}

void on_restart_pressed(UIButton *button)
{
    GameState *game_state = (GameState *)(button->cb_ptr);
    level_state_reset(game_state);
}

Vec2 button_position(const UIButton *button, const GameState *game_state)
{
    Vec2 position = vec2_add(&(game_state->camera_focus), &(button->position));
    return vec2_subtract(&position, &(game_state->camera_half_extension));
}
