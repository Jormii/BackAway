#include <stdlib.h>

#include "input.h"
#include "ui_button.h"

void ui_button_collection_init(UIButtonCollection *button_coll, size_t n_buttons)
{
    button_coll->n_buttons = n_buttons;
    button_coll->buttons = malloc(n_buttons * sizeof(UIButton));
    button_coll->highlighted_idx = 0;
}

void ui_button_collection_update(UIButtonCollection *button_coll)
{
    // Draw
    for (size_t i = 0; i < button_coll->n_buttons; ++i)
    {
        UIButton *button = button_coll->buttons + i;
        if (i == button_coll->highlighted_idx)
        {
            button->on_highlighted_cb(button);
        }

        sprite_draw(button->sprite, button->position.x, button->position.y, FALSE, FALSE);
    }

    // Handle input
    if (input_button_pressed(INPUT_BUTTON_CROSS))
    {
        UIButton *button = button_coll->buttons + button_coll->highlighted_idx;
        button->on_press_cb();
    }
    else if (input_button_pressed(INPUT_BUTTON_LEFT))
    {
        if (button_coll->highlighted_idx != 0)
        {
            button_coll->highlighted_idx -= 1;
        }
        else
        {
            button_coll->highlighted_idx = button_coll->n_buttons - 1;
        }
    }
    else if (input_button_pressed(INPUT_BUTTON_RIGHT))
    {
        button_coll->highlighted_idx = (button_coll->highlighted_idx + 1) % button_coll->n_buttons;
    }
}
