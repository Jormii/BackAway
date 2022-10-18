#ifndef STATE_LOAD_H
#define STATE_LOAD_H

#include "game_state.h"

void load_state_init(GameState *game_state);
void load_state_update(GameState *game_state);
void load_state_draw(const GameState *game_state);

#endif
