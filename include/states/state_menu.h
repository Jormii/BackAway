#ifndef STATE_MENU_H
#define STATE_MENU_H

#include "game_state.h"

void menu_state_init(GameState *game_state);
void menu_state_update(GameState *game_state);
void menu_state_draw(const GameState *game_state);

#endif
