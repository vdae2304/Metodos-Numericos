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

#include "numcpp/config.h"

namespace numcpp {
/**
 * @brief Base class for all tensor subexpressions. All subclasses must inherit
 * from this class using the Curiously Recurring Template Pattern.
 *
 * @tparam Derived Derived tensor subclass.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Derived, class T, size_t Rank> class abstract_tensor {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;

  /**
   * @brief Return the element at the given position.
   */
  T operator[](const index_t<Rank>& index) const { return self()[index]; }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_t<Rank> shape() const { return self().shape(); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_t shape(size_t axis) const { return self().shape(axis); }

  /**
   * @brief Return the number of elements in the tensor.
   *
   * @note Derived tensors are expected to implement this in O(1)
   */
  size_t size() const { return self().size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return self().layout(); }

  /**
   * @brief Return the derived subclass.
   */
  Derived &self() { return static_cast<Derived &>(*this); }

  const Derived &self() const { return static_cast<const Derived &>(*this); }
};

namespace detail {
/**
 * @brief Implementation of @ref tensor_traits for scalar types.
 */
template <class T, typename = void, typename = void>
struct __tensor_traits {
  typedef T value_type;
  static constexpr size_t rank = 0;
};

/**
 * @brief Implementation of @ref tensor_traits for tensor subclasses.
 */
template <class T>
struct __tensor_traits<T, void_t<typename T::value_type>,
                       void_t<decltype(T::rank)>> {
  typedef typename T::value_type value_type;
  static constexpr size_t rank = T::rank;
};

/**
 * @brief Helper class to expose a tensor member types. Treat scalars as tensors
 * of rank 0.
 */
template <class T>
struct tensor_traits : __tensor_traits<typename std::remove_reference<
                           typename std::remove_cv<T>::type>::type> {};

/**
 * @brief Type constraint to request a scalar (non-tensor) argument.
 */
template <class T>
using RequiresScalar =
    typename std::enable_if<(tensor_traits<T>::rank == 0), int>::type;

/**
 * @brief Type constraint to request a tensor argument.
 */
template <class T>
using RequiresTensor =
    typename std::enable_if<(tensor_traits<T>::rank > 0), int>::type;
}
} // namespace numcpp

#endif // NUMCPP_ABSTRACT_TENSOR_H_INCLUDED
