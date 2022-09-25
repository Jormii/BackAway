#include <stdlib.h>

#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_on_collision(const Polygon *collider, const Polygon *collided_with);

void player_update(Player *player, float delta)
{
    // Update position
    Vec2 old_position = entity_update(&(player->entity), delta);
    check_collisions(&(player->collider), player_on_collision);

    // Update collider
    Vec2 v = vec2_subtract(&(player->entity.position), &old_position);
    for (size_t i = 0; i < player->collider.n_vertices; ++i)
    {
        Vec2 *vertex = player->collider.vertices + i;
        *vertex = vec2_add(vertex, &v);
    }

    Vec2 *bbox = &(player->collider.bbox.origin);
    *bbox = vec2_add(bbox, &v);

    // Render
    sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
    draw_polygon(&(player->collider), 0x00FFFFFF);
}

void player_on_collision(const Polygon *collider, const Polygon *collided_with)
{
}