/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
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

#include "numcpp/functional.h"

namespace numcpp {
    /// Tensor creation routines.

    template <class T, size_t Rank>
    inline tensor<T, Rank> empty(const shape_t<Rank> &shape) {
        return tensor<T, Rank>(shape);
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> empty_like(const base_tensor<T, Rank, Tag> &like) {
        return tensor<T, Rank>(like.shape());
    }

    template <class T, size_t Rank>
    inline tensor<T, Rank> zeros(const shape_t<Rank> &shape) {
        return tensor<T, Rank>(shape, T());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> zeros_like(const base_tensor<T, Rank, Tag> &like) {
        return tensor<T, Rank>(like.shape(), T());
    }

    template <class T, size_t Rank>
    inline tensor<T, Rank> ones(const shape_t<Rank> &shape) {
        return tensor<T, Rank>(shape, T(1));
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> ones_like(const base_tensor<T, Rank, Tag> &like) {
        return tensor<T, Rank>(like.shape(), T(1));
    }

    template <class T, size_t Rank>
    inline tensor<T, Rank> full(const shape_t<Rank> &shape, const T &val) {
        return tensor<T, Rank>(shape, val);
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> full_like(
        const base_tensor<T, Rank, Tag> &like,
        const typename base_tensor<T, Rank, Tag>::value_type &val
    ) {
        return tensor<T, Rank>(like.shape(), val);
    }

    /// Numerical ranges.

    template <class T>
    inline base_tensor<T, 1, sequence_tag> arange(const T &stop) {
        size_t size = 0;
        if (stop > 0) {
            size = std::ceil(stop);
        }
        return base_tensor<T, 1, sequence_tag>(T(), size, T(1));
    }

    template <class T, class U>
    inline base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    arange(const T &start, const U &stop) {
        typedef typename std::common_type<T, U>::type Rt;
        size_t size = 0;
        if (start < stop) {
            size = std::ceil(stop - start);
        }
        return base_tensor<Rt, 1, sequence_tag>(start, size, Rt(1));
    }

    template <class T, class U, class V>
    inline base_tensor<
        typename std::common_type<T, U, V>::type, 1, sequence_tag
    > arange(const T &start, const U &stop, const V &step) {
        typedef typename std::common_type<T, U, V>::type Rt;
        size_t size = 0;
        if ((start < stop && step > 0) || (start > stop && step < 0)) {
            size = std::ceil((double)(stop - start) / step);
        }
        return base_tensor<Rt, 1, sequence_tag>(start, size, step);
    }

    template <class T, class U>
    inline base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    linspace(const T &start, const U &stop, size_t num, bool endpoint) {
        typedef typename std::common_type<T, U>::type Rt;
        Rt step = stop - start;
        step /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(start, num, step);
    }

    template <class T, class U>
    inline base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    logspace(const T &start, const U &stop, size_t num, bool endpoint) {
        typedef typename std::common_type<T, U>::type Rt;
        Rt step = stop - start;
        step /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(start, num, step, Rt(10));
    }

    template <class T, class U, class V>
    inline base_tensor<
        typename std::common_type<T, U, V>::type, 1, sequence_tag
    > logspace(
        const T &start, const U &stop, size_t num, bool endpoint,
        const V &base
    ) {
        typedef typename std::common_type<T, U, V>::type Rt;
        Rt step = stop - start;
        step /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(start, num, step, base);
    }

    template <class T, class U>
    inline base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    geomspace(const T &start, const U &stop, size_t num, bool endpoint) {
        typedef typename std::common_type<T, U>::type Rt;
        Rt logstart = std::log10(Rt(start));
        Rt logstop = std::log10(Rt(stop));
        Rt logstep = logstop - logstart;
        logstep /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(logstart, num, logstep, Rt(10));
    }

    /// Building matrices.

    template <class T>
    inline base_tensor<T, 2, eye_tag> eye(size_t n) {
        return base_tensor<T, 2, eye_tag>(n, n);
    }

    template <class T>
    inline base_tensor<T, 2, eye_tag>
    eye(size_t m, size_t n, ptrdiff_t k) {
        return base_tensor<T, 2, eye_tag>(m, n, k);
    }

    template <class T, class Tag>
    inline base_tensor<T, 1, diagonal_tag<Tag> >
    diag(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        typedef diagonal_tag<Tag> Closure;
        return base_tensor<T, 1, Closure>(a, k);
    }

    template <class T, class Tag>
    inline base_tensor<T, 2, diagonal_tag<Tag> >
    diag(const base_tensor<T, 1, Tag> &a, ptrdiff_t k) {
        typedef diagonal_tag<Tag> Closure;
        return base_tensor<T, 2, Closure>(a, k);
    }

    template <class T, class Tag>
    inline base_tensor<T, 2, triangular_tag<Tag> >
    tril(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        typedef triangular_tag<Tag> Closure;
        return base_tensor<T, 2, Closure>(a, true, k);
    }

    template <class T, class Tag>
    inline base_tensor<T, 2, triangular_tag<Tag> >
    triu(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        typedef triangular_tag<Tag> Closure;
        return base_tensor<T, 2, Closure>(a, false, k);
    }

    template <class T, class Tag>
    inline tensor<T, 2> vander(const base_tensor<T, 1, Tag> &x) {
        return vander(x, x.size());
    }

    template <class T, class Tag>
    tensor<T, 2> vander(
        const base_tensor<T, 1, Tag> &x, size_t N, bool increasing
    ) {
        tensor<T, 2> out(x.size(), N);
        for (size_t i = 0; i < x.size(); ++i) {
            T pow = T(1);
            for (size_t j = 0; j < N; ++j) {
                if (increasing) {
                    out(i, j) = pow;
                }
                else {
                    out(i, N - 1 - j) = pow;
                }
                pow *= x[i];
            }
        }
        return out;
    }

    /// Maximums and minimums.

    template <class T, size_t Rank, class Tag>
    index_t<Rank> argmax(const base_tensor<T, Rank, Tag> &a) {
        ranges::argmax pred;
        size_t index = pred(a.begin(), a.end());
        return unravel_index(index, a.shape(), a.layout());
    }

    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank>
    argmax(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        tensor<size_t, Rank> out;
        apply_along_axis(out, ranges::argmax(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    index_t<Rank> argmin(const base_tensor<T, Rank, Tag> &a) {
        ranges::argmin pred;
        size_t index = pred(a.begin(), a.end());
        return unravel_index(index, a.shape(), a.layout());
    }

    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank>
    argmin(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        tensor<size_t, Rank> out;
        apply_along_axis(out, ranges::argmin(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    amax(const base_tensor<T, Rank, Tag> &a) {
        ranges::max pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amax(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::max(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amax(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::max(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    amin(const base_tensor<T, Rank, Tag> &a) {
        ranges::min pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amin(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::min(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amin(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::min(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, Tag1, T, Tag2>
    > maximum(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        typedef lazy_binary_tag<ranges::maximum, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(ranges::maximum(), a, b);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, Tag, T, scalar_tag>
    > maximum(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<ranges::maximum, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::maximum(), a, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag>
    > maximum(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b
    ) {
        typedef lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::maximum(), val, b);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, Tag1, T, Tag2>
    > minimum(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        typedef lazy_binary_tag<ranges::minimum, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(ranges::minimum(), a, b);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, Tag, T, scalar_tag>
    > minimum(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<ranges::minimum, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::minimum(), a, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag>
    > minimum(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b
    ) {
        typedef lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::minimum(), val, b);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<ranges::clamp<T>, T, Tag> >
    clamp(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &a_min,
        const typename base_tensor<T, Rank, Tag>::value_type &a_max
    ) {
        typedef lazy_unary_tag<ranges::clamp<T>, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::clamp<T>(a_min, a_max), a);
    }

    /// Sums and products.

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    sum(const base_tensor<T, Rank, Tag> &a) {
        ranges::sum pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sum(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::sum(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sum(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::sum(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    prod(const base_tensor<T, Rank, Tag> &a) {
        ranges::prod pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    prod(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::prod(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    prod(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::prod(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    cumsum(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        accumulate(out, plus(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    cumprod(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        accumulate(out, multiplies(), a, axis);
        return out;
    }

    /// Logic functions.

    template <size_t Rank, class Tag>
    bool all(const base_tensor<bool, Rank, Tag> &a) {
        ranges::all pred;
        return pred(a.begin(), a.end());
    }

    template <size_t Rank, class Tag>
    tensor<bool, Rank>
    all(const base_tensor<bool, Rank, Tag> &a, size_t axis) {
        tensor<bool, Rank> out;
        apply_along_axis(out, ranges::all(), a, axis);
        return out;
    }

    template <size_t Rank, class Tag, size_t N>
    tensor<bool, Rank>
    all(const base_tensor<bool, Rank, Tag> &a, const shape_t<N> &axes) {
        tensor<bool, Rank> out;
        apply_over_axes(out, ranges::all(), a, axes);
        return out;
    }

    template <size_t Rank, class Tag>
    bool any(const base_tensor<bool, Rank, Tag> &a) {
        ranges::any pred;
        return pred(a.begin(), a.end());
    }

    template <size_t Rank, class Tag>
    tensor<bool, Rank>
    any(const base_tensor<bool, Rank, Tag> &a, size_t axis) {
        tensor<bool, Rank> out;
        apply_along_axis(out, ranges::any(), a, axis);
        return out;
    }

    template <size_t Rank, class Tag, size_t N>
    tensor<bool, Rank>
    any(const base_tensor<bool, Rank, Tag> &a, const shape_t<N> &axes) {
        tensor<bool, Rank> out;
        apply_over_axes(out, ranges::any(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    size_t count_nonzero(const base_tensor<T, Rank, Tag> &a) {
        ranges::count_nonzero pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank>
    count_nonzero(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        tensor<size_t, Rank> out;
        apply_along_axis(out, ranges::count_nonzero(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<size_t, Rank>
    count_nonzero(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        tensor<size_t, Rank> out;
        apply_over_axes(out, ranges::count_nonzero(), a, axes);
        return out;
    }

    bool isclose(float a, float b, float rtol, float atol) {
        ranges::isclose<float> pred(rtol, atol);
        return pred(a, b);
    }

    bool isclose(double a, double b, double rtol, double atol) {
        ranges::isclose<double> pred(rtol, atol);
        return pred(a, b);
    }

    bool isclose(
        long double a, long double b, long double rtol, long double atol
    ) {
        ranges::isclose<long double> pred(rtol, atol);
        return pred(a, b);
    }

    template <class T, class U>
    typename std::enable_if<
        std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, bool
    >::type isclose(
        T a, U b,
        typename std::common_type<T, U, double>::type rtol,
        typename std::common_type<T, U, double>::type atol
    ) {
        typedef typename std::common_type<T, U, double>::type Rt;
        ranges::isclose<Rt> pred(rtol, atol);
        return pred(a, b);
    }

    template <class T>
    bool isclose(
        const std::complex<T> &a, const std::complex<T> &b,
        typename std::complex<T>::value_type rtol,
        typename std::complex<T>::value_type atol
    ) {
        ranges::isclose<std::complex<T> > pred(rtol, atol);
        return pred(a, b);
    }

    template <class T>
    bool isclose(
        const std::complex<T> &a,
        const typename std::complex<T>::value_type &b,
        typename std::complex<T>::value_type rtol,
        typename std::complex<T>::value_type atol
    ) {
        ranges::isclose<std::complex<T> > pred(rtol, atol);
        return pred(a, b);
    }

    template <class T>
    bool isclose(
        const typename std::complex<T>::value_type &a,
        const std::complex<T> &b,
        typename std::complex<T>::value_type rtol,
        typename std::complex<T>::value_type atol
    ) {
        ranges::isclose<std::complex<T> > pred(rtol, atol);
        return pred(a, b);
    }

    template <class T, class U>
    bool isclose(
        const std::complex<T> &a, const std::complex<U> &b,
        typename std::common_type<T, U>::type rtol,
        typename std::common_type<T, U>::type atol
    ) {
        typedef typename std::common_type<T, U>::type Rt;
        ranges::isclose<std::complex<Rt> > pred(rtol, atol);
        return pred(a, b);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, Tag1, T, Tag2>
    > isclose(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const typename base_tensor<T, Rank, Tag1>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag2>::value_type &atol
    ) {
        typedef lazy_binary_tag<ranges::isclose<T>, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(
            ranges::isclose<T>(rtol, atol), a, b
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, Tag, T, scalar_tag>
    > isclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag>::value_type &atol
    ) {
        typedef lazy_binary_tag<ranges::isclose<T>, T, Tag, T, scalar_tag>
            Closure;
        return base_tensor<bool, Rank, Closure>(
            ranges::isclose<T>(rtol, atol), a, val
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, scalar_tag, T, Tag>
    > isclose(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag>::value_type &atol
    ) {
        typedef lazy_binary_tag<ranges::isclose<T>, T, scalar_tag, T, Tag>
            Closure;
        return base_tensor<bool, Rank, Closure>(
            ranges::isclose<T>(rtol, atol), val, b
        );
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, Tag1, T, Tag2
        >
    > isclose(
        const base_tensor<std::complex<T>, Rank, Tag1> &a,
        const base_tensor<std::complex<T>, Rank, Tag2> &b,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        typedef lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, Tag1, T, Tag2
        > Closure;
        return base_tensor<bool, Rank, Closure>(
            ranges::isclose<std::complex<T> >(rtol, atol), a, b
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, Tag, T, scalar_tag
        >
    > isclose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        typedef lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, Tag, T, scalar_tag
        > Closure;
        return base_tensor<bool, Rank, Closure>(
            ranges::isclose<std::complex<T> >(rtol, atol), a, val
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, scalar_tag, T, Tag
        >
    > isclose(
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const base_tensor<std::complex<T>, Rank, Tag> &b,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        typedef lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, scalar_tag, T, Tag
        > Closure;
        return base_tensor<bool, Rank, Closure>(
            ranges::isclose<std::complex<T> >(rtol, atol), val, b
        );
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline bool allclose(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const typename base_tensor<T, Rank, Tag1>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag2>::value_type &atol
    ) {
        return all(isclose(a, b, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag>::value_type &atol
    ) {
        return all(isclose(a, val, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag>::value_type &atol
    ) {
        return all(isclose(val, b, rtol, atol));
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline bool allclose(
        const base_tensor<std::complex<T>, Rank, Tag1> &a,
        const base_tensor<std::complex<T>, Rank, Tag2> &b,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        return all(isclose(a, b, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        return all(isclose(a, val, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const base_tensor<std::complex<T>, Rank, Tag> &b,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        return all(isclose(val, b, rtol, atol));
    }

    /// Sorting and searching.

    template <class T, size_t Rank, class Tag>
    inline tensor<index_t<Rank>, 1> argsort(
        const base_tensor<T, Rank, Tag> &a
    ) {
        return argsort(a, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<index_t<Rank>, 1> argsort(
        const base_tensor<T, Rank, Tag> &a, Compare comp, bool stable
    ) {
        index_sequence<Rank> indices(a.shape());
        tensor<index_t<Rank>, 1> out(indices.begin(), a.size());
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(a[i], a[j]);
        };
        if (stable) {
            std::stable_sort(out.begin(), out.end(), comparator);
        }
        else {
            std::sort(out.begin(), out.end(), comparator);
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank> argsort(
        const base_tensor<T, Rank, Tag> &a, size_t axis
    ) {
        return argsort(a, axis, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<size_t, Rank> argsort(
        const base_tensor<T, Rank, Tag> &a, size_t axis,
        Compare comp, bool stable
    ) {
        shape_t<Rank> shape = a.shape();
        tensor<size_t, Rank> out(shape);
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> out_index : make_indices(shape)) {
            auto first = make_axes_iterator(&out, out_index, axis, 0);
            auto last = make_axes_iterator(&out, out_index, axis, size);
            std::iota(first, last, 0);
            index_t<Rank> i = out_index, j = out_index;
            auto comparator = [&](size_t i_axis, size_t j_axis) {
                i[axis] = i_axis;
                j[axis] = j_axis;
                return comp(a[i], a[j]);
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

    template <class T, size_t Rank, class Tag>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    sort(const base_tensor<T, Rank, Tag> &a) {
        return sort(a, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    sort(const base_tensor<T, Rank, Tag> &a, Compare comp, bool stable) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, 1> out(a.begin(), a.size());
        if (stable) {
            std::stable_sort(out.begin(), out.end(), comp);
        }
        else {
            std::sort(out.begin(), out.end(), comp);
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sort(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        return sort(a, axis, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sort(
        const base_tensor<T, Rank, Tag> &a, size_t axis,
        Compare comp, bool stable
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out(a);
        out.sort(axis, comp, stable);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<index_t<Rank>, 1> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth
    ) {
        return argpartition(a, kth, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<index_t<Rank>, 1> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, Compare comp
    ) {
        index_sequence<Rank> indices(a.shape());
        tensor<index_t<Rank>, 1> out(indices.begin(), a.size());
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(a[i], a[j]);
        };
        std::nth_element(out.begin(), out.begin() + kth, out.end(), comparator);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis
    ) {
        return argpartition(a, kth, axis, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<size_t, Rank> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis,
        Compare comp
    ) {
        shape_t<Rank> shape = a.shape();
        tensor<size_t, Rank> out(shape);
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> out_index : make_indices(shape)) {
            auto first = make_axes_iterator(&out, out_index, axis, 0);
            auto last = make_axes_iterator(&out, out_index, axis, size);
            std::iota(first, last, 0);
            index_t<Rank> i = out_index, j = out_index;
            auto comparator = [&](size_t i_axis, size_t j_axis) {
                i[axis] = i_axis;
                j[axis] = j_axis;
                return comp(a[i], a[j]);
            };
            std::nth_element(first, first + kth, last, comparator);
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    partition(const base_tensor<T, Rank, Tag> &a, size_t kth) {
        return partition(a, kth, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    partition(const base_tensor<T, Rank, Tag> &a, size_t kth, Compare comp) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, 1> out(a.begin(), a.size());
        std::nth_element(out.begin(), out.begin() + kth, out.end(), comp);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    partition(const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis) {
        return partition(a, kth, axis, less());
    }

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> >
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    partition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis,
        Compare comp
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out(a);
        out.partition(kth, axis, comp);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> nonzero(const base_tensor<T, Rank, Tag> &a) {
        size_t size = a.size();
        size -= std::count(a.begin(), a.end(), T());
        tensor<index_t<Rank>, 1> out(size);
        size_t n = 0;
        for (index_t<Rank> i : make_indices(a.shape())) {
            if (a[i] != T()) {
                out[n++] = i;
            }
        }
        return out;
    }

    template <size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> where(
        const base_tensor<bool, Rank, Tag> &condition
    ) {
        size_t size = std::count(condition.begin(), condition.end(), true);
        tensor<index_t<Rank>, 1> out(size);
        size_t n = 0;
        for (index_t<Rank> i : make_indices(condition.shape())) {
            if (condition[i]) {
                out[n++] = i;
            }
        }
        return out;
    }

    template <class T, size_t Rank,
              class TagCond, class TagTrue, class TagFalse>
    inline base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagTrue> &x,
        const base_tensor<T, Rank, TagFalse> &y
    ) {
        typedef lazy_where_tag<TagCond, TagTrue, TagFalse> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    template <class T, size_t Rank, class TagCond, class TagTrue>
    inline base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagTrue> &x,
        const typename base_tensor<T, Rank, TagTrue>::value_type &y
    ) {
        typedef lazy_where_tag<TagCond, TagTrue, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    template <class T, size_t Rank, class TagCond, class TagFalse>
    inline base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const typename base_tensor<T, Rank, TagFalse>::value_type &x,
        const base_tensor<T, Rank, TagFalse> &y
    ) {
        typedef lazy_where_tag<TagCond, scalar_tag, TagFalse> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    /// Rearranging elements.

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, flip_tag<Tag, 1> > reverse(
        const base_tensor<T, Rank, Tag> &a, size_t axis
    ) {
        typedef flip_tag<Tag, 1> Closure;
        return base_tensor<T, Rank, Closure>(a, axis);
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor<T, Rank, flip_tag<Tag, N> > reverse(
        const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes
    ) {
        typedef flip_tag<Tag, N> Closure;
        return base_tensor<T, Rank, Closure>(a, axes);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, roll_tag<Tag, 1> > rotate(
        const base_tensor<T, Rank, Tag> &a,
        size_t shift, size_t axis
    ) {
        typedef roll_tag<Tag, 1> Closure;
        return base_tensor<T, Rank, Closure>(a, shift, axis);
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor<T, Rank, roll_tag<Tag, N> > rotate(
        const base_tensor<T, Rank, Tag> &a,
        const index_t<N> &shift, const shape_t<N> &axes
    ) {
        typedef roll_tag<Tag, N> Closure;
        return base_tensor<T, Rank, Closure>(a, shift, axes);
    }

    /// Basic statistics.

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    mean(const base_tensor<T, Rank, Tag> &a) {
        ranges::mean pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    mean(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::mean(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    mean(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::mean(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    median(const base_tensor<T, Rank, Tag> &a) {
        ranges::median pred;
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    median(const base_tensor<T, Rank, Tag> &a, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::median(), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    median(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::median(), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    var(const base_tensor<T, Rank, Tag> &a, bool bias) {
        ranges::var pred(bias);
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    var(const base_tensor<T, Rank, Tag> &a, size_t axis, bool bias) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::var(bias), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    var(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes, bool bias) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::var(bias), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    stddev(const base_tensor<T, Rank, Tag> &a, bool bias) {
        ranges::stddev pred(bias);
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    stddev(const base_tensor<T, Rank, Tag> &a, size_t axis, bool bias) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::stddev(bias), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    stddev(
        const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes, bool bias
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::stddev(bias), a, axes);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    quantile(
        const base_tensor<T, Rank, Tag> &a, double q,
        const std::string &method
    ) {
        ranges::quantile pred(q, method);
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    quantile(
        const base_tensor<T, Rank, Tag> &a, double q, size_t axis,
        const std::string &method
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, ranges::quantile(q, method), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    quantile(
        const base_tensor<T, Rank, Tag> &a, double q, const shape_t<N> &axes,
        const std::string &method
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, ranges::quantile(q, method), a, axes);
        return out;
    }

    template <class T, class Tag1, class Tag2>
    T cov(
        const base_tensor<T, 1, Tag1> &x, const base_tensor<T, 1, Tag2> &y,
        bool bias
    ) {
        if (x.size() != y.size()) {
            char error[] = "all the tensors must have the same shape";
            throw std::invalid_argument(error);
        }
        size_t size = x.size();
        T val = T(0);
        T x_mean = mean(x);
        T y_mean = mean(y);
        for (size_t i = 0; i < size; ++i) {
            val += (x[i] - x_mean) * (y[i] - y_mean);
        }
        val /= size - 1 + bias;
        return val;
    }

    template <class T, class Tag1, class Tag2>
    std::complex<T> cov(
        const base_tensor<std::complex<T>, 1, Tag1> &x,
        const base_tensor<std::complex<T>, 1, Tag2> &y,
        bool bias
    ) {
        if (x.size() != y.size()) {
            char error[] = "all the tensors must have the same shape";
            throw std::invalid_argument(error);
        }
        size_t size = x.size();
        std::complex<T> val = T(0);
        std::complex<T> x_mean = mean(x);
        std::complex<T> y_mean = mean(y);
        for (size_t i = 0; i < size; ++i) {
            val += (x[i] - x_mean) * std::conj(y[i] - y_mean);
        }
        val /= size - 1 + bias;
        return val;
    }

    template <class T, class Tag>
    tensor<T, 2> cov(const base_tensor<T, 2, Tag> &a, bool rowvar, bool bias) {
        size_t nvar = rowvar ? a.shape(0) : a.shape(1);
        size_t size = rowvar ? a.shape(1) : a.shape(0);
        tensor<T, 2> out(nvar, nvar);
        tensor<T, 2> a_mean = mean(a, rowvar);
        for (size_t i = 0; i < nvar; ++i) {
            for (size_t j = 0; j < nvar; ++j) {
                T val = T(0), x, y;
                for (size_t k = 0; k < size; ++k) {
                    if (rowvar) {
                        x = a(i, k) - a_mean(i, 0);
                        y = a(j, k) - a_mean(j, 0);
                    }
                    else {
                        x = a(k, i) - a_mean(0, i);
                        y = a(k, j) - a_mean(0, j);
                    }
                    val += x * y;
                }
                val /= size - 1 + bias;
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T, class Tag>
    tensor<std::complex<T>, 2> cov(
        const base_tensor<std::complex<T>, 2, Tag> &a, bool rowvar, bool bias
    ) {
        size_t nvar = rowvar ? a.shape(0) : a.shape(1);
        size_t size = rowvar ? a.shape(1) : a.shape(0);
        tensor<std::complex<T>, 2> out(nvar, nvar);
        tensor<std::complex<T>, 2> a_mean = mean(a, rowvar);
        for (size_t i = 0; i < nvar; ++i) {
            for (size_t j = 0; j < nvar; ++j) {
                std::complex<T> val = T(0), x, y;
                for (size_t k = 0; k < size; ++k) {
                    if (rowvar) {
                        x = a(i, k) - a_mean(i, 0);
                        y = a(j, k) - a_mean(j, 0);
                    }
                    else {
                        x = a(k, i) - a_mean(0, i);
                        y = a(k, j) - a_mean(0, j);
                    }
                    val += x * std::conj(y);
                }
                val /= size - 1 + bias;
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    T corrcoef(
        const base_tensor<T, 1, Tag1> &x, const base_tensor<T, 1, Tag2> &y
    ) {
        return cov(x, y) / (stddev(x, false) * stddev(y, false));
    }

    template <class T, class Tag>
    tensor<T, 2> corrcoef(const base_tensor<T, 2, Tag> &a, bool rowvar) {
        tensor<T, 2> out = std::move(cov(a, rowvar));
        for (size_t i = 0; i < out.shape(0); ++i) {
            for (size_t j = i + 1; j < out.shape(1); ++j) {
                T denom = std::sqrt(out(i, i) * out(j, j));
                out(i, j) /= denom;
                out(j, i) /= denom;
            }
            out(i, i) = T(1);
        }
        return out;
    }

    /// Basic linear algebra.

namespace detail {
    /**
     * @brief Helper function. Asserts that the shapes are aligned along the
     * given axes.
     */
    template <size_t Rank1, size_t Rank2>
    void assert_aligned_shapes(
        const shape_t<Rank1> &shape1, size_t axis1,
        const shape_t<Rank2> &shape2, size_t axis2
    ) {
        if (shape1[axis1] != shape2[axis2]) {
            std::ostringstream error;
            error << "shapes " << shape1 << " and " << shape2
                  << " not aligned: " << shape1[axis1] << " (dim " << axis1
                  << ") != " << shape2[axis2] << " (dim " << axis2 << ")";
            throw std::invalid_argument(error.str());
        }
    }

    template <size_t Rank1, size_t Rank2, size_t N>
    void assert_aligned_shapes(
        const shape_t<Rank1> &shape1, const shape_t<N> axes1,
        const shape_t<Rank2> &shape2, const shape_t<N> axes2
    ) {
        for (size_t i = 0; i < axes1.ndim(); ++i) {
            assert_aligned_shapes(shape1, axes1[i], shape2, axes2[i]);
        }
    }

    /**
     * @brief Helper function. Broadcast two shapes for cross product.
     */
    template <size_t Rank>
    shape_t<Rank> broadcast_cross(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2, size_t axis
    ) {
        shape_t<Rank> out_shape = shape1;
        if (shape1[axis] != 3 || shape2[axis] != 3) {
            char error[] = "incompatible dimensions for cross product"
                " (dimension must be 3)";
            throw std::invalid_argument(error);
        }
        for (size_t i = 0; i < shape1.ndim(); ++i) {
            if (i == axis) {
                out_shape[i] = 3;
            }
            else if (shape1[i] == 1) {
                out_shape[i] = shape2[i];
            }
            else if (shape2[i] != shape1[i] && shape2[i] != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shapes "
                      << shape1 << " " << shape2;
                throw std::invalid_argument(error.str());
            }
        }
        return out_shape;
    }

    /**
     * @brief Helper function. Broadcast two shapes for matrix multiplication.
     */
    template <size_t Rank>
    shape_t<Rank> broadcast_matmul(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2
    ) {
        shape_t<Rank> out_shape = shape1;
        for (size_t i = 0; i < shape1.ndim() - 2; ++i) {
            if (shape1[i] == 1) {
                out_shape[i] = shape2[i];
            }
            else if (shape2[i] != shape1[i] && shape2[i] != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shapes "
                      << shape1 << " " << shape2;
                throw std::invalid_argument(error.str());
            }
        }
        out_shape[shape1.ndim() - 2] = shape1[shape1.ndim() - 2];
        out_shape[shape2.ndim() - 1] = shape2[shape2.ndim() - 1];
        return out_shape;
    }
}

    template <class T, class Tag1, class Tag2>
    T dot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t n = a.size();
        T val = T(0);
        for (size_t i = 0; i < n; ++i) {
            val += a[i] * b[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    T vdot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t n = a.size();
        T val = T(0);
        for (size_t i = 0; i < n; ++i) {
            val += a[i] * b[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    std::complex<T> vdot(
        const base_tensor<std::complex<T>, 1, Tag1> &a,
        const base_tensor<std::complex<T>, 1, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t n = a.size();
        std::complex<T> val = T(0);
        for (size_t i = 0; i < n; ++i) {
            val += std::conj(a[i]) * b[i];
        }
        return val;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> cross(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        size_t axis
    ) {
        shape_t<Rank> shape =
            detail::broadcast_cross(a.shape(), b.shape(), axis);
        tensor<T, Rank> out(shape);
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            T v1[3], v2[3];
            for (index[axis] = 0; index[axis] < 3; ++index[axis]) {
                v1[index[axis]] = a[detail::broadcast_index(index, a.shape())];
                v2[index[axis]] = b[detail::broadcast_index(index, b.shape())];
            }
            T v_out[3] = {v1[1] * v2[2] - v2[1] * v1[2],
                          v2[0] * v1[2] - v1[0] * v2[2],
                          v1[0] * v2[1] - v2[0] * v1[1]};
            for (index[axis] = 0; index[axis] < 3; ++index[axis]) {
                out[index] = v_out[index[axis]];
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    inline T matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    ) {
        return dot(a, b);
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 2> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 1, b.shape(), 0);
        size_t m = a.shape(0), p = a.shape(1), n = b.shape(1);
        tensor<T, 2> out(m, n);
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = T(0);
                for (size_t k = 0; k < p; ++k) {
                    val += a(i, k) * b(k, j);
                }
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t m = b.shape(0), n = b.shape(1);
        tensor<T, 1> out(n);
        for (size_t j = 0; j < n; ++j) {
            T val = T(0);
            for (size_t i = 0; i < m; ++i) {
                val += a[i] * b(i, j);
            }
            out[j] = val;
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 1, b.shape(), 0);
        size_t m = a.shape(0), n = a.shape(1);
        tensor<T, 1> out(m);
        for (size_t i = 0; i < m; ++i) {
            T val = T(0);
            for (size_t j = 0; j < n; ++j) {
                val += a(i, j) * b[j];
            }
            out[i] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
        shape_t<Rank> shape = detail::broadcast_matmul(a.shape(), b.shape());
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index =
                detail::broadcast_index(out_index, a.shape());
            index_t<Rank> b_index =
                detail::broadcast_index(out_index, b.shape());
            T val = T(0);
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, 2, Tag2> &b
    ) {
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
        shape_t<Rank> shape = a.shape();
        shape[axis1] = b.shape(1);
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index = out_index;
            index_t<2> b_index(0, out_index[out_index.ndim() - 1]);
            T val = T(0);
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, 2, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
        shape_t<Rank> shape = b.shape();
        shape[axis2] = a.shape(0);
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<2> a_index(out_index[out_index.ndim() - 2], 0);
            index_t<Rank> b_index = out_index;
            T val = T(0);
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    T tensordot(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const shape_t<Rank> &a_axes,
        const shape_t<Rank> &b_axes
    ) {
        detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
        size_t size = a.size();
        index_t<Rank> index;
        auto first1 = make_const_axes_iterator(&a, index, a_axes, 0);
        auto last1 = make_const_axes_iterator(&a, index, a_axes, size);
        auto first2 = make_const_axes_iterator(&b, index, b_axes, 0);
        return std::inner_product(first1, last1, first2, T(0));
    }

    template <class T, size_t Rank1, class Tag1,
              size_t Rank2, class Tag2,
              size_t N>
    tensor<T, (Rank1 - N) + (Rank2 - N)> tensordot(
        const base_tensor<T, Rank1, Tag1> &a,
        const base_tensor<T, Rank2, Tag2> &b,
        const shape_t<N> &a_axes,
        const shape_t<N> &b_axes
    ) {
        static_assert(N <= Rank1 && N <= Rank2, "Contraction dimension must be"
                      " less or equal to tensor dimensions");
        detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
        constexpr size_t Rank = (Rank1 - N) + (Rank2 - N);
        shape_t<Rank> shape;
        // Mask axes to sum over a.
        size_t size = 1, n = 0;
        bool keep_axis1[Rank1];
        std::fill_n(keep_axis1, Rank1, true);
        for (size_t i = 0; i < a_axes.ndim(); ++i) {
            keep_axis1[a_axes[i]] = false;
            size *= a.shape(a_axes[i]);
        }
        for (size_t i = 0; i < a.ndim(); ++i) {
            if (keep_axis1[i]) {
                shape[n++] = a.shape(i);
            }
        }
        // Mask axes to sum over b.
        bool keep_axis2[Rank2];
        std::fill_n(keep_axis2, Rank2, true);
        for (size_t i = 0; i < b_axes.ndim(); ++i) {
            keep_axis2[b_axes[i]] = false;
        }
        for (size_t i = 0; i < b.ndim(); ++i) {
            if (keep_axis2[i]) {
                shape[n++] = b.shape(i);
            }
        }
        // Tensordot computation.
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank1> a_index;
            index_t<Rank2> b_index;
            // Unmask axes to sum over a.
            n = 0;
            for (size_t i = 0; i < a.ndim(); ++i) {
                if (keep_axis1[i]) {
                    a_index[i] = out_index[n++];
                }
            }
            // Unmask axes to sum over b.
            for (size_t i = 0; i < b.ndim(); ++i) {
                if (keep_axis2[i]) {
                    b_index[i] = out_index[n++];
                }
            }
            auto first1 = make_const_axes_iterator(&a, a_index, a_axes, 0);
            auto last1 = make_const_axes_iterator(&a, a_index, a_axes, size);
            auto first2 = make_const_axes_iterator(&b, b_index, b_axes, 0);
            out[out_index] = std::inner_product(first1, last1, first2, T(0));
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a, axes);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a
    ) {
        typedef conj_transpose_tag<Tag> Closure;
        return base_tensor<std::complex<T>, Rank, Closure>(a);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const shape_t<Rank> &axes
    ) {
        typedef conj_transpose_tag<Tag> Closure;
        return base_tensor<std::complex<T>, Rank, Closure>(a, axes);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a, axes);
    }

namespace ranges {
    /**
     * @brief Return the infinity norm of a vector.
     */
    template <class InputIterator, class T>
    T infnorm(InputIterator first, InputIterator last, T init) {
        if (first == last) {
            return init;
        }
        init = std::abs(*first);
        while (++first != last) {
            init = std::max(init, std::abs(*first));
        }
        return init;
    }

    /**
     * @brief Return the -infinity norm of a vector.
     */
    template <class InputIterator, class T>
    T neginfnorm(InputIterator first, InputIterator last, T init) {
        if (first == last) {
            return init;
        }
        init = std::abs(*first);
        while (++first != last) {
            init = std::min(init, std::abs(*first));
        }
        return init;
    }

    /**
     * @brief Return the p-norm of a vector.
     */
    template <class InputIterator, class T>
    T pnorm(InputIterator first, InputIterator last, T init, double p = 2) {
        T max_abs = infnorm(first, last, T());
        if (max_abs > T()) {
            while (first != last) {
                init = init + std::pow(std::abs(*first) / max_abs, p);
                ++first;
            }
            init = max_abs * std::pow(init, 1./p);
        }
        return init;
    }
}

    template <class T, size_t Rank, class Tag>
    T norm(const base_tensor<T, Rank, Tag> &a, double p) {
        if (p == 0) {
            return count_nonzero(a);
        }
        else if (p == HUGE_VAL) {
            return ranges::infnorm(a.begin(), a.end(), T(0));
        }
        else if (p == -HUGE_VAL) {
            return ranges::neginfnorm(a.begin(), a.end(), T(0));
        }
        else {
            return ranges::pnorm(a.begin(), a.end(), T(0), p);
        }
    }

    template <class T, size_t Rank, class Tag>
    T norm(const base_tensor<std::complex<T>, Rank, Tag> &a, double p) {
        if (p == 0) {
            return count_nonzero(a);
        }
        else if (p == HUGE_VAL) {
            return ranges::infnorm(a.begin(), a.end(), T(0));
        }
        else if (p == -HUGE_VAL) {
            return ranges::neginfnorm(a.begin(), a.end(), T(0));
        }
        else {
            return ranges::pnorm(a.begin(), a.end(), T(0), p);
        }
    }
}

#endif // NUMCPP_ROUTINES_TCC_INCLUDED
