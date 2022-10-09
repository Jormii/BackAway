#ifndef ENTITY_H
#define ENTITY_H

#include "vec2.h"

typedef struct Entity_st
{
    float mass;
    Vec2 velocity;
    Vec2 position;
    Vec2 position_last_frame;
} Entity;

void entity_init(Entity *entity, float mass, float x, float y);
void entity_update(Entity *entity, float delta);

void entity_apply_impulse(Entity *entity, const Vec2 *impulse, float delta);
Vec2 entity_movement_vector(const Entity *entity);

#endif
