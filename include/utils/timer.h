#ifndef TIMER_H
#define TIMER_H

#include "types.h"

struct Timer_st;
typedef void (*TimerTrigger_fp)(struct Timer_st *timer);

typedef struct Timer_st
{
    bool_t cycle;
    float countdown;
    TimerTrigger_fp trigger_cb;
    void *trigger_cb_ptr;

    float left;
    bool_t running;
} Timer;

void timer_update(Timer *timer, float delta);

void timer_start(Timer *timer);
void timer_stop(Timer *timer);
void timer_reset(Timer *timer);

#endif
