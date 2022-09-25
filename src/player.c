#include <stdlib.h>

#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_on_collision(const Polygon *collider, const Polygon *collided_with, void *ptr);
void resolve_penetration(Entity *entity, const Vec2 *point, const Polygon *penetrated);

void player_update(Player *player, float delta)
{
    // Reset entity and check for collisions
    player->entity.speed.x = 100.0f;
    player->entity.speed.y = 100.0f;
    player->entity.direction.x = 1.0f;
    player->entity.direction.y = 1.0f;
    check_collisions(&(player->collider), player, player_on_collision);

    // Update positions
    Vec2 old_position = entity_update(&(player->entity), delta);
    Vec2 v = vec2_subtract(&(player->entity.position), &old_position);

    for (size_t i = 0; i < player->collider.n_vertices; ++i)
    {
        Vec2 *vertex = player->collider.vertices + i;
        *vertex = vec2_add(vertex, &v);
    }

    Vec2 *bbox = &(player->collider.bbox.origin);
    *bbox = vec2_add(bbox, &v);

    // Render
    //sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
    draw_polygon(&(player->collider), 0x00FFFFFF);
}

void player_on_collision(const Polygon *collider, const Polygon *collided_with, void *ptr)
{
    Player *player = (Player *)ptr;

    for (size_t i = 0; i < collider->n_vertices; ++i)
    {
        const Vec2 *vertex = collider->vertices + i;
        if (rect_contains_point(&(collided_with->bbox), vertex))
        {
            resolve_penetration(&(player->entity), vertex, collided_with);
        }
    }
}

void resolve_penetration(Entity *entity, const Vec2 *point, const Polygon *penetrated)
{
    Vec2 direction = entity->direction;
    vec2_normalize(&direction);

    for (size_t i = 0; i < penetrated->n_vertices; ++i)
    {
        const Vec2 *normal = penetrated->normals + i;
        const Vec2 *vertex = penetrated->vertices + i;

        float dot = vec2_dot(normal, &direction);
        if (dot >= 0)
        {
            continue;
        }

        // TODO: Remove
        draw_line(
            penetrated->vertices + i,
            penetrated->vertices + ((i + 1) % penetrated->n_vertices),
            0x008888FF);

        Vec2 v = vec2_subtract(vertex, point);
        Vec2 v_on_normal = vec2_project(&v, normal);

        // TODO: Remove
        Vec2 p_plus_proj = vec2_add(point, &v_on_normal);
        draw_line(point, &p_plus_proj, 0x0088FF88);
        draw_line(point, vertex, 0x00FF8888);
    
        float distance_penetrated = vec2_magnitude(&v_on_normal);
        Vec2 penalty = vec2_mult_scalar(-distance_penetrated, &direction);
        entity->direction = vec2_add(&(entity->direction), &penalty);
    }
}