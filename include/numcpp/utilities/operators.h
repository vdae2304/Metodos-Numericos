/*
 * File: include/utilities/operators.h
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

#ifndef NUMCPP_OPERATORS_H_INCLUDED
#define NUMCPP_OPERATORS_H_INCLUDED

#include <utility>

namespace numcpp {
/**
 * @brief Function object implementing static_cast.
 */
template <class From, class To> struct cast_to {
  constexpr To operator()(const From &arg) const {
    return static_cast<To>(arg);
  }
};

/**
 * @brief Function object implementing +arg.
 */
struct unary_plus {
  template <class T>
  constexpr auto operator()(T &&arg) const -> decltype(+std::forward<T>(arg)) {
    return +std::forward<T>(arg);
  }
};

/**
 * @brief Function object implementing -arg.
 */
struct negate {
  template <class T>
  constexpr auto operator()(T &&arg) const -> decltype(-std::forward<T>(arg)) {
    return -std::forward<T>(arg);
  }
};

/**
 * @brief Function object implementing lhs + rhs.
 */
struct plus {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) + std::forward<U>(rhs)) {
    return std::forward<T>(lhs) + std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs - rhs.
 */
struct minus {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) - std::forward<U>(rhs)) {
    return std::forward<T>(lhs) - std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs * rhs.
 */
struct multiplies {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) * std::forward<U>(rhs)) {
    return std::forward<T>(lhs) * std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs / rhs.
 */
struct divides {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) / std::forward<U>(rhs)) {
    return std::forward<T>(lhs) / std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs % rhs.
 */
struct modulus {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) % std::forward<U>(rhs)) {
    return std::forward<T>(lhs) % std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing ~arg.
 */
struct bit_not {
  template <class T>
  constexpr auto operator()(T &&arg) const -> decltype(~std::forward<T>(arg)) {
    return ~std::forward<T>(arg);
  }
};

/**
 * @brief Function object implementing lhs & rhs.
 */
struct bit_and {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) & std::forward<U>(rhs)) {
    return std::forward<T>(lhs) & std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs | rhs.
 */
struct bit_or {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) | std::forward<U>(rhs)) {
    return std::forward<T>(lhs) | std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs ^ rhs.
 */
struct bit_xor {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) ^ std::forward<U>(rhs)) {
    return std::forward<T>(lhs) ^ std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs << rhs.
 */
struct left_shift {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) << std::forward<U>(rhs)) {
    return std::forward<T>(lhs) << std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs >> rhs.
 */
struct right_shift {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) >> std::forward<U>(rhs)) {
    return std::forward<T>(lhs) >> std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing !arg.
 */
struct logical_not {
  template <class T>
  constexpr auto operator()(T &&arg) const -> decltype(!std::forward<T>(arg)) {
    return !std::forward<T>(arg);
  }
};

/**
 * @brief Function object implementing lhs && rhs.
 */
struct logical_and {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) && std::forward<U>(rhs)) {
    return std::forward<T>(lhs) && std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs || rhs.
 */
struct logical_or {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) || std::forward<U>(rhs)) {
    return std::forward<T>(lhs) || std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs == rhs.
 */
struct equal_to {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) == std::forward<U>(rhs)) {
    return std::forward<T>(lhs) == std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs != rhs.
 */
struct not_equal_to {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) != std::forward<U>(rhs)) {
    return std::forward<T>(lhs) != std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs < rhs.
 */
struct less {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) < std::forward<U>(rhs)) {
    return std::forward<T>(lhs) < std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs > rhs.
 */
struct greater {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) > std::forward<U>(rhs)) {
    return std::forward<T>(lhs) > std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs <= rhs.
 */
struct less_equal {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) <= std::forward<U>(rhs)) {
    return std::forward<T>(lhs) <= std::forward<U>(rhs);
  }
};

/**
 * @brief Function object implementing lhs <= rhs.
 */
struct greater_equal {
  template <class T, class U>
  constexpr auto operator()(T &&lhs, U &&rhs) const
      -> decltype(std::forward<T>(lhs) >= std::forward<U>(rhs)) {
    return std::forward<T>(lhs) >= std::forward<U>(rhs);
  }
};
} // namespace numcpp

#endif // NUMCPP_OPERATORS_H_INCLUDED
