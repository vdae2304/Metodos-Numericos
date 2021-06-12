#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <initializer_list>
#include <cmath>

namespace num_met {

    template <class T> class submatrix;

    // Clase matriz.
    template <class T>
    class matrix { 
    public:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef unsigned int size_type;

    private:
        size_type nrows, ncols;
        T *data;

    public:
        // Construye una matriz vacia.
        matrix() {
            nrows = 0;
            ncols = 0;
            data = NULL;
        }

        // Construye una matriz de tamaño dado.
        matrix(size_type m, size_type n) {
            nrows = m;
            ncols = n;
            data = new T[nrows * ncols];
        }

        // Construye una matriz de tamaño dado y la inicializa a un valor dado.
        matrix(size_type m, size_type n, const T &val) {
            nrows = m;
            ncols = n;
            data = new T[nrows * ncols];
            std::fill_n(data, nrows*ncols, val);
        }

        // Construye una matriz de tamaño dado y la inicializa con los datos dados.
        template <class InputIterator>
        matrix(size_type m, size_type n, InputIterator first) {
            nrows = m;
            ncols = n;
            data = new T[nrows * ncols];
            std::copy_n(first, nrows * ncols, data);
        }

        // Construye una matriz como copia de otra.
        matrix(const matrix &A) {
            nrows = A.nrows;
            ncols = A.ncols;
            data = new T[nrows * ncols];
            std::copy_n(A.data, nrows * ncols, data);
        }

        // Construye una matriz desde un initializer_list.
        matrix(std::initializer_list< std::initializer_list<T> > il) {
            nrows = il.size();
            ncols = il.begin()->size();
            data = new T[nrows * ncols];
            size_type index = 0;
            for (const std::initializer_list<T> &row : il)
                for (const T &element : row)
                    data[index++] = element;
        }

        // Destructor.
        ~matrix() {
            nrows = 0;
            ncols = 0;
            delete[] data;
            data = NULL;
        }

        // Asigna a la matriz los valores de otra.
        matrix& operator=(const matrix &A) {
            nrows = A.nrows;
            ncols = A.ncols;
            delete[] data;
            data = new T[nrows * ncols];
            std::copy_n(A.data, nrows * ncols, data);
            return *this;
        }

        // Asigna a la matriz los valores de un initializer_list.
        matrix& operator=(std::initializer_list< std::initializer_list<T> > il) {
            nrows = il.size();
            ncols = il.begin()->size();
            delete[] data;
            data = new T[nrows * ncols];
            size_type index = 0;
            for (const std::initializer_list<T> &row : il)
                for (const T &element : row)
                    data[index++] = element;
           return *this;
        }

        // Regresa un iterador al inicio de la matriz.
        iterator begin() {
            return data;
        }

        const_iterator begin() const {
            return data;
        }

        // Regresa un iterador al final de la matriz.
        iterator end() {
            return data + nrows*ncols;
        }

        const_iterator end() const {
            return data + nrows*ncols;
        }

        // Regresa las dimensiones de la matriz.
        size_type rows() const {
           return nrows;
        }

        size_type columns() const {
            return ncols;
        }

        // Cambia el tamaño de una matriz. 
        void resize(size_type m, size_type n, const T &val = T()) {
            if (m * n != nrows * ncols) {
                T *new_data = new T[m * n];
                std::copy_n(data, std::min(m*n, nrows*ncols), new_data);
                if (nrows*ncols < m*n)
                    std::fill(new_data + nrows*ncols, new_data + m*n, val);
                delete[] data;
                data = new_data;
            }
            nrows = m;
            ncols = n;
        }

        // Regresa una referencia al elemento en la posicion (i, j). Arroja una excepcion si los indices son invalidos.
        T& at(size_type i, size_type j) {
            if (i >= nrows || j >= ncols)
                throw std::out_of_range("Access to an invalid position.");
            return data[i*ncols + j];
        }

