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

#include <vector>
#include "numcpp/iterators/axes_iterator.h"
#include "numcpp/iterators/index_sequence.h"

namespace numcpp {
/// Numerical ranges.

template <class T> inline sequence_expr<T> arange(const T &stop) {
  size_t size = 0;
  if (stop > 0) {
    size = std::ceil(stop);
  }
  return sequence_expr<T>(T(), size, T(1));
}

template <class T, class U>
inline sequence_expr<typename std::common_type<T, U>::type>
arange(const T &start, const U &stop) {
  typedef typename std::common_type<T, U>::type Rt;
  size_t size = 0;
  if (start < stop) {
    size = std::ceil(stop - start);
  }
  return sequence_expr<Rt>(start, size, Rt(1));
}

template <class T, class U, class V>
inline sequence_expr<typename std::common_type<T, U, V>::type>
arange(const T &start, const U &stop, const V &step) {
  typedef typename std::common_type<T, U, V>::type Rt;
  size_t size = 0;
  if ((start < stop && step > 0) || (start > stop && step < 0)) {
    size = std::ceil((double)(stop - start) / step);
  }
  return sequence_expr<Rt>(start, size, step);
}

template <class T, class U>
inline sequence_expr<typename std::common_type<T, U>::type>
linspace(const T &start, const U &stop, size_t num, bool endpoint) {
  typedef typename std::common_type<T, U>::type Rt;
  Rt step = stop - start;
  step /= num - endpoint;
  return sequence_expr<Rt>(start, num, step);
}

template <class T, class U>
inline sequence_expr<typename std::common_type<T, U>::type>
logspace(const T &start, const U &stop, size_t num, bool endpoint) {
  typedef typename std::common_type<T, U>::type Rt;
  Rt step = stop - start;
  step /= num - endpoint;
  return sequence_expr<Rt>(start, num, step, Rt(10));
}

template <class T, class U, class V>
inline sequence_expr<typename std::common_type<T, U, V>::type>
logspace(const T &start, const U &stop, size_t num, bool endpoint,
         const V &base) {
  typedef typename std::common_type<T, U, V>::type Rt;
  Rt step = stop - start;
  step /= num - endpoint;
  return sequence_expr<Rt>(start, num, step, base);
}

template <class T, class U>
inline sequence_expr<typename std::common_type<T, U>::type>
geomspace(const T &start, const U &stop, size_t num, bool endpoint) {
  typedef typename std::common_type<T, U>::type Rt;
  Rt logstart = std::log10(Rt(start));
  Rt logstop = std::log10(Rt(stop));
  Rt logstep = logstop - logstart;
  logstep /= num - endpoint;
  return sequence_expr<Rt>(logstart, num, logstep, Rt(10));
}

/// Building matrices.

template <class Container, class T>
tensor<T, 2> vander(const expression<Container, T, 1> &x) {
  return vander(x, x.size());
}

template <class Container, class T>
tensor<T, 2> vander(const expression<Container, T, 1> &x, size_t N,
                    bool increasing) {
  tensor<T, 2> out(x.size(), N);
  for (size_t i = 0; i < x.size(); ++i) {
    T pow = T(1);
    for (size_t k = 0; k < N; ++k) {
      size_t j = increasing ? k : N - 1 - k;
      out[{i, j}] = pow;
      pow *= x[i];
    }
  }
  return out;
}

/// Maximums and minimums.

// Namespace for implementation details.
namespace detail {
/**
 * @brief Apply a function over multiple axes.
 */
template <class R, class Function, class Container, class T, size_t Rank,
          size_t N>
tensor<R, Rank> apply_over_axes(Function f,
                                const expression<Container, T, Rank> &a,
                                const shape_t<N> &axes, keepdims_t) {
  shape_t<Rank> shape = a.shape();
  size_t size = 1;
  for (size_t i = 0; i < N; ++i) {
    size *= shape[axes[i]];
    shape[axes[i]] = 1;
  }
  tensor<R, Rank> out(shape);
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<const Container, T, Rank, N, void, T> iterator;
    iterator first(&a.self(), index, axes, 0);
    iterator last(&a.self(), index, axes, size);
    out[index] = f(first, last);
  }
  return out;
}

template <class R, class Function, class Container, class T, size_t Rank,
          size_t N>
tensor<R, Rank - N> apply_over_axes(Function f,
                                    const expression<Container, T, Rank> &a,
                                    const shape_t<N> &axes, dropdims_t) {
  shape_t<Rank> shape = a.shape();
  size_t size = 1;
  for (size_t i = 0; i < N; ++i) {
    size *= shape[axes[i]];
    shape[axes[i]] = 1;
  }
  tensor<R, Rank - N> out(detail::remove_axes(a.shape(), axes));
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<const Container, T, Rank, N, void, T> iterator;
    iterator first(&a.self(), index, axes, 0);
    iterator last(&a.self(), index, axes, size);
    out[detail::remove_axes(index, axes)] = f(first, last);
  }
  return out;
}
} // namespace detail

