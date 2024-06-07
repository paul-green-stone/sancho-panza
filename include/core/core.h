#ifndef SANCHO_PANZA_CORE_H
#define SANCHO_PANZA_CORE_H

#include "../../sancho-panza.h"
#include "cJSON.h"

/* ================================================================ */

/* White text color */
#define WHITE "\033[0;37m"

/* Red text color */
#define RED "\033[0;31m"

/* Cyan text color */
#define CYAN  "\033[0;36m"

/* Yellow text color */
#define YELLOW "\033[0;33m"

/* Green text color */
#define GREEN "\033[0;32m"

/* ================================================================ */

extern int read_file2buffer(const char* name, char** buffer);

/* ================================ */

extern void print_error(FILE* stream, const char* format, ...);

/* ================================ */

extern void print_warning(FILE* stream, const char* format, ...);

/* ================================ */

extern void print_success(FILE* stream, const char* format, ...);

/* ================================================================ */

#endif /* SANCHO_PANZA_CORE_H */
