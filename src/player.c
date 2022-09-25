#include <stdlib.h>

#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_update(Player *player, float delta)
{
    // Update positions
    Vec2 old_position = entity_update(&(player->entity), delta);
    Vec2 v = vec2_subtract(&(player->entity.position), &old_position);

    // Render
    sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
}
