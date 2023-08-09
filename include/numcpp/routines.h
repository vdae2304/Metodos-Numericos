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
#include "numcpp/routines/new.h"
#include "numcpp/routines/lazy_where.h"
#include "numcpp/routines/rearrange.h"

namespace numcpp {
/// Tensor creation routines.

/**
 * @brief Return a new tensor of given shape without initializing entries.
 *
 * @tparam T Type of the elements contained in the tensor. Default is double.
 *
 * @param shape Shape of the tensor. It can be a shape_t object or the elements
 *              of the shape passed as separate arguments.
 *
 * @return A tensor of uninitialized data with the given shape.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T = double, class... Sizes,
          detail::RequiresIntegral<Sizes...> = 0>
tensor<T, sizeof...(Sizes)> empty(Sizes... sizes) {
  return tensor<T, sizeof...(Sizes)>(sizes...);
}

template <class T = double, size_t Rank>
tensor<T, Rank> empty(const shape_t<Rank> &shape) {
  return tensor<T, Rank>(shape);
}

/**
 * @brief Return a new tensor with the same shape and type as a given tensor.
 *
 * @param like Tensor-like object defining the shape and data type.
 *
 * @return A tensor of uninitialized data with the same shape and type as
 *         @a like.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, Rank> empty_like(const expression<Container, T, Rank> &like) {
  return tensor<T, Rank>(like.shape());
}

/**
 * @brief Return a tensor of given shape filled with zeros.
 *
 * @tparam T Type of the elements contained in the tensor. Default is double.
 *
 * @param shape Shape of the tensor. It can be a shape_t object or the elements
 *              of the shape passed as separate arguments.
 *
 * @return A tensor of zeros with the given shape. This function does not create
 *         a new tensor, instead, an expression object is returned.
 */
template <class T = double, class... Sizes,
          detail::RequiresIntegral<Sizes...> = 0>
const_expr<T, sizeof...(Sizes)> zeros(Sizes... sizes) {
  return const_expr<T, sizeof...(Sizes)>({sizes...}, T());
}

template <class T = double, size_t Rank>
const_expr<T, Rank> zeros(const shape_t<Rank> &shape) {
  return const_expr<T, Rank>(shape, T());
}

/**
 * @brief Return a tensor of zeros with the same shape and type as a given
 * tensor.
 *
 * @param like Tensor-like object defining the shape and data type.
 *
 * @return A tensor of zeros with the same shape and type as @a like. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned.
 */
template <class Container, class T, size_t Rank>
const_expr<T, Rank> zeros_like(const expression<Container, T, Rank> &like) {
  return const_expr<T, Rank>(like.shape(), T());
}

/**
 * @brief Return a tensor of given shape filled with ones.
 *
 * @tparam T Type of the elements contained in the tensor. Default is double.
 *
 * @param shape Shape of the tensor. It can be a shape_t object or the elements
 *              of the shape passed as separate arguments.
 *
 * @return A tensor of ones with the given shape. This function does not create
 *         a new tensor, instead, an expression object is returned.
 */
template <class T = double, class... Sizes,
          detail::RequiresIntegral<Sizes...> = 0>
const_expr<T, sizeof...(Sizes)> ones(Sizes... sizes) {
  return const_expr<T, sizeof...(Sizes)>({sizes...}, T(1));
}

template <class T = double, size_t Rank>
const_expr<T, Rank> ones(const shape_t<Rank> &shape) {
  return const_expr<T, Rank>(shape, T(1));
}

/**
 * @brief Return a tensor of ones with the same shape and type as a given
 * tensor.
 *
 * @param like Tensor-like object defining the shape and data type.
 *
 * @return A tensor of ones with the same shape and type as @a like. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned.
 */
template <class Container, class T, size_t Rank>
const_expr<T, Rank> ones_like(const expression<Container, T, Rank> &like) {
  return const_expr<T, Rank>(like.shape(), T(1));
}

/**
 * @brief Return a tensor of given shape filled with @a val.
 *
 * @param shape Shape of the tensor.
 * @param val Fill value.
 *
 * @return A tensor of @a val with the given shape. This function does not
 *         create a new tensor, instead, an expression object is returned.
 */
template <class T, size_t Rank>
const_expr<T, Rank> full(const shape_t<Rank> &shape, const T &val) {
  return const_expr<T, Rank>(shape, val);
}

/**
 * @brief Return a tensor filled with @a val with the same shape and type as a
 * given tensor.
 *
 * @param like Tensor-like object defining the shape and data type.
 * @param val Fill value.
 *
 * @return A tensor of @a val with the same shape and type as @a like. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned.
 */
template <class Container, class T, size_t Rank>
const_expr<T, Rank> full_like(const expression<Container, T, Rank> &like,
                              const typename Container::value_type &val) {
  return const_expr<T, Rank>(like.shape(), val);
}

/// Numerical ranges.

/**
 * @brief Return evenly spaced values within a given interval.
 *
 * @details Values are generated within the half-open interval @a [start,stop),
 * i.e., the interval including @a start but excluding @a stop, with spacing
 * between values given by @a step.
 *
 * @param start Start of interval. Defaults to 0 if not provided.
 * @param stop End of interval.
 * @param step Spacing between values. This is the distance between two adjacent
 *             values. Defaults to 1 if not provided.
 *
 * @return A light-weight object with evenly spaced values. This function does
 *         not create a new tensor, instead, an expression object is returned
 *         with evenly spaced values within a given interval.
 */
template <class T> sequence_expr<T> arange(const T &stop);

template <class T, class U>
sequence_expr<typename std::common_type<T, U>::type> arange(const T &start,
                                                            const U &stop);

template <class T, class U, class V>
sequence_expr<typename std::common_type<T, U, V>::type>
arange(const T &start, const U &stop, const V &step);

/**
 * @brief Return evenly spaced numbers over a specified interval.
 *
 * @details Return @a num evenly spaced samples, calculated over the interval
 * @a [start,stop]. The endpoint of the interval can optionally be excluded.
 *
 * @param start The starting value of the sequence.
 * @param stop The end value of the sequence.
 * @param num Number of samples to generate. Default is 50.
 * @param endpoint If true, @a stop is the last sample. Otherwise, it is not
 *                 included. Default is true.
 *
 * @return A light-weight object with equally spaced samples. This function does
 *         not create a new tensor, instead, an expression object is returned
 *         with equally spaced samples over an interval.
 */
template <class T, class U>
sequence_expr<typename std::common_type<T, U>::type>
linspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);

