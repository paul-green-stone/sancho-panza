#ifndef GRID_H
#define GRID_H

#include "../../sancho-panza.h"

/**
 * The macro simplifies the process of writing a serialized grid represented as a `cJSON` object to a specified file.
 * It utilizes the `write_to_file` function to handle the file writing operation
 * and the `cJSON_Print` function to convert the cJSON object into a string format suitable for storage.
 * 
 * @param filename A string representing the path to the file where the JSON data should be written. This parameter must be a valid file path.
 * @param serialized_grid A pointer to a `cJSON` object that contains the serialized representation of the grid.
 * This object should be created using the `Grid_serialize` function.
 * 
 * @return The macro does not have a return value. It directly calls the `write_to_file` function, which returns 0 on success, or -1 on failure.
 */
#define Grid_write2file_JSON(filename, serielized_grid) \
    do { \
        char* final_filename = NULL; \
        size_t filename_length = strlen(filename); \
        char* string = cJSON_Print((serielized_grid)); \
        \
        if (filename_length < 5 || strcmp(filename + filename_length - 5, ".json") != 0) { \
            \
            final_filename = malloc(filename_length + 6); /* 5 for ".json" + 1 for terminator */ \
            sprintf(final_filename, "%s.json", filename); \
        } \
        else { \
            final_filename = malloc(filename_length + 1); \
            strcpy(final_filename, filename); \
        }\
        \
        write_to_file((final_filename), string); \
        free(final_filename); \
        free(string); \
    } while (0)

/* ================================================================ */

typedef struct grid {

    int cell_w;
    int cell_h;

    /* The number of rows in the grid is computed automatically; it is not serialized */
    int rows;
    /* The number of columns in the grid is computed automatically; it is not serialized */
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

/**
 * The `Grid_serialize` function converts a `Grid` structure into a JSON object using the cJSON library.
 * It creates a JSON object that contains the properties of the grid, including cell dimensions and overall grid size.
 * Note that information about the number of rows and columns is not serialized;
 * this information is computed automatically when the grid is created or loaded from a file.
 * 
 * @param grid A pointer to a `Grid` structure that contains the properties to be serialized. The structure must not be `NULL`.
 * 
 * @return A pointer to a `cJSON` object representing the serialized grid.
 * If the serialization fails or if the input `grid` is `NULL`, the function returns `NULL`.
 */
extern cJSON* Grid_serialize(const Grid* grid);

/* ================================================================ */

/**
 * The `Grid_deserialize` function deserializes a grid from a JSON object and populates a `Grid` structure with the extracted data.
 * It extracts the cell width, cell height, grid width, grid height, and color information from the `JSON` object.
 * If any of the required values are missing or invalid, default values are assigned.
 * 
 * @param root A pointer to the root cJSON object containing the grid data. This parameter must not be `NULL`.
 * @param grid A pointer to a `Grid` structure where the deserialized grid data will be stored. This parameter must not be `NULL`.
 * 
 * @return The function returns `0` on success, indicating that the grid was successfully deserialized.
 * It returns `-1` on failure, indicating an error in the deserialization process.
 */
extern int Grid_deserialize(const cJSON* root, Grid* grid);

/* ================================================================ */

#endif /* GRID_H */
