#ifndef LEVEL_OBJECTIVE_H
#define LEVEL_OBJECTIVE_H

#include "vec2.h"
#include "types.h"
#include "game_state.h"
#include "animated_sprite.h"

typedef enum LevelObjectiveState_en
{
    LEVEL_OBJECTIVE_STATE_DEFAULT,
    LEVEL_OBJECTIVE_STATE_IN_RANGE,
    LEVEL_OBJECTIVE_STATE_ACTIVE
} LevelObjectiveState;

typedef struct LevelObjective_st
{
    bool_t active;
    Vec2 position;

    LevelObjectiveState state;
    const Sprite *default_sprite;
    const Sprite *in_range_sprite;
    AnimatedSprite active_anim_sprite;
} LevelObjective;

void level_objective_init(LevelObjective *objective, const Vec2 *position);
void level_objective_update(LevelObjective *objective, GameState *game_state);
void level_objective_draw(const LevelObjective *objective, const GameState *game_state);

void level_objective_set_active(LevelObjective *objective);
void level_objective_trigger_in_range(LevelObjective *objective);
Vec2 level_objective_center(const LevelObjective *objective);

#endif
