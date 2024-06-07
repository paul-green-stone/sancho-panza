#include "../../sancho-panza.h"

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
