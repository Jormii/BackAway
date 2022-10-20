#include "level_clock.h"

void level_clock_init(LevelClock *level_clock)
{
    level_clock->running = FALSE;
    level_clock->current_time = 0.0f;
    level_clock->target_time = LEVEL_CLOCK_NO_TIME_RECORDED;
    // TODO: Sprites
}

void level_clock_update(LevelClock *level_clock, GameState *game_state)
{
    if (level_clock->running)
    {
        level_clock->current_time += game_state->delta;
    }
}

void level_clock_draw(const LevelClock *level_clock, const GameState *game_state)
{
    LevelClockFormatted formatted = level_clock_format(level_clock->current_time);
    printf("C: %u:%u:%u\n", formatted.minutes, formatted.seconds, formatted.milliseconds);
    if (level_clock->target_time != LEVEL_CLOCK_NO_TIME_RECORDED)
    {
        formatted = level_clock_format(level_clock->target_time);
        printf("T: %u:%u:%u\n", formatted.minutes, formatted.seconds, formatted.milliseconds);
    }

    printf("\n");
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
