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

#include "numcpp/routines/ranges.h"
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
    tensor<T, 2> tril(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

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
    tensor<T, 2> triu(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

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
        const typename tensor<T, Rank>::value_type &val
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        T, Rank, lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag>
    > maximum(
        const typename tensor<T, Rank>::value_type &val,
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
        const typename tensor<T, Rank>::value_type &val
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        T, Rank, lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag>
    > minimum(
        const typename tensor<T, Rank>::value_type &val,
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
        const typename tensor<T, Rank>::value_type &a_min,
        const typename tensor<T, Rank>::value_type &a_max
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
     * @return The cumulative sum of the tensor elements.
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
     * @return The cumulative product of the tensor elements.
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
     * @param a A tensor-like object of bool.
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
     *     abs(a - b) <= fmax(rtol * max(abs(a), abs(b)), atol)
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
    bool isclose(float a, float b, float rtol = 1e-8f, float atol = 0.f);

    bool isclose(double a, double b, double rtol = 1e-8, double atol = 0.);

    bool isclose(
        long double a, long double b,
        long double rtol = 1e-8L, long double atol = 0.L
    );

    /**
     * @brief Additional overloads for arithmetic types.
     *
     * @details If any argument has integral type, it is cast to double. If any
     * argument is long double, the other argument is promoted to long double.
     * If any argument is float, it is promoted to double.
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
        const typename tensor<T, Rank>::value_type &rtol = T(1e-8),
        const typename tensor<T, Rank>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, Tag, T, scalar_tag>
    > isclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename tensor<T, Rank>::value_type &val,
        const typename tensor<T, Rank>::value_type &rtol = T(1e-8),
        const typename tensor<T, Rank>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<ranges::isclose<T>, T, scalar_tag, T, Tag>
    > isclose(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename tensor<T, Rank>::value_type &rtol = T(1e-8),
        const typename tensor<T, Rank>::value_type &atol = T(0)
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
        const typename tensor<std::complex<T>, Rank>::value_type &val,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<
        bool, Rank, lazy_binary_tag<
            ranges::isclose<std::complex<T> >, T, scalar_tag, T, Tag
        >
    > isclose(
        const typename tensor<std::complex<T>, Rank>::value_type &val,
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
        const typename tensor<T, Rank>::value_type &rtol = T(1e-8),
        const typename tensor<T, Rank>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const base_tensor<T, Rank, Tag> &a,
        const typename tensor<T, Rank>::value_type &val,
        const typename tensor<T, Rank>::value_type &rtol = T(1e-8),
        const typename tensor<T, Rank>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename tensor<T, Rank>::value_type &rtol = T(1e-8),
        const typename tensor<T, Rank>::value_type &atol = T(0)
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
        const typename tensor<std::complex<T>, Rank>::value_type &val,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
    );

    template <class T, size_t Rank, class Tag>
    bool allclose(
        const typename tensor<std::complex<T>, Rank>::value_type &val,
        const base_tensor<std::complex<T>, Rank, Tag> &b,
        const typename std::complex<T>::value_type &rtol = T(1e-8),
        const typename std::complex<T>::value_type &atol = T(0)
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
