#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    App* app;

    SDL_Init(0);

    cJSON* data = serialize__SDL_Init__flags();

    char* string = cJSON_Print(data);

    cJSON_Delete(data);

    write_to_file("sdl.json", string);
    free(string);

    SDL_Quit();

    /* ======== */

    return 0;
}

/* ================================================================ */
