#include "../../sancho-panza.h"

#define Key_isPressed(key) Input_isKey_pressed(app, (key))

#define Key_wasPressed(key) Input_wasKey_just_pressed(app, (key))

#define Btn_isPressed(btn) Input_isMouseBtn_pressed(app, (btn))

#define Btn_wasPressed(btn) Input_wasBtn_just_pressed(app, (btn))

/* ================================================================ */

int cell_size[] = {5, 10, 20};

int main(int argc, char** argv) {

    App* app;
    SDL_Event event;
    Grid* grid;

    int index = 1;
    int current_size = cell_size[index];

    SDL_Color green = {0, 255, 0, 255};

    grid = Grid_new(5, 5, 600, 400, &green);

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

            if (Key_wasPressed(SDL_SCANCODE_KP_PLUS)) {
                index = (index + 1) % (sizeof(cell_size) / sizeof(cell_size[0])); 

                SDL_Rect r = {cell_size[index], cell_size[index], grid->width, grid->height};

                Grid_update(grid, &r);
            }

            if (Key_wasPressed(SDL_SCANCODE_KP_MINUS)) {
                index = (index + 1) % (sizeof(cell_size) / sizeof(cell_size[0])); 

                SDL_Rect r = {cell_size[index], cell_size[index], grid->width, grid->height};

                Grid_update(grid, &r);
            }

            /* ================================ */

            if (Timer_is_ready(app->timer)) {

                Window_set_HEX(app->window, 0xffffff, 255);

                Window_clear(app->window);

                Grid_draw(app->window, grid, 0, 0);

                Window_update(app->window);
            }
        }
    }

    cJSON* sgrid = Grid_serialize(grid);

    Grid_destroy(&grid);

    Grid_write2file_JSON("grid", sgrid);

    cJSON_Delete(sgrid);

    Application_destroy(&app);

    SP_quit();

    /* ======== */

    return 0;
}

/* ================================================================ */
