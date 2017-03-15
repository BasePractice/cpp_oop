#include <cstdlib>
#include <iostream>

static int add(int a, int b) {
    return a + b;
}

static int sub(int a, int b) {
    return a - b;
}

static int op(int arg1, int arg2, int (*call)(int, int)) {
    return (*call)(arg1, arg2);
}

class Object {
    int _h;
    int _w;
public:
    Object(int h, int w)
            : _h(h), _w(w), o_h(h), o_w(w) {}

    int h() const {
        return _h;
    }

    int w() const {
        return _w;
    }

    int o_h;
    int o_w;
};
template <class Functor>
void call(Functor fun) {
    fun();
}

int
main(int argc, char **argv) {
    {
        long matrix[5];
//        long (*p)[5] = &matrix;
//        long (&pp)[5] = *p;
        long *p_matrix = nullptr;


        p_matrix = matrix;
        (*p_matrix) = 1;
        ++p_matrix;
        (*p_matrix) = 2;
        p_matrix = &matrix[2];
        (*p_matrix) = 3;
        p_matrix = matrix + 3;
        (*p_matrix) = 4;
        p_matrix = matrix;
        *(p_matrix + 4) = 5;
        for (int i = 0; i < sizeof(matrix) / sizeof(matrix[0]); ++i) {
            if (i > 0)
                std::cout << ", ";
            std::cout << matrix[i];
        }
        std::cout << std::endl;

        matrix[3] = 0;
        *(matrix + 3) = 0;

        /**
         T *p = ...

         p += 3 <=> (T *)((unsigned char *)p + 3 * sizeof (T))
         */
    }

    //Mutable
    {
        int x;
        int *p1 = &x;               // Изменяемый указатель на изменяемые данные
        const int *p2 = &x;         // Изменяемый указатель на неизменяемые данные
        int *const p3 = &x;         // Неизменяемый указатель на изменяемые данные
        const int *const p4 = &x;   // Неизменяемый указатель на неизменяемые данные
    }

    //Строка - частный случай массива
    {

    }

    //Указатели на функцию
    //TODO: http://www.newty.de/fpt/index.html

    {
        int ret;
        int (*minus)(int, int) = sub;

        ret = op(10, 10, add);
        std::cout << "ADD: " << ret << std::endl;
        ret = op(10, 10, minus);
        std::cout << "SUB: " << ret << std::endl;
    }

    //Ссылки
    {
        long *p = nullptr;
        long &r = *p; //разименование null указателя

//        std::string &rs;  // Ошибка
        std::string c("H");
        std::string &rs = c;
    }

    //Указатели на методы
    {

        int (Object::*method_call)() const = &Object::h;
        int Object::*field_call = &Object::o_h;

        Object      o(10, 20);
        Object *p = &o;

        std::cout << "H: " << (o.*method_call)() << std::endl;
        std::cout << "H: " << (p->*method_call)() << std::endl;
        method_call = &Object::w;
        std::cout << "W: " << (o.*method_call)() << std::endl;
        std::cout << "W: " << (p->*method_call)() << std::endl;


        std::cout << "H: " << o.*field_call << std::endl;
        std::cout << "H: " << p->*field_call << std::endl;
        field_call = &Object::o_w;
        std::cout << "W: " << o.*field_call << std::endl;
        std::cout << "W: " << p->*field_call << std::endl;
    }

    //Функтор
    {
        class Fun {
        public:
            void operator()() const {
                std::cout << "Вызов функтора без параметров и результата" << std::endl;
            }
        };

        void (Fun::*name)() const = &Fun::operator();

        Fun fun;

        fun();
        (fun.*name)();
        call(fun);
    }


#if defined(_WIN32_WINNT)
    system("pause");
#endif
    return 0;
}

