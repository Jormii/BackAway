#include "line.h"
#include "level.h"
#include "entity.h"
#include "draw_geometries.h"

#define COLLISION_MARGIN 1

void resolve_collision(const Vec2 *point, Vec2 *direction, const RectCollider *collider);

void level_resolve_collisions(const Level *level, Entity *entity, const Rect *entity_rect, float delta)
{
    Vec2 new_direction = entity_movement_vector(entity);
    float magnitude = vec2_magnitude(&new_direction);
    if (magnitude == 0.0f)
    {
        return;
    }

    vec2_normalize(&new_direction);
    new_direction = vec2_mult_scalar(magnitude + COLLISION_MARGIN, &new_direction);

    RectCollider entity_collider;
    rect_collider_init(&entity_collider, entity_rect);
    for (size_t collider_idx = 0; collider_idx < level->n_colliders; ++collider_idx)
    {
        RectCollider collider;
        rect_collider_init(&collider, level->colliders + collider_idx);

        // Only top left and bottom right corners need to be checked
        resolve_collision(entity_collider.vertices, &new_direction, &collider);
        resolve_collision(entity_collider.vertices + 2, &new_direction, &collider);
    }

    // Update position
    entity->position = vec2_add(&(entity->position_last_frame), &new_direction);
}

void resolve_collision(const Vec2 *point, Vec2 *direction, const RectCollider *collider)
{
    // Clockwise starting top left corner
    Vec2 rect_normals[] = {
        {.x = 0, .y = -1},
        {.x = 1, .y = 0},
        {.x = 0, .y = 1},
        {.x = 1, .y = 0}};

    for (size_t vertex_idx = 0; vertex_idx < 4; ++vertex_idx)
    {
        float dot = vec2_dot(direction, rect_normals + vertex_idx);
        if (dot >= 0)
        {
            continue;
        }

        float t;
        Vec2 point_f = vec2_add(point, direction);
        const Vec2 *p = collider->vertices + vertex_idx;
        const Vec2 *q = collider->vertices + ((vertex_idx + 1) % 4);
        bool_t intersection = line_segments_intersect(point, &point_f, p, q, &t);
        if (!intersection)
        {
            continue;
        }

        Vec2 intersection_point = line_segment_interpolate(point, &point_f, t);
        *direction = vec2_subtract(&intersection_point, point);
    }
}
