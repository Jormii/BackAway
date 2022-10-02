#include "line.h"
#include "input.h"
#include "macros.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

void player_handle_input(Player *player, float delta);
void player_on_collision(Entity *entity, const Vec2 *point,
                         const Vec2 *collision_point, const Vec2 *collision_normal,
                         const Rect *collider_boundary, bool_t ephemeral, void *on_collision_ptr);

void player_update(Player *player, float delta)
{
    // Default entity's force to that of gravity
    player->entity.force.x = 0.0f;
    player->entity.force.y = 100.0f;

    // Update positions
    player_handle_input(player, delta);
    entity_update(&(player->entity), delta);
    level_check_collisions(&level, &(player->entity), &(player->collider),
                           player_on_collision, player);

    Vec2 v = entity_movement_vector(&(player->entity));
    player->collider.origin = vec2_add(&(player->collider.origin), &v);

    // Other updates
    player->can_jump = TRUE; // TODO

    // Render
    sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
    draw_rect(&(player->collider), 0x0088FF88);

    const Rect *coll = &(player->collider);
    Vec2 center = {
        .x = coll->origin.x + 0.5 * coll->width,
        .y = coll->origin.y + 0.5 * coll->height};
    Vec2 facing = vec2_mult_scalar(coll->width, &(player->facing_direction));
    Vec2 facing_tgt = vec2_add(&center, &facing);
    draw_line(&center, &facing_tgt, 0x008888FF);
}

void player_handle_input(Player *player, float delta)
{
    Vec2 input_vector = {
        .x = input_button_held(BUTTON_RIGHT) - input_button_held(BUTTON_LEFT),
        .y = player->can_jump * (-input_button_pressed(BUTTON_UP))};

    // Apply impulses to entity
    Entity *entity = &(player->entity);
    Vec2 impulse_strength = {.x = 100.0f, .y = 5000.0f};
    Vec2 impulse = vec2_mult_components(&input_vector, &impulse_strength);
    entity_apply_impulse(entity, &impulse, delta);

    entity->velocity.x = CLAMP(entity->velocity.x, -100.0f, 100.0f);

    // Facing
    if (input_vector.x != 0)
    {
        player->facing_direction.x = input_vector.x;
    }
}

void player_on_collision(Entity *entity, const Vec2 *point,
                         const Vec2 *collision_point, const Vec2 *collision_normal,
                         const Rect *collider_boundary, bool_t ephemeral, void *on_collision_ptr)
{
    bool_t ignore_collisions = FALSE;
    if (ephemeral)
    {
        // TODO: Think about this logic
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
