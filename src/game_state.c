#include <stdlib.h>

#include "game_state.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

void game_state_init()
{
    // Init player
    entity_init(&(player.entity), 1.0f, SCREEN_WIDTH / 2.0f, 100.0f);
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
    level.n_colliders = 2;
    level.colliders = malloc(level.n_colliders * sizeof(Rect));

    Rect *horizontal_coll = level.colliders;
    horizontal_coll->origin.x = 100;
    horizontal_coll->origin.y = 200;
    horizontal_coll->width = SCREEN_WIDTH - 200;
    horizontal_coll->height = 50;

    Rect *vertical_coll = level.colliders + 1;
    vertical_coll->origin.x = SCREEN_WIDTH - 100;
    vertical_coll->origin.y = 10;
    vertical_coll->width = 50;
    vertical_coll->height = SCREEN_HEIGHT - 20;
}

void game_state_update(float delta)
{
    player_update(&player, delta);

    for (size_t i = 0; i < level.n_colliders; ++i)
    {
        draw_rect(level.colliders + i, 0x008888FF);
    }
}