template <class Container, class T, size_t Rank>
index_t<Rank> argmax(const expression<Container, T, Rank> &a) {
  ranges::argmax pred;
  size_t index = pred(a.self().begin(), a.self().end());
  return unravel_index(index, a.shape(), a.layout());
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmax(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis) {
  return argmax(a, axis, dropdims);
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argmax(const expression<Container, T, Rank> &a,
                            const shape_t<1> &axis, keepdims_t) {
  return detail::apply_over_axes<size_t>(ranges::argmax(), a, axis, keepdims);
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmax(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis, dropdims_t) {
  return detail::apply_over_axes<size_t>(ranges::argmax(), a, axis, dropdims);
}

template <class Container, class T, size_t Rank>
index_t<Rank> argmin(const expression<Container, T, Rank> &a) {
  ranges::argmin pred;
  size_t index = pred(a.self().begin(), a.self().end());
  return unravel_index(index, a.shape(), a.layout());
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmin(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis) {
  return argmin(a, axis, dropdims);
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argmin(const expression<Container, T, Rank> &a,
                            const shape_t<1> &axis, keepdims_t) {
  return detail::apply_over_axes<size_t>(ranges::argmin(), a, axis, keepdims);
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank - 1> argmin(const expression<Container, T, Rank> &a,
                                const shape_t<1> &axis, dropdims_t) {
  return detail::apply_over_axes<size_t>(ranges::argmin(), a, axis, dropdims);
}

template <class Container, class T, size_t Rank>
T amax(const expression<Container, T, Rank> &a) {
  ranges::max pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amax(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes) {
  return amax(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> amax(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::max(), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amax(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::max(), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
T amin(const expression<Container, T, Rank> &a) {
  ranges::min pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amin(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes) {
  return amin(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> amin(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::min(), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> amin(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::min(), a, axes, dropdims);
}

/// Sums and products.

template <class Container, class T, size_t Rank>
T sum(const expression<Container, T, Rank> &a) {
  ranges::sum pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> sum(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes) {
  return sum(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> sum(const expression<Container, T, Rank> &a,
                    const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::sum(), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> sum(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::sum(), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
T prod(const expression<Container, T, Rank> &a) {
  ranges::prod pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> prod(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes) {
  return prod(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> prod(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::prod(), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> prod(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::prod(), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
tensor<T, Rank> cumsum(const expression<Container, T, Rank> &a, size_t axis) {
  shape_t<Rank> shape = a.shape();
  tensor<T, Rank> out(shape);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<const Container, T, Rank, 1, void, T> input_iterator;
    typedef axes_iterator<tensor<T, Rank>, T, Rank, 1> output_iterator;
    input_iterator first(&a.self(), index, axis, 0);
    input_iterator last(&a.self(), index, axis, size);
    output_iterator result(&out, index, axis, 0);
    std::partial_sum(first, last, result, plus());
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<T, Rank> cumprod(const expression<Container, T, Rank> &a, size_t axis) {
  shape_t<Rank> shape = a.shape();
  tensor<T, Rank> out(shape);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<const Container, T, Rank, 1, void, T> input_iterator;
    typedef axes_iterator<tensor<T, Rank>, T, Rank, 1> output_iterator;
    input_iterator first(&a.self(), index, axis, 0);
    input_iterator last(&a.self(), index, axis, size);
    output_iterator result(&out, index, axis, 0);
    std::partial_sum(first, last, result, multiplies());
  }
  return out;
}

/// Logic functions.

template <class Container, size_t Rank>
bool all(const expression<Container, bool, Rank> &a) {
  ranges::all pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> all(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes) {
  return all(a, axes, dropdims);
}

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank> all(const expression<Container, bool, Rank> &a,
                       const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<bool>(ranges::all(), a, axes, keepdims);
}

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> all(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<bool>(ranges::all(), a, axes, dropdims);
}

template <class Container, size_t Rank>
bool any(const expression<Container, bool, Rank> &a) {
  ranges::any pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> any(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes) {
  return any(a, axes, dropdims);
}

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank> any(const expression<Container, bool, Rank> &a,
                       const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<bool>(ranges::any(), a, axes, keepdims);
}

template <class Container, size_t Rank, size_t N>
tensor<bool, Rank - N> any(const expression<Container, bool, Rank> &a,
                           const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<bool>(ranges::any(), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
size_t count_nonzero(const expression<Container, T, Rank> &a) {
  ranges::count_nonzero pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<size_t, Rank - N> count_nonzero(const expression<Container, T, Rank> &a,
                                       const shape_t<N> &axes) {
  return count_nonzero(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<size_t, Rank> count_nonzero(const expression<Container, T, Rank> &a,
                                   const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<size_t>(ranges::count_nonzero(), a, axes,
                                         keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<size_t, Rank - N> count_nonzero(const expression<Container, T, Rank> &a,
                                       const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<size_t>(ranges::count_nonzero(), a, axes,
                                         dropdims);
}

template <class T, class U>
bool isclose(T a, U b, typename detail::promote<T, U>::type rtol,
             typename detail::promote<T, U>::type atol) {
  typedef typename detail::promote<T, U>::type Rt;
  ranges::isclose<Rt> pred(rtol, atol);
  return pred(a, b);
}

template <class T, class U>
bool isclose(const std::complex<T> &a, const std::complex<U> &b,
             typename detail::promote<T, U>::type rtol,
             typename detail::promote<T, U>::type atol) {
  typedef typename detail::promote<T, U>::type Rt;
  ranges::isclose<std::complex<Rt>> pred(rtol, atol);
  return pred(a, b);
}

template <class T, class U>
bool isclose(const std::complex<T> &a, const U &b,
             typename detail::promote<T, U>::type rtol,
             typename detail::promote<T, U>::type atol) {
  typedef typename detail::promote<T, U>::type Rt;
  ranges::isclose<std::complex<Rt>> pred(rtol, atol);
  return pred(a, b);
}

template <class T, class U>
bool isclose(const T &a, const std::complex<U> &b,
             typename detail::promote<T, U>::type rtol,
             typename detail::promote<T, U>::type atol) {
  typedef typename detail::promote<T, U>::type Rt;
  ranges::isclose<std::complex<Rt>> pred(rtol, atol);
  return pred(a, b);
}

/// Sorting and searching.

template <class Container, class T, size_t Rank>
tensor<index_t<Rank>, 1> argsort(const expression<Container, T, Rank> &a) {
  return argsort(a, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<index_t<Rank>, 1> argsort(const expression<Container, T, Rank> &a,
                                 Compare comp, bool stable) {
  index_sequence<Rank> indices(a.shape(), a.layout());
  tensor<index_t<Rank>, 1> out(indices.begin(), a.size());
  if (stable) {
    std::stable_sort(out.data(), out.data() + out.size(),
                     [&](const index_t<Rank> &i, const index_t<Rank> &j) {
                       return comp(a[i], a[j]);
                     });
  } else {
    std::sort(out.data(), out.data() + out.size(),
              [&](const index_t<Rank> &i, const index_t<Rank> &j) {
                return comp(a[i], a[j]);
              });
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argsort(const expression<Container, T, Rank> &a,
                             size_t axis) {
  return argsort(a, axis, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<size_t, Rank> argsort(const expression<Container, T, Rank> &a,
                             size_t axis, Compare comp, bool stable) {
  shape_t<Rank> shape = a.shape();
  tensor<size_t, Rank> out(shape);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<tensor<size_t, Rank>, size_t, Rank, 1> iterator;
    iterator first(&out, index, axis, 0);
    iterator last(&out, index, axis, size);
    std::iota(first, last, size_t(0));
    index_t<Rank> i = index, j = index;
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

template <class Container, class T, size_t Rank>
tensor<T, 1> sort(const expression<Container, T, Rank> &a) {
  return sort(a, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<T, 1> sort(const expression<Container, T, Rank> &a, Compare comp,
                  bool stable) {
  tensor<T, 1> out(a.self().begin(), a.size());
  if (stable) {
    std::stable_sort(out.data(), out.data() + out.size(), comp);
  } else {
    std::sort(out.data(), out.data() + out.size(), comp);
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<T, Rank> sort(const expression<Container, T, Rank> &a, size_t axis) {
  return sort(a, axis, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<T, Rank> sort(const expression<Container, T, Rank> &a, size_t axis,
                     Compare comp, bool stable) {
  shape_t<Rank> shape = a.shape();
  tensor<T, Rank> out(a);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<tensor<T, Rank>, T, Rank, 1> iterator;
    iterator first(&out, index, axis, 0);
    iterator last(&out, index, axis, size);
    if (stable) {
      std::stable_sort(first, last, comp);
    } else {
      std::sort(first, last, comp);
    }
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<index_t<Rank>, 1> argpartition(const expression<Container, T, Rank> &a,
                                      size_t kth) {
  return argpartition(a, kth, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<index_t<Rank>, 1> argpartition(const expression<Container, T, Rank> &a,
                                      size_t kth, Compare comp) {
  index_sequence<Rank> indices(a.shape(), a.layout());
  tensor<index_t<Rank>, 1> out(indices.begin(), a.size());
  std::nth_element(out.data(), out.data() + kth, out.data() + out.size(),
                   [&](const index_t<Rank> &i, const index_t<Rank> &j) {
                     return comp(a[i], a[j]);
                   });
  return out;
}

template <class Container, class T, size_t Rank>
tensor<size_t, Rank> argpartition(const expression<Container, T, Rank> &a,
                                  size_t kth, size_t axis) {
  return argpartition(a, kth, axis, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<size_t, Rank> argpartition(const expression<Container, T, Rank> &a,
                                  size_t kth, size_t axis, Compare comp) {
  shape_t<Rank> shape = a.shape();
  tensor<size_t, Rank> out(shape);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<tensor<size_t, Rank>, size_t, Rank, 1> iterator;
    iterator first(&out, index, axis, 0);
    iterator last(&out, index, axis, size);
    std::iota(first, last, size_t(0));
    index_t<Rank> i = index, j = index;
    std::nth_element(first, first + kth, last,
                     [&](size_t i_axis, size_t j_axis) {
                       i[axis] = i_axis;
                       j[axis] = j_axis;
                       return comp(a[i], a[j]);
                     });
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<T, 1> partition(const expression<Container, T, Rank> &a, size_t kth) {
  return partition(a, kth, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<T, 1> partition(const expression<Container, T, Rank> &a, size_t kth,
                       Compare comp) {
  tensor<T, 1> out(a.self().begin(), a.size());
  std::nth_element(out.data(), out.data() + kth, out.data() + out.size(), comp);
  return out;
}

template <class Container, class T, size_t Rank>
tensor<T, Rank> partition(const expression<Container, T, Rank> &a, size_t kth,
                          size_t axis) {
  return partition(a, kth, axis, less());
}

template <class Container, class T, size_t Rank, class Compare,
          detail::RequiresCallable<Compare, T, T>>
tensor<T, Rank> partition(const expression<Container, T, Rank> &a, size_t kth,
                          size_t axis, Compare comp) {
  shape_t<Rank> shape = a.shape();
  tensor<T, Rank> out(a);
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<tensor<T, Rank>, T, Rank, 1> iterator;
    iterator first(&out, index, axis, 0);
    iterator last(&out, index, axis, size);
    std::nth_element(first, first + kth, last, comp);
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<index_t<Rank>, 1> nonzero(const expression<Container, T, Rank> &a) {
  size_t size = count_nonzero(a);
  tensor<index_t<Rank>, 1> out(size);
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    if (a[i] != T()) {
      out[n++] = i;
    }
  }
  return out;
}

template <class Container, size_t Rank>
tensor<index_t<Rank>, 1>
where(const expression<Container, bool, Rank> &condition) {
  return nonzero(condition);
}

/// Set routines.

template <class Container, class T, size_t Rank>
tensor<T, 1> unique(const expression<Container, T, Rank> &a) {
  std::vector<T> buffer(a.self().begin(), a.self().end());
  if (!std::is_sorted(buffer.begin(), buffer.end())) {
    std::sort(buffer.begin(), buffer.end());
  }
  size_t size = std::unique(buffer.begin(), buffer.end()) - buffer.begin();
  return tensor<T, 1>(buffer.begin(), size);
}

template <class Container, class T>
bool includes(const expression<Container, T, 1> &a,
              const typename Container::value_type &val) {
  return std::binary_search(a.self().begin(), a.self().end(), val);
}

template <class Container1, class T, class Container2>
bool includes(const expression<Container1, T, 1> &a,
              const expression<Container2, T, 1> &b) {
  return std::includes(a.self().begin(), a.self().end(), b.self().begin(),
                       b.self().end());
}

template <class Container1, class T, class Container2>
tensor<T, 1> set_union(const expression<Container1, T, 1> &a,
                       const expression<Container2, T, 1> &b) {
  std::vector<T> buffer;
  std::set_union(a.self().begin(), a.self().end(), b.self().begin(),
                 b.self().end(), std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

template <class Container1, class T, class Container2>
tensor<T, 1> set_intersection(const expression<Container1, T, 1> &a,
                              const expression<Container2, T, 1> &b) {
  std::vector<T> buffer;
  std::set_intersection(a.self().begin(), a.self().end(), b.self().begin(),
                        b.self().end(), std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

template <class Container1, class T, class Container2>
tensor<T, 1> set_difference(const expression<Container1, T, 1> &a,
                            const expression<Container2, T, 1> &b) {
  std::vector<T> buffer;
  std::set_difference(a.self().begin(), a.self().end(), b.self().begin(),
                      b.self().end(), std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

template <class Container1, class T, class Container2>
tensor<T, 1> set_symmetric_difference(const expression<Container1, T, 1> &a,
                                      const expression<Container2, T, 1> &b) {
  std::vector<T> buffer;
  std::set_symmetric_difference(a.self().begin(), a.self().end(),
                                b.self().begin(), b.self().end(),
                                std::back_inserter(buffer));
  return tensor<T, 1>(buffer.begin(), buffer.size());
}

/// Basic statistics.

template <class Container, class T, size_t Rank>
T mean(const expression<Container, T, Rank> &a) {
  ranges::mean pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> mean(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes) {
  return mean(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> mean(const expression<Container, T, Rank> &a,
                     const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::mean(), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> mean(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::mean(), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
T median(const expression<Container, T, Rank> &a) {
  ranges::median pred;
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> median(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes) {
  return median(a, axes, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> median(const expression<Container, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::median(), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> median(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::median(), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
T var(const expression<Container, T, Rank> &a, bool bias) {
  ranges::var pred(bias);
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> var(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes, bool bias) {
  return var(a, axes, bias, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> var(const expression<Container, T, Rank> &a,
                    const shape_t<N> &axes, bool bias, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::var(bias), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> var(const expression<Container, T, Rank> &a,
                        const shape_t<N> &axes, bool bias, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::var(bias), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
T stddev(const expression<Container, T, Rank> &a, bool bias) {
  ranges::stddev pred(bias);
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> stddev(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, bool bias) {
  return stddev(a, axes, bias, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> stddev(const expression<Container, T, Rank> &a,
                       const shape_t<N> &axes, bool bias, keepdims_t) {
  return detail::apply_over_axes<T>(ranges::stddev(bias), a, axes, keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> stddev(const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, bool bias, dropdims_t) {
  return detail::apply_over_axes<T>(ranges::stddev(bias), a, axes, dropdims);
}

template <class Container, class T, size_t Rank>
T quantile(const expression<Container, T, Rank> &a, double q,
           const std::string &method) {
  ranges::quantile pred(q, method);
  return pred(a.self().begin(), a.self().end());
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> quantile(const expression<Container, T, Rank> &a, double q,
                             const shape_t<N> &axes,
                             const std::string &method) {
  return quantile(a, q, axes, method, dropdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> quantile(const expression<Container, T, Rank> &a, double q,
                         const shape_t<N> &axes, const std::string &method,
                         keepdims_t) {
  return detail::apply_over_axes<T>(ranges::quantile(q, method), a, axes,
                                    keepdims);
}

template <class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> quantile(const expression<Container, T, Rank> &a, double q,
                             const shape_t<N> &axes, const std::string &method,
                             dropdims_t) {
  return detail::apply_over_axes<T>(ranges::quantile(q, method), a, axes,
                                    dropdims);
}

template <class Container1, class T, class Container2>
T cov(const expression<Container1, T, 1> &x,
      const expression<Container2, T, 1> &y, bool bias) {
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

template <class Container1, class T, class Container2>
T cov(const expression<Container1, std::complex<T>, 1> &x,
      const expression<Container2, std::complex<T>, 1> &y, bool bias) {
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

template <class Container, class T>
tensor<T, 2> cov(const expression<Container, T, 2> &a, bool rowvar, bool bias) {
  size_t nvar = rowvar ? a.shape(0) : a.shape(1);
  size_t size = rowvar ? a.shape(1) : a.shape(0);
  tensor<T, 2> out(nvar, nvar);
  tensor<T, 1> a_mean = mean(a, make_shape(rowvar));
  for (size_t i = 0; i < nvar; ++i) {
    for (size_t j = 0; j < nvar; ++j) {
      T val = T(0);
      for (size_t k = 0; k < size; ++k) {
        if (rowvar) {
          val += (a[{i, k}] - a_mean[i]) * (a[{j, k}] - a_mean[j]);
        } else {
          val += (a[{k, i}] - a_mean[i]) * (a[{k, j}] - a_mean[j]);
        }
      }
      val /= size - 1 + bias;
      out[{i, j}] = val;
    }
  }
  return out;
}

template <class Container, class T>
tensor<std::complex<T>, 2>
cov(const expression<Container, std::complex<T>, 2> &a, bool rowvar,
    bool bias) {
  size_t nvar = rowvar ? a.shape(0) : a.shape(1);
  size_t size = rowvar ? a.shape(1) : a.shape(0);
  tensor<std::complex<T>, 2> out(nvar, nvar);
  tensor<std::complex<T>, 1> a_mean = mean(a, make_shape(rowvar));
  for (size_t i = 0; i < nvar; ++i) {
    for (size_t j = 0; j < nvar; ++j) {
      std::complex<T> val = T(0);
      for (size_t k = 0; k < size; ++k) {
        if (rowvar) {
          val += (a[{i, k}] - a_mean[i]) * std::conj(a[{j, k}] - a_mean[j]);
        } else {
          val += (a[{k, i}] - a_mean[i]) * std::conj(a[{k, j}] - a_mean[j]);
        }
      }
      val /= size - 1 + bias;
      out[{i, j}] = val;
    }
  }
  return out;
}

template <class Container1, class T, class Container2>
T corrcoef(const expression<Container1, T, 1> &x,
           const expression<Container2, T, 1> &y) {
  return cov(x, y, true) / (stddev(x, true) * stddev(y, true));
}

template <class Container, class T>
tensor<T, 2> corrcoef(const expression<Container, T, 2> &a, bool rowvar) {
  tensor<T, 2> out = cov(a, rowvar);
  for (size_t i = 0; i < out.shape(0); ++i) {
    for (size_t j = i + 1; j < out.shape(1); ++j) {
      T denom = std::sqrt(out[{i, i}] * out[{j, j}]);
      out[{i, j}] /= denom;
      out[{j, i}] /= denom;
    }
    out[{i, i}] = T(1);
  }
  return out;
}
} // namespace numcpp

#endif // NUMCPP_ROUTINES_TCC_INCLUDED
