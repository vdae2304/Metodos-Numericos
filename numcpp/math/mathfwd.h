/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

/** @file include/numcpp/math/mathfwd.h
 *  This header defines function objects for several mathematical functions.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATHFWD_H_INCLUDED
#define NUMCPP_MATHFWD_H_INCLUDED

#include <cmath>
#include <type_traits>
#include <utility>

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

template <class T>
typename std::enable_if<std::is_integral<T>::value, double>::type degrees(T x) {
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

template <class T>
typename std::enable_if<std::is_integral<T>::value, double>::type radians(T x) {
  return radians((double)x);
}

/**
 * @brief Return the greatest common divisor of @a |m| and @a |n|.
 *
 * @param m An integer value.
 * @param n An integer value.
 *
 * @return The greatest common divisor of @a |m| and @a |n|. If both @a m and
 *         @a n are zero, return zero.
 */
template <class T1, class T2>
typename std::common_type<
    typename std::enable_if<std::is_integral<T1>::value, T1>::type,
    typename std::enable_if<std::is_integral<T2>::value, T2>::type>::type
gcd(T1 m, T2 n) {
  if (m < 0) {
    m = -m;
  }
  if (n < 0) {
    n = -n;
  }
  while (n != 0) {
    typename std::common_type<T1, T2>::type t = m % n;
    m = n;
    n = t;
  }
  return m;
}

/**
 * @brief Return the least common multiple of @a |m| and @a |n|.
 *
 * @param m An integer value.
 * @param n An integer value.
 *
 * @return The least common multiple of @a |m| and @a |n|. If either @a m or
 *         @a n is zero, return zero.
 */
template <class T1, class T2>
typename std::common_type<
    typename std::enable_if<std::is_integral<T1>::value, T1>::type,
    typename std::enable_if<std::is_integral<T2>::value, T2>::type>::type
lcm(T1 m, T2 n) {
  if (m < 0) {
    m = -m;
  }
  if (n < 0) {
    n = -n;
  }
  if (m != 0 && n != 0) {
    return (m / gcd(m, n)) * n;
  } else {
    return 0;
  }
}

/// Namespace for mathematical functions.
namespace math {
/// Basic functions.

/**
 * @brief Function object implementing @c std::abs.
 */
struct abs {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::abs(std::forward<T>(x))) {
    return std::abs(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::fmod.
 */
struct fmod {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::fmod(x, y)) {
    return std::fmod(x, y);
  }
};

/**
 * @brief Function object implementing @c std::modf.
 */
struct modf {
  template <class T> std::pair<T, T> operator()(T x) const {
    T intpart;
    T fracpart = std::modf(x, &intpart);
    return std::make_pair(intpart, fracpart);
  }
};

/**
 * @brief Function object implementing @c std::fmax.
 */
struct fmax {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::fmax(x, y)) {
    return std::fmax(x, y);
  }
};

/**
 * @brief Function object implementing @c std::fmin.
 */
struct fmin {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::fmin(x, y)) {
    return std::fmin(x, y);
  }
};

/// Trigonometric functions.

/**
 * @brief Function object implementing @c std::cos.
 */
