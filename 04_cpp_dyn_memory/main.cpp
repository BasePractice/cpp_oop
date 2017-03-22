#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>

class PureBase {
public:
    virtual void print() const = 0;

    std::string pub = "pub_PureBase";
};

class OtherPureBase {
public:
    virtual void print() const = 0;

    std::string pub = "pub_OtherPureBase";
};

class Base: public PureBase {
public:
    virtual void print() const {
        std::cout << "Base" << std::endl;
    }

    std::string pub = "pub_Base";
};

class Empty final: public Base {
public:
};


class Single final: public Base {
public:
    void print() const override {
        std::cout << "Single" << std::endl;
    }

    std::string pub = "pub_Single";
};

class Wrapper: public Base {
public:
    void print() const override final {
        Base::print();
        std::cout << "Wrapper" << std::endl;
    }

    std::string pub = "pub_Wrapper";
};

class PureInstance final: public Base, public OtherPureBase {
public:
    void print() const override {
        std::cout << "PureBase" << std::endl;
    }
};


static void
print(const PureBase *base) {
    base->print();
}

static void
print(const PureBase &base) {
    base.print();
}

static void
pub(const PureBase *base) {
    std::cout << base->pub << std::endl;
}

static void
pub(const PureBase &base) {
    std::cout << base.pub << std::endl;
}

namespace DynamicMemory {

    class DynamicClass {
    public:
        static void *operator new(std::size_t sz, void *p) {
            std::cout << "operator DynamicClass::new(" << sz << ")" << std::endl;
            return p;
        }

        static void *operator new(std::size_t sz) {
            std::cout << "operator DynamicClass::new(" << sz << ")" << std::endl;
            return std::malloc(sz);
        }

        static void *operator new[](std::size_t sz) {
            std::cout << "operator DynamicClass::new[](" << sz << ")" << std::endl;
            return ::operator new(sz);
        }

        static void operator delete(void *ptr) {
            std::cout << "operator DynamicClass::delete" << std::endl;
            std::free(ptr);
        }

        static void operator delete[](void *ptr) {
            std::cout << "operator DynamicClass::delete[]" << std::endl;
            ::operator delete [] (ptr);
        }

        static void *operator new(std::size_t sz, const std::string title) {
            std::cout << "operator DynamicClass::new(" << sz << ", " << title << ")" << std::endl;
            return ::operator new(sz);
        }

        static void operator delete(void *ptr, const std::string title) {
            std::cout << "operator DynamicClass::delete(" << title << ")" << std::endl;
            ::operator delete(ptr);
        }
    };

    int main(int argc, char **argv) {
        auto static_pointer = new unsigned char [sizeof(DynamicClass)];
        DynamicClass *forClass = new DynamicClass;
        delete forClass;
        forClass = new ("HELLO") DynamicClass;
        delete forClass ;

        DynamicClass *forClasses = new DynamicClass[100];
        for (int i = 0; i < 100; ++i) {
            forClass[i] = DynamicClass();
        }
        delete [] forClasses;

        forClass = new(static_pointer) DynamicClass;
        fprintf(stdout, "%p == %p\n",
                reinterpret_cast<void *>(forClass),
                reinterpret_cast<void *>(static_pointer));
        forClass->~DynamicClass();
        delete [] static_pointer;
        return 0;
    }
}

int
main(int argc, char **argv) {
    Single single;
    Empty empty;
    Wrapper wrapper;


    std::cout << "----Pointers -----" << std::endl;
    print(&single);
    print(&wrapper);
    print(&empty);
    std::cout << "----Reference-----" << std::endl;
    print(single);
    print(wrapper);
    print(empty);
    std::cout << "----Pure virt-----" << std::endl;
    PureInstance pure_instance;
    OtherPureBase *pure_base = &pure_instance;
    Base *base = &pure_instance;

    print(base);
    print(*base);
    print(pure_instance);
    pure_base->print();
    //FIXME: Dangerous!
    base = reinterpret_cast<Base *>(pure_base);
    print(base);
    print(*base);
    std::cout << "----Members  -----" << std::endl;
    pub(single);
    pub(&single);
    pub(wrapper);
    pub(&wrapper);
    pub(empty);
    pub(&empty);
    //FIXME: Dangerous!
    pub(base);
    pub(*base);

    DynamicMemory::main(argc, argv);
#if defined(_WIN32_WINNT)
    system("pause");
#endif
    return 0;
}

