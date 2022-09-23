#include <stdlib.h>

#include "player.h"

#include <stdio.h> // TODO: Remove
void player_update(Player *player, float delta)
{
    physics_body_update(&(player->pbody), delta);
    sprite_draw(player->pbody.position.x, player->pbody.position.y, player->sprite);
}