#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "rect.h"

typedef void (*CollisionCB)(const Rect *collider);

void game_state_initialize();
void game_state_update(float delta);

void check_collisions(const Rect *rect, CollisionCB collision_cb);

#endif