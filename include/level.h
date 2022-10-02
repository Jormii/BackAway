#ifndef LEVEL_H
#define LEVEL_H

#include "rect.h"
#include "entity.h"

typedef struct LevelCollider_st
{
    bool_t ephemeral; // Collisions are ignored if player goes fast enough
    Rect boundary;
} LevelCollider;

typedef struct Level_st
{
    size_t n_colliders;
    LevelCollider *colliders;
} Level;

typedef void (*CollisionCb)(Entity *entity, const Vec2 *point,
                            const Vec2 *collision_point, const Vec2 *collision_normal,
                            const Rect *collider_boundary, bool_t ephemeral, void *on_collision_ptr);

void level_check_collisions(const Level *level, Entity *entity, const Rect *entity_boundary,
                            CollisionCb on_collision, void *on_collision_ptr);
void level_resolve_collision(Entity *entity, const Vec2 *point, const Vec2 *collision_point, const Vec2 *collision_normal);

#endif