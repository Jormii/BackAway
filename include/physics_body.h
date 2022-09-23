#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "vec2.h"

typedef struct PhysicsBody_st
{
    float mass;
    Vec2 position;
    Vec2 velocity;
} PhysicsBody;

void physics_body_update(PhysicsBody *pbody, float delta);

#endif