/**
 * @brief Return numbers spaced evenly on a log scale.
 *
 * @details In linear space, the sequence starts at pow(base, start) ( @a base
 * to the power of @a start ) and ends with pow(base, stop).
 *
 * @param start pow(base, start) is the starting value of the sequence.
 * @param stop pow(base, stop) is the final value of the sequence.
 * @param num Number of samples to generate. Default is 50.
 * @param endpoint If true, @a stop is the last sample. Otherwise, it is not
 *                 included. Default is true.
 * @param base The base of the log space. Default is 10.
 *
 * @return A light-weight object with equally spaced samples on a log scale.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned with equally spaced samples on a log scale.
 */
template <class T, class U>
sequence_expr<typename std::common_type<T, U>::type>
logspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);

template <class T, class U, class V>
sequence_expr<typename std::common_type<T, U, V>::type>
logspace(const T &start, const U &stop, size_t num, bool endpoint,
         const V &base);

/**
 * @brief Return numbers spaced evenly on a log scale (a geometric progression).
 *
 * @details This is similar to @c logspace, but with endpoints specified
 * directly. Each output sample is a constant multiple of the previous.
 *
 * If the inputs are complex, the output will follow a logarithmic spiral in the
 * complex plane.
 *
 * @param start The starting value of the sequence.
 * @param stop The final value of the sequence.
 * @param num Number of samples to generate. Default is 50.
 * @param endpoint If true, @a stop is the last sample. Otherwise, it is not
 *                 included. Default is true.
 *
 * @return A light-weight object with equally spaced samples on a log scale.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned with equally spaced samples on a log scale.
 */
template <class T, class U>
sequence_expr<typename std::common_type<T, U>::type>
geomspace(const T &start, const U &stop, size_t num = 50, bool endpoint = true);

/// Building matrices.

/**
 * @brief Return a matrix with ones on the diagonal and zeros elsewhere.
 *
 * @tparam T Type of the elements contained in the tensor. Default is double.
 *
 * @param m Number of rows. If not provided, defaults to @a n.
 * @param n Number of columns.
 * @param k Offset of the diagonal from the main diagonal. A positive value
 *          refers to an upper diagonal and a negative value refers to a lower
 *          diagonal. Defaults to main diagonal (0).
 *
 * @return A light-weight object with ones on the diagonal and zeros elsewhere.
 *         This function, does not create a new tensor, instead, an expression
 *         object is returned with ones on the diagonal and zeros elsewhere.
 */
template <class T = double> identity_expr<T> eye(size_t n) {
  return eye<T>(n, n);
}

template <class T = double>
identity_expr<T> eye(size_t m, size_t n, ptrdiff_t k = 0) {
  return identity_expr<T>({m, n}, k);
}

/**
 * @brief Extract a diagonal or construct a diagonal matrix.
 *
 * @details If @a a is a 1-dimensional tensor, returns a matrix with @a a on the
 * @a k -th diagonal and zeros elsewhere.
 * If @a a is a 2-dimensional tensor, returns the diagonal of @a a with
 * the given offset, i.e., an array with the elements of the form @a a(i,i+k).
 * If @a a has more than two dimensions, then the last two axes are used to
 * determine the 2-dimensional sub-tensor whose diagonal is returned.
 *
 *
 * @param a An n-dimensional tensor from which the diagonals are taken or an
 *          1-dimensional tensor with the elements that will be on the diagonal.
 * @param k Offset of the diagonal from the main diagonal. A positive value
 *          refers to an upper diagonal and a negative value refers to a lower
 *          diagonal. Defaults to main diagonal (0).
 *
 * @return The extracted diagonal or the constructed diagonal matrix. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned.
 */
template <class Container, class T, size_t Rank>
diagonal_expr<Container, T, Rank> diag(const expression<Container, T, Rank> &a,
                                       ptrdiff_t k = 0) {
  return diagonal_expr<Container, T, Rank>(a, k);
}

/**
 * @brief Return the lower triangle of a matrix. If the tensor has more than two
 * dimensions, then the last two axes are used to determine the 2-dimensional
 * sub-tensor whose lower triangle is returned.
 *
 * @param a Matrix from which the lower triangle is taken.
 * @param k Diagonal above which to zero elements. A positive value refers to an
 *          upper diagonal and a negative value refers to a lower diagonal.
 *          Defaults to main diagonal (0).
 *
 * @return Lower triangle of @a a. This function does not create a new tensor,
 *         instead, an expression object is returned with the elements above the
 *         @a k -th diagonal zeroed.
 */
template <class Container, class T, size_t Rank>
triangular_expr<Container, T, Rank>
tril(const expression<Container, T, Rank> &a, ptrdiff_t k = 0) {
  return triangular_expr<Container, T, Rank>(a, true, k);
}

/**
 * @brief Return the upper triangle of a matrix. If the tensor has more than two
 * dimensions, then the last two axes are used to determine the 2-dimensional
 * sub-tensor whose upper triangle is returned.
 *
 * @param a Matrix from which the upper triangle is taken.
 * @param k Diagonal below which to zero elements. A positive value refers to an
 *          upper diagonal and a negative value refers to a lower diagonal.
 *          Defaults to main diagonal (0).
 *
 * @return Upper triangle of @a a. This function does not create a new tensor,
 *         instead, an expression object is returned with the elements below the
 *         @a k -th diagonal zeroed.
 */
template <class Container, class T, size_t Rank>
triangular_expr<Container, T, Rank>
triu(const expression<Container, T, Rank> &a, ptrdiff_t k = 0) {
  return triangular_expr<Container, T, Rank>(a, false, k);
}

