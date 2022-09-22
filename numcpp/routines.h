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

#include "numcpp/config.h"

#include "numcpp/routines/ranges.h"
#include "numcpp/routines/sequence_array.h"
#include "numcpp/routines/diagonal_view.h"

#include "numcpp/routines/lazy_where.h"
#include "numcpp/routines/reverse_view.h"
#include "numcpp/routines/transpose_view.h"

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
        const typename base_tensor<T, Rank, Tag>::value_type &val
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
    base_tensor<T, 2, eye_tag> eye(size_t n);

    template <class T>
    base_tensor<T, 2, eye_tag> eye(size_t m, size_t n, ptrdiff_t k = 0);

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
     *     readonly view with the extracted diagonal (when a is 2-dimensional)
     *     or with the constructed diagonal matrix (when a is 1-dimensional).
     */
    template <class T, class Tag>
    base_tensor<T, 1, diagonal_tag<Tag> >
    diag(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

    template <class T, class Tag>
    base_tensor<T, 2, diagonal_tag<Tag> >
    diag(const base_tensor<T, 1, Tag> &a, ptrdiff_t k = 0);

    /**
     * @brief Return the lower triangle of a matrix.
     *
     * @param a Matrix from which the lower triangle is taken.
     * @param k Diagonal above which to zero elements. A positive value refers
     *     to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return Lower triangle of a. This function does not create a new tensor,
     *     instead, it returns a readonly view with elements above the k-th
     *     diagonal zeroed.
     */
    template <class T, class Tag>
    base_tensor<T, 2, triangular_tag<Tag> >
    tril(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

    /**
     * @brief Return the upper triangle of a matrix.
     *
     * @param a Matrix from which the upper triangle is taken.
     * @param k Diagonal below which to zero elements. A positive value refers
     *     to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return Upper triangle of a. This function does not create a new tensor,
     *     instead, it returns a readonly view with elements below the k-th
     *     diagonal zeroed.
     */
    template <class T, class Tag>
    base_tensor<T, 2, triangular_tag<Tag> >
    triu(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

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

    /// Maximums and minimums.

    /**
     * @brief Return the indices of the maximum value in the tensor.
     *
     * @param a A tensor-like object.
     *
     * @return The indices of the maximum value in the tensor. In case of
     *     multiple occurrences of the maximum value, return the index
     *     corresponding to the first occurrence.
     */
    template <class T, size_t Rank, class Tag>
    index_t<Rank> argmax(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the indices of the maximum value in the tensor along the
     * given axis.
     *
     * @param a A tensor-like object.
     * @param axis Axis along which the maximum value is selected.
     *
     * @return A new tensor with the indices of the maximum value along an axis.
     *     The output tensor will have the same dimension and the same shape,
     *     except that the axis which is reduced is left as a dimension of size
     *     one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank>
    argmax(const base_tensor<T, Rank, Tag> &a, size_t axis);

    /**
     * @brief Return the indices of the minimum value in the tensor.
     *
     * @param a A tensor-like object.
     *
     * @return The indices of the minimum value in the tensor. In case of
     *     multiple occurrences of the minimum value, return the index
     *     corresponding to the first occurrence.
     */
    template <class T, size_t Rank, class Tag>
    index_t<Rank> argmin(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the indices of the minimum value in the tensor along the
     * given axis.
     *
     * @param a A tensor-like object.
     * @param axis Axis along which the minimum value is selected.
     *
     * @return A new tensor with the indices of the minimum value along an axis.
     *     The output tensor will have the same dimension and the same shape,
     *     except that the axis which is reduced is left as a dimension of size
     *     one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank>
    argmin(const base_tensor<T, Rank, Tag> &a, size_t axis);

    /**
     * @brief Return the maximum value contained in the tensor.
     *
     * @param a A tensor-like object.
     *
     * @return The maximum value in the tensor.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    amax(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the maximum value contained in the tensor over the given
     * axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the maximum value
     *     is selected.
     *
     * @return A new tensor with the maximum value over the axes. The output
     *     tensor will have the same dimension and the same shape, except that
     *     the axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amax(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amax(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return the minimum value contained in the tensor.
     *
     * @param a A tensor-like object.
     *
     * @return The minimum value in the tensor.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    amin(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the minimum value contained in the tensor over the given
     * axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the minimum value
     *     is selected.
     *
     * @return A new tensor with the minimum value over the axes. The output
     *     tensor will have the same dimension and the same shape, except that
     *     the axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amin(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    amin(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return the element-wise maximum of two tensors.
     *
     * @param a First tensor-like object to compare.
     * @param b Second tensor-like object to compare.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     *
     * @return A light-weight object with the element-wise maximum of two
     *     tensors. This function does not create a new tensor, instead, it
     *     returns an expression object with the element-wise maximum of the
     *     tensor arguments.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    base_tensor<T, Rank, lazy_binary_tag<ranges::maximum, T, Tag1, T, Tag2> >
    maximum(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, Tag, T, scalar_tag>
    > maximum(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag>
    > maximum(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b
    );

    /**
     * @brief Return the element-wise minimum of two tensors.
     *
     * @param a First tensor-like object to compare.
     * @param b Second tensor-like object to compare.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     *
     * @return A light-weight object with the element-wise minimum of two
     *     tensors. This function does not create a new tensor, instead, it
     *     returns an expression object with the element-wise minimum of the
     *     tensor arguments.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    base_tensor<T, Rank, lazy_binary_tag<ranges::minimum, T, Tag1, T, Tag2> >
    minimum(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, Tag, T, scalar_tag>
    > minimum(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag>
    > minimum(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b
    );

    /**
     * @brief Clamp the values in the tensor. Given an interval [a_min, a_max],
     * values smaller than a_min become a_min, and values larger than a_max
     * become a_max. For complex types, the real and imaginary parts are
     * clamped separately.
     *
     * @param a A tensor-like object with the elements to clamp.
     * @param a_min The lower boundary to clamp.
     * @param a_max The upper boundary to clamp.
     *
     * @return A light-weight object with the tensor formed by clamping the
     *     values in the given tensor. This function does not create a new
     *     tensor, instead, it returns a readonly view with the clamped values
     *     in the tensor.
     *
     * @note The behavior is undefined if a_min is greater than a_max.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_unary_tag<ranges::clamp<T>, T, Tag> >
    clamp(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &a_min,
        const typename base_tensor<T, Rank, Tag>::value_type &a_max
    );

    /// Sums and products.

    /**
     * @brief Return the sum of the tensor elements.
     *
     * @param a A tensor-like object.
     *
     * @return The sum of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    sum(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the sum of the tensor elements over the given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the sum is
     *     performed.
     *
     * @return A new tensor with the sum over the axes. The output tensor will
     *     have the same dimension and the same shape, except that the axes
     *     which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sum(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sum(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return the product of the tensor elements.
     *
     * @param a A tensor-like object.
     *
     * @return The product of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    prod(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the product of the tensor elements over the given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the product is
     *     performed.
     *
     * @return A new tensor with the product over the axes. The output tensor
     *     will have the same dimension and the same shape, except that the
     *     axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    prod(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    prod(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return the cumulative sum of the tensor elements along a given
     * axis.
     *
     * @param a A tensor-like object.
     * @param axis Axis along which the cumulative sum is computed. Default is
     *     zero.
     *
     * @return The cumulative sum of the tensor elements. The output tensor
     *     will have the same dimension and the same shape as a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    cumsum(const base_tensor<T, Rank, Tag> &a, size_t axis = 0);

    /**
     * @brief Return the cumulative product of the tensor elements along a
     * given axis.
     *
     * @param a A tensor-like object.
     * @param axis Axis along which the cumulative product is computed. Default
     *     is zero.
     *
     * @return The cumulative product of the tensor elements. The output tensor
     *     will have the same dimension and the same shape as a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    cumprod(const base_tensor<T, Rank, Tag> &a, size_t axis = 0);

    /// Logic functions.

    /**
     * @brief Test whether all tensor elements evaluates to true.
     *
     * @param a A tensor-like object of bool.
     *
     * @return true if all the elements evaluates to true and false otherwise.
     */
    template <size_t Rank, class Tag>
    bool all(const base_tensor<bool, Rank, Tag> &a);

    /**
     * @brief Test whether all tensor elements over the given axes evaluates to
     * true.
     *
     * @param a A tensor-like object of bool.
     * @param axes A shape_t object with the axes along which the logical AND
     *     reduction is performed.
     *
     * @return A new tensor with the results of the test over the axes. The
     *     output tensor will have the same dimension and the same shape,
     *     except that the axes which are reduced are left as dimensions of
     *     size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <size_t Rank, class Tag>
    tensor<bool, Rank>
    all(const base_tensor<bool, Rank, Tag> &a, size_t axis);

    template <size_t Rank, class Tag, size_t N>
    tensor<bool, Rank>
    all(const base_tensor<bool, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Test whether any tensor element evaluates to true.
     *
     * @param a A tensor-like object of bool.
     *
     * @return true if any element evaluates to true and false otherwise.
     */
    template <size_t Rank, class Tag>
    bool any(const base_tensor<bool, Rank, Tag> &a);

    /**
     * @brief Test whether any tensor element over the given axes evaluates to
     * true.
     *
     * @param a A tensor-like object of bool.
     * @param axes A shape_t object with the axes along which the logical OR
     *     reduction is performed.
     *
     * @return A new tensor with the results of the test over the axes. The
     *     output tensor will have the same dimension and the same shape,
     *     except that the axes which are reduced are left as dimensions of
     *     size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <size_t Rank, class Tag>
    tensor<bool, Rank>
    any(const base_tensor<bool, Rank, Tag> &a, size_t axis);

    template <size_t Rank, class Tag, size_t N>
    tensor<bool, Rank>
    any(const base_tensor<bool, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Count the number of non-zero elements in the tensor.
     *
     * @param a A tensor-like object.
     *
     * @return The number of non-zero elements.
     */
    template <class T, size_t Rank, class Tag>
    size_t count_nonzero(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Count the number of non-zero elements over the given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which to count
     *     non-zeros.
     *
     * @return A new tensor with the number of non-zero elements over the axes.
     *     The output tensor will have the same dimension and the same shape,
     *     except that the axes which are reduced are left as dimensions of
     *     size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank>
    count_nonzero(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<size_t, Rank>
    count_nonzero(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return if two numbers are equal within a tolerance.
     *
     * @details Whether or not two values are considered equal is determined
     * according to given absolute and relative tolerance. The tolerances must
     * be non-negative, typically very small numbers. For floating-point
     * values, the function uses the following equation to test whether two
     * numbers are equivalent:
     *     abs(a - b) <= fmax(rtol * fmax(abs(a), abs(b)), atol)
     * NaN is not considered equal to any other value, including NaN. inf and
     * -inf are only considered equal to themselves.
     *
     * @param a An integer, floating-point or complex value.
     * @param b An integer, floating-point or complex value.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return true if the values are considered equal and false otherwise.
     */
    bool isclose(float a, float b, float rtol = 1e-4f, float atol = 0.f);

    bool isclose(double a, double b, double rtol = 1e-8, double atol = 0.);

    bool isclose(
        long double a, long double b,
        long double rtol = 1e-10L, long double atol = 0.L
    );

    /**
     * @brief Additional overloads for arithmetic types.
     *
     * @details If any argument has integral type, it is cast to double.
     * If any argument is long double, the other argument is promoted to long
     * double.
     * If arguments are float and double (in any order), the float argument is
     * promoted to double.
     */
    template <class T, class U>
    typename std::enable_if<
        std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, bool
    >::type isclose(
        T a, U b,
        typename std::common_type<T, U, double>::type rtol = 1e-8,
        typename std::common_type<T, U, double>::type atol = 0.
    );

    /**
     * @brief Additional overloads for complex types. For complex types, the
     * equality is tested on both real and imaginary parts.
     */
    template <class T>
    bool isclose(
        const std::complex<T> &a, const std::complex<T> &b,
        typename std::complex<T>::value_type rtol = T(1e-8),
        typename std::complex<T>::value_type atol = T(0)
    );

    template <class T>
    bool isclose(
        const std::complex<T> &a,
        const typename std::complex<T>::value_type &b,
        typename std::complex<T>::value_type rtol = T(1e-8),
        typename std::complex<T>::value_type atol = T(0)
    );

    template <class T>
    bool isclose(
        const typename std::complex<T>::value_type &a,
        const std::complex<T> &b,
        typename std::complex<T>::value_type rtol = T(1e-8),
        typename std::complex<T>::value_type atol = T(0)
    );

    template <class T, class U>
    bool isclose(
        const std::complex<T> &a, const std::complex<U> &b,
        typename std::common_type<T, U>::type rtol = 1e-8,
        typename std::common_type<T, U>::type atol = 0
    );

    /**
     * @brief Return if two tensors are equal, element-wise, within a
     * tolerance.
     *
     * @param a First tensor-like object to compare.
     * @param b Second tensor-like object to compare.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return A light-weight object of bool with each element set to true
     *     where a and b are equal within the given tolerance, and false
     *     otherwise. This function does not create a new tensor, instead, it
     *     returns an expression object with the element-wise comparison.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, Tag1, T, Tag2>
    > isclose(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const typename base_tensor<T, Rank, Tag1>::value_type &rtol = T(1e-8),
        const typename base_tensor<T, Rank, Tag2>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, Tag, T, scalar_tag>
    > isclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol = T(1e-8),
        const typename base_tensor<T, Rank, Tag>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, scalar_tag, T, Tag>
    > isclose(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol = T(1e-8),
        const typename base_tensor<T, Rank, Tag>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, Tag1, T, Tag2
        >
    > isclose(
        const base_tensor<std::complex<T>, Rank, Tag1> &a,
        const base_tensor<std::complex<T>, Rank, Tag2> &b,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, Tag, T, scalar_tag
        >
    > isclose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, scalar_tag, T, Tag
        >
    > isclose(
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const base_tensor<std::complex<T>, Rank, Tag> &b,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    /**
     * @brief Test whether all elements in two tensors are element-wise equal
     * within a tolerance.
     *
     * @param a First tensor-like object to compare.
     * @param b Second tensor-like object to compare.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     * @param rtol The relative tolerance.
     * @param atol The absolute tolerance.
     *
     * @return true if all the elements are considered equal and false
     *     otherwise.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    bool allclose(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const typename base_tensor<T, Rank, Tag1>::value_type &rtol = T(1e-8),
        const typename base_tensor<T, Rank, Tag2>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol = T(1e-8),
        const typename base_tensor<T, Rank, Tag>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol = T(1e-8),
        const typename base_tensor<T, Rank, Tag>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    bool allclose(
        const base_tensor<std::complex<T>, Rank, Tag1> &a,
        const base_tensor<std::complex<T>, Rank, Tag2> &b,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const base_tensor<std::complex<T>, Rank, Tag> &b,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    /// Sorting and searching.

    /**
     * @brief Return the indices that would sort the tensor.
     *
     * @param a Tensor-like object to sort.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements with
     *     equivalent values. Otherwise, equivalent elements are not guaranteed
     *     to keep their original relative order.
     *
     * @return A one-dimensional tensor of indices that sort the tensor. If
     *     a is a tensor, then a[argsort(a)] yields a sorted a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it
     *     may throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> argsort(const base_tensor<T, Rank, Tag> &a);

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<index_t<Rank>, 1> argsort(
        const base_tensor<T, Rank, Tag> &a, Compare comp, bool stable = false
    );

    /**
     * @brief Return the indices that would sort the tensor along the given
     * axis.
     *
     * @param a Tensor-like object to sort.
     * @param axis Axis along which to sort.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements with
     *     equivalent values. Otherwise, equivalent elements are not guaranteed
     *     to keep their original relative order.
     *
     * @return A tensor of indices of the same shape as a that sort the tensor
     *     along the given axis. If a is a tensor, then
     *     take_along_axis(a, argsort(a, axis), axis) yields a sorted a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it
     *     may throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank> argsort(
        const base_tensor<T, Rank, Tag> &a, size_t axis
    );

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<size_t, Rank> argsort(
        const base_tensor<T, Rank, Tag> &a, size_t axis,
        Compare comp, bool stable = false
    );

    /**
     * @brief Return a sorted copy of the flattened tensor.
     *
     * @param a Tensor-like object to sort.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return A sorted copy of the flattened tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    sort(const base_tensor<T, Rank, Tag> &a);

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    sort(const base_tensor<T, Rank, Tag> &a, Compare comp, bool stable = false);

    /**
     * @brief Return a sorted copy of the tensor.
     *
     * @param a Tensor-like object to sort.
     * @param axis Axis along which to sort.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     * @param stable If true, preserve the relative order of the elements
     *     with equivalent values. Otherwise, equivalent elements are not
     *     guaranteed to keep their original relative order.
     *
     * @return A sorted copy of the tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sort(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    sort(
        const base_tensor<T, Rank, Tag> &a, size_t axis,
        Compare comp, bool stable = false
    );

    /**
     * @brief Return the indices that would partition the tensor.
     *
     * @param a Tensor-like object to partition.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     tensor. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A one-dimensional tensor of indices that partitions the tensor.
     *     If a is a tensor, then a[argpartition(a, kth)] yields a partitioned
     *     a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth
    );

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<index_t<Rank>, 1> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, Compare comp
    );

    /**
     * @brief Return the indices that would partition the tensor along the
     * given axis.
     *
     * @param a Tensor-like object to partition.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     tensor. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param axis Axis along which to partition.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A tensor of indices of the same shape as a that partitions the
     *     tensor along the given axis. If a is a tensor, then
     *     take_along_axis(a, argpartition(a, kth, axis), axis) yields a
     *     partitioned a.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<size_t, Rank> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis
    );

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<size_t, Rank> argpartition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis,
        Compare comp
    );

    /**
     * @brief Return a partitioned copy of the flattened tensor.
     *
     * @param a Tensor-like object to partition.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     tensor. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A partitioned copy of the flattened tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    partition(const base_tensor<T, Rank, Tag> &a, size_t kth);

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    partition(const base_tensor<T, Rank, Tag> &a, size_t kth, Compare comp);

    /**
     * @brief Return a partitioned copy of the tensor.
     *
     * @param a Tensor-like object to partition.
     * @param kth Element index to partition by. The element at the kth
     *     position is the element that would be in that position in the sorted
     *     tensor. The other elements are left without any specific order,
     *     except that none of the elements preceding kth are greater than it,
     *     and none of the elements following it are less.
     * @param axis Axis along which to partition.
     * @param comp Custom comparator. A binary function that accepts two
     *     elements of type T as arguments, and returns a value convertible to
     *     bool. The value returned indicates whether the element passed as
     *     first argument is considered to go before the second.
     *
     * @return A partitioned copy of the tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    partition(const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis);

    template <class T, size_t Rank, class Tag, class Compare,
              detail::RequiresCallable<Compare, T, T> = 0>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    partition(
        const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis,
        Compare comp
    );

    /**
     * @brief Return the indices of the elements that are non-zero.
     *
     * @param a A tensor-like object.
     *
     * @return A new tensor with the indices of the elements that are non-zero.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> nonzero(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return elements chosen from two tensors depending on condition.
     * When only condition is provided, this function is equivalent to nonzero.
     *
     * @param condition A tensor-like object of bool. Where true, yield x,
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
    template <size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> where(
        const base_tensor<bool, Rank, Tag> &condition
    );

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
        const typename base_tensor<T, Rank, TagTrue>::value_type &y
    );

    template <class T, size_t Rank, class TagCond, class TagFalse>
    base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse> >
    where(
        const base_tensor<bool, Rank, TagCond> &condition,
        const typename base_tensor<T, Rank, TagFalse>::value_type &x,
        const base_tensor<T, Rank, TagFalse> &y
    );

    /// Rearranging elements.

    /**
     * @brief Reverse the order of the elements in a tensor along the given
     * axes.
     *
     * @param a A tensor-like object to reverse.
     * @param axes Axis or axes along which to reverse over. The default is
     *     Rank - 1, which reverses along the last axis.
     *
     * @return A light-weight object with the elements in the tensor in
     *     reversed order. This function does not create a new tensor, instead,
     *     it returns a readonly view of the tensor with its elements in
     *     reversed order over the given axes.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, flip_tag<Tag, 1> > reverse(
        const base_tensor<T, Rank, Tag> &a, size_t axis = Rank - 1
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, flip_tag<Tag, N> > reverse(
        const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes
    );

    /**
     * @brief Rotate the elements in a tensor along the given axes. The
     * elements are shifted circularly in such a way that the element at
     * position shift becomes the first element and the element at position
     * shift - 1 becomes the last element.
     *
     * @param a A tensor-like object to rotate.
     * @param shift Number of positions to shift the elements by along each
     *     axis.
     * @param axes Axis or axes along which the elements are rotated. The
     *     default is Rank - 1, which rotates along the last axis.
     *
     * @return A light-weight object with the elements in the tensor shifted
     *     circularly. This function does not create a new tensor, instead, it
     *     returns a readonly view of the tensor with its elements shifted
     *     circularly over the given axes.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, roll_tag<Tag, 1> > rotate(
        const base_tensor<T, Rank, Tag> &a,
        size_t shift, size_t axis = Rank - 1
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, roll_tag<Tag, N> > rotate(
        const base_tensor<T, Rank, Tag> &a,
        const index_t<N> &shift, const shape_t<N> &axes
    );

    /// Basic statistics.

    /**
     * @brief Return the average of the tensor elements.
     *
     * @param a A tensor-like object.
     *
     * @return The average of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    mean(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the average of the tensor elements over the given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the average is
     *     computed.
     *
     * @return A new tensor with the average over the axes. The output tensor
     *     will have the same dimension and the same shape, except that the
     *     axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    mean(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    mean(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return the median of the tensor elements.
     *
     * @details The median is defined as the middle value of a sorted copy of
     * the tensor. If the size is even, the average of the two middle values is
     * returned.
     *
     * @param a A tensor-like object.
     *
     * @return The median of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    median(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return the median of the tensor elements over the given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the median is
     *     computed.
     *
     * @return A new tensor with the median over the axes. The output tensor
     *     will have the same dimension and the same shape, except that the
     *     axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    median(const base_tensor<T, Rank, Tag> &a, size_t axis);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    median(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes);

    /**
     * @brief Return the variance of the tensor elements.
     *
     * @details The variance is defined as the average of the squared
     * deviations from the mean
     *     var(a) = mean(x),    x = abs(a - mean(a))**2
     * The mean is calculated as sum(x)/n, where n = x.size(). However, if bias
     * is false, the divisor n - 1 is used instead of n. In statistics, n - 1
     * provides an unbiased estimator of the sample variance; while n provides
     * the maximum likelihood estimator of the variance for normally
     * distributed variables.
     *
     * @param a A tensor-like object.
     * @param bias If bias is true, then normalization is by n. Otherwise,
     *     normalization is by n - 1.
     *
     * @return The variance of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    var(const base_tensor<T, Rank, Tag> &a, bool bias);

    /**
     * @brief Return the variance of the tensor elements over the given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the variance is
     *     computed.
     * @param bias If bias is true, then normalization is by n. Otherwise,
     *     normalization is by n - 1.
     *
     * @return A new tensor with the variance over the axes. The output tensor
     *     will have the same dimension and the same shape, except that the
     *     axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    var(const base_tensor<T, Rank, Tag> &a, size_t axis, bool bias);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    var(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes, bool bias);

    /**
     * @brief Return the standard deviation of the tensor elements.
     *
     * @details The standard deviation is defined as the square root of the
     * average of the squared deviations from the mean
     *     stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
     * The mean is calculated as sum(x)/n, where n = x.size(). However, if bias
     * is false, the divisor n - 1 is used instead of n. In statistics, n - 1
     * provides an unbiased estimator of the sample variance; while n provides
     * the maximum likelihood estimator of the variance for normally
     * distributed variables.
     *
     * @param a A tensor-like object.
     * @param bias If bias is true, then normalization is by n. Otherwise,
     *     normalization is by n - 1.
     *
     * @return The standard deviation of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    stddev(const base_tensor<T, Rank, Tag> &a, bool bias);

    /**
     * @brief Return the standard deviation of the tensor elements over the
     * given axes.
     *
     * @param a A tensor-like object.
     * @param axes A shape_t object with the axes along which the standard
     *     deviation is computed.
     * @param bias If bias is true, then normalization is by n. Otherwise,
     *     normalization is by n - 1.
     *
     * @return A new tensor with the standard deviation over the axes. The
     *     output tensor will have the same dimension and the same shape,
     *     except that the axes which are reduced are left as dimensions of
     *     size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    stddev(const base_tensor<T, Rank, Tag> &a, size_t axis, bool bias);

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    stddev(
        const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes, bool bias
    );

    /**
     * @brief Return the q-th quantile of the tensor elements.
     *
     * @details Given a tensor of size n, the q-th quantile is the (n - 1)*q-th
     * value of a sorted copy of the tensor. If (n - 1)*q is not an integer,
     * the quantile is a weighted average of the two nearest neighbors. The
     * different methods work as follows:
     *     "lower" Takes the lowest neighbor.
     *     "higher" Takes the highest neighbor.
     *     "midpoint" Takes the average of both neighbors.
     *     "nearest" Takes the nearest neighbor.
     *     "linear" Takes a linear interpolation between both neighbors.
     *
     * @param a A tensor-like object.
     * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
     * @param method This parameter specifies the method to use for estimating
     *     the quantile. Must be one of "lower", "higher", "nearest",
     *     "midpoint" or "linear".
     *
     * @return The q-th quantile of the tensor elements.
     */
    template <class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    quantile(
        const base_tensor<T, Rank, Tag> &a, double q,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the q-th quantile of the tensor elements over the given
     * axes.
     *
     * @param a A tensor-like object.
     * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
     * @param axes A shape_t object with the axes along which the quantile is
     *     computed.
     * @param method This parameter specifies the method to use for estimating
     *     the quantile. Must be one of "lower", "higher", nearest",
     *     "midpoint" or "linear".
     *
     * @return A new tensor with the quantile over the axes. The output tensor
     *     will have the same dimension and the same shape, except that the
     *     axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    quantile(
        const base_tensor<T, Rank, Tag> &a, double q, size_t axis,
        const std::string &method = "linear"
    );

    template <class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    quantile(
        const base_tensor<T, Rank, Tag> &a, double q, const shape_t<N> &axes,
        const std::string &method = "linear"
    );

    /**
     * @brief Return the covariance of two one-dimensional tensors.
     *
     * @details The covariance is defined as the average of the element-wise
     * products of the deviations from the mean:
     *    cov(x, y) = mean((x - mean(x))*(y - mean(y)))
     * For complex types, the complex conjugate of y is used:
     *    cov(x, y) = mean((x - mean(x))*conj(y - mean(y)))
     * The mean is calculated as sum(*)/n, where n = x.size(). However, if bias
     * is false, the divisor n - 1 is used instead of n.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     * @param bias If bias is true, then normalization is by n. Otherwise,
     *     normalization is by n - 1.
     *
     * @return The covariance of the two tensors.
     *
     * @throw std::invalid_argument Thrown if the tensor arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T cov(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b,
        bool bias = false
    );

    template <class T, class Tag1, class Tag2>
    std::complex<T> cov(
        const base_tensor<std::complex<T>, 1, Tag1> &a,
        const base_tensor<std::complex<T>, 1, Tag2> &b,
        bool bias = false
    );

    /**
     * @brief Return the covariance matrix of given data.
     *
     * @param a A 2-dimensional tensor-like object containing multiple
     *     variables and observations.
     * @param rowvar If true, then each row represents a variable, with
     *     observations in the columns. Otherwise, each column represents a
     *     variable, with observations in the rows.
     * @param bias If bias is true, then normalization is by n. Otherwise,
     *     normalization is by n - 1.
     *
     * @return The covariance matrix of the variables. The element (i, j) is
     *     equal to the covariance of the i-th and j-th variables.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    tensor<T, 2> cov(
        const base_tensor<T, 2, Tag> &a, bool rowvar = true, bool bias = false
    );

    template <class T, class Tag>
    tensor<std::complex<T>, 2> cov(
        const base_tensor<std::complex<T>, 2, Tag> &a, bool rowvar = true,
        bool bias = false
    );

    /**
     * @brief Return the Pearson's correlation coefficient of two
     * one-dimensional tensors.
     *
     * @details The correlation coefficient is defined as the covariance of the
     * two variables divided by the product of their standard deviations:
     *     corr(x, y) = cov(x, y)/(stddev(x)*stddev(y))
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The correlation coefficient of the two tensors.
     *
     * @throw std::invalid_argument Thrown if the tensor arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T corrcoef(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    /**
     * @brief Return the correlation matrix of given data.
     *
     * @param a A 2-dimensional tensor-like object containing multiple
     *     variables and observations.
     * @param rowvar If true, then each row represents a variable, with
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
    tensor<T, 2> corrcoef(const base_tensor<T, 2, Tag> &a, bool rowvar = true);

    /// Basic linear algebra.

    /**
     * @brief Return the dot product of two vectors.
     *
     * @details The dot product of two vectors @f$a = [a_1, a_2, \ldots, a_n]@f$
     * and @f$b = [b_1, b_2, \ldots, b_n]@f$ is defined as
     * @f[
     *     a \cdot b = a_1b_1 + a_2b_2 + \ldots + a_nb_n
     * @f]
     *
     * @note Unlike Numpy's dot, dot only supports computing the dot product of
     * two one-dimensional tensors with the same number of elements.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The dot product of the inputs.
     *
     * @throw std::invalid_argument Thrown if the tensor arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T dot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b);

    /**
     * @brief Return the dot product of two vectors.
     *
     * @details The vdot function handles complex numbers differently than dot.
     * For complex numbers, the complex conjugate of the first argument is used
     * for the calcualtion of the dot product.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The dot product of the inputs.
     *
     * @throw std::invalid_argument Thrown if the tensor arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T vdot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b);

    template <class T, class Tag1, class Tag2>
    std::complex<T> vdot(
        const base_tensor<std::complex<T>, 1, Tag1> &a,
        const base_tensor<std::complex<T>, 1, Tag2> &b
    );

    /**
     * @brief Return the cross product of two vectors.
     *
     * @details The cross product of two vectors a and b (in R3) is a vector
     * perpendicular to both a and b.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     * @param axis Axis along which to take cross product. Default is Rank - 1.
     *     Input tensors need to be broadcastable to some shape along the other
     *     axes.
     *
     * @return The cross product of the inputs.
     *
     * @throw std::invalid_argument Thrown if the size of the vectors is not 3
     *     along the given axis.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> cross(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        size_t axis = Rank - 1
    );

    /**
     * @brief Return the matrix multiplication of two matrices.
     *
     * @details The behaviour depends on the arguments in the following way.
     * - If both arguments are one-dimensional, it is the dot product of
     *   vectors (without complex conjugation).
     * - If both arguments are two-dimensional, it is the conventional matrix
     *   multiplication.
     * - If the first argument is one-dimensional and the second argument is
     *   two-dimensional, it is the matrix multiplication with the first
     *   argument treated as a row-vector.
     * - If the first argument is two-dimensional and the second argument is
     *   one-dimensional, it is the matrix multiplication with the second
     *   argument treated as a column-vector.
     * - If both arguments are n-dimensional, n > 2, it is treated as a stack
     *   of matrices residing in the last two dimensions and broadcast
     *   accordingly.
     *
     * The matrix multiplication of a @f$m \times p@f$ matrix
     * @f$A = (a_{ij})@f$ and a @f$p \times n@f$ matrix @f$B = (b_{ij})@f$ is
     * the @f$m \times n@f$ matrix @f$C = (c_{ij})@f$ such that
     * @f[
     *     c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \ldots + a_{ip}b_{pj}
     * @f]
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The matrix multiplication of the inputs.
     *
     * @throw std::invalid_argument Thrown if the last dimension of a is not
     *     the same as the second-to-last dimension of b.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    T matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 2> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, 2, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, 2, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );

    /**
     * @brief Return a contraction of two tensors over multiple axes.
     *
     * @details Given two tensors a and b, and two tuples of axes a_axes and
     * b_axes, sum the products of a's and b's elements over the axes specified
     * by a_axes and b_axes.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     * @param a_axes List of axes to sum over a.
     * @param b_axes List of axes to sum over b.
     *
     * @return The contraction of the inputs. The shape of the result consists
     *     of the non-contracted axes of the first tensor, followed by the
     *     non-contracted axes of the second.
     *
     * @throw std::invalid_argument Thrown if the shapes of and b are not equal
     *     over the contracted axes.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    T tensordot(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const shape_t<Rank> &a_axes,
        const shape_t<Rank> &b_axes
    );

    template <class T, size_t Rank1, class Tag1,
              size_t Rank2, class Tag2,
              size_t N>
    tensor<T, (Rank1 - N) + (Rank2 - N)> tensordot(
        const base_tensor<T, Rank1, Tag1> &a,
        const base_tensor<T, Rank2, Tag2> &b,
        const shape_t<N> &a_axes,
        const shape_t<N> &b_axes
    );

    /**
     * @brief Reverse or permute the axes of a tensor.
     *
     * @param a Input tensor.
     * @param axes If specified, it must be a permutation of (0, 1, ...,
     *     Rank - 1). The i-th axis of the returned tensor will correspond to
     *     the axes[i]-th axis of the input. If not specified, reverses the
     *     order of the axes.
     *
     * @return A light-weight object with the axes of a permuted. This function
     *     does not create a new tensor, instead, it returns a readonly view
     *     of a with its axes permuted.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    );

    /**
     * @brief Return the (complex) conjugate transpose of a tensor. For
     * non-complex types, conj_transpose is equivalent to transpose.
     *
     * @param a Input tensor.
     * @param axes If specified, it must be a permutation of (0, 1, ...,
     *     Rank - 1). The i-th axis of the returned tensor will correspond to
     *     the axes[i]-th axis of the input. If not specified, reverses the
     *     order of the axes.
     *
     * @return A light-weight object with the conjugate transpose of a. This
     *     function does not create a new tensor, instead, it returns a
     *     readonly view of a with its elements complex conjugated and its axes
     *     permuted.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const shape_t<Rank> &axes
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    );

    /**
     * @brief Return the vector norm.
     *
     * @details This function is able to return one of the following norms:
     *        p sum(abs(a)**p)**(1./p)
     *        0 sum(x != 0)
     *      inf amax(abs(x))
     *     -inf amin(abs(x))
     *
     * @param a A one-dimensional tensor-like object.
     * @param p Order of the norm. The default is 2 (Euclidean norm). For
     *     values of p < 1, the result is, strictly speaking, not a
     *     mathematical norm, but it may still be useful for various numerical
     *     purposes.
     *
     * @return Norm of the vector.
     */
    template <class T, size_t Rank, class Tag>
    T norm(const base_tensor<T, Rank, Tag> &a, double p = 2);

    template <class T, size_t Rank, class Tag>
    T norm(const base_tensor<std::complex<T>, Rank, Tag> &a, double p = 2);
}

#include "numcpp/routines/routines.tcc"

#endif // NUMCPP_ROUTINES_H_INCLUDED
