#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "vec2.h"

typedef struct PhysicsBody_st
{
    float mass;
    Vec2 force;
    Vec2 velocity;
    Vec2 position;
    Vec2 position_last_frame;
} PhysicsBody;

void physics_body_reset(PhysicsBody *pbody);
void physics_body_update(PhysicsBody *pbody, float delta);

#endif