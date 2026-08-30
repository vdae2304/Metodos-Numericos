/*
 * File: include/numcpp/expressions/unary_expr.h
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

#ifndef NUMCPP_UNARY_EXPRESSION_H_INCLUDED
#define NUMCPP_UNARY_EXPRESSION_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"
#include "numcpp/utilities/operators.h"

namespace numcpp {
/**
 * @brief An abstract tensor whose values are the result of applying an unary
 * function to each of the elements in another tensor. This class relies on
 * lazy evaluation, meaning that multiple expressions can be nested and the
 * result of the whole expression will be computed only at the end, when the
 * whole expression is evaluated.
 *
 * @tparam Function Type of the applied function.
 * @tparam Expression Type of the tensor where the function is applied.
 */
template <class Function, class Expression>
class unary_expr
    : public abstract_tensor<
          unary_expr<Function, Expression>,
          detail::result_of_t<
              Function, typename detail::tensor_traits<Expression>::value_type>,
          detail::tensor_traits<Expression>::rank> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename detail::tensor_traits<Expression>::value_type>
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

  // Tensor where the function is applied.
  Expression m_arg;

 public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying an
   * unary function to each element in a tensor.
   *
   * @param f The function to apply.
   * @param a Abstract tensor.
   */
  unary_expr(Function&& f, Expression&& a)
      : m_fun(std::forward<Function>(f)), m_arg(std::forward<Expression>(a)) {}

  /// Destructor.
  ~unary_expr() = default;

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
  auto operator()(Indices... indices) -> decltype(m_fun(m_arg(indices...))) {
    return m_fun(m_arg(indices...));
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices) const
      -> decltype(m_fun(m_arg(indices...))) {
    return m_fun(m_arg(indices...));
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
  auto operator[](const index_type& index) -> decltype(m_fun(m_arg[index])) {
    return m_fun(m_arg[index]);
  }

  auto operator[](const index_type& index) const
      -> decltype(m_fun(m_arg[index])) {
    return m_fun(m_arg[index]);
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
 * @brief Apply a function element-wise.
 *
 * @param f The function to apply.
 * @param a An abstract tensor with the values where the function will be
 * invoked.
 *
 * @return An abstract tensor whose values are the result of invoking the
 * function on each element of the tensor. This function does not create a new
 * tensor, instead, an expression object is returned. The returned object uses
 * lazy-evaluation, which means that the function is called only when required,
 * i.e., when the whole expression is evaluated or assigned to a tensor.
 */
template <class Function, class Expr, class T, size_t Rank>
inline unary_expr<Function, const Expr&> apply(
    Function&& f, const abstract_tensor<Expr, T, Rank>& a) {
  return unary_expr<Function, const Expr&>(std::forward<Function>(f), a.self());
}

template <class Function, class Expr, class T, size_t Rank>
inline unary_expr<Function, Expr> apply(Function&& f,
                                        abstract_tensor<Expr, T, Rank>&& a) {
  return unary_expr<Function, Expr>(std::forward<Function>(f),
                                    std::move(a.self()));
}

/// Unary operators.

template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto operator+(Expr&& a)
    -> decltype(apply(unary_plus(), std::forward<Expr>(a))) {
  return apply(unary_plus(), std::forward<Expr>(a));
}

template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto operator-(Expr&& a)
    -> decltype(apply(negate(), std::forward<Expr>(a))) {
  return apply(negate(), std::forward<Expr>(a));
}

template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto operator~(Expr&& a)
    -> decltype(apply(bit_not(), std::forward<Expr>(a))) {
  return apply(bit_not(), std::forward<Expr>(a));
}

template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto operator!(Expr&& a)
    -> decltype(apply(logical_not(), std::forward<Expr>(a))) {
  return apply(logical_not(), std::forward<Expr>(a));
}
} // namespace numcpp

#endif // NUMCPP_UNARY_EXPRESSION_H_INCLUDED
