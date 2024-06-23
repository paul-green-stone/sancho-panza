#include "../../sancho-panza.h"

#define DEFAULT_SDL ".config.json"

/* ================================================================ */

/**
 * A struct containing information necessary to create a window of `Window_t` type
 */
struct window_options {

    Uint32 wflags;
    Uint32 rflags;

    int width;
    int height;

    char* title;
};

/* ================================================================ */
/* ================= Here goes the look-up tables ================= */
/* ================================================================ */

static struct mapping_entry {
    const char* name;
    Uint32 flag;
};

/* ================================ */

struct mapping_entry SDL_Init_Flags[] = {
    {"SDL_INIT_TIMER", SDL_INIT_TIMER},
    {"SDL_INIT_AUDIO", SDL_INIT_AUDIO},
    {"SDL_INIT_VIDEO", SDL_INIT_VIDEO},
    {"SDL_INIT_JOYSTICK", SDL_INIT_JOYSTICK},
    {"SDL_INIT_HAPTIC", SDL_INIT_HAPTIC},
    {"SDL_INIT_GAMECONTROLLER", SDL_INIT_GAMECONTROLLER},
    {"SDL_INIT_EVENTS", SDL_INIT_EVENTS},
    {"SDL_INIT_EVERYTHING", SDL_INIT_EVERYTHING},
    {"SDL_INIT_NOPARACHUTE", SDL_INIT_NOPARACHUTE}
};

/* ================================ */

struct mapping_entry SDL_CreateWindow_Flags[] = {
    {"SDL_WINDOW_FULLSCREEN", SDL_WINDOW_FULLSCREEN},
    {"SDL_WINDOW_OPENGL", SDL_WINDOW_OPENGL},
    {"SDL_WINDOW_SHOWN", SDL_WINDOW_SHOWN},
    {"SDL_WINDOW_HIDDEN", SDL_WINDOW_HIDDEN},
    {"SDL_WINDOW_BORDERLESS", SDL_WINDOW_BORDERLESS},
    {"SDL_WINDOW_RESIZABLE", SDL_WINDOW_RESIZABLE},
    {"SDL_WINDOW_MINIMIZED", SDL_WINDOW_MAXIMIZED},
    {"SDL_WINDOW_MOUSE_GRABBED", SDL_WINDOW_MOUSE_GRABBED},
    {"SDL_WINDOW_INPUT_FOCUS", SDL_WINDOW_INPUT_FOCUS},
    {"SDL_WINDOW_MOUSE_FOCUS", SDL_WINDOW_MOUSE_FOCUS},
    {"SDL_WINDOW_FULLSCREEN_DESKTOP", ( SDL_WINDOW_FULLSCREEN | 0x00001000 )},
    {"SDL_WINDOW_FOREIGN", SDL_WINDOW_FOREIGN},
    {"SDL_WINDOW_ALLOW_HIGHDPI", SDL_WINDOW_ALLOW_HIGHDPI},
    {"SDL_WINDOW_MOUSE_CAPTURE", SDL_WINDOW_MOUSE_CAPTURE},
    {"SDL_WINDOW_ALWAYS_ON_TOP", SDL_WINDOW_ALWAYS_ON_TOP},
    {"SDL_WINDOW_SKIP_TASKBAR", SDL_WINDOW_SKIP_TASKBAR},
    {"SDL_WINDOW_UTILITY", SDL_WINDOW_UTILITY},
    {"SDL_WINDOW_TOOLTIP", SDL_WINDOW_TOOLTIP},
    {"SDL_WINDOW_POPUP_MENU", SDL_WINDOW_POPUP_MENU},
    {"SDL_WINDOW_KEYBOARD_GRABBED", SDL_WINDOW_KEYBOARD_GRABBED},
    {"SDL_WINDOW_VULKAN", SDL_WINDOW_VULKAN},
    {"SDL_WINDOW_METAL", SDL_WINDOW_METAL},
    {"SDL_WINDOW_INPUT_GRABBED", SDL_WINDOW_MOUSE_GRABBED}
};

/* ================================ */

struct mapping_entry SDL_CreateRenderer_flags[] = {
    {"SDL_RENDERER_SOFTWARE", SDL_RENDERER_SOFTWARE},
    {"SDL_RENDERER_ACCELERATED", SDL_RENDERER_ACCELERATED},
    {"SDL_RENDERER_PRESENTVSYNC", SDL_RENDERER_PRESENTVSYNC},
    {"SDL_RENDERER_TARGETTEXTURE", SDL_RENDERER_TARGETTEXTURE}
};