/**
 * @brief Generate a Vandermonde matrix.
 *
 * @details The columns of the output matrix are powers of the input vector. The
 * order of the powers is determined by the @a increasing boolean argument.
 *
 * @param x 1-dimensional input tensor.
 * @param N Number of columns in the output. If @a N is not specified, a square
 *          matrix is returned ( @a N = x.size() ).
 * @param increasing Order of the powers of the columns. If true, the powers
 *                   increase from left to right. If false, they are reversed.
 *                   Default is false.
 *
 * @return Vandermonde matrix.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T>
tensor<T, 2> vander(const expression<Container, T, 1> &x);

template <class Container, class T>
tensor<T, 2> vander(const expression<Container, T, 1> &x, size_t N,
                    bool increasing = false);

/// Maximums and minimums.

/**
 * @brief Return the index of the maximum value in the tensor.
 *
 * @param a A tensor-like object.
 *
 * @return The index of the maximum value in the tensor. In case of multiple
 *         occurrences of the maximum value, return the index corresponding to
 *         the first occurrence.
 */
template <class Container, class T, size_t Rank>
index_t<Rank> argmax(const expression<Container, T, Rank> &a);

/**
 * @brief Return the indices of the maximum value in the tensor along the given
 * axis.
 *
 * @param a A tensor-like object.
 * @param axis Axis along which the maximum value is selected.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the indices of the maximum value along the given
 *         axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmax(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis);

template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argmax(const expression<Container, T, Rank> &a,
                            const shape_t<1> &axis, keepdims_t);

template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmax(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis, dropdims_t);

/**
 * @brief Return the index of the minimum value in the tensor.
 *
 * @param a A tensor-like object.
 *
 * @return The index of the minimum value in the tensor. In case of multiple
 *         occurrences of the minimum value, return the index corresponding to
 *         the first occurrence.
 */
template <class Container, class T, size_t Rank>
index_t<Rank> argmin(const expression<Container, T, Rank> &a);

/**
 * @brief Return the indices of the minimum value in the tensor along the given
 * axis.
 *
 * @param a A tensor-like object.
 * @param axis Axis along which the minimum value is selected.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the indices of the minimum value along the given
 *         axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmin(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis);

template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argmin(const expression<Container, T, Rank> &a,
                            const shape_t<1> &axis, keepdims_t);

template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmin(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis, dropdims_t);

/**
 * @brief Return the maximum value contained in the tensor.
 *
 * @param a A tensor-like object.
 *
 * @return The maximum value in the tensor.
 */
template <class Container, class T, size_t Rank>
T amax(const expression<Container, T, Rank> &a);

/**
 * @brief Return the maximum value contained in the tensor over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the maximum value
 *             is selected.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the maximum value over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amax(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> amax(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amax(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return the minimum value contained in the tensor.
 *
 * @param a A tensor-like object.
 *
 * @return The minimum value in the tensor.
 */
template <class Container, class T, size_t Rank>
T amin(const expression<Container, T, Rank> &a);

/**
 * @brief Return the minimum value contained in the tensor over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the minimum value
 *             is selected.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the minimum value over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amin(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> amin(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amin(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return the element-wise maximum of two tensors.
 *
 * @param a A tensor-like object with the values to pass as first argument.
 * @param b A tensor-like object with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument. Values
 *            are broadcasted to an appropriate shape.
 *
 * @return A light-weight object with the element-wise maximum of two tensors.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned with the element-wise maximum of the arguments.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <class Container1, class T, class Container2, size_t Rank>
binary_expr<ranges::maximum, Container1, T, Container2, T, Rank>
maximum(const expression<Container1, T, Rank> &a,
        const expression<Container2, T, Rank> &b) {
  return binary_expr<ranges::maximum, Container1, T, Container2, T, Rank>(a, b);
}

template <class Container, class T, size_t Rank>
binary_expr<ranges::maximum, Container, T, void, T, Rank>
maximum(const expression<Container, T, Rank> &a,
        const typename Container::value_type &val) {
  binary_expr<ranges::maximum, Container, T, void, T, Rank>(a, val);
}

template <class Container, class T, size_t Rank>
binary_expr<ranges::maximum, void, T, Container, T, Rank>
maximum(const typename Container::value_type &val,
        const expression<Container, T, Rank> &b) {
  return binary_expr<ranges::maximum, void, T, Container, T, Rank>(val, b);
}

/**
 * @brief Return the element-wise minimum of two tensors.
 *
 * @param a A tensor-like object with the values to pass as first argument.
 * @param b A tensor-like object with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument. Values
 *            are broadcasted to an appropriate shape.
 *
 * @return A light-weight object with the element-wise minimum of two tensors.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned with the element-wise minimum of the arguments.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <class Container1, class T, class Container2, size_t Rank>
binary_expr<ranges::minimum, Container1, T, Container2, T, Rank>
minimum(const expression<Container1, T, Rank> &a,
        const expression<Container2, T, Rank> &b) {
  return binary_expr<ranges::minimum, Container1, T, Container2, T, Rank>(a, b);
}

template <class Container, class T, size_t Rank>
binary_expr<ranges::minimum, Container, T, void, T, Rank>
minimum(const expression<Container, T, Rank> &a,
        const typename Container::value_type &val) {
  binary_expr<ranges::minimum, Container, T, void, T, Rank>(a, val);
}

template <class Container, class T, size_t Rank>
binary_expr<ranges::minimum, void, T, Container, T, Rank>
minimum(const typename Container::value_type &val,
        const expression<Container, T, Rank> &b) {
  return binary_expr<ranges::minimum, void, T, Container, T, Rank>(val, b);
}

/**
 * @brief Clamp the values in the tensor. Given an interval @a [a_min,a_max],
 * values smaller than @a a_min become @a a_min, and values larger than @a a_max
 * become @a a_max. For complex types, the real and imaginary parts are clamped
 * separately.
 *
 * @param a A tensor-like object with the elements to clamp.
 * @param a_min The lower boundary to clamp.
 * @param a_max The upper boundary to clamp.
 *
 * @return A light-weight object with the elements in the tensor clamped within
 *         the given interval. This function does not create a new tensor,
 *         instead, an expression object is returned with the clamped values in
 *         the tensor.
 *
 * @note The behavior is undefined if @a a_min is greater than @a a_max.
 */
template <class Container, class T, size_t Rank>
unary_expr<ranges::clamp<T>, Container, T, Rank>
clamp(const expression<Container, T, Rank> &a,
      const typename Container::value_type &a_min,
      const typename Container::value_type &a_max) {
  return unary_expr<ranges::clamp<T>, Container, T, Rank>(
      ranges::clamp<T>(a_min, a_max), a);
}

/// Sums and products.

