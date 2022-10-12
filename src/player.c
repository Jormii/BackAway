#include <math.h>

#include "path.h"
#include "input.h"
#include "level.h"
#include "macros.h"
#include "player.h"
#include "physics.h"
#include "player_aoe.h"
#include "state_level.h"
#include "draw_geometries.h"

#include <stdio.h>

void player_attack(const Player *player, GameState *game_state);
void player_handle_input(Player *player, const GameState *game_state);
void player_check_collisions(Player *player, const GameState *game_state);

float player_attack_radius(const Player *player);

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr);

void player_init(Player *player)
{
    // Entity, sprite and collider
    Vec2 gravity = {.x = 0.0f, .y = 100.0f};
    entity_init(&(player->entity), 1.0f, 0.0f, 0.0f, &gravity);
    sprite_load(&(player->sprite), SPRITE("s_1"));
    hook_init(&(player->hook), &(player->entity), 100.0f, 0.1f);

    Rect sprite_rect = {
        .origin = {.x = player->entity.position.x, .x = player->entity.position.y},
        .width = player->sprite.meta.width,
        .height = player->sprite.meta.height};
    polygon_from_rect(&(player->collider), &sprite_rect);

    // Input related
    player->can_jump = FALSE;
    player->attack = FALSE;
    player->attack_radius = 100.0f;
    player->goal_reached = FALSE;

    // Attack related
    player->attack = FALSE;
    player->attack_radius = 50.0f;
}

void player_update(Player *player, GameState *game_state)
{
    // Defaults
    player->attack = FALSE;

    // Update position
    if (!player->goal_reached)
    {
        player_handle_input(player, game_state);
    }
    hook_update(&(player->hook), game_state);
    entity_update(&(player->entity), game_state->delta);
    player_check_collisions(player, game_state);

    Vec2 v = entity_movement_vector(&(player->entity));
    polygon_move(&(player->collider), &v);
    game_state->camera_focus = rect_center(&(player->collider.bbox));

    if (player->attack)
    {
        player_attack(player, game_state);
    }

    // Check if goal has been reached
    LevelGoal *goal = &(game_state->level->goal);
    if (!player->goal_reached && goal->active)
    {
        bool_t reached_goal = rect_within_rect(&(player->collider.bbox), &(goal->boundary));
        if (reached_goal)
        {
            player->goal_reached = TRUE;
            level_state_end_level(game_state);
        }
    }

    // Final jump update to make sure jumping is disabled when falling from a platform
    if (player->can_jump && player->entity.velocity.y > 0.0f)
    {
        player->can_jump = FALSE;
    }

    printf("V: %f %f\n", player->entity.velocity.x, player->entity.velocity.y);
}

void player_draw(const Player *player, const GameState *game_state)
{
    // TODO: Camera coordinates
    //Vec2 origin = game_state_camera_transform(game_state, &(player->entity.position));
    Vec2 origin = player->entity.position;
    sprite_draw(&(player->sprite), origin.x, origin.y);

    Vec2 center = rect_center(&(player->collider.bbox));
    //center = game_state_camera_transform(game_state, &center);
    player_aoe_draw(&center, player_attack_radius(player));

    hook_draw(&(player->hook));
    if (game_state->slow_motion && !player->hook.fixed)
    {
        // Draw crosshair
        Vec2 crosshair = hook_crosshair(&(player->hook));

        Rect rect;
        Color red = {255, 0, 0, 0};
        rect_given_center(&rect, &crosshair, 5, 5);
        draw_rect(&rect, &red);
        draw_line(&(player->entity.position), &crosshair, &red);
    }

    // TODO: Remove
    Color red = {255, 0, 0, 0};
    Color green = {0, 255, 0, 0};
    Color blue = {0, 0, 255, 0};

    float attack_radius = player_attack_radius(player);
    float sqr_radius = attack_radius * attack_radius;
    const Level *level = game_state->level;
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        const LevelObjective *objective = level->objectives + i;
        Vec2 v = vec2_subtract(&(objective->position), &center);
        float sqr_magnitude = vec2_dot(&v, &v);

        const Color *objective_color = &red;
        if (objective->hit)
        {
            objective_color = &green;
        }
        else if (sqr_magnitude <= sqr_radius)
        {
            objective_color = &blue;
        }

        draw_line(&center, &(objective->position), objective_color);
    }

    draw_polygon(&(player->collider), &green, &red);
    // END TODO
}

void player_attack(const Player *player, GameState *game_state)
{
    // TODO: Some cooldown

    Level *level = game_state->level;
    Vec2 player_pos = rect_center(&(player->collider.bbox));
    float attack_radius = player_attack_radius(player);
    float sqr_radius = attack_radius * attack_radius;

    for (size_t i = 0; i < level->n_colliders; ++i)
    {
        LevelObjective *objective = level->objectives + i;
        if (objective->hit)
        {
            continue;
        }

        Vec2 v = vec2_subtract(&(objective->position), &player_pos);
        float sqr_magnitude = vec2_dot(&v, &v);
        if (sqr_magnitude <= sqr_radius)
        {
            objective->hit = TRUE;
        }
    }
}

void player_handle_input(Player *player, const GameState *game_state)
{
    Vec2 input_vector = {
        .x = input_button_held(INPUT_BUTTON_RIGHT) - input_button_held(INPUT_BUTTON_LEFT),
        .y = -player->can_jump * input_button_pressed(INPUT_BUTTON_CROSS)};

    // Handle free fall
    if (input_button_held(INPUT_BUTTON_DOWN) && input_button_held(INPUT_BUTTON_CROSS))
    {
        input_vector.y = !player->can_jump;
    }

    // Handle hook
    // TODO: Probably need to do something about free falling
    Hook *hook = &(player->hook);

    if (hook->fixed && input_button_pressed(INPUT_BUTTON_CIRCLE))
    {
        // TODO: Think about how to flow between these
        hook_release(hook);
    }

    if (game_state->slow_motion && !hook->fixed)
    {
        hook_move_crosshair(hook, input_vector.x);
        if (input_button_pressed(INPUT_BUTTON_CIRCLE))
        {
            hook_shoot(hook, game_state);
        }
    }

    // Apply impulses
    // TODO: Will need tweaking
    Vec2 impulse = {.x = 100.f, .y = 10000.0f};
    impulse = vec2_mult_components(&input_vector, &impulse);
    entity_apply_impulse(&(player->entity), &impulse, game_state->delta);

    // Update jumping and attack
    if (player->can_jump)
    {
        player->can_jump = input_vector.y >= 0.0f;
    }
    if (input_button_pressed(INPUT_BUTTON_RIGHT_TRIGGER))
    {
        player->attack = TRUE;
    }
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

float player_attack_radius(const Player *player)
{
    //const Vec2 *factor = &(player->inertia.factor);
    //return MAX(factor->x, factor->y) * player->attack_radius;
    return player->attack_radius;
}

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr)
{
    // TODO: "Phasing"
    resolve_collision(entity, collision);

    Player *player = (Player *)ptr;
    player->can_jump = entity->velocity.y >= 0.0f; // Collided with a surface below it
}
