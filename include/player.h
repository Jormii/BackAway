#ifndef PLAYER_H
#define PLAYER_H

#include "hook.h"
#include "timer.h"
#include "entity.h"
#include "sprite.h"
#include "polygon.h"
#include "game_state.h"
#include "player_inertia.h"

typedef enum JumpState_en
{
    JUMP_STATE_GROUNDED,
    JUMP_STATE_GLIDING,
    JUMP_STATE_AIRBORNE,
    JUMP_STATE_FREE_FALLING
} JumpState;

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
    Polygon collider;
    PlayerInertia inertia;
    Hook hook;

    JumpState jump_state;
    Timer forgiveness_timer;
    bool_t attack;
    float attack_radius;
    bool_t goal_reached;

    float delta; // In order to deal with "phasing"
} Player;

void player_init(Player *player);
void player_update(Player *player, GameState *game_state);
void player_draw(const Player *player, const GameState *game_state);

#endif
