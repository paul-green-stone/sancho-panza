#ifndef CJSON_MAPPING_ENTRIES_H
#define CJSON_MAPPING_ENTRIES_H

#include "../../sancho-panza.h"

/* ================================================================ */

enum checks {
    ARRAY,
    BOOLEAN,
    OBJECT,
    NUMBER,
    STRING,
};

/* ================================================================ */

struct cJSON_checker_mapping_entry {

    const int type;
    const char* string;
    cJSON_bool (*function)(const cJSON* const); 
};

/* ================================================================ */

struct cJSON_checker_mapping_entry cJSON_checkers[] = {

    {ARRAY, "array", cJSON_IsArray},
    {BOOLEAN, "boolean", cJSON_IsBool},
    {OBJECT, "object", cJSON_IsObject},
    {NUMBER, "number", cJSON_IsNumber},
    {STRING, "string", cJSON_IsString}
};

/* ================================================================ */

#endif /* CJSON_MAPPING_ENTRIES_H */
