#include "physics_body.h"

void physics_body_update(PhysicsBody *pbody, float delta)
{
    Vec2 force = {.x = 0, .y = 9.81};
    Vec2 acceleration = {
        .x = force.x / pbody->mass,
        .y = force.y / pbody->mass};

    pbody->velocity.x += delta * acceleration.x;
    pbody->velocity.y += delta * acceleration.y;

    pbody->position.x += delta * acceleration.x;
    pbody->position.y += delta * acceleration.y;
}