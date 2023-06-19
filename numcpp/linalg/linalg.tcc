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

/** @file include/numcpp/linalg/linalg.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/linalg.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LINALG_TCC_INCLUDED
#define NUMCPP_LINALG_TCC_INCLUDED

#include <numeric>
#include "numcpp/broadcasting/assert.h"
#include "numcpp/math/constants.h"
#include "numcpp/iterators/axes_iterator.h"

namespace numcpp {
/// Basic linear algebra.

template <class Container1, class Container2, class T>
T dot(const expression<Container1, T, 1> &a,
      const expression<Container2, T, 1> &b) {
  detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
  size_t n = a.size();
  T val = T(0);
  for (size_t i = 0; i < n; ++i) {
    val += a[i] * b[i];
  }
  return val;
}

template <class Container1, class Container2, class T>
inline T vdot(const expression<Container1, T, 1> &a,
              const expression<Container2, T, 1> &b) {
  return dot(a, b);
}

template <class Container1, class Container2, class T>
std::complex<T> vdot(const expression<Container1, std::complex<T>, 1> &a,
                     const expression<Container2, std::complex<T>, 1> &b) {
  detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
  size_t n = a.size();
  std::complex<T> val = T(0);
  for (size_t i = 0; i < n; ++i) {
    val += std::conj(a[i]) * b[i];
  }
  return val;
}

namespace detail {
/**
 * @brief Broadcast two shapes for cross product.
 */
template <size_t Rank>
shape_t<Rank> broadcast_cross(const shape_t<Rank> &shape1,
                              const shape_t<Rank> &shape2, size_t axis) {
  if (shape1[axis] != 3 || shape2[axis] != 3) {
    throw std::invalid_argument(
        "incompatible dimensions for cross product (dimension must be 3)");
  }
  shape_t<Rank> out_shape = shape1;
  for (size_t i = 0; i < Rank; ++i) {
    if (i == axis) {
      out_shape[i] = 3;
    } else if (shape1[i] == 1) {
      out_shape[i] = shape2[i];
    } else if (shape2[i] != shape1[i] && shape2[i] != 1) {
      std::ostringstream error;
      error << "operands could not be broadcast together with shapes " << shape1
            << " " << shape2;
      throw std::invalid_argument(error.str());
    }
  }
  return out_shape;
}
} // namespace detail

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> cross(const expression<Container1, T, Rank> &a,
                      const expression<Container2, T, Rank> &b, size_t axis) {
  shape_t<Rank> shape = detail::broadcast_cross(a.shape(), b.shape(), axis);
  tensor<T, Rank> out(shape);
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    index_t<Rank> a_index, b_index;
    for (size_t axis = 0; axis < Rank; ++axis) {
      a_index[axis] = (a.shape(axis) > 1) ? index[axis] : 0;
      b_index[axis] = (b.shape(axis) > 1) ? index[axis] : 0;
    }
    T u[3], v[3];
    for (a_index[axis] = 0; a_index[axis] < 3; ++a_index[axis]) {
      u[a_index[axis]] = a[a_index];
    }
    for (b_index[axis] = 0; b_index[axis] < 3; ++b_index[axis]) {
      v[b_index[axis]] = b[b_index];
    }
    T w[3] = {u[1] * v[2] - v[1] * u[2], v[0] * u[2] - u[0] * v[2],
              u[0] * v[1] - v[0] * u[1]};
    for (index[axis] = 0; index[axis] < 3; ++index[axis]) {
      out[index] = w[index[axis]];
    }
  }
  return out;
}

template <class Container1, class Container2, class T>
inline T matmul(const expression<Container1, T, 1> &a,
                const expression<Container2, T, 1> &b) {
  return dot(a, b);
}

template <class Container1, class Container2, class T>
tensor<T, 2> matmul(const expression<Container1, T, 2> &a,
                    const expression<Container2, T, 2> &b) {
  detail::assert_aligned_shapes(a.shape(), 1, b.shape(), 0);
  size_t m = a.shape(0), p = a.shape(1), n = b.shape(1);
  tensor<T, 2> out(m, n);
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      T val = T(0);
      for (size_t k = 0; k < p; ++k) {
        val += a[{i, k}] * b[{k, j}];
      }
      out[{i, j}] = val;
    }
  }
  return out;
}

template <class Container1, class Container2, class T>
tensor<T, 1> matmul(const expression<Container1, T, 1> &a,
                    const expression<Container2, T, 2> &b) {
  detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
  size_t m = b.shape(0), n = b.shape(1);
  tensor<T, 1> out(n);
  for (size_t j = 0; j < n; ++j) {
    T val = T(0);
    for (size_t i = 0; i < m; ++i) {
      val += a[i] * b[{i, j}];
    }
    out[j] = val;
  }
  return out;
}

