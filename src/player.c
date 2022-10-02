#include "line.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_on_collision(Entity *entity, const Vec2 *point,
                         const Vec2 *collision_point, const Vec2 *collision_normal,
                         const Rect *collider_boundary, bool_t ephemeral, void *on_collision_ptr);

void player_update(Player *player, float delta)
{
    // Update positions
    player->entity.force.x = 0.0f;
    player->entity.force.y = 100.0f;

    entity_update(&(player->entity), delta);
    level_check_collisions(&level, &(player->entity), &(player->collider),
                           player_on_collision, player);

    Vec2 v = entity_movement_vector(&(player->entity));
    player->collider.origin = vec2_add(&(player->collider.origin), &v);

    // Render
    sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
    draw_rect(&(player->collider), 0x0088FF88);
}

void player_on_collision(Entity *entity, const Vec2 *point,
                         const Vec2 *collision_point, const Vec2 *collision_normal,
                         const Rect *collider_boundary, bool_t ephemeral, void *on_collision_ptr)
{
    bool_t ignore_collisions = FALSE;
    if (ephemeral)
    {
        // Move player's BBOX. If outside the collider, ignore collisions
        const Player *player = (Player *)on_collision_ptr;
        Vec2 movement_vector = entity_movement_vector(entity);

        Rect bbox = player->collider;
        bbox.origin = vec2_add(&(bbox.origin), &movement_vector);
        ignore_collisions = !rect_within_rect(&bbox, collider_boundary);
    }

    if (!ignore_collisions)
    {
        level_resolve_collision(entity, point, collision_point, collision_normal);
    }
}
