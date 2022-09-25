#ifndef ENTITY_H
#define ENTITY_H

#include "vec2.h"

typedef struct Entity_st
{
    Vec2 position;
    Vec2 direction;
    Vec2 speed;
} Entity;

Vec2 entity_update(Entity *entity, float delta);

#endif