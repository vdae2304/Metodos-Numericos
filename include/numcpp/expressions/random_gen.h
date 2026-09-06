/*
 * File: include/numcpp/expressions/random_generator.h
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

#ifndef NUMCPP_RANDOM_GENERATOR_H_INCLUDED
#define NUMCPP_RANDOM_GENERATOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"

namespace numcpp {
/**
 * @brief An abstract tensor whose values are the result of invoking a function.
 */
template <class Distribution, class Generator, size_t Rank>
class random_generator {
 public:
  /// Member types.
  typedef typename Distribution::result_type value_type;
  static constexpr size_t rank = Rank;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of invoking a
   * function.
   *
   * @param rvs Random distribution.
   * @param urng Uniform random generator.
   * @param shape Number of elements along each axis.
   */
  random_generator(Distribution rvs, const shape_type& shape, Generator& urng)
      : m_shape(shape), m_size(shape.prod()), m_rvs(rvs), m_urng(urng) {}

  /// Destructor.
  ~random_generator() = default;

  /// Indexing.

  /**
   * @brief Call operator. Returns the result of invoking the function.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The result of the function evaluation.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  value_type operator()(Indices...) {
    return m_rvs(m_urng);
  }

  /**
   * @brief Subscript operator. Returns the result of invoking the function.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return The result of the function evaluation.
   */
  value_type operator[](const index_type&) { return m_rvs(m_urng); }

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
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return layout_left | layout_right; }

 private:
  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Distribution to invoke.
  Distribution m_rvs;

  // Uniform random generator.
  Generator &m_urng;
};
} // namespace numcpp

#endif // NUMCPP_RANDOM_GENERATOR_H_INCLUDED
