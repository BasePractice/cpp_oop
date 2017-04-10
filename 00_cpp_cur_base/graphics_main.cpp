#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include "graphics.h"

int
main(int argc, char **argv) {
    Console *c = Console::newConsole();

    volatile bool need_refresh;
    volatile bool is_running = true;
    int x = 10, y = 10;

    c->clear();
    c->write(0, 0, "Enter 'q' to exit");
    c->move(x, y);
    c->write("o");
    while (is_running) {
        switch (c->event()) {
            case Console::KEYBOARD: {
                int ch = c->read_ch();
                switch (c->translate_key(ch)) {
                    case Console::KeyDown:
                        c->down();
                        need_refresh = true;
                        break;
                    case Console::KeyUp:
                        c->up();
                        need_refresh = true;
                        break;
                    case Console::KeyRight:
                        c->right();
                        need_refresh = true;
                        break;
                    case Console::KeyLeft:
                        c->left();
                        need_refresh = true;
                        break;
                    default:
                        is_running = ch != 'q' && ch != 'Q';
                        need_refresh = false;
                        //tick
                        usleep(10000);
                        break;
                }
                break;
            }
            case Console::WINDOW: {
                need_refresh = true;
                break;
            }
            case Console::NONE:
                break;
        }
        if (need_refresh) {
            char buffer[256];
            c->clear();
            sprintf(buffer, "Enter 'q' to exit.");
            c->write(0, 0, buffer);
            c->write("o");
            c->update();
            need_refresh = false;
        }
    }
    delete c;
    return EXIT_SUCCESS;
}

