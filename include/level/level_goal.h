#ifndef LEVEL_GOAL_H
#define LEVEL_GOAL_H

#include "sound.h"
#include "sprite.h"
#include "game_state.h"

typedef struct LevelGoal_st
{
    float x;
    float top_y;
    float bottom_y;
    bool_t active;

    Rect bbox;
    const Sprite *inactive_sprite;
    const Sprite *active_sprite;
    Sound *hit_sfx;
    Sound *active_sfx;
} LevelGoal;

void level_goal_init(LevelGoal *goal, float x, float top_y, float bottom_y);
void level_goal_update(LevelGoal *goal, GameState *game_state);
void level_goal_draw(const LevelGoal *goal, const GameState *game_state);

#endif
