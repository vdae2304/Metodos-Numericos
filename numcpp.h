#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace numcpp {

    // This class represents an array slice selector. It does not contain nor
    // refers to any element - it only describes a selection of elements to be
    // used as an index in array::operator[].
    class slice {
        public:
            size_t start, stop, step;

            slice() {
            }

            slice(size_t stop) {
                this->start = 0;
                this->stop = stop;
                this->step = 1;
            }

            slice(size_t start, size_t stop, size_t step = 1) {
                this->start = start;
                this->stop = stop;
                this->step = step;
            }
    };

    template <class T> class subarray;
    template <class T> class matrix;

    // Arrays are fixed-size sequence containers: they hold a specific number
    // of elements ordered in a strict linear sequence.
    template <class T>
    class array {
        protected:
            size_t length;
            T *values;

        public:
            typedef size_t size_type;
            typedef T value_type;

            // Default constructor. Constructs an empty array with no elements.
            array();

            // Size constructor. Constructs an array with n elements.
            array(size_t n);

            // Fill constructor. Constructs an array with n elements, each
            // initialized to val.
            array(size_t n, const T &val);

            // Range constructor. Constructs an array with as many elements as
            // the range [first, last), with each element constructed from its
            // corresponding element in that range, in the same order.
            template <class InputIterator>
            array(InputIterator first, InputIterator last);

            // Copy constructor. Constructs an array with a copy of each of the
            // elements in v, in the same order.
            array(const array &v);

            // Move constructor. Constructs an array that acquires the elements
            // of v.
            array(array &&v);

            // Initializer list. Constructs an array with a copy of each of the
            // elements in il, in the same order.
            array(std::initializer_list<T> il);

            // Destructor. Destroys the array.
            ~array();

            // Copy assignment. Assigns the contents of v to *this after
            // resizing the object (if necessary).
            array& operator= (const array &v);

            // Move assignment. Acquires the contents of v.
            array& operator= (array &&v);

            // Fill assignment. Assigns val to every element.
            array& operator= (const T &val);

            // Compound assignments.
            array& operator+= (const array &v);

            array& operator-= (const array &v);

            array& operator*= (const array &v);

            array& operator/= (const array &v);

            array& operator%= (const array &v);

            array& operator&= (const array &v);

            array& operator|= (const array &v);

            array& operator^= (const array &v);

            array& operator<<= (const array &v);

            array& operator>>= (const array &v);

            array& operator+= (const T &val);

            array& operator-= (const T &val);

            array& operator*= (const T &val);

            array& operator/= (const T &val);

            array& operator%= (const T &val);

            array& operator&= (const T &val);

            array& operator|= (const T &val);

            array& operator^= (const T &val);

            array& operator<<= (const T &val);

            array& operator>>= (const T &val);

            // Array subscript. Returns a reference to the element at position
            // i in the array.
            T& operator[] (size_t i);

            const T& operator[] (size_t i) const;

            // Slice indexing: Return a sub-array object that selects the
            // elements specified by its argument.
            subarray<T> operator[] (slice slc);

            const subarray<T> operator[] (slice slc) const;

            // Integer array indexing: Return a sub-array object that selects
            // the elements specified by its argument.
            subarray<T> operator[] (const array<size_t> &indices);

            const subarray<T> operator[] (const array<size_t> &indices) const;

            // Boolean array indexing: Return a sub-array object that selects
            // the elements specified by its argument.
            subarray<T> operator[] (const array<bool> &mask);

            const subarray<T> operator[] (const array<bool> &mask) const;

            // Apply a function to each of the elements in *this.
            template <class Function = T(T)>
            void apply(Function f);

            // Return the index of the maximum value.
            size_t argmax() const;

            // Return the index of the minimum value.
            size_t argmin() const;

            // Returns the indices that would sort this array.
            array<size_t> argsort() const;

            // Copy of the array, cast to a specified type.
            template <class U> array<U> astype() const;

            // Clip (limit) the values in the array. Given an interval, values
            // outside the interval are clipped to the interval edges.
            void clip(const T &a_min, const T &a_max);

            // Return the cumulative product of the elements.
            array cumprod() const;

            // Return the cumulative sum of the elements.
            array cumsum() const;

            // Returns a pointer to the memory array used internally by the
            // array.
            T* data();

            const T* data() const;

            // Return the dot product between two arrays or between a row
            // vector and a matrix.
            T dot(const array &v) const;

            array dot(const matrix<T> &A) const;

            // Returns the maximum value contained in the array.
            T max() const;

            // Returns the average of the array elements.
            T mean() const;

            // Returns the minimum value contained in the array.
            T min() const;

            // Return the product of the array elements.
            T prod() const;

            // Resizes the array, changing its size to n elements.
            // If n is smaller than the current size, the content is reduced to
            // its first n elements, removing those beyond.
            // If n is greater than the current size, the content is expanded
            // by inserting at the end as many elements as needed to reach a
            // size of n.
            void resize(size_t n, const T &val = T());

            // Returns the number of elements in the array.
            size_t size() const;

            // Sort an array in-place.
            void sort();

            // Returns the standard deviation of the array elements.
            T stddev(size_t ddof = 0) const;

            // Return the sum of the array elements.
            T sum() const;

            // Swap contents with v.
            void swap(array &v);

            // Returns the variance of the array elements.
            T var(size_t ddof = 0) const;
    };

    // This class is used as an intermediate type returned by array's subscript
    // operator (operator[]). It references the elements in the array object
    // that are selected by the subscript, and overloads the assignment and
    // compound assignment operators, allowing direct access to the elements in
    // the selection. The type is convertible to an array, producing a new
    // object with copies of the referred elements.
    template <class T>
    class subarray {
        protected:
            array<T> *parent;
            array<size_t> indices;

            friend class array<T>;

        public:
            // Assignment operators.
            void operator= (const array<T> &v);

            void operator= (const T &val);

            // Compound assignment.
            void operator+= (const array<T> &v);

            void operator-= (const array<T> &v);

            void operator*= (const array<T> &v);

            void operator/= (const array<T> &v);

            void operator%= (const array<T> &v);

            void operator&= (const array<T> &v);

            void operator|= (const array<T> &v);

            void operator^= (const array<T> &v);

            void operator<<= (const array<T> &v);

            void operator>>= (const array<T> &v);

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

            // Array subscript. Returns a reference to the element at position
            // i in the sub-array.
            T& operator[] (size_t i);

            const T& operator[] (size_t i) const;

            // Methods.
            array<T> copy() const;

            size_t size() const;
    };

    template <class T> class submatrix;

    // Matrices are fixed-size two dimensional sequence containers: they hold a
    // specific number of elements arranged in rows and columns.
    template <class T>
    class matrix {
        protected:
            size_t nrows, ncols;
            T *values;

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
            // columns, each element initialized to its corresponding element
            // in the container, in the same order.
            template <class InputIterator>
            matrix(size_t m, size_t n, InputIterator first);

            // Copy constructor. Constructs a matrix with a copy of each of the
            // elements in A, in the same order.
            matrix(const matrix &A);

            // Move constructor. Constructs a matrix that acquires the elements
            // of A.
            matrix(matrix &&A);

            // Initializer list of lists. Constructs a matrix with a copy of
            // each of the elements in ill, in the same order.
            matrix(std::initializer_list< std::initializer_list<T> > ill);

            // Destructor. Destroys the matrix.
            ~matrix();

            // Copy assignment. Assigns the contents of A to *this after
            // resizing the object (if necessary).
            matrix& operator= (const matrix &A);

            // Move assignment. Acquires the contents of A.
            matrix& operator= (matrix &&A);

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

            // Array subscript. Returns a pointer to the beginning of the i-th
            // row.
            T* operator[] (size_t i);

            const T* operator[] (size_t i) const;

            // Returns a reference to the element at row i and column j in the
            // the matrix.
            T& at(size_t i, size_t j);

            const T& at(size_t i, size_t j) const;

            // Index-slice indexing: Return a sub-matrix object that selects
            // the elements specified by its arguments.
            submatrix<T> at(size_t i, slice j);

            const submatrix<T> at(size_t i, slice j) const;

            // Index-integer array indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(size_t i, const array<size_t> &j);

            const submatrix<T> at(size_t i, const array<size_t> &j) const;

            // Index-boolean array indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(size_t i, const array<bool> &j);

            const submatrix<T> at(size_t i, const array<bool> &j) const;

            // Slice-index indexing: Return a sub-matrix object that selects
            // the elements specified by its arguments.
            submatrix<T> at(slice i, size_t j);

            const submatrix<T> at(slice i, size_t j) const;

            // Integer array-index indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(const array<size_t> &i, size_t j);

            const submatrix<T> at(const array<size_t> &i, size_t j) const;

            // Boolean array-index indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(const array<bool> &i, size_t j);

            const submatrix<T> at(const array<bool> &i, size_t j) const;

            // Slice-slice indexing: Return a sub-matrix object that selects
            // the elements specified by its arguments.
            submatrix<T> at(slice i, slice j);

            const submatrix<T> at(slice i, slice j) const;

            // Slice-integer array indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(slice i, const array<size_t> &j);

            const submatrix<T> at(slice i, const array<size_t> &j) const;

            // Slice-boolean array indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(slice i, const array<bool> &j);

            const submatrix<T> at(slice i, const array<bool> &j) const;

            // Integer array-slice indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(const array<size_t> &i, slice j);

            const submatrix<T> at(const array<size_t> &i, slice j) const;

            // Integer array-integer array indexing: Return a sub-matrix object
            // that selects the elements specified by its arguments.
            submatrix<T> at(const array<size_t> &i, const array<size_t> &j);

            const submatrix<T>
            at(const array<size_t> &i, const array<size_t> &j) const;

            // Integer array-boolean array indexing: Return a sub-matrix object
            // that selects the elements specified by its arguments.
            submatrix<T> at(const array<size_t> &i, const array<bool> &j);

            const submatrix<T>
            at(const array<size_t> &i, const array<bool> &j) const;

            // Boolean array-slice indexing: Return a sub-matrix object that
            // selects the elements specified by its arguments.
            submatrix<T> at(const array<bool> &i, slice j);

            const submatrix<T> at(const array<bool> &i, slice j) const;

            // Boolean array-integer array indexing: Return a sub-matrix object
            // that selects the elements specified by its arguments.
            submatrix<T> at(const array<bool> &i, const array<size_t> &j);

            const submatrix<T>
            at(const array<bool> &i, const array<size_t> &j) const;

            // Boolean array-boolean array indexing: Return a sub-matrix object
            // that selects the elements specified by its arguments.
            submatrix<T> at(const array<bool> &i, const array<bool> &j);

            const submatrix<T>
            at(const array<bool> &i, const array<bool> &j) const;

            // Apply a function to each of the elements in *this.
            template <class Function = T(T)>
            void apply(Function f);

            // Return the index of the maximum value.
            std::pair<size_t, size_t> argmax() const;

            array<size_t> argmax(size_t axis) const;

            // Return the index of the minimum value.
            std::pair<size_t, size_t> argmin() const;

            array<size_t> argmin(size_t axis) const;

            // Copy of the matrix, cast to a specified type.
            template <class U> matrix<U> astype() const;

            // Clip (limit) the values in the matrix. Given an interval, values
            // outside the interval are clipped to the interval edges.
            void clip(const T &a_min, const T &a_max);

            // Returns the number of columns in the matrix.
            size_t columns() const;

            // Returns a pointer to the memory array used internally by the
            // matrix.
            T* data();

            const T* data() const;

            // Returns the matrix multiplication between a matrix and a column
            // vector or between two matrices.
            array<T> dot(const array<T> &v) const;

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
            T trace(int offset = 0) const;

            // Returns a copy of the matrix transposed.
            matrix<T> transpose() const;

            // Returns the variance of the matrix elements.
            T var(size_t ddof = 0) const;

            array<T> var(size_t ddof, size_t axis) const;
    };

    // This class is used as an intermediate type returned by matrix's subscript
    // operator (operator[]). It references the elements in the matrix object
    // that are selected by the subscript, and overloads the assignment and
    // compound assignment operators, allowing direct access to the elements in
    // the selection. The type is convertible to a matrix, producing a new
    // object with copies of the referred elements.
    template <class T>
    class submatrix {
        protected:
            matrix<T> *parent;
            array<size_t> row_indices;
            array<size_t> col_indices;

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

    // Context manager for setting print options.
    namespace printoptions {
        // Number of digits of precision for floating point output (default 8).
        size_t precision = 8;

        // Total number of array elements which trigger summarization rather
        // than full representation (default 100).
        size_t threshold = 100;

        // Number of array items in summary at beginning and end of each
        // dimension (default 3).
        size_t edgeitems = 3;

        // Controls printing of the sign of numeric types. If true, always
        // print the sign of positive values. If false, omit the sign character
        // of positive values. (default false)
        bool sign = false;

        // Controls the interpretation of the precision option for
        // floating-point types. Can take the following values:
        // - "default":    write floating-point values in default
        //                 floating-point notation.
        // - "fixed":      write floating-point values in fixed-point notation.
        // - "scientific": write floating-point values in scientific notation.
        std::string floatmode = "default";
    };

    // Returns true if all elements evaluate to true.
    bool all(const array<bool> &v);

    // Returns true if any of the elements evaluate to true.
    bool any(const array<bool> &v);

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding element in v.
    template <class T, class Function = T(T)>
    array<T> apply(Function f, const array<T> &v);

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding element in v and w.
    template <class T, class Function = T(T, T)>
    array<T> apply(Function f, const array<T> &v, const array<T> &w);

    template <class T, class Function = T(T, T)>
    array<T> apply(Function f, const array<T> &v, const T &val);

    template <class T, class Function = T(T, T)>
    array<T> apply(Function f, const T &val, const array<T> &v);

    // Returns a matrix with each of its elements initialized to the result of
    // applying f to the corresponding element in A.
    template <class T, class Function = T(T)>
    matrix<T> apply(Function f, const matrix<T> &A);

    // Returns a matrix with each of its elements initialized to the result of
    // applying f to the corresponding element in A and B.
    template <class T, class Function = T(T, T)>
    matrix<T> apply(Function f, const matrix<T> &A, const matrix<T> &B);

    template <class T, class Function = T(T, T)>
    matrix<T> apply(Function f, const matrix<T> &A, const T &val);

    template <class T, class Function = T(T, T)>
    matrix<T> apply(Function f, const T &val, const matrix<T> &A);

    // Return evenly spaced values within a given interval. Values are
    // generated within the half-open interval [start, stop) (in other words,
    // the interval including start but excluding stop).
    template <class T>
    array<T> arange(const T &stop);

    template <class T>
    array<T> arange(const T &start, const T &stop, const T &step = T(1));

    // Return the index of the maximum value.
    template <class T>
    size_t argmax(const array<T> &v);

    template <class T>
    std::pair<size_t, size_t> argmax(const matrix<T> &A);

    template <class T>
    array<size_t> argmax(const matrix<T> &A, size_t axis);

    // Return the index of the minimum value.
    template <class T>
    size_t argmin(const array<T> &v);

    template <class T>
    std::pair<size_t, size_t> argmin(const matrix<T> &A);

    template <class T>
    array<size_t> argmin(const matrix<T> &A, size_t axis);

    // Returns the indices that would sort this array.
    template <class T>
    size_t argsort(const array<T> &v);

    // Return an array whose values are limited to [a_min, a_max].
    // Given an interval, values outside the interval are clipped to the
    // interval edges.
    template <class T>
    array<T> clip(const array<T> &v, const T &a_min, const T &a_max);

    template <class T>
    matrix<T> clip(const matrix<T> &A, const T &a_min, const T &a_max);

    // Concatenate arrays and matrices vertically.
    template <class T>
    matrix<T> column_stack(const array<T> &v, const array<T> &w);

    template <class T>
    matrix<T> column_stack(const matrix<T> &A, const array<T> &v);

    template <class T>
    matrix<T> column_stack(const array<T> &v, const matrix<T> &A);

    template <class T>
    matrix<T> column_stack(const matrix<T> &A, const matrix<T> &B);

    // Concatenate (join) two arrays.
    template <class T>
    array<T> concatenate(const array<T> &v, const array<T> &w);

    // Returns the correlation matrix.
    template <class T>
    T corrcoef(const array<T> &x, const array<T> &y);

    template <class T>
    matrix<T> corrcoef(const matrix<T> &X, bool rowvar = true);

    // Returns the covariance matrix.
    template <class T>
    T cov(const array<T> &x, const array<T> &y, size_t ddof = 0);

    template <class T>
    matrix<T> cov(const matrix<T> &X, bool rowvar = true, size_t ddof = 0);

    // Return the cumulative product of the elements.
    template <class T>
    array<T> cumprod(const array<T> &v);

    // Return the cumulative sum of the elements.
    template <class T>
    array<T> cumsum(const array<T> &v);

    // Extract a diagonal or construct a diagonal matrix.
    template <class T>
    array<T> diagonal(const matrix<T> &A, int offset = 0);

    template <class T>
    matrix<T> diagonal(const array<T> &v, int offset = 0);

    // Return the dot product of
    // (i)   two vectors,
    // (ii)  row vector and matrix,
    // (iii) matrix and column vector,
    // (iv)  two matrices.
    template <class T>
    T dot(const array<T> &v, const array<T> &w);

    template <class T>
    array<T> dot(const array<T> &v, const matrix<T> &A);

    template <class T>
    array<T> dot(const matrix<T> &A, const array<T> &v);

    template <class T>
    matrix<T> dot(const matrix<T> &A, const matrix<T> &B);

    // Return a new uninitialized array.
    template <class T = double>
    array<T> empty(size_t n);

    // Return a new uninitialized matrix.
    template <class T = double>
    matrix<T> empty(size_t m, size_t n);

    // Delete values from an array.
    template <class T>
    array<T> erase(const array<T> &v, size_t index);

    template <class T>
    array<T> erase(const array<T> &v, const array<size_t> indices);

    // Return a new array of given length filled with value.
    template <class T = double>
    array<T> full(size_t n, const T &val);

    // Return a new matrix of given shape filled with value.
    template <class T = double>
    matrix<T> full(size_t m, size_t n, const T &val);

    // Return numbers spaced evenly on a log scale (a geometric progression).
    // This is similar to logspace, but with endpoints specified directly. Each
    // output sample is a constant multiple of the previous.
    template <class T>
    array<T> geomspace(
        const T &start, const T &stop,
        size_t num = 50,
        bool endpoint = true
    );

    // Returns a matrix with ones on the diagonal and zeros elsewhere.
    template <class T = double>
    matrix<T> eye(size_t m, size_t n, int offset = 0);

    // Insert values before the given indices.
    template <class T>
    array<T> insert(const array<T> &v, size_t index, const T &value);

    template <class T>
    array<T> insert(
        const array<T> &v, const array<size_t> &indices, const array<T> &values
    );

    // Return evenly spaced numbers over a specified interval. Returns num
    // evenly spaced samples, calculated over the interval [start, stop].
    // The endpoint of the interval can optionally be excluded.
    template <class T>
    array<T> linspace(
        const T &start, const T &stop,
        size_t num = 50,
        bool endpoint = true
    );

    // Return a new array from a binary file.
    template <class T>
    array<T> load(const char *file);

    // Return a new matrix from a binary file.
    template <class T>
    matrix<T> load_matrix(const char *file);

    // Return a new matrix from a text file.
    template <class T>
    matrix<T> load_txt(
        const char *file,
        char delimiter = ' ',
        bool header = false
    );

    template <class T>
    matrix<T> load_txt(
        const char *file,
        char delimiter,
        bool header,
        array<std::string> &names
    );

    // Return numbers spaced evenly on a log scale. In linear space, the
    // sequence starts at pow(base, start) (base to the power of start) and
    // ends with pow(base, stop).
    template <class T>
    array<T> logspace(
        const T &start, const T &stop,
        size_t num = 50,
        bool endpoint = true,
        const T &base = 10.0
    );

    // Returns the maximum value contained in the array.
    template <class T>
    T max(const array<T> &v);

    template <class T>
    T max(const matrix<T> &A);

    template <class T>
    array<T> max(const matrix<T> &A, size_t axis);

    // Returns the average of the array elements.
    template <class T>
    T mean(const array<T> &v);

    template <class T>
    T mean(const matrix<T> &A);

    template <class T>
    array<T> mean(const matrix<T> &A, size_t axis);

    // Returns the minimum value contained in the array.
    template <class T>
    T min(const array<T> &v);

    template <class T>
    T min(const matrix<T> &A);

    template <class T>
    array<T> min(const matrix<T> &A, size_t axis);

    // Return a new array setting values to one.
    template <class T = double>
    array<T> ones(size_t n);

    // Return a new matrix setting values to one.
    template <class T = double>
    matrix<T> ones(size_t m, size_t n);

    // Return the product of the array elements.
    template <class T>
    T prod(const array<T> &v);

    template <class T>
    T prod(const matrix<T> &A);

    template <class T>
    array<T> prod(const matrix<T> &A, size_t axis);

    // Concatenate arrays and matrices horizontally.
    template <class T>
    matrix<T> row_stack(const array<T> &v, const array<T> &w);

    template <class T>
    matrix<T> row_stack(const matrix<T> &A, const array<T> &v);

    template <class T>
    matrix<T> row_stack(const array<T> &v, const matrix<T> &A);

    template <class T>
    matrix<T> row_stack(const matrix<T> &A, const matrix<T> &B);

    // Save an array to a binary file.
    template <class T>
    void save(const char *file, const array<T> &v);

    // Save a matrix to a binary file.
    template <class T>
    void save_matrix(const char *file, const matrix<T> &A);

    // Save a matrix to a text file.
    template <class T>
    void save_txt(const char *file, const matrix<T> &A, char delimiter = ' ');

    template <class T>
    void save_txt(
        const char *file,
        const matrix<T> &A,
        char delimiter,
        const array<std::string> &names
    );

    // Return a sorted copy of an array.
    template <class T>
    array<T> sort(const array<T> &v);

    // Returns the standard deviation of the array elements.
    template <class T>
    T stddev(const array<T> &v, size_t ddof = 0);

    template <class T>
    T stddev(const matrix<T> &A, size_t ddof = 0);

    template <class T>
    array<T> stddev(const matrix<T> &A, size_t ddof, size_t axis);

    // Return the sum of the array elements.
    template <class T>
    T sum(const array<T> &v);

    template <class T>
    T sum(const matrix<T> &A);

    template <class T>
    array<T> sum(const matrix<T> &A, size_t axis);

    // Swap contents between two arrays.
    template <class T>
    void swap(array<T> &v, array<T> &w);

    template <class T>
    void swap(matrix<T> &A, matrix<T> &B);

    // Returns the sum along the diagonal in the matrix.
    template <class T>
    T trace(const matrix<T> &A, int offset = 0);

    // Returns a copy of the matrix transposed.
    template <class T>
    matrix<T> transpose(const matrix<T> &A);

    // Returns the variance of the array elements.
    template <class T>
    T var(const array<T> &v, size_t ddof = 0);

    template <class T>
    T var(const matrix<T> &A, size_t ddof = 0);

    template <class T>
    array<T> var(const matrix<T> &A, size_t ddof, size_t axis);

    // Return the indices of the elements that evaluate to true.
    array<size_t> where(const array<bool> &condition);

    // Return elements chosen from v or w depending on condition.
    template <class T>
    array<T> where(const array<bool> &condition, const array<T> &expr_true);

    template <class T>
    array<T> where(
       const array<bool> &condition,
       const array<T> &expr_true, const array<T> &expr_false
    );

    // Return a new array setting values to zero.
    template <class T = double>
    array<T> zeros(size_t n);

    // Return a new matrix setting values to zero.
    template <class T = double>
    matrix<T> zeros(size_t m, size_t n);
}

#include "numcpp/array.h"
#include "numcpp/matrix.h"
#include "numcpp/math.h"

#endif // NUMCPP_H_INCLUDED