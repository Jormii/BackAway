#ifndef ENTITY_H
#define ENTITY_H

#include "vec2.h"

typedef struct Entity_st
{
    float mass;
    Vec2 force;
    Vec2 velocity;
    Vec2 position;
    Vec2 position_last_frame;
} Entity;

void entity_init(Entity *entity, float x, float y, float mass);
void entity_preupdate(Entity *entity);
void entity_update(Entity *entity, float delta);

Vec2 entity_movement_vector(const Entity *entity);

#endif
