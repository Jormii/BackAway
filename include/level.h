#ifndef LEVEL_H
#define LEVEL_H

#include "rect.h"
#include "entity.h"

typedef struct Level_st
{
    Rect *colliders;
    size_t n_colliders;
} Level;

void level_resolve_collisions(const Level *level, Entity *entity, const Rect *entity_rect, float delta);

#endif
