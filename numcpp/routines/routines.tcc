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

 /** @file include/numcpp/routines/routines.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ROUTINES_TCC_INCLUDED
#define NUMCPP_ROUTINES_TCC_INCLUDED

#include <cstdio>
#include <stdexcept>
#include <vector>

namespace numcpp {
    /// Array and matrix creation routines

    template <class T>
    array<T> empty(size_t n) {
        return array<T>(n);
    }

    template <class T>
    matrix<T> empty(size_t m, size_t n) {
        return matrix<T>(m, n);
    }

    template <class T, class Tag>
    array<T> empty_like(const base_array<T, Tag> &arr) {
        return array<T>(arr.size());
    }

    template <class T, class Tag>
    matrix<T> empty_like(const base_matrix<T, Tag> &mat) {
        return matrix<T>(mat.rows(), mat.cols());
    }

    template <class T>
    array<T> zeros(size_t n) {
        return array<T>(n, T(0));
    }

    template <class T>
    matrix<T> zeros(size_t m, size_t n) {
        return matrix<T>(m, n, T(0));
    }

    template <class T, class Tag>
    array<T> zeros_like(const base_array<T, Tag> &arr) {
        return array<T>(arr.size(), T(0));
    }

    template <class T, class Tag>
    matrix<T> zeros_like(const base_matrix<T, Tag> &mat) {
        return matrix<T>(mat.rows(), mat.cols(), T(0));
    }

    template <class T>
    array<T> ones(size_t n) {
        return array<T>(n, T(1));
    }

    template <class T>
    matrix<T> ones(size_t m, size_t n) {
        return matrix<T>(m, n, T(1));
    }

    template <class T, class Tag>
    array<T> ones_like(const base_array<T, Tag> &arr) {
        return array<T>(arr.size(), T(1));
    }

    template <class T, class Tag>
    matrix<T> ones_like(const base_matrix<T, Tag> &mat) {
        return matrix<T>(mat.rows(), mat.cols(), T(1));
    }

    template <class T>
    array<T> full(size_t n, const T &val) {
        return array<T>(n, val);
    }

    template <class T>
    matrix<T> full(size_t m, size_t n, const T &val) {
        return matrix<T>(m, n, val);
    }

    template <class T, class Tag>
    array<T> full_like(
        const base_array<T, Tag> &arr,
        const typename base_array<T, Tag>::value_type &val
    ) {
        return array<T>(arr.size(), val);
    }

    template <class T, class Tag>
    matrix<T> full_like(
        const base_matrix<T, Tag> &mat,
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        return matrix<T>(mat.rows(), mat.cols(), val);
    }

    /// Numerical ranges

    template <class T>
    base_array<T, sequence_tag> arange(const T &stop) {
        size_t size = 0;
        if (stop > 0) {
            size = std::ceil(stop);
        }
        return base_array<T, sequence_tag>(0, size, 1);
    }

    template <class T>
    base_array<T, sequence_tag> arange(const T &start, const T &stop) {
        size_t size = 0;
        if (start < stop) {
            size = std::ceil(stop - start);
        }
        return base_array<T, sequence_tag>(start, size, 1);
    }

    template <class T>
    base_array<T, sequence_tag> arange(
        const T &start, const T &stop, const T &step
    ) {
        size_t size = 0;
        if ((start < stop && step > 0) || (start > stop && step < 0)) {
            size = std::ceil((stop - start) / (double)step);
        }
        return base_array<T, sequence_tag>(start, size, step);
    }

    template <class T>
    base_array<T, sequence_tag> linspace(
        const T &start, const T &stop, size_t num, bool endpoint
    ) {
        T step = stop - start;
        step /= num - endpoint;
        return base_array<T, sequence_tag>(start, num, step);
    }

    template <class T>
    base_array<T, log_sequence_tag> logspace(
        const T &start, const T &stop, size_t num, bool endpoint, const T &base
    ) {
        T step = stop - start;
        step /= num - endpoint;
        return base_array<T, log_sequence_tag>(start, num, step, base);
    }

    template <class T>
    base_array<T, log_sequence_tag> geomspace(
        const T &start, const T &stop, size_t num, bool endpoint
    ) {
        T logstart = std::log10(start);
        T logstop = std::log10(stop);
        T logstep = logstop - logstart;
        logstep /= num - endpoint;
        return base_array<T, log_sequence_tag>(logstart, num, logstep, 10);
    }

    /// Building matrices

    template <class T, class Tag>
    base_matrix< T, diagonal_tag<Tag> > diagonal(
        const base_array<T, Tag> &arr, ptrdiff_t offset
    ) {
        return base_matrix< T, diagonal_tag<Tag> >(arr, offset);
    }

    template <class T, class Tag>
    base_array< T, diagonal_tag<Tag> > diagonal(
        const base_matrix<T, Tag> &mat, ptrdiff_t offset
    ) {
        return base_array< T, diagonal_tag<Tag> >(mat, offset);
    }

    template <class T>
    base_matrix<T, eye_tag> eye(size_t n) {
        return base_matrix<T, eye_tag>(n, n, 0);
    }

    template <class T>
    base_matrix<T, eye_tag> eye(size_t m, size_t n, ptrdiff_t offset) {
        return base_matrix<T, eye_tag>(m, n, offset);
    }

    template <class T, class Tag>
    base_matrix< T, triangular_tag<Tag> >
    tril(const base_matrix<T, Tag> &mat, ptrdiff_t offset) {
        typedef triangular_tag<Tag> Closure;
        return base_matrix<T, Closure>(mat, true, offset);
    }

    template <class T, class Tag>
    base_matrix< T, triangular_tag<Tag> >
    triu(const base_matrix<T, Tag> &mat, ptrdiff_t offset) {
        typedef triangular_tag<Tag> Closure;
        return base_matrix<T, Closure>(mat, false, offset);
    }

    /// Maximums and minimums

    template <class T, class Tag>
    size_t argmax(const base_array<T, Tag> &arr) {
        __range_argmax pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    index_t argmax(const base_matrix<T, Tag> &mat) {
        __range_argmax pred;
        size_t index = pred(mat.begin(true), mat.end(true));
        return index_t(index / mat.cols(), index % mat.cols());
    }

    template <class T, class Tag>
    base_array< size_t, lazy_axis_tag<__range_argmax, T, Tag> >
    argmax(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_argmax, T, Tag> Closure;
        return base_array<size_t, Closure>(__range_argmax(), mat, rowwise);
    }

    template <class T, class Tag>
    size_t argmin(const base_array<T, Tag> &arr) {
        __range_argmin pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    index_t argmin(const base_matrix<T, Tag> &mat) {
        __range_argmin pred;
        size_t index = pred(mat.begin(true), mat.end(true));
        return index_t(index / mat.cols(), index % mat.cols());
    }

    template <class T, class Tag>
    base_array< size_t, lazy_axis_tag<__range_argmin, T, Tag> >
    argmin(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_argmin, T, Tag> Closure;
        return base_array<size_t, Closure>(__range_argmin(), mat, rowwise);
    }

    template <class T, class Tag>
    T amax(const base_array<T, Tag> &arr) {
        __range_max pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T amax(const base_matrix<T, Tag> &mat) {
        __range_max pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_max, T, Tag> >
    amax(const base_matrix<T, Tag> &mat, size_t rowwise) {
        typedef lazy_axis_tag<__range_max, T, Tag> Closure;
        return base_array<T, Closure>(__range_max(), mat, rowwise);
    }

    template <class T, class Tag>
    T amin(const base_array<T, Tag> &arr) {
        __range_min pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T amin(const base_matrix<T, Tag> &mat) {
        __range_min pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_min, T, Tag> >
    amin(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_min, T, Tag> Closure;
        return base_array<T, Closure>(__range_min(), mat, rowwise);
    }

    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_max, T, Tag1, T, Tag2> >
    maximum(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__math_max, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_max(), lhs, rhs);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_max, T, Tag, T, scalar_tag> >
    maximum(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__math_max, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_max(), lhs, val);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_max, T, scalar_tag, T, Tag> >
    maximum(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__math_max, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_max(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_max, T, Tag1, T, Tag2> >
    maximum(const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__math_max, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_max(), lhs, rhs);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_max, T, Tag, T, scalar_tag> >
    maximum(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__math_max, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_max(), lhs, val);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_max, T, scalar_tag, T, Tag> >
    maximum(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__math_max, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_max(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_min, T, Tag1, T, Tag2> >
    minimum(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__math_min, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_min(), lhs, rhs);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_min, T, Tag, T, scalar_tag> >
    minimum(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__math_min, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_min(), lhs, val);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_min, T, scalar_tag, T, Tag> >
    minimum(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__math_min, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_min(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_min, T, Tag1, T, Tag2> >
    minimum(const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__math_min, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_min(), lhs, rhs);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_min, T, Tag, T, scalar_tag> >
    minimum(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__math_min, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_min(), lhs, val);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_min, T, scalar_tag, T, Tag> >
    minimum(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__math_min, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_min(), val, rhs);
    }

    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__clamp<T>, T, Tag> >
    clamp(
        const base_array<T, Tag> &arr,
        const typename base_array<T, Tag>::value_type &a_min,
        const typename base_array<T, Tag>::value_type &a_max
    ) {
        typedef lazy_unary_tag<__clamp<T>, T, Tag> Closure;
        return base_array<T, Closure>(__clamp<T>(a_min, a_max), arr);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__clamp<T>, T, Tag> >
    clamp(
        const base_matrix<T, Tag> &mat,
        const typename base_matrix<T, Tag>::value_type &a_min,
        const typename base_matrix<T, Tag>::value_type &a_max
    ) {
        typedef lazy_unary_tag<__clamp<T>, T, Tag> Closure;
        return base_matrix<T, Closure>(__clamp<T>(a_min, a_max), mat);
    }

    /// Sums and products

    template <class T, class Tag>
    T sum(const base_array<T, Tag> &arr) {
        __range_sum pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T sum(const base_matrix<T, Tag> &mat) {
        __range_sum pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_sum, T, Tag> >
    sum(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_sum, T, Tag> Closure;
        return base_array<T, Closure>(__range_sum(), mat, rowwise);
    }

    template <class T, class Tag>
    T prod(const base_array<T, Tag> &arr) {
        __range_prod pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T prod(const base_matrix<T, Tag> &mat) {
        __range_prod pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_prod, T, Tag> >
    prod(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_prod, T, Tag> Closure;
        return base_array<T, Closure>(__range_prod(), mat, rowwise);
    }

    template <class T, class Tag>
    array<T> cumsum(const base_array<T, Tag> &arr) {
        array<T> out(arr.size());
        __accumulate(arr.begin(), arr.end(), out.begin(), __plus());
        return out;
    }

    template <class T, class Tag>
    matrix<T> cumsum(const base_matrix<T, Tag> &mat, bool rowwise) {
        matrix<T> out(mat.rows(), mat.cols());
        size_t size = rowwise ? mat.rows() : mat.cols();
        size_t tda = rowwise ? mat.cols() : mat.rows();
        for (size_t i = 0; i < size; ++i) {
            __accumulate(
                mat.begin(rowwise) + i*tda, mat.begin(rowwise) + (i + 1)*tda,
                out.begin(rowwise) + i*tda,
                __plus()
            );
        }
        return out;
    }

    template <class T, class Tag>
    array<T> cumprod(const base_array<T, Tag> &arr) {
        array<T> out(arr.size());
        __accumulate(arr.begin(), arr.end(), out.begin(), __multiplies());
        return out;
    }

    template <class T, class Tag>
    matrix<T> cumprod(const base_matrix<T, Tag> &mat, bool rowwise) {
        matrix<T> out(mat.rows(), mat.cols());
        size_t size = rowwise ? mat.rows() : mat.cols();
        size_t tda = rowwise ? mat.cols() : mat.rows();
        for (size_t i = 0; i < size; ++i) {
            __accumulate(
                mat.begin(rowwise) + i*tda, mat.begin(rowwise) + (i + 1)*tda,
                out.begin(rowwise) + i*tda,
                __multiplies()
            );
        }
        return out;
    }

    /// Concatenation

    /// Helper function: Return the size of the concatenation.
    template <class T, class Tag, class... Arrays>
    size_t __concatenate_size(
        const base_array<T, Tag> &arr1, const Arrays&... arr2
    ) {
        return arr1.size() + __concatenate_size(arr2...);
    }

    template <class T, class Tag>
    size_t __concatenate_size(const base_array<T, Tag> &arr) {
        return arr.size();
    }

    /// Helper function: Fill an array with the concatenation of the arguments.
    template <class T, class Tag, class... Arrays>
    void __concatenate_fill(
        array<T> &out, size_t offset,
        const base_array<T, Tag> &arr1, const Arrays&... arr2
    ) {
        out[slice(offset, arr1.size())] = arr1;
        __concatenate_fill(out, offset + arr1.size(), arr2...);
    }

    template <class T, class Tag>
    void __concatenate_fill(
        array<T> &out, size_t offset, const base_array<T, Tag> &arr
    ) {
        out[slice(offset, arr.size())] = arr;
    }

    template <class T, class Tag, class... Arrays>
    array<T> concatenate(
        const base_array<T, Tag> &arr1, const Arrays&... arr2
    ) {
        array<T> out(__concatenate_size(arr1, arr2...));
        __concatenate_fill(out, 0, arr1, arr2...);
        return out;
    }

    /// Helper function: Assert number of columns is the same in row_stack.
    void __vstack_assert_shape(size_t cols1, size_t cols2) {
        if (cols1 != cols2) {
            char error[100];
            sprintf(
                error, "Number of columns is not the same in row stack: "
                "(?,%zu) (?,%zu)", cols1, cols2
            );
            throw std::invalid_argument(error);
        }
    }

    /// Helper function: Return the number of rows in the vertical
    /// concatenation.
    template <class T, class Tag, class... Matrices>
    size_t __vstack_rows(
        size_t cols, const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        __vstack_assert_shape(cols, mat1.cols());
        return mat1.rows() + __vstack_rows(cols, mat2...);
    }

    template <class T, class Tag, class... Matrices>
    size_t __vstack_rows(
        size_t cols, const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        __vstack_assert_shape(cols, arr1.size());
        return 1 + __vstack_rows(cols, mat2...);
    }

    template <class T, class Tag>
    size_t __vstack_rows(size_t cols, const base_matrix<T, Tag> &mat) {
        __vstack_assert_shape(cols, mat.cols());
        return mat.rows();
    }

    template <class T, class Tag>
    size_t __vstack_rows(size_t cols, const base_array<T, Tag> &arr) {
        __vstack_assert_shape(cols, arr.size());
        return 1;
    }

    /// Helper function: Fill a matrix with the vertical concatenation of the
    /// arguments.
    template <class T, class Tag, class... Matrices>
    void __vstack_fill(
        matrix<T> &out, size_t offset,
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        out(slice(offset, mat1.rows()), slice(mat1.cols())) = mat1;
        __vstack_fill(out, offset + mat1.rows(), mat2...);
    }

    template <class T, class Tag, class... Matrices>
    void __vstack_fill(
        matrix<T> &out, size_t offset,
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        out(offset, slice(arr1.size())) = arr1;
        __vstack_fill(out, offset + 1, mat2...);
    }

    template <class T, class Tag>
    void __vstack_fill(
        matrix<T> &out, size_t offset, const base_matrix<T, Tag> &mat
    ) {
        out(slice(offset, mat.rows()), slice(mat.cols())) = mat;
    }

    template <class T, class Tag>
    void __vstack_fill(
        matrix<T> &out, size_t offset, const base_array<T, Tag> &arr
    ) {
        out(offset, slice(arr.size())) = arr;
    }

    template <class T, class Tag, class... Matrices>
    matrix<T> row_stack(
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        matrix<T> out(__vstack_rows(mat1.cols(), mat1, mat2...), mat1.cols());
        __vstack_fill(out, 0, mat1, mat2...);
        return out;
    }

    template <class T, class Tag, class... Matrices>
    matrix<T> row_stack(
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        matrix<T> out(__vstack_rows(arr1.size(), arr1, mat2...), arr1.size());
        __vstack_fill(out, 0, arr1, mat2...);
        return out;
    }

    /// Helper function: Assert number of rows is the same in column_stack.
    void __hstack_assert_shape(size_t rows1, size_t rows2) {
        if (rows1 != rows2) {
            char error[100];
            sprintf(
                error, "Number of rows is not the same in column stack: "
                "(%zu,?) (%zu,?)", rows1, rows2
            );
            throw std::invalid_argument(error);
        }
    }

    /// Helper function: Return the number of columns in the horizontal
    /// concatenation.
    template <class T, class Tag, class... Matrices>
    size_t __hstack_cols(
        size_t rows, const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        __hstack_assert_shape(rows, mat1.rows());
        return mat1.cols() + __hstack_cols(rows, mat2...);
    }

    template <class T, class Tag, class... Matrices>
    size_t __hstack_cols(
        size_t rows, const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        __hstack_assert_shape(rows, arr1.size());
        return 1 + __hstack_cols(rows, mat2...);
    }

    template <class T, class Tag>
    size_t __hstack_cols(size_t rows, const base_matrix<T, Tag> &mat) {
        __hstack_assert_shape(rows, mat.rows());
        return mat.cols();
    }

    template <class T, class Tag>
    size_t __hstack_cols(size_t rows, const base_array<T, Tag> &arr) {
        __hstack_assert_shape(rows, arr.size());
        return 1;
    }

    /// Helper function: Fill a matrix with the horizontal concatenation of the
    /// arguments.
    template <class T, class Tag, class... Matrices>
    void __hstack_fill(
        matrix<T> &out, size_t offset,
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        out(slice(mat1.rows()), slice(offset, mat1.cols())) = mat1;
        __hstack_fill(out, offset + mat1.cols(), mat2...);
    }

    template <class T, class Tag, class... Matrices>
    void __hstack_fill(
        matrix<T> &out, size_t offset,
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        out(slice(arr1.size()), offset) = arr1;
        __hstack_fill(out, offset + 1, mat2...);
    }

    template <class T, class Tag>
    void __hstack_fill(
        matrix<T> &out, size_t offset, const base_matrix<T, Tag> &mat
    ) {
        out(slice(mat.rows()), slice(offset, mat.cols())) = mat;
    }

    template <class T, class Tag>
    void __hstack_fill(
        matrix<T> &out, size_t offset, const base_array<T, Tag> &arr
    ) {
        out(slice(arr.size()), offset) = arr;
    }

    template <class T, class Tag, class... Matrices>
    matrix<T> column_stack(
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        matrix<T> out(mat1.rows(), __hstack_cols(mat1.rows(), mat1, mat2...));
        __hstack_fill(out, 0, mat1, mat2...);
        return out;
    }

    template <class T, class Tag, class... Matrices>
    matrix<T> column_stack(
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        matrix<T> out(arr1.size(), __hstack_cols(arr1.size(), arr1, mat2...));
        __hstack_fill(out, 0, arr1, mat2...);
        return out;
    }

    /// Padding

    /// Helper function: Pads an array with a constant value.
    template <class T>
    void __pad_constant(
        array_view<T> view, size_t before, size_t after,
        const T &const_before, const T &const_after
    ) {
        for (size_t i = 0; i < before; ++i) {
            view[i] = const_before;
        }
        for (size_t i = 0; i < after; ++i) {
            view[view.size() - 1 - i] = const_after;
        }
    }

    /// Helper function: Pads an array with the edge values.
    template <class T>
    void __pad_edge(array_view<T> view, size_t before, size_t after) {
        T edge_before = view[before];
        T edge_after = view[view.size() - 1 - after];
        __pad_constant(view, before, after, edge_before, edge_after);
    }

    /// Helper function: Pads an array with the linear ramp between the edge
    /// values and an end value.
    template <class T>
    void __pad_linear_ramp(
        array_view<T> view, size_t before, size_t after,
        const T &end_before, const T &end_after
    ) {
        T edge_before = view[before];
        T edge_after = view[view.size() - 1 - after];
        for (size_t i = 0; i < before; ++i) {
            double t = (double)i/before;
            view[i] = (1 - t)*end_before + t*edge_before;
        }
        for (size_t i = 0; i < after; ++i) {
            double t = (double)i/after;
            view[view.size() - 1 - i] = (1 - t)*end_after + t*edge_after;
        }
    }

    /// Helper function: Pads an array with the reflection of the array
    /// mirrored on the first and last values.
    template <class T>
    void __pad_reflect(array_view<T> view, size_t before, size_t after) {
        size_t size = view.size() - before - after;
        for (size_t i = 0; i < before; ++i) {
            size_t idx = (before - 1 - i) % (size - 1);
            size_t nblock = (before - 1 - i) / (size - 1);
            size_t sorted = before + size - 2 - idx;
            size_t reversed = before + 1 + idx;
            view[i] = (nblock % 2 == 0) ? view[reversed] : view[sorted];
        }
        for (size_t i = 0; i < after; ++i) {
            size_t idx = (after - 1 - i) % (size - 1);
            size_t nblock = (after - 1 - i) / (size - 1);
            size_t sorted = before + size - 2 - idx;
            size_t reversed = before + 1 + idx;
            view[view.size() - 1 - i] = (nblock % 2 == 0) ? view[sorted]
                                                          : view[reversed];
        }
    }

    /// Helper function: Pads an array with the reflection of the array
    /// mirrored along the edge.
    template <class T>
    void __pad_symmetric(array_view<T> view, size_t before, size_t after) {
        size_t size = view.size() - before - after;
        for (size_t i = 0; i < before; ++i) {
            size_t idx = (before - 1 - i) % size ;
            size_t nblock = (before - 1 - i) / size ;
            size_t sorted = before + size - 1 - idx;
            size_t reversed = before + idx;
            view[i] = (nblock % 2 == 0) ? view[reversed] : view[sorted];
        }
        for (size_t i = 0; i < after; ++i) {
            size_t idx = (after - 1 - i) % size;
            size_t nblock = (after - 1 - i) / size;
            size_t sorted = before + size - 1 - idx;
            size_t reversed = before + idx;
            view[view.size() - 1 - i] = (nblock % 2 == 0) ? view[sorted]
                                                          : view[reversed];
        }
    }

    /// Helper function: Pads an array with the wrap of the array.
    template <class T>
    void __pad_wrap(array_view<T> view, size_t before, size_t after) {
        size_t size = view.size() - before - after;
        for (size_t i = 0; i < before; ++i) {
            size_t idx = before + size - 1 - (before - 1 - i) % size;
            view[i] = view[idx];
        }
        for (size_t i = 0; i < after; ++i) {
            size_t idx = before + (after - 1 - i) % size;
            view[view.size() - 1 - i] = view[idx];
        }
    }

    template <class T, class Tag>
    array<T> pad(
        const base_array<T, Tag> &arr, size_t before, size_t after,
        const std::string &mode,
        std::initializer_list<typename base_array<T, Tag>::value_type> args
    ) {
        size_t n = before + arr.size() + after;
        array<T> out(n);
        out[slice(before, arr.size())] = arr;
        T val_before = T(), val_after = T();
        if (args.size() >= 2) {
            val_before = *args.begin();
            val_after = *(args.begin() + 1);
        }
        else if (args.size() >= 1) {
            val_before = *args.begin();
            val_after = *args.begin();
        }
        if (mode == "constant") {
            __pad_constant(out.view(), before, after, val_before, val_after);
        }
        else if (mode == "edge") {
            __pad_edge(out.view(), before, after);
        }
        else if (mode == "linear_ramp") {
            __pad_linear_ramp(out.view(), before, after, val_before, val_after);
        }
        else if (mode == "reflect") {
            __pad_reflect(out.view(), before, after);
        }
        else if (mode == "symmetric") {
            __pad_symmetric(out.view(), before, after);
        }
        else if (mode == "wrap") {
            __pad_wrap(out.view(), before, after);
        }
        else if (mode != "empty") {
            char error[] = "mode must be one of \"empty\", \"constant\", "
            "\"edge\", \"linear_ramp\", \"reflect\", \"symmetric\" or \"wrap\"";
            throw std::invalid_argument(error);
        }
        return out;
    }

    template <class T, class Tag>
    matrix<T> pad(
        const base_matrix<T, Tag> &mat,
        size_t before1, size_t after1, size_t before2, size_t after2,
        const std::string &mode,
        std::initializer_list<typename base_matrix<T, Tag>::value_type> args
    ) {
        size_t m = before1 + mat.rows() + after1;
        size_t n = before2 + mat.cols() + after2;
        matrix<T> out(m, n);
        out(slice(before1, mat.rows()), slice(before2, mat.cols())) = mat;
        T val_before1 = T(), val_after1 = T();
        T val_before2 = T(), val_after2 = T();
        if (args.size() >= 4) {
            val_before1 = *args.begin();
            val_after1 = *(args.begin() + 1);
            val_before2 = *(args.begin() + 2);
            val_after2 = *(args.begin() + 3);
        }
        else if (args.size() >= 2) {
            val_before1 = *args.begin();
            val_after1 = *args.begin();
            val_before2 = *(args.begin() + 1);
            val_after2 = *(args.begin() + 1);
        }
        else if (args.size() >= 1) {
            val_before1 = *args.begin();
            val_after1 = *args.begin();
            val_before2 = *args.begin();
            val_after2 = *args.begin();
        }
        for (size_t j = before2; j < n - after2; ++j) {
            array_view<T> column = out(slice(m), j);
            if (mode == "constant") {
                __pad_constant(column, before1,after1, val_before1,val_after1);
            }
            else if (mode == "edge") {
                __pad_edge(column, before1, after1);
            }
            else if (mode == "linear_ramp") {
                __pad_linear_ramp(column,before1,after1,val_before1,val_after1);
            }
            else if (mode == "reflect") {
                __pad_reflect(column, before1, after1);
            }
            else if (mode == "symmetric") {
                __pad_symmetric(column, before1, after1);
            }
            else if (mode == "wrap") {
                __pad_wrap(column, before1, after1);
            }
            else if (mode != "empty") {
                char error[] = "mode must be one of \"empty\", \"constant\", "
                "\"edge\", \"linear_ramp\", \"reflect\", \"symmetric\" or "
                "\"wrap\"";
                throw std::invalid_argument(error);
            }
        }
        for (size_t i = 0; i < m; ++i) {
            array_view<T> row = out(i, slice(n));
            if (mode == "constant") {
                __pad_constant(row, before2, after2, val_before2, val_after2);
            }
            else if (mode == "edge") {
                __pad_edge(row, before2, after2);
            }
            else if (mode == "linear_ramp") {
                __pad_linear_ramp(row, before2,after2, val_before2,val_after2);
            }
            else if (mode == "reflect") {
                __pad_reflect(row, before2, after2);
            }
            else if (mode == "symmetric") {
                __pad_symmetric(row, before2, after2);
            }
            else if (mode == "wrap") {
                __pad_wrap(row, before2, after2);
            }
            else if (mode != "empty") {
                char error[] = "mode must be one of \"empty\", \"constant\", "
                "\"edge\", \"linear_ramp\", \"reflect\", \"symmetric\" or "
                "\"wrap\"";
                throw std::invalid_argument(error);
            }
        }
        return out;
    }

    /// Insertion-deletion

    template <class T, class Tag>
    array<T> insert(
        const base_array<T, Tag> &arr,
        size_t index, const typename base_array<T, Tag>::value_type &value
    ) {
        array<T> out(arr.size() + 1);
        for (size_t i = 0; i < index; ++i) {
            out[i] = arr[i];
        }
        out[index] = value;
        for (size_t i = index + 1; i < out.size(); ++i) {
            out[i] = arr[i - 1];
        }
        return out;
    }

    template <class T, class Tag, class IndexTag, class ValueTag>
    array<T> insert(
        const base_array<T, Tag> &arr,
        const base_array<size_t, IndexTag> &index,
        const base_array<T, ValueTag> &value
    ) {
        __assert_equal_length(index.size(), value.size());
        array<T> out(arr.size() + index.size());
        array<size_t> sorted = argsort(index);
        size_t i = 0, j = 0;
        for (size_t n = 0; n < out.size(); ++n) {
            if (i >= arr.size()) {
                out[n] = value[sorted[j++]];
            }
            else if (j >= index.size()) {
                out[n] = arr[i++];
            }
            else {
                out[n] = (i < index[sorted[j]]) ? arr[i++] : value[sorted[j++]];
            }
        }
        return out;
    }

    template <class T, class Tag>
    array<T> erase(const base_array<T, Tag> &arr, size_t index) {
        array<T> out(arr.size() - 1);
        for (size_t i = 0; i < index; ++i) {
            out[i] = arr[i];
        }
        for (size_t i = index + 1; i < arr.size(); ++i) {
            out[i - 1] = arr[i];
        }
        return out;
    }

    template <class T, class Tag, class IndexTag>
    array<T> erase(
        const base_array<T, Tag> &arr,
        const base_array<size_t, IndexTag> &index
    ) {
        array<bool> keep(arr.size(), true);
        for (size_t i = 0; i < index.size(); ++i) {
            keep[index[i]] = false;
        }
        array<T> out(arr.size() - index.size());
        size_t n = 0;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (keep[i]) {
                out[n++] = arr[i];
            }
        }
        return out;
    }

    /// Logic functions

    template <class Tag>
    bool all(const base_array<bool, Tag> &arr) {
        __range_all pred;
        return pred(arr.begin(), arr.end());
    }

    template <class Tag>
    bool all(const base_matrix<bool, Tag> &mat) {
        __range_all pred;
        return pred(mat.begin(), mat.end());
    }

    template <class Tag>
    base_array< bool, lazy_axis_tag<__range_all, bool, Tag> >
    all(const base_matrix<bool, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_all, bool, Tag> Closure;
        return base_array<bool, Closure>(__range_all(), mat, rowwise);
    }

    template <class Tag>
    bool any(const base_array<bool, Tag> &arr) {
        __range_any pred;
        return pred(arr.begin(), arr.end());
    }

    template <class Tag>
    bool any(const base_matrix<bool, Tag> &mat) {
        __range_any pred;
        return pred(mat.begin(), mat.end());
    }

    template <class Tag>
    base_array< bool, lazy_axis_tag<__range_any, bool, Tag> >
    any(const base_matrix<bool, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_any, bool, Tag> Closure;
        return base_array<bool, Closure>(__range_any(), mat, rowwise);
    }

    template <class T, class Tag>
    size_t count_nonzero(const base_array<T, Tag> &arr) {
        __range_count_nonzero pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    size_t count_nonzero(const base_matrix<T, Tag> &mat) {
        __range_count_nonzero pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< size_t, lazy_axis_tag<__range_count_nonzero, T, Tag> >
    count_nonzero(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_count_nonzero, T, Tag> Closure;
        return base_array<size_t, Closure>(
            __range_count_nonzero(), mat, rowwise
        );
    }

    bool isclose(double a, double b, double rtol, double atol) {
        __isclose<double> pred(rtol, atol);
        return pred(a, b);
    }

    bool isclose(float a, float b, float rtol, float atol) {
        __isclose<float> pred(rtol, atol);
        return pred(a, b);
    }

    bool isclose(
        long double a, long double b, long double rtol, long double atol
    ) {
        __isclose<long double> pred(rtol, atol);
        return pred(a, b);
    }

    template <class T>
    bool isclose(
        const std::complex<T> &a, const std::complex<T> &b, T rtol, T atol
    ) {
        __isclose< std::complex<T> > pred(rtol, atol);
        return pred(a, b);
    }

    template <class T, class Tag1, class Tag2>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> >
    isclose(
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__isclose<T>(rtol, atol), lhs, rhs);
    }

    template <class T, class Tag>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> >
    isclose(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__isclose<T>(rtol, atol), lhs, val);
    }

    template <class T, class Tag>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> >
    isclose(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__isclose<T>(rtol, atol), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> >
    isclose(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__isclose<T>(rtol, atol), lhs, rhs);
    }

    template <class T, class Tag>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> >
    isclose(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__isclose<T>(rtol, atol), lhs, val);
    }

    template <class T, class Tag>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> >
    isclose(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__isclose<T>(rtol, atol), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    bool allclose(
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        return all(isclose(lhs, rhs, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        return all(isclose(lhs, val, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        return all(isclose(val, rhs, rtol, atol));
    }

    template <class T, class Tag1, class Tag2>
    bool allclose(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        return all(isclose(lhs, rhs, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const base_matrix<T, Tag> &lhs,
        const typename base_matrix<T, Tag>::value_type &val,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        return all(isclose(lhs, val, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const typename base_matrix<T, Tag>::value_type &val,
        const base_matrix<T, Tag> &rhs,
        typename complex_traits<T>::value_type rtol,
        typename complex_traits<T>::value_type atol
    ) {
        return all(isclose(val, rhs, rtol, atol));
    }

    /// Sorting and searching

    template <class T, class Tag>
    array<size_t> argsort(const base_array<T, Tag> &arr) {
        return argsort(arr, __less());
    }

    template <class T, class Tag, class Compare>
    array<size_t> argsort(
        const base_array<T, Tag> &arr, Compare comp, bool stable
    ) {
        array<size_t> index(arr.size());
        __iota(index.begin(), index.end(), 0, 1);
        auto comparator = [&arr, comp](size_t i, size_t j) {
            return comp(arr[i], arr[j]);
        };
        if (stable) {
            std::stable_sort(index.begin(), index.end(), comparator);
        }
        else {
            std::sort(index.begin(), index.end(), comparator);
        }
        return index;
    }

    template <class T, class Tag>
    array<index_t> argsort(const base_matrix<T, Tag> &mat) {
        return argsort(mat, __less());
    }

    template <class T, class Tag, class Compare>
    array<index_t> argsort(
        const base_matrix<T, Tag> &mat, Compare comp, bool stable
    ) {
        array<index_t> index(mat.size());
        size_t n = 0;
        for (size_t i = 0; i < mat.rows(); ++i) {
            for (size_t j = 0; j < mat.cols(); ++j) {
                index[n++] = index_t(i, j);
            }
        }
        auto comparator = [&mat, comp](index_t i, index_t j) {
            return comp(mat[i], mat[j]);
        };
        if (stable) {
            std::stable_sort(index.begin(), index.end(), comparator);
        }
        else {
            std::sort(index.begin(), index.end(), comparator);
        }
        return index;
    }

    template <class T, class Tag>
    matrix<size_t> argsort(const base_matrix<T, Tag> &mat, bool rowwise) {
        return argsort(mat, rowwise, __less());
    }

    template <class T, class Tag, class Compare>
    matrix<size_t> argsort(
        const base_matrix<T, Tag> &mat, bool rowwise, Compare comp, bool stable
    ) {
        typedef typename matrix<size_t>::iterator iterator;
        matrix<size_t> out(mat.rows(), mat.cols());
        size_t size = rowwise ? mat.rows() : mat.cols();
        size_t tda = rowwise ? mat.cols() : mat.rows();
        for (size_t k = 0; k < size; ++k) {
            iterator first = out.begin(rowwise) + k*tda;
            iterator last = out.begin(rowwise) + (k + 1)*tda;
            __iota(first, last, 0, 1);
            auto comparator = [&mat, rowwise, comp, k](size_t i, size_t j) {
                return rowwise ? comp(mat(k, i), mat(k, j))
                               : comp(mat(i, k), mat(j, k));
            };
            if (stable) {
                std::stable_sort(first, last, comparator);
            }
            else {
                std::sort(first, last, comparator);
            }
        }
        return out;
    }

    template <class T, class Tag>
    array<T> sort(const base_array<T, Tag> &arr) {
        array<T> out(arr);
        out.sort();
        return out;
    }

    template <class T, class Tag, class Compare>
    array<T> sort(
        const base_array<T, Tag> &arr, Compare comp, bool stable
    ) {
        array<T> out(arr);
        out.sort(comp, stable);
        return out;
    }

    template <class T, class Tag>
    array<T> sort(const base_matrix<T, Tag> &mat) {
        array<T> out(mat.begin(), mat.end());
        out.sort();
        return out;
    }

    template <class T, class Tag, class Compare>
    array<T> sort(
        const base_matrix<T, Tag> &mat, Compare comp, bool stable
    ) {
        array<T> out(mat.begin(), mat.end());
        out.sort(comp, stable);
        return out;
    }

    template <class T, class Tag>
    matrix<T> sort(const base_matrix<T, Tag> &mat, bool rowwise) {
        matrix<T> out(mat);
        out.sort(rowwise);
        return out;
    }

    template <class T, class Tag, class Compare>
    matrix<T> sort(
        const base_matrix<T, Tag> &mat, bool rowwise, Compare comp, bool stable
    ) {
        matrix<T> out(mat);
        out.sort(rowwise, comp, stable);
        return out;
    }

    template <class T, class Tag>
    array<size_t> argpartition(const base_array<T, Tag> &arr, size_t kth) {
        return argpartition(arr, kth, __less());
    }

    template <class T, class Tag, class Compare>
    array<size_t> argpartition(
        const base_array<T, Tag> &arr, size_t kth, Compare comp
    ) {
        __assert_within_bounds(arr.size(), kth);
        array<size_t> index(arr.size());
        __iota(index.begin(), index.end(), 0, 1);
        auto comparator = [&arr, comp](size_t i, size_t j) {
            return comp(arr[i], arr[j]);
        };
        std::nth_element(
            index.begin(), index.begin() + kth, index.end(), comparator
        );
        return index;
    }

    template <class T, class Tag>
    array<index_t> argpartition(const base_matrix<T, Tag> &mat, size_t kth) {
        return argpartition(mat, kth, __less());
    }

    template <class T, class Tag, class Compare>
    array<index_t> argpartition(
        const base_matrix<T, Tag> &mat, size_t kth, Compare comp
    ) {
        __assert_within_bounds(mat.size(), kth);
        array<index_t> index(mat.size());
        size_t n = 0;
        for (size_t i = 0; i < mat.rows(); ++i) {
            for (size_t j = 0; j < mat.cols(); ++j) {
                index[n++] = index_t(i, j);
            }
        }
        auto comparator = [&mat, comp](index_t i, index_t j) {
            return comp(mat[i], mat[j]);
        };
        std::nth_element(
            index.begin(), index.begin() + kth, index.end(), comparator
        );
        return index;
    }

    template <class T, class Tag>
    matrix<size_t> argpartition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise
    ) {
        return argpartition(mat, kth, rowwise, __less());
    }

    template <class T, class Tag, class Compare>
    matrix<size_t> argpartition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise, Compare comp
    ) {
        typedef typename matrix<size_t>::iterator iterator;
        size_t size = rowwise ? mat.rows() : mat.cols();
        size_t tda = rowwise ? mat.cols() : mat.rows();
        __assert_within_bounds(tda, kth);
        matrix<size_t> out(mat.rows(), mat.cols());
        for (size_t k = 0; k < size; ++k) {
            iterator first = out.begin(rowwise) + k*tda;
            iterator last = out.begin(rowwise) + (k + 1)*tda;
            __iota(first, last, 0, 1);
            auto comparator = [&mat, rowwise, comp, k](size_t i, size_t j) {
                return rowwise ? comp(mat(k, i), mat(k, j))
                               : comp(mat(i, k), mat(j, k));
            };
            std::nth_element(first, first + kth, last, comparator);
        }
        return out;
    }

    template <class T, class Tag>
    array<T> partition(const base_array<T, Tag> &arr, size_t kth) {
        array<T> out(arr);
        out.partition(kth);
        return out;
    }

    template <class T, class Tag, class Compare>
    array<T> partition(
        const base_array<T, Tag> &arr, size_t kth, Compare comp
    ) {
        array<T> out(arr);
        out.partition(kth, comp);
        return out;
    }

    template <class T, class Tag>
    array<T> partition(const base_matrix<T, Tag> &mat, size_t kth) {
        array<T> out(mat.begin(), mat.end());
        out.partition(kth);
        return out;
    }

    template <class T, class Tag, class Compare>
    array<T> partition(
        const base_matrix<T, Tag> &mat, size_t kth, Compare comp
    ) {
        array<T> out(mat.begin(), mat.end());
        out.partition(kth, comp);
        return out;
    }

    template <class T, class Tag>
    matrix<T> partition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise
    ) {
        matrix<T> out(mat);
        out.partition(kth, rowwise);
        return out;
    }

    template <class T, class Tag, class Compare>
    matrix<T> partition(
        const base_matrix<T, Tag> &mat, size_t kth, bool rowwise, Compare comp
    ) {
        matrix<T> out(mat);
        out.partition(kth, rowwise, comp);
        return out;
    }

    template <class Tag>
    array<size_t> where(const base_array<bool, Tag> &condition) {
        size_t n = std::count(condition.begin(), condition.end(), true);
        array<size_t> out(n);
        n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                out[n++] = i;
            }
        }
        return out;
    }

    template <class Tag>
    array<index_t> where(const base_matrix<bool, Tag> &condition) {
        size_t n = std::count(condition.begin(), condition.end(), true);
        array<index_t> out(n);
        n = 0;
        for (size_t i = 0; i < condition.rows(); ++i) {
            for (size_t j = 0; j < condition.cols(); ++j) {
                if (condition(i, j)) {
                    out[n++] = index_t(i, j);
                }
            }
        }
        return out;
    }

    /// Basic statistics

    template <class T, class Tag>
    T mean(const base_array<T, Tag> &arr) {
        __range_mean pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T mean(const base_matrix<T, Tag> &mat) {
        __range_mean pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_mean, T, Tag> >
    mean(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_mean, T, Tag> Closure;
        return base_array<T, Closure>(__range_mean(), mat, rowwise);
    }

    template <class T, class Tag>
    T median(const base_array<T, Tag> &arr) {
        __range_median pred;
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T median(const base_matrix<T, Tag> &mat) {
        __range_median pred;
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_median, T, Tag> >
    median(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_median, T, Tag> Closure;
        return base_array<T, Closure>(__range_median(), mat, rowwise);
    }

    template <class T, class Tag>
    T var(const base_array<T, Tag> &arr, size_t ddof) {
        __range_var pred(ddof);
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T var(const base_matrix<T, Tag> &mat, size_t ddof) {
        __range_var pred(ddof);
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_var, T, Tag> >
    var(const base_matrix<T, Tag> &mat, size_t ddof, bool rowwise) {
        typedef lazy_axis_tag<__range_var, T, Tag> Closure;
        return base_array<T, Closure>(__range_var(ddof), mat, rowwise);
    }

    template <class T, class Tag>
    T stddev(const base_array<T, Tag> &arr, size_t ddof) {
        __range_stddev pred(ddof);
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T stddev(const base_matrix<T, Tag> &mat, size_t ddof) {
        __range_stddev pred(ddof);
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_stddev, T, Tag> >
    stddev(const base_matrix<T, Tag> &mat, size_t ddof, bool rowwise) {
        typedef lazy_axis_tag<__range_stddev, T, Tag> Closure;
        return base_array<T, Closure>(__range_stddev(ddof), mat, rowwise);
    }

    template <class T, class Tag>
    T quantile(
        const base_array<T, Tag> &arr, double q,
        const std::string &method
    ) {
        __range_quantile pred(q, method);
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T quantile(
        const base_matrix<T, Tag> &mat, double q,
        const std::string &method
    ) {
        __range_quantile pred(q, method);
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_quantile, T, Tag> >
    quantile(
        const base_matrix<T, Tag> &mat, double q, bool rowwise,
        const std::string &method
    ) {
        typedef lazy_axis_tag<__range_quantile, T, Tag> Closure;
        return base_array<T, Closure>(
            __range_quantile(q, method), mat, rowwise
        );
    }

    template <class T, class Tag>
    T percentile(
        const base_array<T, Tag> &arr, double q, const std::string &method
    ) {
        __range_quantile pred(q/100.0, method);
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T percentile(
        const base_matrix<T, Tag> &mat, double q, const std::string &method
    ) {
        __range_quantile pred(q/100.0, method);
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_quantile, T, Tag> >
    percentile(
        const base_matrix<T, Tag> &mat, double q, bool rowwise,
        const std::string &method
    ) {
        typedef lazy_axis_tag<__range_quantile, T, Tag> Closure;
        return base_array<T, Closure>(
            __range_quantile(q/100.0, method), mat, rowwise
        );
    }

    template <class T, class Tag1, class Tag2>
    T cov(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2,
        size_t ddof
    ) {
        __assert_equal_length(arr1.size(), arr2.size());
        T val = T(0);
        T mean1 = mean(arr1);
        T mean2 = mean(arr2);
        __math_conj conj;
        for (size_t i = 0; i < arr1.size(); ++i) {
            val += (arr1[i] - mean1) * conj(arr2[i] - mean2);
        }
        val /= arr1.size() - ddof;
        return val;
    }

    template <class T, class Tag>
    matrix<T> cov(
        const base_matrix<T, Tag> &mat, bool rowwise, size_t ddof
    ) {
        size_t size = rowwise ? mat.rows() : mat.cols();
        size_t tda = rowwise ? mat.cols() : mat.rows();
        matrix<T> out(size, size, T(0));
        array<T> means = mean(mat, rowwise);
        __math_conj conj;
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                for (size_t k = 0; k < tda; ++k) {
                    T ai = rowwise ? mat(i, k) : mat(k, i);
                    T aj = rowwise ? mat(j, k) : mat(k, j);
                    out(i, j) += (ai - means[i]) * conj(aj - means[j]);
                }
                out(i, j) /= tda - ddof;
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    T corrcoef(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    ) {
        return cov(arr1, arr2) / (stddev(arr1)*stddev(arr2));
    }

    template <class T, class Tag>
    matrix<T> corrcoef(const base_matrix<T, Tag> &mat, bool rowwise) {
        matrix<T> out = cov(mat, rowwise);
        for (size_t i = 0; i < out.rows(); ++i) {
            for (size_t j = i + 1; j < out.cols(); ++j) {
                T denom = std::sqrt(out(i, i)*out(j, j));
                out(i, j) /= denom;
                out(j, i) /= denom;
            }
            out(i, i) = T(1);
        }
        return out;
    }

    /// Basic linear algebra

    template <class T, class Tag1, class Tag2>
    T inner(const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2) {
        __assert_equal_length(arr1.size(), arr2.size());
        T val = 0;
        __math_conj conj;
        for (size_t i = 0; i < arr1.size(); ++i) {
            val += conj(arr1[i]) * arr2[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< T, outer_tag<__multiplies, T, Tag1, T, Tag2> >
    outer(const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2) {
        typedef outer_tag<__multiplies, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__multiplies(), arr1, arr2);
    }

    template <class T, class Tag1, class Tag2>
    T dot(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        __assert_equal_length(lhs.size(), rhs.size());
        T val = 0;
        for (size_t i = 0; i < lhs.size(); ++i) {
            val += lhs[i] * rhs[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    matrix<T> dot(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        __assert_matmul_shapes(lhs.rows(), lhs.cols(), rhs.rows(), rhs.cols());
        matrix<T> out(lhs.rows(), rhs.cols(), T(0));
        for (size_t i = 0; i < lhs.rows(); ++i) {
            for (size_t j = 0; j < rhs.cols(); ++j) {
                for (size_t k = 0; k < lhs.cols(); ++k) {
                    out(i, j) += lhs(i, k) * rhs(k, j);
                }
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    array<T> dot(
        const base_array<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        __assert_matmul_shapes(1, lhs.size(), rhs.rows(), rhs.cols());
        array<T> out(rhs.cols(), T(0));
        for (size_t i = 0; i < rhs.rows(); ++i) {
            for (size_t j = 0; j < rhs.cols(); ++j) {
                out[j] += lhs[i] * rhs(i, j);
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    array<T> dot(
        const base_matrix<T, Tag1> &lhs, const base_array<T, Tag2> &rhs
    ) {
        __assert_matmul_shapes(lhs.rows(), lhs.cols(), rhs.size(), 1);
        array<T> out(lhs.rows(), T(0));
        for (size_t i = 0; i < lhs.rows(); ++i) {
            for (size_t j = 0; j < lhs.cols(); ++j) {
                out[i] += lhs(i, j) * rhs[j];
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< T, kronecker_tag<Tag1, Tag2> >
    kron(const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs) {
        typedef kronecker_tag<Tag1, Tag2> Closure;
        return base_matrix<T, Closure>(lhs, rhs);
    }

    template <class T, class Tag1, class Tag2>
    array<T> cross(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    ) {
        if (
            (arr1.size() != 2 && arr1.size() != 3) ||
            (arr2.size() != 2 && arr2.size() != 3)
        ) {
            char error[100];
            sprintf(
                error, "incompatible dimensions for cross product: (%zu,), "
                "(%zu,) (must be 2 or 3)", arr1.size(), arr2.size()
            );
            throw std::invalid_argument(error);
        }
        T x1 = arr1[0], y1 = arr1[1], z1 = (arr1.size() == 3) ? arr1[2] : 0;
        T x2 = arr2[0], y2 = arr2[1], z2 = (arr2.size() == 3) ? arr2[2] : 0;
        array<T> out{y1*z2 - y2*z1, x2*z1 - x1*z2, x1*y2 - x2*y1};
        return out;
    }

    template <class T, class Tag>
    base_matrix< T, transpose_tag<Tag> >
    transpose(const base_matrix<T, Tag> &mat) {
        typedef transpose_tag<Tag> Closure;
        return base_matrix<T, Closure>(mat);
    }

    template <class T, class Tag>
    base_matrix< T, conj_transpose_tag<Tag> >
    conj_transpose(const base_matrix<T, Tag> &mat) {
        typedef conj_transpose_tag<Tag> Closure;
        return base_matrix<T, Closure>(mat);
    }

    template <class T, class Tag>
    typename complex_traits<T>::value_type
    norm(const base_array<T, Tag> &arr, double p) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_abs, T, Tag> Closure;
        if (arr.empty()) {
            return Rt(0);
        }
        if (p == 0) {
            return count_nonzero(arr);
        }
        else if (p == HUGE_VAL) {
            return amax(base_array<Rt, Closure>(__math_abs(), arr));
        }
        else if (p == -HUGE_VAL) {
            return amin(base_array<Rt, Closure>(__math_abs(), arr));
        }
        else {
            Rt val = 0;
            Rt max_abs = amax(base_array<Rt, Closure>(__math_abs(), arr));
            if (max_abs > Rt(0)) {
                for (size_t i = 0; i < arr.size(); ++i) {
                    val += std::pow(std::abs(arr[i]) / max_abs, p);
                }
                val = max_abs * std::pow(val, 1.0/p);
            }
            return val;
        }
    }

    template <class T, class Tag>
    T trace(const base_matrix<T, Tag> &mat, ptrdiff_t offset) {
        T val = 0;
        if (offset >= 0) {
            for (size_t i = 0; i < mat.rows() && i + offset < mat.cols(); ++i) {
                val += mat(i, i + offset);
            }
        }
        else {
            for (size_t i = 0; i - offset < mat.rows() && i < mat.cols(); ++i) {
                val += mat(i - offset, i);
            }
        }
        return val;
    }

    /// Set operations

    template <class T, class Tag>
    array<T> unique(const base_array<T, Tag> &arr) {
        std::vector<T> buffer;
        if (std::is_sorted(arr.begin(), arr.end())) {
            std::unique_copy(
                arr.begin(), arr.end(), std::back_inserter(buffer)
            );
        }
        else {
            array<T> sorted = sort(arr);
            std::unique_copy(
                sorted.begin(), sorted.end(), std::back_inserter(buffer)
            );
        }
        return array<T>(buffer.begin(), buffer.end());
    }

    template <class T, class Tag>
    bool includes(const base_array<T, Tag> &arr, const T &val) {
        return std::binary_search(arr.begin(), arr.end(), val);
    }

    template <class T, class Tag1, class Tag2>
    bool includes(
        const base_array<T, Tag1> &arr, const base_array<T, Tag2> &val
    ) {
        return std::includes(arr.begin(), arr.end(), val.begin(), val.end());
    }

    template <class T, class Tag1, class Tag2>
    array<T> set_union(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    ) {
        std::vector<T> buffer;
        std::set_union(
            arr1.begin(), arr1.end(), arr2.begin(), arr2.end(),
            std::back_inserter(buffer)
        );
        return array<T>(buffer.begin(), buffer.end());
    }

    template <class T, class Tag1, class Tag2>
    array<T> set_intersection(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    ) {
        std::vector<T> buffer;
        std::set_intersection(
            arr1.begin(), arr1.end(), arr2.begin(), arr2.end(),
            std::back_inserter(buffer)
        );
        return array<T>(buffer.begin(), buffer.end());
    }

    template <class T, class Tag1, class Tag2>
    array<T> set_difference(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    ) {
        std::vector<T> buffer;
        std::set_difference(
            arr1.begin(), arr1.end(), arr2.begin(), arr2.end(),
            std::back_inserter(buffer)
        );
        return array<T>(buffer.begin(), buffer.end());
    }

    template <class T, class Tag1, class Tag2>
    array<T> set_symmetric_difference(
        const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
    ) {
        std::vector<T> buffer;
        std::set_symmetric_difference(
            arr1.begin(), arr1.end(), arr2.begin(), arr2.end(),
            std::back_inserter(buffer)
        );
        return array<T>(buffer.begin(), buffer.end());
    }
}

#endif // NUMCPP_ROUTINES_TCC_INCLUDED