/**
 * @brief Return the sum of the tensor elements.
 *
 * @param a A tensor-like object.
 *
 * @return The sum of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T sum(const expression<Container, T, Rank> &a);

/**
 * @brief Return the sum of the tensor elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the sum is
 *             performed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the sum over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> sum(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> sum(const expression<Container, T, Rank> &a,
                    const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> sum(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return the product of the tensor elements.
 *
 * @param a A tensor-like object.
 *
 * @return The product of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T prod(const expression<Container, T, Rank> &a);

/**
 * @brief Return the product of the tensor elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the product is
 *             performed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the product over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> prod(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> prod(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> prod(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return the cumulative sum of the tensor elements along a given axis.
 *
 * @param a A tensor-like object.
 * @param axis Axis along which the cumulative sum is computed. Default is zero.
 *
 * @return The cumulative sum of the tensor elements. The output tensor has the
 *         same dimension and the same shape as @a a.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, Rank> cumsum(const expression<Container, T, Rank> &a,
                       size_t axis = 0);

/**
 * @brief Return the cumulative product of the tensor elements along a given
 * axis.
 *
 * @param a A tensor-like object.
 * @param axis Axis along which the cumulative product is computed. Default is
 *             zero.
 *
 * @return The cumulative product of the tensor elements. The output tensor has
 *         the same dimension and the same shape as @a a.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, Rank> cumprod(const expression<Container, T, Rank> &a,
                        size_t axis = 0);

/// Logic functions.

/**
 * @brief Test whether all tensor elements evaluate to true.
 *
 * @param a A tensor-like object of @c bool.
 *
 * @return true if all the elements evaluate to true and false otherwise.
 */
template <class Container, size_t Rank>
bool all(const expression<Container, bool, Rank> &a);

/**
 * @brief Test whether all tensor elements over the given axes evaluate to true.
 *
 * @param a A tensor-like object of @c bool.
 * @param axes A @c shape_t object with the axes along which the logical AND
 *             reduction is performed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the results of the test over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> all(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes);

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank> all(const expression<Container, bool, Rank> &a,
                       const shape_t<N> &axes, keepdims_t);

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> all(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes, dropdims_t);

/**
 * @brief Test whether any tensor element evaluate to true.
 *
 * @param a A tensor-like object of @c bool.
 *
 * @return true if any element evaluate to true and false otherwise.
 */
template <class Container, size_t Rank>
bool any(const expression<Container, bool, Rank> &a);

/**
 * @brief Test whether any tensor element over the given axes evaluate to true.
 *
 * @param a A tensor-like object of @c bool.
 * @param axes A @c shape_t object with the axes along which the logical OR
 *             reduction is performed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the results of the test over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> any(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes);

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank> any(const expression<Container, bool, Rank> &a,
                       const shape_t<N> &axes, keepdims_t);

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> any(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes, dropdims_t);

/**
 * @brief Count the number of non-zero elements in the tensor.
 *
 * @param a A tensor-like object.
 *
 * @return The number of non-zero elements.
 */
template <class Container, class T, size_t Rank>
size_t count_nonzero(const expression<Container, T, Rank> &a);

/**
 * @brief Count the number of non-zero elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which to count non-zeros.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the number of non-zero elements over the given
 *         axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<size_t, Rank - N> count_nonzero(const expression<Container, T, Rank> &a,
                                       const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<size_t, Rank> count_nonzero(const expression<Container, T, Rank> &a,
                                   const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<size_t, Rank - N> count_nonzero(const expression<Container, T, Rank> &a,
                                       const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return whether two numbers are equal within a tolerance.
 *
 * @details Whether or not two values are considered equal is determined
 * according to given absolute and relative tolerance. The tolerances must be
 * non-negative, typically very small numbers. For floating-point values, the
 * function uses the following equation to test whether two numbers are
 * equivalent:
 *     abs(a - b) <= fmax(rtol * fmax(abs(a), abs(b)), atol)
 * NaN is not considered equal to any other value, including NaN. inf and -inf
 * are only considered equal to themselves.
 *
 * @note If one of the arguments has integral type, it is cast to double.
 * If one of the arguments is long double, the other argument is promoted to
 * long double.
 * If one of the arguments is double and the other argument is float, the float
 * argument is promoted to double.
 *
 * @param a An integer or floating-point value.
 * @param b An integer or floating-point value.
 * @param rtol The relative tolerance.
 * @param atol The absolute tolerance.
 *
 * @return true if the values are considered equal and false otherwise.
 */
template <class T, class U>
bool isclose(T a, U b, typename detail::promote<T, U>::type rtol = 1e-8,
             typename detail::promote<T, U>::type atol = 0.);

/**
 * @brief Additional overloads for complex types. For complex types, the
 * equality is tested on both real and imaginary parts.
 */
template <class T, class U>
bool isclose(const std::complex<T> &a, const std::complex<U> &b,
             typename detail::promote<T, U>::type rtol = 1e-8,
             typename detail::promote<T, U>::type atol = 0);

template <class T, class U>
bool isclose(const std::complex<T> &a, const U &b,
             typename detail::promote<T, U>::type rtol = 1e-8,
             typename detail::promote<T, U>::type atol = 0);

template <class T, class U>
bool isclose(const T &a, const std::complex<U> &b,
             typename detail::promote<T, U>::type rtol = 1e-8,
             typename detail::promote<T, U>::type atol = 0);

