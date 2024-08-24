#ifndef GRID_H
#define GRID_H

#include "../../sancho-panza.h"

/* ================================================================ */

typedef struct grid {

    int cell_w;
    int cell_h;

    int rows;
    int cols;

    int width;
    int height;

    SDL_Color color;
} Grid;

/* ================================ */

extern Grid* Grid_new(int cell_width, int cell_height, int width, int height, SDL_Color* color);

/* ================================================================ */

extern void Grid_destroy(Grid** grid);

/* ================================================================ */

extern int Grid_draw(const Window* window, const Grid* grid, int x, int y);

/* ================================================================ */

extern int Grid_setColor(Grid* grid, SDL_Color color);

/* ================================================================ */

extern int Grid_update(Grid* grid, SDL_Rect* new_d);

/* ================================================================ */

#endif /* GRID_H */
