/*
 * File: include/numcpp/math/gcd.h
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

#ifndef NUMCPP_GCD_H_INCLUDED
#define NUMCPP_GCD_H_INCLUDED

#include <numeric>
#include <type_traits>

namespace numcpp {
#if __cplusplus >= 201703L
using std::gcd;
using std::lcm;
#else
/**
 * @brief Return the greatest common divisor of abs(m) and abs(n).
 *
 * @param m An integer value.
 * @param n An integer value.
 *
 * @return The greatest common divisor of abs(m) and abs(n). If both m and n are
 * zero, return zero.
 */
template <class T>
typename std::enable_if<std::is_integral<T>::value, T>::type gcd(T m, T n) {
  // Euclidean algorithm
  if (m < 0) m = -m;
  if (n < 0) n = -n;
  while (n != 0) {
    T t = m % n;
    m = n;
    n = t;
  }
  return m;
}

template <class T1, class T2>
typename std::common_type<
    typename std::enable_if<std::is_integral<T1>::value, T1>::type,
    typename std::enable_if<std::is_integral<T2>::value, T2>::type>::type
gcd(T1 m, T2 n) {
  typedef typename std::common_type<T1, T2>::type T;
  return gcd(T(m), T(n));
}

/**
 * @brief Return the least common multiple of abs(m) and abs(n).
 *
 * @param m An integer value.
 * @param n An integer value.
 *
 * @return The least common multiple of abs(m) and abs(n). If either m or n is
 * zero, return zero.
 */
template <class T>
typename std::enable_if<std::is_integral<T>::value, T>::type lcm(T m, T n) {
  if (m == 0 || n == 0) return 0;
  if (m < 0) m = -m;
  if (n < 0) n = -n;
  return (m / gcd(m, n)) * n;
}

template <class T1, class T2>
typename std::common_type<
    typename std::enable_if<std::is_integral<T1>::value, T1>::type,
    typename std::enable_if<std::is_integral<T2>::value, T2>::type>::type
lcm(T1 m, T2 n) {
  typedef typename std::common_type<T1, T2>::type T;
  return lcm(T(m), T(n));
}
#endif

namespace math {
/**
 * @brief Function object implementing @ref np::gcd.
 */
struct gcd {
  template <class T, class U>
  auto operator()(T m, U n) const -> decltype(numcpp::gcd(m, n)) {
    return numcpp::gcd(m, n);
  }
};

/**
 * @brief Function object implementing @ref np::lcm.
 */
struct lcm {
  template <class T, class U>
  auto operator()(T m, U n) const -> decltype(numcpp::lcm(m, n)) {
    return numcpp::lcm(m, n);
  }
};
} // namespace math
} // namespace numcpp

#endif // NUMCPP_GCD_H_INCLUDED