struct cos {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::cos(std::forward<T>(x))) {
    return std::cos(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::sin.
 */
struct sin {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::sin(std::forward<T>(x))) {
    return std::sin(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::tan.
 */
struct tan {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::tan(std::forward<T>(x))) {
    return std::tan(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::acos.
 */
struct acos {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::acos(std::forward<T>(x))) {
    return std::acos(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::asin.
 */
struct asin {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::asin(std::forward<T>(x))) {
    return std::asin(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::atan.
 */
struct atan {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::atan(std::forward<T>(x))) {
    return std::atan(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::atan2.
 */
struct atan2 {
  template <class T, class U>
  auto operator()(T y, U x) const -> decltype(std::atan2(y, x)) {
    return std::atan2(y, x);
  }
};

/**
 * @brief Function object implementing @c std::hypot.
 */
struct hypot {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::hypot(x, y)) {
    return std::hypot(x, y);
  }
};

/**
 * @brief Function object implementing @c np::degrees.
 */
struct degrees {
  template <class T>
  auto operator()(T x) const -> decltype(numcpp::degrees(x)) {
    return numcpp::degrees(x);
  }
};

/**
 * @brief Function object implementing @c np::radians.
 */
struct radians {
  template <class T>
  auto operator()(T x) const -> decltype(numcpp::radians(x)) {
    return numcpp::radians(x);
  }
};

/// Hyperbolic functions.

/**
 * @brief Function object implementing @c std::cosh.
 */
struct cosh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::cosh(std::forward<T>(x))) {
    return std::cosh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::sinh.
 */
struct sinh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::sinh(std::forward<T>(x))) {
    return std::sinh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::tanh.
 */
struct tanh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::tanh(std::forward<T>(x))) {
    return std::tanh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::acosh.
 */
struct acosh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::acosh(std::forward<T>(x))) {
    return std::acosh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::asinh.
 */
struct asinh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::asinh(std::forward<T>(x))) {
    return std::asinh(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::atanh.
 */
struct atanh {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::atanh(std::forward<T>(x))) {
    return std::atanh(std::forward<T>(x));
  }
};

/// Exponential and logarithmic functions.

/**
 * @brief Function object implementing @c std::exp.
 */
struct exp {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::exp(std::forward<T>(x))) {
    return std::exp(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::log.
 */
struct log {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::log(std::forward<T>(x))) {
    return std::log(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::log10.
 */
struct log10 {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::log10(std::forward<T>(x))) {
    return std::log10(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::exp2.
 */
struct exp2 {
  template <class T> auto operator()(T x) const -> decltype(std::exp2(x)) {
    return std::exp2(x);
  }
};

/**
 * @brief Function object implementing @c std::log2.
 */
struct log2 {
  template <class T> auto operator()(T x) const -> decltype(std::log2(x)) {
    return std::log2(x);
  }
};

/**
 * @brief Function object implementing @c std::expm1.
 */
struct expm1 {
  template <class T> auto operator()(T x) const -> decltype(std::expm1(x)) {
    return std::expm1(x);
  }
};

/**
 * @brief Function object implementing @c std::log1p.
 */
struct log1p {
  template <class T> auto operator()(T x) const -> decltype(std::log1p(x)) {
    return std::log1p(x);
  }
};

/// Power functions.

/**
 * @brief Function object implementing @c std::pow.
 */
struct pow {
  template <class T, class U>
  auto operator()(T &&x, U &&y) const
      -> decltype(std::pow(std::forward<T>(x), std::forward<U>(y))) {
    return std::pow(std::forward<T>(x), std::forward<U>(y));
  }
};

/**
 * @brief Function object implementing @c std::sqrt.
 */
struct sqrt {
  template <class T>
  auto operator()(T &&x) const -> decltype(std::sqrt(std::forward<T>(x))) {
    return std::sqrt(std::forward<T>(x));
  }
};

/**
 * @brief Function object implementing @c std::cbrt.
 */
struct cbrt {
  template <class T> auto operator()(T x) const -> decltype(std::cbrt(x)) {
    return std::cbrt(x);
  }
};

/// Rounding.

/**
 * @brief Function object implementing @c std::ceil.
 */
struct ceil {
  template <class T> auto operator()(T x) const -> decltype(std::ceil(x)) {
    return std::ceil(x);
  }
};

/**
 * @brief Function object implementing @c std::floor.
 */
struct floor {
  template <class T> auto operator()(T x) const -> decltype(std::floor(x)) {
    return std::floor(x);
  }
};

/**
 * @brief Function object implementing @c std::trunc.
 */
struct trunc {
  template <class T> auto operator()(T x) const -> decltype(std::trunc(x)) {
    return std::trunc(x);
  }
};

/**
 * @brief Function object implementing @c std::round.
 */
struct round {
  template <class T> auto operator()(T x) const -> decltype(std::round(x)) {
    return std::round(x);
  }
};

/// Floating-point manipulation functions.

/**
 * @brief Function object implementing @c std::frexp.
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
 * @brief Function object implementing @c std::ldexp.
 */
struct ldexp {
  template <class T>
  auto operator()(T x, int exp) const -> decltype(std::ldexp(x, exp)) {
    return std::ldexp(x, exp);
  }
};

/**
 * @brief Function object implementing @c std::copysign.
 */
struct copysign {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::copysign(x, y)) {
    return std::copysign(x, y);
  }
};

/**
 * @brief Function object implementing @c std::nextafter.
 */
struct nextafter {
  template <class T, class U>
  auto operator()(T x, U y) const -> decltype(std::nextafter(x, y)) {
    return std::nextafter(x, y);
  }
};

/// Integer-valued functions.

/**
 * @brief Function object implementing @c np::gcd.
 */
struct gcd {
  template <class T, class U>
  auto operator()(T m, U n) const -> decltype(numcpp::gcd(m, n)) {
    return numcpp::gcd(m, n);
  }
};

/**
 * @brief Function object implementing @c np::lcm.
 */
struct lcm {
  template <class T, class U>
  auto operator()(T m, U n) const -> decltype(numcpp::lcm(m, n)) {
    return numcpp::lcm(m, n);
  }
};

/// Clasification functions.

/**
 * @brief Function object implementing @c std::isfinite.
 */
struct isfinite {
  template <class T> bool operator()(T x) const { return std::isfinite(x); }
};

/**
 * @brief Function object implementing @c std::isinf.
 */
struct isinf {
  template <class T> bool operator()(T x) const { return std::isinf(x); }
};

/**
 * @brief Function object implementing @c std::isnan.
 */
struct isnan {
  template <class T> bool operator()(T x) const { return std::isnan(x); }
};

/**
 * @brief Function object implementing @c std::signbit.
 */
struct signbit {
  template <class T> bool operator()(T x) const { return std::signbit(x); }
};
} // namespace math
} // namespace numcpp

#endif // NUMCPP_MATHFWD_H_INCLUDED
