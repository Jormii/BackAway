#ifndef INPUT_H
#define INPUT_H

#include <pspctrl.h>

#include "types.h"

typedef enum InputButton_en
{
    BUTTON_START = PSP_CTRL_START
} InputButton;

void input_init();
void input_update();
bool_t input_button_pressed(InputButton button);
bool_t input_button_held(InputButton button);

#endif
