/*
 * File: include/numcpp/math/constants.h
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

#ifndef NUMCPP_MATH_CONSTANTS_H_INCLUDED
#define NUMCPP_MATH_CONSTANTS_H_INCLUDED

#include <limits>

namespace numcpp {
template <class T> struct constants {
  /// Floating point representation of (positive) infinity.
  static constexpr T inf = std::numeric_limits<T>::infinity();

  /// Floating point representation of quiet NaN (Not a Number).
  static constexpr T nan = std::numeric_limits<T>::quiet_NaN();

  /// Euler's constant, base of natural logarithm.
  static constexpr T e = T(2.7182818284590452353602874713526625L);

  /// Pi, ratio of a circle's circunferemce to its diameter.
  static constexpr T pi = T(3.1415926535897932384626433832795029L);

  /// Golden ratio.
  static constexpr T golden = T(1.6180339887498948482045868343656381L);

  /// Euler–Mascheroni's constant.
  static constexpr T egamma = T(0.5772156649015328606065120900824024L);
};
} // namespace numcpp

#endif // NUMCPP_MATH_CONSTANTS_H_INCLUDED
