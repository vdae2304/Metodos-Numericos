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

#include <algorithm>
#include <numeric>
#include <sstream>
#include <stdexcept>

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
    tensor<T, 2> tril(const base_tensor<T, 2, Tag> &mat, ptrdiff_t k) {
        tensor<T, 2> out(mat.shape(), T());
        if (k >= 0) {
            for (size_t i = 0; i < mat.shape(0); ++i) {
                for (size_t j = 0; j <= i + k && j < mat.shape(1); ++j) {
                    out(i, j) = mat(i, j);
                }
            }
        }
        else {
            for (size_t j = 0; j < mat.shape(1); ++j) {
                for (size_t i = j - k; i < mat.shape(0); ++i) {
                    out(i, j) = mat(i, j);
                }
            }
        }
        return out;
    }

    template <class T, class Tag>
    tensor<T, 2> triu(const base_tensor<T, 2, Tag> &mat, ptrdiff_t k) {
        tensor<T, 2> out(mat.shape(), T());
        if (k >= 0) {
            for (size_t i = 0; i < mat.shape(0); ++i) {
                for (size_t j = i + k; j < mat.shape(1); ++j) {
                    out(i, j) = mat(i, j);
                }
            }
        }
        else {
            for (size_t j = 0; j < mat.shape(1); ++j) {
                for (size_t i = 0; i <= j - k && i < mat.shape(0); ++i) {
                    out(i, j) = mat(i, j);
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

    /// Sorting and searching.

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
