#ifndef STATE_LEVEL_H
#define STATE_LEVEL_H

#include "game_state.h"

void level_state_init(GameState *game_state);
void level_state_update(GameState *game_state);
void level_state_draw(const GameState *game_state);

void level_state_end_level(GameState *game_state);

#endif
