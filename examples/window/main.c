#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    App_t app;
    SDL_Event event;

    /* ======== */

    if (SP_init(&app) == 0) {

            while (app->run) {

                Timer_tick(app->timer);

                while (SDL_PollEvent(&event)) {

                    switch (event.type) {

                    case SDL_QUIT:
                        app->run = 0;
                        break ;
                    }
                }

                /* ================================ */

                if (Timer_is_ready(app->timer)) {

                    Window_set_HEX(app->window, 0xff0000, 255);

                    Window_clear(app->window);

                    Window_update(app->window);
                }
            }
        }

    Application_destroy(&app);

    SP_quit();

    /* ======== */

    return 0;
}

/* ================================================================ */
