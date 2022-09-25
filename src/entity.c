#include "entity.h"

Vec2 entity_update(Entity *entity, float delta)
{
    Vec2 old_position = entity->position;

    vec2_normalize(&(entity->direction));
    Vec2 velocity = vec2_mult_components(&(entity->speed), &(entity->direction));
    Vec2 delta_velocity = vec2_mult_scalar(delta, &velocity);
    entity->position = vec2_add(&old_position, &delta_velocity);

    return old_position;
}