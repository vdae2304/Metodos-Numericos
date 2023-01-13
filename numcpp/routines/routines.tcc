/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
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
#include <vector>

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
inline tensor<T, Rank>
full_like(const base_tensor<T, Rank, Tag> &like,
          const typename base_tensor<T, Rank, Tag>::value_type &val) {
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
inline base_tensor<typename std::common_type<T, U, V>::type, 1, sequence_tag>
arange(const T &start, const U &stop, const V &step) {
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
inline base_tensor<typename std::common_type<T, U, V>::type, 1, sequence_tag>
logspace(const T &start, const U &stop, size_t num, bool endpoint,
         const V &base) {
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

template <class T> inline base_tensor<T, 2, eye_tag> eye(size_t n) {
  return base_tensor<T, 2, eye_tag>(n, n);
}

template <class T>
inline base_tensor<T, 2, eye_tag> eye(size_t m, size_t n, ptrdiff_t k) {
  return base_tensor<T, 2, eye_tag>(m, n, k);
}

template <class T, class Tag>
inline base_tensor<T, 1, diagonal_tag<Tag>>
diag(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
  typedef diagonal_tag<Tag> Closure;
  return base_tensor<T, 1, Closure>(a, k);
}

template <class T, class Tag>
inline base_tensor<T, 2, diagonal_tag<Tag>>
diag(const base_tensor<T, 1, Tag> &a, ptrdiff_t k) {
  typedef diagonal_tag<Tag> Closure;
  return base_tensor<T, 2, Closure>(a, k);
}

template <class T, class Tag>
inline base_tensor<T, 2, triangular_tag<Tag>>
tril(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
  typedef triangular_tag<Tag> Closure;
  return base_tensor<T, 2, Closure>(a, true, k);
}

template <class T, class Tag>
inline base_tensor<T, 2, triangular_tag<Tag>>
triu(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
  typedef triangular_tag<Tag> Closure;
  return base_tensor<T, 2, Closure>(a, false, k);
}

template <class T, class Tag>
inline tensor<T, 2> vander(const base_tensor<T, 1, Tag> &x) {
  return vander(x, x.size());
}

template <class T, class Tag>
tensor<T, 2> vander(const base_tensor<T, 1, Tag> &x, size_t N,
                    bool increasing) {
  tensor<T, 2> out(x.size(), N);
  for (size_t i = 0; i < x.size(); ++i) {
    T pow = T(1);
    for (size_t j = 0; j < N; ++j) {
      if (increasing) {
        out(i, j) = pow;
      } else {
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
tensor<size_t, Rank> argmax(const base_tensor<T, Rank, Tag> &a, size_t axis) {
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
tensor<size_t, Rank> argmin(const base_tensor<T, Rank, Tag> &a, size_t axis) {
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
inline base_tensor<T, Rank, lazy_binary_tag<ranges::maximum, T, Tag1, T, Tag2>>
maximum(const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b) {
  typedef lazy_binary_tag<ranges::maximum, T, Tag1, T, Tag2> Closure;
  return base_tensor<T, Rank, Closure>(ranges::maximum(), a, b);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank,
                   lazy_binary_tag<ranges::maximum, T, Tag, T, scalar_tag>>
maximum(const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val) {
  typedef lazy_binary_tag<ranges::maximum, T, Tag, T, scalar_tag> Closure;
  return base_tensor<T, Rank, Closure>(ranges::maximum(), a, val);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank,
                   lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag>>
maximum(const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b) {
  typedef lazy_binary_tag<ranges::maximum, T, scalar_tag, T, Tag> Closure;
  return base_tensor<T, Rank, Closure>(ranges::maximum(), val, b);
}

template <class T, size_t Rank, class Tag1, class Tag2>
inline base_tensor<T, Rank, lazy_binary_tag<ranges::minimum, T, Tag1, T, Tag2>>
minimum(const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b) {
  typedef lazy_binary_tag<ranges::minimum, T, Tag1, T, Tag2> Closure;
  return base_tensor<T, Rank, Closure>(ranges::minimum(), a, b);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank,
                   lazy_binary_tag<ranges::minimum, T, Tag, T, scalar_tag>>
minimum(const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val) {
  typedef lazy_binary_tag<ranges::minimum, T, Tag, T, scalar_tag> Closure;
  return base_tensor<T, Rank, Closure>(ranges::minimum(), a, val);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank,
                   lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag>>
minimum(const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b) {
  typedef lazy_binary_tag<ranges::minimum, T, scalar_tag, T, Tag> Closure;
  return base_tensor<T, Rank, Closure>(ranges::minimum(), val, b);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank, lazy_unary_tag<ranges::clamp<T>, T, Tag>>
clamp(const base_tensor<T, Rank, Tag> &a,
      const typename base_tensor<T, Rank, Tag>::value_type &a_min,
      const typename base_tensor<T, Rank, Tag>::value_type &a_max) {
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
tensor<bool, Rank> all(const base_tensor<bool, Rank, Tag> &a, size_t axis) {
  tensor<bool, Rank> out;
  apply_along_axis(out, ranges::all(), a, axis);
  return out;
}

template <size_t Rank, class Tag, size_t N>
tensor<bool, Rank> all(const base_tensor<bool, Rank, Tag> &a,
                       const shape_t<N> &axes) {
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
tensor<bool, Rank> any(const base_tensor<bool, Rank, Tag> &a, size_t axis) {
  tensor<bool, Rank> out;
  apply_along_axis(out, ranges::any(), a, axis);
  return out;
}

template <size_t Rank, class Tag, size_t N>
tensor<bool, Rank> any(const base_tensor<bool, Rank, Tag> &a,
                       const shape_t<N> &axes) {
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
tensor<size_t, Rank> count_nonzero(const base_tensor<T, Rank, Tag> &a,
                                   size_t axis) {
  tensor<size_t, Rank> out;
  apply_along_axis(out, ranges::count_nonzero(), a, axis);
  return out;
}

template <class T, size_t Rank, class Tag, size_t N>
tensor<size_t, Rank> count_nonzero(const base_tensor<T, Rank, Tag> &a,
                                   const shape_t<N> &axes) {
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

bool isclose(long double a, long double b, long double rtol, long double atol) {
  ranges::isclose<long double> pred(rtol, atol);
  return pred(a, b);
}

template <class T, class U>
typename std::enable_if<
    std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, bool>::type
isclose(T a, U b, typename std::common_type<T, U, double>::type rtol,
        typename std::common_type<T, U, double>::type atol) {
  typedef typename std::common_type<T, U, double>::type Rt;
  ranges::isclose<Rt> pred(rtol, atol);
  return pred(a, b);
}

template <class T>
bool isclose(const std::complex<T> &a, const std::complex<T> &b,
             typename std::complex<T>::value_type rtol,
             typename std::complex<T>::value_type atol) {
  ranges::isclose<std::complex<T>> pred(rtol, atol);
  return pred(a, b);
}

template <class T>
bool isclose(const std::complex<T> &a,
             const typename std::complex<T>::value_type &b,
             typename std::complex<T>::value_type rtol,
             typename std::complex<T>::value_type atol) {
  ranges::isclose<std::complex<T>> pred(rtol, atol);
  return pred(a, b);
}

template <class T>
bool isclose(const typename std::complex<T>::value_type &a,
             const std::complex<T> &b,
             typename std::complex<T>::value_type rtol,
             typename std::complex<T>::value_type atol) {
  ranges::isclose<std::complex<T>> pred(rtol, atol);
  return pred(a, b);
}

template <class T, class U>
bool isclose(const std::complex<T> &a, const std::complex<U> &b,
             typename std::common_type<T, U>::type rtol,
             typename std::common_type<T, U>::type atol) {
  typedef typename std::common_type<T, U>::type Rt;
  ranges::isclose<std::complex<Rt>> pred(rtol, atol);
  return pred(a, b);
}

template <class T, size_t Rank, class Tag1, class Tag2>
inline base_tensor<bool, Rank,
                   lazy_binary_tag<ranges::isclose<T>, T, Tag1, T, Tag2>>
isclose(const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const typename base_tensor<T, Rank, Tag1>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag2>::value_type &atol) {
  typedef lazy_binary_tag<ranges::isclose<T>, T, Tag1, T, Tag2> Closure;
  return base_tensor<bool, Rank, Closure>(ranges::isclose<T>(rtol, atol), a, b);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<bool, Rank,
                   lazy_binary_tag<ranges::isclose<T>, T, Tag, T, scalar_tag>>
isclose(const base_tensor<T, Rank, Tag> &a,
        const typename base_tensor<T, Rank, Tag>::value_type &val,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag>::value_type &atol) {
  typedef lazy_binary_tag<ranges::isclose<T>, T, Tag, T, scalar_tag> Closure;
  return base_tensor<bool, Rank, Closure>(ranges::isclose<T>(rtol, atol), a,
                                          val);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<bool, Rank,
                   lazy_binary_tag<ranges::isclose<T>, T, scalar_tag, T, Tag>>
isclose(const typename base_tensor<T, Rank, Tag>::value_type &val,
        const base_tensor<T, Rank, Tag> &b,
        const typename base_tensor<T, Rank, Tag>::value_type &rtol,
        const typename base_tensor<T, Rank, Tag>::value_type &atol) {
  typedef lazy_binary_tag<ranges::isclose<T>, T, scalar_tag, T, Tag> Closure;
  return base_tensor<bool, Rank, Closure>(ranges::isclose<T>(rtol, atol), val,
                                          b);
}

template <class T, size_t Rank, class Tag1, class Tag2>
inline base_tensor<
    bool, Rank,
    lazy_binary_tag<ranges::isclose<std::complex<T>>, T, Tag1, T, Tag2>>
isclose(const base_tensor<std::complex<T>, Rank, Tag1> &a,
        const base_tensor<std::complex<T>, Rank, Tag2> &b,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol) {
  typedef lazy_binary_tag<ranges::isclose<std::complex<T>>, T, Tag1, T, Tag2>
      Closure;
  return base_tensor<bool, Rank, Closure>(
      ranges::isclose<std::complex<T>>(rtol, atol), a, b);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<
    bool, Rank,
    lazy_binary_tag<ranges::isclose<std::complex<T>>, T, Tag, T, scalar_tag>>
isclose(const base_tensor<std::complex<T>, Rank, Tag> &a,
        const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol) {
  typedef lazy_binary_tag<ranges::isclose<std::complex<T>>, T, Tag, T,
                          scalar_tag>
      Closure;
  return base_tensor<bool, Rank, Closure>(
      ranges::isclose<std::complex<T>>(rtol, atol), a, val);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<
    bool, Rank,
    lazy_binary_tag<ranges::isclose<std::complex<T>>, T, scalar_tag, T, Tag>>
isclose(const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
        const base_tensor<std::complex<T>, Rank, Tag> &b,
        const typename std::complex<T>::value_type &rtol,
        const typename std::complex<T>::value_type &atol) {
  typedef lazy_binary_tag<ranges::isclose<std::complex<T>>, T, scalar_tag, T,
                          Tag>
      Closure;
  return base_tensor<bool, Rank, Closure>(
      ranges::isclose<std::complex<T>>(rtol, atol), val, b);
}

template <class T, size_t Rank, class Tag1, class Tag2>
inline bool
allclose(const base_tensor<T, Rank, Tag1> &a,
         const base_tensor<T, Rank, Tag2> &b,
         const typename base_tensor<T, Rank, Tag1>::value_type &rtol,
         const typename base_tensor<T, Rank, Tag2>::value_type &atol) {
  return all(isclose(a, b, rtol, atol));
}

template <class T, size_t Rank, class Tag>
inline bool
allclose(const base_tensor<T, Rank, Tag> &a,
         const typename base_tensor<T, Rank, Tag>::value_type &val,
         const typename base_tensor<T, Rank, Tag>::value_type &rtol,
         const typename base_tensor<T, Rank, Tag>::value_type &atol) {
  return all(isclose(a, val, rtol, atol));
}

template <class T, size_t Rank, class Tag>
inline bool
allclose(const typename base_tensor<T, Rank, Tag>::value_type &val,
         const base_tensor<T, Rank, Tag> &b,
         const typename base_tensor<T, Rank, Tag>::value_type &rtol,
         const typename base_tensor<T, Rank, Tag>::value_type &atol) {
  return all(isclose(val, b, rtol, atol));
}

template <class T, size_t Rank, class Tag1, class Tag2>
inline bool allclose(const base_tensor<std::complex<T>, Rank, Tag1> &a,
                     const base_tensor<std::complex<T>, Rank, Tag2> &b,
                     const typename std::complex<T>::value_type &rtol,
                     const typename std::complex<T>::value_type &atol) {
  return all(isclose(a, b, rtol, atol));
}

template <class T, size_t Rank, class Tag>
inline bool allclose(
    const base_tensor<std::complex<T>, Rank, Tag> &a,
    const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
    const typename std::complex<T>::value_type &rtol,
    const typename std::complex<T>::value_type &atol) {
  return all(isclose(a, val, rtol, atol));
}

template <class T, size_t Rank, class Tag>
inline bool allclose(
    const typename base_tensor<std::complex<T>, Rank, Tag>::value_type &val,
    const base_tensor<std::complex<T>, Rank, Tag> &b,
    const typename std::complex<T>::value_type &rtol,
    const typename std::complex<T>::value_type &atol) {
  return all(isclose(val, b, rtol, atol));
}

/// Sorting and searching.

template <class T, size_t Rank, class Tag>
inline tensor<index_t<Rank>, 1> argsort(const base_tensor<T, Rank, Tag> &a) {
  return argsort(a, less());
}

template <class T, size_t Rank, class Tag, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<index_t<Rank>, 1> argsort(const base_tensor<T, Rank, Tag> &a,
                                 Compare comp, bool stable) {
  index_sequence<Rank> indices(a.shape());
  tensor<index_t<Rank>, 1> out(indices.begin(), a.size());
  if (stable) {
    std::stable_sort(out.begin(), out.end(),
                     [&](const index_t<Rank> &i, const index_t<Rank> &j) {
                       return comp(a[i], a[j]);
                     });
  } else {
    std::sort(out.begin(), out.end(),
              [&](const index_t<Rank> &i, const index_t<Rank> &j) {
                return comp(a[i], a[j]);
              });
  }
  return out;
}

template <class T, size_t Rank, class Tag>
inline tensor<size_t, Rank> argsort(const base_tensor<T, Rank, Tag> &a,
                                    size_t axis) {
  return argsort(a, axis, less());
}

template <class T, size_t Rank, class Tag, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<size_t, Rank> argsort(const base_tensor<T, Rank, Tag> &a, size_t axis,
                             Compare comp, bool stable) {
  shape_t<Rank> shape = a.shape();
  tensor<size_t, Rank> out(shape);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    auto first = make_axes_iterator(&out, out_index, axis, 0);
    auto last = make_axes_iterator(&out, out_index, axis, size);
    std::iota(first, last, 0);
    index_t<Rank> i = out_index, j = out_index;
    if (stable) {
      std::stable_sort(first, last, [&](size_t i_axis, size_t j_axis) {
        i[axis] = i_axis;
        j[axis] = j_axis;
        return comp(a[i], a[j]);
      });
    } else {
      std::sort(first, last, [&](size_t i_axis, size_t j_axis) {
        i[axis] = i_axis;
        j[axis] = j_axis;
        return comp(a[i], a[j]);
      });
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
          detail::RequiresCallable<Compare, T, T>>
tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
sort(const base_tensor<T, Rank, Tag> &a, Compare comp, bool stable) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, 1> out(a.begin(), a.size());
  if (stable) {
    std::stable_sort(out.begin(), out.end(), comp);
  } else {
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
          detail::RequiresCallable<Compare, T, T>>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
sort(const base_tensor<T, Rank, Tag> &a, size_t axis, Compare comp,
     bool stable) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out(a);
  out.sort(axis, comp, stable);
  return out;
}

template <class T, size_t Rank, class Tag>
inline tensor<index_t<Rank>, 1> argpartition(const base_tensor<T, Rank, Tag> &a,
                                             size_t kth) {
  return argpartition(a, kth, less());
}

template <class T, size_t Rank, class Tag, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<index_t<Rank>, 1> argpartition(const base_tensor<T, Rank, Tag> &a,
                                      size_t kth, Compare comp) {
  index_sequence<Rank> indices(a.shape());
  tensor<index_t<Rank>, 1> out(indices.begin(), a.size());
  std::nth_element(out.begin(), out.begin() + kth, out.end(),
                   [&](const index_t<Rank> &i, const index_t<Rank> &j) {
                     return comp(a[i], a[j]);
                   });
  return out;
}

template <class T, size_t Rank, class Tag>
inline tensor<size_t, Rank> argpartition(const base_tensor<T, Rank, Tag> &a,
                                         size_t kth, size_t axis) {
  return argpartition(a, kth, axis, less());
}

template <class T, size_t Rank, class Tag, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<size_t, Rank> argpartition(const base_tensor<T, Rank, Tag> &a,
                                  size_t kth, size_t axis, Compare comp) {
  shape_t<Rank> shape = a.shape();
  tensor<size_t, Rank> out(shape);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    auto first = make_axes_iterator(&out, out_index, axis, 0);
    auto last = make_axes_iterator(&out, out_index, axis, size);
    std::iota(first, last, 0);
    index_t<Rank> i = out_index, j = out_index;
    std::nth_element(first, first + kth, last,
                     [&](size_t i_axis, size_t j_axis) {
                       i[axis] = i_axis;
                       j[axis] = j_axis;
                       return comp(a[i], a[j]);
                     });
  }
  return out;
}

template <class T, size_t Rank, class Tag>
inline tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
partition(const base_tensor<T, Rank, Tag> &a, size_t kth) {
  return partition(a, kth, less());
}

template <class T, size_t Rank, class Tag, class Compare,
          detail::RequiresCallable<Compare, T, T>>
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
          detail::RequiresCallable<Compare, T, T>>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
partition(const base_tensor<T, Rank, Tag> &a, size_t kth, size_t axis,
          Compare comp) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out(a);
  out.partition(kth, axis, comp);
  return out;
}

template <class T, size_t Rank, class Tag>
tensor<index_t<Rank>, 1> nonzero(const base_tensor<T, Rank, Tag> &a) {
  size_t size = a.size() - std::count(a.begin(), a.end(), T());
  tensor<index_t<Rank>, 1> out(size);
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    if (a[i] != T()) {
      out[n++] = i;
    }
  }
  return out;
}

template <size_t Rank, class Tag>
tensor<index_t<Rank>, 1> where(const base_tensor<bool, Rank, Tag> &condition) {
  size_t size = std::count(condition.begin(), condition.end(), true);
  tensor<index_t<Rank>, 1> out(size);
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      out[n++] = i;
    }
  }
  return out;
}

template <class T, size_t Rank, class TagCond, class TagTrue, class TagFalse>
inline base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse>>
where(const base_tensor<bool, Rank, TagCond> &condition,
      const base_tensor<T, Rank, TagTrue> &x,
      const base_tensor<T, Rank, TagFalse> &y) {
  typedef lazy_where_tag<TagCond, TagTrue, TagFalse> Closure;
  return base_tensor<T, Rank, Closure>(condition, x, y);
}

template <class T, size_t Rank, class TagCond, class TagTrue>
inline base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag>>
where(const base_tensor<bool, Rank, TagCond> &condition,
      const base_tensor<T, Rank, TagTrue> &x,
      const typename base_tensor<T, Rank, TagTrue>::value_type &y) {
  typedef lazy_where_tag<TagCond, TagTrue, scalar_tag> Closure;
  return base_tensor<T, Rank, Closure>(condition, x, y);
}

template <class T, size_t Rank, class TagCond, class TagFalse>
inline base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse>>
where(const base_tensor<bool, Rank, TagCond> &condition,
      const typename base_tensor<T, Rank, TagFalse>::value_type &x,
      const base_tensor<T, Rank, TagFalse> &y) {
  typedef lazy_where_tag<TagCond, scalar_tag, TagFalse> Closure;
  return base_tensor<T, Rank, Closure>(condition, x, y);
}

/// Rearranging elements.

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank, flip_tag<Tag, 1>>
reverse(const base_tensor<T, Rank, Tag> &a, size_t axis) {
  typedef flip_tag<Tag, 1> Closure;
  return base_tensor<T, Rank, Closure>(a, axis);
}

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor<T, Rank, flip_tag<Tag, N>>
reverse(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes) {
  typedef flip_tag<Tag, N> Closure;
  return base_tensor<T, Rank, Closure>(a, axes);
}

template <class T, size_t Rank, class Tag>
inline base_tensor<T, Rank, roll_tag<Tag, 1>>
rotate(const base_tensor<T, Rank, Tag> &a, size_t shift, size_t axis) {
  typedef roll_tag<Tag, 1> Closure;
  return base_tensor<T, Rank, Closure>(a, shift, axis);
}

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor<T, Rank, roll_tag<Tag, N>>
rotate(const base_tensor<T, Rank, Tag> &a, const index_t<N> &shift,
       const shape_t<N> &axes) {
  typedef roll_tag<Tag, N> Closure;
  return base_tensor<T, Rank, Closure>(a, shift, axes);
}

/// Set routines.

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
unique(const base_tensor<T, Rank, Tag> &arg) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  std::vector<Rt> buffer(arg.begin(), arg.end());
  if (!std::is_sorted(buffer.begin(), buffer.end())) {
    std::sort(buffer.begin(), buffer.end());
  }
  size_t size = std::unique(buffer.begin(), buffer.end()) - buffer.begin();
  return tensor<Rt, 1>(buffer.begin(), size);
}

template <class T, class Tag>
bool includes(const base_tensor<T, 1, Tag> &arg,
              const typename base_tensor<T, 1, Tag>::value_type &val) {
  return std::binary_search(arg.begin(), arg.end(), val);
}

template <class T, class Tag1, class Tag2>
bool includes(const base_tensor<T, 1, Tag1> &arg1,
              const base_tensor<T, 1, Tag2> &arg2) {
  return std::includes(arg1.begin(), arg1.end(), arg2.begin(), arg2.end());
}

template <class T, class Tag1, class Tag2>
tensor<T, 1> set_union(const base_tensor<T, 1, Tag1> &arg1,
                       const base_tensor<T, 1, Tag2> &arg2) {
  std::vector<T> buffer;
  std::set_union(arg1.begin(), arg1.end(), arg2.begin(), arg2.end(),
                 std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

template <class T, class Tag1, class Tag2>
tensor<T, 1> set_intersection(const base_tensor<T, 1, Tag1> &arg1,
                              const base_tensor<T, 1, Tag2> &arg2) {
  std::vector<T> buffer;
  std::set_intersection(arg1.begin(), arg1.end(), arg2.begin(), arg2.end(),
                        std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

template <class T, class Tag1, class Tag2>
tensor<T, 1> set_difference(const base_tensor<T, 1, Tag1> &arg1,
                            const base_tensor<T, 1, Tag2> &arg2) {
  std::vector<T> buffer;
  std::set_difference(arg1.begin(), arg1.end(), arg2.begin(), arg2.end(),
                      std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

template <class T, class Tag1, class Tag2>
tensor<T, 1> set_symmetric_difference(const base_tensor<T, 1, Tag1> &arg1,
                                      const base_tensor<T, 1, Tag2> &arg2) {
  std::vector<T> buffer;
  std::set_symmetric_difference(arg1.begin(), arg1.end(), arg2.begin(),
                                arg2.end(), std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
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
stddev(const base_tensor<T, Rank, Tag> &a, const shape_t<N> &axes, bool bias) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out;
  apply_over_axes(out, ranges::stddev(bias), a, axes);
  return out;
}

template <class T, size_t Rank, class Tag>
typename base_tensor<T, Rank, Tag>::value_type
quantile(const base_tensor<T, Rank, Tag> &a, double q,
         const std::string &method) {
  ranges::quantile pred(q, method);
  return pred(a.begin(), a.end());
}

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
quantile(const base_tensor<T, Rank, Tag> &a, double q, size_t axis,
         const std::string &method) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out;
  apply_along_axis(out, ranges::quantile(q, method), a, axis);
  return out;
}

template <class T, size_t Rank, class Tag, size_t N>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
quantile(const base_tensor<T, Rank, Tag> &a, double q, const shape_t<N> &axes,
         const std::string &method) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out;
  apply_over_axes(out, ranges::quantile(q, method), a, axes);
  return out;
}

template <class T, class Tag1, class Tag2>
T cov(const base_tensor<T, 1, Tag1> &x, const base_tensor<T, 1, Tag2> &y,
      bool bias) {
  if (x.size() != y.size()) {
    throw std::invalid_argument("all the tensors must have the same shape");
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
std::complex<T> cov(const base_tensor<std::complex<T>, 1, Tag1> &x,
                    const base_tensor<std::complex<T>, 1, Tag2> &y, bool bias) {
  if (x.size() != y.size()) {
    throw std::invalid_argument("all the tensors must have the same shape");
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
      T val = T(0);
      for (size_t k = 0; k < size; ++k) {
        T x, y;
        if (rowvar) {
          x = a(i, k) - a_mean(i, 0);
          y = a(j, k) - a_mean(j, 0);
        } else {
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
tensor<std::complex<T>, 2> cov(const base_tensor<std::complex<T>, 2, Tag> &a,
                               bool rowvar, bool bias) {
  size_t nvar = rowvar ? a.shape(0) : a.shape(1);
  size_t size = rowvar ? a.shape(1) : a.shape(0);
  tensor<std::complex<T>, 2> out(nvar, nvar);
  tensor<std::complex<T>, 2> a_mean = mean(a, rowvar);
  for (size_t i = 0; i < nvar; ++i) {
    for (size_t j = 0; j < nvar; ++j) {
      std::complex<T> val = T(0);
      for (size_t k = 0; k < size; ++k) {
        std::complex<T> x, y;
        if (rowvar) {
          x = a(i, k) - a_mean(i, 0);
          y = a(j, k) - a_mean(j, 0);
        } else {
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
T corrcoef(const base_tensor<T, 1, Tag1> &x, const base_tensor<T, 1, Tag2> &y) {
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
} // namespace numcpp

#endif // NUMCPP_ROUTINES_TCC_INCLUDED
