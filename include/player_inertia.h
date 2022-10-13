#ifndef PLAYER_INERTIA_H
#define PLAYER_INERTIA_H

#include "vec2.h"
#include "timer.h"

typedef struct PlayerInertia_st
{
    Vec2 base_velocity;
    Vec2 current_velocity;
    Vec2 velocity_target;
    float increment_speed;

    int button_press_count;
    Timer button_press_timer;
} PlayerInertia;

void player_inertia_init(PlayerInertia *inertia, const Vec2 *base_velocity, float timer_countdown);
void player_inertia_update(PlayerInertia *inertia, float delta);

void player_inertia_constrain_velocity(const PlayerInertia *inertia, Vec2 *velocity);
void player_inertia_button_pressed(PlayerInertia *inertia);

#endif
