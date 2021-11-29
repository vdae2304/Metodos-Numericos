#ifndef ROUTINES_H_INCLUDED
#define ROUTINES_H_INCLUDED

// This header contains several routines.

namespace numcpp {
    // Returns true if all elements evaluate to true.
    bool all(const array<bool> &v);

    // Returns true if two arrays are element-wise equal within a tolerance.
    template <class T>
    bool allclose(
        const array<T> &v, const array<T> &w,
        const T &atol = 1e-8, const T &rtol = 1e-5
    );

    // Returns true if two matrices are element-wise equal within a tolerance.
    template <class T>
    bool allclose(
        const matrix<T> &A, const matrix<T> &B,
        const T &atol = 1e-8, const T &rtol = 1e-5
    );

    // Returns true if any of the elements evaluate to true.
    bool any(const array<bool> &v);

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding element in v.
    template <class T, class Function = T(T)>
    array<T> apply(Function f, const array<T> &v);

    // Returns an array with each of its elements initialized to the result of
    // applying f to the corresponding elements in v and w.
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
    // applying f to the corresponding elements in A and B.
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

    // Return the index of the maximum value in the array.
    template <class T>
    size_t argmax(const array<T> &v);

    // Return the index of the maximum value in the matrix.
    template <class T>
    std::pair<size_t, size_t> argmax(const matrix<T> &A);

    template <class T>
    array<size_t> argmax(const matrix<T> &A, size_t axis);

    // Return the index of the minimum value in the array.
    template <class T>
    size_t argmin(const array<T> &v);

    // Return the index of the minimum value in the matrix.
    template <class T>
    std::pair<size_t, size_t> argmin(const matrix<T> &A);

    template <class T>
    array<size_t> argmin(const matrix<T> &A, size_t axis);

    // Returns the indices that would sort the array.
    template <class T>
    array<size_t> argsort(const array<T> &v);

    // Return an array whose values are limited to [a_min, a_max].
    // Given an interval, values outside the interval are clipped to the
    // interval edges.
    template <class T>
    array<T> clip(const array<T> &v, const T &a_min, const T &a_max);

    // Return a matrix whose values are limited to [a_min, a_max].
    // Given an interval, values outside the interval are clipped to the
    // interval edges.
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

    // Returns the Pearson's correlation coefficient.
    template <class T>
    T corrcoef(const array<T> &x, const array<T> &y);

    // Returns the correlation matrix.
    template <class T>
    matrix<T> corrcoef(const matrix<T> &X, bool rowvar = true);

    // Returns the covariance of two arrays.
    template <class T>
    T cov(const array<T> &x, const array<T> &y, size_t ddof = 0);

    // Returns the covariance matrix.
    template <class T>
    matrix<T> cov(const matrix<T> &X, bool rowvar = true, size_t ddof = 0);

    // Return the cumulative product of the array elements.
    template <class T>
    array<T> cumprod(const array<T> &v);

    // Return the cumulative sum of the array elements.
    template <class T>
    array<T> cumsum(const array<T> &v);

    // Extract a diagonal from a matrix.
    template <class T>
    array<T> diagonal(const matrix<T> &A, int offset = 0);

    // Construct a diagonal matrix.
    template <class T>
    matrix<T> diagonal(const array<T> &v, int offset = 0);

    // Return the dot product of two arrays.
    template <class T>
    T dot(const array<T> &v, const array<T> &w);

    // Return the matrix multiplication of a row vector and a matrix.
    template <class T>
    array<T> dot(const array<T> &v, const matrix<T> &A);

    // Return the matrix multiplication of a matrix and a column vector.
    template <class T>
    array<T> dot(const matrix<T> &A, const array<T> &v);

    // Return the matrix multiplication of two matrices.
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
        const T &start, const T &stop, size_t num = 50, bool endpoint = true
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

    // Returns true if two values are equal within a tolerance.
    template <class T>
    bool isclose(
        const T &a, const T &b, const T &atol = 1e-8, const T &rtol = 1e-5
    );

    // Return evenly spaced numbers over a specified interval. Returns num
    // evenly spaced samples, calculated over the interval [start, stop].
    // The endpoint of the interval can optionally be excluded.
    template <class T>
    array<T> linspace(
        const T &start, const T &stop, size_t num = 50, bool endpoint = true
    );

    // Return numbers spaced evenly on a log scale. In linear space, the
    // sequence starts at pow(base, start) (base to the power of start) and
    // ends with pow(base, stop).
    template <class T>
    array<T> logspace(
        const T &start, const T &stop, size_t num = 50, bool endpoint = true,
        const T &base = 10.0
    );

    // Returns the maximum value contained in the array.
    template <class T>
    T max(const array<T> &v);

    // Returns the maximum value contained in the matrix.
    template <class T>
    T max(const matrix<T> &A);

    template <class T>
    array<T> max(const matrix<T> &A, size_t axis);

    // Returns the average of the array elements.
    template <class T>
    T mean(const array<T> &v);

    // Returns the average of the matrix elements.
    template <class T>
    T mean(const matrix<T> &A);

    template <class T>
    array<T> mean(const matrix<T> &A, size_t axis);

    // Returns the minimum value contained in the array.
    template <class T>
    T min(const array<T> &v);

    // Returns the minimum value contained in the matrix.
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

    // Return the product of the matrix elements.
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

    // Return a sorted copy of an array.
    template <class T>
    array<T> sort(const array<T> &v);

    // Return the standard deviation of the array elements.
    template <class T>
    T stddev(const array<T> &v, size_t ddof = 0);

    // Return the standard deviation of the matrix elements.
    template <class T>
    T stddev(const matrix<T> &A, size_t ddof = 0);

    template <class T>
    array<T> stddev(const matrix<T> &A, size_t ddof, size_t axis);

    // Return the sum of the array elements.
    template <class T>
    T sum(const array<T> &v);

    // Return the sum of the matrix elements.
    template <class T>
    T sum(const matrix<T> &A);

    template <class T>
    array<T> sum(const matrix<T> &A, size_t axis);

    // Swap contents of two arrays.
    template <class T>
    void swap(array<T> &v, array<T> &w);

    // Swap contents of two matrices.
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

    // Returns the variance of the matrix elements.
    template <class T>
    T var(const matrix<T> &A, size_t ddof = 0);

    template <class T>
    array<T> var(const matrix<T> &A, size_t ddof, size_t axis);

    // Return the indices of the elements that evaluate to true.
    array<size_t> where(const array<bool> &condition);

    // Return elements chosen from two arrays depending on condition.
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

#endif // ROUTINES_H_INCLUDED
