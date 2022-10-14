#include <math.h>

#include "path.h"
#include "input.h"
#include "level.h"
#include "assets.h"
#include "macros.h"
#include "player.h"
#include "physics.h"
#include "player_aoe.h"
#include "state_level.h"
#include "screen_buffer.h"
#include "draw_geometries.h"

#define MOVE_LEFT INPUT_BUTTON_LEFT
#define MOVE_RIGHT INPUT_BUTTON_RIGHT
#define DOWN INPUT_BUTTON_DOWN
#define JUMP INPUT_BUTTON_CROSS
#define USE_HOOK INPUT_BUTTON_CIRCLE
#define ATTACK INPUT_BUTTON_RIGHT_TRIGGER

#define GLIDING_GRAVITY_Y 200.0f
#define FREE_FALL_GRAVITY_Y 1500.0f
#define DEFAULT_GRAVITY_Y 700.0f

void player_handle_input(Player *player, const GameState *game_state);
void player_handle_attack(const Player *player, GameState *game_state);
void player_check_collisions(Player *player, const GameState *game_state);
void player_handle_sprites(Player *player, float delta);
const Sprite *player_get_sprite(const Player *player);
bool_t player_is_idle(const Player *player);
Vec2 player_phasing_position(const Player *player);

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr);
void player_forgiveness_trigger(Timer *timer);

void player_init(Player *player)
{
    // Initialize sprite's first. Other components require the size of the sprites
    player->flip_x = FALSE;
    animated_sprite_init(&(player->idle_anim_sprite), 2, 6);
    animated_sprite_init(&(player->run_anim_sprite), 2, 6);

    player->idle_anim_sprite.frames[0] = all_sprites + SPRITE_ID_PLAYER_IDLE_1;
    player->idle_anim_sprite.frames[1] = all_sprites + SPRITE_ID_PLAYER_IDLE_2;
    player->run_anim_sprite.frames[0] = all_sprites + SPRITE_ID_PLAYER_RUN_1;
    player->run_anim_sprite.frames[1] = all_sprites + SPRITE_ID_PLAYER_RUN_2;
    player->jump_sprite = all_sprites + SPRITE_ID_PLAYER_JUMP;
    player->fall_sprite = all_sprites + SPRITE_ID_PLAYER_FALL;
    player->swing_sprite = all_sprites + SPRITE_ID_PLAYER_SWING;

    // Entity and collider
    Vec2 gravity = {.x = 0.0f, .y = DEFAULT_GRAVITY_Y};
    entity_init(&(player->entity), 1.0f, 50.0f, 0.0f, &gravity);

    const Sprite *sprite = player->jump_sprite;
    Rect sprite_rect = {
        .origin = {.x = player->entity.position.x, .y = player->entity.position.y},
        .width = sprite->meta.width,
        .height = sprite->meta.height};
    polygon_from_rect(&(player->collider), &sprite_rect);

    // Inertia
    Vec2 inertia_base_velocity = {.x = 100.0f, .y = 150.0f};
    player_inertia_init(&(player->inertia), &(inertia_base_velocity), 1.0f);

    // Hook
    // TODO: Tweak offset so hook is drawn starting at player's hands
    Vec2 center = rect_center(&(player->collider.bbox));
    Vec2 hook_offset = vec2_subtract(&center, &(player->entity.position));
    hook_init(&(player->hook), &(player->entity), &hook_offset,
              player->inertia.current_velocity.x, 0.05f);

    // Jump related
    player->jump_state = JUMP_STATE_AIRBORNE;
    player->forgiveness_timer.cycle = FALSE;
    player->forgiveness_timer.countdown = 1.0f;
    player->forgiveness_timer.trigger_cb = player_forgiveness_trigger;
    player->forgiveness_timer.trigger_cb_ptr = player;

    // Attack related
    player->attack = FALSE;
    player->attack_radius = player->inertia.current_velocity.x;

    // Other
    player->goal_reached = FALSE;
}

