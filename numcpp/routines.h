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

 /** @file include/numcpp/routines.h
  *  This header defines an assortment of routines for tensor objects.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ROUTINES_H_INCLUDED
#define NUMCPP_ROUTINES_H_INCLUDED

#include "numcpp/tensor.h"
#include "numcpp/routines/lazy_where.h"
#include "numcpp/routines/lazy_reverse.h"
#include "numcpp/routines/lazy_shift.h"

namespace numcpp {
    /// Tensor creation routines.

    /**
     * @brief Return a new tensor of given shape without initializing entries.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor.
     *
     * @param shape Shape of the empty tensor.
     *
     * @return A tensor of uninitialized data with the given shape.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank>
    tensor<T, Rank> empty(const shape_t<Rank> &shape);

    /**
     * @brief Return a new tensor with the same shape and type as a given
     * tensor.
     *
     * @param like Tensor-like object defining the shape and data type.
     *
     * @return A tensor of uninitialized data with the same shape and type as
     *     like.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> empty_like(const base_tensor<T, Rank, Tag> &like);

    /**
     * @brief Return a new tensor of given shape filled with zeros.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor.
     *
     * @param shape Shape of the new tensor.
     *
     * @return A tensor of zeros with the given shape.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank>
    tensor<T, Rank> zeros(const shape_t<Rank> &shape);

    /**
     * @brief Return a new tensor of zeros with the same shape and type as a
     * given tensor.
     *
     * @param like Tensor-like object defining the shape and data type.
     *
     * @return A tensor of zeros with the same shape and type as like.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> zeros_like(const base_tensor<T, Rank, Tag> &like);

    /**
     * @brief Return a new tensor of given shape filled with ones.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor.
     *
     * @param shape Shape of the new tensor.
     *
     * @return A tensor of ones with the given shape.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank>
    tensor<T, Rank> ones(const shape_t<Rank> &shape);

    /**
     * @brief Return a new tensor of ones with the same shape and type as a
     * given tensor.
     *
     * @param like Tensor-like object defining the shape and data type.
     *
     * @return A tensor of ones with the same shape and type as like.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> ones_like(const base_tensor<T, Rank, Tag> &like);

    /**
     * @brief Return a new tensor of given shape filled with val.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor.
     *
     * @param shape Shape of the new tensor.
     * @param val Fill value.
     *
     * @return A tensor of val with the given shape.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank>
    tensor<T, Rank> full(const shape_t<Rank> &shape, const T &val);

    /**
     * @brief Return a new tensor with the same shape and type as a given
     * tensor filled with val.
     *
     * @param like Tensor-like object defining the shape and data type.
     * @param val Fill value.
     *
     * @return A tensor of val with the same shape and type as like.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> full_like(
        const base_tensor<T, Rank, Tag> &like,
        const typename tensor<T, Rank>::value_type &val
    );

    /// Sorting and searching.

    /**
     * @brief Return elements chosen from two tensors depending on condition.
     *
     * @param condition A tensor-like object of bool, where true, yield x,
     *     otherwise yield y.
     * @param x A single value or a tensor-like object with the values from
     *     which to choose where condition is true.
     * @param y A single value or a tensor-like object with the values from
     *     which to choose where condition is false.
     *
     * @return A light-weight object with the elements chosen from x where
     *     condition is true and from y where condition is false. This function
     *     does not create a new tensor, instead, an expression object is
     *     returned.
     *
     * @throw std::invalid_argument Thrown if condition, x and y shapes are not
     *     compatible and cannot be broadcasted according to broadcasting
     *     rules.
     */
    template <class T, size_t Rank,
              class TagCond, class TagTrue, class TagFalse>
    base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagTrue> &x,
        const base_tensor<T, Rank, TagFalse> &y
    );

    template <class T, size_t Rank, class TagCond, class TagTrue>
    base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagTrue> &x,
        const typename tensor<T, Rank>::value_type &y
    );

    template <class T, size_t Rank, class TagCond, class TagFalse>
    base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, TagFalse> &y
    );

    template <class T, size_t Rank, class TagCond>
    base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const T &x,
        const T &y
    );

    /// Rearranging elements.

    /**
     * @brief Reverse the order of the elements in a tensor along the given
     * axes.
     *
     * @param arg A tensor-like object to reverse.
     * @param axes Axis or axes along which to reverse over. The default is
     *     Rank - 1, which reverses along the last axis.
     *
     * @return A light-weight object with the elements in the tensor in
     *     reversed order. This function does not create a new tensor, instead,
     *     it returns a readonly view of the tensor with its elements in
     *     reversed order over the given axes.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_reverse_tag<Tag, 1> > reverse(
        const base_tensor<T, Rank, Tag> &arg, size_t axis = Rank - 1
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, lazy_reverse_tag<Tag, N> > reverse(
        const base_tensor<T, Rank, Tag> &arg, const shape_t<N> &axes
    );

    /**
     * @brief Shift the elements in a tensor along the given axes. The elements
     * are circularly shifted in such a way that the element at position count
     * becomes the first element.
     *
     * @param arg A tensor-like object to shift.
     * @param count Number of positions to shift the elements by along each
     *     axis.
     * @param axes Axis or axes along which the elements are shifted. The
     *     default is Rank - 1, which shifts along the last axis.
     *
     * @return A light-weight object with the elements in the tensor circularly
     *     shifted. This function does not create a new tensor, instead, it
     *     returns a readonly view of the tensor with its elements circularly
     *     shifted over the given axes.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_shift_tag<Tag, 1> > shift(
        const base_tensor<T, Rank, Tag> &arg,
        size_t count, size_t axis = Rank - 1
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, lazy_shift_tag<Tag, N> > shift(
        const base_tensor<T, Rank, Tag> &arg,
        const index_t<N> &count, const shape_t<N> &axes
    );
}

#include "numcpp/routines/routines.tcc"

#endif // NUMCPP_ROUTINES_H_INCLUDED