/**
 * @brief Return whether two tensors are equal, element-wise, within a
 * tolerance.
 *
 * @param a A tensor-like object with the values to pass as first argument.
 * @param b A tensor-like object with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument. Values
 *            are broadcasted to an appropriate shape.
 * @param rtol The relative tolerance.
 * @param atol The absolute tolerance.
 *
 * @return A light-weight object of bool with each element set to true where
 *         @a a and @a b are equal within the given tolerance, and false
 *         otherwise. This function does not create a new tensor, instead, an
 *         expression object is returned with the element-wise comparison.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <class Container1, class T, class Container2, size_t Rank>
binary_expr<ranges::isclose<T>, Container1, T, Container2, T, Rank>
isclose(const expression<Container1, T, Rank> &a,
        const expression<Container2, T, Rank> &b,
        const typename detail::complex_traits<T>::value_type &rtol = 1e-8,
        const typename detail::complex_traits<T>::value_type &atol = 0) {
  typedef ranges::isclose<T> Closure;
  Closure f(rtol, atol);
  return binary_expr<Closure, Container1, T, Container2, T, Rank>(f, a, b);
}

template <class Container, class T, size_t Rank>
binary_expr<ranges::isclose<T>, Container, T, void, T, Rank>
isclose(const expression<Container, T, Rank> &a,
        const typename Container::value_type &val,
        const typename detail::complex_traits<T>::value_type &rtol = 1e-8,
        const typename detail::complex_traits<T>::value_type &atol = 0) {
  typedef ranges::isclose<T> Closure;
  Closure f(rtol, atol);
  return binary_expr<Closure, Container, T, void, T, Rank>(f, a, val);
}

template <class Container, class T, size_t Rank>
binary_expr<ranges::isclose<T>, void, T, Container, T, Rank>
isclose(const typename Container::value_type &val,
        const expression<Container, T, Rank> &b,
        const typename detail::complex_traits<T>::value_type &rtol = 1e-8,
        const typename detail::complex_traits<T>::value_type &atol = 0) {
  typedef ranges::isclose<T> Closure;
  Closure f(rtol, atol);
  return binary_expr<Closure, void, T, Container, T, Rank>(f, val, b);
}

/**
 * @brief Test whether two tensors are element-wise equal within a tolerance.
 *
 * @param a A tensor-like object with the values to pass as first argument.
 * @param b A tensor-like object with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument. Values
 *            are broadcasted to an appropriate shape.
 * @param rtol The relative tolerance.
 * @param atol The absolute tolerance.
 *
 * @return true if all the elements are considered equal and false otherwise.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <class Container1, class T, class Container2, size_t Rank>
bool allclose(const expression<Container1, T, Rank> &a,
              const expression<Container2, T, Rank> &b,
              const typename detail::complex_traits<T>::value_type &rtol = 1e-8,
              const typename detail::complex_traits<T>::value_type &atol = 0) {
  return all(isclose(a, b, rtol, atol));
}

template <class Container, class T, size_t Rank>
bool allclose(const expression<Container, T, Rank> &a,
              const typename Container::value_type &val,
              const typename detail::complex_traits<T>::value_type &rtol = 1e-8,
              const typename detail::complex_traits<T>::value_type &atol = 0) {
  return all(isclose(a, val, rtol, atol));
}

template <class Container, class T, size_t Rank>
bool allclose(const typename Container::value_type &val,
              const expression<Container, T, Rank> &b,
              const typename detail::complex_traits<T>::value_type &rtol = 1e-8,
              const typename detail::complex_traits<T>::value_type &atol = 0) {
  return all(isclose(val, b, atol, rtol));
}

/// Sorting and searching.

/**
 * @brief Return the indices that would sort the tensor.
 *
 * @param a Tensor-like object to sort.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 * @param stable If true, preserve the relative order of the elements with
 *               equivalent values. Otherwise, equivalent elements are not
 *               guaranteed to keep their original relative order.
 *
 * @return A 1-dimensional tensor of indices that sort the tensor. If @a indices
 *         is the returned tensor of indices for @a a, then @c a[indices] yields
 *         a sorted @a a.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<index_t<Rank>, 1> argsort(const expression<Container, T, Rank> &a);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<index_t<Rank>, 1> argsort(const expression<Container, T, Rank> &a,
                                 Compare comp, bool stable = false);

/**
 * @brief Return the indices that would sort the tensor along the given axis.
 *
 * @param a Tensor-like object to sort.
 * @param axis Axis along which to sort.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 * @param stable If true, preserve the relative order of the elements with
 *               equivalent values. Otherwise, equivalent elements are not
 *               guaranteed to keep their original relative order.
 *
 * @return A tensor of indices of the same shape as @a a that sort the tensor
 *         along the given axis. If @a indices is the returned tensor of indices
 *         for @a a, then @c take_along_axis(a,indices,axis) yields a sorted
 *         @a a along the given axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argsort(const expression<Container, T, Rank> &a,
                             size_t axis);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<size_t, Rank> argsort(const expression<Container, T, Rank> &a,
                             size_t axis, Compare comp, bool stable = false);

/**
 * @brief Return a sorted copy of the flattened tensor.
 *
 * @param a Tensor-like object to sort.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 * @param stable If true, preserve the relative order of the elements with
 *               equivalent values. Otherwise, equivalent elements are not
 *               guaranteed to keep their original relative order.
 *
 * @return A sorted copy of the flattened tensor.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, 1> sort(const expression<Container, T, Rank> &a);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<T, 1> sort(const expression<Container, T, Rank> &a, Compare comp,
                  bool stable = false);

/**
 * @brief Return a sorted copy of the tensor along the given axis.
 *
 * @param a Tensor-like object to sort.
 * @param axis Axis along which to sort.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 * @param stable If true, preserve the relative order of the elements with
 *               equivalent values. Otherwise, equivalent elements are not
 *               guaranteed to keep their original relative order.
 *
 * @return A sorted copy of the tensor along the given axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, Rank> sort(const expression<Container, T, Rank> &a, size_t axis);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<T, Rank> sort(const expression<Container, T, Rank> &a, size_t axis,
                     Compare comp, bool stable = false);

/**
 * @brief Return the indices that would partition the tensor.
 *
 * @param a Tensor-like object to partition.
 * @param kth Element index to partition by. The element at the @a kth position
 *            is the element that would be in that position in the sorted
 *            tensor. The other elements are left without any specific order,
 *            except that none of the elements preceding @a kth are greater than
 *            it, and none of the elements following it are less.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 *
 * @return A 1-dimensional tensor of indices that partition the tensor. If
 *         @a indices is the returned tensor of indices for @a a, then
 *         @c a[indices] yields a partitioned @a a.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<index_t<Rank>, 1> argpartition(const expression<Container, T, Rank> &a,
                                      size_t kth);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<index_t<Rank>, 1> argpartition(const expression<Container, T, Rank> &a,
                                      size_t kth, Compare comp);

/**
 * @brief Return the indices that would partition the tensor along the given
 * axis.
 *
 * @param a Tensor-like object to partition.
 * @param kth Element index to partition by. The element at the @a kth position
 *            is the element that would be in that position in the sorted
 *            tensor. The other elements are left without any specific order,
 *            except that none of the elements preceding @a kth are greater than
 *            it, and none of the elements following it are less.
 * @param axis Axis along which to partition.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 *
 * @return A tensor of indices of the same shape as @a a that partition the
 *         tensor along the given axis. If @a indices is the returned tensor of
 *         indices for @a a, then @c take_along_axis(a,indices,axis)
 *         yields a partitioned @a a along the given axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argpartition(const expression<Container, T, Rank> &a,
                                  size_t kth, size_t axis);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<size_t, Rank> argpartition(const expression<Container, T, Rank> &a,
                                  size_t kth, size_t axis, Compare comp);

/**
 * @brief Return a partitioned copy of the flattened tensor.
 *
 * @param a Tensor-like object to partition.
 * @param kth Element index to partition by. The element at the @a kth position
 *            is the element that would be in that position in the sorted
 *            tensor. The other elements are left without any specific order,
 *            except that none of the elements preceding @a kth are greater than
 *            it, and none of the elements following it are less.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 *
 * @return A partitioned copy of the flattened tensor.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, 1> partition(const expression<Container, T, Rank> &a, size_t kth);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<T, 1> partition(const expression<Container, T, Rank> &a, size_t kth,
                       Compare comp);

/**
 * @brief Return a partitioned copy of the tensor along the given axis.
 *
 * @param a Tensor-like object to partition.
 * @param kth Element index to partition by. The element at the @a kth position
 *            is the element that would be in that position in the sorted
 *            tensor. The other elements are left without any specific order,
 *            except that none of the elements preceding @a kth are greater than
 *            it, and none of the elements following it are less.
 * @param axis Axis along which to partition.
 * @param comp Custom comparator. A binary function that accepts two elements of
 *             type T as arguments, and returns a value convertible to bool. The
 *             value returned indicates whether the element passed as first
 *             argument is considered to go before the second.
 *
 * @return A partitioned copy of the tensor along the given axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, Rank> partition(const expression<Container, T, Rank> &a, size_t kth,
                          size_t axis);

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T> = 0>
tensor<T, Rank> partition(const expression<Container, T, Rank> &a, size_t kth,
                          size_t axis, Compare comp);

/**
 * @brief Return the indices of the elements that are non-zero.
 *
 * @param a A tensor-like object.
 *
 * @return A 1-dimensional tensor with the indices of the elements that are
 *         non-zero.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<index_t<Rank>, 1> nonzero(const expression<Container, T, Rank> &a);

/**
 * @brief Return elements chosen from two tensors depending on @a condition.
 * When only @a condition is provided, this function is equivalent to
 * @c nonzero.
 *
 * @param condition A tensor-like object of @c bool. Where true, yield @a x,
 *                  otherwise yield @a y.
 * @param x A single value or a tensor-like object with the values from which to
 *          choose where @a condition is true.
 * @param y A single value or a tensor-like object with the values from which to
 *          choose where @a condition is false.
 *
 * @return A light-weight object with the elements chosen from @a x where
 *         @a condition is true and from @a y where @a condition is false. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned.
 *
 * @throw std::invalid_argument Thrown if @a condition, @a x and @a y shapes are
 *                              not compatible and cannot be broadcasted
 *                              according to broadcasting rules.
 */
