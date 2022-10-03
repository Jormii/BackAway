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
    bool_t reached_goal;
    Vec2 velocity_add;
} Player;

void player_update(Player *player, float delta);

#endif
