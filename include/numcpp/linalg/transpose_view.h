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

/** @file include/numcpp/linalg/transpose_view.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/linalg.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TRANSPOSE_VIEW_H_INCLUDED
#define NUMCPP_TRANSPOSE_VIEW_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/iterators/flat_iterator.h"

namespace numcpp {
/**
 * @brief A light-weight object which stores the elements of a tensor with its
 * axes reversed or permuted. This class represents a readonly view of the
 * elements of another tensor rather than a new tensor.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor.
 */
template <class Container, class T, size_t Rank>
class transpose_expr
    : public expression<transpose_expr<Container, T, Rank>, T, Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const transpose_expr<Container, T, Rank>, value_type,
                        rank, pointer, reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Tensor to transpose.
  const Container &m_arg;

  // Permuted shape.
  shape_type m_shape;

  // Permutation to apply.
  shape_type m_axes;

public:
  /// Constructors.

  /**
   * @brief Constructs a readonly view which reverse or permute the axes of a
   * tensor.
   *
   * @param a Tensor to transpose.
   * @param axes A permutation of (0, 1, ..., Rank - 1). Defaults to
   *             (Rank - 1, ..., 1, 0).
   */
  transpose_expr(const expression<Container, T, Rank> &a) : m_arg(a.self()) {
    for (size_t i = 0; i < Rank; ++i) {
      m_axes[i] = Rank - 1 - i;
      m_shape[i] = a.shape(m_axes[i]);
    }
  }

  transpose_expr(const expression<Container, T, Rank> &a,
                 const shape_type &axes)
      : m_arg(a.self()), m_axes(axes) {
    for (size_t i = 0; i < Rank; ++i) {
      m_shape[i] = a.shape(m_axes[i]);
    }
  }

  /// Destructor.
  ~transpose_expr() = default;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor.
   */
  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   */
  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns the element at the given position.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The element at the specified position.
   */
  auto operator[](const index_type &index) const -> decltype(m_arg[index]) {
    index_type out_index;
    for (size_t i = 0; i < Rank; ++i) {
      out_index[i] = index[m_axes[i]];
    }
    return m_arg[out_index];
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};

/**
 * @brief A light-weight object which stores the complex conjugate of the
 * elements in a tensor with its axes reversed or permuted. This class
 * represents a readonly view of the elements of another tensor rather than a
 * new tensor.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor.
 */
template <class Container, class T, size_t Rank> class conj_transpose_expr;

template <class Container, class T, size_t Rank>
class conj_transpose_expr<Container, std::complex<T>, Rank>
    : public expression<conj_transpose_expr<Container, std::complex<T>, Rank>,
                        std::complex<T>, Rank> {
public:
  /// Member types.
  typedef std::complex<T> value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef std::complex<T> reference;
  typedef flat_iterator<
      const conj_transpose_expr<Container, std::complex<T>, Rank>, value_type,
      rank, pointer, reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Tensor to conjugate transpose.
  const Container &m_arg;

  // Permuted shape.
  shape_type m_shape;

  // Permutation to apply.
  shape_type m_axes;

public:
  /// Constructors.

  /**
   * @brief Constructs a readonly view which reverse or permute the axes of a
   * tensor.
   *
   * @param a Tensor to conjugate transpose.
   * @param axes A permutation of (0, 1, ..., Rank - 1). Defaults to
   *             (Rank - 1, ..., 1, 0).
   */
  conj_transpose_expr(const expression<Container, std::complex<T>, Rank> &a)
      : m_arg(a.self()) {
    for (size_t i = 0; i < Rank; ++i) {
      m_axes[i] = Rank - 1 - i;
      m_shape[i] = a.shape(m_axes[i]);
    }
  }

  conj_transpose_expr(const expression<Container, std::complex<T>, Rank> &a,
                      const shape_type &axes)
      : m_arg(a.self()), m_axes(axes) {
    for (size_t i = 0; i < Rank; ++i) {
      m_shape[i] = a.shape(m_axes[i]);
    }
  }

  /// Destructor.
  ~conj_transpose_expr() = default;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor.
   */
  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   */
  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns the element at the given position.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The element at the specified position.
   */
  std::complex<T> operator[](const index_type &index) const {
    index_type out_index;
    for (size_t i = 0; i < Rank; ++i) {
      out_index[i] = index[m_axes[i]];
    }
    return std::conj(m_arg[out_index]);
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};
} // namespace numcpp

#endif // NUMCPP_TRANSPOSE_VIEW_H_INCLUDED
