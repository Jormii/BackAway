#ifndef HOOK_H
#define HOOK_H

#include "entity.h"
#include "game_state.h"

typedef struct Hook_st
{
    float reach;
    float crosshair_angle;
    float crosshair_angular_vel;

    bool_t fixed;
    float length;
    Vec2 fixed_to;
    Entity *attached_to;
} Hook;

void hook_init(Hook *hook, Entity *attach_to, float reach, float angular_vel);
void hook_update(Hook *hook, GameState *game_state);
void hook_draw(const Hook *hook);

void hook_shoot(Hook *hook, const GameState *game_state);
void hook_release(Hook *hook);
void hook_move_crosshair(Hook *hook, float sign);

Vec2 hook_crosshair(const Hook *hook);

#endif
