#ifndef INPUT_H
#define INPUT_H

#include <pspctrl.h>

#include "types.h"

typedef enum InputButton_en
{
    INPUT_BUTTON_SELECT = PSP_CTRL_SELECT,
    INPUT_BUTTON_START = PSP_CTRL_START,

    INPUT_BUTTON_UP = PSP_CTRL_UP,
    INPUT_BUTTON_RIGHT = PSP_CTRL_RIGHT,
    INPUT_BUTTON_DOWN = PSP_CTRL_DOWN,
    INPUT_BUTTON_LEFT = PSP_CTRL_LEFT,

    INPUT_BUTTON_LEFT_TRIGGER = PSP_CTRL_LTRIGGER,
    INPUT_BUTTON_RIGHT_TRIGGER = PSP_CTRL_RTRIGGER,
    INPUT_BUTTON_CIRCLE = PSP_CTRL_CIRCLE,
    INPUT_BUTTON_CROSS = PSP_CTRL_CROSS,
    INPUT_BUTTON_SQUARE = PSP_CTRL_SQUARE
} InputButton;

void input_init();
void input_update();
bool_t input_button_pressed(InputButton button);
bool_t input_button_held(InputButton button);

#endif
