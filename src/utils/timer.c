#include "timer.h"

void timer_update(Timer *timer, float delta)
{
    if (!timer->running)
    {
        return;
    }

    timer->left -= delta;
    if (timer->left <= 0.0f)
    {
        if (timer->cycle)
        {
            timer_reset(timer);
        }
        else
        {
            timer_stop(timer);
        }

        timer->trigger_cb(timer);
    }
}

void timer_start(Timer *timer)
{
    timer->running = TRUE;
}

void timer_stop(Timer *timer)
{
    timer->running = FALSE;
}

void timer_reset(Timer *timer)
{
    timer->left = timer->countdown;
}
