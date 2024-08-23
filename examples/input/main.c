#include "../../sancho-panza.h"

#define Key_isPressed(key) Input_isKey_pressed(app, (key))

#define Key_wasPressed(key) Input_wasKey_just_pressed(app, (key))

#define Btn_isPressed(btn) Input_isMouseBtn_pressed(app, (btn))

#define Btn_wasPressed(btn) Input_wasBtn_just_pressed(app, (btn))

/* ================================================================ */

int main(int argc, char** argv) {

    App* app;
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

                Input_update(app);

                if (Key_isPressed(SDL_SCANCODE_W)) {
                    printf("W is pressed\n");
                }

                if (Key_wasPressed(SDL_SCANCODE_A)) {
                    printf("A was pressed\n");
                }

                if (Btn_wasPressed(0)) {
                    printf("Left mouse button has been pressed\n");
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