template <class Container, size_t Rank>
tensor<index_t<Rank>, 1>
where(const expression<Container, bool, Rank> &condition);

template <class Container, class Container1, class Container2, class T,
          size_t Rank>
where_expr<Container, Container1, Container2, T, Rank>
where(const expression<Container, bool, Rank> &condition,
      const expression<Container1, T, Rank> &x,
      const expression<Container2, T, Rank> &y) {
  return where_expr<Container, Container1, Container2, T, Rank>(condition, x,
                                                                y);
}

template <class Container, class Container1, class T, size_t Rank>
where_expr<Container, Container1, void, T, Rank>
where(const expression<Container, bool, Rank> &condition,
      const expression<Container1, T, Rank> &x,
      const typename Container1::value_type &y) {
  return where_expr<Container, Container1, void, T, Rank>(condition, x, y);
}

template <class Container, class Container1, class T, size_t Rank>
where_expr<Container, void, Container1, T, Rank>
where(const expression<Container, bool, Rank> &condition,
      const typename Container1::value_type &x,
      const expression<Container1, T, Rank> &y) {
  return where_expr<Container, void, Container1, T, Rank>(condition, x, y);
}

template <class Container, class T, size_t Rank, detail::RequiresScalar<T> = 0>
where_expr<Container, void, void, T, Rank>
where(const expression<Container, bool, Rank> &condition, const T &x,
      const T &y) {
  return where_expr<Container, void, void, T, Rank>(condition, x, y);
}

/// Rearranging elements.

/**
 * @brief Reverse the order of the elements in a tensor along the given axes.
 *
 * @param a A tensor-like object to reverse.
 * @param axes Axis or axes along which to reverse over. The default is
 *             Rank - 1, which reverses along the last axis.
 *
 * @return A light-weight object with the elements in the tensor in reversed
 *         order. This function does not create a new tensor, instead, an
 *         expression object is returned with the elements of the tensor in
 *         reversed order.
 */
template <class Container, class T, size_t Rank>
reverse_expr<Container, T, Rank, 1>
reverse(const expression<Container, T, Rank> &a, size_t axis = Rank - 1) {
  return reverse_expr<Container, T, Rank, 1>(a, axis);
}

template <class Container, class T, size_t Rank, size_t N>
reverse_expr<Container, T, Rank, N>
reverse(const expression<Container, T, Rank> &a, const shape_t<N> &axes) {
  return reverse_expr<Container, T, Rank, N>(a, axes);
}

/**
 * @brief Rotate the elements in a tensor along the given axes. The elements are
 * shifted circularly in such a way that the element at position @a shift
 * becomes the first element and the element at position @a shift - 1 becomes
 * the last element.
 *
 * @param a A tensor-like object to rotate.
 * @param shift Number of positions to shift the elements by along each axis.
 * @param axes Axis or axes along which the elements are rotated. The default is
 *             Rank - 1, which rotates along the last axis.
 *
 * @return A light-weight object with the elements in the tensor shifted
 *         circularly. This function does not create a new tensor, instead, an
 *         expression object is returned with the elements of the tensor shifted
 *         circularly.
 */
