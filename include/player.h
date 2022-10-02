#ifndef PLAYER_H
#define PLAYER_H

#include "rect.h"
#include "entity.h"
#include "sprite.h"

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
    Rect collider;

    bool_t can_jump;
    Vec2 facing_direction;
} Player;

void player_update(Player *player, float delta);

#endif
