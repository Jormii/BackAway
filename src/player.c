#include <stdlib.h>

#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_update(Player *player, float delta)
{
    // Update positions
    entity_update(&(player->entity), delta);
    level_resolve_collisions(&level, &(player->entity), &(player->collider), delta);

    Vec2 v = entity_movement_vector(&(player->entity));
    player->collider.origin = vec2_add(&(player->collider.origin), &v);

    // Render
    sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
    draw_rect(&(player->collider), 0x0088FF88);
}
