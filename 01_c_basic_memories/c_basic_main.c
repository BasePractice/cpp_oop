#include <stdlib.h>
#include <stdio.h>
#include "c_basic.h"

#define MAIN_EMPTY          0
#define MAIN_WITH_ARGS      1
#define MAIN_WITH_ENV       2


#define MAIN_CONFIGURATION  MAIN_WITH_ENV

#if MAIN_CONFIGURATION == MAIN_EMPTY
void
main() {}
#elif MAIN_CONFIGURATION == MAIN_WITH_ARGS
int
main(int argc, char **argv) {
    int i = 0;

    for (i = 0; i < argc; ++i) {
        fprintf(stdout, "Argument[%02d]: %s\n", i, argv[i]);
    }
    return EXIT_SUCCESS;
}
#elif MAIN_CONFIGURATION == MAIN_WITH_ENV
int
main(int argc, char **argv, char **env) {
    char **p;

    for (p = env; (*p) != NULL; ++p) {
        fprintf(stdout, "%s\n", (*p));
    }
    return EXIT_SUCCESS;
}
#endif


