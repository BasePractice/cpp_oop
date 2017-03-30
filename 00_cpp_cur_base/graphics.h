#ifndef CPP_OOP_GRAPHICS_H
#define CPP_OOP_GRAPHICS_H




class Graphics {
public:
    virtual ~Graphics() {}

    virtual void clear() = 0;
    virtual void test() = 0;


    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual int read_ch() const = 0;

    static Graphics *newGraphics();
};


#endif //CPP_OOP_GRAPHICS_H
