#include "../../sancho-panza.h"

/* ================================================================ */

Grid* Grid_new(int cell_width, int cell_height, int width, int height, SDL_Color* color) {

    Grid* grid = NULL;

    /* ================ */

    if ((grid = calloc(1, sizeof(Grid))) == NULL) {
        return NULL;
    }

    grid->cell_w = cell_width;
    grid->cell_h = cell_height;

    grid->rows = height / cell_height;
    grid->cols = width / cell_width;

    grid->width = width;
    grid->height = height;

    grid->color = *color;

    /* ======== */

    return grid;
}

/* ================================================================ */

void Grid_destroy(Grid** grid) {

    if ((grid == NULL) || (*grid == NULL)) {
        return ;
    }

    free(*grid);
    *grid = NULL;
}

/* ================================================================ */

int Grid_draw(const Window* window, const Grid* grid, int x, int y) {

    SDL_Color current_color;

    /* ================ */

    if (grid == NULL) {
        return -1;
    }

    if ((grid->width <= 0) || (grid->height <= 0)) {
        return 0;
    }

    SDL_GetRenderDrawColor(window->r, &current_color.r, &current_color.g, &current_color.b, &current_color.a);
    Window_set_RGBA((Window*) window, grid->color.r, grid->color.g, grid->color.b, grid->color.a);

    for (size_t i = 0; i <= grid->rows; i++) {
        SDL_RenderDrawLine(window->r, x + 0, y + i * grid->cell_h, x + grid->width, y + i * grid->cell_h);
    }

    for (size_t i = 0; i <= grid->cols; i++) {
        SDL_RenderDrawLine(window->r, x + i * grid->cell_w, y + 0, x + i * grid->cell_w, y + grid->height);
    }

    Window_set_RGBA((Window*) window, current_color.r, current_color.g, current_color.b, current_color.a);

    /* ======== */

    return 0;
}

/* ================================================================ */

int Grid_update(Grid* grid, SDL_Rect* new_d) {

    if (grid == NULL) {
        return -1;
    }

    if (new_d == NULL) {
        return -1;
    }

    /* New cell width */
    grid->cell_w = (new_d->w < 2) ? grid->cell_w : new_d->x;
    /* New cell height */
    grid->cell_h = (new_d->h < 2) ? grid->cell_h : new_d->y;

    grid->width = grid->cols * grid->cell_w;
    grid->height = grid->rows * grid->cell_h;

    /* ======== */

    return 0;
}

/* ================================================================ */
