#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <cstddef>
#include <initializer_list>
#include <utility>

// Matrices are fixed-size two dimensional sequence containers: they hold a
// specific number of elements arranged in rows and columns.

namespace numcpp {
    template <class T>
    class matrix {
    protected:
        size_t nrows, ncols;
        T *values;
        template <class U> friend class matrix;

    public:
        typedef size_t size_type;
        typedef T value_type;

        // Default constructor. Constructs an empty matrix with no elements.
        matrix();

        // Size constructor. Constructs a matrix with m rows and n columns.
        matrix(size_t m, size_t n);

        // Fill constructor. Constructs a matrix with m rows and n columns,
        // each element initialized to val.
        matrix(size_t m, size_t n, const T &val);

        // Container constructor. Constructs a matrix with m rows and n
        // columns, each element initialized to its corresponding element in
        // the container, in the same order.
        template <class InputIterator>
        matrix(size_t m, size_t n, InputIterator first);

        // Copy constructor. Constructs a matrix with a copy of each of the
        // elements in A, in the same order.
        matrix(const matrix &A);

        template <class U>
        matrix(const matrix<U> &A);

        // Move constructor. Constructs a matrix that acquires the elements of
        // A.
        matrix(matrix &&A);

        // Submatrix constructor. Constructs a matrix with a copy of each of the
        // elements in A, in the same order.
        matrix(const submatrix<T> &A);

        // Initializer list of lists. Constructs a matrix with a copy of each
        // of the elements in ill, in the same order.
        matrix(std::initializer_list< std::initializer_list<T> > ill);

        // Destructor. Destroys the matrix.
        ~matrix();

        // Copy assignment. Assigns the contents of A to *this after resizing
        // the object (if necessary).
        matrix& operator= (const matrix &A);

        template <class U>
        matrix& operator= (const matrix<U> &A);

        // Move assignment. Acquires the contents of A.
        matrix& operator= (matrix &&A);

        // Submatrix assignment. Assigns the contents of A to *this after 
        // resizing the object (if necessary).
        matrix& operator= (const submatrix<T> &A);

        // Fill assignment. Assigns val to every element.
        matrix& operator= (const T &val);

        // Compound assignments.
        matrix& operator+= (const matrix &A);

        matrix& operator-= (const matrix &A);

        matrix& operator*= (const matrix &A);

        matrix& operator/= (const matrix &A);

        matrix& operator%= (const matrix &A);

        matrix& operator&= (const matrix &A);

        matrix& operator|= (const matrix &A);

        matrix& operator^= (const matrix &A);

        matrix& operator<<= (const matrix &A);

        matrix& operator>>= (const matrix &A);

        matrix& operator+= (const T &val);

        matrix& operator-= (const T &val);

        matrix& operator*= (const T &val);

        matrix& operator/= (const T &val);

        matrix& operator%= (const T &val);

        matrix& operator&= (const T &val);

        matrix& operator|= (const T &val);

        matrix& operator^= (const T &val);

        matrix& operator<<= (const T &val);

        matrix& operator>>= (const T &val);

        // Array subscript. Returns a pointer to the beginning of the i-th row.
        T* operator[] (size_t i);

        const T* operator[] (size_t i) const;

        // Returns a reference to the element at row i and column j in the the
        // matrix.
        T& at(size_t i, size_t j);

        const T& at(size_t i, size_t j) const;

        // Slice/Integer array indexing: Return a sub-matrix object that selects 
        // the elements specified by its argument.
        slice_slice_submatrix<T> at(slice i, slice j);

        const slice_slice_submatrix<T> at(slice i, slice j) const;

        slice_index_submatrix<T> at(slice i, const array<size_t> &j);

        const slice_index_submatrix<T> 
        at(slice i, const array<size_t> &j) const;

        index_slice_submatrix<T> at(const array<size_t> &i, slice j);

        const index_slice_submatrix<T> 
        at(const array<size_t> &i, slice j) const;

        index_index_submatrix<T> 
        at(const array<size_t> &i, const array<size_t> &j);

        const index_index_submatrix<T> 
        at(const array<size_t> &i, const array<size_t> &j) const;

        // Row/Column indexing: Return a sub-array object that selects the 
        // elements specified by its argument.
        slice_subarray<T> at(size_t i, slice j);

        const slice_subarray<T> at(size_t i, slice j) const;

        slice_subarray<T> at(slice i, size_t j);

        const slice_subarray<T> at(slice i, size_t j) const;

        index_subarray<T> at(size_t i, const array<size_t> &j);

        const index_subarray<T> at(size_t i, const array<size_t> &j) const;

        index_subarray<T> at(const array<size_t> &i, size_t j);

        const index_subarray<T> at(const array<size_t> &i, size_t j) const;

        // Coordinate array indexing: Return a sub-array object that selects the
        // elements specified by its argument.
        index_subarray<T> at(const array< std::pair<size_t, size_t> > &index);

        const index_subarray<T>
        at(const array< std::pair<size_t, size_t> > &index) const;

        // Apply a function to each of the elements in *this.
        template <class Function = T(T)>
        void apply(Function f);

        // Return the index of the maximum value.
        std::pair<size_t, size_t> argmax() const;

        array<size_t> argmax(size_t axis) const;

        // Return the index of the minimum value.
        std::pair<size_t, size_t> argmin() const;

