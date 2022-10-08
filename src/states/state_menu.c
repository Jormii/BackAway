#include <stdlib.h>

#include "path.h"
#include "state_menu.h"
#include "draw_geometries.h"

#include <stdio.h> // TODO: Remove

void on_button_press()
{
    printf("Button pressed\n");
}

void on_button_highlight(UIButton *button)
{
    Rect rect;
    const Sprite *sprite = button->sprite;
    rect_given_center(&rect, &(button->position), sprite->meta.width + 10, sprite->meta.height + 10);

    Color color = {255, 255, 255, 255};
    draw_rect(&rect, &color);
}

void menu_state_init(GameState *game_state)
{
    Sprite *shared_sprite = malloc(sizeof(Sprite)); // TODO: Remove. Memory leak
    sprite_load(shared_sprite, SPRITE("t_sprite_2"));

    int n_buttons = 3; // TODO: Read from somewhere
    ui_button_collection_init(&(game_state->button_collection), n_buttons);
    for (size_t i = 0; i < game_state->button_collection.n_buttons; ++i)
    {
        UIButton *button = game_state->button_collection.buttons + i;
        button->position.x = 10 + i * (50 + shared_sprite->meta.width);
        button->position.y = 100;
        button->sprite = shared_sprite;
        button->on_press_cb = on_button_press;
        button->on_highlighted_cb = on_button_highlight;
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
        sprite_draw(button->sprite, button->position.x, button->position.y);
    }
}