/* ================================================================ */
/* ================== Static, internal functions ================== */
/* ================================================================ */

/**
 * The function maps a string to its corresponding (SDL) flag.
 * To achieve this, it requires information about the table to search in,
 * its size, and the string to be converted into a flag.
 * If the flag is unrecognized, the function will return 0.
 * 
 * \param[in] lookup_table an array holding mappings of strings to flags
 * \param[in] string_flag a string equivalent of the flag to be searched in the `lookup_table`
 * \param[in] table_size number of elements, flags, in the `lookup_table`
 * 
 * \return The function returns a valid SDL flag upon success, and 0 to indicate an error if an unrecognized flag is encountered..
 */
static Uint32 SDL_lookup_flag(const struct mapping_entry* table, const char* string_flag, size_t table_size) {

    size_t i;

    /* ======== */

    for (i = 0; i < table_size; i++) {

        if (strcmp(string_flag, (table + i)->name) == 0) {
            return (table + i)->flag;
        }
    }

    /* ======== */

    print_warning(stdout, "unrecognized flags (%s) are simply ignored\n", string_flag);

    /* ======== */

    return 0;
}

/* ================================ */

/**
 * The function retrieves the flags required for the `SDL_Init` function to initialize the necessary subsystems.
 * These flags are extracted from a `cJSON` object,
 * which is obtained by reading a `.json` file into a buffer and parsing the buffer.
 * Subsequently, the function returns the bitwise OR (|) of the flags after mapping
 * their string equivalents to the corresponding SDL macros.
 * 
 * \param[in] root a string parsed by the `cJSON_Parse` function
 * 
 * \return The function returns the bitwise OR (|) of the flags after mapping their string equivalents to the corresponding SDL macros, or 0 if an error occurs. This result is used for initializing the default SDL subsystems.
 */
static Uint32 SDL_Init_get_flags(cJSON* root) {

    Uint32 SDL_flags = 0;

    cJSON* array;
    size_t array_size;

    size_t i;

    /* ================================================================ */
    /* ==================== Extracting `SDL` array ==================== */
    /* ================================================================ */

    if (extract_JSON_data(root, "SDL", ARRAY, &array) != 0) {
        return SDL_flags;
    }

    array_size = cJSON_GetArraySize(array);

    /* ================================ */

    for (i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(array, i);

        /* Map a string to its equivalent flag */
        SDL_flags |= SDL_lookup_flag(SDL_Init_Flags, array_elm->valuestring, sizeof(SDL_Init_Flags) / sizeof(SDL_Init_Flags[0]));
    }

    /* ======== */

    return SDL_flags;
}

/* ================================ */

/**
 * The function retrieves information necessary for creating the application,
 * including window flags (used in `SDL_CreateWindow`), renderer flags (used in `SDL_CreateRenderer`),
 * window width, and window height, among other things.
 * 
 * \param[in] root parsed string (JSON)
 * \param[out] options a pointer to the 'window_props' structure to store properties in
 * 
 * \return The function returns 0 on success, nonzero value on failure.
 */