template <class Container1, class Container2, class T>
tensor<T, 1> matmul(const expression<Container1, T, 2> &a,
                    const expression<Container2, T, 1> &b) {
  detail::assert_aligned_shapes(a.shape(), 1, b.shape(), 0);
  size_t m = a.shape(0), n = a.shape(1);
  tensor<T, 1> out(m);
  for (size_t i = 0; i < m; ++i) {
    T val = T(0);
    for (size_t j = 0; j < n; ++j) {
      val += a[{i, j}] * b[j];
    }
    out[i] = val;
  }
  return out;
}

namespace detail {
/**
 * @brief Broadcast two shapes for matrix multiplication.
 */
template <size_t Rank>
shape_t<Rank> broadcast_matmul(const shape_t<Rank> &shape1,
                               const shape_t<Rank> &shape2) {
  shape_t<Rank> out_shape = shape1;
  for (size_t i = 0; i < Rank - 2; ++i) {
    if (shape1[i] == 1) {
      out_shape[i] = shape2[i];
    } else if (shape2[i] != shape1[i] && shape2[i] != 1) {
      std::ostringstream error;
      error << "operands could not be broadcast together with shapes " << shape1
            << " " << shape2;
      throw std::invalid_argument(error.str());
    }
  }
  out_shape[Rank - 2] = shape1[Rank - 2];
  out_shape[Rank - 1] = shape2[Rank - 1];
  return out_shape;
}
} // namespace detail

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> matmul(const expression<Container1, T, Rank> &a,
                       const expression<Container2, T, Rank> &b) {
  size_t axis1 = Rank - 1, axis2 = Rank - 2;
  detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
  shape_t<Rank> shape = detail::broadcast_matmul(a.shape(), b.shape());
  size_t n = a.shape(axis1);
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence_for(out)) {
    index_t<Rank> a_index, b_index;
    T val = T(0);
    for (size_t axis = 0; axis < Rank - 2; ++axis) {
      a_index[axis] = (a.shape(axis) > 1) ? out_index[axis] : 0;
      b_index[axis] = (b.shape(axis) > 1) ? out_index[axis] : 0;
    }
    for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
      b_index[axis2] = a_index[axis1];
      val += a[a_index] * b[b_index];
    }
    out[out_index] = val;
  }
  return out;
}

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> matmul(const expression<Container1, T, Rank> &a,
                       const expression<Container2, T, 2> &b) {
  size_t axis1 = Rank - 1, axis2 = 0;
  detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
  shape_t<Rank> shape = a.shape();
  shape[axis1] = b.shape(1);
  size_t n = a.shape(axis1);
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence_for(out)) {
    index_t<Rank> a_index = out_index;
    index_t<2> b_index(out_index[Rank - 2], out_index[Rank - 1]);
    T val = T(0);
    for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
      b_index[axis2] = a_index[axis1];
      val += a[a_index] * b[b_index];
    }
    out[out_index] = val;
  }
  return out;
}

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> matmul(const expression<Container1, T, 2> &a,
                       const expression<Container2, T, Rank> &b) {
  size_t axis1 = 1, axis2 = Rank - 2;
  detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
  shape_t<Rank> shape = b.shape();
  shape[axis2] = a.shape(0);
  size_t n = a.shape(axis1);
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence_for(out)) {
    index_t<2> a_index(out_index[Rank - 2], out_index[Rank - 1]);
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

template <class Container1, class T, size_t Rank1, class Container2,
          size_t Rank2, size_t N>
tensor<T, (Rank1 - N) + (Rank2 - N)>
tensordot(const expression<Container1, T, Rank1> &a,
          const expression<Container2, T, Rank2> &b, const shape_t<N> &a_axes,
          const shape_t<N> &b_axes) {
  static_assert(N < Rank1 && N < Rank2,
                "The number of dimensions to contract cannot be larger than "
                "the tensor dimension");
  size_t size =
      detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
  constexpr size_t Rank = (Rank1 - N) + (Rank2 - N);
  shape_t<Rank1 - N> a_squeezed = detail::remove_axes(a.shape(), a_axes);
  shape_t<Rank2 - N> b_squeezed = detail::remove_axes(b.shape(), b_axes);
  shape_t<Rank> shape = shape_cat(a_squeezed, b_squeezed);
  tensor<T, Rank> out(shape);
  for (index_t<Rank1 - N> i : make_index_sequence(a_squeezed)) {
    for (index_t<Rank2 - N> j : make_index_sequence(b_squeezed)) {
      typedef axes_iterator<const Container1, T, Rank1, N, void, T> iterator1;
      typedef axes_iterator<const Container2, T, Rank2, N, void, T> iterator2;
      index_t<Rank1> a_index = detail::insert_axes(i, a_axes);
      index_t<Rank2> b_index = detail::insert_axes(j, b_axes);
      index_t<Rank> out_index = shape_cat(i, j);
      iterator1 first1(&a.self(), a_index, a_axes, 0);
      iterator1 last1(&a.self(), a_index, a_axes, size);
      iterator2 first2(&b.self(), b_index, b_axes, 0);
      out[out_index] = std::inner_product(first1, last1, first2, T());
    }
  }
  return out;
}

template <class Container1, class T, size_t Rank1, class Container2,
          size_t Rank2>
tensor<T, Rank1 - Rank2> tensordot(const expression<Container1, T, Rank1> &a,
                                   const expression<Container2, T, Rank2> &b,
                                   const shape_t<Rank2> &a_axes,
                                   const shape_t<Rank2> &b_axes) {
  static_assert(Rank2 < Rank1, "The number of dimensions to contract cannot "
                               "be larger than the tensor dimension");
  size_t size =
      detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
  constexpr size_t Rank = Rank1 - Rank2;
  shape_t<Rank> shape = detail::remove_axes(a.shape(), a_axes);
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence_for(out)) {
    typedef axes_iterator<const Container1, T, Rank1, Rank2, void, T> iterator1;
    typedef axes_iterator<const Container2, T, Rank2, Rank2, void, T> iterator2;
    index_t<Rank1> a_index = detail::insert_axes(out_index, a_axes);
    index_t<Rank2> b_index;
    iterator1 first1(&a.self(), a_index, a_axes, 0);
    iterator1 last1(&a.self(), a_index, a_axes, size);
    iterator2 first2(&b.self(), b_index, b_axes, 0);
    out[out_index] = std::inner_product(first1, last1, first2, T());
  }
  return out;
}

