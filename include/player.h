#ifndef PLAYER_H
#define PLAYER_H

#include "timer.h"
#include "entity.h"
#include "sprite.h"
#include "polygon.h"
#include "game_state.h"

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
    Polygon collider;

    bool_t can_jump;
    Timer input_timer;
    Vec2 button_press_count;
    Vec2 max_velocity;  // Default maximum velocity
    Vec2 input_impulse; // Impulse applied when pushing buttons
} Player;

void player_init(Player *player);
void player_update(Player *player, GameState *game_state);
void player_draw(const Player *player, const GameState *game_state);

#endif
