#include "../../sancho-panza.h"

/* ================================================================ */

int main(int argc, char** argv) {

    char* buffer;
    int status = 0;

    /* ======== */

    if ((status = read_file2buffer("inpdut.txt", &buffer)) == 0) {
        printf("%s", buffer);
    }
    else {

        print_warning(stdout, "%s\n", "Missing the configuration file. Creating a default one...");

        return errno;
    }

    free(buffer);

    /* ======== */

    return status;
}

/* ================================================================ */
