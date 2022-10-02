#include "line.h"
#include "level.h"
#include "entity.h"

void check_collision(Entity *entity, const Vec2 *point, const Rect *collider_boundary, bool_t ephemeral,
                     CollisionCb on_collision, void *on_collision_ptr);

void level_check_collisions(const Level *level, Entity *entity, const Rect *entity_boundary,
                            CollisionCb on_collision, void *on_collision_ptr)
{
    // Entity's collider stores positions from last frame. Entity IS updated =>
    //  Objective: Consider this updated motion to handle collisions
    RectCollider entity_collider;
    rect_collider_init(&entity_collider, entity_boundary);

    for (size_t collider_idx = 0; collider_idx < level->n_colliders; ++collider_idx)
    {
        const LevelCollider *collider = level->colliders + collider_idx;

        for (int i = 0; i < 4; ++i)
        {
            check_collision(entity, entity_collider.vertices + i, &(collider->boundary), collider->ephemeral,
                            on_collision, on_collision_ptr);
        }
    }
}

void check_collision(Entity *entity, const Vec2 *point, const Rect *collider_boundary, bool_t ephemeral,
                     CollisionCb on_collision, void *on_collision_ptr)
{
    RectCollider rect_collider;
    rect_collider_init(&rect_collider, collider_boundary);
    Vec2 rect_normals[] = {
        {.x = 0, .y = -1},
        {.x = 1, .y = 0},
        {.x = 0, .y = 1},
        {.x = -1, .y = 0}}; // Clockwise starting at top left corner

    for (size_t vertex_idx = 0; vertex_idx < 4; ++vertex_idx)
    {
        Vec2 movement_vec = entity_movement_vector(entity);
        Vec2 point_next_frame = vec2_add(point, &movement_vec);
        const Vec2 *coll_p = rect_collider.vertices + vertex_idx;
        const Vec2 *coll_q = rect_collider.vertices + ((vertex_idx + 1) % 4);
        const Vec2 *coll_normal = rect_normals + vertex_idx;

        float movement_dot = vec2_dot(&movement_vec, coll_normal);
        if (movement_dot >= 0)
        {
            continue;
        }

        float t;
        bool_t intersection = line_segments_intersect(point, &point_next_frame, coll_p, coll_q, &t);
        if (!intersection)
        {
            continue;
        }

        Vec2 collision_point = line_segment_interpolate(point, &point_next_frame, t);
        on_collision(entity, point, &collision_point, coll_normal, collider_boundary, ephemeral, on_collision_ptr);
    }
}

void level_resolve_collision(Entity *entity, const Vec2 *point, const Vec2 *collision_point, const Vec2 *collision_normal)
{
    // TODO: Problem because it lies on the exact edge
    Vec2 v = entity_movement_vector(entity);
    Vec2 point_next_frame = vec2_add(point, &v);

    // Antipenetration
    Vec2 anti_force = vec2_project(&(entity->force), collision_normal);
    Vec2 anti_velocity = vec2_project(&(entity->velocity), collision_normal);

    entity->force = vec2_subtract(&(entity->force), &anti_force);
    entity->velocity = vec2_subtract(&(entity->velocity), &anti_velocity);

    // Penalty
    Vec2 surface_vec = vec2_subtract(collision_point, &point_next_frame);
    Vec2 penalty = vec2_project(&surface_vec, collision_normal);
    entity->position = vec2_add(&(entity->position), &penalty);

    // TODO: Friction
}
