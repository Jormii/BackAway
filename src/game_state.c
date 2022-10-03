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

    player.can_jump = FALSE;

    // Init level
    // TODO: Do it properly
    level.n_colliders = 2;
    level.colliders = malloc(level.n_colliders * sizeof(LevelCollider));

    LevelCollider *horizontal_coll = level.colliders;
    horizontal_coll->ephemeral = FALSE;
    horizontal_coll->boundary.origin.x = 100;
    horizontal_coll->boundary.origin.y = 200;
    horizontal_coll->boundary.width = SCREEN_WIDTH - 200;
    horizontal_coll->boundary.height = 50;

    LevelCollider *vertical_coll = level.colliders + 1;
    vertical_coll->ephemeral = FALSE;
    vertical_coll->boundary.origin.x = SCREEN_WIDTH - 100;
    vertical_coll->boundary.origin.y = 10;
    vertical_coll->boundary.width = 50;
    vertical_coll->boundary.height = SCREEN_HEIGHT - 20;
}

void game_state_update(float delta)
{
    player_update(&player, delta);

    for (size_t i = 0; i < level.n_colliders; ++i)
    {
        const LevelCollider *collider = level.colliders + i;

        rgb_t color = (collider->ephemeral) ? 0x00FF8888 : 0x00222222;
        draw_rect(&(collider->boundary), color);
    }
}
