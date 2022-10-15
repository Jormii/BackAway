#include <math.h>

#include "hook.h"
#include "line.h"
#include "level.h"
#include "macros.h"
#include "player.h"
#include "draw_geometries.h"

#define RESTORING_FACTOR 1000

void hook_apply_impulse(const Hook *hook, float delta);

void hook_init(Hook *hook, Entity *attach_to, const Vec2 *position_offset, float reach, float angular_vel)
{
    hook->reach = reach;
    hook->crosshair_angle = 0.0f;
    hook->crosshair_angular_vel = angular_vel;

    hook->fixed = FALSE;
    hook->fixed_to.x = 0.0f;
    hook->fixed_to.y = 0.0f;
    hook->attached_to = attach_to;
    hook->attachment_position_offset = *position_offset;
}

void hook_update(Hook *hook, GameState *game_state)
{
    // TODO: Speed
    if (hook->fixed)
    {
        hook_apply_impulse(hook, game_state->delta);
    }
}

void hook_draw(const Hook *hook)
{
    if (!hook->fixed)
    {
        return;
    }

    Color black = {0, 0, 0, 0};
    Vec2 origin = hook_attachment_position(hook);
    draw_line(&origin, &(hook->fixed_to), &black);
}

void hook_shoot(Hook *hook, const GameState *game_state)
{
    Vec2 shot_from = hook_attachment_position(hook);
    Vec2 crosshair = hook_crosshair(hook);
    Vec2 vector = vec2_subtract(&crosshair, &shot_from);

    const Level *level = game_state->level;
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        const Polygon *collider = level->colliders + i;

        for (size_t vertex_idx = 0; vertex_idx < collider->n_vertices; ++vertex_idx)
        {
            const Vec2 *p = collider->vertices + vertex_idx;
            const Vec2 *q = collider->vertices + (vertex_idx + 1) % collider->n_vertices;
            const Vec2 *normal = collider->normals + vertex_idx;

            if (vec2_dot(&vector, normal) >= 0.0f)
            {
                continue;
            }

            float t;
            if (!line_segments_intersect(&shot_from, &crosshair, p, q, &t))
            {
                continue;
            }

            Vec2 intersection_point = line_segment_interpolate(&shot_from, &crosshair, t);
            Vec2 v = vec2_subtract(&intersection_point, &shot_from);
            float distance = vec2_magnitude(&v);
            if (distance > hook->reach)
            {
                continue;
            }

            if (!hook->fixed || distance < hook->length)
            {
                hook->fixed = TRUE;
                hook->length = distance;
                hook->fixed_to = intersection_point;
            }
        }
    }
}

void hook_release(Hook *hook)
{
    hook->fixed = FALSE;
}

void hook_move_crosshair(Hook *hook, float sign)
{
    hook->crosshair_angle += sign * hook->crosshair_angular_vel;

    while (hook->crosshair_angle > 0.0f)
    {
        hook->crosshair_angle -= 2 * M_PI;
    }
    while (hook->crosshair_angle > (2 * M_PI))
    {
        hook->crosshair_angle += 2 * M_PI;
    }
}

Vec2 hook_crosshair(const Hook *hook)
{
    Vec2 origin = hook_attachment_position(hook);
    Vec2 vector = {
        .x = hook->reach * cosf(hook->crosshair_angle),
        .y = hook->reach * sinf(hook->crosshair_angle)};

    return vec2_add(&origin, &vector);
}

Vec2 hook_attachment_position(const Hook *hook)
{
    return vec2_add(&(hook->attached_to->position), &(hook->attachment_position_offset));
}

void hook_apply_impulse(const Hook *hook, float delta)
{
    Entity *entity = hook->attached_to;
    Vec2 origin = hook_attachment_position(hook);

    Vec2 hook_v = vec2_subtract(&(hook->fixed_to), &origin);
    Vec2 hook_vu = vec2_normalized(&hook_v);

    // Tension
    Vec2 neg_gravity = vec2_mult_scalar(-1.0f, &(entity->gravity));
    Vec2 neg_gravity_u = vec2_normalized(&neg_gravity);

    float cos = vec2_dot(&hook_vu, &neg_gravity_u);
    float gravity_mag = vec2_magnitude(&neg_gravity);
    float tension_mag = gravity_mag * cos;
    Vec2 tension = vec2_mult_scalar(tension_mag, &hook_vu);

    entity->force = vec2_add(&(entity->force), &tension);

    // Centripetal
    float velocity_mag = vec2_magnitude(&(entity->velocity));
    float centripetal_mag = (velocity_mag * velocity_mag) / hook->length;
    Vec2 centripetal = vec2_mult_scalar(centripetal_mag, &hook_vu);

    entity->force = vec2_add(&(entity->force), &centripetal);

    // Restoring force
    float current_length = vec2_magnitude(&hook_v);
    float length_diff = current_length - hook->length;
    Vec2 restoring = vec2_mult_scalar(RESTORING_FACTOR * length_diff, &hook_vu);

    entity->force = vec2_add(&(entity->force), &restoring);
}
