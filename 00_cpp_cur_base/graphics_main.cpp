#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include "graphics.h"

int
main(int argc, char **argv) {
    Console *g = Console::newConsole();

    volatile bool need_refresh = true;
    int x = 10, y = 10;

    g->clear();
    g->write(0, 0, "Enter 'q' to exit");
    g->move(x, y);
    g->write("o");
    for (int ch = g->read_ch(); ch != 'q' && ch != 'Q';) {
        switch (g->to_key(ch)) {
            case Console::KEY_DOWN:
                g->down();
                need_refresh = true;
                break;
            case Console::KEY_UP:
                g->up();
                need_refresh = true;
                break;
            case Console::KEY_RIGHT:
                g->right();
                need_refresh = true;
                break;
            case Console::KEY_LEFT:
                g->left();
                need_refresh = true;
                break;
            default:
                need_refresh = false;
                //tick
                usleep(10000);
                break;
        }
        if (need_refresh) {
            char buffer[256];
            need_refresh = false;
            g->clear();
            sprintf(buffer, "Enter 'q' to exit. Current: %d", ch);
            g->write(0, 0, buffer);
            g->write("o");
            g->refresh();
        }
        ch = g->read_ch();
    }
    delete g;
    return EXIT_SUCCESS;
}

