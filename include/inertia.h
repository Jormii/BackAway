#ifndef INERTIA_H
#define INERTIA_H

#include "vec2.h"

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

#endif
