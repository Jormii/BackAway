#include "macros.h"
#include "inertia.h"

void player_inertia_update(PlayerInertia *inertia, float delta)
{
    Vec2 factor_v = vec2_subtract(&(inertia->factor_target), &(inertia->factor));
    Vec2 factor_vn = vec2_normalized(&factor_v);
    Vec2 factor_inc = vec2_mult_scalar(delta * inertia->factor_speed, &factor_vn);

    if (fabsf(factor_inc.x) > fabs(factor_v.x))
    {
        factor_inc.x = factor_v.x;
    }
    if (fabsf(factor_inc.y) > fabs(factor_v.y))
    {
        factor_inc.y = factor_v.y;
    }

    inertia->factor = vec2_add(&(inertia->factor), &factor_inc);
}

float player_inertia_function(float x)
{
    float base = x / 4.0f - 1.0f;
    return 2.0f + base * base * base;
}

void player_inertia_set(PlayerInertia *inertia, const Vec2 *vector)
{
    inertia->factor_target.x = player_inertia_function(vector->x);
    inertia->factor_target.y = player_inertia_function(vector->y);

    inertia->factor_target.x = MIN(inertia->factor_target.x, inertia->factor_limit.x);
    inertia->factor_target.y = MIN(inertia->factor_target.y, inertia->factor_limit.y);
}

void player_inertia_constrain_velocity(const PlayerInertia *inertia, Vec2 *velocity)
{
    const Vec2 *max_velocity = &(inertia->max_velocity);
    velocity->x = CLAMP(velocity->x, -max_velocity->x, max_velocity->y);
    velocity->y = CLAMP(velocity->y, -max_velocity->y, max_velocity->y);
}

Vec2 player_inertia_impulse(const PlayerInertia *inertia)
{
    return vec2_mult_components(&(inertia->factor), &(inertia->input_impulse));
}