void player_update(Player *player, GameState *game_state)
{
    // Defaults and derived
    player->attack = FALSE;
    player->delta = game_state->delta;

    float reach = MIN(0.45f * SCREEN_HEIGHT, 0.5 * player->inertia.current_velocity.x);
    player->hook.reach = reach;
    player->attack_radius = reach;

    // Update position
    if (!player->goal_reached)
    {
        player_handle_input(player, game_state);
    }

    hook_update(&(player->hook), game_state);
    player_inertia_update(&(player->inertia), game_state->delta);

    entity_preupdate(&(player->entity), game_state->delta);
    if (!player->hook.fixed)
    {
        player_inertia_constrain_velocity(&(player->inertia), &(player->entity.velocity));
    }
    entity_update(&(player->entity), game_state->delta);
    player_check_collisions(player, game_state);

    Vec2 v = entity_movement_vector(&(player->entity));
    polygon_move(&(player->collider), &v);
    game_state->camera_focus = vec2_add(&(game_state->camera_focus), &v);

    // Attack
    player_handle_attack(player, game_state);

    // Check if goal has been reached
    LevelGoal *goal = &(game_state->level->goal);
    if (!player->goal_reached && goal->active)
    {
        bool_t reached_goal = rect_within_rect(&(player->collider.bbox), &(goal->bbox));
        if (reached_goal)
        {
            player->goal_reached = TRUE;
            level_state_end_level(game_state);
        }
    }

    // Update timer and check if fell from platform
    Timer *timer = &(player->forgiveness_timer);
    timer_update(&(player->forgiveness_timer), game_state->delta);

    if (player->jump_state == JUMP_STATE_GROUNDED && player->entity.velocity.y > 0.0f)
    {
        // Handle forgiveness
        if (!timer->running)
        {
            timer_reset(timer);
            timer_start(timer);
        }
    }

    // Deal with animated sprites
    player_handle_sprites(player, game_state->delta);

    // TODO: Remove
    // To draw player next frame if no collisions had happened
    player->collider.bbox.origin = player_phasing_position(player);
}

void player_draw(const Player *player, const GameState *game_state)
{
    // TODO: Camera coordinates
    //Vec2 origin = game_state_camera_transform(game_state, &(player->entity.position));

    Vec2 origin = player->entity.position;
    const Sprite *sprite = player_get_sprite(player);
    sprite_draw(sprite, origin.x, origin.y, player->flip_x, FALSE);

    Vec2 center = rect_center(&(player->collider.bbox));
    //center = game_state_camera_transform(game_state, &center);
    player_aoe_draw(&center, player->attack_radius);

    hook_draw(&(player->hook));
    if (game_state->slow_motion && !player->hook.fixed)
    {
        // Draw crosshair
        Vec2 crosshair = hook_crosshair(&(player->hook));

        Rect rect;
        Color red = {255, 0, 0, 0};
        rect_given_center(&rect, &crosshair, 5, 5);
        draw_rect(&rect, &red);

        Vec2 origin = hook_attachment_position(&(player->hook));
        draw_line(&origin, &crosshair, &red);
    }

    // TODO: Remove
    Color red = {255, 0, 0, 0};
    Color green = {0, 255, 0, 0};

    // Current and future colliders
    draw_polygon(&(player->collider), &green, &red);
    draw_rect(&(player->collider.bbox), &red);

    // END TODO
}

