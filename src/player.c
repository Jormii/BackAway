#include <stdlib.h>

#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_on_collision(PhysicsBody *pbody, const Polygon *collider, const Polygon *collided_with);

void player_update(Player *player, float delta)
{
    // Update position
    check_collisions(&(player->pbody), &(player->collider), player_on_collision);
    physics_body_update(&(player->pbody), delta);

    // Update collider
    const Vec2 *p0 = &(player->pbody.position_last_frame);
    const Vec2 *pf = &(player->pbody.position);
    Vec2 v = {
        .x = pf->x - p0->x,
        .y = pf->y - p0->y};

    for (size_t i = 0; i < player->collider.n_vertices; ++i)
    {
        Vec2 *vertex = player->collider.vertices + i;
        vertex->x += v.x;
        vertex->y += v.y;
    }

    player->collider.bbox.origin.x += v.x;
    player->collider.bbox.origin.y += v.y;

    // Render
    sprite_draw(player->pbody.position.x, player->pbody.position.y, player->sprite);
    draw_polygon(&(player->collider), 0x00FFFFFF);
}

void player_on_collision(PhysicsBody *pbody, const Polygon *collider, const Polygon *collided_with)
{
    const Vec2 v = {
        pbody->position.x - pbody->position_last_frame.x,
        pbody->position.y - pbody->position_last_frame.y};

    for (size_t i = 0; i < collider->n_vertices; ++i)
    {
        const Vec2 *pf = collider->vertices + i;
        if (!rect_contains_point(&(collided_with->bbox), pf))
        {
            continue;
        }

        Vec2 p0 = {
            .x = pf->x - v.x,
            .y = pf->y - v.y};
        for (size_t j = 0; j < collided_with->n_vertices; ++j)
        {
            const Vec2 *n = collided_with->normals + j;
            float dot = vec2_dot(&v, n);
            if (dot >= 0.0f)
            {
                continue;
            }

            const Vec2 *q0 = collided_with->vertices + j;
            const Vec2 *qf = collided_with->vertices + ((j + 1) % collided_with->n_vertices);
            if (lines_intersect(&p0, pf, q0, qf))
            {
                // TODO: Resolve collision
                Vec2 penalty = vec2_project(&(pbody->force), n);
            }
        }
    }
}