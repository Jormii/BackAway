#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "sprite.h"
#include "polygon.h"
#include "game_state.h"

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
    Polygon collider;
} Player;

void player_init(Player *player);
void player_update(Player *player, GameState *game_state);
void player_draw(const Player *player, const GameState *game_state);

#endif
