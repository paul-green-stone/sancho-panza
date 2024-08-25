#ifndef SANCHO_PANZA_CORE_H
#define SANCHO_PANZA_CORE_H

#include "../../sancho-panza.h"

/* ================================================================ */

typedef struct window Window;
typedef struct application App;

/* ================================ */

/* White text color */
#define WHITE "\033[0;37m"

/* Red text color */
#define RED "\033[0;31m"

/* Cyan text color. File names */
#define CYAN  "\033[0;36m"

/* Yellow text color */
#define YELLOW "\033[0;33m"

/* Green text color */
#define GREEN "\033[0;32m"

/* Purple text color */
#define PURPLE "\033[0;35m"

/* Blue text color. Functions names */
#define BLUE "\033[0;34m"

/* ================================================================ */

enum checks {
    ARRAY,
    BOOLEAN,
    OBJECT,
    NUMBER,
    STRING,
};

/* ================================================================ */

extern int read_file2buffer(const char* name, char** buffer);

/* ================================================================ */

extern void print_error(FILE* stream, const char* format, ...);

/* ================================================================ */

extern void print_warning(FILE* stream, const char* format, ...);

/* ================================================================ */

extern void print_success(FILE* stream, const char* format, ...);

/* ================================================================ */

extern int JSON_parse(const char* buffer, cJSON** root);

/* ================================================================ */

extern int write_to_file(const char* name, const char* string);

/* ================================================================ */

extern int extract_JSON_data(const cJSON* root, const char* name, int type, cJSON** data);

/* ================================================================ */

extern int SP_init(App** app);

/* ================================================================ */

/**
 * The `serialize__SDL_Color` function takes an `SDL_Color` struct and serializes it into a `JSON` object using the cJSON library.
 * The resulting `JSON` object will have properties for each component of the color (red, green, blue, and alpha).
 * 
 * @param color A pointer to an `SDL_Color` struct containing the color values to be serialized. This parameter must not be NULL.
 * 
 * @return A pointer to a `cJSON` object representing the serialized color.
 * If the serialization fails or if the input color is `NULL`, the function returns `NULL`.
 */
extern cJSON* serialize__SDL_Color(const SDL_Color* color);

/* ================================================================ */

/**
 * The `deserialize__SDL_Color` function extracts color information from a specified `JSON` object and populates an `SDL_Color`
 * structure with the corresponding red, green, blue, and alpha components.
 * The function uses a `label` to identify the specific color object within the JSON structure.
 * The color is expected to be stored as a JSON object in the document.
 * 
 * @param root A pointer to the root `cJSON` object containing the color data. This parameter must not be `NULL`.
 * @param color A pointer to an `SDL_Color` structure where the extracted color components will be stored. This parameter must not be `NULL`.
 * @param label A string representing the key in the `JSON` object that identifies the color data. This parameter must not be `NULL`.
 * 
 * @return The function returns `0` on success, indicating that the color was successfully extracted and populated.
 * It returns `-1` on failure, indicating an error in the deserialization process.
*/
extern int deserialize__SDL_Color(const cJSON* root, SDL_Color* color, const char* label);

/* ================================================================ */

extern int SP_quit(void);

/* ================================================================ */

#endif /* SANCHO_PANZA_CORE_H */
