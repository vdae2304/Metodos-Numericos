/*
 * File: include/numcpp/math/mathfwd.h
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

#ifndef NUMCPP_MATHFWD_H_INCLUDED
#define NUMCPP_MATHFWD_H_INCLUDED

#include <cmath>
#include <complex>
#include <utility>

namespace numcpp {
/// Namespace for mathematical functions.
namespace math {
/// Basic functions.

/**
 * @brief Function object implementing @ref std::abs.
 */
struct abs {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::abs(std::forward<T>(x))) {
    return std::abs(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::fmod.
 */
struct fmod {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::fmod(x, y)) {
    return std::fmod(x, y);
  }
};

/**
 * @brief Function object implementing @ref std::modf.
 */
struct modf {
  template <class T> std::pair<T, T> operator()(T x) const {
    T intpart;
    T fracpart = std::modf(x, &intpart);
    return std::make_pair(intpart, fracpart);
  }
};

/**
 * @brief Function object implementing @ref std::fmax.
 */
struct fmax {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::fmax(x, y)) {
    return std::fmax(x, y);
  }
};

/**
 * @brief Function object implementing @ref std::fmin.
 */
struct fmin {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::fmin(x, y)) {
    return std::fmin(x, y);
  }
};

/// Trigonometric functions.

/**
 * @brief Function object implementing @ref std::cos.
 */
