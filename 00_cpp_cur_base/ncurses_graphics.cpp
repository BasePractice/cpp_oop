#include <curses.h>
#include <unistd.h>
#include "graphics.h"

class CursesConsole final : public Console {
    int _width;
    int _height;
    int _x;
    int _y;
public:

    CursesConsole() {
        initscr();
        noecho();
        curs_set(FALSE);

        _width = getmaxx(stdscr);
        _height = getmaxy(stdscr);

        _x = 0;
        _y = 0;
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
    }

    ~CursesConsole() {
        endwin();
    }

    void clear() override {
        ::clear();
    }

    void left() override {
        --y;
        if (y < 0)
            y = 0;
        move(x, y);
    }

    void right() override {
        ++y;
        if (y >= width() - 1)
            y = width() - 1;
        move(x, y);
    }

    void up() override {
        --x;
        if (x < 0)
            x = 0;
        move(x, y);
    }

    void down() override {
        ++x;
        if (x >= height() - 1)
            x = height() - 1;
        move(x, y);
    }

    void write(int x, int y, const char *const text) override {
        mvprintw(x, y, text);
    }

    void move(int x, int y) override {
        wmove(stdscr, x, y);
    }

    void refresh() override {
        ::refresh();
    }

    enum Key to_key(int key) override  {
        switch (key) {
            case ::KEY_UP:
                return Graphics::KEY_UP;
            case ::KEY_DOWN:
                return Graphics::KEY_DOWN;
            case ::KEY_LEFT:
                return Graphics::KEY_LEFT;
            case ::KEY_RIGHT:
                return Graphics::KEY_RIGHT;
            default:
                return UNKNOWN;
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

Console *
Console::newConsole() {
    return new CursesConsole;
}

