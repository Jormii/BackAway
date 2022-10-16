#include <stdlib.h>

#include "path.h"
#include "state_menu.h"
#include "draw_geometries.h"

typedef struct ButtonData_st
{
    GameState *game_state;
    LevelID level_id;
} ButtonData;

ButtonData buttons_data[_LEVEL_ID_COUNT_];

void on_button_press(UIButton *button);
void on_button_highlight(UIButton *button);

void menu_state_init(GameState *game_state)
{
    Sprite *shared_sprite = all_sprites + SPRITE_ID_OBJECTIVE_ACTIVE_1;
    ui_button_collection_init(&(game_state->button_collection), _LEVEL_ID_COUNT_);
    for (size_t i = 0; i < game_state->button_collection.n_buttons; ++i)
    {
        UIButton *button = game_state->button_collection.buttons + i;
        button->position.x = 10 + i * (50 + shared_sprite->meta.width);
        button->position.y = 100;
        button->sprite = shared_sprite;
        button->on_press_cb = on_button_press;
        button->on_highlighted_cb = on_button_highlight;

        ButtonData *button_data = buttons_data + i;
        button_data->game_state = game_state;
        button_data->level_id = i;
        button->cb_ptr = button_data;
    }
}

void menu_state_update(GameState *game_state)
{
    ui_button_collection_update(&(game_state->button_collection));
}

void menu_state_draw(const GameState *game_state)
{
    for (size_t i = 0; i < game_state->button_collection.n_buttons; ++i)
    {
        const UIButton *button = game_state->button_collection.buttons + i;
        sprite_draw(button->sprite, button->position.x, button->position.y, FALSE, FALSE);
    }
}

void on_button_press(UIButton *button)
{
    ButtonData *button_data = (ButtonData *)(button->cb_ptr);

    button_data->game_state->state_id = GAME_STATE_LEVEL;
    button_data->game_state->level_id = button_data->level_id;
    button_data->game_state->level = all_levels[button_data->level_id];
    button_data->game_state->restart_level = TRUE;
}

void on_button_highlight(UIButton *button)
{
    Rect rect;
    const Sprite *sprite = button->sprite;
    rect_given_center(&rect, &(button->position), sprite->meta.width + 10, sprite->meta.height + 10);

    Color color = {0, 0, 0, 255};
    draw_rect(&rect, &color);
}
