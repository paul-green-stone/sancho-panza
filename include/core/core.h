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

extern int SP_quit(void);

/* ================================================================ */

#endif /* SANCHO_PANZA_CORE_H */
