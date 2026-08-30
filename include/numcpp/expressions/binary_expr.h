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
 * @tparam Expression1 Type of the first tensor where the function is applied.
 * @tparam Expression2 Type of the second tensor where the function is applied.
 */
template <class Function, class Expression1, class Expression2>
class binary_expr
    : public abstract_tensor<
          binary_expr<Function, Expression1, Expression2>,
          detail::result_of_t<
              Function, typename detail::tensor_traits<Expression1>::value_type,
              typename detail::tensor_traits<Expression2>::value_type>,
          detail::tensor_traits<Expression1>::rank> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename detail::tensor_traits<Expression1>::value_type,
      typename detail::tensor_traits<Expression2>::value_type>
      value_type;
  static constexpr size_t rank = detail::tensor_traits<Expression1>::rank;
  typedef value_type reference;
  typedef void pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

  static_assert(rank == detail::tensor_traits<Expression2>::rank,
                "Arguments must have equal rank");

 private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  Expression1 m_arg1;

  // Second tensor argument.
  Expression2 m_arg2;

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
  binary_expr(Function&& f, Expression1&& a, Expression2&& b)
      : m_fun(std::forward<Function>(f)),
        m_arg1(std::forward<Expression1>(a)),
        m_arg2(std::forward<Expression2>(b)) {}

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
  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices)
      -> decltype(m_fun(m_arg1(indices...), m_arg2(indices...))) {
    return m_fun(m_arg1(indices...), m_arg2(indices...));
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices) const
      -> decltype(m_fun(m_arg1(indices...), m_arg2(indices...))) {
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
  auto operator[](const index_type& index)
      -> decltype(m_fun(m_arg1[index], m_arg2[index])) {
    return m_fun(m_arg1[index], m_arg2[index]);
  }

  auto operator[](const index_type& index) const
      -> decltype(m_fun(m_arg1[index], m_arg2[index])) {
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
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return (size() == 0); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const {
    layout_t layout1 = m_arg1.layout();
    layout_t layout2 = m_arg2.layout();
    if (layout1 == no_layout) return layout2;
    if (layout2 == no_layout) return layout1;
    return (layout1 == layout2) ? layout1 : no_layout;
  }
};

/**
 * Class specialization when second argument is a scalar.
 */
template <class Function, class Expression, class T>
class binary_expr<Function, Expression, detail::identity<T>>
    : public abstract_tensor<
          binary_expr<Function, Expression, detail::identity<T>>,
          detail::result_of_t<
              Function, typename detail::tensor_traits<Expression>::value_type,
              T>,
          detail::tensor_traits<Expression>::rank> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename detail::tensor_traits<Expression>::value_type, T>
      value_type;
  static constexpr size_t rank = detail::tensor_traits<Expression>::rank;
  typedef value_type reference;
  typedef void pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  Expression m_arg;

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
  binary_expr(Function&& f, Expression&& a, const T& val)
      : m_fun(std::forward<Function>(f)),
        m_arg(std::forward<Expression>(a)),
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
  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices)
      -> decltype(m_fun(m_arg(indices...), m_val)) {
    return m_fun(m_arg(indices...), m_val);
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices) const
      -> decltype(m_fun(m_arg(indices...), m_val)) {
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
  auto operator[](const index_type& index)
      -> decltype(m_fun(m_arg[index], m_val)) {
    return m_fun(m_arg[index], m_val);
  }

  auto operator[](const index_type& index) const
      -> decltype(m_fun(m_arg[index], m_val)) {
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
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return (size() == 0); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};

/**
 * Class specialization when first argument is a scalar.
 */
template <class Function, class Expression, class T>
class binary_expr<Function, detail::identity<T>, Expression>
    : public abstract_tensor<
          binary_expr<Function, detail::identity<T>, Expression>,
          detail::result_of_t<
              Function, T,
              typename detail::tensor_traits<Expression>::value_type>,
          detail::tensor_traits<Expression>::rank> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, T, typename detail::tensor_traits<Expression>::value_type>
      value_type;
  static constexpr size_t rank = detail::tensor_traits<Expression>::rank;
  typedef value_type reference;
  typedef void pointer;
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
  Expression m_arg;

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
  binary_expr(Function&& f, const T& val, Expression& b)
      : m_fun(std::forward<Function>(f)),
        m_val(val),
        m_arg(std::forward<Expression>(b)) {}

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
  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices)
      -> decltype(m_fun(m_val, m_arg(indices...))) {
    return m_fun(m_val, m_arg(indices...));
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices) const
      -> decltype(m_fun(m_val, m_arg(indices...))) {
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
  auto operator[](const index_type& index)
      -> decltype(m_fun(m_val, m_arg[index])) {
    return m_fun(m_val, m_arg[index]);
  }

  auto operator[](const index_type& index) const
      -> decltype(m_fun(m_val, m_arg[index])) {
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
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return (size() == 0); }

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
template <class Function, class Expr1, class T, class Expr2, class U,
          size_t Rank>
inline binary_expr<Function, const Expr1&, const Expr2&> apply2(
    Function&& f, const abstract_tensor<Expr1, T, Rank>& a,
    const abstract_tensor<Expr2, U, Rank>& b) {
  return binary_expr<Function, const Expr1&, const Expr2&>(
      std::forward<Function>(f), a.self(), b.self());
}

template <class Function, class Expr1, class T, class Expr2, class U,
          size_t Rank>
inline binary_expr<Function, Expr1, const Expr2&> apply2(
    Function&& f, abstract_tensor<Expr1, T, Rank>&& a,
    const abstract_tensor<Expr2, U, Rank>& b) {
  return binary_expr<Function, Expr1, const Expr2&>(
      std::forward<Function>(f), std::move(a.self()), b.self());
}

template <class Function, class Expr1, class T, class Expr2, class U,
          size_t Rank>
inline binary_expr<Function, const Expr1&, Expr2> apply2(
    Function&& f, const abstract_tensor<Expr1, T, Rank>& a,
    abstract_tensor<Expr2, U, Rank>&& b) {
  return binary_expr<Function, const Expr1&, Expr2>(
      std::forward<Function>(f), a.self(), std::move(b.self()));
}

template <class Function, class Expr1, class T, class Expr2, class U,
          size_t Rank>
inline binary_expr<Function, Expr1, Expr2> apply2(
    Function&& f, abstract_tensor<Expr1, T, Rank>&& a,
    abstract_tensor<Expr2, U, Rank>&& b) {
  return binary_expr<Function, Expr1, Expr2>(
      std::forward<Function>(f), std::move(a.self()), std::move(b.self()));
}

template <class Function, class Expr, class T, class U, size_t Rank,
          detail::RequiresScalar<U> = 0>
inline binary_expr<Function, const Expr&, detail::identity<U>> apply2(
    Function&& f, const abstract_tensor<Expr, T, Rank>& a, const U& val) {
  return binary_expr<Function, const Expr&, detail::identity<U>>(
      std::forward<Function>(f), a.self(), val);
}

template <class Function, class Expr, class T, class U, size_t Rank,
          detail::RequiresScalar<U> = 0>
inline binary_expr<Function, Expr, detail::identity<U>> apply2(
    Function&& f, abstract_tensor<Expr, T, Rank>&& a, const U& val) {
  return binary_expr<Function, Expr, detail::identity<U>>(
      std::forward<Function>(f), std::move(a.self()), val);
}

template <class Function, class Expr, class T, class U, size_t Rank,
          detail::RequiresScalar<T> = 0>
inline binary_expr<Function, detail::identity<T>, const Expr&> apply2(
    Function&& f, const T& val, const abstract_tensor<Expr, U, Rank>& b) {
  return binary_expr<Function, detail::identity<T>, const Expr&>(
      std::forward<Function>(f), val, b.self());
}

template <class Function, class Expr, class T, class U, size_t Rank,
          detail::RequiresScalar<T> = 0>
inline binary_expr<Function, detail::identity<T>, Expr> apply2(
    Function&& f, const T& val, abstract_tensor<Expr, U, Rank>&& b) {
  return binary_expr<Function, detail::identity<T>, Expr>(
      std::forward<Function>(f), val, std::move(b.self()));
}

/// Arithmetic operators.

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator+(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(plus(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(plus(), std::forward<LhsExpr>(lhs), std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator+(Expr&& lhs, const T& val)
    -> decltype(apply2(plus(), std::forward<Expr>(lhs), val)) {
  return apply2(plus(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator+(const T& val, Expr&& rhs)
    -> decltype(apply2(plus(), val, std::forward<Expr>(rhs))) {
  return apply2(plus(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator-(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(minus(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(minus(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator-(Expr&& lhs, const T& val)
    -> decltype(apply2(minus(), std::forward<Expr>(lhs), val)) {
  return apply2(minus(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator-(const T& val, Expr&& rhs)
    -> decltype(apply2(minus(), val, std::forward<Expr>(rhs))) {
  return apply2(minus(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator*(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(multiplies(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(multiplies(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator*(Expr&& lhs, const T& val)
    -> decltype(apply2(multiplies(), std::forward<Expr>(lhs), val)) {
  return apply2(multiplies(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator*(const T& val, Expr&& rhs)
    -> decltype(apply2(multiplies(), val, std::forward<Expr>(rhs))) {
  return apply2(multiplies(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator/(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(divides(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(divides(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator/(Expr&& lhs, const T& val)
    -> decltype(apply2(divides(), std::forward<Expr>(lhs), val)) {
  return apply2(divides(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator/(const T& val, Expr&& rhs)
    -> decltype(apply2(divides(), val, std::forward<Expr>(rhs))) {
  return apply2(divides(), val, std::forward<Expr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator%(Expr&& lhs, const T& val)
    -> decltype(apply2(modulus(), std::forward<Expr>(lhs), val)) {
  return apply2(modulus(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator%(const T& val, Expr&& rhs)
    -> decltype(apply2(modulus(), val, std::forward<Expr>(rhs))) {
  return apply2(modulus(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator%(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(modulus(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(modulus(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

/// Bitwise operators.

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator&(Expr&& lhs, const T& val)
    -> decltype(apply2(bit_and(), std::forward<Expr>(lhs), val)) {
  return apply2(bit_and(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator&(const T& val, Expr&& rhs)
    -> decltype(apply2(bit_and(), val, std::forward<Expr>(rhs))) {
  return apply2(bit_and(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator&(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(bit_and(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(bit_and(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator|(Expr&& lhs, const T& val)
    -> decltype(apply2(bit_or(), std::forward<Expr>(lhs), val)) {
  return apply2(bit_or(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator|(const T& val, Expr&& rhs)
    -> decltype(apply2(bit_or(), val, std::forward<Expr>(rhs))) {
  return apply2(bit_or(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator|(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(bit_or(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(bit_or(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator^(Expr&& lhs, const T& val)
    -> decltype(apply2(bit_xor(), std::forward<Expr>(lhs), val)) {
  return apply2(bit_xor(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator^(const T& val, Expr&& rhs)
    -> decltype(apply2(bit_xor(), val, std::forward<Expr>(rhs))) {
  return apply2(bit_xor(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator^(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(bit_xor(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(bit_xor(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator<<(Expr&& lhs, const T& val)
    -> decltype(apply2(left_shift(), std::forward<Expr>(lhs), val)) {
  return apply2(left_shift(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator<<(const T& val, Expr&& rhs)
    -> decltype(apply2(left_shift(), val, std::forward<Expr>(rhs))) {
  return apply2(left_shift(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator<<(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(left_shift(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(left_shift(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator>>(Expr&& lhs, const T& val)
    -> decltype(apply2(right_shift(), std::forward<Expr>(lhs), val)) {
  return apply2(right_shift(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator>>(const T& val, Expr&& rhs)
    -> decltype(apply2(right_shift(), val, std::forward<Expr>(rhs))) {
  return apply2(right_shift(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator>>(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(right_shift(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(right_shift(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

/// Logical operators.

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator&&(Expr&& lhs, const T& val)
    -> decltype(apply2(logical_and(), std::forward<Expr>(lhs), val)) {
  return apply2(logical_and(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator&&(const T& val, Expr&& rhs)
    -> decltype(apply2(logical_and(), val, std::forward<Expr>(rhs))) {
  return apply2(logical_and(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator&&(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(logical_and(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(logical_and(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator||(Expr&& lhs, const T& val)
    -> decltype(apply2(logical_or(), std::forward<Expr>(lhs), val)) {
  return apply2(logical_or(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator||(const T& val, Expr&& rhs)
    -> decltype(apply2(logical_or(), val, std::forward<Expr>(rhs))) {
  return apply2(logical_or(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator||(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(logical_or(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(logical_or(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

/// Relational operators.

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator==(Expr&& lhs, const T& val)
    -> decltype(apply2(equal_to(), std::forward<Expr>(lhs), val)) {
  return apply2(equal_to(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator==(const T& val, Expr&& rhs)
    -> decltype(apply2(equal_to(), val, std::forward<Expr>(rhs))) {
  return apply2(equal_to(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator==(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(equal_to(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(equal_to(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator!=(Expr&& lhs, const T& val)
    -> decltype(apply2(not_equal_to(), std::forward<Expr>(lhs), val)) {
  return apply2(not_equal_to(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator!=(const T& val, Expr&& rhs)
    -> decltype(apply2(not_equal_to(), val, std::forward<Expr>(rhs))) {
  return apply2(not_equal_to(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator!=(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(not_equal_to(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(not_equal_to(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator<(Expr&& lhs, const T& val)
    -> decltype(apply2(less(), std::forward<Expr>(lhs), val)) {
  return apply2(less(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator<(const T& val, Expr&& rhs)
    -> decltype(apply2(less(), val, std::forward<Expr>(rhs))) {
  return apply2(less(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator<(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(less(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(less(), std::forward<LhsExpr>(lhs), std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator>(Expr&& lhs, const T& val)
    -> decltype(apply2(greater(), std::forward<Expr>(lhs), val)) {
  return apply2(greater(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator>(const T& val, Expr&& rhs)
    -> decltype(apply2(greater(), val, std::forward<Expr>(rhs))) {
  return apply2(greater(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator>(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(greater(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(greater(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator<=(Expr&& lhs, const T& val)
    -> decltype(apply2(less_equal(), std::forward<Expr>(lhs), val)) {
  return apply2(less_equal(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator<=(const T& val, Expr&& rhs)
    -> decltype(apply2(less_equal(), val, std::forward<Expr>(rhs))) {
  return apply2(less_equal(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator<=(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(less_equal(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(less_equal(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator>=(Expr&& lhs, const T& val)
    -> decltype(apply2(greater_equal(), std::forward<Expr>(lhs), val)) {
  return apply2(greater_equal(), std::forward<Expr>(lhs), val);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto operator>=(const T& val, Expr&& rhs)
    -> decltype(apply2(greater_equal(), val, std::forward<Expr>(rhs))) {
  return apply2(greater_equal(), val, std::forward<Expr>(rhs));
}

template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto operator>=(LhsExpr&& lhs, RhsExpr&& rhs)
    -> decltype(apply2(greater_equal(), std::forward<LhsExpr>(lhs),
                       std::forward<RhsExpr>(rhs))) {
  return apply2(greater_equal(), std::forward<LhsExpr>(lhs),
                std::forward<RhsExpr>(rhs));
}
}  // namespace numcpp

#endif  // NUMCPP_BINARY_EXPRESSION_H_INCLUDED
