#include <math.h>

#include "vec2.h"

float vec2_dot(const Vec2 *a, const Vec2 *b)
{
    return (a->x * b->x) + (a->y * b->y);
}

void vec2_normalize(Vec2 *vector)
{
    float sqr_magnitude = (vector->x * vector->x) + (vector->y * vector->y);
    float magnitude = sqrtf(sqr_magnitude);

    vector->x /= magnitude;
    vector->y /= magnitude;
}

Vec2 vec2_project(const Vec2 *vector, const Vec2 *onto)
{
    float scalar = vec2_dot(vector, onto) / vec2_dot(onto, onto);
    Vec2 projection = {
        .x = scalar * onto->x,
        .y = scalar * onto->y};

    return projection;
}