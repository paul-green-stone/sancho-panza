#include "../../sancho-panza.h"

#define DEFAULT_SDL ".config.json"

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

static struct mapping_entry SDL_Init_Flags[] = {
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
/* ==================== SDL_CreateWindow Flags ==================== */
/* ================================================================ */

static struct mapping_entry SDL_CreateWindow_Flags[] = {
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
/* =================== SDL_CreateRenderer Flags =================== */
/* ================================================================ */

static struct mapping_entry SDL_CreateRenderer_flags[] = {
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

    print_warning(stdout, "unrecognized flags (%s) are simply ignored\n", string_flag);

    /* ======== */

    return EXIT_SUCCESS;
}

/* ================================================================ */

static Uint32 SDL_Init_get_flags(const cJSON* root) {

    Uint32 SDL_flags = 0;

    cJSON* array;
    size_t array_size;

    size_t i;
    
    /* ================ */

    /* ================================================================ */
    /* ==================== Extracting `SDL` array ==================== */
    /* ====================== from a root object ====================== */
    /* ================================================================ */

    if (extract_JSON_data(root, "SDL", ARRAY, &array) != 0) {
        return SDL_flags;
    }

    array_size = cJSON_GetArraySize(array);

    /* ================================================================ */
    /* ================ Mapping arrays' string values ================= */
    /* ================== to its numeric equivalents ================== */
    /* ================================================================ */

    for (i = 0; i < array_size; i++) {

        /* Retrieve the next element from the given array */
        cJSON* array_elm = cJSON_GetArrayItem(array, i);

        /* Map a string to its equivalent flag */
        SDL_flags |= SDL_lookup_flag(SDL_Init_Flags, array_elm->valuestring, sizeof(SDL_Init_Flags) / sizeof(SDL_Init_Flags[0]));
    }

    /* ======== */

    return SDL_flags;
}

/* ================================================================ */

static int get_window_options(const cJSON* root, struct window_options* options) {
    
    /* Corresponds to the `Window` object in the configuration file */
    cJSON* object;
    /* A simple container for extracted data */
    cJSON* data;

    size_t array_size;
    size_t i;
    
    /* ================ */

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
        print_error(stderr, "Sancho-Panza initialization failure (%s%s%s)\n", RED, strerror(errno), WHITE);

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

    if (extract_JSON_data(object, "wflags", ARRAY, &data) != 0) {
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
    if (extract_JSON_data(object, "rflags", ARRAY, &data) != 0) {
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

static int create_default_init_file(void) {
    
    /* The main container */
    cJSON* root;
    /* This is an array */
    cJSON* flags;
    /* Different values of the window object */
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

    /* ============ */
    
    /* ================================================ */
    /* =========== Creating the root object =========== */
    /* ===== as a container for dependent elements ==== */
    /* ================================================ */

    if ((root = cJSON_CreateObject()) == NULL) {
        /* Most likely it's a memory allocation error */

        goto ON_ERROR;
    }

    /* ================================================ */
    /* ============== Creating an array =============== */
    /* =============== containing flags =============== */
    /* ================================================ */
    
    if ((flags = cJSON_CreateArray()) == NULL) {
        /* Most likely it's a memory allocation error */

        goto ON_ERROR;
    }

    /* No return value check? Inserting the array into the main object */
    cJSON_AddItemToObject(root, "SDL", flags);
    
    /* Populating the array with values */
    for (i = 0; i < size; i++) {

        if ((flag = cJSON_CreateString(default_SDL_flags[i])) == NULL) {
            goto ON_ERROR;
        }

        cJSON_AddItemToArray(flags, flag);
    }

    /* ================================================ */
    /* =========== Creating a window object =========== */
    /* ================================================ */

    if ((window = cJSON_CreateObject()) == NULL) {
        /* Most likely it's a memory allocation error */
        
        goto ON_ERROR;
    }
    
    /* ================================================ */
    /* ========= Populating the window object ========= */
    /* ================= with values ================== */
    /* ================================================ */
    
    /* Window title */
    flag = cJSON_CreateString("Sancho Panza");
    cJSON_AddItemToObject(window, "title", flag);
    
    /* Window width */
    flag = cJSON_CreateNumber(600);
    cJSON_AddItemToObject(window, "width", flag);
    
    /* Window height */
    flag = cJSON_CreateNumber(400);
    cJSON_AddItemToObject(window, "height", flag);

    /* ================================================ */
    /* ====== Creating an array of window flags ======= */
    /* ================================================ */

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

    /* ================================================ */
    /* ===== Creating an array of renderer flags ====== */
    /* ================================================ */

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

    /* ================================================ */
    /* ============= Writing to the file ============== */
    /* ================================================ */

    /* Converting to `char*` */
    if ((text = cJSON_Print(root)) == NULL) {
        goto ON_ERROR;
    }

    if (write_to_file(".config.json", text) != 0) {
        goto ON_ERROR;
    }

    /* ================================================ */
    /* =============== Freeing memory ================= */
    /* ================================================ */

    free(text);
    cJSON_Delete(root);

    /* ================ */

    return 0;

    ON_ERROR:
    {
        #ifdef STRICT
            print_error(stderr, "%s (%s%s%s)\n", "unable to create a default configuration file", RED, errno != 0 ? strerror(errno) : "", WHITE);
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

    /* ================ */
    
    /* ================================================ */
    /* =============== Opening the file =============== */
    /* ================================================ */

    if ((file = fopen(name, "r")) == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s (%s%s%s)\n", strerror(errno), CYAN, name, WHITE);
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
            print_error(stderr, "%s\n", strerror(errno));
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

    /* ================ */
    
    /* ================================================ */
    /* ================ Just a wrapper ================ */
    /* ============== around `cJSON_Parse` ============ */
    /* ================================================ */

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

    /* ================ */

    if (name == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s [%s%s%s]\n", "filename is not specified", BLUE, __func__, WHITE);
        #endif

        /* ======== */
        return -1;
    }
    
    /* ================================================ */
    /* =============== Opening the file =============== */
    /* ================================================ */

    if ((file = fopen(name, "w+")) == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s [%s%s%s]\n", strerror(errno), BLUE, __func__, WHITE);
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

int extract_JSON_data(const cJSON* root, const char* name, int type, cJSON** data) {

    cJSON_bool (*check)(const cJSON* const);
    size_t size = sizeof(cJSON_checkers) / sizeof(cJSON_checkers[0]);

    /* ================ */

    if ((type < 0) || (type > size)) {

        #ifdef STRICT
            print_error(stderr, "unsupported type\n");
        #endif

        /* ======== */
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
            print_error(stderr, "there is no such element in the document [%s%s%s]\n", PURPLE, name, WHITE);
        #endif

        /* ======== */
        return -1;
    }
    
    /* ================================================ */
    /* ============== Data type mismatch ============== */
    /* ================================================ */

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

    /* ================ */
    
    /* ================================================ */
    /* ========== Trying to read the default ========== */
    /* ============== configuration file ============== */
    /* ================================================ */

    while ((status = read_file2buffer(DEFAULT_SDL, &buffer)) != 0) {

        if (status == ENOENT) {

            /* The error message from `read_file2buffer` is outputed as well (if STRICT) */
            print_warning(stdout, "[%s%s%s] missing configuration file. Creating a default one...\n", BLUE, __func__, WHITE);
        }
        
        /* ================================================ */
        /* ========= Trying to create the default ========= */
        /* ============== configuration file ============== */
        /* ================================================ */

        if ((status = create_default_init_file()) != 0) {

            /* The error message from `read_file2buffer` is outputed (if STRICT) */
            return -1;
        }
        else {
            print_success(stdout, "file (%s%s%s) has been created\n", CYAN, DEFAULT_SDL, WHITE);
        }
    }
    
    /* ================================================ */
    /* ========== Parsing the buffer content ========== */
    /* ================================================ */

    if ((status = JSON_parse(buffer, &root)) != 0) {
        /* `JSON_parse` prints the error location to the console */
        goto ON_ERROR;
    }
    
    /* ================================================ */
    /* ======== Initializing the SDL2 Library ========= */
    /* ================================================ */

    SDL_flags = SDL_Init_get_flags(root);

    if ((status = SDL_Init(SDL_flags)) != 0) {
        print_error(stderr, "Initialization failed. Unable to initialize SDL (%s%s%s)\n", RED, SDL_GetError(), WHITE);

        goto ON_ERROR;
    }

    /* ================================================ */
    /* ============ Getting window options ============ */
    /* ================================================ */
    
    if (get_window_options(root, &opts) != 0) {
        print_error(stderr, "Initialization failed. Unable to get window flags\n");
        
        goto ON_ERROR;
    }
    
    /* ================================================ */
    /* =========== Getting the Application ============ */
    /* ================================================ */

    if ((*app = Application_new()) == NULL) {
        goto ON_ERROR;
    }

    if (((*app)->window = Window_new(opts.title, opts.width, opts.height, opts.wflags, opts.rflags)) == NULL) {
        goto ON_ERROR;
    }
    
    /* The application timer is used to cap FPS */
    if (((*app)->timer = Timer_new(1.0f / 60)) == NULL) {
        goto ON_ERROR;
    }

    /* ================================================ */
    /* ================== Freeing memory ============== */
    /* ================================================ */

    free(opts.title);
    free(buffer);
    cJSON_Delete(root);

    /* ======== */

    return status;

    ON_ERROR:
    {
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

#undef DEFAULT_SDL
#undef STRICT
