#include <cstdlib>
#include <unistd.h>
#include "graphics.h"

int
main(int argc, char **argv) {
    Graphics *g = Graphics::newGraphics();
    g->clear();
    g->test();
    delete g;
    return EXIT_SUCCESS;
}

