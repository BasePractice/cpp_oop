#ifndef CPP_OOP_MATRIX_H
#define CPP_OOP_MATRIX_H

#include <iostream>

class Matrix {
    int      _rows;
    int      _cols;
    double **_data;
public:
    Matrix(int rows, int cols)
            : _rows(rows), _cols(cols), _data(create(rows, cols)) {
        std::cout << "Create  constructor(" << rows << ", " << cols << ")" << std::endl;
    }

    ~Matrix() {
        std::cout << "Call     destructor(" << rows() << ", " << cols() << ")" << std::endl;
        destroy(_data, _rows, _cols);
    }

    Matrix(const Matrix &other)
            : _rows(other.rows()), _cols(other.cols()), _data(create(other.rows(), other.cols())) {
        std::cout << "Copy    constructor(" << rows() << ", " << cols() << ")" << std::endl;
        copy(*this, other);
    }

    Matrix &operator=(const Matrix &other) {
        /** Not implement yet */
        std::cout << "Assignment operator(" << other.rows() << ", " << other.cols() << ")" << std::endl;
        return (*this);
    }

    Matrix add(const Matrix &that) {
        /** Not implement yet */
        return Matrix(0, 0);
    }

    Matrix sub(const Matrix &that) {
        /** Not implement yet */
        return Matrix(0, 0);
    }

    Matrix mul(const Matrix &that) {
        /** Not implement yet */
        return Matrix(0, 0);
    }

    Matrix div(const Matrix &that) {
        /** Not implement yet */
        return Matrix(0, 0);
    }

    int rows() const {
        return _rows;
    }

    int cols() const {
        return _cols;
    }

    double item(int row, int col) const {
        return _data[row][col];
    }

    void set(int row, int col, double value) {
        _data[row][col] = value;
    }
private:
    static void copy(Matrix &dst, const Matrix &src) {
        for (int i = 0; i < src.rows(); ++i) {
            for (int j = 0; j < src.cols(); ++j) {
                dst.set(i, j, src.item(i, j));
            }
        }
    }

    static double ** create(int rows, int cols) {
        double ** ret = new double * [rows];
        for (int i = 0; i < rows; ++i) {
            ret[i] = new double [cols];
        }
        return ret;
    }

    static void destroy(double ** array, int rows, int cols) {
        for (int i = 0; i < rows; ++i) {
            delete [] array[i];
        }
        delete [] array;
    }
};

#endif //CPP_OOP_MATRIX_H