void player_handle_input(Player *player, const GameState *game_state)
{
    // Read input
    Vec2 input_pressed = {
        .x = input_button_pressed(MOVE_RIGHT) - input_button_pressed(MOVE_LEFT),
        .y = -input_button_pressed(JUMP)};
    Vec2 input_held = {
        .x = input_button_held(MOVE_RIGHT) - input_button_held(MOVE_LEFT),
        .y = -input_button_held(JUMP)};

    // Handle jump
    bool_t jumped_this_frame = FALSE;
    bool_t forgiveness_triggered = FALSE;

    // - Trigger jump
    if (input_pressed.y != 0.0f)
    {
        // Jump if player is on the ground or forgiveness is active
        if (player->jump_state == JUMP_STATE_GROUNDED || player->forgiveness_timer.running)
        {
            jumped_this_frame = TRUE;
            forgiveness_triggered = player->forgiveness_timer.running;

            player->jump_state = JUMP_STATE_GLIDING;
            timer_stop(&(player->forgiveness_timer));

            player_inertia_button_pressed(&(player->inertia));
        }
    }

    // - Gliding
    if (player->jump_state == JUMP_STATE_GLIDING && input_held.y == 0.0f)
    {
        player->jump_state = JUMP_STATE_AIRBORNE;
    }

    // - Free fall
    if (input_button_held(DOWN) && input_button_pressed(JUMP))
    {
        bool_t free_fall = player->jump_state == JUMP_STATE_AIRBORNE || player->jump_state == JUMP_STATE_GLIDING;
        if (free_fall)
        {
            jumped_this_frame = FALSE;
            forgiveness_triggered = FALSE; // Set both to FALSE. Forgiveness might be active

            player->jump_state = JUMP_STATE_FREE_FALLING;
            timer_stop(&(player->forgiveness_timer));
        }
    }

    // Handle gravity
    player->entity.gravity.x = 0.0f;
    switch (player->jump_state)
    {
    case JUMP_STATE_GLIDING:
        player->entity.gravity.y = GLIDING_GRAVITY_Y;
        break;
    case JUMP_STATE_FREE_FALLING:
        player->entity.gravity.y = FREE_FALL_GRAVITY_Y;
        break;
    default:
        player->entity.gravity.y = DEFAULT_GRAVITY_Y;
        break;
    }

    // Calculate impulse
    Vec2 impulse = {.x = 500.f, .y = 15000.0f};
    impulse = vec2_mult_components(&input_held, &impulse);

    // - Press impulse
    if (input_pressed.x != 0.0f)
    {
        bool_t different_direction = (impulse.x * player->entity.velocity.x) < 0.0f;
        if (different_direction)
        {
            float sign = (player->entity.velocity.x >= 0) ? 1.0f : -1.0f;
            impulse.x += sign * player->entity.velocity.x;
        }

        impulse.x *= 1.25f;
        player_inertia_button_pressed(&(player->inertia));
    }

    // - Jump impulse
    if (impulse.y != 0.0f)
    {
        if (forgiveness_triggered)
        {
            player->entity.velocity.y = 0.0f; // Force to be 0
        }
        else if (!jumped_this_frame)
        {
            // Set impulse to 0 if jump didn't trigger this frame
            impulse.y = 0.0f;
        }
    }

    // Apply impulse
    entity_apply_impulse(&(player->entity), &impulse, game_state->delta);

    // Inertia timer
    if (input_held.x != 0.0f || player->jump_state == JUMP_STATE_FREE_FALLING)
    {
        timer_reset(&(player->inertia.button_press_timer));
    }

    // Attack
    if (input_button_pressed(ATTACK))
    {
        player->attack = TRUE;
    }

    // Handle hook
    Hook *hook = &(player->hook);
    if (hook->fixed)
    {
        if (player->jump_state == JUMP_STATE_FREE_FALLING || input_button_pressed(USE_HOOK))
        {
            hook_release(hook);
        }
    }

    if (game_state->slow_motion && !hook->fixed)
    {
        hook_move_crosshair(hook, input_held.x);
        if (input_button_pressed(INPUT_BUTTON_CIRCLE))
        {
            hook_shoot(hook, game_state);
        }
    }

    // Handle sprite flipping
    if (player->hook.fixed && !(EQUAL_EPSILON(player->entity.velocity.x, 0.0f, 0.1f)))
    {
        // TODO: Improve
        bool_t heading_left = player->entity.velocity.x < 0.0f;
        player->flip_x = heading_left;
    }
    else if (input_pressed.x != 0.0f)
    {
        player->flip_x = input_pressed.x < 0.0f;
    }
}

