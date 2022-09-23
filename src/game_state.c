#include <stdlib.h>

#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"

struct
{
    Player player;
} game_state;

void init_player()
{
    Player *player = &(game_state.player);

    player->pbody.mass = 1;
    player->pbody.position.x = 0;
    player->pbody.position.y = 0;

    player->sprite = sprite_allocate(30, 30);
    size_t dim = player->sprite->width * player->sprite->height;
    for (size_t i = 0; i < dim; ++i)
    {
        player->sprite->bitmap[i] = 0x00AAAAAA;
    }
}

void game_state_initialize()
{
    init_player();
}

void game_state_update(float delta)
{
    physics_body_reset(&(game_state.player.pbody));
    player_update(&(game_state.player), delta);
}