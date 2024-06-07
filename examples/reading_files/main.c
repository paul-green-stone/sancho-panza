#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    char* buffer;

    /* ======== */

    if (read_file2buffer("input.txt", &buffer) == 0) {

        print_success(stdout, "%s", "file has been read\n");

        printf("%s", buffer);
    }
    else {
        return errno;
    }

    free(buffer);

    /* ======== */

    return 0;
}

/* ================================================================ */