        const T& at(size_type i, size_type j) const {
            if (i >= nrows || j >= ncols)
                throw std::out_of_range("Access to an invalid position.");
            return data[i*ncols + j];
        }

        // Regresa un iterador al comienzo del i-esimo renglon.
        T* operator[](size_type i) {
           return data + i*ncols;
        }

        const T* operator[](size_type i) const {
           return data + i*ncols;
        }

        // Regresa una submatriz.
        submatrix<T> operator()(size_type i_start, size_type i_end, size_type j_start, size_type j_end) {
            if (!(i_start < i_end && i_end <= nrows && j_start < j_end && j_end <= ncols))
                throw std::invalid_argument("Invalid submatrix.");
            submatrix<T> sub;
            sub.mat = this;
            sub.i_start = i_start;
            sub.i_end = i_end;
            sub.j_start = j_start;
            sub.j_end = j_end;
            return sub;
        }

        // Intercambia los valores con otra matriz.
        void swap(matrix &A) {
            std::swap(nrows, A.nrows);
            std::swap(ncols, A.ncols);
            std::swap(data, A.data);
        }

        // Suma a la matriz un escalar.
        matrix& operator+=(const T &x) {
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] += x;
            return *this;
        }

        // Suma a la matriz los valores de otra.
        matrix& operator+=(const matrix &A) {
            if (nrows != A.nrows || ncols != A.ncols)
                throw std::invalid_argument("Matrix shapes don't match.");
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] += A.data[i];
            return *this;
        }

        // Resta a la matriz un escalar.
        matrix& operator-=(const T &x) {
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] -= x;
            return *this;
        }

        // Resta a la matriz los valores de otra.
        matrix& operator-=(const matrix &A) {
            if (nrows != A.nrows || ncols != A.ncols)
                throw std::invalid_argument("Matrix shapes don't match.");
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] -= A.data[i];
            return *this;
        }

        // Multiplica la matriz por un escalar.
        matrix& operator*=(const T &x) {
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] *= x;
            return *this;
        }

        // Multiplica la matriz por los valores de otra.
        matrix& operator*=(const matrix &A) {
            if (nrows != A.nrows || ncols != A.ncols)
                throw std::invalid_argument("Matrix shapes don't match.");
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] *= A.data[i];
            return *this;
        }

        // Divide la matriz por un escalar.
        matrix& operator/=(const T &x) {
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] /= x;
            return *this;
        }

        // Divide la matriz por los valores de otra.
        matrix& operator/=(const matrix &A) {
            if (nrows != A.nrows || ncols != A.ncols)
                throw std::invalid_argument("Matrix shapes don't match.");
            for (size_type i = 0; i < nrows*ncols; ++i)
                data[i] /= A.data[i];
            return *this;
        }
    };


    // Regresa la matriz identidad.
    template <class T>
    matrix<T> identity(unsigned int n) {
        matrix<T> I(n, n, T());
        for (unsigned int i = 0; i < n; ++i)
            I[i][i] = T(1);
        return I;
    }

    // Regresa una matriz con un mismo valor en la diagonal.
    template <class T>
    matrix<T> diagonal(unsigned int n, const T &val) {
        matrix<T> D(n, n, T());
        for (unsigned int i = 0; i < n; ++i)
            D[i][i] = val;
        return D;
    } 

    // Regresa una matriz con una lista de valores en la diagonal.
    template <class InputIterator, class T = typename std::iterator_traits<InputIterator>::value_type>
    matrix<T> diagonal(InputIterator first, InputIterator last) {
        unsigned int n = std::distance(first, last);
        matrix<T> D(n, n, T());
        for (unsigned int i = 0; i < n; ++i) {
            D[i][i] = *first;
            ++first;
        }
        return D;
    }

    // Regresa una matriz triangular con la mitad de los elementos de otra matriz.
    template <class T>
    matrix<T> triangular(const matrix<T> &A, bool lower, bool unit_diagonal = false) {
        if (A.rows() != A.columns())
            throw std::invalid_argument("Expected square matrix");
        matrix<T> B(A.rows(), A.columns(), T());
        if (lower) {
            for (unsigned int i = 0; i < A.rows(); ++i)
                for (unsigned int j = 0; j <= i; ++j)
                    B[i][j] = A[i][j];
        }
        else {
            for (unsigned int i = 0; i < A.rows(); ++i)
                for (unsigned int j = i; j < A.columns(); ++j)
                    B[i][j] = A[i][j];
        }
        if (unit_diagonal) {
            for (unsigned int i = 0; i < A.rows(); ++i)
                B[i][i] = T(1);
        }
        return B;
    } 

    // Regresa la matriz transpuesta.
    template <class T>
    matrix<T> transpose(const matrix<T> &A) {
        matrix<T> B(A.columns(), A.rows());
        for (unsigned int i = 0; i < A.rows(); ++i)
            for (unsigned int j = 0; j < A.columns(); ++j)
                B[j][i] = A[i][j];
        return B;
    }

    // Regresa la traza de una matriz.
    template <class T>
    T trace(const matrix<T> &A) {
        if (A.rows() != A.columns())
            throw std::invalid_argument("Expected square matrix.");
        T out = T();
        for (unsigned int i = 0; i < A.rows(); ++i)
            out += A[i][i];
        return out;
    }    

    // Regresa el producto punto de los datos aplanados.
    template <class T>
    T dot(const matrix<T> &A, const matrix<T> &B) {
        if (A.rows()*A.columns() != B.rows()*B.columns())
            throw std::invalid_argument("Matrix lenghts don't match for dot product.");
        T out = T();
        const T *v = A.begin(), *w = B.begin();
        for (unsigned int i = 0; i < A.rows()*A.columns(); ++i)
            out += v[i] * w[i];
        return out;
    }

    // Regresa el producto de matrices.
    template <class T>
    matrix<T> matmul(const matrix<T> &A, const matrix<T> &B) {
        if (A.columns() != B.rows())
            throw std::invalid_argument("Matrix shapes don't match for matrix multiplication.");
        matrix<T> C(A.rows(), B.columns(), T());
        for (unsigned int i = 0; i < A.rows(); ++i)
            for (unsigned int j = 0; j < B.columns(); ++j)
                for (unsigned int k = 0; k < A.columns(); ++k)
                    C[i][j] += A[i][k] * B[k][j];
        return C;
    }

    // Compara si dos matrices son iguales con cierta tolerancia.
    template <class T>
    bool allclose(const matrix<T> &A, const matrix<T> &B, const T &tol = 1e-6) {
        if (A.rows() != B.rows() || A.columns() != B.columns())
            return false;
        for (unsigned int i = 0; i < A.rows(); ++i)
            for (unsigned int j = 0; j < A.columns(); ++j)
                if (std::abs(A[i][j] - B[i][j]) > tol)
                    return false;
        return true;
    }

    // Operadores aritmeticos matriz-matriz.
    template <class T>
    matrix<T> operator+(const matrix<T> &A, const matrix<T> &B) {
        matrix<T> C(A);
        return C += B;
    }

    template <class T>
    matrix<T> operator-(const matrix<T> &A, const matrix<T> &B) {
        matrix<T> C(A);
        return C -= B;
    }

    template <class T>
    matrix<T> operator*(const matrix<T> &A, const matrix<T> &B) {
        matrix<T> C(A);
        return C *= B;
    }

    template <class T>
    matrix<T> operator/(const matrix<T> &A, const matrix<T> &B) {
        matrix<T> C(A);
        return C /= B;
    }

    // Operadores aritmeticos matriz-escalar.
    template <class T>
    matrix<T> operator+(const matrix<T> &A, const T &x) {
        matrix<T> B(A);
        return B += x;
    }

    template <class T>
    matrix<T> operator+(const T &x, const matrix<T> &A) {
        return A + x;
    }

    template <class T>
    matrix<T> operator-(const matrix<T> &A, const T &x) {
        matrix<T> B(A);
        return B -= x;
    }

    template <class T>
    matrix<T> operator-(const T &x, const matrix<T> &A) {
        matrix<T> B(A.rows(), A.columns(), x);
        return B -= A;
    }

    template <class T>
    matrix<T> operator*(const matrix<T> &A, const T &x) {
        matrix<T> B(A);
        return B *= x;
    }

    template <class T>
    matrix<T> operator*(const T &x, const matrix<T> &A) {
        return A * x;
    }

    template <class T>
    matrix<T> operator/(const matrix<T> &A, const T &x) {
        matrix<T> B(A);
        return B /= x;
    }

    template <class T>
    matrix<T> operator/(const T &x, const matrix<T> &A) {
        matrix<T> B(A.rows(), A.columns(), x);
        return B /= A;
    }

    // Operadores unarios.
    template <class T>
    matrix<T> operator+(const matrix<T> &A) {
        return T() + A;
    }

    template <class T>
    matrix<T> operator-(const matrix<T> &A) {
        return T() - A;
    }


    // Clase submatriz.
    template <class T>
    class submatrix {
    private:
        unsigned int i_start, i_end, j_start, j_end;
        matrix<T> *mat;

    public:
        // Regresa una copia de la submatriz.
        matrix<T> to_matrix() {
            matrix<T> A(i_end - i_start, j_end - j_start);
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    A[i - i_start][j - j_start] = (*mat)[i][j];
            return A;
        }

        // Asigna un mismo valor a toda la submatriz.
        void operator=(const T &val) {
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] = val;
        }

        // Asigna los valores de una matriz.
        void operator=(const matrix<T> &A) {
            if (A.rows() < i_end - i_start || A.columns() < j_end - j_start)
                throw std::invalid_argument("Matrix is too small.");
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] = A[i - i_start][j - j_start];
        }

        // Suma a la submatriz un escalar.
        void operator+=(const T &val) {
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] += val;
        }

        // Suma a la submatriz otra matriz.
        void operator+=(const matrix<T> &A) {
            if (A.rows() < i_end - i_start || A.columns() < j_end - j_start)
                throw std::invalid_argument("Matrix is too small.");
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] += A[i - i_start][j - j_start];
        }

        // Resta a la submatriz un escalar.
        void operator-=(const T &val) {
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] -= val;
        }

        // Resta a la submatriz otra matriz.
        void operator-=(const matrix<T> &A) {
            if (A.rows() < i_end - i_start || A.columns() < j_end - j_start)
                throw std::invalid_argument("Matrix is too small.");
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] -= A[i - i_start][j - j_start];
        }

        // Multiplica la submatriz por un escalar.
        void operator*=(const T &val) {
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] *= val;
        }

        // Multiplica la submatriz por otra matriz.
        void operator*=(const matrix<T> &A) {
            if (A.rows() < i_end - i_start || A.columns() < j_end - j_start)
                throw std::invalid_argument("Matrix is too small.");
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] *= A[i - i_start][j - j_start];
        }

        // Divide la submatriz por un escalar.
        void operator/=(const T &val) {
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] *= val;
        }

        // Divide la submatriz por otra matriz.
        void operator/=(const matrix<T> &A) {
            if (A.rows() < i_end - i_start || A.columns() < j_end - j_start)
                throw std::invalid_argument("Matrix is too small.");
            for (unsigned int i = i_start; i < i_end; ++i)
                for (unsigned int j = j_start; j < j_end; ++j)
                    (*mat)[i][j] *= A[i - i_start][j - j_start];
        }

        friend class matrix<T>;
    };

}

#endif // MATRIX_H_INCLUDED