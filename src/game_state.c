#include <stdlib.h>

#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"

struct
{
    Player player;

    size_t n_colliders;
    Rect *colliders;
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

    player->collider.origin = player->pbody.position;
    player->collider.width = player->sprite->width;
    player->collider.height = player->sprite->height;
}

void game_state_initialize()
{
    init_player();

    game_state.n_colliders = 1;
    game_state.colliders = malloc(game_state.n_colliders * sizeof(Rect));

    Rect *rect = game_state.colliders;
    rect->origin.x = 0;
    rect->origin.y = SCREEN_HEIGHT / 2;
    rect->width = SCREEN_WIDTH;
    rect->height = 20;
}

void game_state_update(float delta)
{
    physics_body_reset(&(game_state.player.pbody));
    player_update(&(game_state.player), delta);

    rect_draw(game_state.colliders, 0x008888FF);
}

void check_collisions(const Rect *rect, CollisionCB collision_cb)
{
    for (size_t i = 0; i < game_state.n_colliders; ++i)
    {
        const Rect *collider = game_state.colliders + i;
        if (rect_within_rect(rect, collider))
        {
            collision_cb(collider);
        }
    }
}