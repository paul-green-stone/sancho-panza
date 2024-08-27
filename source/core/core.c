#include "../../sancho-panza.h"

#define DEFAULT_SDL ".config.json"

#define BUFFER_SIZE 256

/* ================================================================ */
/* ============= Here are the arrays that map strings ============= */
/* =============== to their corresponding SDL flags =============== */
/* ================================================================ */

struct mapping_entry {
    const char* name;
    Uint32 flag;
};

/* ================================================================ */
/* ======================== SDL_Init Flags ======================== */
/* ================================================================ */

static const struct mapping_entry SDL_Init_Flags[] = {
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

/* ================================================================ */
/* ======================= SDL_Window Flags ======================= */
/* ================================================================ */

static const struct mapping_entry SDL_CreateWindow_Flags[] = {
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

/* ================================================================ */
/* ====================== SDL_Renderer Flags ====================== */
/* ================================================================ */

static const struct mapping_entry SDL_CreateRenderer_flags[] = {
    {"SDL_RENDERER_SOFTWARE", SDL_RENDERER_SOFTWARE},
    {"SDL_RENDERER_ACCELERATED", SDL_RENDERER_ACCELERATED},
    {"SDL_RENDERER_PRESENTVSYNC", SDL_RENDERER_PRESENTVSYNC},
    {"SDL_RENDERER_TARGETTEXTURE", SDL_RENDERER_TARGETTEXTURE}
};

















/* 
 * This structure defines a mapping entry for cJSON checkers.
 * Each entry associates a specific type with a corresponding string
 * representation and a function pointer that checks whether a cJSON 
 * object matches the specified type.
 */
struct cJSON_checker_mapping_entry {
    
    /* The type identifier (e.g., ARRAY, BOOLEAN, etc.) */
    const int type;

    /* The string representation of the type */
    const char* string;
    
    /* Pointer to the function that checks the type */
    cJSON_bool (*function)(const cJSON* const);
};

/* ================================================================ */

/* 
 * The cJSON_checkers array contains a collection of mapping entries.
 * Each entry in this array links a type identifier to its string 
 * representation and the corresponding function that verifies the 
 * type of a cJSON object. This mapping is useful for dynamically 
 * checking the types of JSON elements during parsing or validation.
 */
struct cJSON_checker_mapping_entry cJSON_checkers[] = {
    {ARRAY, "array", cJSON_IsArray},
    {BOOLEAN, "boolean", cJSON_IsBool},
    {OBJECT, "object", cJSON_IsObject},
    {NUMBER, "number", cJSON_IsNumber},
    {STRING, "string", cJSON_IsString}
};

















struct window_options {
    
    /* Window flags used in the `SDL_CreateWindow` function */
    Uint32 wflags;
    /* Renderer flags used in the `SDL_CreateRenderer` function */
    Uint32 rflags;

    int width;
    int height;

    char* title;
};

/* ================================================================ */
/* ================== Static, internal functions ================== */
/* ================================================================ */

/**
 * The `SDL_lookup_flag` function searches a mapping table for a specific string flag and returns its corresponding flag value.
 * If the string flag is not recognized, it issues a warning and returns a default success value.
 * 
 * @param table A pointer to an array of `mapping_entry` structures that defines the mapping between string flags and their corresponding flag values.
 * @param string_flag A pointer to a null-terminated string representing the flag to be looked up in the mapping table.
 * @param table_size The size of the mapping table (i.e., the number of entries in the table).
 * 
 * @return Returns the corresponding flag value of type `Uint32` if the `string_flag` is found in the mapping table. Returns `EXIT_SUCCESS` if the `string_flag` is not recognized.
 */
static Uint32 SDL_lookup_flag(const struct mapping_entry* table, const char* string_flag, size_t table_size) {

    size_t i;
    
    /* ======================================== */
    /* ====== Traversing the given table ====== */
    /* ==== in order to find the flag value === */
    /* ======================================== */

    for (i = 0; i < table_size; i++) {

        if (strcmp(string_flag, (table + i)->name) == 0) {
            return (table + i)->flag;
        }
    }

    /* ======== */

    warning(stdout, "unrecognized flags (%s) are simply ignored\n", string_flag);

    /* ======== */

    return EXIT_SUCCESS;
}

/* ================================================================ */

static int get_window_options(const cJSON* root, struct window_options* options) {
    
    /* Corresponds to the `Window` object in the configuration file */
    cJSON* object;
    /* A simple container for extracted data */
    cJSON* data;

    size_t array_size;
    size_t i;

    /* ================================================ */
    /* ======== Extracting the `Window` object ======== */
    /* =============== from the `root` ================ */
    /* ================================================ */

    if (extract_JSON_data(root, "Window", OBJECT, &object) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */
        
        /* ======== */
        return -1;
    }

    /* ================================================ */
    /* ========= Extracting the `title` string ======== */
    /* ============ from the Window object ============ */
    /* ================================================ */

    if (extract_JSON_data(object, "title", STRING, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */
        
        /* ======== */
        return -1;
    }

    /* ================================================ */
    /* =========== Allocating enough memory =========== */
    /* ============== to store the title ============== */
    /* ================================================ */
    
    if ((options->title = malloc(strlen(data->valuestring) * sizeof(char))) == NULL) {
        error(stderr, "Sancho-Panza initialization failure (%s%s%s)\n", RED, strerror(errno), WHITE);

        /* ======== */
        return -1;
    }

    /* Copying the title */
    strncpy(options->title, data->valuestring, strlen(data->valuestring));

    /* ================================================ */
    /* ==== Extracting the window width and height ==== */
    /* ============ from the Window object ============ */
    /* ================================================ */

    if (extract_JSON_data(object, "width", NUMBER, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */
        
        /* ======== */
        return -1;
    }

    options->width = data->valueint;

    /* ================ */

    if (extract_JSON_data(object, "height", NUMBER, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */

        return -1;
    }

    options->height = data->valueint;

    /* ================================================ */
    /* ========= Extracting the window flags ========== */
    /* ============ from the Window object ============ */
    /* ================================================ */

    if (extract_JSON_data(object, "SDL_Window__flags", ARRAY, &data) != 0) {
        /* `extract_JSON_data` prints the error message here if STRICT */
        
        /* ======== */
        return -1;
    }

    array_size = cJSON_GetArraySize(data);

    for (i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(data, i);

        /* Map a string to its equivalent flag */
        options->wflags |= SDL_lookup_flag(SDL_CreateWindow_Flags, array_elm->valuestring, sizeof(SDL_CreateWindow_Flags) / sizeof(SDL_CreateWindow_Flags[0]));
    }

    /* ================================================ */
    /* ======== Extracting the renderer flags ========= */
    /* ============ from the Window object ============ */
    /* ================================================ */

    /* Extracting the renderer flags */
    if (extract_JSON_data(object, "SDL_Renderer__flags", ARRAY, &data) != 0) {
        return -1;
    }

    array_size = cJSON_GetArraySize(data);

    for (i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(data, i);

        /* Map a string to its equivalent flag */
        options->rflags |= SDL_lookup_flag(SDL_CreateRenderer_flags, array_elm->valuestring, sizeof(SDL_CreateRenderer_flags) / sizeof(SDL_CreateRenderer_flags[0]));
    }

    /* ======== */

    return EXIT_SUCCESS;
}

/* ================================================================ */

static const char* extract_checker_name(int type) {

    size_t i;
    /* Size of the array of `cJSON_Is` checkers */
    size_t size = sizeof(cJSON_checkers) / sizeof(cJSON_checkers[0]);

    /* ============ */
    
    /* ================================================ */
    /* ============= Traversing the array ============= */
    /* ======== in order to find the flag value ======= */
    /* ================================================ */

    for (i = 0; i < size; i++) {

        if (cJSON_checkers[i].type == type) {
            return cJSON_checkers[i].string;
        }
    }

    /* ======== */

    return "unknown";
}

/* ================================================================ */

/**
 * The §create_config_file§ function is responsible for creating a default configuration file in JSON format.
 * It sets up a configuration for an SDL-based application, including window properties and initialization flags.
 * The configuration is saved to a file named `.config.json`.
 *
 * @return `0` on success. `-1` on failure, indicating an error occurred during the creation of the configuration file.
 */
static int create_config_file(void) {
    
    /* The main container */
    cJSON* root;
    /* This is an array */
    cJSON* flags;
    /* Different values of the window object */
    cJSON* flag;
    cJSON* window;

    char* text = NULL;

    errno = 0;
    
    /**
     * Defines a structure containing default values for window properties and SDL initialization flags.
     */
    struct {
        const char* title; // Window title
        const int width; // Window width
        const int height; // Window height
        
        const char* SDL_Init__flags[3]; // Subsystems to initialize by default. They will appear in the file
        const char* SDL_Window__flags[1];
        const char* SDL_Renderer__flags[1];
        
    } default_settings = {
        "Sancho Panza Framework",
        600,
        400,
        {"SDL_INIT_VIDEO", "SDL_INIT_EVENTS", "SDL_INIT_TIMER"},
        {"SDL_WINDOW_SHOWN"},
        {"SDL_RENDERER_ACCELERATED"}
    };
    
    /* ================================================ */
    
    /**
     * It initializes a JSON object (root) to serve as the main container for the configuration data.
    */
    if ((root = cJSON_CreateObject()) == NULL) {
        /* Most likely it's a memory allocation error */

        goto END;
    }
    
    /* ================================================ */

    /**
     * Creates a JSON array for SDL initialization flags and populates it with default values.
     */
    if ((flags = cJSON_CreateArray()) == NULL) {
        /* Most likely it's a memory allocation error */
        goto END;
    }

    /* No return value check? Inserting the array into the main object */
    cJSON_AddItemToObject(root, "SDL_Init__flags", flags);
    
    /* Populating the array with values */
    for (size_t i = 0; i < sizeof(default_settings.SDL_Init__flags) / sizeof(default_settings.SDL_Init__flags[0]); i++) {

        if ((flag = cJSON_CreateString(default_settings.SDL_Init__flags[i])) == NULL) {
            goto END;
        }

        cJSON_AddItemToArray(flags, flag);
    }
    
    /* ================================================ */

    /**
     * Creates a JSON object for window properties and populates it with the title, width, and height.
     */
    if ((window = cJSON_CreateObject()) == NULL) {
        /* Most likely it's a memory allocation error */
        
        goto END;
    }
    
    /* Window title */
    flag = cJSON_CreateString(default_settings.title);
    cJSON_AddItemToObject(window, "title", flag);
    
    /* Window width */
    flag = cJSON_CreateNumber(default_settings.width);
    cJSON_AddItemToObject(window, "width", flag);
    
    /* Window height */
    flag = cJSON_CreateNumber(default_settings.height);
    cJSON_AddItemToObject(window, "height", flag);
    
    /* ================================================ */

    /**
     * Creates additional arrays for window and renderer flags, populating them with default values.
     */
    if ((flags = cJSON_CreateArray()) == NULL) {
        /* Most likely it's a memory allocation error */
        goto END;
    }

    for (size_t i = 0; i < sizeof(default_settings.SDL_Window__flags) / sizeof(default_settings.SDL_Window__flags[0]); i++) {

        if ((flag = cJSON_CreateString(default_settings.SDL_Window__flags[i])) == NULL) {
            goto END;
        }

        cJSON_AddItemToArray(flags, flag);
    }

    /* Add window flags to the window object */
    cJSON_AddItemToObject(window, "SDL_Window__flags", flags);
    
    /* ======================== */

    if ((flags = cJSON_CreateArray()) == NULL) {
        /* Most likely it's a memory allocation error */
        goto END;
    }

    for (size_t i = 0; i < sizeof(default_settings.SDL_Renderer__flags) / sizeof(default_settings.SDL_Renderer__flags[0]); i++) {

        if ((flag = cJSON_CreateString(default_settings.SDL_Renderer__flags[i])) == NULL) {
            goto END;
        }

        cJSON_AddItemToArray(flags, flag);
    }

    /* Add renderer flags to the window object */
    cJSON_AddItemToObject(window, "SDL_Renderer__flags", flags);

    /* ================================================ */

    /**
     * The window object is added to the root object.
     */
    cJSON_AddItemToObject(root, "Window", window);

    /* ================================================ */

    /**
     * The JSON object is converted to a string format.
     */
    if ((text = cJSON_Print(root)) == NULL) {
        goto END;
    }
    
    /**
     * The string is written to a file named `.config.json`. If the writing process fails, the function will handle the error appropriately.
     */
    if (write_to_file(".config.json", text) != 0) {
        goto END;
    }
    
    /* ================================================ */
    
    /**
     * Allocated memory for JSON objects and strings is freed before returning from the function to prevent memory leaks.
     */
    free(text);
    cJSON_Delete(root);

    /* ================ */

    return 0;
    
    /**
     * If any step fails (e.g., memory allocation, file writing), the function jumps to the error handling section, where it logs the error and cleans up resources.
     */
    END:
    {
        #ifdef STRICT
            error(stderr, "%s (%s%s%s)\n", "unable to create a default configuration file", RED, errno != 0 ? strerror(errno) : "", WHITE);
        #endif

        free(text);
        cJSON_Delete(root);

        /* ================ */

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
    
    /* ================================================ */
    /* =============== Opening the file =============== */
    /* ================================================ */

    if ((file = fopen(name, "r")) == NULL) {

        #ifdef STRICT
            error(stderr, "%s (%s%s%s)\n", strerror(errno), CYAN, name, WHITE);
        #endif

        /* ======== */
        return errno;
    }
    
    /* ================================================ */
    /* ========= Figuring out how many bytes ========== */
    /* ============= of memory to allocate ============ */
    /* ================================================ */
    
    fseek(file, 0L, SEEK_END);

    /* Do not forget about the `\0` character */
    number_of_bytes = ftell(file) + 1;

    fseek(file, 0L, SEEK_SET);
    
    /* ================================================ */
    /* ============== Allocating memory =============== */
    /* ================================================ */
    
    if ((*buffer = (char*) calloc(number_of_bytes, sizeof(char))) == NULL) {

        #ifdef STRICT
            error(stderr, "%s\n", strerror(errno));
        #endif

        fclose(file);

        /* ======== */
        return errno;
    }

    /* ================================================ */
    /* =============== Reading the file =============== */
    /* ================================================ */
    
    bytes_read = fread(*buffer, sizeof(char), number_of_bytes, file);

    *(*buffer + bytes_read) = '\0';

    fclose(file);

    /* ======== */

    return 0;
}

/* ================================================================ */

void print_message(FILE* stream, Message_Type msg_type, const char* format, ...) {

    char buffer[BUFFER_SIZE];
    size_t bytes_written;

    va_list args;
    va_start(args, format);

    const char* prefix;

    /* ================ */

    stream = (!stream) ? ((msg_type == ERROR) ? stderr : stdout) : stream;

    switch (msg_type) {

        case ERROR:
            prefix = RED "Error" WHITE;
            break ;

        case WARNING:
            prefix = YELLOW "Warning" WHITE;
            break ;

        case SUCCESS:
            prefix = GREEN "Success" WHITE;
            break ;
    }

    if (((bytes_written = snprintf(buffer, sizeof(buffer), "%s: ", prefix)) < 0) || (bytes_written >= sizeof(buffer))) {
        fprintf(stream, "%s%s%s: error formatting error message\n", RED, "Error", WHITE);

        return ;
    }

    if ((bytes_written = vsnprintf(buffer + bytes_written, sizeof(buffer) - bytes_written, format, args) < 0) || (bytes_written >= sizeof(buffer) - bytes_written)) {
        fprintf(stream, "%s%s%s: error formatting error message\n", RED, "Error", WHITE);

        return ;
    }

    fprintf(stream, "%s", buffer);
    fflush(stream);

    va_end(args);
}

/* ================================================================ */

int JSON_parse(const char* buffer, cJSON** root) {

    const char* error_ptr;
    
    /* ================================================ */
    /* ================ Just a wrapper ================ */
    /* ============== around `cJSON_Parse` ============ */
    /* ================================================ */

    if ((*root = cJSON_Parse(buffer)) == NULL) {

        if ((error_ptr = cJSON_GetErrorPtr()) != NULL) {
            
            error(stderr, "before %s%s%s\n", PURPLE, error_ptr, WHITE);

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

    /* ================ */

    if (name == NULL) {

        #ifdef STRICT
            error(stderr, "%s [%s%s%s]\n", "filename is not specified", BLUE, __func__, WHITE);
        #endif

        /* ======== */
        return -1;
    }
    
    /* ================================================ */
    /* =============== Opening the file =============== */
    /* ================================================ */

    if ((file = fopen(name, "w+")) == NULL) {

        #ifdef STRICT
            error(stderr, "%s [%s%s%s]\n", strerror(errno), BLUE, __func__, WHITE);
        #endif

        /* ======== */
        return -1;
    }
    
    /* ================================================ */

    fprintf(file, "%s", string);

    fclose(file);

    /* ======== */

    return 0;
}

/* ================================================================ */

int extract_JSON_data(const cJSON* root, const char* name, JSON_Entity type, cJSON** data) {

    cJSON_bool (*check)(const cJSON* const);
    size_t size = sizeof(cJSON_checkers) / sizeof(cJSON_checkers[0]);

    /* ================ */

    if ((type < 0) || (type > size)) {

        #ifdef STRICT
            error(stderr, "unsupported type\n", "");
        #endif

        return -1;
    }

    check = cJSON_checkers[type].function;
    
    /* ================================================ */

    if (root == NULL) {
        return -1;
    }
    
    /* ================================================ */
    /* ============ Extracting the element ============ */
    /* ================================================ */

    if ((*data = cJSON_GetObjectItemCaseSensitive(root, name)) == NULL) {

        #ifdef STRICT
            error(stderr, "there is no such element in the document [%s%s%s]\n", PURPLE, name, WHITE);
        #endif

        return -1;
    }
    
    /* ================================================ */
    /* ============== Data type mismatch ============== */
    /* ================================================ */

    if (check(*data) != 1) {

        #ifdef STRICT
            error(stderr, "the found element doesn't belong to the specified type [%s%s%s != %s]\n", PURPLE, name, WHITE, extract_checker_name(type));
        #endif

        *data = NULL;

        return -1;
    }

    /* ========*/

    return 0;
}

/* ================================================================ */

int SP_init(App** app) {

    char* buffer;
    cJSON* root;

    Uint32 SDL_flags;
    struct window_options opts = {0, 0, 0, 0, 0};

    int status = 0;

    /* ================ */

    /**
     * The function attempts to read the default configuration file specified by `DEFAULT_SDL` into a buffer using the `read_file2buffer` function.
     * If the file is missing (indicated by `ENOENT`), a warning message is printed to `stdout`, and the function attempts to create a default configuration file using `create_config_file`.
     */
    while ((status = read_file2buffer(DEFAULT_SDL, &buffer)) != 0) {

        if (status == ENOENT) {

            /* The error message from `read_file2buffer` is outputed as well (if STRICT) */
            warning(stdout, "[%s%s%s] missing configuration file. Creating a default one...\n", BLUE, __func__, WHITE);
        }
        
        /**
         * If the creation of the default configuration file fails, the function returns `-1`.
         * If successful, it prints a success message indicating that the file has been created.
         */
        if ((status = create_config_file()) != 0) {

            /* The error message from `read_file2buffer` is outputed (if STRICT) */
            return -1;
        }
        else {
            success(stdout, "file (%s%s%s) has been created\n", CYAN, DEFAULT_SDL, WHITE);
        }
    }

    /* ================================ */
    
    /**
     * The function attempts to parse the contents of the buffer as JSON using the `JSON_parse` function.
     * If parsing fails, the function jumps to the error handling section.
     */
    if ((status = JSON_parse(buffer, &root)) != 0) {
        /* `JSON_parse` prints the error location to the console */
        goto END;
    }

    /* ================================ */
    
    /**
     * The function retrieves the SDL initialization flags from the parsed JSON data and attempts to initialize SDL using `SDL_Init`.
     * If initialization fails, an error message is printed to `stderr`, and the function jumps to the error handling section.
     */
    SDL_flags = deserialize__SDL_Init__flags(root, "SDL_Init__flags");

    if ((status = SDL_Init(SDL_flags)) != 0) {
        error(stderr, "Initialization failed. Unable to initialize SDL (%s%s%s)\n", RED, SDL_GetError(), WHITE);

        goto END;
    }

    /* ================================ */
    
    /**
     * The function retrieves window options from the parsed JSON data using `get_window_options`. If this fails, an error message is printed to `stderr`, and the function jumps to the error handling section.
     */
    if (get_window_options(root, &opts) != 0) {
        error(stderr, "Initialization failed. Unable to get SDL_Window flags\n", "");
        
        goto END;
    }

    /* ================================ */

    /**
     * The function creates a new application instance using `Application_new`.
     * If this fails, it jumps to the error handling section.
     */
    if ((*app = Application_new()) == NULL) {
        goto END;
    }

    /* ================================ */

    /**
     * the function creates a new window for the application using `Window_new`.
     * If this fails, it jumps to the error handling section.
     */
    if (((*app)->window = Window_new(opts.title, opts.width, opts.height, opts.wflags, opts.rflags)) == NULL) {
        goto END;
    }

    /* ================================ */
    
    /**
     * The function creates a timer for the application using `Timer_new` to cap the frame rate at 60 FPS.
     * If this fails, it jumps to the error handling section.
     */
    if (((*app)->timer = Timer_new(1.0f / 60)) == NULL) {
        goto END;
    }

    /* ================================ */

    /**
     * If all initializations are successful, the function frees any allocated memory (e.g., the window title, buffer, and JSON root) before returning 0
     */
    free(opts.title);
    free(buffer);
    cJSON_Delete(root);

    /* ======== */

    return status;

    /**
     * If any step fails, the function jumps to the `END` label, where it cleans up resources by destroying the application instance, freeing allocated memory, and quitting SDL before returning `-1`.
     */
    { END:
        Application_destroy(app);
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

cJSON* serialize__SDL_Color(const SDL_Color* color) {

    cJSON* serialized_color = NULL;

    /* ================ */

    if (color == NULL) {
        return NULL;
    }

    if ((serialized_color = cJSON_CreateObject()) == NULL) {
        return NULL;
    }

    /* ======== Red component ======== */
    if (cJSON_AddNumberToObject(serialized_color, "r", color->r) == NULL) {
        goto END;
    }

    /* ======= Green component ======= */
    if (cJSON_AddNumberToObject(serialized_color, "g", color->g) == NULL) {
        goto END;
    }

    /* ======= Blue component ======== */
    if (cJSON_AddNumberToObject(serialized_color, "b", color->b) == NULL) {
        goto END;
    }

    /* ======= Alpha component ======= */
    if (cJSON_AddNumberToObject(serialized_color, "a", color->a) == NULL) {
        goto END;
    }

    /* ======== */

    return serialized_color;

    { END:
        cJSON_Delete(serialized_color);
        serialized_color = NULL;

        return serialized_color; // NULL
    }
}

/* ================================================================ */

int deserialize__SDL_Color(const cJSON* root, SDL_Color* color, const char* label) {

    cJSON* _color = NULL;
    cJSON* component = NULL;

    /* ================ */

    if (root == NULL) {
        return -1;
    }

    if (color == NULL) {
        return -1;
    }

    /* ================================================ */
    /* ========= Extracting the color object ========== */
    /* ================================================ */

    if (extract_JSON_data(root, label, OBJECT, &_color) != 0) {
        return -1;
    }

    /* ======================================== */
    /* === Extracting the color components ==== */
    /* ======================================== */

    /* RED */
    if (extract_JSON_data(_color, "r", NUMBER, &component) != 0) {
        goto END;
    }
    color->r = (Uint8) component->valuedouble;

    /* GREEN */
    if (extract_JSON_data(_color, "g", NUMBER, &component) != 0) {
        goto END;
    }
    color->g = (Uint8) component->valuedouble;

    /* BLUE */
    if (extract_JSON_data(_color, "b", NUMBER, &component) != 0) {
        goto END;
    }
    color->b = (Uint8) component->valuedouble;

    /* ALPHA */
    if (extract_JSON_data(_color, "a", NUMBER, &component) != 0) {
        goto END;
    }
    color->a = (Uint8) component->valuedouble;

    /* ======== */

    return 0;

    { END:
        cJSON_Delete(_color);

        return -1;
    }
}

/* ================================================================ */

cJSON* serialize__SDL_Init__flags(void) {

    cJSON* array = NULL;
    Uint32 subsystem = SDL_WasInit(SDL_INIT_EVERYTHING);

    /* ================ */

    if ((array = cJSON_CreateArray()) == NULL) {
        goto END;
    }

    for (size_t i = 0; i < sizeof(SDL_Init_Flags) / sizeof(SDL_Init_Flags[0]); i++) {
        if (subsystem & SDL_Init_Flags[i].flag) {

            if (!cJSON_AddItemToArray(array, cJSON_CreateString(SDL_Init_Flags[i].name))) {
                goto END;
            }
        }
    }

    /* ======== */

    return array;

    { END:
        cJSON_Delete(array);
        array = NULL;

        return array;
    }
}

/* ================================================================ */

Uint32 deserialize__SDL_Init__flags(const cJSON* root, const char* label) {

    Uint32 flags = 0;
    const char* default_label = "SDL_Init__flags";

    cJSON* array = NULL;
    size_t array_size;

    /* ================ */

    if (root == NULL) {

        #ifdef STRICT
            warning(stdout, "[in %s%s%s] no root element", BLUE, __func__, WHITE);
        #endif

        return flags; // 0
    }

    if (extract_JSON_data(root, label ? label : default_label, ARRAY, &array) != 0) {
        return flags;
    }

    array_size = cJSON_GetArraySize(array);

    for (size_t i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(array, i);

        /* Map a string to its equivalent flag */
        flags |= SDL_lookup_flag(SDL_Init_Flags, array_elm->valuestring, sizeof(SDL_Init_Flags) / sizeof(SDL_Init_Flags[0]));
    }

    /* ======== */

    return flags;
}

/* ================================================================ */

#undef DEFAULT_SDL
#undef STRICT
