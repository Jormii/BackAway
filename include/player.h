#ifndef PLAYER_H
#define PLAYER_H

#include "hook.h"
#include "timer.h"
#include "entity.h"
#include "sprite.h"
#include "polygon.h"
#include "game_state.h"

typedef struct PlayerInertia_st
{
    Vec2 factor;
    Vec2 factor_target;
    Vec2 factor_limit;
    float factor_speed;
    Vec2 input_impulse; // Impulse applied when pushing buttons
    Vec2 max_velocity;
} PlayerInertia;

void player_inertia_update(PlayerInertia *inertia, float delta);

float player_inertia_function(float x);
void player_inertia_set(PlayerInertia *inertia, const Vec2 *vector);
void player_inertia_constrain_velocity(const PlayerInertia *inertia, Vec2 *velocity);
Vec2 player_inertia_impulse(const PlayerInertia *inertia);

typedef struct Player_st
{
    Entity entity;
    Sprite sprite;
    Polygon collider;
    PlayerInertia inertia;
    Hook hook;

    bool_t can_jump;
    bool_t goal_reached;
    Timer input_timer;
    Vec2 button_press_count;

    bool_t attack;
    float attack_radius;
} Player;

void player_init(Player *player);
void player_update(Player *player, GameState *game_state);
void player_draw(const Player *player, const GameState *game_state);

#endif
