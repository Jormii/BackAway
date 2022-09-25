#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "sprite.h"

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
} Player;

void player_update(Player *player, float delta);

#endif
