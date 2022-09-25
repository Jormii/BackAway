#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "polygon.h"
#include "physics_body.h"

// Callback that resolves the collision
typedef void (*CollisionCB)(PhysicsBody *pbody, const Polygon *collider, const Polygon *collided_with);

void game_state_initialize();
void game_state_update(float delta);

void check_collisions(PhysicsBody *pbody, const Polygon *collider, CollisionCB collision_cb);

#endif