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
    void argmax(const base_matrix<T, Tag> &mat, size_t &i, size_t &j) {
        __range_argmax pred;
        base_matrix_const_iterator<T, Tag> it(
            &mat, pred(mat.begin(true), mat.end(true)), true
        );
        i = it.row();
        j = it.col();
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
    void argmin(const base_matrix<T, Tag> &mat, size_t &i, size_t &j) {
        __range_argmin pred;
        base_matrix_const_iterator<T, Tag> it(
            &mat, pred(mat.begin(true), mat.end(true)), true
        );
        i = it.row();
        j = it.col();
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
        __cumsum(arr.begin(), arr.end(), out.begin());
        return out;
    }

    template <class T, class Tag>
    matrix<T> cumsum(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef base_matrix_const_iterator<T, Tag> input_iterator;
        typedef base_matrix_iterator<T, matrix_tag> output_iterator;
        matrix<T> out(mat.rows(), mat.cols());
        if (rowwise) {
            for (size_t i = 0; i < mat.rows(); ++i) {
                input_iterator first(&mat, i, 0, true);
                input_iterator last(&mat, i, mat.cols(), true);
                output_iterator result(&out, i, 0, true);
                __cumsum(first, last, result);
            }
        }
        else {
            for (size_t j = 0; j < mat.cols(); ++j) {
                input_iterator first(&mat, 0, j, false);
                input_iterator last(&mat, mat.rows(), j, false);
                output_iterator result(&out, 0, j, false);
                __cumsum(first, last, result);
            }
        }
        return out;
    }

    template <class T, class Tag>
    array<T> cumprod(const base_array<T, Tag> &arr) {
        array<T> out(arr.size());
        __cumprod(arr.begin(), arr.end(), out.begin());
        return out;
    }

    template <class T, class Tag>
    matrix<T> cumprod(const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef base_matrix_const_iterator<T, Tag> input_iterator;
        typedef base_matrix_iterator<T, matrix_tag> output_iterator;
        matrix<T> out(mat.rows(), mat.cols());
        if (rowwise) {
            for (size_t i = 0; i < mat.rows(); ++i) {
                input_iterator first(&mat, i, 0, true);
                input_iterator last(&mat, i, mat.cols(), true);
                output_iterator result(&out, i, 0, true);
                __cumprod(first, last, result);
            }
        }
        else {
            for (size_t j = 0; j < mat.cols(); ++j) {
                input_iterator first(&mat, 0, j, false);
                input_iterator last(&mat, mat.rows(), j, false);
                output_iterator result(&out, 0, j, false);
                __cumprod(first, last, result);
            }
        }
        return out;
    }

    /// Concatenation

    template <class T, class Tag, class... Arrays>
    base_array< T, concat_tag<base_array<T, Tag>, Arrays...> > concatenate(
        const base_array<T, Tag> &arr1, const Arrays&... arr2
    ) {
        typedef concat_tag<base_array<T, Tag>, Arrays...> Closure;
        return base_array<T, Closure>(arr1, arr2...);
    }

    template <class T, class Tag, class... Matrices>
    base_matrix< T, vstack_tag<base_matrix<T, Tag>, Matrices...> > 
    row_stack(
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        typedef vstack_tag<base_matrix<T, Tag>, Matrices...> Closure;
        return base_matrix<T, Closure>(mat1, mat2...);
    }

    template <class T, class Tag, class... Matrices>
    base_matrix< T, vstack_tag<base_array<T, Tag>, Matrices...> > 
    row_stack(
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        typedef vstack_tag<base_array<T, Tag>, Matrices...> Closure;
        return base_matrix<T, Closure>(arr1, mat2...);
    }

    template <class T, class Tag, class... Matrices>
    base_matrix< T, hstack_tag<base_matrix<T, Tag>, Matrices...> > 
    column_stack(
        const base_matrix<T, Tag> &mat1, const Matrices&... mat2
    ) {
        typedef hstack_tag<base_matrix<T, Tag>, Matrices...> Closure;
        return base_matrix<T, Closure>(mat1, mat2...);
    }

    template <class T, class Tag, class... Matrices>
    base_matrix< T, hstack_tag<base_array<T, Tag>, Matrices...> > 
    column_stack(
        const base_array<T, Tag> &arr1, const Matrices&... mat2
    ) {
        typedef hstack_tag<base_array<T, Tag>, Matrices...> Closure;
        return base_matrix<T, Closure>(arr1, mat2...);
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

    template <class T, class Tag1, class Tag2>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> > 
    isclose(
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs,
        const T &rtol, const T &atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__isclose<T>(rtol, atol), lhs, rhs);
    }

    template <class T, class Tag>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> > 
    isclose(
        const base_array<T, Tag> &lhs, 
        const typename base_array<T, Tag>::value_type &val,
        const T &rtol, const T &atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__isclose<T>(rtol, atol), lhs, val);
    }

    template <class T, class Tag>
    base_array< bool, lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> > 
    isclose(
        const typename base_array<T, Tag>::value_type &val, 
        const base_array<T, Tag> &rhs,
        const T &rtol, const T &atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__isclose<T>(rtol, atol), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> > 
    isclose(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs,
        const T &rtol, const T &atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__isclose<T>(rtol, atol), lhs, rhs);
    }

    template <class T, class Tag>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> > 
    isclose(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val,
        const T &rtol, const T &atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__isclose<T>(rtol, atol), lhs, val);
    }

    template <class T, class Tag>
    base_matrix< bool, lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> > 
    isclose(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs,
        const T &rtol, const T &atol
    ) {
        typedef lazy_binary_tag<__isclose<T>, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__isclose<T>(rtol, atol), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    bool allclose(
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs,
        const T &rtol, const T &atol
    ) {
        return all(isclose(lhs, rhs, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const base_array<T, Tag> &lhs, 
        const typename base_array<T, Tag>::value_type &val,
        const T &rtol, const T &atol
    ) {
        return all(isclose(lhs, val, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const typename base_array<T, Tag>::value_type &val, 
        const base_array<T, Tag> &rhs,
        const T &rtol, const T &atol
    ) {
        return all(isclose(val, rhs, rtol, atol));
    }

    template <class T, class Tag1, class Tag2>
    bool allclose(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs,
        const T &rtol, const T &atol
    ) {
        return all(isclose(lhs, rhs, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val,
        const T &rtol, const T &atol
    ) {
        return all(isclose(lhs, val, rtol, atol));
    }

    template <class T, class Tag>
    bool allclose(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs,
        const T &rtol, const T &atol
    ) {
        return all(isclose(val, rhs, rtol, atol));
    }

    /// Sorting and searching 

    template <class T, class Tag>
    array<size_t> argsort(const base_array<T, Tag> &arr) {
        array<size_t> index(arr.size());
        __iota(index.begin(), index.end(), 0, 1);
        auto comparator = [&arr](size_t i, size_t j) {
            return arr[i] < arr[j];
        };
        std::sort(index.begin(), index.end(), comparator);
        return index;
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
    array<T> sort(const base_array<T, Tag> &arr) {
        array<T> out(arr);
        std::sort(out.begin(), out.end());
        return out;
    }

    template <class T, class Tag, class Compare>
    array<T> sort(
        const base_array<T, Tag> &arr, Compare comp, bool stable
    ) {
        array<T> out(arr);
        if (stable) {
            std::stable_sort(out.begin(), out.end(), comp);
        }
        else {
            std::sort(out.begin(), out.end(), comp);
        }
        return out;
    }

    template <class T, class Tag>
    array<size_t> argpartition(const base_array<T, Tag> &arr, size_t kth) {
        __assert_within_bounds(arr.size(), kth);
        array<size_t> index(arr.size());
        __iota(index.begin(), index.end(), 0, 1);
        auto comparator = [&arr](size_t i, size_t j) {
            return arr[i] < arr[j];
        };
        std::nth_element(
            index.begin(), index.begin() + kth, index.end(), comparator
        );
        return index;
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
    array<T> partition(const base_array<T, Tag> &arr, size_t kth) {
        __assert_within_bounds(arr.size(), kth);
        array<T> out(arr);
        std::nth_element(out.begin(), out.begin() + kth, out.end());
        return out;
    }

    template <class T, class Tag, class Compare>
    array<T> partition(
        const base_array<T, Tag> &arr, size_t kth, Compare comp
    ) {
        __assert_within_bounds(arr.size(), kth);
        array<T> out(arr);
        std::nth_element(out.begin(), out.begin() + kth, out.end(), comp);
        return out;
    }

    template <class T, class Tag>
    base_array< T, reverse_tag<Tag> > reverse(const base_array<T, Tag> &arr) {
        typedef reverse_tag<Tag> Closure;
        return base_array<T, Closure>(arr);
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
    array< std::pair<size_t, size_t> > where(
        const base_matrix<bool, Tag> &condition
    ) {
        size_t n = std::count(condition.begin(), condition.end(), true);
        array< std::pair<size_t, size_t> > out(n);
        n = 0;
        for (size_t i = 0; i < condition.rows(); ++i) {
            for (size_t j = 0; j < condition.cols(); ++j) {
                if (condition(i, j)) {
                    out[n++] = std::make_pair(i, j);
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
        const base_array<T, Tag> &arr, int q, const std::string &method
    ) {
        __range_quantile pred(q/100.0, method);
        return pred(arr.begin(), arr.end());
    }

    template <class T, class Tag>
    T percentile(
        const base_matrix<T, Tag> &mat, int q, const std::string &method
    ) {
        __range_quantile pred(q/100.0, method);
        return pred(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    base_array< T, lazy_axis_tag<__range_quantile, T, Tag> >
    percentile(
        const base_matrix<T, Tag> &mat, int q, bool rowwise, 
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
        T val = 0;
        T mean1 = mean(arr1);
        T mean2 = mean(arr2);
        __conjugate conj;
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
        array<T> means = mean(mat, rowwise);
        __conjugate conj;
        if (rowwise) {
            matrix<T> out(mat.rows(), mat.rows());
            for (size_t i = 0; i < mat.rows(); ++i) {
                for (size_t j = 0; j < mat.rows(); ++j) {
                    out(i, j) = 0;
                    for (size_t k = 0; k < mat.cols(); ++k) {
                        out(i, j) += (mat(i, k) - means[i]) * 
                                     conj(mat(j, k) - means[j]);
                    }
                    out(i, j) /= mat.cols() - ddof;
                }
            }
            return out;
        }
        else {
            matrix<T> out(mat.cols(), mat.cols());
            for (size_t i = 0; i < mat.cols(); ++i) {
                for (size_t j = 0; j < mat.cols(); ++j) {
                    out(i, j) = 0;
                    for (size_t k = 0; k < mat.rows(); ++k) {
                        out(i, j) += (mat(k, i) - means[i]) * 
                                     conj(mat(k, j) - means[j]);
                    }
                    out(i, j) /= mat.rows() - ddof;
                }
            }
            return out;
        }
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
            out(i, i) = 1;
        }
        return out;
    }

    /// Basic linear algebra

    template <class T, class Tag1, class Tag2>
    T inner(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        __assert_equal_length(lhs.size(), rhs.size());
        T val = 0;
        __conjugate conj;
        for (size_t i = 0; i < lhs.size(); ++i) {
            val += conj(lhs[i]) * rhs[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    base_matrix< T, outer_tag<__multiplies, T, Tag1, T, Tag2> >
    outer(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef outer_tag<__multiplies, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__multiplies(), lhs, rhs);
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
        const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs
    ) {
        if (
            (lhs.size() != 2 && lhs.size() != 3) || 
            (rhs.size() != 2 && rhs.size() != 3)
        ) {
            char error[100];
            sprintf(
                error, "incompatible dimensions for cross product: (%zu,), "
                "(%zu,) (must be 2 or 3)", lhs.size(), rhs.size()
            );
            throw std::invalid_argument(error);
        }
        T x1 = lhs[0], y1 = lhs[1], z1 = (lhs.size() == 3) ? lhs[2] : 0;
        T x2 = rhs[0], y2 = rhs[1], z2 = (rhs.size() == 3) ? rhs[2] : 0;
        array<T> out(3);
        out[0] = y1*z2 - y2*z1;
        out[1] = x2*z1 - x1*z2;
        out[2] = x1*y2 - x2*y1;
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
        typedef typename complex_traits<T>::value_type value_type;
        if (arr.empty()) {
            return value_type(0);
        }
        if (p == 0) {
            return count_nonzero(arr);
        }
        else if (p == HUGE_VAL) {
            value_type val = std::abs(arr[0]);
            for (size_t i = 1; i < arr.size(); ++i) {
                if (val < std::abs(arr[i])) {
                    val = std::abs(arr[i]);
                }
            }
            return val;
        }
        else if (p == -HUGE_VAL) {
            value_type val = std::abs(arr[0]);
            for (size_t i = 1; i < arr.size(); ++i) {
                if (val > std::abs(arr[i])) {
                    val = std::abs(arr[i]);
                }
            }
            return val;
        }
        else {
            value_type max_abs = std::abs(arr[0]);
            value_type val = 0;
            for (size_t i = 1; i < arr.size(); ++i) {
                if (max_abs < std::abs(arr[i])) {
                    max_abs = std::abs(arr[i]);
                }
            }
            if (max_abs > value_type(0)) {
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
