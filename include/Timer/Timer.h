#ifndef SANCHO_PANZA_TIMER_H
#define SANCHO_PANZA_TIMER_H

#include "../../sancho-panza.h"

/* ================================================================ */

struct timer {

    /* Last frame end time in ticks */
    uint64_t pt;

    /* Delta time between two frames */
    double d;

    /* Accumulator */
    double acc;

    /* The variable can be used to specify the amount of time to wait before executing a particular action */
    double time;
};

typedef struct timer Timer;

/* ================================================================ */

extern Timer* Timer_new(double t);

/* ================================================================ */

extern int Timer_destroy(Timer** t);

/* ================================================================ */

extern int Timer_set(Timer* t, double v);

/* ================================================================ */

extern int Timer_is_ready(const Timer* t);

/* ================================================================ */

extern int Timer_reset(Timer* t);

/* ================================================================ */

extern void Timer_tick(Timer* t);

/* ================================================================ */

#endif /* SANCHO_PANZA_TIMER_H */
