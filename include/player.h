#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include "polygon.h"

typedef struct Player_st
{
    Sprite *sprite;
    Polygon collider;
} Player;

void player_update(Player *player, float delta);

#endif