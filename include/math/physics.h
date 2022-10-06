#ifndef PHYSICS_H
#define PHYSICS_H

#include "entity.h"
#include "polygon.h"

struct CollisionData_st;
typedef void (*OnCollision_fp)(
    Entity *entity, const struct CollisionData_st *collision, void *ptr);

typedef struct CollisionData_st
{
    const Vec2 *vertex;
    const Polygon *polygon;
    const Polygon *collided_with;

    Vec2 point;
    Vec2 tangent;
    Vec2 normal;
} CollisionData;

void check_collision(Entity *entity, const Polygon *entitys_collider, const Polygon *other_collider,
                     OnCollision_fp collision_cb, void *collision_cb_ptr);
void resolve_collision(Entity *entity, const CollisionData *collision);

#endif
