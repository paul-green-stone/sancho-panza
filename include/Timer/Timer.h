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

typedef struct timer* Timer_t;

/* ================================ */

extern Timer_t Timer_new(double t);

/* ================================ */

extern int Timer_destroy(Timer_t* t);

/* ================================ */

extern int Timer_set(const Timer_t t, double v);

/* ================================ */

extern int Timer_is_ready(const Timer_t t);

/* ================================ */

extern int Timer_reset(const Timer_t t);

/* ================================ */

extern void Timer_tick(const Timer_t t);

/* ================================================================ */

#endif /* SANCHO_PANZA_TIMER_H */
