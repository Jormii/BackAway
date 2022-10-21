#include <stdlib.h>

#include "level.h"
#include "state_menu.h"
#include "state_level.h"
#include "screen_buffer.h"

void on_press(UIButton *button);

void menu_state_init(GameState *game_state)
{
    game_state->store_time = FALSE;

    ui_button_collection_init(&(game_state->buttons), _LEVEL_ID_COUNT_);
    game_state->buttons_data = malloc(game_state->buttons.n_buttons * sizeof(MenuButtonData));

    float gap = 9.6f;
    const Sprite *sprite = all_sprites + SPRITE_ID_MENU_OFF;
    float width = sprite->meta.width;
    float y = 0.5f * SCREEN_HEIGHT - 0.5f * sprite->meta.height;
    
    for (size_t i = 0; i < game_state->buttons.n_buttons; ++i)
    {
        MenuButtonData *data = game_state->buttons_data + i;
        data->level_id = i;
        data->level_target_time = LEVEL_CLOCK_NO_TIME_RECORDED;
        data->game_state = game_state;

        UIButton *button = game_state->buttons.buttons + i;
        button->position.x = (i + 1) * gap + (i * width);
        button->position.y = y;
        button->sprite = sprite;
        button->on_press_cb = on_press;
        button->on_highlighted_cb = NULL;
        button->cb_ptr = data;
    }
}

void menu_state_update(GameState *game_state)
{
    if (game_state->store_time)
    {
        UIButton *button = game_state->buttons.buttons + game_state->buttons.highlighted_idx;
        MenuButtonData *data = (MenuButtonData *)(button->cb_ptr);
        data->level_target_time = game_state->level->level_clock.current_time;

        game_state->store_time = FALSE;
    }

    game_state->camera_focus = game_state->camera_half_extension;
    ui_button_collection_update(&(game_state->buttons));
}

void menu_state_draw(const GameState *game_state)
{
    for (size_t i = 0; i < game_state->buttons.n_buttons; ++i)
    {
        const UIButton *button = game_state->buttons.buttons + i;
        const Sprite *sprite = button->sprite;
        if (i == game_state->buttons.highlighted_idx)
        {
            sprite = all_sprites + SPRITE_ID_MENU_ON;
        }

        sprite_draw(sprite, button->position.x, button->position.y, 1.0f, FALSE, FALSE);
    }
}

void on_press(UIButton *button)
{
    MenuButtonData *data = (MenuButtonData *)(button->cb_ptr);
    GameState *game_state = data->game_state;

    game_state->skip_frame = TRUE;
    game_state->state_id = GAME_STATE_LEVEL;
    level_state_load_level(game_state, data->level_id, data->level_target_time);
}
