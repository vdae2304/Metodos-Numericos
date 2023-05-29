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

/** @file include/numcpp/math.h
 *  This header defines mathematical functions for array and matrix classes.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATH_H_INCLUDED
#define NUMCPP_MATH_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/functional/lazy_expression.h"
#include "numcpp/math/complexfwd.h"
#include "numcpp/math/constants.h"
#include "numcpp/math/mathfwd.h"

namespace numcpp {
/// Basic functions.
using std::abs;
using std::fmod;
using std::modf;
using std::fmax;
using std::fmin;

/// Trigonometric functions.
using std::cos;
using std::sin;
using std::tan;
using std::acos;
using std::asin;
using std::atan;
using std::atan2;
using std::hypot;

/// Hyperbolic functions.
using std::cosh;
using std::sinh;
using std::tanh;
using std::acosh;
using std::asinh;
using std::atanh;

/// Exponential and logarithmic functions.
using std::exp;
using std::log;
using std::log10;
using std::exp2;
using std::log2;
using std::expm1;
using std::log1p;

/// Power functions.
using std::pow;
using std::sqrt;
using std::cbrt;

/// Rounding.
using std::ceil;
using std::floor;
using std::trunc;
using std::round;

/// Floating-point manipulation functions.
using std::frexp;
using std::ldexp;
using std::copysign;
using std::nextafter;

/// Complex numbers.
using std::real;
using std::imag;
using std::conj;
using std::arg;

/// Clasification functions.
using std::isfinite;
using std::isinf;
using std::isnan;
using std::signbit;

/// Basic functions.

/**
 * @brief Return the absolute value, element-wise.
 *
 * @param x A tensor-like object with the values whose absolute value is
 *          computed.
 *
 * @return A light-weight object with the absolute value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::abs, Container, T, Rank>
abs(const expression<Container, T, Rank> &x) {
  return unary_expr<math::abs, Container, T, Rank>(x);
}

/**
 * @brief Return the floating-point remainder of @a x/y, element-wise. The
 * floating-point remainder of @a x/y is @a x-n*y, where @a n is the truncated
 * value (i.e., rounded towards zero) of @a x/y.
 *
 * @param x A tensor-like object with the values of the quotient numerator.
 * @param y A tensor-like object with the values of the quotient denominator.
 *
 * @return A light-weight object with the remainder of @a x/y, element-wise.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::fmod, Container1, T, Container2, T, Rank>
fmod(const expression<Container1, T, Rank> &x,
     const expression<Container2, T, Rank> &y) {
  return binary_expr<math::fmod, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::fmod, Container, T, void, T, Rank>
fmod(const expression<Container, T, Rank> &x,
     const typename Container::value_type &y) {
  return binary_expr<math::fmod, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::fmod, void, T, Container, T, Rank>
fmod(const typename Container::value_type &x,
     const expression<Container, T, Rank> &y) {
  return binary_expr<math::fmod, void, T, Container, T, Rank>(x, y);
}

/**
 * @brief Descompose @a x into integral and fractional parts, element-wise.
 *
 * @param x A tensor-like object with the values to be decomposed.
 *
 * @return A light-weight object with the integral and fractional parts of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::modf, Container, T, Rank>
modf(const expression<Container, T, Rank> &x) {
  return unary_expr<math::modf, Container, T, Rank>(x);
}

/**
 * @brief Return the maximum value, element-wise. If one of the elements being
 * compared is a NaN, then the non-NaN element is returned.
 *
 * @param x A tensor-like object with floating-point or integer values.
 * @param y A tensor-like object with floating-point or integer values.
 *
 * @return A light-weight object with the element-wise maximum. This function
 *         does not create a new tensor, instead, an expression object is
 *         returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::fmax, Container1, T, Container2, T, Rank>
fmax(const expression<Container1, T, Rank> &x,
     const expression<Container2, T, Rank> &y) {
  return binary_expr<math::fmax, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::fmax, Container, T, void, T, Rank>
fmax(const expression<Container, T, Rank> &x,
     const typename Container::value_type &y) {
  return binary_expr<math::fmax, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::fmax, void, T, Container, T, Rank>
fmax(const typename Container::value_type &x,
     const expression<Container, T, Rank> &y) {
  return binary_expr<math::fmax, void, T, Container, T, Rank>(x, y);
}

/**
 * @brief Return the minimum value, element-wise. If one of the elements being
 * compared is a NaN, then the non-NaN element is returned.
 *
 * @param x A tensor-like object with floating-point or integer values.
 * @param y A tensor-like object with floating-point or integer values.
 *
 * @return A light-weight object with the element-wise minimum. This function
 *         does not create a new tensor, instead, an expression object is
 *         returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::fmin, Container1, T, Container2, T, Rank>
fmin(const expression<Container1, T, Rank> &x,
     const expression<Container2, T, Rank> &y) {
  return binary_expr<math::fmin, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::fmin, Container, T, void, T, Rank>
fmin(const expression<Container, T, Rank> &x,
     const typename Container::value_type &y) {
  return binary_expr<math::fmin, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::fmin, void, T, Container, T, Rank>
fmin(const typename Container::value_type &x,
     const expression<Container, T, Rank> &y) {
  return binary_expr<math::fmin, void, T, Container, T, Rank>(x, y);
}

/// Trigonometric functions.

/**
 * @brief Return the cosine, element-wise.
 *
 * @param x A tensor-like object with the angles in radians.
 *
 * @return A light-weight object with the cosine of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::cos, Container, T, Rank>
cos(const expression<Container, T, Rank> &x) {
  return unary_expr<math::cos, Container, T, Rank>(x);
}

/**
 * @brief Return the sine, element-wise.
 *
 * @param x A tensor-like object with the angles in radians.
 *
 * @return A light-weight object with the sine of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::sin, Container, T, Rank>
sin(const expression<Container, T, Rank> &x) {
  return unary_expr<math::sin, Container, T, Rank>(x);
}

/**
 * @brief Return the tangent, element-wise.
 *
 * @param x A tensor-like object with the angles in radians.
 *
 * @return A light-weight object with the tangent of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::tan, Container, T, Rank>
tan(const expression<Container, T, Rank> &x) {
  return unary_expr<math::tan, Container, T, Rank>(x);
}

/**
 * @brief Return the hypotenuse of a right-angled triangle whose legs are @a x
 * and @a y, element-wise.
 *
 * @param x A tensor-like object with one of the legs of the right-angle
 *          triangles.
 * @param y A tensor-like object with one of the legs of the right-angle
 *          triangles.
 *
 * @return A light-weight object with the hypotenuse of the triangles. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::hypot, Container1, T, Container2, T, Rank>
hypot(const expression<Container1, T, Rank> &x,
      const expression<Container2, T, Rank> &y) {
  return binary_expr<math::hypot, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::hypot, Container, T, void, T, Rank>
hypot(const expression<Container, T, Rank> &x,
      const typename Container::value_type &y) {
  return binary_expr<math::hypot, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::hypot, void, T, Container, T, Rank>
hypot(const typename Container::value_type &x,
      const expression<Container, T, Rank> &y) {
  return binary_expr<math::hypot, void, T, Container, T, Rank>(x, y);
}

/**
 * @brief Return the principal value of the arc cosine, element-wise.
 *
 * @param x A tensor-like object with the values whose arc cosine is computed.
 *
 * @return A light-weight object with the arc cosine, in radians, of each value
 *         in the tensor. This function does not create a new tensor, instead,
 *         an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::acos, Container, T, Rank>
acos(const expression<Container, T, Rank> &x) {
  return unary_expr<math::acos, Container, T, Rank>(x);
}

/**
 * @brief Return the principal value of the arc sine, element-wise.
 *
 * @param x A tensor-like object with the values whose arc sine is computed.
 *
 * @return A light-weight object with the arc sine, in radians, of each value in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::asin, Container, T, Rank>
asin(const expression<Container, T, Rank> &x) {
  return unary_expr<math::asin, Container, T, Rank>(x);
}

/**
 * @brief Return the principal value of the arc tangent, element-wise.
 *
 * @param x A tensor-like object with the values whose arc tangent is computed.
 *
 * @return A light-weight object with the arc tangent, in radians, of each value
 *         in the tensor. This function does not create a new tensor, instead,
 *         an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::atan, Container, T, Rank>
atan(const expression<Container, T, Rank> &x) {
  return unary_expr<math::atan, Container, T, Rank>(x);
}

/**
 * @brief Return the principal value of the arc tangent of @a y/x, element-wise.
 * To compute the value, the function takes into account the sign of both
 * arguments in order to determine the quadrant.
 *
 * @param y A tensor-like object with the @a y -coordinates.
 * @param x A tensor-like object with the @a x -coordinates.
 *
 * @return A light-weight object with the arc tangent, in radians, of @a y/x.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::atan2, Container1, T, Container2, T, Rank>
atan2(const expression<Container1, T, Rank> &y,
      const expression<Container2, T, Rank> &x) {
  return binary_expr<math::atan2, Container1, T, Container2, T, Rank>(y, x);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::atan2, Container, T, void, T, Rank>
atan2(const expression<Container, T, Rank> &y,
      const typename Container::value_type &x) {
  return binary_expr<math::atan2, Container, T, void, T, Rank>(y, x);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::atan2, void, T, Container, T, Rank>
atan2(const typename Container::value_type &y,
      const expression<Container, T, Rank> &x) {
  return binary_expr<math::atan2, void, T, Container, T, Rank>(y, x);
}

/**
 * @brief Convert angles from radians to degrees, element-wise.
 *
 * @param x A tensor-like object with the angles in radians.
 *
 * @return A light-weight object with the corresponding angles in degrees. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::degrees, Container, T, Rank>
degrees(const expression<Container, T, Rank> &x) {
  return unary_expr<math::degrees, Container, T, Rank>(x);
}

/**
 * @brief Convert angles from degrees to radians, element-wise.
 *
 * @param x A tensor-like object with the angles in degrees.
 *
 * @return A light-weight object with the corresponding angles in radians. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::radians, Container, T, Rank>
radians(const expression<Container, T, Rank> &x) {
  return unary_expr<math::radians, Container, T, Rank>(x);
}

/// Hyperbolic functions.

/**
 * @brief Return the hyperbolic cosine, element-wise.
 *
 * @param x A tensor-like object with the hyperbolic angles.
 *
 * @return A light-weight object with the hyperbolic cosine of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::cosh, Container, T, Rank>
cosh(const expression<Container, T, Rank> &x) {
  return unary_expr<math::cosh, Container, T, Rank>(x);
}

/**
 * @brief Return the hyperbolic sine, element-wise.
 *
 * @param x A tensor-like object with the hyperbolic angles.
 *
 * @return A light-weight object with the hyperbolic sine of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::sinh, Container, T, Rank>
sinh(const expression<Container, T, Rank> &x) {
  return unary_expr<math::sinh, Container, T, Rank>(x);
}

/**
 * @brief Return the hyperbolic tangent, element-wise.
 *
 * @param x A tensor-like object with the hyperbolic angles.
 *
 * @return A light-weight object with the hyperbolic tangent of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::tanh, Container, T, Rank>
tanh(const expression<Container, T, Rank> &x) {
  return unary_expr<math::tanh, Container, T, Rank>(x);
}

/**
 * @brief Return the inverse hyperbolic cosine, element-wise.
 *
 * @param x A tensor-like object with the values whose inverse hyperbolic cosine
 *          is computed.
 *
 * @return A light-weight object with the inverse hyperbolic cosine of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::acosh, Container, T, Rank>
acosh(const expression<Container, T, Rank> &x) {
  return unary_expr<math::acosh, Container, T, Rank>(x);
}

/**
 * @brief Return the inverse hyperbolic sine, element-wise.
 *
 * @param x A tensor-like object with the values whose inverse hyperbolic sine
 *          is computed.
 *
 * @return A light-weight object with the inverse hyperbolic sine of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::asinh, Container, T, Rank>
asinh(const expression<Container, T, Rank> &x) {
  return unary_expr<math::asinh, Container, T, Rank>(x);
}

/**
 * @brief Return the inverse hyperbolic tangent, element-wise.
 *
 * @param x A tensor-like object with the values whose inverse hyperbolic
 *          tangent is computed.
 *
 * @return A light-weight object with the inverse hyperbolic tangent of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::atanh, Container, T, Rank>
atanh(const expression<Container, T, Rank> &x) {
  return unary_expr<math::atanh, Container, T, Rank>(x);
}

/// Exponential and logarithmic functions.

/**
 * @brief Return the base-e exponential, which is @a e raised to the power @a x:
 * @a e^x, element-wise.
 *
 * @param x A tensor-like object with the values of the exponent.
 *
 * @return A light-weight object with the exponential of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::exp, Container, T, Rank>
exp(const expression<Container, T, Rank> &x) {
  return unary_expr<math::exp, Container, T, Rank>(x);
}

/**
 * @brief Return the natural logarithm, which is the inverse of the exponential
 * function (exp), element-wise.
 *
 * @param x A tensor-like object with the values whose logarithm is computed.
 *
 * @return A light weight-object with the natural logarithm of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::log, Container, T, Rank>
log(const expression<Container, T, Rank> &x) {
  return unary_expr<math::log, Container, T, Rank>(x);
}

/**
 * @brief Return the common (base-10) logarithm, element-wise.
 *
 * @param x A tensor-like object with the values whose logarithm is computed.
 *
 * @return A light-weight object with the common logarithm of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::log10, Container, T, Rank>
log10(const expression<Container, T, Rank> &x) {
  return unary_expr<math::log10, Container, T, Rank>(x);
}

/**
 * @brief Return the base-2 exponential, which is 2 raised to the power @a x:
 * @a 2^x, element-wise.
 *
 * @param x A tensor-like object with the values of the exponent.
 *
 * @return A light-weight object with the base-2 exponential of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::exp2, Container, T, Rank>
exp2(const expression<Container, T, Rank> &x) {
  return unary_expr<math::exp2, Container, T, Rank>(x);
}

/**
 * @brief Return the binary (base-2) logarithm, element-wise.
 *
 * @param x A tensor-like object with the values whose logarithm is computed.
 *
 * @return A light-weight object with the binary logarithm of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::log2, Container, T, Rank>
log2(const expression<Container, T, Rank> &x) {
  return unary_expr<math::log2, Container, T, Rank>(x);
}

/**
 * @brief Return @a e raised to the power @a x minus one: @a e^x - 1,
 * element-wise. For values of @a x close to zero, expm1 is more accurate than
 * exp(x) - 1.
 *
 * @param x A tensor-like object with the values of the exponent.
 *
 * @return A light-weight object with the exponential minus one of each element
 *         in the tensor. This function does not create a new tensor, instead,
 *         an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::expm1, Container, T, Rank>
expm1(const expression<Container, T, Rank> &x) {
  return unary_expr<math::expm1, Container, T, Rank>(x);
}

/**
 * @brief Return the natural logarithm of one plus @a x, element-wise. For
 * values of @a x close to zero, log1p is more accurate than log(1 + x).
 *
 * @param x A tensor-like object with the values whose logarithm is computed.
 *
 * @return A light-weight object with the natural logarithm of (1 + x) for each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::log1p, Container, T, Rank>
log1p(const expression<Container, T, Rank> &x) {
  return unary_expr<math::log1p, Container, T, Rank>(x);
}

/// Power functions.

/**
 * @brief Return @a x raised to the power @a y, element-wise.
 *
 * @param x A tensor-like object with the values of the base.
 * @param y A tensor-like object with the values of the exponent.
 *
 * @return A light-weight object with the result of raising @a x to the power
 *         @a y, element-wise. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::pow, Container1, T, Container2, T, Rank>
pow(const expression<Container1, T, Rank> &x,
    const expression<Container2, T, Rank> &y) {
  return binary_expr<math::pow, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::pow, Container, T, void, T, Rank>
pow(const expression<Container, T, Rank> &x,
    const typename Container::value_type &y) {
  return binary_expr<math::pow, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::pow, void, T, Container, T, Rank>
pow(const typename Container::value_type &x,
    const expression<Container, T, Rank> &y) {
  return binary_expr<math::pow, void, T, Container, T, Rank>(x, y);
}

/**
 * @brief Return the square root, element-wise.
 *
 * @param x A tensor-like object with the values whose square root is computed.
 *
 * @return A light-weight object with the square root of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::sqrt, Container, T, Rank>
sqrt(const expression<Container, T, Rank> &x) {
  return unary_expr<math::sqrt, Container, T, Rank>(x);
}

/**
 * @brief Return the cubic root, element-wise.
 *
 * @param x A tensor-like object with the values whose cubic root is computed.
 *
 * @return A light-weight object with the cubic root of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::cbrt, Container, T, Rank>
cbrt(const expression<Container, T, Rank> &x) {
  return unary_expr<math::cbrt, Container, T, Rank>(x);
}

/// Rounding.

/**
 * @brief Rounds @a x upward. Return the smallest integral value that is not
 * less than @a x, element-wise.
 *
 * @param x A tensor-like object with the values to round up.
 *
 * @return A light-weight object with the ceiling of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::ceil, Container, T, Rank>
ceil(const expression<Container, T, Rank> &x) {
  return unary_expr<math::ceil, Container, T, Rank>(x);
}

/**
 * @brief Rounds @a x downward. Return the largest integral value that is not
 * greater than @a x, element-wise.
 *
 * @param x A tensor-like object with the values to round down.
 *
 * @return A light-weight object with the floor of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::floor, Container, T, Rank>
floor(const expression<Container, T, Rank> &x) {
  return unary_expr<math::floor, Container, T, Rank>(x);
}

/**
 * @brief Rounds @a x toward zero. Return the nearest integral value that is not
 * larger in magnitude than @a x, element-wise.
 *
 * @param x A tensor-like object with the values to truncate.
 *
 * @return A light-weight object with the truncated value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::trunc, Container, T, Rank>
trunc(const expression<Container, T, Rank> &x) {
  return unary_expr<math::trunc, Container, T, Rank>(x);
}

/**
 * @brief Return the integral value that is nearest to @a x, element-wise.
 * Halfway cases are rounded away from zero.
 *
 * @param x A tensor-like object with the values to round.
 *
 * @return A light-weight object with the rounded value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::round, Container, T, Rank>
round(const expression<Container, T, Rank> &x) {
  return unary_expr<math::round, Container, T, Rank>(x);
}

/// Floating-point manipulation functions.

/**
 * @brief Descompose @a x into its binary significand and an integral power of
 * two, such that @a x = significand * 2^exponent, element-wise.
 *
 * @param x A tensor-like object with the values to be decomposed.
 *
 * @return A light-weight object with the binary significand and the exponent of
 *         each element in the tensor. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::frexp, Container, T, Rank>
frexp(const expression<Container, T, Rank> &x) {
  return unary_expr<math::frexp, Container, T, Rank>(x);
}

/**
 * @brief Return the result of multiplying @a x (the significand) by 2 raised to
 * the power of @a exp (the exponent), element-wise.
 *
 * @param x A tensor-like object with the values of the significand.
 * @param exp A tensor-like object with the values of the exponent.
 *
 * @return A light-weight object with the result of @a x*2^exp, element-wise.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::ldexp, Container1, T, Container2, int, Rank>
ldexp(const expression<Container1, T, Rank> &x,
      const expression<Container2, int, Rank> &exp) {
  return binary_expr<math::ldexp, Container1, T, Container2, int, Rank>(x, exp);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::ldexp, Container, T, void, int, Rank>
ldexp(const expression<Container, T, Rank> &x, int exp) {
  return binary_expr<math::ldexp, Container, T, void, int, Rank>(x, exp);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::ldexp, void, T, Container, int, Rank>
ldexp(const T &x, const expression<Container, int, Rank> &exp) {
  return binary_expr<math::ldexp, void, T, Container, int, Rank>(x, exp);
}

/**
 * @brief Return a value with the magnitude of @a x and the sign of @a y,
 * element-wise.
 *
 * @param x A tensor-like object with the values to change the sign of.
 * @param y A tensor-like object with the values to copy the sign from.
 *
 * @return A light-weight object with the values from the first tensor and the
 *         signs from the second tensor. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::copysign, Container1, T, Container2, T, Rank>
copysign(const expression<Container1, T, Rank> &x,
         const expression<Container2, T, Rank> &y) {
  return binary_expr<math::copysign, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::copysign, Container, T, void, T, Rank>
copysign(const expression<Container, T, Rank> &x,
         const typename Container::value_type &y) {
  return binary_expr<math::copysign, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::copysign, void, T, Container, T, Rank>
copysign(const typename Container::value_type &x,
         const expression<Container, T, Rank> &y) {
  return binary_expr<math::copysign, void, T, Container, T, Rank>(x, y);
}

/**
 * @brief Return the next representable value after @a x in the direction of
 * @a y, element-wise.
 *
 * @param x A tensor-like object with the base values.
 * @param y A tensor-like object with the directions where to look for the next
 *          representable values.
 *
 * @return A light-weight object with the next representable value of each
 *         element in the first tensor in the direction of the second tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::nextafter, Container1, T, Container2, T, Rank>
nextafter(const expression<Container1, T, Rank> &x,
          const expression<Container2, T, Rank> &y) {
  return binary_expr<math::nextafter, Container1, T, Container2, T, Rank>(x, y);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::nextafter, Container, T, void, T, Rank>
nextafter(const expression<Container, T, Rank> &x,
          const typename Container::value_type &y) {
  return binary_expr<math::nextafter, Container, T, void, T, Rank>(x, y);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::nextafter, void, T, Container, T, Rank>
nextafter(const typename Container::value_type &x,
          const expression<Container, T, Rank> &y) {
  return binary_expr<math::nextafter, void, T, Container, T, Rank>(x, y);
}

/// Integer-valued functions.

/**
 * @brief Return the greatest common divisor, element-wise.
 *
 * @param m A tensor-like object with integer values.
 * @param n A tensor-like object with integer values.
 *
 * @return A light-weight object with the greatest common divisor of @a |m| and
 *         @a |n|, element-wise. If both @a m and @a n are zero, return zero.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::gcd, Container1, T, Container2, T, Rank>
gcd(const expression<Container1, T, Rank> &m,
    const expression<Container2, T, Rank> &n) {
  return binary_expr<math::gcd, Container1, T, Container2, T, Rank>(m, n);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::gcd, Container, T, void, T, Rank>
gcd(const expression<Container, T, Rank> &m,
    const typename Container::value_type &n) {
  return binary_expr<math::gcd, Container, T, void, T, Rank>(m, n);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::gcd, void, T, Container, T, Rank>
gcd(const typename Container::value_type &m,
    const expression<Container, T, Rank> &n) {
  return binary_expr<math::gcd, void, T, Container, T, Rank>(m, n);
}

/**
 * @brief Return the least common multiple, element-wise.
 *
 * @param m A tensor-like object with integer values.
 * @param n A tensor-like object with integer values.
 *
 * @return A light-weight object with the least common multiple of @a |m| and
 *         @a |n|, element-wise. If either @a m or @a n is zero, return zero.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<math::lcm, Container1, T, Container2, T, Rank>
lcm(const expression<Container1, T, Rank> &m,
    const expression<Container2, T, Rank> &n) {
  return binary_expr<math::lcm, Container1, T, Container2, T, Rank>(m, n);
}

template <class Container, class T, size_t Rank>
inline binary_expr<math::lcm, Container, T, void, T, Rank>
lcm(const expression<Container, T, Rank> &m,
    const typename Container::value_type &n) {
  return binary_expr<math::lcm, Container, T, void, T, Rank>(m, n);
}

template <class T, class Container, size_t Rank>
inline binary_expr<math::lcm, void, T, Container, T, Rank>
lcm(const typename Container::value_type &m,
    const expression<Container, T, Rank> &n) {
  return binary_expr<math::lcm, void, T, Container, T, Rank>(m, n);
}

/// Complex numbers.

/**
 * @brief Return the real part, element-wise.
 *
 * @param z A tensor-like object with complex values.
 *
 * @return A light-weight object with the real part of each element in the
 *         tensor. Non-complex types are treated as complex numbers with zero
 *         imaginary part component. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::real, Container, T, Rank>
real(const expression<Container, T, Rank> &z) {
  return unary_expr<math::real, Container, T, Rank>(z);
}

/**
 * @brief Return the imaginary part, element-wise.
 *
 * @param z A tensor-like object with complex values.
 *
 * @return A light-weight object with the imaginary part of each element in the
 *         tensor. Non-complex types are treated as complex numbers with zero
 *         imaginary part component. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::imag, Container, T, Rank>
imag(const expression<Container, T, Rank> &z) {
  return unary_expr<math::imag, Container, T, Rank>(z);
}

/**
 * @brief Return the complex conjugate, element-wise. The conjugate of a complex
 * number is obtained by changing the sign of its imaginary part.
 *
 * @param z A tensor-like object with the values whose complex conjugate is
 *          computed.
 *
 * @return A light-weight object with the complex conjugate of each element in
 *         the tensor. Non-complex types are treated as complex numbers with
 *         zero imaginary part component. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::conj, Container, T, Rank>
conj(const expression<Container, T, Rank> &z) {
  return unary_expr<math::conj, Container, T, Rank>(z);
}

/**
 * @brief Return the absolute value, element-wise.
 *
 * @param z A tensor-like object with the values whose absolute value is
 *          computed.
 *
 * @return A light-weight object with the absolute value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */

