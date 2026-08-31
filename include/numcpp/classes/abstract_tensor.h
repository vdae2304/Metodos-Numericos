/*
 * File: include/numcpp/classes/abstract_tensor.h
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

#ifndef NUMCPP_ABSTRACT_TENSOR_H_INCLUDED
#define NUMCPP_ABSTRACT_TENSOR_H_INCLUDED

#if __cplusplus < 202002L
#error This file requires compiler and library support for the ISO C++ 2020 \
standard. This support must be enabled with the -std=c++20 or -std=gnu++20 \
compiler options.
#else
#include "numcpp/shape.h"
#include "numcpp/enums/layout_t.h"
#include "numcpp/utilities/traits.h"

namespace numcpp {
/**
 * @brief Concept for tensor expressions.
 */
template <class T>
concept abstract_tensor = requires(T t) {
  /// Member types.
  typename T::value_type;
  T::rank;

  /**
   * @brief Return the element at the given position.
   */
  { t[index_t<T::rank>{}] } -> std::convertible_to<typename T::value_type>;

  /**
   * @brief Return the shape of the tensor.
   */
  { t.shape() } -> std::convertible_to<shape_t<T::rank>>;

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  { t.shape(size_t{}) } -> std::convertible_to<size_t>;

  /**
   * @brief Return the number of elements in the tensor.
   */
  { t.size() } -> std::convertible_to<size_t>;

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  { t.layout() } -> std::convertible_to<layout_t>;
};

/**
 * Concept for 1-dimensional tensor expressions.
 */
template <class T>
concept abstract_vector = abstract_tensor<T> && (T::rank == 1);

/**
 * Concept for 2-dimensional tensor expressions.
 */
template <class T>
concept abstract_matrix = abstract_tensor<T> && (T::rank == 2);

/**
 * @brief Tensors are contiguous multidimensional sequence containers: they
 * hold a variable number of elements arranged in multiple axes.
 */
template <class T, size_t Rank>
class tensor;

/**
 * 1-dimensional tensor.
 */
template <class T>
using vector = tensor<T, 1>;

/**
 * 2-dimensional tensor.
 */
template <class T>
using matrix = tensor<T, 2>;

/**
 * @brief A @ref tensor_view is a view of a multidimensional array. It
 * references the elements in the original array.
 */
template <class T, size_t Rank>
class tensor_view;

/**
 * 1-dimensional tensor_view.
 */
template <class T>
using vector_view = tensor_view<T, 1>;

/**
 * 2-dimensional tensor_view.
 */
template <class T>
using matrix_view = tensor_view<T, 2>;
} // namespace numcpp

#endif // C++20
#endif // NUMCPP_ABSTRACT_TENSOR_H_INCLUDED
