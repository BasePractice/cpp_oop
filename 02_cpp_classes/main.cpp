#include <cstdlib>
#include <iostream>

class Object {
    int _m1;
    int _m2;
public:
    Object(int m1, int m2): _m1(m1), _m2(m2) {};
    ~Object() {};

    int m1() const {return _m1;}
    int m2() const {return _m2;}

} object(10, 10);

class IntObject {
    int _value;
public:
    IntObject(int value) : _value(value) {}
//    IntObject(int value) { _value =  value;};
};

class ObjectObject {
    Object _object;
public:
    ObjectObject(int m1, int m2): _object{m1, m2} {};
};

class PrivateClass {
    int _m1;
public:
    PrivateClass(int m1) : _m1(m1) {}
private:
    friend class FriendClass;
};


class FriendClass {
public:
    void m(const PrivateClass &p) {
        std::cout << p._m1 << std::endl;
    }
};

class StaticMemberClass {
public:
    static int p;
    static const int c = 20;
};

int StaticMemberClass::p = 0;


class CopyableObject {
    int _p;
public:
    CopyableObject() {
        _p = 0;
        std::cout << "Default constructor" << std::endl;
    }
    CopyableObject(int p): _p(p) {
        std::cout << "Constructor object: " << p << std::endl;
    }
    CopyableObject(const CopyableObject &other)
            : _p(other._p) {
        std::cout << "Copy object: " << other._p << std::endl;
    }
    CopyableObject &operator=(const CopyableObject &other) {
        _p = other._p;
        std::cout << "Assignment object: " << other._p << std::endl;
        return (*this);
    }
};

class NonCopyableObject {
    int _p;
public:
    NonCopyableObject(int p): _p(p) {}
private:
    NonCopyableObject(const NonCopyableObject &other) = delete;
    NonCopyableObject &operator=(const NonCopyableObject &other) = delete;
};

class InitializerListObject;
std::ostream &operator<<(std::ostream &output, const InitializerListObject &object);

class InitializerListObject {
public:
    InitializerListObject() {
        std::cout << "[InitializerListObject]Default constructor" << std::endl;
    }
    InitializerListObject(const InitializerListObject &){
        std::cout << "[InitializerListObject]Copy constructor" << std::endl;
    }
    //Without const - error
    InitializerListObject(const std::initializer_list<InitializerListObject> &list) {
        std::cout << "[InitializerListObject]Initializer constructor(";
        int i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            if (i > 0)
                std::cout << ", ";
            std::cout << "Object: " << (*it);
        }
        std::cout << ")" << std::endl;
    }
};

std::ostream &operator<<(std::ostream &output, const InitializerListObject &object) {
    std::cout << "InitializerListObject";
    return output;
}

int
main(int argc, char **argv) {
    std::cout << "M1: " << object.m1() << ", M2: " << object.m2() << std::endl;

    //Init
    {
        IntObject int1(10);
        IntObject int2 = 10;
        IntObject int3 {10};
        IntObject int4 = {10};
    }
    //Friend class
    {
        PrivateClass p = 10;
        FriendClass().m(p);
    }
    //Static member
    {
        std::cout << "Static p: " << StaticMemberClass::p << std::endl;
        std::cout << "Static c: " << StaticMemberClass::c << std::endl;
    }
    //Copyable.
    {
        CopyableObject object1;
        CopyableObject object2(); //This is the function declaration
        CopyableObject object3{};
        CopyableObject object4 = 10;
        CopyableObject object5 = {10};
        CopyableObject object6 = object4;

        //NonCopyableObject nobject1; //Disable default constructor
        NonCopyableObject nobject2(); //This is the function declaration
        //NonCopyableObject nobject3{};
//        NonCopyableObject nobject4 = 10; //Not construct object
        NonCopyableObject nobject5 = {10};
        //NonCopyableObject nobject6 = nobject4;
    }
    //Destructor
    {

    }
    //Default
    {
        InitializerListObject defaultObject;
        InitializerListObject object = InitializerListObject {defaultObject}; //Call initializer-list constructor(not copy)
        InitializerListObject object2 = InitializerListObject {defaultObject, defaultObject};
    }


#if defined(_WIN32_WINNT)
    system("pause");
#endif
    return 0;
}

