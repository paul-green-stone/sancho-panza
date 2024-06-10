#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    char* buffer = NULL;
    cJSON* root = NULL;
    cJSON* data = NULL;

    /* ======== */

    if (read_file2buffer("input.json", &buffer) != 0) {
        return -1;
    }

    if (JSON_parse(buffer, &root) != 0) {

        free(buffer);

        return -1;
    }

    extract_JSON_data(root, "name", STRING, &data);
    if (data) printf("%s: %s\n", data->string, data->valuestring);

    extract_JSON_data(root, "extension", STRING, &data);
    if (data) printf("%s: %s\n", data->string, data->valuestring);

    extract_JSON_data(root, "array", STRING, &data);
    if (data) printf("%s: %s\n", data->string, data->valuestring);

    cJSON_Delete(root);
    free(buffer);

    /* ======== */

    return 0;
}

/* ================================================================ */
