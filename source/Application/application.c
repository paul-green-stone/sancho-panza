#include "../../sancho-panza.h"

/* ================================================================ */

App_t Application_new(void) {

    App_t app;

    /* ======== */

    if ((app = calloc(1, sizeof(struct application))) == NULL) {

        #ifdef STRICT
            print_error(stderr, "in %s%s%s (%s%s%s)\n", BLUE, __func__, WHITE, RED, strerror(errno), WHITE);
        #endif

        /* ======== */
        return NULL;
    }

    app->run = 1;

    /* ======== */

    return app;
}

/* ================================================================ */

int Application_destroy(App_t* app) {

    if ((app == NULL) || (*app == NULL)) {
        return -1;
    }

    Window_destroy(&(*app)->window);
    Timer_destroy(&(*app)->timer);
    free(*app);

    *app = NULL;

    #ifdef STRICT
        print_success(stdout, "application has been destroyed\n");
    #endif

    /* ======== */
    
    return 0;
}

/* ================================================================ */
