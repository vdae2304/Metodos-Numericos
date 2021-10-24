#ifndef SUBMATRIX_H_INCLUDED
#define SUBMATRIX_H_INCLUDED

#include <cstddef>

// Submatrices are intermediate types returned by matrix method at. It
// references the elements in the matrix object that are selected by the
// subscript, and overloads the assignment and compound assignment operators,
// allowing direct access to the elements in the selection. The type is
// convertible to a matrix, producing a new object with copies of the referred
// elements.

namespace numcpp {
    template <class T>
    class submatrix {
    protected:
        matrix<T> *values;
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

        // Submatrix subscript. Returns a reference to the element at row i
        // and column j in the the submatrix.
        virtual T& at(size_t i, size_t j);

        virtual const T& at(size_t i, size_t j) const;

        // Methods.
        virtual size_t columns() const;

        virtual size_t rows() const;
    };

    template <class T>
    class slice_slice_submatrix : public submatrix<T> {
    protected:
        slice i, j;
        friend class matrix<T>;
    
    public:
        // Assignment operators.
        void operator= (const matrix<T> &A);
        void operator= (const T &val);

        // Submatrix subscript. Returns a reference to the element at row i
        // and column j in the the submatrix.
        T& at(size_t i, size_t j);

        const T& at(size_t i, size_t j) const;

        // Methods.
        size_t columns() const;

        size_t rows() const;
    };

    template <class T>
    class slice_index_submatrix : public submatrix<T> {
    protected:
        slice i;
        array<size_t> j;
        friend class matrix<T>;
    
    public:
        // Assignment operators.
        void operator= (const matrix<T> &A);
        void operator= (const T &val);

        // Submatrix subscript. Returns a reference to the element at row i
        // and column j in the the submatrix.
        T& at(size_t i, size_t j);

        const T& at(size_t i, size_t j) const;

        // Methods.
        size_t columns() const;

        size_t rows() const;
    };

    template <class T>
    class index_slice_submatrix : public submatrix<T> {
    protected:
        array<size_t> i;
        slice j;
        friend class matrix<T>;
    
    public:
        // Assignment operators.
        void operator= (const matrix<T> &A);
        void operator= (const T &val);

        // Submatrix subscript. Returns a reference to the element at row i
        // and column j in the the submatrix.
        T& at(size_t i, size_t j);

        const T& at(size_t i, size_t j) const;

        // Methods.
        size_t columns() const;

        size_t rows() const;
    };

    template <class T>
    class index_index_submatrix : public submatrix<T> {
    protected:
        array<size_t> i, j;
        friend class matrix<T>;
    
    public:
        // Assignment operators.
        void operator= (const matrix<T> &A);
        void operator= (const T &val);

        // Submatrix subscript. Returns a reference to the element at row i
        // and column j in the the submatrix.
        T& at(size_t i, size_t j);

        const T& at(size_t i, size_t j) const;

        // Methods.
        size_t columns() const;

        size_t rows() const;
    };
}

#endif // SUBMATRIX_H_INCLUDED
