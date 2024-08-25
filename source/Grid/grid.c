#include "../../sancho-panza.h"

/* ================================================================ */

Grid *Grid_new(int cell_width, int cell_height, int width, int height, SDL_Color *color)
{

    Grid *grid = NULL;

    /* ================ */

    if ((grid = calloc(1, sizeof(Grid))) == NULL)
    {
        return NULL;
    }

    grid->cell_w = cell_width;
    grid->cell_h = cell_height;


    grid->rows = (cell_height == 0) ? 0 : height / cell_height;
    grid->cols = (cell_width == 0) ? 0 : width / cell_width;

    grid->width = width;
    grid->height = height;

    grid->color = (color) ? *color : (SDL_Color) {0, 0, 0, 255};

    /* ======== */

    return grid;
}

/* ================================================================ */

void Grid_destroy(Grid **grid)
{

    if ((grid == NULL) || (*grid == NULL))
    {
        return;
    }

    free(*grid);
    *grid = NULL;
}

/* ================================================================ */

int Grid_draw(const Window *window, const Grid *grid, int x, int y)
{

    SDL_Color current_color;

    /* ================ */

    if (grid == NULL)
    {
        return -1;
    }

    if ((grid->width <= 0) || (grid->height <= 0))
    {
        return 0;
    }

    SDL_GetRenderDrawColor(window->r, &current_color.r, &current_color.g, &current_color.b, &current_color.a);
    Window_set_RGBA((Window *)window, grid->color.r, grid->color.g, grid->color.b, grid->color.a);

    for (size_t i = 0; i <= grid->rows; i++)
    {
        SDL_RenderDrawLine(window->r, x + 0, y + i * grid->cell_h, x + grid->width, y + i * grid->cell_h);
    }

    for (size_t i = 0; i <= grid->cols; i++)
    {
        SDL_RenderDrawLine(window->r, x + i * grid->cell_w, y + 0, x + i * grid->cell_w, y + grid->height);
    }

    Window_set_RGBA((Window *)window, current_color.r, current_color.g, current_color.b, current_color.a);

    /* ======== */

    return 0;
}

/* ================================================================ */

int Grid_update(Grid *grid, SDL_Rect *new_d)
{

    if (grid == NULL)
    {
        return -1;
    }

    if (new_d == NULL)
    {
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

cJSON *Grid_serialize(const Grid *grid) {

    cJSON *object = NULL;
    cJSON *data = NULL;

    /* ================ */

    if (grid == NULL)
    {
        return NULL;
    }

    if ((object = cJSON_CreateObject()) == NULL)
    {
        return object; // NULL
    }

    /* ================================================= */
    /* =========== Creating the `cell_width` =========== */
    /* ================================================= */

    if ((data = cJSON_CreateNumber(grid->cell_w)) == NULL)
    {
        goto END;
    }
    /* After creation was successful, immediately add it to the `object`,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(object, "cell_width", data);

    /* ================================================= */
    /* ========== Creating the `cell_height` =========== */
    /* ================================================= */

    if ((data = cJSON_CreateNumber(grid->cell_h)) == NULL)
    {
        goto END;
    }
    cJSON_AddItemToObject(object, "cell_height", data);

    /* ================================================= */
    /* =========== Creating the `grid_width` =========== */
    /* ================================================= */

    if ((data = cJSON_CreateNumber(grid->width)) == NULL)
    {
        goto END;
    }
    cJSON_AddItemToObject(object, "width", data);

    /* ================================================= */
    /* ========== Creating the `grid_height` =========== */
    /* ================================================= */

    if ((data = cJSON_CreateNumber(grid->height)) == NULL)
    {
        goto END;
    }
    cJSON_AddItemToObject(object, "height", data);

    /* ================================================= */
    /* ============= Creating the `color` ============== */
    /* ================================================= */

    if ((data = serialize__SDL_Color(&grid->color)) == NULL) {
        goto END;
    }
    cJSON_AddItemToObject(object, "color", data);

    /* ======== */

    return object;

    {
    END:
        cJSON_Delete(object);
        object = NULL;

        return object;
    }
}

/* ================================================================ */

int Grid_deserialize(const cJSON* root, Grid* grid) {

    int cell_width;
    int cell_height;
    int width;
    int height;

    SDL_Color color;

    cJSON* data;

    /* ================ */

    if (root == NULL) {
        return -1;
    }

    if (grid == NULL) {
        return -1;
    }

    /* ================================================ */
    /* =========== Extracting `cell_width` ============ */
    /* ================================================ */

    if (extract_JSON_data(root, "cell_width", NUMBER, &data) != 0) {
        cell_width = 0;
    }
    cell_width = (data != NULL) ? data->valueint : cell_width;

    /* ================================================ */
    /* =========== Extracting `cell_height` =========== */
    /* ================================================ */

    if (extract_JSON_data(root, "cell_height", NUMBER, &data) != 0) {
        cell_height = 0;
    }
    cell_height = (data != NULL) ? data->valueint : cell_height;

    /* ================================================ */
    /* ============== Extracting `width` ============== */
    /* ================================================ */

    if (extract_JSON_data(root, "width", NUMBER, &data) != 0) {
        width = 0;
    }
    width = (data != NULL) ? data->valueint : width;

    /* ================================================ */
    /* ============= Extracting `height` ============== */
    /* ================================================ */

    if (extract_JSON_data(root, "height", NUMBER, &data) != 0) {
        height = 0;
    }
    height = (data != NULL) ? data->valueint : height;

    /* ================================================ */
    /* ============= Extracting `color` =============== */
    /* ================================================ */

    if (deserialize__SDL_Color(root, &color, "color") != 0) {
        color = (SDL_Color) {0, 0, 0, 255};
    }

    /* ================================================ */

    /* If width is not set (0) and height is set, assign height to width */
    if (!cell_width && cell_height) { cell_width = cell_height; }
    /* If height is not set (0) and width is set, assign width to height. */
    if (cell_width && !cell_height) { cell_height = cell_width; }

    /* ================================================ */

    grid->cell_w = cell_width;
    grid->cell_h = cell_height;

    grid->rows = (cell_height == 0) ? 0 : height / cell_height;
    grid->cols = (cell_width == 0) ? 0 : width / cell_width;
    
    grid->width = width;
    grid->height = height;

    grid->color = color;

    /* ======== */

    return 0;
}

/* ================================================================ */
