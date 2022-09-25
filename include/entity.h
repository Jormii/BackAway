#ifndef ENTITY_H
#define ENTITY_H

#include "vec2.h"

typedef struct Entity_st
{
    float mass;
    Vec2 force;
    Vec2 position;
    Vec2 velocity;
} Entity;

void entity_init(Entity *entity, float mass);
void entity_preupdate(Entity *entity);
Vec2 entity_update(Entity *entity, float delta);

#endif
