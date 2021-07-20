#ifndef SUBMATRIX_H_INCLUDED
#define SUBMATRIX_H_INCLUDED

#include <cstddef>

// Sub-matrices are intermediate types returned by matrix method at. It
// references the elements in the matrix object that are selected by the
// subscript, and overloads the assignment and compound assignment operators,
// allowing direct access to the elements in the selection. The type is
// convertible to a matrix, producing a new object with copies of the referred
// elements.

namespace numcpp {
    template <class T>
    class submatrix {
    protected:
        matrix<T> *parent;
        slice i, j;

        friend class matrix<T>;

    public:
        // Assignment operators.
        void operator= (const matrix<T> &A);

        void operator= (const T &val);

        // Compound assignment.
        void operator+= (const matrix<T> &A);

        void operator-= (const matrix<T> &A);

        void operator*= (const matrix<T> &A);

        void operator/= (const matrix<T> &A);

        void operator%= (const matrix<T> &A);

        void operator&= (const matrix<T> &A);

        void operator|= (const matrix<T> &A);

        void operator^= (const matrix<T> &A);

        void operator<<= (const matrix<T> &A);

        void operator>>= (const matrix<T> &A);

        void operator+= (const T &val);

        void operator-= (const T &val);

        void operator*= (const T &val);

        void operator/= (const T &val);

        void operator%= (const T &val);

        void operator&= (const T &val);

        void operator|= (const T &val);

        void operator^= (const T &val);

        void operator<<= (const T &val);

        void operator>>= (const T &val);

        // Matrix subscript. Returns a reference to the element at row i
        // and column j in the the sub-matrix.
        T& at(size_t i, size_t j);

        const T& at(size_t i, size_t j) const;

        // Methods.
        size_t columns() const;

        matrix<T> copy() const;

        size_t rows() const;
    };
}

#endif // SUBMATRIX_H_INCLUDED
