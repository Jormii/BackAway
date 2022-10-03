#include "line.h"
#include "input.h"
#include "macros.h"
#include "player.h"
#include "game_state.h"
#include "draw_geometries.h"

#define MOVE_LEFT BUTTON_LEFT
#define MOVE_RIGHT BUTTON_RIGHT
#define JUMP BUTTON_CROSS
#define ATTACK BUTTON_RIGHT_TRIGGER

void player_handle_input(Player *player, float delta);
void player_attack(const Player *player);
void player_on_collision(Entity *entity, const Vec2 *point,
                         const Vec2 *collision_point, const Vec2 *collision_normal,
                         const Rect *collider_boundary, bool_t ephemeral, void *on_collision_ptr);

void player_update(Player *player, float delta)
{
    // Default entity's force to that of gravity
    player->entity.force.x = 0.0f;
    player->entity.force.y = 100.0f;

    // Other defaults
    player->attack = FALSE;

    // Update positions
    player_handle_input(player, delta);
    entity_update(&(player->entity), delta);
    level_check_collisions(&level, &(player->entity), &(player->collider),
                           player_on_collision, player);

    Vec2 v = entity_movement_vector(&(player->entity));
    player->collider.origin = vec2_add(&(player->collider.origin), &v);

    // Attack
    if (player->attack)
    {
        player_attack(player);
    }

    // Other updates
    player->can_jump = TRUE; // TODO

    // Render
    sprite_draw(player->entity.position.x, player->entity.position.y, &(player->sprite));
    draw_rect(&(player->collider), 0x0088FF88);
}

void player_handle_input(Player *player, float delta)
{
    Vec2 input_vector = {
        .x = input_button_held(MOVE_RIGHT) - input_button_held(MOVE_LEFT),
        .y = player->can_jump * (-input_button_pressed(JUMP))};

    // Apply impulses
    Entity *entity = &(player->entity);
    Vec2 impulse_strength = {.x = 100.0f, .y = 5000.0f};
    Vec2 impulse = vec2_mult_components(&input_vector, &impulse_strength);
    entity_apply_impulse(entity, &impulse, delta);

    entity->velocity.x = CLAMP(entity->velocity.x, -100.0f, 100.0f);

    // Attack
    player->attack = input_button_pressed(ATTACK);
}

void player_attack(const Player *player)
{
    float attack_range = 100;
    Vec2 player_center = rect_center(&(player->collider));

    // Check if any objetive is in range
    for (size_t i = 0; i < level.n_targets; ++i)
    {
        LevelTarget *target = level.targets + i;
        if (target->hit)
        {
            continue;
        }

        Vec2 target_center = rect_center(&(target->boundary));
        Vec2 v = vec2_subtract(&target_center, &player_center);
        float distance = vec2_magnitude(&v);

        target->hit = distance <= attack_range;

        draw_line(&player_center, &target_center, 0x0000FF00);
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
