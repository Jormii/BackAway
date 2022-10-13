#include "macros.h"
#include "player_inertia.h"

#define INCREMENT_SPEED 50.0f
#define DECREMENT_SPEED 25.0f

void player_inertia_trigger(Timer *timer);

void player_inertia_init(PlayerInertia *inertia, const Vec2 *base_velocity, float timer_countdown)
{
    inertia->base_velocity = *base_velocity;
    inertia->current_velocity = inertia->base_velocity;
    inertia->velocity_target = inertia->base_velocity;
    inertia->increment_speed = INCREMENT_SPEED;

    inertia->button_press_count = 0;
    inertia->button_press_timer.cycle = FALSE;
    inertia->button_press_timer.countdown = timer_countdown;
    inertia->button_press_timer.trigger_cb = player_inertia_trigger;
    inertia->button_press_timer.trigger_cb_ptr = inertia;
}

void player_inertia_update(PlayerInertia *inertia, float delta)
{
    // Update velocity
    Vec2 v = vec2_subtract(&(inertia->velocity_target), &(inertia->current_velocity));
    Vec2 vu = vec2_normalized(&v);

    Vec2 increment = vec2_mult_scalar(50.0f * delta, &vu);
    if (fabs(increment.x) > fabs(v.x))
    {
        increment.x = v.x;
    }
    if (fabs(increment.y) > fabs(v.y))
    {
        increment.y = v.y;
    }

    inertia->current_velocity = vec2_add(&(inertia->current_velocity), &increment);

    // Update timer
    timer_update(&(inertia->button_press_timer), delta);
}

void player_inertia_constrain_velocity(const PlayerInertia *inertia, Vec2 *velocity)
{
    // TODO: Does weird things when swinging
    const Vec2 *max_velocity = &(inertia->current_velocity);

    velocity->x = CLAMP(velocity->x, -max_velocity->x, max_velocity->x);
    velocity->y = MAX(velocity->y, -max_velocity->y);
}

void player_inertia_button_pressed(PlayerInertia *inertia)
{
    // Update velocity
    Vec2 new_velocity = inertia->current_velocity;

    float speed = vec2_magnitude(&(inertia->current_velocity));
    float new_speed = speed;
    do
    {
        inertia->button_press_count += 1;
        float sqr = inertia->button_press_count * inertia->button_press_count;
        float scalar = 0.75f * sqr;
        Vec2 base_increment = {
            .x = scalar * log10f(inertia->base_velocity.x),
            .y = 0.25f * scalar * log10f(inertia->base_velocity.y)};

        new_velocity = vec2_add(&(inertia->base_velocity), &base_increment);
        new_speed = vec2_magnitude(&new_velocity);
    } while (new_speed <= speed); // TODO: This loop is ugly

    inertia->velocity_target = new_velocity;
    inertia->increment_speed = INCREMENT_SPEED;

    // Set timer
    Timer *timer = &(inertia->button_press_timer);
    timer_reset(timer);
    timer_start(timer);
}

void player_inertia_trigger(Timer *timer)
{
    PlayerInertia *inertia = (PlayerInertia *)(timer->trigger_cb_ptr);

    inertia->button_press_count = 0;
    inertia->velocity_target = inertia->base_velocity;
    inertia->increment_speed = DECREMENT_SPEED;
}