        array<size_t> argmin(size_t axis) const;

        // Clip (limit) the values in the matrix. Given an interval, values
        // outside the interval are clipped to the interval edges.
        void clip(const T &a_min, const T &a_max);

        // Returns the number of columns in the matrix.
        size_t columns() const;

        // Returns a pointer to the memory array used internally by the matrix.
        T* data();

        const T* data() const;

        // Returns the matrix multiplication of a matrix and a column vector.
        array<T> dot(const array<T> &v) const;

        // Returns the matrix multiplication of two matrices.
        matrix dot(const matrix &A) const;

        // Returns a copy of the matrix collapsed into an array.
        array<T> flatten() const;

        // Returns the maximum value contained in the matrix.
        T max() const;

        array<T> max(size_t axis) const;

        // Returns the average of the matrix elements.
        T mean() const;

        array<T> mean(size_t axis) const;

        // Returns the minimum value contained in the matrix.
        T min() const;

        array<T> min(size_t axis) const;

        // Return the product of the matrix elements.
        T prod() const;

        array<T> prod(size_t axis) const;

        // Resizes the matrix, changing its size to m rows and n columns.
        // If the new size is smaller than the current size, the content is
        // reduced, removing the last elements.
        // If the new size is greater than the current size, the content is
        // expanded by inserting at the end as many elements as needed.
        // In any case, the contents will be rearranged to fit the new size.
        void resize(size_t m, size_t n, const T &val = T());

        // Returns the number of rows in the matrix.
        size_t rows() const;

        // Returns the standard deviation of the matrix elements.
        T stddev(size_t ddof = 0) const;

        array<T> stddev(size_t ddof, size_t axis) const;

        // Return the sum of the matrix elements.
        T sum() const;

        array<T> sum(size_t axis) const;

        // Swap contents with A.
        void swap(matrix &A);

        // Returns the sum along the diagonal in the matrix.
        T trace(long offset = 0) const;

        // Returns a copy of the matrix transposed.
        matrix<T> transpose() const;

        // Returns the variance of the matrix elements.
        T var(size_t ddof = 0) const;

        array<T> var(size_t ddof, size_t axis) const;
    };

    // Unary operators.
    template <class T>
    matrix<T> operator+ (const matrix<T> &A);

    template <class T>
    matrix<T> operator- (const matrix<T> &A);

    template <class T>
    matrix<T> operator~ (const matrix<T> &A);

    matrix<bool> operator! (const matrix<bool> &A);

    // Arithmetic operators.
    template <class T>
    matrix<T> operator+ (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator+ (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator+ (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator- (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator- (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator- (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator* (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator* (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator* (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator/ (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator/ (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator/ (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator% (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator% (const matrix<T> &A, const T &val) ;

    template <class T>
    matrix<T> operator% (const T &val, const matrix<T> &A) ;

    // Bitwise operators.
    template <class T>
    matrix<T> operator& (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator& (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator& (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator| (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator| (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator| (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator^ (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator^ (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator^ (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator<< (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator<< (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator<< (const T &val, const matrix<T> &A);

    template <class T>
    matrix<T> operator>> (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<T> operator>> (const matrix<T> &A, const T &val);

    template <class T>
    matrix<T> operator>> (const T &val, const matrix<T> &A);

    // Logical operators.
    matrix<bool> operator&& (const matrix<bool> &A, const matrix<bool> &B);

    matrix<bool> operator&& (const matrix<bool> &A, bool val);

    matrix<bool> operator&& (bool val, const matrix<bool> &A);

    matrix<bool> operator|| (const matrix<bool> &A, const matrix<bool> &B);

    matrix<bool> operator|| (const matrix<bool> &A, bool val);

    matrix<bool> operator|| (bool val, const matrix<bool> &A);

    // Relational operators.
    template <class T>
    matrix<bool> operator== (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<bool> operator== (const matrix<T> &A, const T &val);

    template <class T>
    matrix<bool> operator== (const T &val, const matrix<T> &A);

    template <class T>
    matrix<bool> operator!= (const matrix<T> &A, const matrix<T> &B) ;

    template <class T>
    matrix<bool> operator!= (const matrix<T> &A, const T &val);

    template <class T>
    matrix<bool> operator!= (const T &val, const matrix<T> &A);

    template <class T>
    matrix<bool> operator< (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<bool> operator< (const matrix<T> &A, const T &val);

    template <class T>
    matrix<bool> operator< (const T &val, const matrix<T> &A);

    template <class T>
    matrix<bool> operator> (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<bool> operator> (const matrix<T> &A, const T &val);

    template <class T>
    matrix<bool> operator> (const T &val, const matrix<T> &A);

    template <class T>
    matrix<bool> operator<= (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<bool> operator<= (const matrix<T> &A, const T &val);

    template <class T>
    matrix<bool> operator<= (const T &val, const matrix<T> &A);

    template <class T>
    matrix<bool> operator>= (const matrix<T> &A, const matrix<T> &B);

    template <class T>
    matrix<bool> operator>= (const matrix<T> &A, const T &val);

    template <class T>
    matrix<bool> operator>= (const T &val, const matrix<T> &A);
}

#endif // MATRIX_H_INCLUDED
