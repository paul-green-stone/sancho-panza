#include "../../sancho-panza.h"

/* ================================================================ */

App* Application_new(void) {

    App* app;

    /* ================ */

    if ((app = calloc(1, sizeof(struct application))) == NULL) {

        #ifdef STRICT
            error(stderr, "in %s%s%s (%s%s%s)\n", BLUE, __func__, WHITE, RED, strerror(errno), WHITE);
        #endif

        /* ======== */
        return NULL;
    }

    /* Initialize the Input Manager */
    memset(app->imanager.key_states, 0, sizeof(app->imanager.key_states));
    memset(app->imanager.previous_key_states, 0, sizeof(app->imanager.previous_key_states));
    memset(app->imanager.mouse_BTN_states, 0, sizeof(app->imanager.mouse_BTN_states));

    app->run = 1;

    /* ======== */

    return app;
}

/* ================================================================ */

int Application_destroy(App** app) {

    if ((app == NULL) || (*app == NULL)) {
        return -1;
    }

    Window_destroy(&(*app)->window);
    Timer_destroy(&(*app)->timer);
    free(*app);

    *app = NULL;

    #ifdef STRICT
        success(stdout, "application has been destroyed\n", "");
    #endif

    /* ======== */
    
    return 0;
}

/* ================================================================ */
