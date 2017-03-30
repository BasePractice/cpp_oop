#include <windows.h>
#include <unistd.h>
#include <rpc.h>
#include "graphics.h"

class Win32Console final : public Console {
    int _width;
    int _height;
    HANDLE h_output;
    HANDLE h_input;
    COORD _coord;
public:
    Win32Console() : h_output(GetStdHandle(STD_OUTPUT_HANDLE)), h_input(GetStdHandle(STD_INPUT_HANDLE)) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        CONSOLE_CURSOR_INFO info;
        SetConsoleMode(h_input, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
        GetConsoleScreenBufferInfo(h_output, &csbi);
        _width = csbi.dwSize.X;
        _height = csbi.dwSize.Y;
        GetConsoleCursorInfo(h_output, &info);
        info.bVisible = FALSE;
        SetConsoleCursorInfo(h_output, &info);
        _coord.X = 0;
        _coord.Y = 0;
    }

    ~Win32Console() {
        CloseHandle(h_output);
        CloseHandle(h_input);
    }

    int width() const override {
        return _width;
    }

    int height() const override {
        return _height;
    }

    int read_ch() const override {
        INPUT_RECORD record;
        DWORD read = 0;
        if (ReadConsoleInput(h_input, &record, 1, &read)) {
            if (read == 1) {
                switch (record.EventType) {
                    case KEY_EVENT: {
                        if (record.Event.KeyEvent.bKeyDown) {
                            if (record.Event.KeyEvent.uChar.UnicodeChar == 0) {
                                return record.Event.KeyEvent.wVirtualKeyCode;
                            }
                            return record.Event.KeyEvent.uChar.UnicodeChar;
                        }
                        break;
                    }
                    case WINDOW_BUFFER_SIZE_EVENT: {
                        //_width = record.Event.WindowBufferSizeEvent.dwSize.X;
                        //_height = record.Event.WindowBufferSizeEvent.dwSize.Y;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        return -1;
    }

    void write(int x, int y, const char *const text) override {
        COORD coord = {(SHORT) x, (SHORT) y};
        SetConsoleCursorPosition(h_output, coord);
        write(text);
        SetConsoleCursorPosition(h_output, _coord);
    }

    void write(const char *const text) override {
        DWORD written;
        WriteConsole(h_output, text, (DWORD) lstrlen(text), &written, nullptr);
    }

    void move(int x, int y) override {
        _coord.X = (SHORT) x;
        _coord.Y = (SHORT) y;
        SetConsoleCursorPosition(h_output, _coord);
    }

    void clear() override {
        COORD coordScreen = {0, 0};
        BOOL bSuccess;
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;
        dwConSize = (DWORD) (_width * _height);
        FillConsoleOutputCharacter(h_output, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
        GetConsoleScreenBufferInfo(h_output, &csbi);
        FillConsoleOutputAttribute(h_output, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
        SetConsoleCursorPosition(h_output, coordScreen);
    }

    enum Key to_key(int key) override  {
        switch (key) {
            case VK_UP:
                return KEY_UP;
            case VK_DOWN:
                return KEY_DOWN;
            case VK_LEFT:
                return KEY_LEFT;
            case VK_RIGHT:
                return KEY_RIGHT;
            default:
                return UNKNOWN;
        }
    }

    void refresh() override {
        FlushFileBuffers(h_output);
    }

    void left() override {
        --_coord.X;
        if (_coord.X < 0)
            _coord.X = 0;
        SetConsoleCursorPosition(h_output, _coord);
    }

    void right() override {
        ++_coord.X;
        if (_coord.X >= width() - 1)
            _coord.X = (SHORT) (width() - 1);
        SetConsoleCursorPosition(h_output, _coord);
    }

    void up() override {
        --_coord.Y;
        if (_coord.Y < 0)
            _coord.Y = 0;
        SetConsoleCursorPosition(h_output, _coord);
    }

    void down() override {
        ++_coord.Y;
        if (_coord.Y >= height() - 1)
            _coord.Y = (SHORT) (height() - 1);
        SetConsoleCursorPosition(h_output, _coord);
    }
};


Console *
Console::newConsole() {
    return new Win32Console;
}

