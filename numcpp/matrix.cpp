#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    ////////////////////////////////////////////////////////////////////////////
    // Constructors and destructor                                            //
    ////////////////////////////////////////////////////////////////////////////

    // Default constructor. Constructs an empty matrix with no elements.
    template <class T>
    matrix<T>::matrix() {
        this->nrows = 0;
        this->ncols = 0;
        this->values = nullptr;
    }

    // Size constructor. Constructs a matrix with m rows and n columns.
    template <class T>
    matrix<T>::matrix(size_t m, size_t n) {
        this->nrows = m;
        this->ncols = n;
        this->values = new T[m * n];
    }

    // Fill constructor. Constructs a matrix with m rows and n columns, each
    // element initialized to val.
    template <class T>
    matrix<T>::matrix(size_t m, size_t n, const T &val) {
        this->nrows = m;
        this->ncols = n;
        this->values = new T[m * n];
        for (size_t i = 0; i < m * n; ++i) {
            this->values[i] = val;
        }
    }

    // Container constructor. Constructs an array with m rows and n columns,
    // each element initialized to its corresponding element in the container,
    // in the same order.
    template <class T>
    template <class InputIterator>
    matrix<T>::matrix(size_t m, size_t n, InputIterator first) {
        this->nrows = m;
        this->ncols = n;
        this->values = new T[m * n];
        for (size_t i = 0; i < m * n; ++i) {
            this->values[i] = *first;
            ++first;
        }
    }

    // Copy constructor. Constructs a matrix with a copy of each of the
    // elements in A, in the same order.
    template <class T>
    matrix<T>::matrix(const matrix &A) {
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        this->values = new T[this->nrows * this->ncols];
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = A.values[i];
        }
    }

    template <class T>
    template <class U>
    matrix<T>::matrix(const matrix<U> &A) {
        this->nrows = A.rows();
        this->ncols = A.columns();
        this->values = new T[this->nrows * this->ncols];
        size_t n = 0;
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                this->values[n++] = U(A.at(i, j));
            }
        }
    }

    // Move constructor. Constructs a matrix that acquires the elements of A.
    template <class T>
    matrix<T>::matrix(matrix &&A) {
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        this->values = A.values;
        A.nrows = 0;
        A.ncols = 0;
        A.values = nullptr;
    }

    // Submatrix constructor. Constructs a matrix with a copy of each of the
    // elements in A, in the same order.
    template <class T>
    matrix<T>::matrix(const submatrix<T> &A) {
        this->nrows = A.rows();
        this->ncols = A.columns();
        this->values = new T[this->nrows * this->ncols];
        size_t n = 0;
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                this->values[n++] = A.at(i, j);
            }
        }
    }

    // Initializer list of lists. Constructs a matrix with a copy of each of the
    // elements in il, in the same order.
    template <class T>
    matrix<T>::matrix(std::initializer_list< std::initializer_list<T> > ill) {
        this->nrows = ill.size();
        this->ncols = 0;
        for (const std::initializer_list<T> &il : ill) {
            if (this->ncols < il.size()) {
                this->ncols = il.size();
            }
        }
        this->values = new T[this->nrows * this->ncols];
        size_t n = 0;
        for (const std::initializer_list<T> &il : ill) {
            for (const T &val : il) {
                this->values[n++] = val;
            }
            n += this->ncols - il.size();
        }
    }

    // Destructor. Destroys the matrix.
    template <class T>
    matrix<T>::~matrix() {
        delete[] this->values;
        this->nrows = 0;
        this->ncols = 0;
        this->values = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Assignment operators                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Copy assignment. Assigns the contents of A to *this after resizing the
    // object (if necessary).
    template <class T>
    matrix<T>& matrix<T>::operator= (const matrix<T> &A) {
        if (this->nrows * this->ncols != A.nrows * A.ncols) {
            delete[] this->values;
            this->values = new T[A.nrows * A.ncols];
        }
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = A.values[i];
        }
        return *this;
    }

    template <class T>
    template <class U>
    matrix<T>& matrix<T>::operator= (const matrix<U> &A) {
        if (this->nrows * this->ncols != A.rows() * A.columns()) {
            delete[] this->values;
            this->values = new T[A.rows() * A.columns()];
        }
        this->nrows = A.rows();
        this->ncols = A.columns();
        size_t n = 0;
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                this->values[n++] = U(A.at(i, j));
            }
        }
        return *this;
    }

    // Move assignment. Acquires the contents of A.
    template <class T>
    matrix<T>& matrix<T>::operator= (matrix<T> &&A) {
        if (this != &A) {
            delete[] this->values;
            this->nrows = A.nrows;
            this->ncols = A.ncols;
            this->values = A.values;
            A.nrows = 0;
            A.ncols = 0;
            A.values = nullptr;
        }
        return *this;
    }

    // Submatrix assignment. Assigns the contents of A to *this after resizing 
    // the object (if necessary).
    template <class T>
    matrix<T>& matrix<T>::operator= (const submatrix<T> &A) {
        if (this->nrows * this->ncols != A.rows() * A.columns()) {
            delete[] this->values;
            this->values = new T[A.rows() * A.columns()];
        }
        this->nrows = A.rows();
        this->ncols = A.columns();
        size_t n = 0;
        for (size_t i = 0; i < this->nrows; ++i) {
            for (size_t j = 0; j < this->ncols; ++j) {
                this->values[n++] = A.at(i, j);
            }
        }
        return *this;
    }

    // Fill assignment. Assigns val to every element.
    template <class T>
    matrix<T>& matrix<T>::operator= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] = val;
        }
    }

    // Compound assignments.
    template <class T>
    matrix<T>& matrix<T>::operator+= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] += A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator-= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] -= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator*= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] *= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator/= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] /= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator%= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] %= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator&= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] &= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator|= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] |= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator^= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] ^= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator<<= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] <<= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator>>= (const matrix<T> &A) {
        if (this->nrows != A.nrows || this->ncols != A.ncols) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << this->nrows << "," << this->ncols << ") ("
                  << A.nrows << "," << A.ncols << ")";
            throw std::runtime_error(error.str());
        }
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] >>= A.values[i];
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator+= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] += val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator-= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] -= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator*= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] *= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator/= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] /= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator%= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] %= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator&= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] &= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator|= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] |= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator^= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] ^= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator<<= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] <<= val;
        }
        return *this;
    }

    template <class T>
    matrix<T>& matrix<T>::operator>>= (const T &val) {
        for (size_t i = 0; i < this->nrows * this->ncols; ++i) {
            this->values[i] >>= val;
        }
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Operator overloading                                                   //
    ////////////////////////////////////////////////////////////////////////////

    // Unary operators.
    template <class T>
    matrix<T> operator+ (const matrix<T> &A) {
        return A;
    }

    template <class T>
    matrix<T> operator- (const matrix<T> &A) {
        matrix<T> out(A);
        return A *= -1;
    }

    template <class T>
    matrix<T> operator~ (const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = ~A[i][j];
            }
        }
        return out;
    }

    matrix<bool> operator! (const matrix<bool> &A) {
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                out[i][j] = !A[i][j];
            }
        }
        return out;
    }

    // Arithmetic operators.
    template <class T>
    matrix<T> operator+ (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out += B;
    }

    template <class T>
    matrix<T> operator+ (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out += val;
    }

    template <class T>
    matrix<T> operator+ (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out += A;
    }

    template <class T>
    matrix<T> operator- (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out -= B;
    }

    template <class T>
    matrix<T> operator- (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out -= val;
    }

    template <class T>
    matrix<T> operator- (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out -= A;
    }

    template <class T>
    matrix<T> operator* (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out *= B;
    }

    template <class T>
    matrix<T> operator* (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out *= val;
    }

    template <class T>
    matrix<T> operator* (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out *= A;
    }

    template <class T>
    matrix<T> operator/ (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out /= B;
    }

    template <class T>
    matrix<T> operator/ (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out /= val;
    }

    template <class T>
    matrix<T> operator/ (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out /= A;
    }

    template <class T>
    matrix<T> operator% (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out %= B;
    }

    template <class T>
    matrix<T> operator% (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out %= val;
    }

    template <class T>
    matrix<T> operator% (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out %= A;
    }

    // Bitwise operators.
    template <class T>
    matrix<T> operator& (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out &= B;
    }

    template <class T>
    matrix<T> operator& (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out &= val;
    }

    template <class T>
    matrix<T> operator& (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out &= A;
    }

    template <class T>
    matrix<T> operator| (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out |= B;
    }

    template <class T>
    matrix<T> operator| (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out |= val;
    }

    template <class T>
    matrix<T> operator| (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out |= A;
    }

    template <class T>
    matrix<T> operator^ (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out ^= B;
    }

    template <class T>
    matrix<T> operator^ (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out ^= val;
    }

    template <class T>
    matrix<T> operator^ (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out ^= A;
    }

    template <class T>
    matrix<T> operator<< (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out <<= B;
    }

    template <class T>
    matrix<T> operator<< (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out <<= val;
    }

    template <class T>
    matrix<T> operator<< (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out <<= A;
    }

    template <class T>
    matrix<T> operator>> (const matrix<T> &A, const matrix<T> &B) {
        matrix<T> out(A);
        return out >>= B;
    }

    template <class T>
    matrix<T> operator>> (const matrix<T> &A, const T &val) {
        matrix<T> out(A);
        return out >>= val;
    }

    template <class T>
    matrix<T> operator>> (const T &val, const matrix<T> &A) {
        matrix<T> out(A.rows(), A.columns(), val);
        return out >>= A;
    }

    // Logical operators.
    matrix<bool> operator&& (const matrix<bool> &A, const matrix<bool> &B) {
        return (A & B);
    }

    matrix<bool> operator&& (const matrix<bool> &A, bool val) {
        return (A & val);
    }

    matrix<bool> operator&& (bool val, const matrix<bool> &A) {
        return (val & A);
    }

    matrix<bool> operator|| (const matrix<bool> &A, const matrix<bool> &B) {
        return (A | B);
    }

    matrix<bool> operator|| (const matrix<bool> &A, bool val) {
        return (A | val);
    }

    matrix<bool> operator|| (bool val, const matrix<bool> &A) {
        return (val | A);
    }

    // Relational operators.
    template <class T>
    matrix<bool> operator== (const matrix<T> &A, const matrix<T> &B) {
        if (A.rows() != B.rows() || A.columns() != B.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (A[i][j] == B[i][j]);
            }
        }
        return out;
    }

    template <class T>
    matrix<bool> operator== (const matrix<T> &A, const T &val) {
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (A[i][j] == val);
            }
        }
        return out;
    }

    template <class T>
    matrix<bool> operator== (const T &val, const matrix<T> &A) {
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (val == A[i][j]);
            }
        }
        return out;
    }

    template <class T>
    matrix<bool> operator!= (const matrix<T> &A, const matrix<T> &B) {
        return !(A == B);
    }

    template <class T>
    matrix<bool> operator!= (const matrix<T> &A, const T &val) {
        return !(A == val);
    }

    template <class T>
    matrix<bool> operator!= (const T &val, const matrix<T> &A) {
        return !(val == A);
    }

    template <class T>
    matrix<bool> operator< (const matrix<T> &A, const matrix<T> &B) {
        if (A.rows() != B.rows() || A.columns() != B.columns()) {
            std::ostringstream error;
            error << "operands could not be broadcast together with shapes ("
                  << A.rows() << "," << A.columns() << ") ("
                  << B.rows() << "," << B.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (A[i][j] < B[i][j]);
            }
        }
        return out;
    }

    template <class T>
    matrix<bool> operator< (const matrix<T> &A, const T &val) {
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (A[i][j] < val);
            }
        }
        return out;
    }

    template <class T>
    matrix<bool> operator< (const T &val, const matrix<T> &A) {
        matrix<bool> out(A.rows(), A.columns());
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = 0; j < out.columns(); ++j) {
                out[i][j] = (val < A[i][j]);
            }
        }
        return out;
    }

    template <class T>
    matrix<bool> operator> (const matrix<T> &A, const matrix<T> &B) {
        return (B < A);
    }

    template <class T>
    matrix<bool> operator> (const matrix<T> &A, const T &val) {
        return (val < A);
    }

    template <class T>
    matrix<bool> operator> (const T &val, const matrix<T> &A) {
        return (A < val);
    }

    template <class T>
    matrix<bool> operator<= (const matrix<T> &A, const matrix<T> &B) {
        return !(B < A);
    }

    template <class T>
    matrix<bool> operator<= (const matrix<T> &A, const T &val) {
        return !(val < A);
    }

    template <class T>
    matrix<bool> operator<= (const T &val, const matrix<T> &A) {
        return !(A < val);
    }

    template <class T>
    matrix<bool> operator>= (const matrix<T> &A, const matrix<T> &B) {
        return !(A < B);
    }

    template <class T>
    matrix<bool> operator>= (const matrix<T> &A, const T &val) {
        return !(A < val);
    }

    template <class T>
    matrix<bool> operator>= (const T &val, const matrix<T> &A) {
        return !(val < A);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Matrix indexing                                                        //
    ////////////////////////////////////////////////////////////////////////////

    // Array subscript. Returns a pointer to the beginning of the i-th row.
    template <class T>
    T* matrix<T>::operator[] (size_t i) {
        return this->values + (i * this->ncols);
    }

    template <class T>
    const T* matrix<T>::operator[] (size_t i) const {
        return this->values + (i * this->ncols);
    }

    // Returns a reference to the element at row i and column j in the the
    // matrix.
    template <class T>
    T& matrix<T>::at(size_t i, size_t j) {
        return this->values[i * this->ncols + j];
    }

    template <class T>
    const T& matrix<T>::at(size_t i, size_t j) const {
        return this->values[i * this->ncols + j];
    }

    ////////////////////////////////////////////////////////////////////////////
    // Methods                                                                //
    ////////////////////////////////////////////////////////////////////////////

    // Apply a function to each of the elements in *this.
    template <class T>
    template <class Function>
    void matrix<T>::apply(Function f) {
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                this->at(i, j) = f(this->at(i, j));
            }
        }
    }

    // Return the index of the maximum value.
    template <class T>
    std::pair<size_t, size_t> matrix<T>::argmax() const {
        std::pair<size_t, size_t> index(0, 0);
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                if (this->at(index.first, index.second) < this->at(i, j)) {
                    index.first = i;
                    index.second = j;
                }
            }
        }
        return index;
    }

    // Return the index of the maximum value along a given axis.
    template <class T>
    array<size_t> matrix<T>::argmax(size_t axis) const {
        if (axis == 0) {
            array<size_t> out(this->columns(), 0);
            for (size_t i = 1; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    if (this->at(out[j], j) < this->at(i, j)) {
                        out[j] = i;
                    }
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<size_t> out(this->rows(), 0);
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 1; j < this->columns(); ++j) {
                    if (this->at(i, out[i]) < this->at(i, j)) {
                        out[i] = j;
                    }
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Return the index of the minimum value.
    template <class T>
    std::pair<size_t, size_t> matrix<T>::argmin() const {
        std::pair<size_t, size_t> index(0, 0);
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                if (this->at(i, j) < this->at(index.first, index.second)) {
                    index.first = i;
                    index.second = j;
                }
            }
        }
        return index;
    }

    // Return the index of the minimum value along a given axis.
    template <class T>
    array<size_t> matrix<T>::argmin(size_t axis) const {
        if (axis == 0) {
            array<size_t> out(this->columns(), 0);
            for (size_t i = 1; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    if (this->at(i, j) < this->at(out[j], j)) {
                        out[j] = i;
                    }
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<size_t> out(this->rows(), 0);
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 1; j < this->columns(); ++j) {
                    if (this->at(i, j) < this->at(i, out[i])) {
                        out[i] = j;
                    }
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Clip (limit) the values in the matrix. Given an interval, values outside
    // the interval are clipped to the interval edges.
    template <class T>
    void matrix<T>::clip(const T &a_min, const T &a_max) {
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                if (this->at(i, j) < a_min) {
                    this->at(i, j) = a_min;
                }
                else if (a_max < this->at(i, j)) {
                    this->at(i, j) = a_max;
                }
            }
        }
    }

    // Returns the number of columns in the matrix.
    template <class T>
    size_t matrix<T>::columns() const {
        return this->ncols;
    }

    // Returns a pointer to the memory array used internally by the matrix.
    template <class T>
    T* matrix<T>::data() {
        return this->values;
    }

    template <class T>
    const T* matrix<T>::data() const {
        return this->values;
    }

    // Returns the matrix multiplication of a matrix and a column vector.
    template <class T>
    array<T> matrix<T>::dot(const array<T> &v) const {
        if (this->columns() != v.size()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: (" << this->rows()
                  << "," << this->columns() << ") (" << v.size() << ",)";
            throw std::runtime_error(error.str());
        }
        array<T> out(this->rows(), T(0));
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                out[i] += this->at(i, j) * v[j];
            }
        }
        return out;
    }

    // Returns the matrix multiplication of two matrices.
    template <class T>
    matrix<T> matrix<T>::dot(const matrix &A) const {
        if (this->columns() != A.rows()) {
            std::ostringstream error;
            error << "matmul: Number of columns in left operand does not match "
                  << "number of rows in right operand: (" << this->rows()
                  << "," << this->columns() << ") (" << A.rows() << ","
                  << A.columns() << ")";
            throw std::runtime_error(error.str());
        }
        matrix<T> out(this->rows(), A.columns(), T(0));
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < A.columns(); ++j) {
                for (size_t k = 0; k < this->columns(); ++k) {
                    out.at(i, j) += this->at(i, k) * A.at(k, j);
                }
            }
        }
        return out;
    }

    // Returns a copy of the matrix collapsed into an array.
    template <class T>
    array<T> matrix<T>::flatten() const {
        size_t length = this->rows() * this->columns();
        return array<T>(this->data(), this->data() + length);
    }

    // Returns the maximum value contained in the matrix.
    template <class T>
    T matrix<T>::max() const {
        std::pair<size_t, size_t> index = this->argmax();
        return this->at(index.first, index.second);
    }

    // Returns the maximum value contained in the matrix along a given axis.
    template <class T>
    array<T> matrix<T>::max(size_t axis) const {
        array<size_t> indices = this->argmax(axis);
        array<T> out(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            out[i] = (axis == 0) ? this->at(indices[i], i)
                                 : this->at(i, indices[i]);
        }
        return out;
    }

    // Returns the average of the matrix elements.
    template <class T>
    T matrix<T>::mean() const {
        return this->sum() / (this->rows() * this->columns());
    }

    // Returns the average of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::mean(size_t axis) const {
        array<T> out = this->sum(axis);
        out /= (axis == 0) ? this->rows() : this->columns();
        return out;
    }

    // Returns the minimum value contained in the matrix.
    template <class T>
    T matrix<T>::min() const {
        std::pair<size_t, size_t> index = this->argmin();
        return this->at(index.first, index.second);
    }

    // Returns the minimum value contained in the matrix along a given axis.
    template <class T>
    array<T> matrix<T>::min(size_t axis) const {
        array<size_t> indices = this->argmin(axis);
        array<T> out(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            out[i] = (axis == 0) ? this->at(indices[i], i)
                                 : this->at(i, indices[i]);
        }
        return out;
    }

    // Return the product of the matrix elements.
    template <class T>
    T matrix<T>::prod() const {
        T out = T(1);
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                out *= this->at(i, j);
            }
        }
        return out;
    }

    // Return the product of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::prod(size_t axis) const {
        if (axis == 0) {
            array<T> out(this->columns(), T(1));
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    out[j] *= this->at(i, j);
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<T> out(this->rows(), T(1));
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    out[i] *= this->at(i, j);
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Resizes the matrix, changing its size to m rows and n columns.
    // If the new size is smaller than the current size, the content is
    // reduced, removing the last elements.
    // If the new size is greater than the current size, the content is
    // expanded by inserting at the end as many elements as needed.
    // In any case, the contents will be rearranged to fit the new size.
    template <class T>
    void matrix<T>::resize(size_t m, size_t n, const T &val) {
        if (this->nrows * this->ncols != m * n) {
            matrix<T> tmp_copy(*this);
            delete[] this->values;
            this->values = new T[m * n];
            for (size_t i = 0; i < this->nrows*this->ncols && i < m*n; ++i) {
                this->values[i] = tmp_copy.values[i];
            }
            for (size_t i = this->nrows*this->ncols; i < m*n; ++i) {
                this->values[i] = val;
            }
        }
        this->nrows = m;
        this->ncols = n;
    }

    // Returns the number of rows in the matrix.
    template <class T>
    size_t matrix<T>::rows() const {
        return this->nrows;
    }

    // Returns the standard deviation of the matrix elements.
    template <class T>
    T matrix<T>::stddev(size_t ddof) const {
        return std::sqrt(this->var(ddof));
    }

    // Returns the standard deviation of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::stddev(size_t ddof, size_t axis) const {
        array<T> out = this->var(ddof, axis);
        out.apply(std::sqrt);
        return out;
    }

    // Return the sum of the matrix elements.
    template <class T>
    T matrix<T>::sum() const {
        T out = T(0);
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                out += this->at(i, j);
            }
        }
        return out;
    }

    // Return the sum of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::sum(size_t axis) const {
        if (axis == 0) {
            array<T> out(this->columns(), T(0));
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    out[j] += this->at(i, j);
                }
            }
            return out;
        }
        else if (axis == 1) {
            array<T> out(this->rows(), T(0));
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    out[i] += this->at(i, j);
                }
            }
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }

    // Swap contents with A.
    template <class T>
    void matrix<T>::swap(matrix &A) {
        size_t tmp_nrows = this->nrows;
        size_t tmp_ncols = this->ncols;
        T *tmp_values = this->values;
        this->nrows = A.nrows;
        this->ncols = A.ncols;
        this->values = A.values;
        A.nrows = tmp_nrows;
        A.ncols = tmp_ncols;
        A.values = tmp_values;
    }

    // Returns the sum along the diagonal in the matrix.
    template <class T>
    T matrix<T>::trace(long offset) const {
        T out = T(0);
        size_t i = (offset >= 0) ? 0 : -offset;
        for (; i < this->rows() && i + offset < this->columns(); ++i) {
            out += this->at(i, i + offset);
        }
        return out;
    }

    // Returns a copy of the matrix transposed.
    template <class T>
    matrix<T> matrix<T>::transpose() const {
        matrix<T> out(this->columns(), this->rows());
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                out.at(j, i) = this->at(i, j);
            }
        }
        return out;
    }

    // Returns the variance of the matrix elements.
    template <class T>
    T matrix<T>::var(size_t ddof) const {
        T matrix_mean = this->mean();
        T out = T(0);
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->columns(); ++j) {
                T deviation = this->at(i, j) - matrix_mean;
                out += deviation*deviation;
            }
        }
        out /= (this->rows() * this->columns() - ddof);
        return out;
    }

    // Returns the variance of the matrix elements along a given axis.
    template <class T>
    array<T> matrix<T>::var(size_t ddof, size_t axis) const {
        if (axis == 0) {
            array<T> matrix_mean = this->mean(axis);
            array<T> out(this->columns(), T(0));
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    T deviation = this->at(i, j) - matrix_mean[j];
                    out[j] += deviation * deviation;
                }
            }
            out /= (this->rows() - ddof);
            return out;
        }
        else if (axis == 1) {
            array<T> matrix_mean = this->mean(axis);
            array<T> out(this->rows(), T(0));
            for (size_t i = 0; i < this->rows(); ++i) {
                for (size_t j = 0; j < this->columns(); ++j) {
                    T deviation = this->at(i, j) - matrix_mean[i];
                    out[i] += deviation * deviation;
                }
            }
            out /= (this->columns() - ddof);
            return out;
        }
        else {
            throw std::invalid_argument("axis must be either 0 or 1");
        }
    }
}
