#include <math.h>

#include "path.h"
#include "input.h"
#include "level.h"
#include "macros.h"
#include "player.h"
#include "physics.h"
#include "draw_geometries.h"

void player_handle_input(Player *player, const GameState *game_state);
void player_check_collisions(Player *player, const GameState *game_state);

void player_on_collision(Entity *entity, const CollisionData *collision, void *ptr);
void player_input_timer_trigger(Timer *timer);

void player_init(Player *player)
{
    // Entity, sprite and collider
    entity_init(&(player->entity), 1.0f, 0.0f, 0.0f);
    sprite_load(&(player->sprite), SPRITE("t_sprite_1"));

    Rect sprite_rect = {
        .origin = {.x = player->entity.position.x, .x = player->entity.position.y},
        .width = player->sprite.meta.width,
        .height = player->sprite.meta.height};
    polygon_from_rect(&(player->collider), &sprite_rect);

    // Input related
    player->can_jump = FALSE;
    player->button_press_count.x = 0;
    player->button_press_count.y = 0;

    player->input_timer.cycle = FALSE;
    player->input_timer.countdown = 5.0f;
    player->input_timer.trigger_cb = player_input_timer_trigger;
    player->input_timer.trigger_cb_ptr = player;

    // Inertia related
    player->max_velocity.x = 100.0f;
    player->max_velocity.y = 500.0f;
    player->input_impulse.x = 100.0f;
    player->input_impulse.y = 5000.0f;
}

void player_update(Player *player, GameState *game_state)
{
    // Defaults
    // *Gravity
    player->entity.force.x = 0.0f;
    player->entity.force.y = 100.0f;

    // *Inputs
    timer_update(&(player->input_timer), game_state->delta);

    // Update positions
    player_handle_input(player, game_state);
    entity_update(&(player->entity), game_state->delta);
    player_check_collisions(player, game_state);

    Vec2 v = entity_movement_vector(&(player->entity));
    polygon_move(&(player->collider), &v);
    game_state->camera_focus = rect_center(&(player->collider.bbox));

    // Final jump update to make sure jumping is disabled when falling from a platform
    if (player->can_jump && player->entity.velocity.y > 0.0f)
    {
        player->can_jump = FALSE;
    }
}

void player_draw(const Player *player, const GameState *game_state)
{
    // TODO: Camera
    // Vec2 camera = game_state_camera_coordinates(game_state, &(player->entity.position));
    Vec2 camera = player->entity.position;
    sprite_draw(&(player->sprite), camera.x, camera.y);
}

void player_handle_input(Player *player, const GameState *game_state)
{
    Vec2 input_vector = {
        .x = input_button_held(INPUT_BUTTON_RIGHT) - input_button_held(INPUT_BUTTON_LEFT),
        .y = -player->can_jump * input_button_pressed(INPUT_BUTTON_CROSS)};

    // Handle fast descend
    if (input_button_held(INPUT_BUTTON_DOWN) && input_button_held(INPUT_BUTTON_CROSS))
    {
        input_vector.y = !player->can_jump;
    }

    // Apply impulses
    Vec2 multiplier = {
        .x = 1.0f + log10f(2 * player->button_press_count.x + 1),
        .y = 1.0f + log10f(2 * player->button_press_count.y + 1)}; // TODO: Will need tweaking

    Vec2 impulse = vec2_mult_components(&multiplier, &(player->input_impulse));
    impulse = vec2_mult_components(&input_vector, &impulse);
    entity_apply_impulse(&(player->entity), &impulse, game_state->delta);

    // Limit velocity
    Vec2 *velocity = &(player->entity.velocity);
    Vec2 max_velocity = vec2_mult_components(&multiplier, &(player->max_velocity));
    velocity->x = CLAMP(velocity->x, -max_velocity.x, max_velocity.x);
    velocity->y = CLAMP(velocity->y, -max_velocity.y, max_velocity.y);

    // Update jumping
    if (player->can_jump)
    {
        player->can_jump = input_vector.y >= 0.0f;
    }

    // Handle buttons pressed for next frame
    Vec2 input_press_vector = {
        .x = input_button_pressed(INPUT_BUTTON_RIGHT) - input_button_pressed(INPUT_BUTTON_LEFT),
        .y = input_vector.y};

    if (vec2_magnitude(&input_press_vector) != 0.0f)
    {
        Timer *timer = &(player->input_timer);
        timer_reset(timer);
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
