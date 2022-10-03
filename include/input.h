#ifndef INPUT_H
#define INPUT_H

#include <pspctrl.h>

#include "types.h"

typedef enum InputButton_en
{
    BUTTON_RIGHT = PSP_CTRL_RIGHT,
    BUTTON_DOWN = PSP_CTRL_DOWN,
    BUTTON_LEFT = PSP_CTRL_LEFT,

    BUTTON_RIGHT_TRIGGER = PSP_CTRL_RTRIGGER,

    BUTTON_CROSS = PSP_CTRL_CROSS
} InputButton;

void input_init();
void input_update();
bool_t input_button_pressed(InputButton button);
bool_t input_button_held(InputButton button);

#endif
