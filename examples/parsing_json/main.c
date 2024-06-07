#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    char* buffer;
    cJSON* root;

    /* ======== */

    if (read_file2buffer("valid_input.json", &buffer) == 0) {
        
        if (JSON_parse(buffer, &root) == 0) {
            print_success(stderr, "%s\n", "file has been parsed successfully");
        }
        else {
            free(buffer);

            return -1;
        }
    }
    else {
        return errno;
    }

    free(buffer);
    cJSON_Delete(root);

    if (read_file2buffer("wrong_input.json", &buffer) == 0) {
        
        if (JSON_parse(buffer, &root) == 0) {
            print_success(stderr, "%s\n", "file parsed successfully");
        }
        else {
            free(buffer);

            return -1;
        }
    }
    else {
        return errno;
    }

    free(buffer);
    cJSON_Delete(root);

    /* ======== */

    return 0;
}

/* ================================================================ */
