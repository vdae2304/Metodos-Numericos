/*
 * File: include/numcpp/expressions/binary_expr.h
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

#ifndef NUMCPP_BINARY_EXPRESSION_H_INCLUDED
#define NUMCPP_BINARY_EXPRESSION_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"
#include "numcpp/utilities/operators.h"

namespace numcpp {
/**
 * @brief An abstract tensor storing the results of applying a binary function
 * to each of the elements in two tensors, element-wise. This class relies on
 * lazy evaluation, meaning that multiple expressions can be nested and the
 * result of the whole expression will be computed only at the end, when the
 * whole expression is evaluated.
 *
 * @tparam Function Type of the applied function.
 * @tparam TensorLike1 Type of the first tensor where the function is applied.
 * @tparam TensorLike2 Type of the second tensor where the function is applied.
 */
template <class Function, class TensorLike1, class TensorLike2>
class binary_expr {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename std::remove_cvref_t<TensorLike1>::value_type,
      typename std::remove_cvref_t<TensorLike2>::value_type>
      value_type;
  static constexpr size_t rank = std::remove_cvref_t<TensorLike1>::rank;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  TensorLike1 m_arg1;

  // Second tensor argument.
  TensorLike2 m_arg2;

 public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * binary function to each element in two tensors.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   *
   * @note Undefined behaviour if arguments does not have the same shape.
   */
  binary_expr(Function&& f, TensorLike1&& a, TensorLike2&& b)
      : m_fun(std::forward<Function>(f)),
        m_arg1(std::forward<TensorLike1>(a)),
        m_arg2(std::forward<TensorLike2>(b)) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Indexing.

  /**
   * @brief Call operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) {
    return m_fun(m_arg1(indices...), m_arg2(indices...));
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) const {
    return m_fun(m_arg1(indices...), m_arg2(indices...));
  }

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  decltype(auto) operator[](const index_type& index) {
    return m_fun(m_arg1[index], m_arg2[index]);
  }

  decltype(auto) operator[](const index_type& index) const {
    return m_fun(m_arg1[index], m_arg2[index]);
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_type shape() const { return m_arg1.shape(); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const { return m_arg1.shape(axis); }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_type size() const { return m_arg1.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg1.layout() & m_arg2.layout(); }
};

/**
 * Class specialization when second argument is a scalar.
 */
template <class Function, class TensorLike, class T>
class binary_expr<Function, TensorLike, detail::identity<T>> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename std::remove_cvref_t<TensorLike>::value_type, T>
      value_type;
  static constexpr size_t rank = std::remove_cvref_t<TensorLike>::rank;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  TensorLike m_arg;

  // Second scalar argument.
  T m_val;

 public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * binary function to each element in two tensors.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   *
   * @note Undefined behaviour if arguments does not have the same shape.
   */
  binary_expr(Function&& f, TensorLike&& a, const T& val)
      : m_fun(std::forward<Function>(f)),
        m_arg(std::forward<TensorLike>(a)),
        m_val(val) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Indexing.

  /**
   * @brief Call operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) {
    return m_fun(m_arg(indices...), m_val);
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) const {
    return m_fun(m_arg(indices...), m_val);
  }

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  decltype(auto) operator[](const index_type& index) {
    return m_fun(m_arg[index], m_val);
  }

  decltype(auto) operator[](const index_type& index) const {
    return m_fun(m_arg[index], m_val);
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_type shape() const { return m_arg.shape(); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const { return m_arg.shape(axis); }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};

/**
 * Class specialization when first argument is a scalar.
 */
template <class Function, class TensorLike, class T>
class binary_expr<Function, detail::identity<T>, TensorLike> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, T, typename std::remove_cvref_t<TensorLike>::value_type>
      value_type;
  static constexpr size_t rank = std::remove_cvref_t<TensorLike>::rank;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 private:
  // Function to apply.
  Function m_fun;

  // First scalar argument.
  T m_val;

  // Second tensor argument.
  TensorLike m_arg;

 public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * binary function to each element in two tensors.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   *
   * @note Undefined behaviour if arguments does not have the same shape.
   */
  binary_expr(Function&& f, const T& val, TensorLike&& b)
      : m_fun(std::forward<Function>(f)),
        m_val(val),
        m_arg(std::forward<TensorLike>(b)) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Indexing.

