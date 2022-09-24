#include <stdlib.h>

#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"

struct
{
    Player player;

    size_t n_colliders;
    Polygon *colliders;
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

    Rect rect = {
        .origin = player->pbody.position,
        .width = player->sprite->width,
        .height = player->sprite->height};
    polygon_from_rect(&rect, &(player->collider));
}

void game_state_initialize()
{
    init_player();

    game_state.n_colliders = 1;
    game_state.colliders = malloc(game_state.n_colliders * sizeof(Rect));

    Rect rect = {
        .origin = {.x = 30, .y = SCREEN_HEIGHT / 2},
        .width = SCREEN_WIDTH - 60,
        .height = 30};
    polygon_from_rect(&rect, game_state.colliders);
}

void game_state_update(float delta)
{
    physics_body_reset(&(game_state.player.pbody));
    player_update(&(game_state.player), delta);

    for (size_t i = 0; i < game_state.n_colliders; ++i)
    {
        polygon_draw(game_state.colliders + i, 0x008888FF);
    }
}

void check_collisions(const Polygon *polygon, CollisionCB collision_cb)
{
    for (size_t i = 0; i < game_state.n_colliders; ++i)
    {
        const Polygon *collider = game_state.colliders + i;
        if (rect_within_rect(&(polygon->bbox), &(collider->bbox)))
        {
            bool_t collision = TRUE; // TODO
            if (collision)
            {
                collision_cb(collider);
            }
        }
    }
}