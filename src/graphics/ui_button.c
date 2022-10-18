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
            if (button->on_highlighted_cb != NULL)
            {
                button->on_highlighted_cb(button);
            }
        }
    }

    // Handle input
    if (input_button_pressed(INPUT_BUTTON_CROSS))
    {
        UIButton *button = button_coll->buttons + button_coll->highlighted_idx;
        button->on_press_cb(button);
    }
    else if (input_button_pressed(INPUT_BUTTON_LEFT) || input_button_pressed(INPUT_BUTTON_UP))
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
    else if (input_button_pressed(INPUT_BUTTON_RIGHT) || input_button_pressed(INPUT_BUTTON_DOWN))
    {
        button_coll->highlighted_idx = (button_coll->highlighted_idx + 1) % button_coll->n_buttons;
    }
}
