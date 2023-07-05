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

/** @file include/numcpp/broadcasting/padding.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_PADDING_H_INCLUDED
#define NUMCPP_PADDING_H_INCLUDED

#include <tuple>
#include <utility>
#include "numcpp/shape.h"

namespace numcpp {
namespace detail {
/**
 * @brief Function object implementing @c std::make_pair and @c std::make_tuple.
 */
struct zip {
  template <class T, class U>
  auto operator()(T &&a, U &&b) const
      -> decltype(std::make_pair(std::forward<T>(a), std::forward<U>(b))) {
    return std::make_pair(std::forward<T>(a), std::forward<U>(b));
  }

  template <class... T>
  auto operator()(T &&...args) const
      -> decltype(std::make_tuple(std::forward<T>(args)...)) {
    return std::make_tuple(std::forward<T>(args)...);
  }
};

/**
 * @brief Function object implementing @c std::get.
 */
template <size_t I> struct unzip_by_index {
  template <class Tuple>
  auto operator()(Tuple &&arg) const
      -> decltype(std::get<I>(std::forward<Tuple>(arg))) {
    return std::get<I>(std::forward<Tuple>(arg));
  }
};

#if __cplusplus >= 201402L
template <class T> struct unzip_by_type {
  template <class Tuple>
  auto operator()(Tuple &&arg) const
      -> decltype(std::get<T>(std::forward<Tuple>(arg))) {
    return std::get<T>(std::forward<Tuple>(arg));
  }
};
#endif // C++14

/**
 * @brief Function object implementing @c np::ravel_index.
 */
template <size_t Rank> struct ravel_index {
  // Shape used for raveling.
  shape_t<Rank> shape;

  // Whether the indices should be viewed as indexing in row-major or
  // column-major order
  layout_t order;

  /**
   * @brief Constructor.
   */
  ravel_index(const shape_t<Rank> &shape, layout_t order = default_layout)
      : shape(shape), order(order) {}

  /**
   * @brief Converts a tuple of indices into a flat index.
   *
   * @param index A tuple of indices to flatten.
   *
   * @return The flattened index.
   */
  size_t operator()(const index_t<Rank> &index) const {
    return numcpp::ravel_index(index, shape, order);
  }
};

/**
 * @brief Function object implementing @c np::unravel_index.
 */
template <size_t Rank> struct unravel_index {
  // Shape used for unraveling.
  shape_t<Rank> shape;

  // Whether the indices should be viewed as indexing in row-major or
  // column-major order
  layout_t order;

  /**
   * @brief Constructor.
   */
  unravel_index(const shape_t<Rank> &shape, layout_t order = default_layout)
      : shape(shape), order(order) {}

  /**
   * @brief Converts a flat index into a tuple of indices.
   *
   * @param index Index to unravel.
   *
   * @return The unraveled index.
   */
  index_t<Rank> operator()(size_t index) const {
    return numcpp::unravel_index(index, shape, order);
  }
};
} // namespace detail

/// Namespace for padding modes.
namespace pad_mode {
/**
 * @brief Pads with a constant value.
 *
 * @param args... The values to set the padded elements for each axis.
 */
struct constant {
  /**
   * @param args... If no arguments are passed, all the axes are padded with
   *                zeros.
   */
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis) const {
    this->operator()(view, before, after, axis, T(), T());
  }

  /**
   * @param args... If a single value is passed, the same constant is used for
   *                all the axes.
   */
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis,
                  const typename tensor_view<T, 1>::value_type &val) const {
    this->operator()(view, before, after, axis, val, val);
  }

  /**
   * @param args... If two values are passed, the same before and after
   *                constants are used for each axis.
   */
  template <class T>
  void
  operator()(tensor_view<T, 1> &view, size_t before, size_t after, size_t,
             const typename tensor_view<T, 1>::value_type &val_before,
             const typename tensor_view<T, 1>::value_type &val_after) const {
    for (size_t i = 0; i < before; ++i) {
      view[i] = val_before;
    }
    for (size_t i = 0; i < after; ++i) {
      view[view.size() - 1 - i] = val_after;
    }
  }

  /**
   * @param args... If a matrix of values is passed, unique pad constants are
   *                used for each axis, where @a values(i,0) and @a values(i,1)
   *                are the before and after constants for axis @a i.
   */
  template <class Container, class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis,
                  const expression<Container, T, 2> &values) const {
    this->operator()(view, before, after, axis, values(axis, 0),
                     values(axis, 1));
  }
};

