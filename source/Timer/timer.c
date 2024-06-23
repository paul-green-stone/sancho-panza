#include "../../sancho-panza.h"

/* ================================================================ */

Timer_t Timer_new(double t) {

    Timer_t timer;

    /* ======== */

    if ((timer = calloc(1, sizeof(struct timer))) == NULL) {

        #ifdef STRICT
            print_error(stderr, "in %s%s%s (%s%s%s)\n", BLUE, __func__, WHITE, RED, strerror(errno), WHITE);
        #endif

        /* ======== */
        return NULL;
    }

    timer->pt = SDL_GetPerformanceCounter();
    timer->time = t;

    /* ======== */

    return timer;
}

/* ================================================================ */

int Timer_destroy(Timer_t* t) {

    if ((t == NULL) || (*t == NULL)) {
        return -1;
    }

    free(*t);
    *t = NULL;

    #ifdef STRICT
        print_success(stdout, "timer has been destroyed\n");
    #endif

    /* ======== */

    return 0;
}

/* ================================================================ */

int Timer_set(const Timer_t t, double v) {

    if (t == NULL) {
        return -1;
    }

    if (v <= 0) {
        return -1;
    }

    t->time = v;

    return 0;
}

/* ================================================================ */

int Timer_is_ready(const Timer_t t) {
    return (t != NULL) ? t->acc >= t->time : 0;
}

/* ================================================================ */

int Timer_reset(const Timer_t t) {

    if (t == NULL) {
        return -1;
    }

    t->acc = 0;

    return 0;
}

/* ================================================================ */

void Timer_tick(const Timer_t t) {

    /* Current frame start */
    uint64_t current_ticks = 0;

    uint64_t delta = 0;

    /* ======== */

    if (t == NULL) {
        return ;
    }

    current_ticks = SDL_GetPerformanceCounter();

    /* How many ticks have passed since the last frame */
    delta = current_ticks - t->pt;

    t->pt = current_ticks;

    /* Compute how many SECONDS have passed since the previous frame */
    t->d = (float) delta / (float) SDL_GetPerformanceFrequency();

    t->acc += t->d;
}

/* ================================================================ */