void player_handle_attack(const Player *player, GameState *game_state)
{
    // TODO: Some cooldown?
    Vec2 player_pos = rect_center(&(player->collider.bbox));
    float radius_sqr = player->attack_radius * player->attack_radius;

    Level *level = game_state->level;
    for (size_t i = 0; i < level->n_objectives; ++i)
    {
        LevelObjective *objective = level->objectives + i;
        if (objective->active)
        {
            continue;
        }

        Vec2 objective_pos = level_objective_center(objective);
        Vec2 v = vec2_subtract(&objective_pos, &player_pos);
        float sqr_mag = vec2_dot(&v, &v);
        if (sqr_mag <= radius_sqr)
        {
            level_objective_trigger_in_range(objective);

            if (player->attack)
            {
                level_objective_set_active(objective);
            }
        }
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

void player_handle_sprites(Player *player, float delta)
{
    if (player->jump_state == JUMP_STATE_GROUNDED)
    {
        bool_t is_idle = player_is_idle(player);
        AnimatedSprite *idle_anim_sprite = &(player->idle_anim_sprite);
        AnimatedSprite *run_anim_sprite = &(player->run_anim_sprite);

        AnimatedSprite *to_update = (is_idle) ? idle_anim_sprite : run_anim_sprite;
        if (!to_update->frame_tick.running)
        {
            timer_reset(&(to_update->frame_tick));
            timer_start(&(to_update->frame_tick));
            to_update->rendered_frame_idx = 0;
        }
        animated_sprite_update(to_update, delta);
    }
}

const Sprite *player_get_sprite(const Player *player)
{
    if (player->hook.fixed)
    {
        return player->swing_sprite;
    }

    const Vec2 *velocity = &(player->entity.velocity);
    const Sprite *sprite = player->idle_anim_sprite.frames[0];
    switch (player->jump_state)
    {
    case JUMP_STATE_GROUNDED:
    {
        const Sprite *idle_sprite = animated_sprite_get(&(player->idle_anim_sprite));
        const Sprite *run_sprite = animated_sprite_get(&(player->run_anim_sprite));
        bool_t idle = player_is_idle(player);
        sprite = (idle) ? idle_sprite : run_sprite;
    }
    break;
    case JUMP_STATE_GLIDING:
        sprite = player->jump_sprite;
        break;
    case JUMP_STATE_AIRBORNE:
        sprite = (velocity->y <= 0) ? player->jump_sprite : player->fall_sprite;
        break;
    case JUMP_STATE_FREE_FALLING:
        sprite = player->fall_sprite;
        break;
    default:
        break;
    }

    return sprite;
}

bool_t player_is_idle(const Player *player)
{
    return EQUAL_EPSILON(0.0f, vec2_magnitude(&(player->entity.velocity)), 0.5f);
}

Vec2 player_phasing_position(const Player *player)
{
    float scalar = 0.75f * (float)(player->inertia.button_press_count);
    Vec2 delta_p = vec2_mult_scalar(scalar * player->delta, &(player->entity.velocity));
    return vec2_add(&(player->entity.position), &delta_p);
}

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr)
{
    Player *player = (Player *)ptr;

    bool_t ignore = FALSE;
    if (collision->collided_with->ephemeral)
    {
        // If player is moving fast enough, check if collider should be skipped
        Rect bbox = collision->polygon->bbox;
        Vec2 expected_origin = player_phasing_position(player);
        Vec2 bbox_vertices[] = {
            expected_origin,
            expected_origin,
            expected_origin,
            expected_origin};

        bbox_vertices[1].x += bbox.width;
        bbox_vertices[2].x += bbox.width;
        bbox_vertices[2].y += bbox.height;
        bbox_vertices[3].y += bbox.height;

        bool_t all_outside = TRUE;
        for (size_t i = 0; i < 4 && all_outside; ++i)
        {
            const Vec2 *vertex = bbox_vertices + i;
            all_outside = all_outside && !rect_contains_point(&(collision->collided_with->bbox), vertex);
        }
        ignore = all_outside;
    }

    if (!ignore)
    {
        resolve_collision(entity, collision);

        if (player->jump_state != JUMP_STATE_GROUNDED && collision->normal.y < 0.0f)
        {
            // Fell on a surface below player
            player->jump_state = JUMP_STATE_GROUNDED;
        }
    }
}

void player_forgiveness_trigger(Timer *timer)
{
    Player *player = (Player *)(timer->trigger_cb_ptr);
    player->jump_state = JUMP_STATE_AIRBORNE;
}