template <class Container, class T, size_t Rank>
rotate_expr<Container, T, Rank, 1>
rotate(const expression<Container, T, Rank> &a, size_t shift,
       size_t axis = Rank - 1) {
  return rotate_expr<Container, T, Rank, 1>(a, shift, axis);
}

template <class Container, class T, size_t Rank, size_t N>
rotate_expr<Container, T, Rank, N>
rotate(const expression<Container, T, Rank> &a, const index_t<N> &shift,
       const shape_t<N> &axes) {
  return rotate_expr<Container, T, Rank, N>(a, shift, axes);
}

/// Set routines.

/**
 * @brief Find the sorted unique elements of a tensor.
 *
 * @param a A tensor-like object.
 *
 * @return A new tensor with the sorted unique elements of @a a.
 *
 * @note The elements of @a a are not required to be sorted before calling
 * @c unique. However, the function might perform faster if the elements of @a a
 * are already sorted.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank>
tensor<T, 1> unique(const expression<Container, T, Rank> &a);

/**
 * @brief Test whether a value is present in a tensor.
 *
 * @param a A 1-dimensional tensor-like object. The function tests whether @a a
 *          contains @a val. The elements of @a a must be sorted.
 * @param val The value to test.
 *
 * @return true if @a val is present in @a a and false otherwise.
 */
template <class Container, class T>
bool includes(const expression<Container, T, 1> &a,
              const typename Container::value_type &val);

/**
 * @brief Test whether all the elements in a tensor are also present in another
 * tensor.
 *
 * @param a A 1-dimensional tensor-like object. The function test whether @a a
 *          contains all the elements of @a b. The elements of @a a must be
 *          sorted.
 * @param b A 1-dimensional tensor-like object with the values to test. The
 *          elements of @a b must be sorted.
 *
 * @return true if @a b is a subset of @a a and false otherwise.
 */
template <class Container1, class T, class Container2>
bool includes(const expression<Container1, T, 1> &a,
              const expression<Container2, T, 1> &b);

/**
 * @brief Find the set union of two tensors. Return the unique sorted elements
 * that are present in either one of two tensors, or in both.
 *
 * @param a A 1-dimensional tensor-like object. The elements in @a a must be
 *          sorted.
 * @param b A 1-dimensional tensor-like object. The elements in @a b must be
 *          sorted.
 *
 * @return A new tensor with the set union of both tensors.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class T, class Container2>
tensor<T, 1> set_union(const expression<Container1, T, 1> &a,
                       const expression<Container2, T, 1> &b);

/**
 * @brief Find the set intersection of two tensors. Return the unique sorted
 * elements that are present in both tensors.
 *
 * @param a A 1-dimensional tensor-like object. The elements in @a a must be
 *          sorted.
 * @param b A 1-dimensional tensor-like object. The elements in @a b must be
 *          sorted.
 *
 * @return A new tensor with the set intersection of both tensors.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class T, class Container2>
tensor<T, 1> set_intersection(const expression<Container1, T, 1> &a,
                              const expression<Container2, T, 1> &b);

/**
 * @brief Find the set difference of two tensors. Return the unique sorted
 * elements that are present in the first tensor, but not in the second.
 *
 * @param a A 1-dimensional tensor-like object. The elements in @a a must be
 *          sorted.
 * @param b A 1-dimensional tensor-like object. The elements in @a b must be
 *          sorted.
 *
 * @return A new tensor with the set difference of both tensors.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class T, class Container2>
tensor<T, 1> set_difference(const expression<Container1, T, 1> &a,
                            const expression<Container2, T, 1> &b);

/**
 * @brief Find the set symmetric difference of two tensors. Return the unique
 * sorted elements that are present in one of the tensors, but not in the other.
 *
 * @param a A 1-dimensional tensor-like object. The elements in @a a must be
 *          sorted.
 * @param b A 1-dimensional tensor-like object. The elements in @a b must be
 *          sorted.
 *
 * @return A new tensor with the set symmetric difference of both tensors.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class T, class Container2>
tensor<T, 1> set_symmetric_difference(const expression<Container1, T, 1> &a,
                                      const expression<Container2, T, 1> &b);

/// Basic statistics.

/**
 * @brief Return the average of the tensor elements.
 *
 * @param a A tensor-like object.
 *
 * @return The average of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T mean(const expression<Container, T, Rank> &a);

/**
 * @brief Return the average of the tensor elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the average is
 *             computed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the average over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> mean(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> mean(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> mean(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return the median of the tensor elements.
 *
 * @details The median is defined as the middle value of a sorted copy of the
 * tensor. If the size is even, the average of the two middle values is
 * returned.
 *
 * @param a A tensor-like object.
 *
 * @return The median of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T median(const expression<Container, T, Rank> &a);

/**
 * @brief Return the median of the tensor elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the median is
 *             computed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the median over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> median(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> median(const expression<Container, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> median(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t);

/**
 * @brief Return the variance of the tensor elements.
 *
 * @details The variance is defined as the average of the squared deviations
 * from the mean
 *     var(a) = mean(x),    x = abs(a - mean(a))**2
 * The mean is calculated as sum(x)/n, where n = x.size(). However, if @a bias
 * is false, the divisor n - 1 is used instead of n. In statistics, n - 1
 * provides an unbiased estimator of the sample variance; while n provides the
 * maximum likelihood estimator of the variance for normally distributed
 * variables.
 *
 * @param a A tensor-like object.
 * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
 *             normalization is by @a n - 1. Defaults to true.
 *
 * @return The variance of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T var(const expression<Container, T, Rank> &a, bool bias = true);

/**
 * @brief Return the variance of the tensor elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the variance is
 *             computed.
 * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
 *             normalization is by @a n - 1. Defaults to true.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the variance over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> var(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes, bool bias = true);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> var(const expression<Container, T, Rank> &a,
                    const shape_t<N> &axes, bool bias, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> var(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes, bool bias, dropdims_t);

/**
 * @brief Return the standard deviation of the tensor elements.
 *
 * @details The standard deviation is defined as the square root of the average
 * of the squared deviations from the mean
 *     stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
 * The mean is calculated as sum(x)/n, where n = x.size(). However, if @a bias
 * is false, the divisor n - 1 is used instead of n. In statistics, n - 1
 * provides an unbiased estimator of the sample variance; while n provides the
 * maximum likelihood estimator of the variance for normally distributed
 * variables.
 *
 * @param a A tensor-like object.
 * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
 *             normalization is by @a n - 1. Defaults to true.
 *
 * @return The standard deviation of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T stddev(const expression<Container, T, Rank> &a, bool bias = true);

/**
 * @brief Return the standard deviation of the tensor elements over the given
 * axes.
 *
 * @param a A tensor-like object.
 * @param axes A @c shape_t object with the axes along which the standard
 *             deviation is computed.
 * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
 *             normalization is by @a n - 1. Defaults to true.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the standard deviation over the given axes. The
 *         output tensor has the same dimension as @a a, but the axes which are
 *         reduced are left as dimensions of size one.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> stddev(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, bool bias = true);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> stddev(const expression<Container, T, Rank> &a,
                       const shape_t<N> &axes, bool bias, keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> stddev(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, bool bias, dropdims_t);

/**
 * @brief Return the q-th quantile of the tensor elements.
 *
 * @details Given a tensor of size n, the q-th quantile is the (n - 1)*q-th
 * value of a sorted copy of the tensor. If (n - 1)*q is not an integer, the
 * quantile is a weighted average of the two nearest neighbors. The different
 * methods work as follows:
 *   "lower" Takes the lowest neighbor.
 *   "higher" Takes the highest neighbor.
 *   "midpoint" Takes the average of both neighbors.
 *   "nearest" Takes the nearest neighbor.
 *   "linear" Takes a linear interpolation between both neighbors.
 *
 * @param a A tensor-like object.
 * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
 * @param method This parameter specifies the method to use for estimating the
 *               quantile. Must be one of "lower", "higher", "nearest",
 *               "midpoint" or "linear".
 *
 * @return The q-th quantile of the tensor elements.
 */
