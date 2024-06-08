#include "../../sancho-panza.h"

/* ================================================================ */

static struct mapping_entry {

    const char* name;
    Uint32 flag;
};

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

/* ================================================================ */

static int create_default_init_file(void) {

    cJSON* root;
    cJSON* flags;
    cJSON* flag;

    char* text = NULL;

    size_t i;

    char* default_SDL_flags[] = {"SDL_INIT_VIDEO", "SDL_INIT_EVENTS", "SDL_INIT_TIMER"};
    size_t size = sizeof(default_SDL_flags) / sizeof(default_SDL_flags[0]);

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

    /* ================================ */

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
/* ================================================================ */
/* ================================================================ */

int read_file2buffer(const char* name, char** buffer) {

    FILE* file;

    size_t number_of_bytes;
    size_t bytes_read;

    /* ======== */

    if ((file = fopen(name, "r")) == NULL) {

        #ifdef STRICT
            print_error(stderr, "%s [%s%s%s]\n", strerror(errno), CYAN, name, WHITE);
        #endif

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
