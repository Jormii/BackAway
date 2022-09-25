#include "entity.h"

void entity_init(Entity *entity, float mass)
{
    entity->mass = mass;
    entity->force.x = 0.0f;
    entity->force.y = 9.81f;
    entity->position.x = 0.0f;
    entity->position.y = 0.0f;
    entity->velocity.x = 0.0f;
    entity->velocity.y = 0.0f;
}

void entity_preupdate(Entity *entity)
{
    entity->force.x = 0.0f;
    entity->force.y = 9.81f;
}

Vec2 entity_update(Entity *entity, float delta)
{
    Vec2 old_position = entity->position;

    Vec2 acceleration = vec2_mult_scalar(1.0f / entity->mass, &(entity->force));

    Vec2 v_increment = vec2_mult_scalar(delta, &acceleration);
    entity->velocity = vec2_add(&(entity->velocity), &v_increment);

    Vec2 p_increment = vec2_mult_scalar(delta, &(entity->velocity));
    entity->position = vec2_add(&(entity->position), &(p_increment));

    return old_position;
}
