/*
 * File: include/numcpp/expressions/elementwise_expr.h
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

#ifndef NUMCPP_ELEMENTWISE_EXPRESSION_H_INCLUDED
#define NUMCPP_ELEMENTWISE_EXPRESSION_H_INCLUDED

#include <type_traits>
#include <tuple>
#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"

namespace numcpp {
#if __cplusplus >= 201402L
/**
 * @brief An abstract tensor storing the results of applying a function
 * element-wise. @ref element_wise_expr is a generalization of @ref unary_expr
 * and @ref binary_expr, accepting any number of arguments. This class relies on
 * lazy evaluation, meaning that multiple expressions can be nested and the
 * result of the whole expression will be computed only at the end, when the
 * whole expression is evaluated.
 *
 * @tparam Function Type of the applied function.
 * @tparam Expressions... Type of the tensors where the function is applied.
 */
template <class Function, class... Expressions>
class element_wise_expr
    : public abstract_tensor<
          element_wise_expr<Function, Expressions...>,
          detail::result_of_t<Function, typename detail::tensor_traits<
                                            Expressions>::value_type...>,
          detail::max_value<
              size_t, detail::tensor_traits<Expressions>::rank...>::value> {
 public:
  /// Member types.
  typedef detail::result_of_t<
      Function, typename detail::tensor_traits<Expressions>::value_type...>
      value_type;
  static constexpr size_t rank =
      detail::max_value<size_t,
                        detail::tensor_traits<Expressions>::rank...>::value;
  typedef value_type reference;
  typedef void pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

  static_assert(rank > 0,
                "All arguments cannot be scalar in element-wise expression");

 private:
  // Function to apply.
  Function m_fun;

  // Tensor arguments.
  std::tuple<Expressions...> m_args;

 public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * function element-wise.
   *
   * @param f The function to apply.
   * @param args... Tensor arguments.
   */
  element_wise_expr(Function f, Expressions... args)
      : m_fun(f), m_args(args...) {}

  element_wise_expr(Expressions... args)
      : element_wise_expr(Function(), args...) {}

  /// Destructor.
  ~element_wise_expr() = default;

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
  decltype(auto) operator()(Indices... indices) {
    return __at(indices..., std::make_index_sequence<sizeof...(Expressions)>());
  }

  template <class... Indices, detail::RequiresNIntegers<rank, Indices...> = 0>
  decltype(auto) operator()(Indices... indices) const {
    return __at(indices..., std::make_index_sequence<sizeof...(Expressions)>());
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
    return __at(index, std::make_index_sequence<sizeof...(Expressions)>());
  }

  decltype(auto) operator[](const index_type& index) const {
    return __at(index, std::make_index_sequence<sizeof...(Expressions)>());
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_type shape() const {
    return __shape(std::make_index_sequence<sizeof...(Expressions)>());
  }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const {
    return __shape_axis(axis,
                        std::make_index_sequence<sizeof...(Expressions)>());
  }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_type size() const {
    return __size(std::make_index_sequence<sizeof...(Expressions)>());
  }

  /**
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { (size() == 0); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const {
    return __layout(std::make_index_sequence<sizeof...(Expressions)>());
  }

 private:
  /**
   * @brief Return tensor element at the given position.
   */
  template <class T, detail::RequiresTensor<T> = 0>
  decltype(auto) __broadcast(T&& arg, const index_type& index) const {
    static_assert(detail::tensor_traits<T>::rank == rank,
                  "Expressions must have same dimension");
    return std::forward<T>(arg)[index];
  }

  template <class T, class... Indices, detail::RequiresTensor<T> = 0>
  decltype(auto) __broadcast(T&& arg, Indices... indices) const {
    static_assert(detail::tensor_traits<T>::rank == rank,
                  "Expressions must have same dimension");
    return std::forward<T>(arg)(indices...);
  }

  /**
   * @brief Overload for scalar types.
   */
  template <class T, detail::RequiresScalar<T> = 0>
  T __broadcast(T&& val, const index_type&) const {
    return std::forward<T>(val);
  }

  template <class T, class... Indices, detail::RequiresScalar<T> = 0>
  T __broadcast(T&& val, Indices...) {
    return std::forward<T>(val);
  }

  /**
   * @brief Implementation of call operator.
   */
  template <class... Indices, size_t... Is>
  decltype(auto) __at(Indices... indices, std::index_sequence<Is...>) {
    return m_fun(__broadcast(std::get<Is>(m_args), indices...)...);
  }

  template <class... Indices, size_t... Is>
  decltype(auto) __at(Indices... indices, std::index_sequence<Is...>) const {
    return m_fun(__broadcast(std::get<Is>(m_args), indices...)...);
  }

  /**
   * @brief Implementation of subscript operator.
   */
  template <size_t... Is>
  decltype(auto) __at(const index_type& index, std::index_sequence<Is...>) {
    return m_fun(__broadcast(std::get<Is>(m_args), index)...);
  }

  template <size_t... Is>
  decltype(auto) __at(const index_type& index,
                      std::index_sequence<Is...>) const {
    return m_fun(__broadcast(std::get<Is>(m_args), index)...);
  }

  /**
   * @brief Return the common shape for all arguments.
   */
  template <class T, class... Ts, detail::RequiresTensor<T> = 0>
  shape_type __common_shape(const T& a, const Ts&... b) const {
    return a.shape();
  }

  template <class T, class... Ts, detail::RequiresScalar<T> = 0>
  shape_type __common_shape(const T& a, const Ts&... b) const {
    return __common_shape(b...);
  }

  template <class T>
  shape_type __common_shape(const T& a) const {
    return a.shape();
  }

  /**
   * @brief Implementation of @ref shape().
   */
  template <size_t... Is>
  shape_type __shape(std::index_sequence<Is...>) const {
    return __common_shape(std::get<Is>(m_args)...);
  }

  /**
   * @brief Return the common shape for all arguments along given axis.
   */
  template <class T, class... Ts, detail::RequiresTensor<T> = 0>
  size_type __common_shape_axis(size_type axis, const T& a,
                                const Ts&... b) const {
    return a.shape(axis);
  }

  template <class T, class... Ts, detail::RequiresScalar<T> = 0>
  size_type __common_shape_axis(size_type axis, const T& a,
                                const Ts&... b) const {
    return __common_shape_axis(axis, b...);
  }

  template <class T>
  size_type __common_shape_axis(size_type axis, const T& a) const {
    return a.shape(axis);
  }

  /**
   * @brief Implementation of @ref shape(size_type).
   */
  template <size_t... Is>
  size_type __shape_axis(size_type axis, std::index_sequence<Is...>) const {
    return __common_shape_axis(axis, std::get<Is>(m_args)...);
  }

  /**
   * @brief Return the common size for all arguments.
   */
  template <class T, class... Ts, detail::RequiresTensor<T> = 0>
  size_type __common_size(const T& a, const Ts&... b) const {
    return a.size();
  }

  template <class T, class... Ts, detail::RequiresScalar<T> = 0>
  size_type __common_size(const T& a, const Ts&... b) const {
    return __common_size(b...);
  }

  template <class T>
  size_type __common_size(const T& a) const {
    return a.size();
  }

  /**
   * @brief Implementation of @ref size().
   */
  template <size_t... Is>
  size_type __size(std::index_sequence<Is...>) const {
    return __common_size(std::get<Is>(m_args)...);
  }

  /**
   * @brief Return the common layout for all arguments.
   */
  template <class T, class... Ts, detail::RequiresTensor<T> = 0>
  layout_t __common_layout(const T& a, const Ts&... b) const {
    return a.layout();
  }

  template <class T, class... Ts, detail::RequiresScalar<T> = 0>
  layout_t __common_layout(const T& a, const Ts&... b) const {
    return __common_layout(b...);
  }

  template <class T>
  layout_t __common_layout(const T& a) const {
    return a.layout();
  }

  /**
   * @brief Implementation of @ref layout().
   */
  template <size_t... Is>
  layout_t __layout(std::index_sequence<Is...>) const {
    return __common_layout(std::get<Is>(m_args)...);
  }
};
#endif // C++14
} // namespace numcpp

#endif // NUMCPP_ELEMENTWISE_EXPRESSION_H_INCLUDED
