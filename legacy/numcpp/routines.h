/*
 * This file is part of the NumCpp project.
 *
 * NumCpp is a package for scientific computing in C++. It is a C++ library
 * that provides an array and a matrix object, and an assortment of routines
 * for fast operations on arrays and matrices, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * The NumCpp package is inspired by the NumPy package for Python, although it
 * is not related to it or any of its parts.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/routines.h
  *  This header defines an assortment of routines for array and matrix
  *  classes.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ROUTINES_H_INCLUDED
#define NUMCPP_ROUTINES_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/math/mathfwd.h"
#include "numcpp/routines/algo.h"

#include "numcpp/routines/array_sequence.h"
#include "numcpp/routines/diagonal_matrix.h"
#include "numcpp/routines/triangular_matrix.h"
#include "numcpp/routines/matrix_transpose.h"
#include "numcpp/routines/outer_matrix.h"

namespace numcpp {
    /// Array and matrix creation routines

    /**
     * @brief Return a new array of given size without initializing entries.
     *
     * @param n Length of array.
     *
     * @return An array of uninitialized data with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    array<T> empty(size_t n);

    /**
     * @brief Return a new matrix of given size without initializing entries.
     *
     * @param m Number of rows.
     * @param n Number of columns.
     *
     * @return A matrix of uninitialized data with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    matrix<T> empty(size_t m, size_t n);

    /**
     * @brief Return a new array with the same size and type as a given array.
     *
     * @param arr An array-like object.
     *
     * @return An array of uninitialized data with the same size and type as
     *     arr.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> empty_like(const base_array<T, Tag> &arr);

    /**
     * @brief Return a new matrix with the same size and type as a given
     * matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return A matrix of uninitialized data with the same size and type as
     *     mat.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> empty_like(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return a new array of given size filled with zeros.
     *
     * @param n Length of array.
     *
     * @return An array of zeros with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    array<T> zeros(size_t n);

    /**
     * @brief Return a new matrix of given size filled with zeros.
     *
     * @param m Number of rows.
     * @param n Number of columns.
     *
     * @return A matrix of zeros with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    matrix<T> zeros(size_t m, size_t n);

    /**
     * @brief Return a new array of zeros with the same size and type as a
     * given array.
     *
     * @param arr An array-like object.
     *
     * @return An array of zeros with the same size and type as arr.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> zeros_like(const base_array<T, Tag> &arr);

    /**
     * @brief Return a new matrix of zeros with the same size and type as a
     * given matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return A matrix of zeros with the same size and type as mat.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> zeros_like(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return a new array of given size filled with ones.
     *
     * @param n Length of array.
     *
     * @return An array of ones with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    array<T> ones(size_t n);

    /**
     * @brief Return a new matrix of given size filled with ones.
     *
     * @param m Number of rows.
     * @param n Number of columns.
     *
     * @return A matrix of ones with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    matrix<T> ones(size_t m, size_t n);

    /**
     * @brief Return a new array of ones with the same size and type as a given
     * array.
     *
     * @param arr An array-like object.
     *
     * @return An array of ones with the same size and type as arr.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> ones_like(const base_array<T, Tag> &arr);

    /**
     * @brief Return a new matrix of ones with the same size and type as a
     * given matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return A matrix of ones with the same size and type as mat.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> ones_like(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return a new array of given size filled with val.
     *
     * @param n Length of array.
     * @param val Fill value.
     *
     * @return An array of val with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    array<T> full(size_t n, const T &val);

    /**
     * @brief Return a new matrix of given size filled with val.
     *
     * @param m Number of rows.
     * @param n Number of columns.
     * @param val Fill value.
     *
     * @return A matrix of val with the given size.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T>
    matrix<T> full(size_t m, size_t n, const T &val);

    /**
     * @brief Return a new array of val with the same size and type as a given
     * array.
     *
     * @param arr An array-like object.
     * @param val Fill value.
     *
     * @return An array of val with the same size and type as arr.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> full_like(
        const base_array<T, Tag> &arr,
        const typename base_array<T, Tag>::value_type &val
    );

    /**
     * @brief Return a new matrix of val with the same size and type as a given
     * matrix.
     *
     * @param mat A matrix-like object.
     * @param val Fill value.
     *
     * @return A matrix of val with the same size and type as mat.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *      throw an exception.
     */
    template <class T, class Tag>
    matrix<T> full_like(
        const base_matrix<T, Tag> &mat,
        const typename base_matrix<T, Tag>::value_type &val
    );

    /// Numerical ranges

    /**
     * @brief Return evenly spaced values within a given interval. Values are
     * generated within the half-open interval [start, stop), i.e., the
     * interval including start but excluding stop.
     *
     * @param start Start of interval. Defaults to 0 if not provided.
     * @param stop End of interval.
     * @param step Spacing between values. This is the distance between two
     *     adjacent values. Defaults to 1 if not provided.
     *
     * @return A light-weight object with evenly spaced values. Convertible to
     *     an array object.
     */
    template <class T>
    base_array<T, sequence_tag> arange(const T &stop);

    template <class T>
    base_array<T, sequence_tag> arange(const T &start, const T &stop);

    template <class T>
    base_array<T, sequence_tag> arange(
        const T &start, const T &stop, const T &step
    );

    /**
     * @brief Return evenly spaced numbers over a specified interval. Return
     * num evenly spaced samples, calculated over the interval [start, stop].
     * The endpoint of the interval can optionally be excluded.
     *
     * @param start The starting value of the sequence.
     * @param stop The end value of the sequence.
     * @param num Number of samples to generate. Default is 50.
     * @param endpoint If true, stop is the last sample. Otherwise, it is not
     *     included. Default is true.
     *
     * @return A light-weight object with equally spaced samples. Convertible
     *     to an array object.
     */
    template <class T>
    base_array<T, sequence_tag> linspace(
        const T &start, const T &stop, size_t num = 50, bool endpoint = true
    );

    /**
     * @brief Return numbers spaced evenly on a log scale. In linear space, the
     * sequence starts at pow(base, start) (base to the power of start) and
     * ends with pow(base, stop).
     *
     * @param start pow(base, stop) is the starting value of the sequence.
     * @param stop pow(base, stop) is the final value of the sequence.
     * @param num Number of samples to generate. Default is 50.
     * @param endpoint If true, stop is the last sample. Otherwise, it is not
     *     included. Default is true.
     * @param base The base of the log space. Default is 10.
     *
     * @return A light-weight object with equally spaced samples on a log
     *     scale. Convertible to an array object.
     */
    template <class T>
    base_array<T, log_sequence_tag> logspace(
        const T &start, const T &stop, size_t num = 50, bool endpoint = true,
        const T &base = T(10)
    );

    /**
     * @brief Return numbers spaced evenly on a log scale (a geometric
     * progression). This is similar to logspace, but with endpoints specified
     * directly. Each output sample is a constant multiple of the previous.
     *
     * @param start The starting value of the sequence.
     * @param stop The final value of the sequence.
     * @param num Number of samples to generate. Default is 50.
     * @param endpoint If true, stop is the last sample. Otherwise, it is not
     *     included. Default is true.
     *
     * @return A light-weight object with equally spaced samples on a log
     *     scale. Convertible to an array object.
     */
    template <class T>
    base_array<T, log_sequence_tag> geomspace(
        const T &start, const T &stop, size_t num = 50, bool endpoint = true
    );

    /// Building matrices

    /**
     * @brief Return a matrix with given values on the diagonal and zeros
     * elsewhere.
     *
     * @param arr An array-like object with the elements on the diagonal.
     * @param offset Offset of the diagonal from the main diagonal. A positive
     *     value refers to an upper diagonal and a negative value refers to a
     *     lower diagonal. Defaults to main diagonal (0).
     *
     * @return A light-weight object with the given values on the diagonal and
     *     zeros elsewhere. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, diagonal_tag<Tag> > diagonal(
        const base_array<T, Tag> &arr, ptrdiff_t offset = 0
    );

    /**
     * @brief Return the diagonal of a matrix.
     *
     * @param mat A matrix-like object from which the diagonal is taken.
     * @param offset Offset of the diagonal from the main diagonal. A positive
     *     value refers to an upper diagonal and a negative value refers to a
     *     lower diagonal. Defaults to main diagonal (0).
     *
     * @return A light-weight object with the elements on the diagonal.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, diagonal_tag<Tag> > diagonal(
        const base_matrix<T, Tag> &mat, ptrdiff_t offset = 0
    );

    /**
     * @brief Return a matrix with ones on the diagonal and zeros elsewhere.
     *
     * @param m Number of rows. If not provided, defaults to n.
     * @param n Number of columns.
     * @param offset Offset of the diagonal from the main diagonal. A positive
     *     value refers to an upper diagonal and a negative value refers to a
     *     lower diagonal. Defaults to main diagonal (0).
     *
     * @return A light-weight object with ones on the diagonal and zeros
     *     elsewhere. Convertible to a matrix object.
     */
    template <class T>
    base_matrix<T, eye_tag> eye(size_t n);

    template <class T>
    base_matrix<T, eye_tag> eye(size_t m, size_t n, ptrdiff_t offset = 0);

    /**
     * @brief Return the lower triangle portion of a matrix.
     *
     * @param mat A matrix-like object from which to extract the lower
     *     triangle.
     * @param offset Diagonal above which to zero elements. A positive value
     *     refers to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return A light-weight object with the lower triangle of the matrix.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, triangular_tag<Tag> >
    tril(const base_matrix<T, Tag> &mat, ptrdiff_t offset = 0);

    /**
     * @brief Return the upper triangle portion of a matrix.
     *
     * @param mat A matrix-like object from which to extract the upper
     *     triangle.
     * @param offset Diagonal below which to zero elements. A positive value
     *     refers to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return A light-weight object with the upper triangle of the matrix.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, triangular_tag<Tag> >
    triu(const base_matrix<T, Tag> &mat, ptrdiff_t offset = 0);

    /// Maximums and minimums

    /**
     * @brief Return the index of the maximum value in the array.
     *
     * @param arr An array-like object.
     *
     * @return The index of the maximum value in the array.
     *
     * @note In case of multiple occurrences of the maximum value, return
     *     the index corresponding to the first occurrence.
     */
    template <class T, class Tag>
    size_t argmax(const base_array<T, Tag> &arr);

    /**
     * @brief Return the indices of the maximum value in the matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return The indices of the maximum value in the matrix.
     *
     * @note In case of multiple occurrences of the maximum value, return the
     *     index corresponding to the first occurrence.
     */
    template <class T, class Tag>
    index_t argmax(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the index of the maximum value in the matrix along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the index of the maximum value along
     *     each row. Otherwise, return the index of the maximum value along
     *     each column.
     *
     * @return A light-weight object with the index of the maximum value along
     *     an axis. Convertible to an array object.
     *
     * @note In case of multiple occurrences of the maximum value, return the
     *     index corresponding to the first occurrence.
     */
    template <class T, class Tag>
    base_array< size_t, lazy_axis_tag<__range_argmax, T, Tag> >
    argmax(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the index of the minimum value in the array.
     *
     * @param arr An array-like object.
     *
     * @return The index of the minimum value in the array.
     *
     * @note In case of multiple occurrences of the minimum value, return the
     *     index corresponding to the first occurrence.
     */
    template <class T, class Tag>
    size_t argmin(const base_array<T, Tag> &arr);

    /**
     * @brief Return the indices of the minimum value in the matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return The indices of the minimum value in the matrix.
     *
     * @note In case of multiple occurrences of the minimum value, return the
     *     index corresponding to the first occurrence.
     */
    template <class T, class Tag>
    index_t argmin(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the index of the minimum value in the matrix along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the index of the minimum value along
     *     each row. Otherwise, return the index of the minimum value along
     *     each column.
     *
     * @return A light-weight object with the index of the minimum value along
     *     an axis. Convertible to an array object.
     *
     * @note In case of multiple occurrences of the minimum value, return the
     *     index corresponding to the first occurrence.
     */
    template <class T, class Tag>
    base_array< size_t, lazy_axis_tag<__range_argmin, T, Tag> >
    argmin(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the maximum value contained in the array.
     *
     * @param arr An array-like object.
     *
     * @return The maximum value in the array.
     */
    template <class T, class Tag>
    T amax(const base_array<T, Tag> &arr);

    /**
     * @brief Return the maximum value contained in the matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return The maximum value in the matrix.
     */
    template <class T, class Tag>
    T amax(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the maximum value contained in the matrix along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the maximum value along each row.
     *     Otherwise, return the maximum value along each column.
     *
     * @return A light-weight object with the maximum value along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_max, T, Tag> >
    amax(const base_matrix<T, Tag> &mat, size_t rowwise);

    /**
     * @brief Return the minimum value contained in the array.
     *
     * @param arr An array-like object.
     *
     * @return The minimum value in the array.
     */
    template <class T, class Tag>
    T amin(const base_array<T, Tag> &arr);

    /**
     * @brief Return the minimum value contained in the matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return The minimum value in the matrix.
     */
    template <class T, class Tag>
    T amin(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the minimum value contained in the matrix along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the minimum value along each row.
     *     Otherwise, return the minimum value along each column.
     *
     * @return A light-weight object with the minimum value along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_min, T, Tag> >
    amin(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the element-wise maximum of two arrays.
     *
     * @param lhs Left-hand side array-like object.
     * @param rhs Right-hand side array-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object with the element-wise maximum of the two
     *     arrays. Convertible to an array object.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are array objects with different sizes.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_max, T, Tag1, T, Tag2> >
    maximum(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs);

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_max, T, Tag, T, scalar_tag> >
    maximum(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    );

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_max, T, scalar_tag, T, Tag> >
    maximum(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    );

    /**
     * @brief Return the element-wise maximum of two matrices.
     *
     * @param lhs Left-hand side matrix-like object.
     * @param rhs Right-hand side matrix-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object with the element-wise maximum of the two
     *     matrices. Convertible to a matrix object.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are matrix objects with different number of rows and
     *     columns.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_max, T, Tag1, T, Tag2> >
    maximum(const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs);

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_max, T, Tag, T, scalar_tag> >
    maximum(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val
    );

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_max, T, scalar_tag, T, Tag> >
    maximum(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs
    );

    /**
     * @brief Return the element-wise minimum of two arrays.
     *
     * @param lhs Left-hand side array-like object.
     * @param rhs Right-hand side array-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object with the element-wise minimum of the two
     *     arrays. Convertible to an array object.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are array objects with different sizes.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_min, T, Tag1, T, Tag2> >
    minimum(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs);

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_min, T, Tag, T, scalar_tag> >
    minimum(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    );

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_min, T, scalar_tag, T, Tag> >
    minimum(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    );

    /**
     * @brief Return the element-wise minimum of two matrices.
     *
     * @param lhs Left-hand side matrix-like object.
     * @param rhs Right-hand side matrix-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object with the element-wise minimum of the two
     *     matrices. Convertible to a matrix object.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are matrix objects with different number of rows and
     *     columns.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_min, T, Tag1, T, Tag2> >
    minimum(const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs);

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_min, T, Tag, T, scalar_tag> >
    minimum(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val
    );

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_min, T, scalar_tag, T, Tag> >
    minimum(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs
    );

    /**
     * @brief Clamp the values in the array. Given an interval [a_min, a_max],
     * values smaller than a_min become a_min, and values larger than a_max
     * become a_max. For complex types, the real and imaginary parts are
     * clamped separately.
     *
     * @param arr An array-like object.
     * @param a_min The lower boundary to clamp.
     * @param a_max The upper boundary to clamp.
     *
     * @return A light-weight object with the array formed by clamping the
     *     values in the given array. Convertible to an array object.
     *
     * @note The behavior is undefined if a_min is greater than a_max.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__clamp<T>, T, Tag> >
    clamp(
        const base_array<T, Tag> &arr,
        const typename base_array<T, Tag>::value_type &a_min,
        const typename base_array<T, Tag>::value_type &a_max
    );

    /**
     * @brief Clamp the values in the matrix. Given an interval [a_min, a_max],
     * values smaller than a_min become a_min, and values larger than a_max
     * become a_max. For complex types, the real and imaginary parts are
     * clamped separately.
     *
     * @param mat A matrix-like object.
     * @param a_min The lower boundary to clamp.
     * @param a_max The upper boundary to clamp.
     *
     * @return A light-weight object with the matrix formed by clamping the
     *     values in the given matrix. Convertible to a matrix object.
     *
     * @note The behavior is undefined if a_min is greater than a_max.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__clamp<T>, T, Tag> >
    clamp(
        const base_matrix<T, Tag> &mat,
        const typename base_matrix<T, Tag>::value_type &a_min,
        const typename base_matrix<T, Tag>::value_type &a_max
    );

    /// Sums and products

    /**
     * @brief Return the sum of the array elements.
     *
     * @param arr An array-like object.
     *
     * @return The sum of the array elements.
     */
    template <class T, class Tag>
    T sum(const base_array<T, Tag> &arr);

    /**
     * @brief Return the sum of the matrix elements.
     *
     * @param mat A matrix-like object.
     *
     * @return The sum of the matrix elements.
     */
    template <class T, class Tag>
    T sum(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the sum of the matrix elements along the specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the sum of the elements along each row.
     *     Otherwise, return the sum of the elements along each column.
     *
     * @return A light-weight object with the sum along an axis. Convertible to
     *     an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_sum, T, Tag> >
    sum(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the product of the array elements.
     *
     * @param arr An array-like object.
     *
     * @return The product of the array elements.
     */
    template <class T, class Tag>
    T prod(const base_array<T, Tag> &arr);

    /**
     * @brief Return the product of the matrix elements.
     *
     * @param mat A matrix-like object.
     *
     * @return The product of the matrix elements.
     */
    template <class T, class Tag>
    T prod(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the product of the matrix elements along the specified
     * axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the product of the elements along each
     *     row. Otherwise, return the product of the elements along each
     *     column.
     *
     * @return A light-weight object with the product along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_prod, T, Tag> >
    prod(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the cumulative sum of the array elements.
     *
     * @param arr An array-like object.
     *
     * @return The cumulative sum of the array elements.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> cumsum(const base_array<T, Tag> &arr);

    /**
     * @brief Return the cumulative sum of the matrix elements along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the cumulative sum of the elements along
     *     each row. Otherwise, return the cumulative sum of the elements along
     *     each column.
     *
     * @return The cumulative sum along an axis.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> cumsum(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the cumulative product of the array elements.
     *
     * @param arr An array-like object.
     *
     * @return The cumulative product of the array elements.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> cumprod(const base_array<T, Tag> &arr);

    /**
     * @brief Return the cumulative product of the matrix elements along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the cumulative product of the elements
     *     along each row. Otherwise, return the cumulative product of the
     *     elements along each column.
     *
     * @return The cumulative product along an axis.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> cumprod(const base_matrix<T, Tag> &mat, bool rowwise);

    /// Concatenation

    /**
     * @brief Concatenate one or more arrays.
     *
     * @param arr1, arr2... Arrays to concatenate. All arrays must have same
     *     data type.
     *
     * @return The array formed by concatenating the array arguments.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class... Arrays>
    array<T> concatenate(const base_array<T, Tag> &arr1, const Arrays&... arr2);

    /**
     * @brief Concatenate one or more arrays and matrices vertically.
     *
     * @param mat1, mat2... Matrices to stack. All matrices must have same data
     *     type. Arrays are treated as row vectors.
     *
     * @return The matrix formed by stacking the array and matrix arguments
     *     row-wise.
     *
     * @throw std::invalid_argument Thrown if the number of columns is not the
     *     same for all the arguments.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class... Matrices>
    matrix<T> row_stack(
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    );

    template <class T, class Tag, class... Matrices>
    matrix<T> row_stack(
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    );

    /**
     * @brief Concatenate one or more arrays and matrices horizontally.
     *
     * @param mat1, mat2... Matrices to stack. All matrices must have same data
     *     type. Arrays are treated as column vectors.
     *
     * @return The matrix formed by stacking the array and matrix arguments
     *     column-wise.
     *
     * @throw std::invalid_argument Thrown if the number of rows is not the
     *     same for all the arguments.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class... Matrices>
    matrix<T> column_stack(
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    );

    template <class T, class Tag, class... Matrices>
    matrix<T> column_stack(
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    );

    /// Padding

    /**
     * @brief Pad an array.
     *
     * @param arr The array to pad.
     * @param before Number of elements to pad at the beginning of the array.
     * @param after Number of elements to pad at the end of the array.
     * @param mode One of the following string values:
     *     "empty" (default) Pads with uninitialized values.
     *     "constant" Pads with a constant value.
     *     "edge" Pads with the edge values of the array.
     *     "linear_ramp" Pads with the linear ramp between the array edge value
     *     and an end value.
     *     "reflect" Pads with the reflection of the array mirrored on the
     *     first and last values.
     *     "symmetric" Pads with the reflection of the array mirrored along the
     *     edge.
     *     "wrap" Pads with the wrap of the array.
     * @param args An initializer list with additional arguments to be used in
     *     "constant" and "linear_ramp" modes. If empty, a 0 will be used as
     *     before and after constants. If a single value is passed, it will be
     *     used as both before and after constants. If two values are passed,
     *     they will be used as before and after constants.
     *
     * @return The padded array.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> pad(
        const base_array<T, Tag> &arr, size_t before, size_t after,
        const std::string &mode = "empty",
        std::initializer_list<typename base_array<T, Tag>::value_type> args = {}
    );

    /**
     * @brief Pad a matrix.
     *
     * @param mat The matrix to pad.
     * @param before1 Number of elements to pad at the top of the matrix.
     * @param after1 Number of elements to pad at the bottom of the matrix.
     * @param before2 Number of elements to pad at the left of the matrix.
     * @param after2 Number of elements to pad at the right of the matrix.
     * @param mode One of the following string values:
     *     "empty" (default) Pads with uninitialized values.
     *     "constant" Pads with a constant value.
     *     "edge" Pads with the edge values along each axis.
     *     "linear_ramp" Pads with the linear ramp between the edge value and
     *     an end value.
     *     "reflect" Pads with the reflection mirrored on the first and last
     *     values along each axis.
     *     "symmetric" Pads with the reflection mirrored along the edge on
     *     each axis.
     *     "wrap" Pads with the wrap of the matrix.
     * @param args An initializer list with additional arguments to be used in
     *     "constant" and "linear_ramp" modes. If empty, a 0 will be used for
     *     all the constants. If a single value is passed, the same value will
     *     be used for all the constants. If two values are passed, the first
     *     one will be used as before1 and after1 constants and the later oner
     *     as before2 and after2 constants. If four values are passed, they
     *     will be used as before1, after1, before2 and after2 constants.
     *
     * @return The padded matrix.
     *
     * @note The matrix is padded first column-wise and then row-wise.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> pad(
        const base_matrix<T, Tag> &mat,
        size_t before1, size_t after1, size_t before2, size_t after2,
        const std::string &mode = "empty",
        std::initializer_list<typename base_matrix<T, Tag>::value_type> args={}
    );

    /// Insertion-deletion

    /**
     * @brief Insert values before the given indices.
     *
     * @param arr An array-like object.
     * @param index A single position or an array of positions in the array
     *     where the values are inserted.
     * @param value A single value or an array of values to be inserted into
     *     the array.
     *
     * @return A new array with the values inserted. Note that insert does not
     *     occur in-place, a new array is returned.
     *
     * @throw std::invalid_argument Thrown if index and value arguments are
     *     array objects with different sizes.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> insert(
        const base_array<T, Tag> &arr,
        size_t index, const typename base_array<T, Tag>::value_type &value
    );

    template <class T, class Tag, class IndexTag, class ValueTag>
    array<T> insert(
        const base_array<T, Tag> &arr,
        const base_array<size_t, IndexTag> &index,
        const base_array<T, ValueTag> &value
    );

    /**
     * @brief Removes elements from the array.
     *
     * @param arr An array-like object.
     * @param index A single position or an array of positions in the array of
     *     the elements to be removed.
     *
     * @return A new array with the elements removed. Note that erase does not
     *     occur in-place, a new array is returned.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> erase(const base_array<T, Tag> &arr, size_t index);

    template <class T, class Tag, class IndexTag>
    array<T> erase(
        const base_array<T, Tag> &arr,
        const base_array<size_t, IndexTag> &index
    );

    /// Logic functions

    /**
     * @brief Test whether all the elements in the array evaluate to true.
     *
     * @param arr An array-like of bool.
     *
     * @return true if all the elements evaluate to true and false otherwise.
     */
    template <class Tag>
    bool all(const base_array<bool, Tag> &arr);

    /**
     * @brief Test whether all the elements in the matrix evaluate to true.
     *
     * @param mat A matrix-like of bool.
     *
     * @return true if all the elements evaluate to true and false otherwise.
     */
    template <class Tag>
    bool all(const base_matrix<bool, Tag> &mat);

    /**
     * @brief Test whether all the elements along the specified axis evaluate
     * to true.
     *
     * @param mat A matrix-like of bool.
     * @param rowwise If true, test along the elements on each row. Otherwise,
     *     test along the elements on each column.
     *
     * @return A light-weight object with the results of the test along an
     *     axis. Convertible to an array of bool.
     */
    template <class Tag>
    base_array< bool, lazy_axis_tag<__range_all, bool, Tag> >
    all(const base_matrix<bool, Tag> &mat, bool rowwise);

    /**
     * @brief Test whether any element in the array evaluate to true.
     *
     * @param arr An array-like of bool.
     *
     * @return true if any element evaluate to true and false otherwise.
     */
    template <class Tag>
    bool any(const base_array<bool, Tag> &arr);

    /**
     * @brief Test whether any element in the matrix evaluate to true.
     *
     * @param mat A matrix-like of bool.
     *
     * @return true if any element evaluate to true and false otherwise.
     */
    template <class Tag>
    bool any(const base_matrix<bool, Tag> &mat);

    /**
     * @brief Test whether any element along the specified axis evaluate to
     * true.
     *
     * @param mat A matrix-like of bool.
     * @param rowwise If true, test along the elements on each row. Otherwise,
     *     test along the elements on each column.
     *
     * @return A light-weight object with the results of the test along an
     *     axis. Convertible to an array of bool.
     */
    template <class Tag>
    base_array< bool, lazy_axis_tag<__range_any, bool, Tag> >
    any(const base_matrix<bool, Tag> &mat, bool rowwise);

    /**
     * @brief Count the number of non-zero elements in the array.
     *
     * @param arr An array-like object.
     *
     * @return The number of non-zero elements.
     */
    template <class T, class Tag>
    size_t count_nonzero(const base_array<T, Tag> &arr);

    /**
     * @brief Count the number of non-zero elements in the matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return The number of non-zero elements.
     */
    template <class T, class Tag>
    size_t count_nonzero(const base_matrix<T, Tag> &mat);

    /**
     * @brief Count the number of non-zero elements along the specified axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, count the number of non-zero elements along each
     *     row. Otherwise, count the number of non-zero elements along each
     *     column.
     *
     * @return A light-weight object with the number of non-zero elements along
     *     an axis. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< size_t, lazy_axis_tag<__range_count_nonzero, T, Tag> >
    count_nonzero(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return if two numbers are equal within a tolerance.
     *
     * @details Whether or not two values are considered equal is determined
     * according to given absolute and relative tolerance. The tolerances must
     * be non-negative, typically very small numbers. For floating-point
     * values, the function uses the following equation to test whether two
     * numbers are equivalent:
     *     abs(a - b) <= max(rtol * max(abs(a), abs(b)), atol)
     * NaN is not considered equal to any other value, including NaN. inf and
     * -inf are only considered equal to themselves.
     * For complex types, the equality is tested on both real and imaginary
     * parts.
     *
     * @param a A floating-point or complex number.
     * @param b A floating-point or complex number.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return true if the values are considered equal and false otherwise.
     *
     */
    bool isclose(double a, double b, double rtol = 1e-8, double atol = 0);

    bool isclose(float a, float b, float rtol = 1e-8f, float atol = 0);

    bool isclose(
        long double a, long double b,
        long double rtol = 1e-8L, long double atol = 0
    );

    template <class T>
    bool isclose(
        const std::complex<T> &a, const std::complex<T> &b,
        T rtol = T(1e-8), T atol = T(0)
    );

    /**
     * @brief Return if two arrays are equal, element-wise, within a tolerance.
     *
     * @param lhs Left-hand side array-like object.
     * @param rhs Right-hand side array-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return A light-weight object with the results of the test. Convertible
     *     to an array of bool.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are array objects with different sizes.
     */
    template <class T, class Tag1, class Tag2>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> >
    isclose(
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> >
    isclose(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> >
    isclose(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    /**
     * @brief Return if two matrices are equal, element-wise, within a
     * tolerance.
     *
     * @param lhs Left-hand side matrix-like object.
     * @param rhs Right-hand side matrix-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return A light-weight object with the results of the test. Convertible
     *     to a matrix of bool.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are matrix objects with different number of rows and
     *     columns.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> >
    isclose(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> >
    isclose(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> >
    isclose(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    /**
     * @brief Test whether all the elements in two arrays are element-wise
     * equal within a tolerance.
     *
     * @param lhs Left-hand side array-like object.
     * @param rhs Right-hand side array-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return true if all the elements are considered equal and false
     *     otherwise.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are array objects with different sizes.
     */
    template <class T, class Tag1, class Tag2>
    bool allclose(
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    bool allclose(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    bool allclose(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    /**
     * @brief Test whether all the elements in two matrices are element-wise
     * equal within a tolerance.
     *
     * @param lhs Left-hand side matrix-like object.
     * @param rhs Right-hand side matrix-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return true if all the elements are considered equal and false
     *     otherwise.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are matrix objects with different number of rows and
     *     columns.
     */
    template <class T, class Tag1, class Tag2>
    bool allclose(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    bool allclose(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    template <class T, class Tag>
    bool allclose(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol = 1e-8,
        typename complex_traits<T>::value_type atol = 0
    );

    /// Sorting and searching

    /**
     * @brief Return the indices that would sort the array.
     *
     * @param arr An array-like object.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return An array of indices that sort the array.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<size_t> argsort(const base_array<T, Tag> &arr);

    template <class T, class Tag, class Compare>
    array<size_t> argsort(
        const base_array<T, Tag> &arr, Compare comp, bool stable = false
    );

    /**
     * @brief Return the indices that would sort the matrix.
     *
     * @param mat A matrix-like object.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return An array of indices that sort the matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<index_t> argsort(const base_matrix<T, Tag> &mat);

    template <class T, class Tag, class Compare>
    array<index_t> argsort(
        const base_matrix<T, Tag> &mat, Compare comp, bool stable = false
    );

    /**
     * @brief Return the indices that would sort the matrix along an axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, sort the elements along each row. Otherwise,
     *     sort the elements along each column.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return A matrix of indices that sort the matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<size_t> argsort(const base_matrix<T, Tag> &mat, bool rowwise);

    template <class T, class Tag, class Compare>
    matrix<size_t> argsort(
        const base_matrix<T, Tag> &mat, bool rowwise,
        Compare comp, bool stable = false
    );

    /**
     * @brief Return a sorted copy of the array.
     *
     * @param arr An array-like object.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return A sorted copy of the array.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> sort(const base_array<T, Tag> &arr);

    template <class T, class Tag, class Compare>
    array<T> sort(
        const base_array<T, Tag> &arr, Compare comp, bool stable = false
    );

    /**
     * @brief Return a sorted copy of the flattened matrix.
     *
     * @param mat A matrix-like object.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return A sorted copy of the flattened matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> sort(const base_matrix<T, Tag> &mat);

    template <class T, class Tag, class Compare>
    array<T> sort(
        const base_matrix<T, Tag> &mat, Compare comp, bool stable = false
    );

    /**
     * @brief Return a sorted copy of the matrix.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, sort the elements along each row. Otherwise,
     *     sort the elements along each column.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return A sorted copy of the matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> sort(const base_matrix<T, Tag> &mat, bool rowwise);

    template <class T, class Tag, class Compare>
    matrix<T> sort(
        const base_matrix<T, Tag> &mat, bool rowwise,
        Compare comp, bool stable = false
    );

    /**
     * @brief Return the indices that would partition the array.
     *
     * @param arr An array-like object.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     array. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return An array of indices that partitions the array.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<size_t> argpartition(const base_array<T, Tag> &arr, size_t kth);

    template <class T, class Tag, class Compare>
    array<size_t> argpartition(
        const base_array<T, Tag> &arr, size_t kth, Compare comp
    );

    /**
     * @brief Return the indices that would partition the matrix.
     *
     * @param mat A matrix-like object.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     array. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return An array of indices that partitions the matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<index_t> argpartition(const base_matrix<T, Tag> &mat, size_t kth);

    template <class T, class Tag, class Compare>
    array<index_t> argpartition(
        const base_matrix<T, Tag> &mat, size_t kth, Compare comp
    );

    /**
     * @brief Return the indices that would partition the matrix along an axis.
     *
     * @param mat A matrix-like object.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     array. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param rowwise If true, partition the elements along each row.
     *     Otherwise, partition the elements along each column.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A matrix of indices that partitions the matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<size_t> argpartition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise
    );

    template <class T, class Tag, class Compare>
    matrix<size_t> argpartition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise, Compare comp
    );

    /**
     * @brief Return a partitioned copy of the array.
     *
     * @param arr An array-like object.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     array. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A partitioned copy of the array.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> partition(const base_array<T, Tag> &arr, size_t kth);

    template <class T, class Tag, class Compare>
    array<T> partition(
        const base_array<T, Tag> &arr, size_t kth, Compare comp
    );

    /**
     * @brief Return a partitioned copy of the flattened matrix.
     *
     * @param mat A matrix-like object.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     array. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A partitioned copy of the flattened matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> partition(const base_matrix<T, Tag> &mat, size_t kth);

    template <class T, class Tag, class Compare>
    array<T> partition(
        const base_matrix<T, Tag> &mat, size_t kth, Compare comp
    );

    /**
     * @brief Return a partitioned copy of the matrix.
     *
     * @param mat A matrix-like object.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     array. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param rowwise If true, partition the elements along each row.
     *     Otherwise, partition the elements along each column.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A partitioned copy of the matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> partition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise
    );

    template <class T, class Tag, class Compare>
    matrix<T> partition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise, Compare comp
    );

    /**
     * @brief Find the indices of the elements that evaluate to true.
     *
     * @param condition An array-like of bool.
     *
     * @return An array of indices of the elements equal to true.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Tag>
    array<size_t> where(const base_array<bool, Tag> &condition);

    /**
     * @brief Find the indices of the elements that evaluate to true.
     *
     * @param condition A matrix-like of bool.
     *
     * @return An array of indices of the elements equal to true.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Tag>
    array<index_t> where(const base_matrix<bool, Tag> &condition);

    /// Basic statistics

    /**
     * @brief Return the average of the array elements.
     *
     * @param arr An array-like object.
     *
     * @return The average of the array elements.
     */
    template <class T, class Tag>
    T mean(const base_array<T, Tag> &arr);

    /**
     * @brief Return the average of the matrix elements.
     *
     * @param mat A matrix-like object.
     *
     * @return The average of the matrix elements.
     */
    template <class T, class Tag>
    T mean(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the average of the matrix elements along the specified
     * axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the average of the elements along each
     *     row. Otherwise, return the average of the elements along each
     *     column.
     *
     * @return A light-weight object with the average along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_mean, T, Tag> >
    mean(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the median of the array elements.
     *
     * @details The median is defined as the middle value of a sorted copy of
     * the array. If the length is even, the average of the two middle values
     * is returned.
     *
     * @param arr An array-like object.
     *
     * @return The median of the array elements.
     */
    template <class T, class Tag>
    T median(const base_array<T, Tag> &arr);

    /**
     * @brief Return the median of the matrix elements.
     *
     * @param mat A matrix-like object.
     *
     * @return The median of the matrix elements.
     */
    template <class T, class Tag>
    T median(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the median of the matrix elements along the specified
     * axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, return the median of the elements along each
     *     row. Otherwise, return the median of the elements along each
     *     column.
     *
     * @return A light-weight object with the median along an axis. Convertible
     *     to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_median, T, Tag> >
    median(const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Return the variance of the array elements.
     *
     * @details The variance is defined as the average of the squared
     * deviations from the mean.
     *     var(a) = mean(abs(a - mean(a))**2)
     * The mean is calculated as sum(*)/n, where n = a.size(). However, if ddof
     * is specified, the divisor n - ddof is used instead of n.
     * In statistics, ddof = 1 provides an unbiased estimator of the sample
     * variance; while ddof = 0 provides the maximum likelihood estimator of
     * the variance for normally distributed variables.
     *
     * @param arr An array-like object.
     * @param ddof Delta degrees of freedom.
     *
     * @return The variance of the array elements.
     */
    template <class T, class Tag>
    T var(const base_array<T, Tag> &arr, size_t ddof = 0);

    /**
     * @brief Return the variance of the matrix elements.
     *
     * @param mat A matrix-like object.
     * @param ddof Delta degrees of freedom.
     *
     * @return The variance of the matrix elements.
     */
    template <class T, class Tag>
    T var(const base_matrix<T, Tag> &mat, size_t ddof = 0);

    /**
     * @brief Return the variance of the matrix elements along the specified
     * axis.
     *
     * @param mat A matrix-like object.
     * @param ddof Delta degrees of freedom.
     * @param rowwise If true, return the variance of the elements along each
     *     row. Otherwise, return the variance of the elements along each
     *     column.
     *
     * @return A light-weight object with the variance along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_var, T, Tag> >
    var(const base_matrix<T, Tag> &mat, size_t ddof, bool rowwise);

    /**
     * @brief Return the standard deviation of the array elements.
     *
     * @details The standard deviation is defined as the square root of the
     * variance.
     *     stddev(a) = sqrt(var(a))
     *
     * @param arr An array-like object.
     * @param ddof Delta degrees of freedom.
     *
     * @return The standard deviation of the array elements.
     */
    template <class T, class Tag>
    T stddev(const base_array<T, Tag> &arr, size_t ddof = 0);

    /**
     * @brief Return the standard deviation of the matrix elements.
     *
     * @param mat A matrix-like object.
     * @param ddof Delta degrees of freedom.
     *
     * @return The standard deviation of the matrix elements.
     */
    template <class T, class Tag>
    T stddev(const base_matrix<T, Tag> &mat, size_t ddof = 0);

    /**
     * @brief Return the standard deviation of the matrix elements along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param ddof Delta degrees of freedom.
     * @param rowwise If true, return the standard deviation of the elements
     *     along each row. Otherwise, return the standard deviation of the
     *     elements along each column.
     *
     * @return A light-weight object with the standard deviation along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_stddev, T, Tag> >
    stddev(const base_matrix<T, Tag> &mat, size_t ddof, bool rowwise);

    /**
     * @brief Return the q-th quantile of the array elements.
     *
     * @details Given an array of size n, the q-th quantile is the (n - 1)*q-th
     * value of a sorted copy of the array. If (n - 1)*q is not an integer, the
     * quantile is a weighted average of the two nearest neighbors. The
     * different methods work as follows:
     *     "lower" Takes the lowest neighbor.
     *     "higher" Takes the highest neighbor.
     *     "nearest" Takes the nearest neighbor.
     *     "midpoint" Takes the average of both neighbors.
     *     "linear" Takes a linear interpolation between both neighbors.
     *
     * @param arr An array-like object.
     * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
     * @param method This parameter specifies the method to use for
     *     estimating the quantile. Must be one of "lower", "higher",
     *     "nearest", "midpoint" or "linear".
     *
     * @return The q-th quantile of the array elements.
     */
    template <class T, class Tag>
    T quantile(
        const base_array<T, Tag> &arr, double q,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the q-th quantile of the matrix elements.
     *
     * @param mat A matrix-like object.
     * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
     * @param method This parameter specifies the method to use for
     *     estimating the quantile. Must be one of "lower", "higher",
     *     "nearest", "midpoint" or "linear".
     *
     * @return The q-th quantile of the matrix elements.
     */
    template <class T, class Tag>
    T quantile(
        const base_matrix<T, Tag> &mat, double q,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the q-th quantile of the matrix elements along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
     * @param rowwise If true, return the quantile of the elements along each
     *     row. Otherwise, return the quantile of the elements along each
     *     column.
     * @param method This parameter specifies the method to use for
     *     estimating the quantile. Must be one of "lower", "higher",
     *     "nearest", "midpoint" or "linear".
     *
     * @return A light-weight object with the q-th quantile along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_quantile, T, Tag> >
    quantile(
        const base_matrix<T, Tag> &mat, double q, bool rowwise,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the q-th percentile of the array elements. The q-th
     * percentile is defined as the q/100-th quantile.
     *
     * @param arr An array-like object.
     * @param q Percentile to compute, which must be between 0 and 100
     *     (inclusive).
     * @param method This parameter specifies the method to use for
     *     estimating the percentile. Must be one of "lower", "higher",
     *     "nearest", "midpoint" or "linear".
     *
     * @return The q-th percentile of the array elements.
     */
    template <class T, class Tag>
    T percentile(
        const base_array<T, Tag> &arr, double q,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the q-th percentile of the matrix elements.
     *
     * @param mat A matrix-like object.
     * @param q Percentile to compute, which must be between 0 and 100
     *     (inclusive).
     * @param method This parameter specifies the method to use for
     *     estimating the percentile. Must be one of "lower", "higher",
     *     "nearest", "midpoint" or "linear".
     *
     * @return The q-th percentile of the matrix elements.
     */
    template <class T, class Tag>
    T percentile(
        const base_matrix<T, Tag> &mat, double q,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the q-th percentile of the matrix elements along the
     * specified axis.
     *
     * @param mat A matrix-like object.
     * @param q Percentile to compute, which must be between 0 and 100
     *     (inclusive).
     * @param rowwise If true, return the percentile of the elements along each
     *     row. Otherwise, return the percentile of the elements along each
     *     column.
     * @param method This parameter specifies the method to use for
     *     estimating the percentile. Must be one of "lower", "higher",
     *     "nearest", "midpoint" or "linear".
     *
     * @return A light-weight object with the q-th percentile along an axis.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_quantile, T, Tag> >
    percentile(
        const base_matrix<T, Tag> &mat, double q, bool rowwise,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the covariance of two arrays.
     *
     * @details The covariance is defined as the average of the element-wise
     * products of the deviations from the mean:
     *    cov(x, y) = mean((x - mean(x))*(y - mean(y)))
     * For complex types, the complex conjugate of y is used:
     *    cov(x, y) = mean((x - mean(x))*conj(y - mean(y)))
     * The mean is calculated as sum(*)/n, where n = x.size(). However, if ddof
     * is specified, the divisor n - ddof is used instead of n.
     *
     * @param arr1 First array-like argument.
     * @param arr2 Second array-like argument.
     * @param ddof Delta degrees of freedom.
     *
     * @return The covariance of the two arrays.
     *
     * @throw std::invalid_argument Thrown if the array arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T cov(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2,
        size_t ddof = 0
    );

    /**
     * @brief Return the covariance matrix of given data.
     *
     * @param mat A matrix-like object containing multiple variables and
     *     observations.
     * @param rowwise If true, then each row represents a variable, with
     *     observations in the columns. Otherwise, each column represents a
     *     variable, with observations in the rows.
     * @param ddof Delta degrees of freedom.
     *
     * @return The covariance matrix of the variables. The element (i, j) is
     *     equal to the covariance of the i-th and j-th variables.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> cov(
        const base_matrix<T, Tag> &mat, bool rowwise, size_t ddof = 0
    );

    /**
     * @brief Return the Pearson's correlation coefficient of two arrays.
     *
     * @details The correlation coefficient is defined as the covariance of the
     * two variables divided by the product of their standard deviations.
     *     corr(x, y) = cov(x, y)/sqrt(var(x)*var(y))
     *
     * @param arr1 First array-like argument.
     * @param arr2 Second array-like argument.
     *
     * @return The correlaction coefficient of the two arrays.
     *
     * @throw std::invalid_argument Thrown if the array arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T corrcoef(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    );

    /**
     * @brief Return the correlation matrix of given data.
     *
     * @param mat A matrix-like object containing multiple variables and
     *     observations.
     * @param rowwise If true, then each row represents a variable, with
     *     observations in the columns. Otherwise, each column represents a
     *     variable, with observations in the rows.
     *
     * @return The correlation matrix of the variables. The element (i, j) is
     *     equal to the correlation coefficient of the i-th and j-th variables.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> corrcoef(const base_matrix<T, Tag> &mat, bool rowwise);

    /// Basic linear algebra

    /**
     * @brief Return the inner product of two arrays.
     *
     * @details For real arrays, the inner product is defined as the sum of the
     * element-wise products of the two arrays:
     *     inner(a, b) = sum(a * b)
     * For complex arrays, the complex conjugate of the first argument is used:
     *     inner(a, b) = sum(conj(a) * b)
     *
     * @param arr1 First array-like argument.
     * @param arr2 Second array-like argument.
     *
     * @return The inner product of the two arrays.
     *
     * @throw std::invalid_argument Thrown if the sizes do not match.
     */
    template <class T, class Tag1, class Tag2>
    T inner(const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2);

    /**
     * @brief Return the outer product of two arrays.
     *
     * @details The outer product of two arrays is defined as the matrix whose
     * element (i, j) is equal to the product of the i-th and j-th elements of
     * both arrays.
     *
     * @param arr1 First array-like argument.
     * @param arr2 Second array-like argument.
     *
     * @return A light-weight object with the outer product of the two arrays.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, outer_tag<__multiplies, T, Tag1, T, Tag2> >
    outer(const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2);

    /**
     * @brief Return the dot product of two arrays or the matrix multiplication
     * of two matrices.
     *
     * @details If both arguments are arrays, return the inner product of the
     * arrays (without complex conjugation).
     * If both arguments are matrices, return the matrix multiplication of the
     * matrices.
     * If the first argument is an array and the second argument is a matrix,
     * return the matrix multiplication with the array treated as a row
     * vector.
     * If the first argument is a matrix and the second argument is an array,
     * return the matrix multiplication with the array treated as a column
     * vector.
     *
     * The matrix multiplication is the matrix whose element (i, j) is equal to
     * the inner product (without complex conjugation) of the i-th row of the
     * first argument and the j-th column of the second argument.
     *
     * @param lhs Left-hand side array-like or matrix-like object.
     * @param rhs Right-hand side array-like or matrix-like object.
     *
     * @return The dot product or matrix multiplication of the arguments.
     *
     * @throw std::invalid_argument Thrown if the number of columns in the
     *     first argument do not match the number of rows in the second
     *     argument.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    T dot(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs);

    template <class T, class Tag1, class Tag2>
    matrix<T> dot(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    );

    template <class T, class Tag1, class Tag2>
    array<T> dot(
        const base_array<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    );

    template <class T, class Tag1, class Tag2>
    array<T> dot(
        const base_matrix<T, Tag1> &lhs, const base_array<T, Tag2> &rhs
    );

    /**
     * @brief Return the Kronecker product of two matrices.
     *
     * @details If A is an m x n matrix and B is a p x q matrix, the Kronecker
     * product of A and B is the pm x qn block matrix:
     *     [[    A[0, 0]*B,     A[0, 1]*B, ... ,     A[0, n - 1]*B],
     *      [    A[1, 0]*B,     A[1, 1]*B, ... ,     A[1, n - 1]*B],
     *      ...
     *      [A[m - 1, 0]*B, A[m - 1, 1]*B, ... , A[m - 1, n - 1]*B]]
     *
     * @param lhs Left-hand side matrix-like object.
     * @param rhs Right-hand side matrix-like object.
     *
     * @return A light-weight object with the Kronecker product of the two
     *     matrices. Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, kronecker_tag<Tag1, Tag2> >
    kron(const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs);

    /**
     * @brief Return the cross product of two arrays.
     *
     * @details The cross product of two vectors a and b (in R3) is a vector
     * perpendicular to both a and b. When the dimension of either a or b is 2,
     * the third component of the vector is assumed to be zero.
     *
     * @param arr1 First array-like argument.
     * @param arr2 Second array-like argument.
     *
     * @return The cross product of the arrays.
     *
     * @throw std::invalid_argument Thrown if the size of the arrays is neither
     *     2 nor 3.
     */
    template <class T, class Tag1, class Tag2>
    array<T> cross(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    );

    /**
     * @brief Return the transpose of a matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return A light-weight object with the matrix transposed. Convertible to
     *     a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, transpose_tag<Tag> >
    transpose(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the complex conjugate transpose of a matrix.
     *
     * @param mat A matrix-like object.
     *
     * @return A light-weight object with the conjugate transposed matrix. If T
     *     is not a complex type, the function returns the matrix transposed.
     */
    template <class T, class Tag>
    base_matrix< T, conj_transpose_tag<Tag> >
    conj_transpose(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return the vector norm.
     *
     * @details This function is able to return one of the following norms:
     *        p sum(abs(arr)**p)**(1/p)
     *        0 sum(x != 0)
     *      inf max(abs(x))
     *     -inf min(abs(x))
     *
     * @param arr An array-like object.
     * @param p Order of the norm. The default is 2 (Euclidean norm). For
     *     values of p < 1, the result is, strictly speaking, not a
     *     mathematical norm, but it may still be useful for various numerical
     *     purposes.
     *
     * @return Norm of the array.
     */
    template <class T, class Tag>
    typename complex_traits<T>::value_type
    norm(const base_array<T, Tag> &arr, double p = 2);

    /**
     * @brief Return the sum along a diagonal of the matrix, i.e., the sum
     * of the elements a(i, i + offset) for all i.
     *
     * @param mat A matrix-like object.
     * @param offset Offset of the diagonal from the main diagonal. A positive
     *     value refers to an upper diagonal and a negative value refers to a
     *     lower diagonal. Defaults to main diagonal (0).
     *
     * @return The sum along the diagonal.
     */
    template <class T, class Tag>
    T trace(const base_matrix<T, Tag> &mat, ptrdiff_t offset = 0);

    /// Set operations

    /**
     * @brief Find the unique elements of an array.
     *
     * @param arr An array-like object.
     *
     * @return A new array with the sorted unique values.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> unique(const base_array<T, Tag> &arr);

    /**
     * @brief Test whether a value is present in an array.
     *
     * @param arr An array-like object. The function tests whether arr
     *     contains val. It must be sorted.
     * @param val The value to test.
     *
     * @return A bool with the result of the test.
     */
    template <class T, class Tag>
    bool includes(const base_array<T, Tag> &arr, const T &val);

    /**
     * @brief Test whether all the elements of an array are also present in
     * another array.
     *
     * @param arr An array-like object. The function tests whether arr
     *     contains all the elements of val. It must be sorted.
     * @param val An array-like object with the elements to test. It must be
     *     sorted.
     *
     * @return A bool with the results of the test.
     */
    template <class T, class Tag1, class Tag2>
    bool includes(
        const base_array<T, Tag1> &arr, const base_array<T, Tag2> &val
    );

    /**
     * @brief Return a new array with the unique, sorted elements that are
     * present in either one of the two arrays, or in both.
     *
     * @param arr1 An array-like object. It must be sorted.
     * @param arr2 An array-like object. It must be sorted.
     *
     * @return An array with the set union of the two arrays.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    array<T> set_union(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    );

    /**
     * @brief Return a new array with the unique, sorted elements that are
     * present in both arrays.
     *
     * @param arr1 An array-like object. It must be sorted.
     * @param arr2 An array-like object. It must be sorted.
     *
     * @return An array with the set intersection of the two arrays.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    array<T> set_intersection(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    );

    /**
     * @brief Return a new array with the unique, sorted elements that are
     * present in the first array, but not in the second.
     *
     * @param arr1 An array-like object. It must be sorted.
     * @param arr2 An array-like object. It must be sorted.
     *
     * @return An array with the set difference of the two arrays.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    array<T> set_difference(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    );

    /**
     * @brief Return a new array with the unique, sorted elements that are
     * present in one of the two arrays, but not in both.
     *
     * @param arr1 An array-like object. It must be sorted.
     * @param arr2 An array-like object. It must be sorted.
     *
     * @return An array with the set symmetric difference of the two arrays.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    array<T> set_symmetric_difference(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    );
}

#include "numcpp/routines/routines.tcc"

#endif // NUMCPP_ROUTINES_H_INCLUDED
