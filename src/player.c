#include <stdlib.h>

#include "player.h"
#include "game_state.h"

void player_on_collision(const Polygon *collider);

void player_update(Player *player, float delta)
{
    // Update position
    Vec2 p0 = player->pbody.position;
    check_collisions(&(player->collider), player_on_collision);
    physics_body_update(&(player->pbody), delta);

    // Update collider
    Vec2 pf = player->pbody.position;
    Vec2 v = {
        .x = pf.x - p0.x,
        .y = pf.y - p0.y};

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
    polygon_draw(&(player->collider), 0x00FFFFFF);
}

#include <stdio.h> // TODO: Remove
void player_on_collision(const Polygon *collider)
{
    printf("Collision!\n");
}