  /**
   * @brief Call operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) {
    return m_fun(m_val, m_arg(indices...));
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) const {
    return m_fun(m_val, m_arg(indices...));
  }

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  decltype(auto) operator[](const index_type& index) {
    return m_fun(m_val, m_arg[index]);
  }

  decltype(auto) operator[](const index_type& index) const {
    return m_fun(m_val, m_arg[index]);
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_type shape() const { return m_arg.shape(); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const { return m_arg.shape(axis); }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};

/**
 * @brief Apply a binary function element-wise.
 *
 * @param f The function to apply.
 * @param a An abstract tensor with the values to pass as first argument.
 * @param b An abstract tensor with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument.
 *
 * @return An abstract tensor whose values are the result of invoking the
 * function on each element of the tensors. This function does not create a new
 * tensor, instead, an expression object is returned. The returned object uses
 * lazy-evaluation, which means that the function is called only when required,
 * i.e., when the whole expression is evaluated or assigned to a tensor.
 */
template <class Function, class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto apply2(Function&& f, TensorLike1&& a, TensorLike2&& b) {
  static_assert(a.rank == b.rank, "Tensor arguments must have equal rank");
  return binary_expr<Function, TensorLike1, TensorLike2>(
      std::forward<Function>(f), std::forward<TensorLike1>(a),
      std::forward<TensorLike2>(b));
}

template <class Function, class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto apply2(Function&& f, TensorLike&& a, const T& val) {
  return binary_expr<Function, TensorLike, detail::identity<T>>(
      std::forward<Function>(f), std::forward<TensorLike>(a), val);
}

template <class Function, class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto apply2(Function&& f, const T& val, TensorLike&& b) {
  return binary_expr<Function, detail::identity<T>, TensorLike>(
      std::forward<Function>(f), val, std::forward<TensorLike>(b));
}

/// Arithmetic operators.

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator+(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(plus(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator+(TensorLike&& lhs, const T& val) {
  return apply2(plus(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator+(const T& val, TensorLike&& rhs) {
  return apply2(plus(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator-(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(minus(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator-(TensorLike&& lhs, const T& val) {
  return apply2(minus(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator-(const T& val, TensorLike&& rhs) {
  return apply2(minus(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator*(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(multiplies(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator*(TensorLike&& lhs, const T& val) {
  return apply2(multiplies(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator*(const T& val, TensorLike&& rhs) {
  return apply2(multiplies(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator/(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(divides(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator/(TensorLike&& lhs, const T& val) {
  return apply2(divides(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator/(const T& val, TensorLike&& rhs) {
  return apply2(divides(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator%(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(modulus(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator%(TensorLike&& lhs, const T& val) {
  return apply2(modulus(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator%(const T& val, TensorLike&& rhs) {
  return apply2(modulus(), val, std::forward<TensorLike>(rhs));
}

/// Bitwise operators.

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator&(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(bit_and(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator&(TensorLike&& lhs, const T& val) {
  return apply2(bit_and(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator&(const T& val, TensorLike&& rhs) {
  return apply2(bit_and(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator|(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(bit_or(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator|(TensorLike&& lhs, const T& val) {
  return apply2(bit_or(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator|(const T& val, TensorLike&& rhs) {
  return apply2(bit_or(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator^(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(bit_xor(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator^(TensorLike&& lhs, const T& val) {
  return apply2(bit_xor(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator^(const T& val, TensorLike&& rhs) {
  return apply2(bit_xor(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator<<(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(left_shift(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator<<(TensorLike&& lhs, const T& val) {
  return apply2(left_shift(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator<<(const T& val, TensorLike&& rhs) {
  return apply2(left_shift(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator>>(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(right_shift(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator>>(TensorLike&& lhs, const T& val) {
  return apply2(right_shift(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator>>(const T& val, TensorLike&& rhs) {
  return apply2(right_shift(), val, std::forward<TensorLike>(rhs));
}

/// Logical operators.

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator&&(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(logical_and(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator&&(TensorLike&& lhs, const T& val) {
  return apply2(logical_and(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator&&(const T& val, TensorLike&& rhs) {
  return apply2(logical_and(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator||(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(logical_or(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator||(TensorLike&& lhs, const T& val) {
  return apply2(logical_or(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator||(const T& val, TensorLike&& rhs) {
  return apply2(logical_or(), val, std::forward<TensorLike>(rhs));
}

/// Relational operators.

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator==(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(equal_to(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator==(TensorLike&& lhs, const T& val) {
  return apply2(equal_to(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator==(const T& val, TensorLike&& rhs) {
  return apply2(equal_to(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator!=(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(not_equal_to(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator!=(TensorLike&& lhs, const T& val) {
  return apply2(not_equal_to(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator!=(const T& val, TensorLike&& rhs) {
  return apply2(not_equal_to(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator<(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(less(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator<(TensorLike&& lhs, const T& val) {
  return apply2(less(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator<(const T& val, TensorLike&& rhs) {
  return apply2(less(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator>(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(greater(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator>(TensorLike&& lhs, const T& val) {
  return apply2(greater(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator>(const T& val, TensorLike&& rhs) {
  return apply2(greater(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator<=(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(less_equal(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator<=(TensorLike&& lhs, const T& val) {
  return apply2(less_equal(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator<=(const T& val, TensorLike&& rhs) {
  return apply2(less_equal(), val, std::forward<TensorLike>(rhs));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto operator>=(TensorLike1&& lhs, TensorLike2&& rhs) {
  return apply2(greater_equal(), std::forward<TensorLike1>(lhs),
                std::forward<TensorLike2>(rhs));
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator>=(TensorLike&& lhs, const T& val) {
  return apply2(greater_equal(), std::forward<TensorLike>(lhs), val);
}

template <class TensorLike, class T>
  requires abstract_tensor<std::remove_cvref_t<TensorLike>>
inline auto operator>=(const T& val, TensorLike&& rhs) {
  return apply2(greater_equal(), val, std::forward<TensorLike>(rhs));
}
}  // namespace numcpp

#endif  // NUMCPP_BINARY_EXPRESSION_H_INCLUDED
