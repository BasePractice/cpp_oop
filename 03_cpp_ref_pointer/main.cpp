#include <cstdlib>
#include <iostream>

long *p_matrix = nullptr;

int
main(int argc, char **argv) {
    long matrix[5];

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

#if defined(_WIN32_WINNT)
    system("pause");
#endif
    return 0;
}

