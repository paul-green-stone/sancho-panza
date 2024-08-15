#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    App_t app;

    SP_init(&app);

    Application_destroy(&app);

    SP_quit();

    /* ======== */

    return 0;
}

/* ================================================================ */
