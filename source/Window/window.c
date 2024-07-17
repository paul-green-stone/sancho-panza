#include "../../sancho-panza.h"

/* ================================================================ */

struct window {

    SDL_Window* w;
    SDL_Renderer* r;

    SDL_Color color;
};

typedef struct window* Window_t;

/* ================================================================ */

Window_t Window_new(const char* title, int w, int h, Uint32 wflags, Uint32 rflags) {

    Window_t new_window;

    /* ================ */
    
    /* ================================================ */
    /* ============== Memory Allocation =============== */
    /* ================================================ */

    if ((new_window = calloc(1, sizeof(struct window))) == NULL) {
        
        #ifdef STRICT
            print_error(stderr, "%s\n", strerror(errno));
        #endif

        /* ======== */
        return NULL;
    }

    /* ================================================ */
    /* ============= SDL_Window Creation ============== */
    /* ================================================ */

    if ((new_window->w = SDL_CreateWindow((title != NULL) ? title : "Sancho-Panza", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, wflags)) == NULL) {

        #ifdef STRICT
            print_error(stderr, "[%s%s%s] %s\n", BLUE, "SDL_CreateWindow", WHITE, strerror(errno));
        #endif

        free(new_window);

        /* ======== */
        return NULL;
    }

    /* ================================================ */
    /* ============ SDL_Renderer Creation ============= */
    /* ================================================ */

    if ((new_window->r = SDL_CreateRenderer(new_window->w, -1, rflags)) == NULL) {

        #ifdef STRICT
            print_error(stderr, "[%s%s%s] %s\n", BLUE, "SDL_CreateRenderer", WHITE, strerror(errno));
        #endif

        SDL_DestroyWindow(new_window->w);
        free(new_window);

        /* ======== */
        return NULL;
    }

    #ifdef STRICT
        print_success(stderr, "%s\n", "window has been created");
    #endif

    /* ======== */

    return new_window;
}

/* ================================================================ */

int Window_destroy(Window_t* w) {

    if ((w == NULL) || (*w == NULL)) {
        return -1;
    }

    SDL_DestroyWindow((*w)->w);
    SDL_DestroyRenderer((*w)->r);
    free(*w);

    *w = NULL;

    #ifdef STRICT
        print_success(stdout, "window has been destroyed\n");
    #endif

    /* ======== */

    return 0;
}

/* ================================================================ */

void Window_update(const Window_t w) {
    SDL_RenderPresent(w->r);
}

/* ================================================================ */

int Window_clear(const Window_t w) {
    return SDL_RenderClear(w->r);
}

/* ================================================================ */

int Window_set_HEX(Window_t const w, Uint32 color, Uint8 alpha) {

    w->color = (SDL_Color) {(color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, alpha};

    /* ======== */

    return SDL_SetRenderDrawColor(w->r, w->color.r, w->color.g, w->color.b, w->color.a);
}

/* ================================================================ */

int Window_set_RGBA(Window_t const w, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {

    w->color = (SDL_Color) {red, green, blue, alpha};

    /* ======== */

    return SDL_SetRenderDrawColor(w->r, w->color.r, w->color.g, w->color.b, w->color.a);
}

/* ================================================================ */
