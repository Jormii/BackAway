#ifndef PLAYER_H
#define PLAYER_H

#include "rect.h"
#include "sprite.h"
#include "physics_body.h"

typedef struct Player_st
{
    Sprite *sprite;
    Rect collider;
    PhysicsBody pbody;
} Player;

void player_update(Player *player, float delta);

#endif