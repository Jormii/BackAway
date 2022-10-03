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

    bool_t attack;
    bool_t can_jump;
} Player;

void player_update(Player *player, float delta);

#endif
