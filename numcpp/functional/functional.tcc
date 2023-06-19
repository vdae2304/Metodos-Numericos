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

/** @file include/numcpp/functional/functional.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/functional.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_FUNCTIONAL_TCC_INCLUDED
#define NUMCPP_FUNCTIONAL_TCC_INCLUDED

#include "numcpp/broadcasting/assert.h"
#include "numcpp/iterators/index_sequence.h"
#include "numcpp/iterators/nested_index_sequence.h"

namespace numcpp {
/// Functional programming.

template <class OutContainer, class R, class Function, class Container, class T,
          size_t Rank>
void apply(dense_tensor<OutContainer, R, Rank> &out, Function &&f,
           const expression<Container, T, Rank> &a) {
  detail::assert_output_shape(out.shape(), a.shape());
  for (index_t<Rank> i : make_index_sequence_for(out)) {
    out[i] = std::forward<Function>(f)(a[i]);
  }
}

template <class OutContainer, class R, class Function, class Container1,
          class T, class Container2, class U, size_t Rank>
void apply2(dense_tensor<OutContainer, R, Rank> &out, Function &&f,
            const expression<Container1, T, Rank> &a,
            const expression<Container2, U, Rank> &b) {
  detail::assert_output_shape(out.shape(),
                              broadcast_shapes(a.shape(), b.shape()));
  for (index_t<Rank> index : make_index_sequence_for(out)) {
    index_t<Rank> i, j;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (a.shape(axis) > 1) ? index[axis] : 0;
      j[axis] = (b.shape(axis) > 1) ? index[axis] : 0;
    }
    out[index] = std::forward<Function>(f)(a[i], b[j]);
  }
}

template <class OutContainer, class R, class Function, class Container, class T,
          class U, size_t Rank, detail::RequiresScalar<U>>
void apply2(dense_tensor<OutContainer, R, Rank> &out, Function &&f,
            const expression<Container, T, Rank> &a, const U &val) {
  detail::assert_output_shape(out.shape(), a.shape());
  for (index_t<Rank> i : make_index_sequence_for(out)) {
    out[i] = std::forward<Function>(f)(a[i], val);
  }
}

template <class OutContainer, class R, class Function, class T, class Container,
          class U, size_t Rank, detail::RequiresScalar<T>>
void apply2(dense_tensor<OutContainer, R, Rank> &out, Function &&f,
            const T &val, const expression<Container, U, Rank> &b) {
  detail::assert_output_shape(out.shape(), b.shape());
  for (index_t<Rank> i : make_index_sequence_for(out)) {
    out[i] = std::forward<Function>(f)(val, b[i]);
  }
}

template <class Function, class Container, class T, size_t Rank>
T reduce(Function &&f, const expression<Container, T, Rank> &a) {
  if (a.size() == 0) {
    throw std::invalid_argument(
        "attempt to reduce on an empty sequence with no initial value");
  }
  T val;
  bool is_null = true;
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    val = is_null ? a[i] : std::forward<Function>(f)(std::move(val), a[i]);
    is_null = false;
  }
  return val;
}

template <class Function, class Container, class T, size_t Rank>
T reduce(Function &&f, const expression<Container, T, Rank> &a,
         typename Container::value_type init) {
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    init = std::forward<Function>(f)(std::move(init), a[i]);
  }
  return init;
}

template <class Function, class Container1, class T, size_t Rank,
          class Container2>
T reduce(Function &&f, const expression<Container1, T, Rank> &a,
         typename Container1::value_type init,
         const expression<Container2, bool, Rank> &where) {
  detail::assert_mask_shape(a.shape(), where.shape());
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    if (where[i]) {
      init = std::forward<Function>(f)(std::move(init), a[i]);
    }
  }
  return init;
}

template <class Function, class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f,
                           const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes) {
  return reduce(std::forward<Function>(f), a, axes, dropdims);
}

template <class Function, class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N>
reduce(Function &&f, const expression<Container, T, Rank> &a,
       const shape_t<N> &axes, typename Container::value_type init) {
  return reduce(std::forward<Function>(f), a, axes, dropdims, init);
}

template <class Function, class Container1, class T, size_t Rank, size_t N,
          class Container2>
tensor<T, Rank - N>
reduce(Function &&f, const expression<Container1, T, Rank> &a,
       const shape_t<N> &axes, typename Container1::value_type init,
       const expression<Container2, bool, Rank> &where) {
  return reduce(std::forward<Function>(f), a, axes, dropdims, init, where);
}

template <class Function, class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const expression<Container, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t) {
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < N; ++i) {
    shape[axes[i]] = 1;
  }
  tensor<T, Rank> out(shape);
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val;
    bool is_null = true;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axes)) {
      val = is_null ? a[i] : std::forward<Function>(f)(std::move(val), a[i]);
      is_null = false;
    }
    out[index] = val;
  }
  return out;
}

template <class Function, class Container, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const expression<Container, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t,
                       typename Container::value_type init) {
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < N; ++i) {
    shape[axes[i]] = 1;
  }
  tensor<T, Rank> out(shape);
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val = init;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axes)) {
      val = std::forward<Function>(f)(std::move(val), a[i]);
    }
    out[index] = val;
  }
  return out;
}

template <class Function, class Container1, class T, size_t Rank, size_t N,
          class Container2>
tensor<T, Rank> reduce(Function &&f, const expression<Container1, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t,
                       typename Container1::value_type init,
                       const expression<Container2, bool, Rank> &where) {
  detail::assert_mask_shape(a.shape(), where.shape());
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < N; ++i) {
    shape[axes[i]] = 1;
  }
  tensor<T, Rank> out(shape);
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val = init;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axes)) {
      if (where[i]) {
        val = std::forward<Function>(f)(std::move(val), a[i]);
      }
    }
    out[index] = val;
  }
  return out;
}

template <class Function, class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f,
                           const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t) {
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < N; ++i) {
    shape[axes[i]] = 1;
  }
  tensor<T, Rank - N> out(detail::remove_axes(a.shape(), axes));
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val;
    bool is_null = true;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axes)) {
      val = is_null ? a[i] : std::forward<Function>(f)(std::move(val), a[i]);
      is_null = false;
    }
    out[detail::remove_axes(index, axes)] = val;
  }
  return out;
}

template <class Function, class Container, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f,
                           const expression<Container, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t,
                           typename Container::value_type init) {
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < N; ++i) {
    shape[axes[i]] = 1;
  }
  tensor<T, Rank - N> out(detail::remove_axes(a.shape(), axes));
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val = init;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axes)) {
      val = std::forward<Function>(f)(std::move(val), a[i]);
    }
    out[detail::remove_axes(index, axes)] = val;
  }
  return out;
}

template <class Function, class Container1, class T, size_t Rank, size_t N,
          class Container2>
tensor<T, Rank - N>
reduce(Function &&f, const expression<Container1, T, Rank> &a,
       const shape_t<N> &axes, dropdims_t, typename Container1::value_type init,
       const expression<Container2, bool, Rank> &where) {
  detail::assert_mask_shape(a.shape(), where.shape());
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < N; ++i) {
    shape[axes[i]] = 1;
  }
  tensor<T, Rank - N> out(detail::remove_axes(a.shape(), axes));
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val = init;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axes)) {
      if (where[i]) {
        val = std::forward<Function>(f)(std::move(val), a[i]);
      }
    }
    out[detail::remove_axes(index, axes)] = val;
  }
  return out;
}

template <class Function, class Container, class T, size_t Rank>
tensor<T, Rank>
accumulate(Function &&f, const expression<Container, T, Rank> &a, size_t axis) {
  shape_t<Rank> shape = a.shape();
  tensor<T, Rank> out(shape);
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    T val;
    bool is_null = true;
    for (index_t<Rank> i : make_nested_index_sequence_for(a, index, axis)) {
      val = is_null ? a[i] : std::forward<Function>(f)(std::move(val), a[i]);
      is_null = false;
      out[i] = val;
    }
  }
  return out;
}

template <class OutContainer, class R, class Function, class Container1,
          class T, size_t Rank1, class Container2, class U, size_t Rank2>
void outer(dense_tensor<OutContainer, R, Rank1 + Rank2> &out, Function &&f,
           const expression<Container1, T, Rank1> &a,
           const expression<Container2, U, Rank2> &b) {
  detail::assert_output_shape(out.shape(), shape_cat(a.shape(), b.shape()));
  for (index_t<Rank1> i : make_index_sequence_for(a)) {
    for (index_t<Rank2> j : make_index_sequence_for(b)) {
      out[shape_cat(i, j)] = std::forward<Function>(f)(a[i], b[j]);
    }
  }
}
} // namespace numcpp

#endif // NUMCPP_FUNCTIONAL_TCC_INCLUDED
