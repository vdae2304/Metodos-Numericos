/*
 * File: include/numcpp/expressions/sequence_expr.h
 * Repository: https://github.com/vdae2304/Metodos-Numericos
 * 
 * Copyright (C) 2026 vdae2304
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NUMCPP_SEQUENCE_EXPR_H_INCLUDED
#define NUMCPP_SEQUENCE_EXPR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"

namespace numcpp {
/**
 * @brief An abstract tensor which stores a sequence of evenly spaced values.
 * This class represents a 1-dimensional tensor.
 *
 * @tparam T Type of the elements contained in the tensor.
 */
template <class T>
class sequence_expr : public abstract_tensor<sequence_expr<T>, T, 1> {
 public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = 1;
  typedef T reference;
  typedef void pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 public:
  /// Constructors.

  /**
   * @brief Constructs a sequence of evenly spaced values.
   *
   * @param start The starting value of the sequence.
   * @param size Number of values in the sequence.
   * @param step Spacing between values.
   */
  sequence_expr(const T& start, size_type size, const T& step)
      : m_start(start), m_shape{size}, m_step(step) {}

  /// Destructor.
  ~sequence_expr() = default;

  /// Indexing.

  /**
   * @brief Call operator. Return the element at the given position.
   *
   * @param i Position of an element along each axis.
   *
   * @return The element at the specified position.
   */
  T operator()(size_type i) const { return m_start + T(i) * m_step; }

  /**
   * @brief Subscript operator. Returns the element at the given position.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return The element at the specified position.
   */
  T operator[](const index_type& index) const {
    return this->operator()(index[0]);
  }

  /**
   * @brief Return the shape of the tensor.
   */
  const shape_type& shape() const { return m_shape; }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_type size() const { return m_shape[0]; }

  /**
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return (m_shape[0] == 0); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return default_layout; }

 private:
  // Starting value of the sequence.
  T m_start;

  // Number of elements along each axis.
  shape_type m_shape;

  // Step of the sequence.
  T m_step;
};
} // namespace numcpp

#endif // NUMCPP_SEQUENCE_EXPR_H_INCLUDED
