#include "macros.h"
#include "sprite.h"
#include "level_clock.h"

void level_clock_paint(const LevelClock *level_clock, const GameState *game_state,
                       float time, const Vec2 *position, float alpha);

void level_clock_init(LevelClock *level_clock)
{
    level_clock->running = FALSE;
    level_clock->current_time = 0.0f;
    level_clock->target_time = LEVEL_CLOCK_NO_TIME_RECORDED;

    for (size_t i = 0; i < 10; ++i)
    {
        level_clock->numeric_sprites[i] = all_sprites + SPRITE_ID_NUMBER_0 + i;
    }
}

void level_clock_update(LevelClock *level_clock, GameState *game_state)
{
    if (level_clock->running)
    {
        level_clock->current_time += game_state->delta;
        level_clock->current_time = MIN(level_clock->current_time, LEVEL_CLOCK_TIME_LIMIT);
    }
}

void level_clock_draw(const LevelClock *level_clock, const GameState *game_state)
{
    Vec2 position = {10.0f, 10.0f};
    const Sprite *sprite = level_clock->numeric_sprites[0];
    level_clock_paint(level_clock, game_state, level_clock->current_time, &position, 1.0f);

    if (level_clock->target_time != LEVEL_CLOCK_NO_TIME_RECORDED)
    {
        position.y += 1.5f * sprite->meta.height;
        level_clock_paint(level_clock, game_state, level_clock->target_time, &position, 0.5f);
    }
}

void level_clock_reset(LevelClock *level_clock)
{
    level_clock->running = TRUE;
    level_clock->current_time = 0.0f;
}

void level_clock_resume(LevelClock *level_clock)
{
    level_clock->running = TRUE;
}

void level_clock_pause(LevelClock *level_clock)
{
    level_clock->running = FALSE;
}

LevelClockFormatted level_clock_format(float time)
{
    uint64_t time_in_millis = time * 1000.0f;

    uint64_t millis = time_in_millis % 1000;
    uint64_t seconds = time_in_millis / 1000;

    uint64_t minutes = seconds / 60;
    seconds = seconds % 60;

    return (LevelClockFormatted){
        .minutes = minutes,
        .seconds = seconds,
        .milliseconds = millis};
}

void level_clock_paint(const LevelClock *level_clock, const GameState *game_state,
                       float time, const Vec2 *position, float alpha)
{
    Vec2 screen = *position;
    float gap = 1.0f + level_clock->numeric_sprites[0]->meta.width;
    LevelClockFormatted formatted = level_clock_format(time);

    // Minutes
    u8_t digit = formatted.minutes % 10;
    digit = MIN(digit, 9); // Do not allow time greater than 10 minutes
    const Sprite *sprite = level_clock->numeric_sprites[digit];
    sprite_draw(sprite, screen.x, screen.y, alpha, FALSE, FALSE);

    screen.x += 1.75f * gap;

    // Seconds
    u8_t seconds[2];
    for (int i = 1; i >= 0; --i)
    {
        seconds[i] = formatted.seconds % 10;
        formatted.seconds /= 10;
    }

    for (size_t i = 0; i < 2; ++i)
    {
        const Sprite *sprite = level_clock->numeric_sprites[seconds[i]];
        sprite_draw(sprite, screen.x, screen.y, alpha, FALSE, FALSE);

        screen.x += gap;
    }

    screen.x += 0.75f * gap;

    // Milliseconds
    u8_t milliseconds[3];
    for (int i = 2; i >= 0; --i)
    {
        milliseconds[i] = formatted.milliseconds % 10;
        formatted.milliseconds /= 10;
    }

    for (size_t i = 0; i < 3; ++i)
    {
        const Sprite *sprite = level_clock->numeric_sprites[milliseconds[i]];
        sprite_draw(sprite, screen.x, screen.y, alpha, FALSE, FALSE);

        screen.x += gap;
    }
}
