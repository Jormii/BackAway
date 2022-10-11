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

void player_attack(const Player *player, GameState *game_state);
void player_handle_input(Player *player, const GameState *game_state);
void player_check_collisions(Player *player, const GameState *game_state);

float player_attack_radius(const Player *player);

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr);
void player_input_timer_trigger(Timer *timer);

void player_inertia_update(PlayerInertia *inertia, float delta)
{
    Vec2 factor_v = vec2_subtract(&(inertia->factor_target), &(inertia->factor));
    Vec2 factor_vn = factor_v;
    vec2_normalize(&factor_vn);
    Vec2 factor_inc = vec2_mult_scalar(delta * inertia->factor_speed, &factor_vn);

    if (fabsf(factor_inc.x) > fabs(factor_v.x))
    {
        factor_inc.x = factor_v.x;
    }
    if (fabsf(factor_inc.y) > fabs(factor_v.y))
    {
        factor_inc.y = factor_v.y;
    }

    inertia->factor = vec2_add(&(inertia->factor), &factor_inc);
}

float player_inertia_function(float x)
{
    float base = x / 4.0f - 1.0f;
    return 2.0f + base * base * base;
}

void player_inertia_set(PlayerInertia *inertia, const Vec2 *vector)
{
    inertia->factor_target.x = player_inertia_function(vector->x);
    inertia->factor_target.y = player_inertia_function(vector->y);

    inertia->factor_target.x = MIN(inertia->factor_target.x, inertia->factor_limit.x);
    inertia->factor_target.y = MIN(inertia->factor_target.y, inertia->factor_limit.y);
}

void player_inertia_constrain_velocity(const PlayerInertia *inertia, Vec2 *velocity)
{
    const Vec2 *max_velocity = &(inertia->max_velocity);
    velocity->x = CLAMP(velocity->x, -max_velocity->x, max_velocity->y);
    velocity->y = CLAMP(velocity->y, -max_velocity->y, max_velocity->y);
}

Vec2 player_inertia_impulse(const PlayerInertia *inertia)
{
    return vec2_mult_components(&(inertia->factor), &(inertia->input_impulse));
}

void player_init(Player *player)
{
    // Entity, sprite and collider
    Vec2 gravity = {.x = 0.0f, .y = 100.0f};
    entity_init(&(player->entity), 1.0f, 0.0f, 0.0f, &gravity);
    sprite_load(&(player->sprite), SPRITE("t_sprite_1"));
    hook_init(&(player->hook), &(player->entity), 100.0f, 0.1f);

    Rect sprite_rect = {
        .origin = {.x = player->entity.position.x, .x = player->entity.position.y},
        .width = player->sprite.meta.width,
        .height = player->sprite.meta.height};
    polygon_from_rect(&(player->collider), &sprite_rect);

    // Input related
    player->can_jump = FALSE;
    player->goal_reached = FALSE;
    player->button_press_count.x = 0;
    player->button_press_count.y = 0;

    player->input_timer.cycle = FALSE;
    player->input_timer.countdown = 0.5f;
    player->input_timer.trigger_cb = player_input_timer_trigger;
    player->input_timer.trigger_cb_ptr = player;

    // Inertia related
    player->inertia.factor.x = 1.0f;
    player->inertia.factor.y = 1.0f;
    player->inertia.factor_target = player->inertia.factor;
    player->inertia.factor_limit.x = 1.0f;
    player->inertia.factor_limit.y = 1.0f;
    player->inertia.factor_speed = 0.5f;
    player->inertia.input_impulse.x = 100.0f;
    player->inertia.input_impulse.y = 10000.0f;
    player->inertia.max_velocity.x = 300.0f;
    player->inertia.max_velocity.y = 500.0f;

    // Attack related
    player->attack = FALSE;
    player->attack_radius = 50.0f;
}

void player_update(Player *player, GameState *game_state)
{
    // Defaults
    player->attack = FALSE;

    // Update velocity
    timer_update(&(player->input_timer), game_state->delta);
    player_inertia_update(&(player->inertia), game_state->delta);

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
        //hook_release(hook);
    }

    if (game_state->slow_motion && !hook->fixed)
    {
        hook_move_crosshair(hook, input_vector.x);
        if (input_button_pressed(INPUT_BUTTON_CIRCLE))
        {
            hook_shoot(hook, game_state);
        }

        // Nullify input so player doesn't move
        input_vector.x = 0.0f;
        input_vector.y = 0.0f;
    }

    // Apply impulses
    // TODO: Will need tweaking
    PlayerInertia *inertia = &(player->inertia);
    player_inertia_set(inertia, &(player->button_press_count));

    bool_t inputed_opposite = (input_vector.x * player->entity.velocity.x) < 0.0f;
    if (inputed_opposite)
    {
        // Forcefully 'mirror' X velocity
        player->entity.velocity.x *= -1.0f;
    }

    Vec2 impulse = player_inertia_impulse(inertia);
    impulse = vec2_mult_components(&input_vector, &impulse);

    entity_apply_impulse(&(player->entity), &impulse, game_state->delta);
    player_inertia_constrain_velocity(inertia, &(player->entity.velocity));

    // Update jumping and attack
    if (player->can_jump)
    {
        player->can_jump = input_vector.y >= 0.0f;
    }
    if (input_button_pressed(INPUT_BUTTON_RIGHT_TRIGGER))
    {
        player->attack = TRUE;
    }

    // Handle buttons pressed for next frame
    Vec2 input_press_vector = {
        .x = input_button_pressed(INPUT_BUTTON_RIGHT) - input_button_pressed(INPUT_BUTTON_LEFT),
        .y = input_vector.y};

    Timer *timer = &(player->input_timer);
    if (vec2_magnitude(&input_vector) != 0.0f)
    {
        timer_reset(timer);
    }

    if (vec2_magnitude(&input_press_vector) != 0.0f)
    {
        if (!(timer->running))
        {
            timer_start(&(player->input_timer));
        }

        player->button_press_count.x += fabsf(input_press_vector.x);
        player->button_press_count.y += fabsf(input_press_vector.y);
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
    const Vec2 *factor = &(player->inertia.factor);
    return MAX(factor->x, factor->y) * player->attack_radius;
}

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr)
{
    // TODO: "Phasing"
    resolve_collision(entity, collision);

    Player *player = (Player *)ptr;
    player->can_jump = entity->velocity.y >= 0.0f; // Collided with a surface below it
}

void player_input_timer_trigger(Timer *timer)
{
    Player *player = (Player *)(timer->trigger_cb_ptr);
    player->button_press_count.x = 0;
    player->button_press_count.y = 0;
}
