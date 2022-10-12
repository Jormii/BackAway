#ifndef VEC2_H
#define VEC2_H

typedef struct Vec2_st
{
    float x;
    float y;
} Vec2;

float vec2_magnitude(const Vec2 *vector);
void vec2_normalize(Vec2 *vector);
Vec2 vec2_normalized(const Vec2 *vector);
float vec2_dot(const Vec2 *a, const Vec2 *b);
Vec2 vec2_project(const Vec2 *vector, const Vec2 *onto);

Vec2 vec2_add(const Vec2 *a, const Vec2 *b);
Vec2 vec2_subtract(const Vec2 *a, const Vec2 *b);
Vec2 vec2_mult_scalar(float scalar, const Vec2 *vector);
Vec2 vec2_mult_components(const Vec2 *a, const Vec2 *b);

#endif
