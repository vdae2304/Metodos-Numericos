/*
 * File: include/numcpp/expressions/const_expr.h
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

#ifndef NUMCPP_CONST_EXPR_H_INCLUDED
#define NUMCPP_CONST_EXPR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"

namespace numcpp {
/**
 * @brief An abstract tensor which always return the same constant value.
 *
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class T, size_t Rank>
class const_expr {
 public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef const T& reference;
  typedef const T* pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 public:
  /// Constructors.

  /**
   * @brief Constructs a const_expr.
   *
   * @param shape Number of elements along each axis.
   * @param val Value to return.
   */
  const_expr(const shape_type& shape, const T& val)
      : m_shape(shape), m_size(m_shape.prod()), m_val(val) {}

  /// Destructor.
  ~const_expr() = default;

  /// Indexing.

  /**
   * @brief Call operator. Returns a constant value.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return A constant value.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  const T& operator()(Indices...) const {
    return m_val;
  }

  /**
   * @brief Subscript operator. Returns a constant value.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return A constant value.
   */
  const T& operator[](const index_type&) const { return m_val; }

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
  size_type size() const { return m_size; }

  /**
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return (m_size == 0); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return no_layout; }

 private:
  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Value to return.
  T m_val;
};
} // namespace numcpp

#endif // NUMCPP_CONST_EXPR_H_INCLUDED
