#include <stdlib.h>

#include "player.h"
#include "game_state.h"

void player_on_collision(const Rect *rect);

void player_update(Player *player, float delta)
{
    check_collisions(&(player->collider), player_on_collision);
    physics_body_update(&(player->pbody), delta);
    player->collider.origin = player->pbody.position;

    sprite_draw(player->pbody.position.x, player->pbody.position.y, player->sprite);
    rect_draw(&(player->collider), 0x00FFFFFF);
}

#include <stdio.h> // TODO: Remove
void player_on_collision(const Rect *rect)
{
    printf("Collision!\n");
}