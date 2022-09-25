#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "sprite.h"
#include "polygon.h"

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
    Polygon collider;
} Player;

void player_update(Player *player, float delta);

#endif