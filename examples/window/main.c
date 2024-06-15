#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    Window_t w;
    SDL_Event event;

    int quit = 0;

    /* ======== */

    if (SP_init(&w) == 0) {

            while (!quit) {

                while (SDL_PollEvent(&event)) {

                    switch (event.type) {

                    case SDL_QUIT:
                        quit = 1;
                        break ;
                    }
                }

                /* ================================ */

                Window_set_HEX(w, 0xff0000, 255);

                Window_clear(w);

                Window_update(w);
            }
        }

    Window_destroy(&w);

    SP_quit();

    /* ======== */

    return 0;
}

/* ================================================================ */
