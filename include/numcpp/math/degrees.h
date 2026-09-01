/*
 * File: include/numcpp/math/degrees.h
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

#ifndef NUMCPP_DEGREES_H_INCLUDED
#define NUMCPP_DEGREES_H_INCLUDED

#include <concepts>

namespace numcpp {
/**
 * @brief Convert angles from radians to degrees.
 *
 * @param x Angle in radians.
 *
 * @return The corresponding angle in degrees.
 */
double degrees(double x) {
  const double inv_pi = 0.31830988618379067;
  return 180.0 * x * inv_pi;
}

float degrees(float x) {
  const float inv_pi = 0.318309886f;
  return 180.0f * x * inv_pi;
}

long double degrees(long double x) {
  const long double inv_pi = 0.318309886183790671537767526745028L;
  return 180.0L * x * inv_pi;
}

template <std::integral T>
double degrees(T x) {
  return degrees((double)x);
}

/**
 * @brief Convert angles from degrees to radians.
 *
 * @param x Angle in degrees.
 *
 * @return The corresponding angle in radians.
 */
double radians(double x) {
  const double pi = 3.14159265358979324;
  return pi * x / 180.0;
}

float radians(float x) {
  const float pi = 3.141592654f;
  return pi * x / 180.0f;
}

long double radians(long double x) {
  const long double pi = 3.141592653589793238462643383279502L;
  return pi * x / 180.0L;
}

template <std::integral T>
double radians(T x) {
  return radians((double)x);
}

namespace math {
/**
 * @brief Function object implementing @ref np::degrees.
 */
struct degrees {
  template <class T>
  auto operator()(T x) const -> decltype(numcpp::degrees(x)) {
    return numcpp::degrees(x);
  }
};

/**
 * @brief Function object implementing @ref np::radians.
 */
struct radians {
  template <class T>
  auto operator()(T x) const -> decltype(numcpp::radians(x)) {
    return numcpp::radians(x);
  }
};
}  // namespace math
} // namespace numcpp

#endif // NUMCPP_DEGREES_H_INCLUDED
