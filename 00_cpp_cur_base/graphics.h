#ifndef CPP_OOP_GRAPHICS_H
#define CPP_OOP_GRAPHICS_H

class Console {
public:
    virtual ~Console() {}

    virtual void clear() = 0;

    virtual int width() const = 0;

    virtual int height() const = 0;

    virtual int read_ch() const = 0;

    virtual void move(int x, int y) = 0;

    virtual void write(int x, int y, const char *const text) = 0;

    virtual void write(const char *const text) = 0;

    virtual void left() = 0;

    virtual void right() = 0;

    virtual void up() = 0;

    virtual void down() = 0;

    virtual void refresh() = 0;

    enum Key {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        UNKNOWN
    };

    virtual enum Key to_key(int key) = 0;

    static Console *newConsole();
};


#endif //CPP_OOP_GRAPHICS_H
