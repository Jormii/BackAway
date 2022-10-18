#ifndef LEVEL_H
#define LEVEL_H

#include "vec2.h"
#include "polygon.h"
#include "game_state.h"
#include "level_goal.h"
#include "level_objective.h"

typedef struct Level_st
{
    Vec2 spawn_position;
    LevelGoal goal;

    size_t n_objectives;
    LevelObjective *objectives;
    size_t n_colliders;
    Polygon *colliders;
} Level;

bool_t level_load(Level *level, const char *path);
void level_free(Level *level);
void level_update(Level *level, GameState *game_state);
void level_draw(const Level *level, const GameState *game_state);

void level_reset(Level *level);

#endif
