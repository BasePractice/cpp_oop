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

    int _ch;

public:
    Win32Console() : h_output(GetStdHandle(STD_OUTPUT_HANDLE)), h_input(GetStdHandle(STD_INPUT_HANDLE)), _ch(-1) {
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
        return _ch;
    }

    Event event() override {
        INPUT_RECORD record;
        DWORD read = 0;
        if (ReadConsoleInput(h_input, &record, 1, &read)) {
            if (read == 1) {
                switch (record.EventType) {
                    case KEY_EVENT: {
                        if (record.Event.KeyEvent.bKeyDown) {
                            if (record.Event.KeyEvent.uChar.UnicodeChar == 0) {
                                _ch = record.Event.KeyEvent.wVirtualKeyCode;
                            } else {
                                _ch = record.Event.KeyEvent.uChar.UnicodeChar;
                            }
                        } else {
                            _ch = -1;
                        }
                        return KEYBOARD;
                    }
                    case WINDOW_BUFFER_SIZE_EVENT: {
                        _width = record.Event.WindowBufferSizeEvent.dwSize.X;
                        _height = record.Event.WindowBufferSizeEvent.dwSize.Y;
                        return WINDOW;
                    }
                    default:
                        break;
                }
            }
        }
        return NONE;
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

    enum Key translate_key(int key) override  {
        switch (key) {
            case VK_UP:
                return KeyUp;
            case VK_DOWN:
                return KeyDown;
            case VK_LEFT:
                return KeyLeft;
            case VK_RIGHT:
                return KeyRight;
            default:
                return KeyUnknown;
        }
    }

    void update() override {
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

