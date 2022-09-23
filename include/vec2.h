#ifndef VEC2_H
#define VEC2_H

typedef struct Vec2_st
{
    float x;
    float y;
} Vec2;

void vec2_normalize(Vec2 *vector);

#endif