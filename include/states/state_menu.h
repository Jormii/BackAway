#ifndef STATE_MENU_H
#define STATE_MENU_H

#include "game_state.h"

typedef struct MenuButtonData_st
{
    LevelID level_id;
    float level_target_time;
    GameState *game_state;
} MenuButtonData;

void menu_state_init(GameState *game_state);
void menu_state_update(GameState *game_state);
void menu_state_draw(const GameState *game_state);

#endif
