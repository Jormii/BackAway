#ifndef level_clock_H
#define level_clock_H

#include "game_state.h"

#define LEVEL_CLOCK_NO_TIME_RECORDED -1.0f

typedef struct LevelClock_st
{
    bool_t running;
    float current_time;
    float target_time;
    Sprite *semicolon_sprite;
    Sprite *numeric_sprites[10];
} LevelClock;

typedef struct LevelClockFormatted_st
{
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;
} LevelClockFormatted;

void level_clock_init(LevelClock *level_clock);
void level_clock_update(LevelClock *level_clock, GameState *game_state);
void level_clock_draw(const LevelClock *level_clock, const GameState *game_state);

void level_clock_reset(LevelClock *level_clock);
void level_clock_resume(LevelClock *level_clock);
void level_clock_pause(LevelClock *level_clock);

LevelClockFormatted level_clock_format(float time);

#endif