static int get_window_options(cJSON* root, struct window_options* options) {

    cJSON* object;
    cJSON* data;
    cJSON* array;

    size_t array_size;
    size_t i;

    /* ================================================================ */
    /* ================ Extracting the `window` object ================ */
    /* ================================================================ */

    if (extract_JSON_data(root, "Window", OBJECT, &object) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */

        return -1;
    }

    /* ================================================================ */
    /* ================= Extracting the window title ================== */
    /* ================================================================ */

    if (extract_JSON_data(object, "title", STRING, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */

        return -1;
    }

    /* == Memory allocation for the title. Do not forget to free it === */
    if ((options->title = malloc(strlen(data->valuestring) * sizeof(char))) == NULL) {

        print_error(stderr, "Sancho-Panza initialization failure (%s%s%s)\n", RED, strerror(errno), WHITE);

        /* ======== */
        return -1;
    }

    /* ====================== Copying the title ======================= */
    strncpy(options->title, data->valuestring, strlen(data->valuestring));

    /* ================================================================ */
    /* ============ Extracting the window width and height ============ */
    /* ================================================================ */

    if (extract_JSON_data(object, "width", NUMBER, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */

        return -1;
    }

    options->width = data->valueint;

    /* ================ */

    if (extract_JSON_data(object, "height", NUMBER, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */

        return -1;
    }

    options->height = data->valueint;

    /* ================================================================ */
    /* ================= Extracting the window flags ================== */
    /* ================================================================ */

    if (extract_JSON_data(object, "wflags", ARRAY, &array) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */

        return -1;
    }

    array_size = cJSON_GetArraySize(array);

    for (i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(array, i);

        /* Map a string to its equivalent flag */
        options->wflags |= SDL_lookup_flag(SDL_CreateWindow_Flags, array_elm->valuestring, sizeof(SDL_CreateWindow_Flags) / sizeof(SDL_CreateWindow_Flags[0]));
    }

    /* ================================================================ */
    /* ================ Extracting the renderer flags ================= */
    /* ================================================================ */

    /* Extracting the renderer flags */
    if (extract_JSON_data(object, "rflags", ARRAY, &array) != 0) {
        return -1;
    }

    array_size = cJSON_GetArraySize(array);

    for (i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(array, i);

        /* Map a string to its equivalent flag */
        options->rflags |= SDL_lookup_flag(SDL_CreateRenderer_flags, array_elm->valuestring, sizeof(SDL_CreateRenderer_flags) / sizeof(SDL_CreateRenderer_flags[0]));
    }

    /* ======== */

    return 0;
}

/* ================================================================ */
/* ================================================================ */
/* ================================================================ */

static struct cJSON_checker_mapping_entry {

    const int type;
    const char* string;
    cJSON_bool (*function)(const cJSON* const); 
};

/* ================================ */

static struct cJSON_checker_mapping_entry cJSON_checkers[] = {

    {ARRAY, "array", cJSON_IsArray},
    {BOOLEAN, "boolean", cJSON_IsBool},
    {OBJECT, "object", cJSON_IsObject},
    {NUMBER, "number", cJSON_IsNumber},
    {STRING, "string", cJSON_IsString}
};

/* ================================ */

/* Size of the array of `cJSON_Is` checkers */
static size_t size = sizeof(cJSON_checkers) / sizeof(cJSON_checkers[0]);

/* ================================ */

static const char* extract_checker_name(int type) {

    size_t i;

    /* ======== */

    for (i = 0; i < size; i++) {

        if (cJSON_checkers[i].type == type) {
            return cJSON_checkers[i].string;
        }
    }

    /* ======== */

    return "unknown";
}

static int create_default_init_file(void) {

    cJSON* root;
    cJSON* flags;
    cJSON* flag;
    cJSON* window;

    char* text = NULL;

    size_t i;

    char* default_SDL_flags[] = {"SDL_INIT_VIDEO", "SDL_INIT_EVENTS", "SDL_INIT_TIMER"};
    size_t size = sizeof(default_SDL_flags) / sizeof(default_SDL_flags[0]);

    char* default_window_flags[] = {"SDL_WINDOW_SHOWN"};
    size_t w_size = sizeof(default_window_flags) / sizeof(default_window_flags[0]);

    char* default_renderer_flags[] = {"SDL_RENDERER_ACCELERATED"};
    size_t r_size = sizeof(default_renderer_flags) / sizeof(default_renderer_flags[0]);

    errno = 0;

    /* ================================ */

     /* Creating a root object as a container for dependent elements */
    if ((root = cJSON_CreateObject()) == NULL) {
        /* Most likely it's a memory allocation error */

        goto ON_ERROR;
    }

    if ((flags = cJSON_CreateArray()) == NULL) {
        /* Most likely it's a memory allocation error */

        goto ON_ERROR;
    }

    /* No return value check? */
    cJSON_AddItemToObject(root, "SDL", flags);

    for (i = 0; i < size; i++) {

        if ((flag = cJSON_CreateString(default_SDL_flags[i])) == NULL) {
            goto ON_ERROR;
        }

        cJSON_AddItemToArray(flags, flag);
    }

    /* ================================================================ */
    /* =================== Creating a window object =================== */
    /* ================================================================ */

    if ((window = cJSON_CreateObject()) == NULL) {
        
        /* Most likely it's a memory allocation error */
        goto ON_ERROR;
    }

    flag = cJSON_CreateString("Sancho Panza");
    cJSON_AddItemToObject(window, "title", flag);

    /* ================================= */

    flag = cJSON_CreateNumber(600);
    cJSON_AddItemToObject(window, "width", flag);

    flag = cJSON_CreateNumber(400);
    cJSON_AddItemToObject(window, "height", flag);

    /* ================================= */
    /* Creating an array of window flags */
    /* ================================= */

    if ((flags = cJSON_CreateArray()) == NULL) {

        /* Most likely it's a memory allocation error */
        goto ON_ERROR;
    }

    for (i = 0; i < w_size; i++) {

        if ((flag = cJSON_CreateString(default_window_flags[i])) == NULL) {
            goto ON_ERROR;
        }

        cJSON_AddItemToArray(flags, flag);
    }

    /* Add window flags to the window object */
    cJSON_AddItemToObject(window, "wflags", flags);

    /* ================================================================ */
    /* ================================================================ */
    /* ================================================================ */

    /* =================================== */
    /* Creating an array of renderer flags */
    /* =================================== */

    if ((flags = cJSON_CreateArray()) == NULL) {

        /* Most likely it's a memory allocation error */
        goto ON_ERROR;
    }

    for (i = 0; i < r_size; i++) {

        if ((flag = cJSON_CreateString(default_renderer_flags[i])) == NULL) {
            goto ON_ERROR;
        }

        cJSON_AddItemToArray(flags, flag);
    }

    /* Add renderer flags to the window object */
    cJSON_AddItemToObject(window, "rflags", flags);

    /* ================================ */

    /* Add window object to the root object */
    cJSON_AddItemToObject(root, "Window", window);

    /* ================================ */

    /* Convert to `char*`*/
    if ((text = cJSON_Print(root)) == NULL) {
        goto ON_ERROR;
    }

    if (write_to_file(".config.json", text) != 0) {
        goto ON_ERROR;
    }

    /* ================ */

    free(text);
    cJSON_Delete(root);

    /* ======== */

    return 0;

    ON_ERROR:
    {
        #ifdef STRICT
            print_error(stderr, "%s (%s%s%s)\n", "unable to create a default configuration file", RED, errno != 0 ? strerror(errno) : "", WHITE);
        #endif

        free(text);
        cJSON_Delete(root);

        /* ======== */

        return -1;
    }
}

/* ================================================================ */
/* ============== Definitions of external functions =============== */
/* ================================================================ */

int read_file2buffer(const char* name, char** buffer) {

    FILE* file;

    size_t number_of_bytes;
    size_t bytes_read;

    /* ======== */

    if ((file = fopen(name, "r")) == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s (%s%s%s)\n", strerror(errno), CYAN, name, WHITE);
        #endif

        /* ======== */
        return errno;
    }

    /* ====== Figuring out how many bytes of memory to allocate ======= */
    fseek(file, 0L, SEEK_END);

    /* Do not forget about the `\0` character */
    number_of_bytes = ftell(file) + 1;

    fseek(file, 0L, SEEK_SET);

    /* ====================== Actual allocation ======================= */
    if ((*buffer = (char*) calloc(number_of_bytes, sizeof(char))) == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s\n", strerror(errno));
        #endif

        fclose(file);

        /* ======== */
        return errno;
    }

    /* =================== Reading into the buffer ==================== */
    bytes_read = fread(*buffer, sizeof(char), number_of_bytes, file);

    *(*buffer + bytes_read) = '\0';

    fclose(file);

    /* ======== */

    return 0;
}

/* ================================================================ */

void print_error(FILE* stream, const char* format, ...) {

    va_list args;
    va_start(args, format);

    fprintf(stream, "%s%s%s: ", RED, "Error", WHITE);
    vfprintf(stream, format, args);

    va_end(args);
}

/* ================================================================ */

void print_warning(FILE* stream, const char* format, ...) {

    va_list args;
    va_start(args, format);

    fprintf(stream, "%s%s%s: ", YELLOW, "Warning", WHITE);
    vfprintf(stream, format, args);

    va_end(args);
}

/* ================================================================ */

void print_success(FILE* stream, const char* format, ...) {

    va_list args;
    va_start(args, format);

    fprintf(stream, "%s%s%s: ", GREEN, "Success", WHITE);
    vfprintf(stream, format, args);

    va_end(args);
}

/* ================================================================ */

int JSON_parse(const char* buffer, cJSON** root) {

    const char* error_ptr;

    /* ======== */

    if ((*root = cJSON_Parse(buffer)) == NULL) {

        if ((error_ptr = cJSON_GetErrorPtr()) != NULL) {
            
            print_error(stderr, "before %s%s%s\n", PURPLE, error_ptr, WHITE);

            /* ======== */
            return -1;
        }
    }

    /* ======== */

    return 0;
}

/* ================================================================ */

int write_to_file(const char* name, const char* string) {

    FILE* file;

    /* ======== */

    if (name == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s [%s%s%s]\n", "filename is not specified", BLUE, __func__, WHITE);
        #endif

        /* ======== */
        return -1;
    }

    if ((file = fopen(name, "w+")) == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s [%s%s%s]\n", strerror(errno), BLUE, __func__, WHITE);
        #endif

        /* ======== */
        return -1;
    }

    fprintf(file, "%s", string);

    fclose(file);

    /* ======== */

    return 0;
}

/* ================================================================ */

int extract_JSON_data(const cJSON* root, const char* name, int type, cJSON** data) {

    cJSON_bool (*check)(const cJSON* const);

    /* ======== */

    if ((type < 0) || (type > size)) {

        #ifdef STRICT
            print_error(stderr, "unsupported type\n");
        #endif

        /* ======== */
        return -1;
    }

    check = cJSON_checkers[type].function;

    if (root == NULL) {
        return -1;
    }

    if ((*data = cJSON_GetObjectItemCaseSensitive(root, name)) == NULL) {

        #ifdef STRICT
            print_error(stderr, "there is no such element in the document [%s%s%s]\n", PURPLE, name, WHITE);
        #endif

        /* ======== */
        return -1;
    }

    if (check(*data) != 1) {

        #ifdef STRICT
            print_error(stderr, "the found element doesn't belong to the specified type [%s%s%s != %s]\n", PURPLE, name, WHITE, extract_checker_name(type));
        #endif

        *data = NULL;

        /* ======== */
        return -1;
    }

    /* ========*/

    return 0;
}

/* ================================================================ */

int SP_init(App_t* app) {

    char* buffer;
    cJSON* root;

    Uint32 SDL_flags;
    struct window_options opts = {0, 0, 0, 0, 0};

    int status = 0;

    /* ======== */

    while ((status = read_file2buffer(DEFAULT_SDL, &buffer)) != 0) {

        if (status == ENOENT) {

            /* The error message from `read_file2buffer` is outputed as well (if STRICT) */
            print_warning(stdout, "[%s%s%s] missing configuration file. Creating a default one...\n", BLUE, __func__, WHITE);
        }

        if ((status = create_default_init_file()) != 0) {

            /* The error message from `read_file2buffer` is outputed (if STRICT) */
            return -1;
        }
        else {
            print_success(stdout, "file (%s%s%s) has been created\n", CYAN, DEFAULT_SDL, WHITE);
        }
    }

    /* ================================ */

    if ((status = JSON_parse(buffer, &root)) != 0) {

        /* `JSON_parse` prints the error location to the console */
        goto ON_ERROR;
    }

    SDL_flags = SDL_Init_get_flags(root);

    /* ======================= Initializing SDL ======================= */
    if ((status = SDL_Init(SDL_flags)) != 0) {

        print_error(stderr, "Initialization failed. Unable to initialize SDL (%s%s%s)\n", RED, SDL_GetError(), WHITE);

        goto ON_ERROR;
    }

    /* ===================== Getting window flags ===================== */
    if (get_window_options(root, &opts) != 0) {

        print_error(stderr, "Initialization failed. Unable to get window flags\n");

        goto ON_ERROR;
    }

    if ((*app = Application_new()) == NULL) {
        goto ON_ERROR;
    }

    if (((*app)->window = Window_new(opts.title, opts.width, opts.height, opts.wflags, opts.rflags)) == NULL) {

        Application_destroy(app);

        goto ON_ERROR;
    }

    if (((*app)->timer = Timer_new(1.0f / 60)) == NULL) {

        Application_destroy(app);

        goto ON_ERROR;
    }

    /* ================================ */

    free(opts.title);
    free(buffer);
    cJSON_Delete(root);

    /* ======== */

    return status;

    ON_ERROR:
    {

        free(buffer);
        cJSON_Delete(root);
        SDL_Quit();

        return -1;
    }
}

/* ================================================================ */

int SP_quit(void) {

    SDL_Quit();

    /* ======== */

    return 0;
}

/* ================================================================ */

#undef DEFAULT_SDL
#undef STRICT
