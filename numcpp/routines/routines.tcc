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
        const typename tensor<T, Rank>::value_type &val
    ) {
        return tensor<T, Rank>(like.shape(), val);
    }

    /// Numerical ranges.

    template <class T>
    base_tensor<T, 1, sequence_tag> arange(const T &stop) {
        size_t size = 0;
        if (stop > 0) {
            size = std::ceil(stop);
        }
        return base_tensor<T, 1, sequence_tag>(T(), size, T(1));
    }

    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    arange(const T &start, const U &stop) {
        typedef typename std::common_type<T, U>::type Rt;
        size_t size = 0;
        if (start < stop) {
            size = std::ceil(stop - start);
        }
        return base_tensor<Rt, 1, sequence_tag>(start, size, Rt(1));
    }

    template <class T, class U, class V>
    base_tensor<typename std::common_type<T, U, V>::type, 1, sequence_tag>
    arange(const T &start, const U &stop, const V &step) {
        typedef typename std::common_type<T, U, V>::type Rt;
        size_t size = 0;
        if ((start < stop && step > 0) || (start > stop && step < 0)) {
            size = std::ceil((double)(stop - start) / step);
        }
        return base_tensor<Rt, 1, sequence_tag>(start, size, step);
    }

    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    linspace(const T &start, const U &stop, size_t num, bool endpoint) {
        typedef typename std::common_type<T, U>::type Rt;
        Rt step = stop - start;
        step /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(start, num, step);
    }

    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    logspace(const T &start, const U &stop, size_t num, bool endpoint) {
        typedef typename std::common_type<T, U>::type Rt;
        Rt step = stop - start;
        step /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(start, num, step, Rt(10));
    }

    template <class T, class U, class V>
    base_tensor<typename std::common_type<T, U, V>::type, 1, sequence_tag>
    logspace(
        const T &start, const U &stop, size_t num, bool endpoint,
        const V &base
    ) {
        typedef typename std::common_type<T, U, V>::type Rt;
        Rt step = stop - start;
        step /= num - endpoint;
        return base_tensor<Rt, 1, sequence_tag>(start, num, step, base);
    }

    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
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
    inline base_tensor<T, 2, lazy_eye_tag> eye(size_t n) {
        return base_tensor<T, 2, lazy_eye_tag>(n, n);
    }

    template <class T>
    inline base_tensor<T, 2, lazy_eye_tag>
    eye(size_t m, size_t n, ptrdiff_t k) {
        return base_tensor<T, 2, lazy_eye_tag>(m, n, k);
    }

    template <class T, class Tag>
    inline base_tensor<T, 1, lazy_diagonal_tag<Tag> >
    diag(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        typedef lazy_diagonal_tag<Tag> Closure;
        return base_tensor<T, 1, Closure>(a, k);
    }

    template <class T, class Tag>
    inline base_tensor<T, 2, lazy_diagonal_tag<Tag> >
    diag(const base_tensor<T, 1, Tag> &a, ptrdiff_t k) {
        typedef lazy_diagonal_tag<Tag> Closure;
        return base_tensor<T, 2, Closure>(a, k);
    }

    template <class T, class Tag>
    tensor<T, 2> tril(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        tensor<T, 2> out(a.shape(), T());
        if (k >= 0) {
            for (size_t i = 0; i < a.shape(0); ++i) {
                for (size_t j = 0; j <= i + k && j < a.shape(1); ++j) {
                    out(i, j) = a(i, j);
                }
            }
        }
        else {
            for (size_t j = 0; j < a.shape(1); ++j) {
                for (size_t i = j - k; i < a.shape(0); ++i) {
                    out(i, j) = a(i, j);
                }
            }
        }
        return out;
    }

    template <class T, class Tag>
    tensor<T, 2> triu(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        tensor<T, 2> out(a.shape(), T());
        if (k >= 0) {
            for (size_t i = 0; i < a.shape(0); ++i) {
                for (size_t j = i + k; j < a.shape(1); ++j) {
                    out(i, j) = a(i, j);
                }
            }
        }
        else {
            for (size_t j = 0; j < a.shape(1); ++j) {
                for (size_t i = 0; i <= j - k && i < a.shape(0); ++i) {
                    out(i, j) = a(i, j);
                }
            }
        }
        return out;
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
        layout_t order = a.layout();
        return unravel_index(index, a.shape(), order);
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
        layout_t order = a.layout();
        return unravel_index(index, a.shape(), order);
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
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<ranges::maximum, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::maximum(), a, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag>
    > maximum(
        const typename tensor<T, Rank>::value_type &val,
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
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<ranges::minimum, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::minimum(), a, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag>
    > minimum(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &b
    ) {
        typedef lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(ranges::minimum(), val, b);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<ranges::clamp<T>, T, Tag> >
    clamp(
        const base_tensor<T, Rank, Tag> &a,
        const typename tensor<T, Rank>::value_type &a_min,
        const typename tensor<T, Rank>::value_type &a_max
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
        const typename tensor<T, Rank>::value_type &rtol,
        const typename tensor<T, Rank>::value_type &atol
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
        const typename tensor<T, Rank>::value_type &val,
        const typename tensor<T, Rank>::value_type &rtol,
        const typename tensor<T, Rank>::value_type &atol
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
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename tensor<T, Rank>::value_type &rtol,
        const typename tensor<T, Rank>::value_type &atol
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
        const typename tensor<std::complex<T>, Rank>::value_type &val,
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
        const typename tensor<std::complex<T>, Rank>::value_type &val,
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
        const typename tensor<T, Rank>::value_type &rtol,
        const typename tensor<T, Rank>::value_type &atol
    ) {
        return all(isclose(a, b, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename tensor<T, Rank>::value_type &val,
        const typename tensor<T, Rank>::value_type &rtol,
        const typename tensor<T, Rank>::value_type &atol
    ) {
        return all(isclose(a, val, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename tensor<T, Rank>::value_type &rtol,
        const typename tensor<T, Rank>::value_type &atol
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
        const typename tensor<std::complex<T>, Rank>::value_type &val,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol
    ) {
        return all(isclose(a, val, rtol, atol));
    }

    template <class T, size_t Rank, class Tag>
    inline bool allclose(
        const typename tensor<std::complex<T>, Rank>::value_type &val,
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
        shape_t<Rank> shape = a.shape();
        size_t size = a.size();
        tensor<index_t<Rank>, 1> out(make_indices(shape).begin(), size);
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
            auto first = make_reduce_iterator(&out, out_index, axis, 0);
            auto last = make_reduce_iterator(&out, out_index, axis, size);
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
        shape_t<Rank> shape = a.shape();
        size_t size = a.size();
        tensor<index_t<Rank>, 1> out(make_indices(shape).begin(), size);
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
            auto first = make_reduce_iterator(&out, out_index, axis, 0);
            auto last = make_reduce_iterator(&out, out_index, axis, size);
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

    template <class T, size_t Rank,
              class TagCond, class TagTrue, class TagFalse>
    base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagTrue> &x,
        const base_tensor<T, Rank, TagFalse> &y
    ) {
        typedef lazy_where_tag<TagCond, TagTrue, TagFalse> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    template <class T, size_t Rank, class TagCond, class TagTrue>
    base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagTrue> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_where_tag<TagCond, TagTrue, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    template <class T, size_t Rank, class TagCond, class TagFalse>
    base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, TagFalse> &y
    ) {
        typedef lazy_where_tag<TagCond, scalar_tag, TagFalse> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    template <class T, size_t Rank, class TagCond>
    base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const T &x,
        const T &y
    ) {
        typedef lazy_where_tag<TagCond, scalar_tag, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(condition, x, y);
    }

    /// Rearranging elements.

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_reverse_tag<Tag, 1> > reverse(
        const base_tensor<T, Rank, Tag> &arg, size_t axis
    ) {
        typedef lazy_reverse_tag<Tag, 1> Closure;
        return base_tensor<T, Rank, Closure>(arg, axis);
    }

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, lazy_reverse_tag<Tag, N> > reverse(
        const base_tensor<T, Rank, Tag> &arg, const shape_t<N> &axes
    ) {
        typedef lazy_reverse_tag<Tag, N> Closure;
        return base_tensor<T, Rank, Closure>(arg, axes);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_shift_tag<Tag, 1> > shift(
        const base_tensor<T, Rank, Tag> &arg,
        size_t count, size_t axis
    ) {
        typedef lazy_shift_tag<Tag, 1> Closure;
        return base_tensor<T, Rank, Closure>(arg, count, axis);
    }

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, lazy_shift_tag<Tag, N> > shift(
        const base_tensor<T, Rank, Tag> &arg,
        const index_t<N> &count, const shape_t<N> &axes
    ) {
        typedef lazy_shift_tag<Tag, N> Closure;
        return base_tensor<T, Rank, Closure>(arg, count, axes);
    }

}

#endif // NUMCPP_ROUTINES_TCC_INCLUDED
