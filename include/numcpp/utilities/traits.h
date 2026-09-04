/*
 * File: include/utilities/traits.h
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

#ifndef NUMCPP_TRAITS_H_INCLUDED
#define NUMCPP_TRAITS_H_INCLUDED

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace numcpp {
/// Namespace for implementation details.
namespace detail {
using std::conjunction;
using std::disjunction;
using std::void_t;

/**
 * @brief Returns the type argument unchanged.
 */
template <class T> struct identity {
  typedef T type;
};

/**
 * Implementation of @ref promote.
 */
template <class T, bool = std::is_integral<T>::value>
struct __promote {
  typedef double type;
};

template <class T>
struct __promote<T, false> {};

template <>
struct __promote<float> {
  typedef float type;
};

template <>
struct __promote<double> {
  typedef double type;
};

template <>
struct __promote<long double> {
  typedef long double type;
};

/**
 * @brief Promotes integral types to floating-point.
 */
template <class... T>
using promote = std::common_type<typename __promote<T>::type...>;

/**
 * @brief Constructs a nested initializer_list of given depth.
 */
template <class T, size_t Depth>
struct nested_initializer_list {
  typedef std::initializer_list<
      typename nested_initializer_list<T, Depth - 1>::type>
      type;
};

template <class T>
struct nested_initializer_list<T, 0> {
  typedef T type;
};

/**
 * @brief Number of slice arguments in slice indexing.
 */
template <class... T>
constexpr size_t slicing_rank =
    (static_cast<size_t>(std::same_as<T, slice>) + ...);

/**
 * @brief Result type of function call.
 */
template <class Function, class... Args>
using result_of_t = typename std::remove_reference<typename std::remove_cv<
    decltype(std::declval<Function>()(std::declval<Args>()...))>::type>::type;

template <class Signature, typename = void>
struct __is_callable_impl : std::false_type {};

template <class F, class... Args>
struct __is_callable_impl<F(Args...), void_t<result_of_t<F, Args...>>>
    : std::true_type {};
} // namespace detail
} // namespace numcpp

#endif // NUMCPP_TRAITS_H_INCLUDED
