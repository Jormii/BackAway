#include <stdlib.h>

#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_on_collision(const Polygon *collider, const Polygon *collided_with);

void player_update(Player *player, float delta)
{
    // Update position
    check_collisions(&(player->collider), player_on_collision);

    // Render
    sprite_draw( // TODO: XY args
        player->collider.vertices[0].x, player->collider.vertices[0].y, player->sprite);
    draw_polygon(&(player->collider), 0x00FFFFFF);
}

void player_on_collision(const Polygon *collider, const Polygon *collided_with)
{
}