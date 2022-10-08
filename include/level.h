#ifndef LEVEL_H
#define LEVEL_H

#include "vec2.h"
#include "polygon.h"
#include "game_state.h"

typedef struct LevelGoal_st
{
    bool_t active;
    Rect boundary;
} LevelGoal;

typedef struct LevelObjective_st
{
    bool_t hit;
    Vec2 position;
} LevelObjective;

typedef struct Level_st
{
    Vec2 spawn_position;
    LevelGoal goal;

    size_t n_objectives;
    LevelObjective *objectives;
    size_t n_colliders;
    Polygon *colliders;
} Level;

void level_update(Level *level, GameState *game_state);
void level_draw(const Level *level, const GameState *game_state);

#endif
