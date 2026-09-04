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
 * @tparam TensorLike1 Type of the first tensor where the function is applied.
 * @tparam TensorLike2 Type of the second tensor where the function is applied.
 */
template <class Function, class TensorLike1, class TensorLike2>
class outer_expr {
 private:
  static constexpr size_t rank1 = std::remove_cvref_t<TensorLike1>::rank;
  static constexpr size_t rank2 = std::remove_cvref_t<TensorLike2>::rank;

 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename std::remove_cvref_t<TensorLike1>::value_type,
      typename std::remove_cvref_t<TensorLike2>::value_type>
      value_type;
  static constexpr size_t rank = rank1 + rank2;
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
   * binary function to all pairs of elements.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   */
  outer_expr(Function&& f, TensorLike1&& a, TensorLike2&& b)
      : m_fun(std::forward<Function>(f)),
        m_arg1(std::forward<TensorLike1>(a)),
        m_arg2(std::forward<TensorLike2>(b)) {}

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
  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) {
    return __split_call_impl(std::forward_as_tuple(indices...),
                             std::make_index_sequence<rank1>(),
                             std::make_index_sequence<rank2>());
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) const {
    return __split_call_impl(std::forward_as_tuple(indices...),
                             std::make_index_sequence<rank1>(),
                             std::make_index_sequence<rank2>());
  }

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
  decltype(auto) operator[](const index_type& index) {
    index_t<rank1> i;
    index_t<rank2> j;
    std::copy_n(index.data(), rank1, i.data());
    std::copy_n(index.data() + rank1, rank2, j.data());
    return m_fun(m_arg1[i], m_arg2[j]);
  }

  decltype(auto) operator[](const index_type& index) const {
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
  layout_t layout() const { return m_arg1.layout() & m_arg2.layout(); }

 private:
  /**
   * Implement call operator.
   */
  template <class Tuple, size_t... Is, size_t... Js>
  decltype(auto) __split_call_impl(const Tuple& tuple,
                                   std::index_sequence<Is...>,
                                   std::index_sequence<Js...>) {
    return m_fun(m_arg1(std::get<Is>(tuple)...),
                 m_arg2(std::get<Js + rank1>(tuple)...));
  }

  template <class Tuple, size_t... Is, size_t... Js>
  decltype(auto) __split_call_impl(const Tuple& tuple,
                                   std::index_sequence<Is...>,
                                   std::index_sequence<Js...>) const {
    return m_fun(m_arg1(std::get<Is>(tuple)...),
                 m_arg2(std::get<Js + rank1>(tuple)...));
  }
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
template <class Function, class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto outer(Function&& f, TensorLike1&& a, TensorLike2&& b) {
  return outer_expr<Function, TensorLike1, TensorLike2>(
      std::forward<Function>(f), std::forward<TensorLike1>(a),
      std::forward<TensorLike2>(b));
}

template <class TensorLike1, class TensorLike2>
  requires abstract_tensor<std::remove_cvref_t<TensorLike1>> &&
           abstract_tensor<std::remove_cvref_t<TensorLike2>>
inline auto outer(TensorLike1&& a, TensorLike2&& b) {
  return outer(multiplies() std::forward<TensorLike1>(a),
               std::forward<TensorLike2>(b));
}
} // namespace numcpp

#endif // NUMCPP_OUTER_EXPRESSION_H_INCLUDED
