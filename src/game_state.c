#include <stdlib.h>

#include "game_state.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

void game_state_init()
{
    // Init player
    entity_init(&(player.entity), SCREEN_WIDTH / 2, 50, 1.05f);
    sprite_init(&(player.sprite), 30, 30);
    player.collider.origin.x = player.entity.position.x;
    player.collider.origin.y = player.entity.position.y;
    player.collider.width = player.sprite.width;
    player.collider.height = player.sprite.height;

    size_t dim = player.sprite.width * player.sprite.height;
    for (size_t i = 0; i < dim; ++i)
    {
        player.sprite.bitmap[i] = 0x00AAAAAA;
    }

    // Init level
    // TODO: Do it properly
    level.n_colliders = 1;
    level.colliders = malloc(level.n_colliders * sizeof(Rect));
    level.colliders->origin.x = 30;
    level.colliders->origin.y = 200;
    level.colliders->width = SCREEN_WIDTH - 60;
    level.colliders->height = 50;
}

void game_state_update(float delta)
{
    entity_preupdate(&(player.entity));
    player_update(&player, delta);

    for (size_t i = 0; i < level.n_colliders; ++i)
    {
        draw_rect(level.colliders + i, 0x008888FF);
    }
}
