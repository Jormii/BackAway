#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "polygon.h"

// Callback that resolves the collision
typedef void (*CollisionCB)(const Polygon *collider, const Polygon *collided_with);

void game_state_initialize();
void game_state_update(float delta);

void check_collisions(const Polygon *collider, CollisionCB collision_cb);

#endif