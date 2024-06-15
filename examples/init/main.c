#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    Window_t w;

    SP_init(&w);

    Window_destroy(&w);

    SP_quit();

    /* ======== */

    // "SDL_INIT_VIDEO", "SDL_INIT_EVENTS", "SDL_INIT_TIMER"

    return 0;
}

/* ================================================================ */