/**
 * @brief Return the phase angle (in radians) of a complex number, element-wise.
 *
 * @param z A tensor-like object with the values whose phase angle is computed.
 *
 * @return A light-weight object with the phase angle of each element in the
 *         tensor. Non-complex types are treated as complex numbers with zero
 *         imaginary part component. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::arg, Container, T, Rank>
arg(const expression<Container, T, Rank> &z) {
  return unary_expr<math::arg, Container, T, Rank>(z);
}

/// Clasification functions.

/**
 * @brief Return whether @a x is a finite value (neither infinite nor NaN),
 * element-wise.
 *
 * @param x A tensor-like object with floating-point values.
 *
 * @return A light-weight object with each element set to true where @a x is
 *         finite and false otherwise. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::isfinite, Container, T, Rank>
isfinite(const expression<Container, T, Rank> &x) {
  return unary_expr<math::isfinite, Container, T, Rank>(x);
}

/**
 * @brief Return whether @a x is an infinity value (either positive infinity
 * or negative infinity), element-wise.
 *
 * @param x A tensor-like object with floating-point values.
 *
 * @return A light-weight object with each element set to true where @a x is
 *         infinity and false otherwise. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::isinf, Container, T, Rank>
isinf(const expression<Container, T, Rank> &x) {
  return unary_expr<math::isinf, Container, T, Rank>(x);
}

/**
 * @brief Return whether @a x is a NaN (Not a Number) value, element-wise. The
 * NaN values are used to identify undefined or non-representable values for
 * floating-point numbers, such as the square root of negative numbers or the
 * result of 0/0.
 *
 * @param x A tensor-like object with floating-point values.
 *
 * @return A light-weight object with each element set to true where @a x is NaN
 *         and false otherwise. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::isnan, Container, T, Rank>
isnan(const expression<Container, T, Rank> &x) {
  return unary_expr<math::isnan, Container, T, Rank>(x);
}

/**
 * @brief Return whether the sign of @a x is negative, element-wise.
 *
 * @param x A tensor-like object with floating-point or integer values.
 *
 * @return A light-weight object with each element set to true where @a x is
 *         negative and false otherwise. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Container, class T, size_t Rank>
inline unary_expr<math::signbit, Container, T, Rank>
signbit(const expression<Container, T, Rank> &x) {
  return unary_expr<math::signbit, Container, T, Rank>(x);
}
} // namespace numcpp

#endif // NUMCPP_MATH_H_INCLUDED