struct cos {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::cos(std::forward<T>(x))) {
    return std::cos(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::sin.
 */
struct sin {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::sin(std::forward<T>(x))) {
    return std::sin(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::tan.
 */
struct tan {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::tan(std::forward<T>(x))) {
    return std::tan(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::acos.
 */
struct acos {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::acos(std::forward<T>(x))) {
    return std::acos(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::asin.
 */
struct asin {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::asin(std::forward<T>(x))) {
    return std::asin(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::atan.
 */
struct atan {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::atan(std::forward<T>(x))) {
    return std::atan(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::atan2.
 */
struct atan2 {
  template <class T, class U>
  auto operator()(T y, U x) const -> decltype(std::atan2(y, x)) {
    return std::atan2(y, x);
  }
};

/**
 * @brief Function object implementing @ref std::hypot.
 */
struct hypot {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::hypot(x, y)) {
    return std::hypot(x, y);
  }
};

/// Hyperbolic functions.

/**
 * @brief Function object implementing @ref std::cosh.
 */
struct cosh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::cosh(std::forward<T>(x))) {
    return std::cosh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::sinh.
 */
struct sinh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::sinh(std::forward<T>(x))) {
    return std::sinh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::tanh.
 */
struct tanh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::tanh(std::forward<T>(x))) {
    return std::tanh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::acosh.
 */
struct acosh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::acosh(std::forward<T>(x))) {
    return std::acosh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::asinh.
 */
struct asinh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::asinh(std::forward<T>(x))) {
    return std::asinh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::atanh.
 */
struct atanh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::atanh(std::forward<T>(x))) {
    return std::atanh(std::forward<T>(x));
  }
};

/// Exponential and logarithmic functions.

/**
 * @brief Function object implementing @ref std::exp.
 */
struct exp {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::exp(std::forward<T>(x))) {
    return std::exp(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::log.
 */
struct log {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::log(std::forward<T>(x))) {
    return std::log(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::log10.
 */
struct log10 {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::log10(std::forward<T>(x))) {
    return std::log10(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::exp2.
 */
struct exp2 {
  template <class T> auto operator()(T x) const -> decltype(std::exp2(x)) {
    return std::exp2(x);
  }
};

/**
 * @brief Function object implementing @ref std::log2.
 */
struct log2 {
  template <class T> auto operator()(T x) const -> decltype(std::log2(x)) {
    return std::log2(x);
  }
};

/**
 * @brief Function object implementing @ref std::expm1.
 */
struct expm1 {
  template <class T> auto operator()(T x) const -> decltype(std::expm1(x)) {
    return std::expm1(x);
  }
};

/**
 * @brief Function object implementing @ref std::log1p.
 */
struct log1p {
  template <class T> auto operator()(T x) const -> decltype(std::log1p(x)) {
    return std::log1p(x);
  }
};

/// Power functions.

/**
 * @brief Function object implementing @ref std::pow.
 */
struct pow {
  template <class T, class U>
  auto operator()(T &&x, U &&y) const
      -> decltype(std::pow(std::forward<T>(x), std::forward<U>(y))) {
    return std::pow(std::forward<T>(x), std::forward<U>(y));
  }
};

/**
 * @brief Function object implementing @ref std::sqrt.
 */
struct sqrt {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::sqrt(std::forward<T>(x))) {
    return std::sqrt(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @ref std::cbrt.
 */
struct cbrt {
  template <class T> auto operator()(T x) const -> decltype(std::cbrt(x)) {
    return std::cbrt(x);
  }
};

/// Rounding.

/**
 * @brief Function object implementing @ref std::ceil.
 */
struct ceil {
  template <class T> auto operator()(T x) const -> decltype(std::ceil(x)) {
    return std::ceil(x);
  }
};

/**
 * @brief Function object implementing @ref std::floor.
 */
struct floor {
  template <class T> auto operator()(T x) const -> decltype(std::floor(x)) {
    return std::floor(x);
  }
};

/**
 * @brief Function object implementing @ref std::trunc.
 */
struct trunc {
  template <class T> auto operator()(T x) const -> decltype(std::trunc(x)) {
    return std::trunc(x);
  }
};

/**
 * @brief Function object implementing @ref std::round.
 */
struct round {
  template <class T> auto operator()(T x) const -> decltype(std::round(x)) {
    return std::round(x);
  }
};

/// Floating-point manipulation functions.

/**
 * @brief Function object implementing @ref std::frexp.
 */
struct frexp {
  template <class T>
  auto operator()(T x) const
      -> std::pair<decltype(std::frexp(x, std::declval<int *>())), int> {
    int exp;
    auto mantissa = std::frexp(x, &exp);
    return std::make_pair(mantissa, exp);
  }
};

/**
 * @brief Function object implementing @ref std::ldexp.
 */
struct ldexp {
  template <class T>
  auto operator()(T x, int exp) const -> decltype(std::ldexp(x, exp)) {
    return std::ldexp(x, exp);
  }
};

/**
 * @brief Function object implementing @ref std::copysign.
 */
struct copysign {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::copysign(x, y)) {
    return std::copysign(x, y);
  }
};

/**
 * @brief Function object implementing @ref std::nextafter.
 */
struct nextafter {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::nextafter(x, y)) {
    return std::nextafter(x, y);
  }
};

/// Complex numbers.

/**
 * @brief Function object implementing @ref std::real.
 */
struct real {
  template <class T>
  auto operator()(T &&z) const -> decltype(std::real(std::forward<T>(z))) {
    return std::real(std::forward<T>(z));
  }
};

/**
 * @brief Function object implementing @ref std::imag.
 */
struct imag {
  template <class T>
  auto operator()(T &&z) const -> decltype(std::imag(std::forward<T>(z))) {
    return std::imag(std::forward<T>(z));
  }
};

/**
 * @brief Function object implementing @ref std::conj.
 */
struct conj {
  template <class T>
  auto operator()(T &&z) const -> decltype(std::conj(std::forward<T>(z))) {
    return std::conj(std::forward<T>(z));
  }
};

/**
 * @brief Function object implementing @ref std::arg.
 */
struct arg {
  template <class T>
  auto operator()(T &&z) const -> decltype(std::arg(std::forward<T>(z))) {
    return std::arg(std::forward<T>(z));
  }
};

/// Clasification functions.

/**
 * @brief Function object implementing @ref std::isfinite.
 */
struct isfinite {
  template <class T> bool operator()(T x) const { return std::isfinite(x); }
};

/**
 * @brief Function object implementing @ref std::isinf.
 */
struct isinf {
  template <class T> bool operator()(T x) const { return std::isinf(x); }
};

/**
 * @brief Function object implementing @ref std::isnan.
 */
struct isnan {
  template <class T> bool operator()(T x) const { return std::isnan(x); }
};

/**
 * @brief Function object implementing @ref std::signbit.
 */
struct signbit {
  template <class T> bool operator()(T x) const { return std::signbit(x); }
};
} // namespace math
} // namespace numcpp

#endif // NUMCPP_MATHFWD_H_INCLUDED
