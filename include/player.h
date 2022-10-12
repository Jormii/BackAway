#ifndef PLAYER_H
#define PLAYER_H

#include "hook.h"
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
    Hook hook;

    bool_t can_jump;
    bool_t released_jump;
    bool_t attack;
    float attack_radius;
    bool_t goal_reached;
} Player;

void player_init(Player *player);
void player_update(Player *player, GameState *game_state);
void player_draw(const Player *player, const GameState *game_state);

#endif
