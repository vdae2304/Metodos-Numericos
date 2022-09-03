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
