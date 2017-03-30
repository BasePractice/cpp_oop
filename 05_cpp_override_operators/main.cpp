#include <cstdlib>
#include <string>
#include <iomanip>
#include <malloc.h>
#include <iostream>
#include <ostream>

template<class T>
constexpr T pi = T(3.1415926535897932385);

template<typename __Type>
__Type circular_area(__Type r) {
    return pi<__Type> * r * r;
}

template<typename __Type>
class Numeric final {
    __Type _value;
public:
    Numeric(__Type value)
            : _value(value)
    {}

    Numeric(const Numeric<__Type> &other)
            : _value(other._value){
    }

    Numeric<__Type> &operator=(const Numeric<__Type> &other) {
        _value = other._value;
        return (*this);
    }

    __Type operator*() const {
        return _value;
    }

    Numeric<__Type> operator+(const Numeric<__Type> &other) {
        return Numeric<__Type>(_value + other._value);
    }

    Numeric<__Type> operator+(const __Type &value) {
        _value += value;
        return Numeric<__Type>(_value + value);
    }
};

template<typename __Type>
std::ostream &
operator<<(std::ostream &output, const Numeric<__Type> &numeric) {
    output << *numeric;
    return output;
}

class Integer {
    int _v;
public:
    Integer(int v)
            : _v(v)
    {}

    Integer operator+(const Integer &other) const {
        return Integer(value() + other.value());
    }

    Integer operator+(int value) const {
        return Integer(this->value() + value);
    }

    Integer &operator+=(const Integer &other) {
        _v += other.value();
        return (*this);
    }

    Integer &operator+=(int value) {
        _v += value;
        return (*this);
    }

    int operator*() const {
        return value();
    }

    int value() const {
        return _v;
    }

    void set(int value) {
        _v = value;
    }
};

std::ostream &
operator<<(std::ostream &output, const Integer &object) {
    output << object.value();
    return output;
}

std::istream &
operator>>(std::istream &input, Integer &object) {
    int value;
    input >> value;
    object.set(value);
    return input;
}

int
main(int argc, char **argv) {
    Integer object(10);

    std::cout << object << std::endl;
    std::cout << object + object << std::endl;
    std::cout << object << std::endl;
    object += object;
    std::cout << object << std::endl;

    std::cout << object + 10 << std::endl;

    std::cout << *object << std::endl;


    std::cout << "Input integer: ";
    std::cin >> object;
    std::cout << *object << std::endl;

    Numeric<int> numeric(10);
    Numeric<double> numeric2(10.40);

    std::cout << numeric << std::endl;
    std::cout << numeric2 << std::endl;
    std::cout << numeric + numeric << std::endl;
    std::cout << numeric2 + numeric2 << std::endl;

    std::cout << circular_area(10.) << std::endl;
    std::cout << circular_area(10.f) << std::endl;
#if defined(_WIN32_WINNT)
    system("pause");
#endif
    return 0;
}

