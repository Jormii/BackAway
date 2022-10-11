#include <math.h>

#include "vec2.h"

float vec2_magnitude(const Vec2 *vector)
{
    return sqrtf(vec2_dot(vector, vector));
}

void vec2_normalize(Vec2 *vector)
{
    float magnitude = vec2_magnitude(vector);
    if (magnitude != 0.0f)
    {
        vector->x /= magnitude;
        vector->y /= magnitude;
    }
}

float vec2_dot(const Vec2 *a, const Vec2 *b)
{
    return (a->x * b->x) + (a->y * b->y);
}

Vec2 vec2_project(const Vec2 *vector, const Vec2 *onto)
{
    float scalar = vec2_dot(vector, onto) / vec2_dot(onto, onto);
    Vec2 projection = {
        .x = scalar * onto->x,
        .y = scalar * onto->y};

    return projection;
}

Vec2 vec2_add(const Vec2 *a, const Vec2 *b)
{
    return (Vec2){
        .x = a->x + b->x,
        .y = a->y + b->y};
}

Vec2 vec2_subtract(const Vec2 *a, const Vec2 *b)
{
    return (Vec2){
        .x = a->x - b->x,
        .y = a->y - b->y};
}

Vec2 vec2_mult_scalar(float scalar, const Vec2 *vector)
{
    return (Vec2){
        .x = scalar * vector->x,
        .y = scalar * vector->y};
}

Vec2 vec2_mult_components(const Vec2 *a, const Vec2 *b)
{
    return (Vec2){
        .x = a->x * b->x,
        .y = a->y * b->y};
}
