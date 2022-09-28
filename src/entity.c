#include "entity.h"

void entity_init(Entity *entity, float x, float y, float mass)
{
    entity->mass = mass;
    entity->force.x = 0.0f;
    entity->force.y = 9.81f;
    entity->velocity.x = 0.0f;
    entity->velocity.y = 0.0f;
    entity->position.x = x;
    entity->position.y = y;
}

void entity_preupdate(Entity *entity)
{
    entity->force.x = 0;
    entity->force.y = 9.81f;
}

void entity_update(Entity *entity, float delta)
{
    entity->position_last_frame = entity->position;

    Vec2 acceleration = vec2_mult_scalar(1.0f / entity->mass, &(entity->force));

    Vec2 v_increment = vec2_mult_scalar(delta, &acceleration);
    entity->velocity = vec2_add(&(entity->velocity), &v_increment);

    Vec2 p_increment = vec2_mult_scalar(delta, &(entity->velocity));
    entity->position = vec2_add(&(entity->position), &(p_increment));
}

Vec2 entity_movement_vector(const Entity *entity)
{
    return vec2_subtract(&(entity->position), &(entity->position_last_frame));
}
