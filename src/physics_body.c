#include "physics_body.h"

void physics_body_reset(PhysicsBody *pbody)
{
    pbody->force.x = 0.0f;
    pbody->force.y = 9.81f;
}

void physics_body_update(PhysicsBody *pbody, float delta)
{
    pbody->position_last_frame = pbody->position;

    // Update velocity and position
    Vec2 acceleration = {
        .x = pbody->force.x / pbody->mass,
        .y = pbody->force.y / pbody->mass};

    pbody->velocity.x += delta * acceleration.x;
    pbody->velocity.y += delta * acceleration.y;

    pbody->position.x += delta * pbody->velocity.x;
    pbody->position.y += delta * pbody->velocity.y;
}