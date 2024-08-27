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
/* ==== These macros simplify the process of printing messages ==== */ 
/* ============= with appropriate prefixes and colors ============= */
/* ================================================================ */

/**
 * The `error` macro is a convenient way to print error messages using the `print_message` function.
 * It prepends the message with a red "Error:" prefix and sends the output to the specified stream (usually `stderr`).
 */
#define error(stream, format, ...) print_message((stream), ERROR, (format), __VA_ARGS__)

/**
 * The `warning` macro is used to print warning messages using the `print_message` function. It prepends the message with a yellow "Warning:" prefix and sends the output to the specified stream (usually `stdout`).
 */
#define warning(stream, format, ...) print_message((stream), WARNING, (format), __VA_ARGS__)

/**
 * The `success` macro is used to print success messages using the `print_message` function. It prepends the message with a green "Success:" prefix and sends the output to the specified stream (usually stdout).
 */
#define success(stream, format, ...) print_message((stream), SUCCESS, (format), __VA_ARGS__)

/* ================================================================ */

typedef enum {
    ARRAY,
    BOOLEAN,
    OBJECT,
    NUMBER,
    STRING,
} JSON_Entity;

typedef enum {ERROR, WARNING, SUCCESS} Message_Type;

/* ================================================================ */

/**
 * The `read_file2buffer` function reads the contents of a file specified by `name` into a dynamically allocated buffer pointed to by `buffer`.
 * If compiled with the `STRICT` option, the function will also print error messages describing any issues encountered
 * 
 * @param name A null-terminated string specifying the path to the file to be read.
 * @param buffer A pointer to a `char*` that will hold the address of the dynamically allocated buffer containing the file contents.
 * 
 * @return On success, the function returns `0`. On failure, the function returns the value of `errno` indicating the error that occurred.
 */
extern int read_file2buffer(const char* name, char** buffer);

/* ================================================================ */

/**
 * The `print_message` function prints a formatted message to the specified output stream.
 * It prepends the message with a colored prefix indicating the message type ("Error:", "Warning:", or "Success:") and resets the text color after the message.
 * 
 * @param stream A pointer to a `FILE` object representing the output stream where the message will be written.
 * If stream is `NULL`, it defaults to `stderr` for errors and `stdout` for warnings and successes.
 * @param msg_type A value of type `Message_Type` specifying the type of message to be printed. It can be one of `ERROR`, `WARNING`, or `SUCCESS`.
 * @param format A null-terminated string that specifies the format of the message. It can contain conversion specifiers and other formatting characters.
 * @param ... A variable number of arguments, which are converted and inserted in the resulting string according to the corresponding conversion specifiers in the format string.
 * 
 * @return None.
 */
extern void print_message(FILE* stream, Message_Type msg_type, const char* format, ...);

/* ================================================================ */

/**
 * The `JSON_parse` function is a wrapper around the `cJSON_Parse` function that parses a JSON-formatted string and returns a pointer to the resulting cJSON object.
 * It also handles error reporting if the parsing fails.
 * 
 * @param buffer A pointer to a null-terminated string containing the JSON data to be parsed.
 * @param root A pointer to a pointer of type `cJSON*`. This will be set to point to the parsed `cJSON` object if the parsing is successful.
 * 
 * @return `0` on success, indicating that the JSON data was parsed successfully. `-1` on failure, indicating that an error occurred during parsing.
 */
extern int JSON_parse(const char* buffer, cJSON** root);

/* ================================================================ */

/**
 * This function opens a specified file for writing and writes a given string to it.
 * It includes error handling to manage cases where the filename is not specified or the file cannot be opened,
 * with error messages printed only when compiled with the `STRICT` option.
 * 
 * @param name A pointer to a null-terminated string that specifies the name of the file to be opened.
 * @param string A pointer to a null-terminated string that contains the data to be written to the file.
 * 
 * @return `0` on success, indicating that the string was successfully written to the file. `-1` on failure, indicating that an error occurred during the operation.
 */
extern int write_to_file(const char* name, const char* string);

/* ================================================================ */

/**
 * The `extract_JSON_data` function extracts a specified JSON element from a `cJSON` object based on its name and type.
 * It verifies that the extracted element matches the expected type and handles errors appropriately.
 * 
 * @param root A pointer to a `cJSON` object representing the root of the JSON data structure from which the element will be extracted.
 * @param name A pointer to a null-terminated string that specifies the name of the JSON element to be extracted.
 * @param type A value of type `JSON_Entity` that specifies the expected type of the JSON element (`ARRAY`, `BOOLEAN`, `OBJECT`, `NUMBER`, or `STRING`).
 * @param data A pointer to a pointer of type `cJSON*`. This will be set to point to the extracted `cJSON` object if the extraction is successful.
 * 
 * @return `0` on success, indicating that the JSON element was successfully extracted and matches the expected type. `-1` on failure, indicating that an error occurred during the extraction process.
 */
extern int extract_JSON_data(const cJSON* root, const char* name, JSON_Entity type, cJSON** data);

/* ================================================================ */

/**
 * The `SP_init` function initializes the Sancho Panza application by reading a configuration file, parsing its contents,
 * and setting up the SDL2 library along with the application window and timer.
 * It handles errors gracefully and ensures that resources are properly managed.
 * 
 * @param app A pointer to a pointer of type `App*`. This will be set to point to the newly created application instance if the initialization is successful.
 * 
 * @return `0` on success, indicating that the application was successfully initialized. `-1` on failure, indicating that an error occurred during the initialization process.
 */
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

/**
 * The `serialize__SDL_Init__flags` function creates a JSON array that contains the names of the SDL subsystems that are currently initialized. 
 * It checks the initialization flags using the SDL library and populates the array accordingly.
 * 
 * @return Returns a pointer to a `cJSON` array containing the names of the initialized SDL subsystems. Returns `NULL` if the array creation fails or if an error occurs while adding items to the array.
 */
extern cJSON* serialize__SDL_Init__flags(void);

/* ================================================================ */

/**
 * The `deserialize__SDL_Init__flags` function is designed to extract and deserialize an array of flags from a JSON object, specifically for initializing SDL (Simple DirectMedia Layer).
 * It maps string representations of flags to their corresponding SDL flag values and combines them into a single `Uint32` value.
 * 
 * @param root A pointer to the root JSON object from which the flags will be extracted.
 * @param label A string label used to identify the specific array within the JSON object that contains the flags. If `NULL`, it defaults to "SDL_Init__flags".
 * 
 * @return Returns a `Uint32` value representing the combined SDL initialization flags. If the root JSON object is `NULL` or the extraction fails, it returns `0`.
 */
extern Uint32 deserialize__SDL_Init__flags(const cJSON* root, const char* label);

/* ================================================================ */

extern int SP_quit(void);

/* ================================================================ */

#endif /* SANCHO_PANZA_CORE_H */
