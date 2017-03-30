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
        --_y;
        if (_y < 0)
            _y = 0;
        move(_x, _y);
    }

    void right() override {
        ++_y;
        if (_y >= width() - 1)
            _y = width() - 1;
        move(_x, _y);
    }

    void up() override {
        --_x;
        if (_x < 0)
            _x = 0;
        move(_x, _y);
    }

    void down() override {
        ++_x;
        if (_x >= height() - 1)
            _x = height() - 1;
        move(_x, _y);
    }

    void write(int x, int y, const char *const text) override {
        mvprintw(x, y, text);
    }

    void write(const char *const text) override {
        mvprintw(_x, _y, text);
    }

    void move(int x, int y) override {
        wmove(stdscr, x, y);
        _x = x;
        _y = y;
    }

    void update() override {
        ::refresh();
    }

    enum Key translate_key(int key) override  {
        switch (key) {
            case KEY_UP:
                return KeyUp;
            case KEY_DOWN:
                return KeyDown;
            case KEY_LEFT:
                return KeyLeft;
            case KEY_RIGHT:
                return KeyRight;
            default:
                return KeyUnknown;
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

