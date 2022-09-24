#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "polygon.h"

typedef void (*CollisionCB)(const Polygon *collider);

void game_state_initialize();
void game_state_update(float delta);

void check_collisions(const Polygon *polygon, CollisionCB collision_cb);

#endif