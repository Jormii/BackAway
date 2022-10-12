#include "line.h"
#include "physics.h"

#define FRICTION_COEFF 0.6

bool_t check_boundary(const Entity *entity, const Polygon *collider, const Polygon *other_collider);
void check_vertex_collision(Entity *entity, CollisionData *collision,
                            OnCollision_fp collision_cb, void *collision_cb_ptr);

void check_collision(Entity *entity, const Polygon *entitys_collider, const Polygon *other_collider,
                     OnCollision_fp collision_cb, void *collision_cb_ptr)
{
#if 0 // TODO
    if (!check_boundary(entity, entitys_collider, other_collider))
    {
        return;
    }
#endif

    CollisionData collision = {
        .polygon = entitys_collider,
        .collided_with = other_collider}; // TODO: Sort of ugly
    for (size_t vertex_idx = 0; vertex_idx < entitys_collider->n_vertices; ++vertex_idx)
    {
        collision.vertex = entitys_collider->vertices + vertex_idx;
        collision.vertex_normal = entitys_collider->vertices_normals + vertex_idx;
        check_vertex_collision(entity, &collision, collision_cb, collision_cb_ptr);
    }
}

void resolve_collision(Entity *entity, const CollisionData *collision)
{
    Vec2 movement_vec = entity_movement_vector(entity);
    Vec2 vertex_next_frame = vec2_add(collision->vertex, &movement_vec);

    // Antipenetration
    Vec2 anti_velocity = vec2_project(&(entity->velocity), &(collision->normal));
    anti_velocity = vec2_mult_scalar(-1.0f, &anti_velocity);
    entity->velocity = vec2_add(&(entity->velocity), &anti_velocity);

    // Friction
    // TODO: Static friction
    Vec2 tangent = collision->tangent;
    float tangent_dot = vec2_dot(&(entity->velocity), &tangent);
    if (tangent_dot > 0.0f)
    {
        tangent = vec2_mult_scalar(-1.0f, &tangent);
    }

    float normal_magnitude = vec2_magnitude(&anti_velocity);
    Vec2 friction = vec2_mult_scalar(FRICTION_COEFF * normal_magnitude, &tangent);
    if (vec2_magnitude(&friction) >= vec2_magnitude(&(entity->velocity)))
    {
        friction = vec2_mult_scalar(-1.0f, &(entity->velocity));
    }

    entity->velocity = vec2_add(&(entity->velocity), &friction);

    // Penalty
    Vec2 surface_vec = vec2_subtract(&(collision->point), &vertex_next_frame);
    Vec2 penalty = vec2_project(&surface_vec, &(collision->normal));
    entity->position = vec2_add(&(entity->position), &penalty);
}

bool_t check_boundary(const Entity *entity, const Polygon *collider, const Polygon *other_collider)
{
    // Move collider's bbox along entity's movement to see if they may collide
    Vec2 movement = entity_movement_vector(entity);
    Rect extended_bbox = collider->bbox;
    extended_bbox.width += movement.x;
    extended_bbox.height += movement.y;

    if (movement.x < 0)
    {
        extended_bbox.origin.x += movement.x;
    }
    if (movement.y < 0)
    {
        extended_bbox.origin.y += movement.y;
    }

    return rect_within_rect(&extended_bbox, &(other_collider->bbox));
}

void check_vertex_collision(Entity *entity, CollisionData *collision,
                            OnCollision_fp collision_cb, void *collision_cb_ptr)
{
    Vec2 movement_vec = entity_movement_vector(entity);
    Vec2 vertex_next_frame = vec2_add(collision->vertex, &movement_vec);

    const Polygon *collider = collision->collided_with;
    for (size_t coll_vertex_idx = 0; coll_vertex_idx < collider->n_vertices; ++coll_vertex_idx)
    {
        const Vec2 *coll_p = collider->vertices + coll_vertex_idx;
        const Vec2 *coll_q = collider->vertices + ((coll_vertex_idx + 1) % collider->n_vertices);
        const Vec2 *coll_normal = collider->normals + coll_vertex_idx;

        float movement_dot = vec2_dot(&movement_vec, coll_normal);
        if (movement_dot >= 0)
        {
            continue;
        }

        float normal_dot = vec2_dot(collision->vertex_normal, coll_normal);
        if (normal_dot > 0)
        {
            continue;
        }

        float t;
        bool_t intersection = line_segments_intersect(
            collision->vertex, &vertex_next_frame, coll_p, coll_q, &t);
        if (!intersection)
        {
            continue;
        }

        collision->point = line_segment_interpolate(collision->vertex, &vertex_next_frame, t);
        collision->tangent = vec2_subtract(coll_q, coll_p);
        vec2_normalize(&(collision->tangent));
        collision->normal = *coll_normal;

        collision_cb(entity, collision, collision_cb_ptr);
    }
}
