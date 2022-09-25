#include <stdlib.h>

#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

struct
{
    Player player;

    size_t n_colliders;
} game_state;

void init_player()
{
    Player *player = &(game_state.player);

    player->entity.position.x = SCREEN_WIDTH / 2;
    player->entity.position.y = 50;

    sprite_create(30, 30, &(player->sprite));
    size_t dim = player->sprite.width * player->sprite.height;
    for (size_t i = 0; i < dim; ++i)
    {
        player->sprite.bitmap[i] = 0x00AAAAAA;
    }
}

void game_state_initialize()
{
    init_player();

    game_state.n_colliders = 1;
}

void game_state_update(float delta)
{
    player_update(&(game_state.player), delta);

    for (size_t i = 0; i < game_state.n_colliders; ++i)
    {
        //draw_polygon(game_state.colliders + i, 0x008888FF);
    }
}
