#ifndef VEC2_H
#define VEC2_H

typedef struct Vec2_st
{
    float x;
    float y;
} Vec2;

float vec2_dot(const Vec2 *a, const Vec2 *b);
void vec2_normalize(Vec2 *vector);
Vec2 vec2_project(const Vec2 *vector, const Vec2 *onto);

#endif