template <class Container, class T, size_t Rank>
T quantile(const expression<Container, T, Rank> &a, double q,
           const std::string &method = "linear");

/**
 * @brief Return the q-th quantile of the tensor elements over the given axes.
 *
 * @param a A tensor-like object.
 * @param q Quantile to compute, which must be between 0 and 1 (inclusive).
 * @param axes A @c shape_t object with the axes along which the quantile is
 *             computed.
 * @param method This parameter specifies the method to use for estimating the
 *               quantile. Must be one of "lower", "higher", "nearest",
 *               "midpoint" or "linear".
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 *
 * @return A new tensor with the quantile over the given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> quantile(const expression<Container, T, Rank> &a, double q,
                             const shape_t<N> &axes,
                             const std::string &method = "linear");

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> quantile(const expression<Container, T, Rank> &a, double q,
                         const shape_t<N> &axes, const std::string &method,
                         keepdims_t);

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> quantile(const expression<Container, T, Rank> &a, double q,
                             const shape_t<N> &axes, const std::string &method,
                             dropdims_t);

/**
 * @brief Return the covariance of two 1-dimensional tensors.
 *
 * @details The covariance is defined as the average of the element-wise
 * products of the deviations from the mean
 *     cov(x, y) = mean(a),    a = (x - mean(x))*(y - mean(y))
 * For complex types, the complex conjugate of y is used
 *     cov(x, y) = mean(a),    a = (x - mean(x))*conj(y - mean(y))
 * The mean is calculated as sum(a)/n, where n = a.size(). However, if @a bias
 * is false, the divisor n - 1 is used instead of n.
 *
 * @param x First tensor-like argument.
 * @param y Second tensor-like argument.
 * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
 *             normalization is by @a n - 1. Default is false.
 *
 * @return The covariance of the two tensors.
 *
 * @throw std::invalid_argument Thrown if the tensor arguments have different
 *                              sizes.
 */
template <class Container1, class T, class Container2>
T cov(const expression<Container1, T, 1> &x,
      const expression<Container2, T, 1> &y, bool bias = false);

template <class Container1, class T, class Container2>
T cov(const expression<Container1, std::complex<T>, 1> &x,
      const expression<Container2, std::complex<T>, 1> &y, bool bias = false);

/**
 * @brief Return the covariance matrix of given data.
 *
 * @param a A 2-dimensional tensor-like object containing multiple variables and
 *          observations.
 * @param rowvar If true, then each row represents a variable, with observations
 *               in the columns. Otherwise, each column represents a variable,
 *               with observations in the rows. Default is true.
 * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
 *             normalization is by @a n - 1. Default is false.
 *
 * @return The covariance matrix of the variables. The element at position
 *         @a (i,j) is equal to the covariance of the @a i -th and @a j -th
 *         variables.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T>
tensor<T, 2> cov(const expression<Container, T, 2> &a, bool rowvar = true,
                 bool bias = false);

template <class Container, class T>
tensor<std::complex<T>, 2>
cov(const expression<Container, std::complex<T>, 2> &a, bool rowvar = true,
    bool bias = false);

/**
 * @brief Return the Pearson's correlation coefficient of two 1-dimensional
 * tensors.
 *
 * @details The correlation coefficient is defined as the covariance of the
 * variables divided by the product of their standard deviations
 *     corr(x, y) = cov(x, y) / (stddev(x) * stddev(y))
 *
 * @param x First tensor-like argument.
 * @param y Second tensor-like argument.
 *
 * @return The correlation coefficient of the two tensors.
 *
 * @throw std::invalid_argument Thrown if the tensor arguments have different
 *                              sizes.
 */
template <class Container1, class T, class Container2>
T corrcoef(const expression<Container1, T, 1> &x,
           const expression<Container2, T, 1> &y);

/**
 * @brief Return the correlation matrix of given data.
 *
 * @param a A 2-dimensional tensor-like object containing multiple variables and
 *          observations.
 * @param rowvar If true, then each row represents a variable, with observations
 *               in the columns. Otherwise, each column represents a variable,
 *               with observations in the rows. Default is true.
 *
 * @return The correlation matrix of the variables. The element at position
 *         @a (i,j) is equal to the correlation coefficient of the @a i -th and
 *         @a j -th variables.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container, class T>
tensor<T, 2> corrcoef(const expression<Container, T, 2> &a, bool rowvar = true);
} // namespace numcpp

#include "numcpp/routines/routines.tcc"

#endif // NUMCPP_ROUTINES_H_INCLUDED
