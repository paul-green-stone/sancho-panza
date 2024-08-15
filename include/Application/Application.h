#ifndef SANCHO_PANZA_APPLICATION_H
#define SANCHO_PANZA_APPLICATION_H

#include "../../sancho-panza.h"

/* ================================================================ */

struct application {

    Window* window;
    Timer* timer;

    int run;
};

typedef struct application App;

/* ================================================================ */

extern App* Application_new(void);

/* ================================================================ */

extern int Application_destroy(App** app);

/* ================================================================ */

extern void App_handle_input(void);

/* ================================================================ */

extern void App_handle_physics(void);

/* ================================================================ */

extern void App_handle_drawing(void);

/* ================================================================ */

#endif /* SANCHO_PANZA_APPLICATION_H */
