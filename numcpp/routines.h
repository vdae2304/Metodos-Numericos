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
#include "numcpp/routines/lazy_sequence.h"
#include "numcpp/routines/lazy_diagonal.h"
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

    /// Numerical ranges.

    /**
     * @brief Return evenly spaced values within a given interval.
     *
     * @details Values are generated within the half-open interval
     * [start, stop), i.e., the interval including start but excluding stop,
     * with spacing between values given by step.
     *
     * @param start Start of interval. Defaults to 0 if not provided.
     * @param stop End of interval.
     * @param step Spacing between values. This is the distance between two
     *     adjacent values. Defaults to 1 if not provided.
     *
     * @return A light-weight object with evenly spaced values. This function
     *     does not create a new tensor, instead, it returns an expression
     *     object with evenly spaced values in a given interval.
     */
    template <class T>
    base_tensor<T, 1, sequence_tag> arange(const T &stop);

    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    arange(const T &start, const U &stop);

    template <class T, class U, class V>
    base_tensor<typename std::common_type<T, U, V>::type, 1, sequence_tag>
    arange(const T &start, const U &stop, const V &step);

    /**
     * @brief Return evenly spaced numbers over a specified interval.
     *
     * @details Return num evenly spaced samples, calculated over the interval
     * [start, stop]. The endpoint of the interval can optionally be excluded.
     *
     * @param start The starting value of the sequence.
     * @param stop The end value of the sequence.
     * @param num Number of samples to generate. Default is 50.
     * @param endpoint If true, stop is the last sample. Otherwise, it is not
     *     included. Default is true.
     *
     * @return A light-weight object with equally spaced samples. This function
     *     does not create a new tensor, instead, it returns an expression
     *     object with equally spaced samples over an interval.
     */
    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    linspace(
        const T &start, const U &stop, size_t num = 50, bool endpoint = true
    );

    /**
     * @brief Return numbers spaced evenly on a log scale.
     *
     * @details In linear space, the sequence starts at pow(base, start) (base
     * to the power of start) and ends with pow(base, stop).
     *
     * @param start pow(base, start) is the starting value of the sequence.
     * @param stop pow(base, stop) is the final value of the sequence.
     * @param num Number of samples to generate. Default is 50.
     * @param endpoint If true, stop is the last sample. Otherwise, it is not
     *     included. Default is true.
     * @param base The base of the log space. Default is 10.
     *
     * @return A light-weight object with equally spaced samples on a log
     *     scale. This function does not create a new tensor, instead, it
     *     returns an expression object with equally spaced samples on a log
     *     scale.
     */
    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    logspace(
        const T &start, const U &stop, size_t num = 50, bool endpoint = true
    );

    template <class T, class U, class V>
    base_tensor<typename std::common_type<T, U, V>::type, 1, sequence_tag>
    logspace(
        const T &start, const U &stop, size_t num, bool endpoint,
        const V &base
    );

    /**
     * @brief Return numbers spaced evenly on a log scale (a geometric
     * progression).
     *
     * @details This is similar to logspace, but with endpoints specified
     * directly. Each output sample is a constant multiple of the previous.
     *
     * @param start The starting value of the sequence.
     * @param stop The final value of the sequence.
     * @param num Number of samples to generate. Default is 50.
     * @param endpoint If true, stop is the last sample. Otherwise, it is not
     *     included. Default is true.
     *
     * @return A light-weight object with equally spaced samples on a log
     *     scale. This function does not create a new tensor, instead, it
     *     returns an expression object with equally spaced samples on a log
     *     scale.
     */
    template <class T, class U>
    base_tensor<typename std::common_type<T, U>::type, 1, sequence_tag>
    geomspace(
        const T &start, const U &stop, size_t num = 50, bool endpoint = true
    );

    /// Building matrices.

    /**
     * @brief Return a matrix with ones on the diagonal and zeros elsewhere.
     *
     * @tparam T Type of the elements contained in the tensor.
     *
     * @param m Number of rows. If not provided, defaults to n.
     * @param n Number of columns.
     * @param k Offset of the diagonal from the main diagonal. A positive value
     *     refers to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return A light-weight object with ones on the diagonal and zeros
     *     elsewhere. This function, does not create a new tensor, instead, it
     *     returns an expression object with ones on the diagonal and zeros
     *     elsewhere.
     */
    template <class T>
    base_tensor<T, 2, lazy_eye_tag> eye(size_t n);

    template <class T>
    base_tensor<T, 2, lazy_eye_tag> eye(size_t m, size_t n, ptrdiff_t k = 0);

    /**
     * @brief Extract a diagonal or construct a diagonal matrix.
     *
     * @details If a is a 2-dimensional tensor, returns the diagonal of a with
     * the given offset, i.e., an array with the elements of the form
     * a(i, i + k). If a is a 1-dimensional tensor, returns a matrix with
     * a on the k-th diagonal.
     *
     * @param a Matrix from which the diagonal is taken or array with the
     *     elements that will be on the diagonal.
     * @param k Offset of the diagonal from the main diagonal. A positive value
     *     refers to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return The extracted diagonal or the constructed diagonal matrix. This
     *     function does not create a new tensor, instead, it returns a
     *     readonly view with the extracted diagonal or the constructed
     *     diagonal matrix.
     */
    template <class T, class Tag>
    base_tensor<T, 1, lazy_diagonal_tag<Tag> >
    diag(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

    template <class T, class Tag>
    base_tensor<T, 2, lazy_diagonal_tag<Tag> >
    diag(const base_tensor<T, 1, Tag> &a, ptrdiff_t k = 0);

    /**
     * @brief Return the lower triangle of a matrix.
     *
     * @param a Matrix from which the lower triangle is taken.
     * @param k Diagonal above which to zero elements. A positive value refers
     *     to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return Lower triangle of a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    tensor<T, 2> tril(const base_tensor<T, 2, Tag> &mat, ptrdiff_t k = 0);

    /**
     * @brief Return the upper triangle of a matrix.
     *
     * @param a Matrix from which the upper triangle is taken.
     * @param k Diagonal below which to zero elements. A positive value refers
     *     to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return Upper triangle of a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    tensor<T, 2> triu(const base_tensor<T, 2, Tag> &mat, ptrdiff_t k = 0);

    /**
     * @brief Generate a Vandermonde matrix.
     *
     * @details The columns of the output matrix are powers of the input
     * vector. The order of the powers is determined by the increasing boolean
     * argument.
     *
     * @param x One-dimensional input tensor.
     * @param N Number of columns in the output. If N is not specified, a
     *     square matrix is returned (N = x.size()).
     * @param increasing Order of the powers of the columns. If true, the
     *     powers increase from left to right. If false, they are reversed.
     *     Default is false.
     *
     * @return Vandermonde matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    tensor<T, 2> vander(const base_tensor<T, 1, Tag> &x);

    template <class T, class Tag>
    tensor<T, 2> vander(
        const base_tensor<T, 1, Tag> &x, size_t N, bool increasing = false
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
