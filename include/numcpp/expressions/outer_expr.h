/*
 * File: include/numcpp/expressions/outer_expr.h
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

#ifndef NUMCPP_OUTER_EXPRESSION_H_INCLUDED
#define NUMCPP_OUTER_EXPRESSION_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"
#include "numcpp/utilities/operators.h"

namespace numcpp {
/**
 * @brief An abstract tensor storing the result of applying a binary function
 * to all pairs of elements from two tensors. This class relies on lazy
 * evaluation, meaning that multiple expressions can be nested and the result
 * of the whole expression will be computed only at the end, when the
 * whole expression is evaluated.
 *
 * @tparam Function Type of the applied function.
 * @tparam Expression1 Type of the first tensor where the function is applied.
 * @tparam Expression2 Type of the second tensor where the function is applied.
 */
template <class Function, class Expression1, class Expression2>
class outer_expr
    : public abstract_tensor<
          outer_expr<Function, Expression1, Expression2>,
          detail::result_of_t<
              Function, typename detail::tensor_traits<Expression1>::value_type,
              typename detail::tensor_traits<Expression2>::value_type>,
          detail::tensor_traits<Expression1>::rank +
              detail::tensor_traits<Expression2>::rank> {
 private:
  static constexpr size_t rank1 = detail::tensor_traits<Expression1>::rank;
  static constexpr size_t rank2 = detail::tensor_traits<Expression2>::rank;

 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename detail::tensor_traits<Expression1>::value_type,
      typename detail::tensor_traits<Expression2>::value_type>
      value_type;
  static constexpr size_t rank = rank1 + rank2;
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
  Expression1 m_arg1;

  // Second tensor argument.
  Expression2 m_arg2;

 public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * binary function to all pairs of elements.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   */
  outer_expr(Function&& f, Expression1&& a, Expression2&& b)
      : m_fun(std::forward<Function>(f)),
        m_arg1(std::forward<Expression1>(a)),
        m_arg2(std::forward<Expression2>(b)) {}

  /// Destructor.
  ~outer_expr() = default;

  /// Indexing.

  /**
   * @brief Call operator. Returns the result of applying the function to a
   * pair of elements in the tensor.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
#if __cplusplus >= 201402L
  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  decltype(auto) operator()(Indices... indices) {
    return __split_call_impl(std::forward_as_tuple(indices...),
                             std::make_index_sequence<rank1>(),
                             std::make_index_sequence<rank2>());
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  decltype(auto) operator()(Indices... indices) const {
    return __split_call_impl(std::forward_as_tuple(indices...),
                             std::make_index_sequence<rank1>(),
                             std::make_index_sequence<rank2>());
  }
#else
  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices)
      -> decltype(this->operator[](index_type())) {
    return this->operator[](
        index_type{static_cast<difference_type>(indices)...});
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  auto operator()(Indices... indices) const
      -> decltype(this->operator[](index_type())) {
    return this->operator[](
        index_type{static_cast<difference_type>(indices)...});
  }
#endif // C++14

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * a pair of elements in the tensor.
   *
   * @param index An @ref index_t object with the position of an element in the
   * tensor.
   *
   * @return The result of the function evaluation at the specified position in
   * the tensor.
   */
  auto operator[](const index_type& index)
      -> decltype(m_fun(m_arg1[index_t<rank1>()], m_arg2[index_t<rank2>()])) {
    index_t<rank1> i;
    index_t<rank2> j;
    std::copy_n(index.data(), rank1, i.data());
    std::copy_n(index.data() + rank1, rank2, j.data());
    return m_fun(m_arg1[i], m_arg2[j]);
  }

  auto operator[](const index_type& index) const
      -> decltype(m_fun(m_arg1[index_t<rank1>()], m_arg2[index_t<rank2>()])) {
    index_t<rank1> i;
    index_t<rank2> j;
    std::copy_n(index.data(), rank1, i.data());
    std::copy_n(index.data() + rank1, rank2, j.data());
    return m_fun(m_arg1[i], m_arg2[j]);
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_type shape() const { return shape_cat(m_arg1.shape(), m_arg2.shape()); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const {
    return (axis < rank1) ? m_arg1.shape(axis) : m_arg2.shape(axis - rank1);
  }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_arg1.size() * m_arg2.size(); }

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

#if __cplusplus >= 201402L
 private:
  /**
   * Implement call operator.
   */
  template <class Tuple, size_t... Is, size_t... Js>
  decltype(auto) __split_call_impl(Tuple tuple, std::index_sequence<Is...>,
                              std::index_sequence<Js...>) {
    return m_fun(m_arg1(std::get<Is>(tuple)...),
                 m_arg2(std::get<Js + rank1>(tuple)...));
  }

  template <class Tuple, size_t... Is, size_t... Js>
  decltype(auto) __split_call_impl(Tuple tuple, std::index_sequence<Is...>,
                              std::index_sequence<Js...>) const {
    return m_fun(m_arg1(std::get<Is>(tuple)...),
                 m_arg2(std::get<Js + rank1>(tuple)...));
  }
#endif // C++14
};

/**
 * @brief Apply a function to all pairs (ai, bj) with ai in @ref a and bj in
 * @ref b.
 *
 * @details Let a and b be tensors of dimension M and N, respectively. The
 * result of @ref outer is a tensor of dimension M + N such that
 * @f[
 *   out(i_0, ..., i_{M-1}, j_0, ..., j_{N-1})
 *     = f(a(i_0, ..., i_{M-1}), b(j_0, ..., j_{N-1}))
 * @f]
 * 
 * @param f The function to apply.
 * @param a An abstract tensor with the values to pass as first argument.
 * @param b An abstract tensor with the values to pass as second argument.
 * 
 * @return An abstract tensor whose values are the result of invoking the
 * function to all pairs of elements. This function does not create a new
 * tensor, instead, an expression object is returned. The returned object uses
 * lazy-evaluation, which means that the function is called only when required,
 * i.e., when the whole expression is evaluated or assigned to a tensor.
 */
template <class Function, class Expr1, class T, size_t Rank1, class Expr2,
          class U, size_t Rank2>
inline outer_expr<Function, const Expr1&, const Expr2&> outer(
    Function&& f, const abstract_tensor<Expr1, T, Rank1>& a,
    const abstract_tensor<Expr2, U, Rank2>& b) {
  return outer_expr<Function, const Expr1&, const Expr2&>(
      std::forward<Function>(f), a.self(), b.self());
}

template <class Function, class Expr1, class T, size_t Rank1, class Expr2,
          class U, size_t Rank2>
inline outer_expr<Function, Expr1, const Expr2&> outer(
    Function&& f, abstract_tensor<Expr1, T, Rank1>&& a,
    const abstract_tensor<Expr2, U, Rank2>& b) {
  return outer_expr<Function, Expr1, const Expr2&>(
      std::forward<Function>(f), std::move(a.self()), b.self());
}

template <class Function, class Expr1, class T, size_t Rank1, class Expr2,
          class U, size_t Rank2>
inline outer_expr<Function, const Expr1&, Expr2> outer(
    Function&& f, const abstract_tensor<Expr1, T, Rank1>& a,
    abstract_tensor<Expr2, U, Rank2>&& b) {
  return outer_expr<Function, const Expr1&, Expr2>(
      std::forward<Function>(f), a.self(), std::move(b.self()));
}

template <class Function, class Expr1, class T, size_t Rank1, class Expr2,
          class U, size_t Rank2>
inline outer_expr<Function, Expr1, Expr2> outer(
    Function&& f, abstract_tensor<Expr1, T, Rank1>&& a,
    abstract_tensor<Expr2, U, Rank2>&& b) {
  return outer_expr<Function, Expr1, Expr2>(
      std::forward<Function>(f), std::move(a.self()), std::move(b.self()));
}
} // namespace numcpp

#endif // NUMCPP_OUTER_EXPRESSION_H_INCLUDED