/**
 * @brief Pads with the edge values of the tensor.
 */
struct edge : constant {
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis) const {
    T edge_before = view[before];
    T edge_after = view[view.size() - 1 - after];
    constant::operator()(view, before, after, axis, edge_before, edge_after);
  }
};

/**
 * @brief Pads with the linear ramp between an end value and the tensor edge
 * value.
 *
 * @param args.. The values used for the ending values of the linear ramp.
 */
struct linear_ramp {
  /**
   * @param args... If no arguments are passed, all the end values are set to
   *                zero.
   */
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis) const {
    this->operator()(view, before, after, axis, T(), T());
  }

  /**
   * @param args... If a single value is passed, the same value is used for all
   *                the axes.
   */
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis,
                  const typename tensor_view<T, 1>::value_type &end) const {
    this->operator()(view, before, after, axis, end, end);
  }

  /**
   * @param args... If two values are passed, the same before and after end
   *                values are used for each axis.
   */
  template <class T>
  void
  operator()(tensor_view<T, 1> &view, size_t before, size_t after, size_t,
             const typename tensor_view<T, 1>::value_type &end_before,
             const typename tensor_view<T, 1>::value_type &end_after) const {
    T edge_before = view[before];
    T edge_after = view[view.size() - 1 - after];
    for (size_t i = 0; i < before; ++i) {
      double t = (double)i / (double)before;
      view[i] = (1 - t) * end_before + t * edge_before;
    }
    for (size_t i = 0; i < after; ++i) {
      double t = (double)i / (double)after;
      view[view.size() - 1 - i] = (1 - t) * end_after + t * edge_after;
    }
  }

  /**
   * @param args... If a matrix of values is passed, unique end values are used
   *                for each axis, where @a values(i,0) and @a values(i,1) are
   *                the before and after end values for axis @a i.
   */
  template <class Container, class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t axis,
                  const expression<Container, T, 2> &end_values) const {
    this->operator()(view, before, after, axis, end_values(axis, 0),
                     end_values(axis, 1));
  }
};

/**
 * @brief Pads with the reflection of the tensor mirrored on the first and last
 * values.
 */
struct reflect {
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t) const {
    size_t size = view.size() - before - after;
    for (size_t i = 0; i < before; ++i) {
      size_t idx = 1 + (before - 1 - i) % (size - 1);
      size_t nblock = (before - 1 - i) / (size - 1);
      view[i] = (nblock % 2 == 0) ? view[before + idx]
                                  : view[before + size - 1 - idx];
    }
    for (size_t i = 0; i < after; ++i) {
      size_t idx = 1 + (after - 1 - i) % (size - 1);
      size_t nblock = (after - 1 - i) / (size - 1);
      view[view.size() - 1 - i] = (nblock % 2 == 0)
                                      ? view[before + size - 1 - idx]
                                      : view[before + idx];
    }
  }
};

/**
 * @brief Pads with the reflection of the tensor mirrored along the edge.
 */
struct symmetric {
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t) const {
    size_t size = view.size() - before - after;
    for (size_t i = 0; i < before; ++i) {
      size_t idx = (before - 1 - i) % size;
      size_t nblock = (before - 1 - i) / size;
      view[i] = (nblock % 2 == 0) ? view[before + idx]
                                  : view[before + size - 1 - idx];
    }
    for (size_t i = 0; i < after; ++i) {
      size_t idx = (after - 1 - i) % size;
      size_t nblock = (after - 1 - i) / size;
      view[view.size() - 1 - i] = (nblock % 2 == 0)
                                      ? view[before + size - 1 - idx]
                                      : view[before + idx];
    }
  }
};

/**
 * @brief Pads with the wrap of the tensor along the axis. The first values are
 * used to pad the end and the end values are used to pad the beginning.
 */
struct wrap {
  template <class T>
  void operator()(tensor_view<T, 1> &view, size_t before, size_t after,
                  size_t) const {
    size_t size = view.size() - before - after;
    for (size_t i = 0; i < before; ++i) {
      size_t idx = (before - 1 - i) % size;
      view[i] = view[before + size - 1 - idx];
    }
    for (size_t i = 0; i < after; ++i) {
      size_t idx = (after - 1 - i) % size;
      view[view.size() - 1 - i] = view[before + idx];
    }
  }
};
} // namespace pad_mode
} // namespace numcpp

#endif // NUMCPP_PADDING_H_INCLUDED
