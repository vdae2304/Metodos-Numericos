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

/** @file include/numcpp/routines/rearrange.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/routines.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_REARRANGE_H_INCLUDED
#define NUMCPP_REARRANGE_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/iterators/flat_iterator.h"

namespace numcpp {
/**
 * @brief A light-weight object which stores the elements of a tensor in
 * reversed order over multiple axes. This class represents a readonly view of
 * the elements of another tensor.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor.
 * @tparam N Number of axes to reverse.
 */
template <class Container, class T, size_t Rank, size_t N>
class reverse_expr
    : public expression<reverse_expr<Container, T, Rank, N>, T, Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const reverse_expr<Container, T, Rank, N>, T, Rank,
                        void, T>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Tensor to reverse.
  const Container &m_arg;

  // Axes along which to reverse over.
  shape_t<N> m_axes;

public:
  /// Constructors.

  /**
   * @brief Constructs a readonly view which stores the elements of a tensor in
   * reversed order.
   *
   * @param a Tensor to reverse.
   * @param axes Axes along which to reverse over.
   */
  reverse_expr(const expression<Container, T, Rank> &a, const shape_t<N> &axes)
      : m_arg(a.self()), m_axes(axes) {}

  /// Destructor.
  ~reverse_expr() = default;

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
  auto operator[](index_type index) const -> decltype(m_arg[index]) {
    for (size_t i = 0; i < N; ++i) {
      size_t axis = m_axes[i];
      index[axis] = m_arg.shape(axis) - 1 - index[axis];
    }
    return m_arg[index];
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  shape_type shape() const { return m_arg.shape(); }

  size_type shape(size_type axis) const { return m_arg.shape(axis); }

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
 * @brief A light-weight object which stores the elements of a tensor shifted
 * circularly over multiple axes. This class represents a readonly view of the
 * elements of another tensor.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor.
 * @tparam N Number of axes to shift.
 */
template <class Container, class T, size_t Rank, size_t N>
class rotate_expr
    : public expression<rotate_expr<Container, T, Rank, N>, T, Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const rotate_expr<Container, T, Rank, N>, T, Rank, void,
                        T>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Tensor to rotate.
  const Container &m_arg;

  // Number of positions to shift along each axis.
  index_t<N> m_shift;

  // Axes along which to rotate over.
  shape_t<N> m_axes;

public:
  /// Constructors.

  /**
   * @brief Constructs a readonly view which stores the elements of a tensor
   * shifted circularly.
   *
   * @param a Tensor to rotate.
   * @param shift Number of positions to shift along each axis.
   * @param axes Axes along which to rotate over.
   */
  rotate_expr(const expression<Container, T, Rank> &a, const index_t<N> &shift,
              const shape_t<N> &axes)
      : m_arg(a.self()), m_shift(shift), m_axes(axes) {}

  /// Destructor.
  ~rotate_expr() = default;

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
  auto operator[](index_type index) const -> decltype(m_arg[index]) {
    for (size_t i = 0; i < N; ++i) {
      size_t axis = m_axes[i];
      index[axis] = (index[axis] + m_shift[i]) % m_arg.shape(axis);
    }
    return m_arg[index];
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  shape_type shape() const { return m_arg.shape(); }

  size_type shape(size_type axis) const { return m_arg.shape(axis); }

  /**
   * @brief Return the number of elements in the tensor (i.e., the} product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};
} // namespace numcpp

#endif // NUMCPP_REARRANGE_H_INCLUDED
