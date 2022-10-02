#include "entity.h"

void entity_init(Entity *entity, float mass, float x, float y)
{
    entity->mass = mass;
    entity->force.x = 0.0f;
    entity->force.y = 0.0f;
    entity->velocity.x = 0.0f;
    entity->velocity.y = 0.0f;
    entity->position.x = x;
    entity->position.y = y;
}

void entity_update(Entity *entity, float delta)
{
    entity->position_last_frame = entity->position;

    Vec2 acceleration = vec2_mult_scalar(1.0f / entity->mass, &(entity->force));

    Vec2 delta_v = vec2_mult_scalar(delta, &acceleration);
    entity->velocity = vec2_add(&(entity->velocity), &delta_v);

    Vec2 delta_p = vec2_mult_scalar(delta, &(entity->velocity));
    entity->position = vec2_add(&(entity->position), &delta_p);
}

Vec2 entity_movement_vector(const Entity *entity)
{
    return vec2_subtract(&(entity->position), &(entity->position_last_frame));
}
