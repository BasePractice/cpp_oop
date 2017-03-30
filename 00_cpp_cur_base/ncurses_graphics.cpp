#include <curses.h>
#include <unistd.h>
#include "graphics.h"

class NCursesGraphics final: public Graphics {
    int _width;
    int _height;
public:

    NCursesGraphics() {
        initscr();
        noecho();
        curs_set(FALSE);

        _width = getmaxx(stdscr);
        _height = getmaxy(stdscr);

        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
    }

    ~NCursesGraphics() {
        endwin();
    }

    void clear() override {
        ::clear();
    }

    void test() override {
        volatile bool need_refresh = true;
        int x = 10, y = 10;

        clear();
        mvprintw(0, 0, "Enter 'q' to exit");
        mvprintw(x, y, "o");
        for (int ch = read_ch(); ch != 'q' && ch != 'Q';) {
            switch (ch) {
                case KEY_DOWN:
                    ++x;
                    if (x >= height() - 1)
                        x = height() - 1;
                    wmove(stdscr, x, y);
                    need_refresh = true;
                    break;
                case KEY_UP:
                    --x;
                    if (x < 0)
                        x = 0;
                    wmove(stdscr, x, y);
                    need_refresh = true;
                    break;
                case KEY_RIGHT:
                    ++y;
                    if (y >= width() - 1)
                        y = width() - 1;
                    wmove(stdscr, x, y);
                    need_refresh = true;
                    break;
                case KEY_LEFT:
                    --y;
                    if (y < 0)
                        y = 0;
                    wmove(stdscr, x, y);
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
                clear();
                sprintf(buffer, "Enter 'q' to exit. Current: %d", ch);
                mvprintw(0, 0, buffer);
                mvprintw(x, y, "o");
                refresh();
            }
            ch = read_ch();
        }
    }

    int width() const override {
        return _width;
    }

    int height() const override {
        return _height;
    }

    int read_ch() const override {
        return wgetch(stdscr);
    }

};

Graphics *
Graphics::newGraphics() {
    return new NCursesGraphics;
}

