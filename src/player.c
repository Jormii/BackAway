#include "path.h"
#include "level.h"
#include "player.h"
#include "physics.h"
#include "draw_geometries.h"

void player_check_collisions(Player *player, const GameState *game_state);
void player_on_collision(Entity *entity, const CollisionData *collision, void *player);

void player_init(Player *player)
{
    entity_init(&(player->entity), 1.0f, 0.0f, 0.0f);
    sprite_load(&(player->sprite), SPRITE("t_sprite_1"));

    Rect sprite_rect = {
        .origin = {.x = player->entity.position.x, .x = player->entity.position.y},
        .width = player->sprite.meta.width,
        .height = player->sprite.meta.height};
    polygon_from_rect(&(player->collider), &sprite_rect);
}

void player_update(Player *player, GameState *game_state)
{
    // Set gravity
    player->entity.force.x = 0.0f;
    player->entity.force.y = 100.0f;

    // Update positions
    entity_update(&(player->entity), game_state->delta);
    player_check_collisions(player, game_state);

    Vec2 v = entity_movement_vector(&(player->entity));
    polygon_move(&(player->collider), &v);
    game_state->camera_focus = rect_center(&(player->collider.bbox));
}

void player_draw(const Player *player, const GameState *game_state)
{
    // TODO: Camera
    // Vec2 camera = game_state_camera_coordinates(game_state, &(player->entity.position));
    Vec2 camera = player->entity.position;
    sprite_draw(&(player->sprite), camera.x, camera.y);
}

void player_check_collisions(Player *player, const GameState *game_state)
{
    const Level *level = game_state->level;
    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        const Polygon *other_collider = level->colliders + i;
        check_collision(&(player->entity), &(player->collider), other_collider, player_on_collision, player);
    }
}

void player_on_collision(Entity *entity, const CollisionData *collision, void *player)
{
    resolve_collision(entity, collision);
}