template <class Container1, class T, size_t Rank1, class Container2,
          size_t Rank2>
tensor<T, Rank2 - Rank1> tensordot(const expression<Container1, T, Rank1> &a,
                                   const expression<Container2, T, Rank2> &b,
                                   const shape_t<Rank1> &a_axes,
                                   const shape_t<Rank1> &b_axes) {
  return tensordot(b, a, b_axes, a_axes);
}

template <class Container1, class T, size_t Rank, class Container2>
T tensordot(const expression<Container1, T, Rank> &a,
            const expression<Container2, T, Rank> &b,
            const shape_t<Rank> &a_axes, const shape_t<Rank> &b_axes) {
  size_t size =
      detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
  typedef axes_iterator<const Container1, T, Rank, Rank, void, T> iterator1;
  typedef axes_iterator<const Container2, T, Rank, Rank, void, T> iterator2;
  index_t<Rank> a_index, b_index;
  iterator1 first1(&a.self(), a_index, a_axes, 0);
  iterator1 last1(&a.self(), a_index, a_axes, size);
  iterator2 first2(&b.self(), b_index, b_axes, 0);
  return std::inner_product(first1, last1, first2, T());
}

namespace detail {
/**
 * @brief Zero norm.
 */
template <class InputIterator, class T>
T zero_norm(InputIterator first, InputIterator last, T) {
  return std::distance(first, last) - std::count(first, last, T());
}

/**
 * @brief Positive infinity norm.
 */
template <class InputIterator, class T>
T posinf_norm(InputIterator first, InputIterator last, T init) {
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
 * @brief Negative infinity norm.
 */
template <class InputIterator, class T>
T neginf_norm(InputIterator first, InputIterator last, T init) {
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
 * @brief p-norm.
 */
template <class InputIterator, class T>
T p_norm(InputIterator first, InputIterator last, T init, double ord) {
  T max_abs = detail::posinf_norm(first, last, T());
  if (max_abs > T()) {
    while (first != last) {
      init = std::move(init) + std::pow(std::abs(*first) / max_abs, ord);
      ++first;
    }
    init = max_abs * std::pow(init, 1. / ord);
  }
  return init;
}
} // namespace detail

namespace linalg {
template <class Container, class T>
T norm(const expression<Container, T, 1> &a, double ord) {
  if (ord == 0) {
    return detail::zero_norm(a.self().begin(), a.self().end(), T());
  } else if (ord == constants<double>::inf) {
    return detail::posinf_norm(a.self().begin(), a.self().end(), T());
  } else if (ord == -constants<double>::inf) {
    return detail::neginf_norm(a.self().begin(), a.self().end(), T());
  } else {
    return detail::p_norm(a.self().begin(), a.self().end(), T());
  }
}

template <class Container, class T>
T norm(const expression<Container, std::complex<T>, 1> &a, double ord) {
  if (ord == 0) {
    return detail::zero_norm(a.self().begin(), a.self().end(), T());
  } else if (ord == constants<double>::inf) {
    return detail::posinf_norm(a.self().begin(), a.self().end(), T());
  } else if (ord == -constants<double>::inf) {
    return detail::neginf_norm(a.self().begin(), a.self().end(), T());
  } else {
    return detail::p_norm(a.self().begin(), a.self().end(), T());
  }
}
} // namespace linalg

template <class Container, class T>
T trace(const expression<Container, T, 2> &a, ptrdiff_t k) {
  T val = T(0);
  if (k >= 0) {
    for (size_t i = 0; i < a.shape(0) && i + k < a.shape(1); ++i) {
      val += a[{i, i + k}];
    }
  } else {
    for (size_t i = 0; i - k < a.shape(0) && i < a.shape(1); ++i) {
      val += a[{i - k, i}];
    }
  }
  return val;
}
} // namespace numcpp

#endif // NUMCPP_LINALG_TCC_INCLUDED
