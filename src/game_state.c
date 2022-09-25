#include <stdlib.h>

#include "player.h"
#include "game_state.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

struct
{
    Player player;

    size_t n_colliders;
    Polygon *colliders;
} game_state;

void init_player()
{
    Player *player = &(game_state.player);

    player->entity.position.x = 50;
    player->entity.position.y = 100;
    player->entity.direction.x = 0;
    player->entity.direction.y = 1;
    player->entity.speed.x = 10;
    player->entity.speed.y = 10;

    sprite_create(30, 30, &(player->sprite));
    size_t dim = player->sprite.width * player->sprite.height;
    for (size_t i = 0; i < dim; ++i)
    {
        player->sprite.bitmap[i] = 0x00AAAAAA;
    }

    Rect rect = {
        .origin = player->entity.position,
        .width = player->sprite.width,
        .height = player->sprite.height};
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
    player_update(&(game_state.player), delta);

    for (size_t i = 0; i < game_state.n_colliders; ++i)
    {
        draw_polygon(game_state.colliders + i, 0x008888FF);
    }
}

void check_collisions(const Polygon *collider, CollisionCB collision_cb)
{
    for (size_t i = 0; i < game_state.n_colliders; ++i)
    {
        const Polygon *other_collider = game_state.colliders + i;
        if (rect_within_rect(&(collider->bbox), &(other_collider->bbox)))
        {
            collision_cb(collider, other_collider);
        }
    }
}