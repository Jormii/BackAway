#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "level.h"
#include "player.h"

Level level;
Player player;

void game_state_init();
void game_state_update(float delta);

#endif
