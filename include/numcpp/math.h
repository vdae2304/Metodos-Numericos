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

#include "numcpp/tensor/abstract_tensor.h"
#include "numcpp/functional/lazy_expression.h"
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
 * @param x An abstact tensor with the values whose absolute value is computed.
 *
 * @return A light-weight object with the absolute value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::abs, Expr>
abs(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::abs, Expr>(x);
}

/**
 * @brief Return the floating-point remainder of @a x/y, element-wise. The
 * floating-point remainder of @a x/y is @a x-n*y, where @a n is the truncated
 * value (i.e., rounded towards zero) of @a x/y.
 *
 * @param x An abstract tensor with the values of the quotient numerator.
 * @param y An abstract tensor with the values of the quotient denominator.
 *
 * @return A light-weight object with the remainder of @a x/y, element-wise.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::fmod, Expr1, Expr2>
fmod(const abstract_tensor<Expr1, T, Rank> &x,
     const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::fmod, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::fmod, Expr, detail::identity<T>>
fmod(const abstract_tensor<Expr, T, Rank> &x,
     const typename detail::identity<T>::type &y) {
  return binary_expr<math::fmod, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::fmod, detail::identity<T>, Expr>
fmod(const typename detail::identity<T>::type &x,
     const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::fmod, detail::identity<T>, Expr>(x, y);
}

/**
 * @brief Descompose @a x into integral and fractional parts, element-wise.
 *
 * @param x An abstract tensor with the values to be decomposed.
 *
 * @return A light-weight object with the integral and fractional parts of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::modf, Expr>
modf(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::modf, Expr>(x);
}

/**
 * @brief Return the maximum value, element-wise. If one of the elements being
 * compared is a NaN, then the non-NaN element is returned.
 *
 * @param x An abstract tensor with floating-point or integer values.
 * @param y An abstract tensor with floating-point or integer values.
 *
 * @return A light-weight object with the element-wise maximum. This function
 *         does not create a new tensor, instead, an expression object is
 *         returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::fmax, Expr1, Expr2>
fmax(const abstract_tensor<Expr1, T, Rank> &x,
     const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::fmax, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::fmax, Expr, detail::identity<T>>
fmax(const abstract_tensor<Expr, T, Rank> &x,
     const typename detail::identity<T>::type &y) {
  return binary_expr<math::fmax, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::fmax, detail::identity<T>, Expr>
fmax(const typename detail::identity<T>::type &x,
     const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::fmax, detail::identity<T>, Expr>(x, y);
}

/**
 * @brief Return the minimum value, element-wise. If one of the elements being
 * compared is a NaN, then the non-NaN element is returned.
 *
 * @param x An abstract tensor with floating-point or integer values.
 * @param y An abstract tensor with floating-point or integer values.
 *
 * @return A light-weight object with the element-wise minimum. This function
 *         does not create a new tensor, instead, an expression object is
 *         returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::fmin, Expr1, Expr2>
fmin(const abstract_tensor<Expr1, T, Rank> &x,
     const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::fmin, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::fmin, Expr, detail::identity<T>>
fmin(const abstract_tensor<Expr, T, Rank> &x,
     const typename detail::identity<T>::type &y) {
  return binary_expr<math::fmin, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::fmin, detail::identity<T>, Expr>
fmin(const typename detail::identity<T>::type &x,
     const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::fmin, detail::identity<T>, Expr>(x, y);
}

/// Trigonometric functions.

/**
 * @brief Return the cosine, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return A light-weight object with the cosine of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::cos, Expr>
cos(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::cos, Expr>(x);
}

/**
 * @brief Return the sine, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return A light-weight object with the sine of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::sin, Expr>
sin(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::sin, Expr>(x);
}

/**
 * @brief Return the tangent, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return A light-weight object with the tangent of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::tan, Expr>
tan(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::tan, Expr>(x);
}

/**
 * @brief Return the hypotenuse of a right-angled triangle whose legs are @a x
 * and @a y, element-wise.
 *
 * @param x An abstract tensor with one of the legs of the right-angle
 *          triangles.
 * @param y An abstract tensor with one of the legs of the right-angle
 *          triangles.
 *
 * @return A light-weight object with the hypotenuse of the triangles. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::hypot, Expr1, Expr2>
hypot(const abstract_tensor<Expr1, T, Rank> &x,
      const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::hypot, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::hypot, Expr, detail::identity<T>>
hypot(const abstract_tensor<Expr, T, Rank> &x,
      const typename detail::identity<T>::type &y) {
  return binary_expr<math::hypot, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::hypot, detail::identity<T>, Expr>
hypot(const typename detail::identity<T>::type &x,
      const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::hypot, detail::identity<T>, Expr>(x, y);
}

/**
 * @brief Return the principal value of the arc cosine, element-wise.
 *
 * @param x An abstract tensor with the values whose arc cosine is computed.
 *
 * @return A light-weight object with the arc cosine, in radians, of each value
 *         in the tensor. This function does not create a new tensor, instead,
 *         an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::acos, Expr>
acos(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::acos, Expr>(x);
}

/**
 * @brief Return the principal value of the arc sine, element-wise.
 *
 * @param x An abstract tensor with the values whose arc sine is computed.
 *
 * @return A light-weight object with the arc sine, in radians, of each value in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::asin, Expr>
asin(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::asin, Expr>(x);
}

/**
 * @brief Return the principal value of the arc tangent, element-wise.
 *
 * @param x An abstract tensor with the values whose arc tangent is computed.
 *
 * @return A light-weight object with the arc tangent, in radians, of each value
 *         in the tensor. This function does not create a new tensor, instead,
 *         an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::atan, Expr>
atan(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::atan, Expr>(x);
}

/**
 * @brief Return the principal value of the arc tangent of @a y/x, element-wise.
 * To compute the value, the function takes into account the sign of both
 * arguments in order to determine the quadrant.
 *
 * @param y An abstract tensor with the @a y -coordinates.
 * @param x An abstract tensor with the @a x -coordinates.
 *
 * @return A light-weight object with the arc tangent, in radians, of @a y/x.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::atan2, Expr1, Expr2>
atan2(const abstract_tensor<Expr1, T, Rank> &y,
      const abstract_tensor<Expr2, T, Rank> &x) {
  return binary_expr<math::atan2, Expr1, Expr2>(y, x);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::atan2, Expr, detail::identity<T>>
atan2(const abstract_tensor<Expr, T, Rank> &y,
      const typename detail::identity<T>::type &x) {
  return binary_expr<math::atan2, Expr, detail::identity<T>>(y, x);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::atan2, detail::identity<T>, Expr>
atan2(const typename detail::identity<T>::type &y,
      const abstract_tensor<Expr, T, Rank> &x) {
  return binary_expr<math::atan2, detail::identity<T>, Expr>(y, x);
}

/**
 * @brief Convert angles from radians to degrees, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return A light-weight object with the corresponding angles in degrees. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::degrees, Expr>
degrees(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::degrees, Expr>(x);
}

/**
 * @brief Convert angles from degrees to radians, element-wise.
 *
 * @param x An abstract tensor with the angles in degrees.
 *
 * @return A light-weight object with the corresponding angles in radians. This
 *         function does not create a new tensor, instead, an expression object
 *         is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::radians, Expr>
radians(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::radians, Expr>(x);
}

/// Hyperbolic functions.

/**
 * @brief Return the hyperbolic cosine, element-wise.
 *
 * @param x An abstract tensor with the hyperbolic angles.
 *
 * @return A light-weight object with the hyperbolic cosine of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::cosh, Expr>
cosh(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::cosh, Expr>(x);
}

/**
 * @brief Return the hyperbolic sine, element-wise.
 *
 * @param x An abstract tensor with the hyperbolic angles.
 *
 * @return A light-weight object with the hyperbolic sine of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::sinh, Expr>
sinh(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::sinh, Expr>(x);
}

/**
 * @brief Return the hyperbolic tangent, element-wise.
 *
 * @param x An abstract tensor with the hyperbolic angles.
 *
 * @return A light-weight object with the hyperbolic tangent of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::tanh, Expr>
tanh(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::tanh, Expr>(x);
}

/**
 * @brief Return the inverse hyperbolic cosine, element-wise.
 *
 * @param x An abstract tensor with the values whose inverse hyperbolic cosine
 *          is computed.
 *
 * @return A light-weight object with the inverse hyperbolic cosine of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::acosh, Expr>
acosh(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::acosh, Expr>(x);
}

/**
 * @brief Return the inverse hyperbolic sine, element-wise.
 *
 * @param x An abstract tensor with the values whose inverse hyperbolic sine is
 *          computed.
 *
 * @return A light-weight object with the inverse hyperbolic sine of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::asinh, Expr>
asinh(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::asinh, Expr>(x);
}

/**
 * @brief Return the inverse hyperbolic tangent, element-wise.
 *
 * @param x An abstract tensor with the values whose inverse hyperbolic tangent
 *          is computed.
 *
 * @return A light-weight object with the inverse hyperbolic tangent of each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::atanh, Expr>
atanh(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::atanh, Expr>(x);
}

/// Exponential and logarithmic functions.

/**
 * @brief Return the base-e exponential, which is @a e raised to the power @a x:
 * @a e^x, element-wise.
 *
 * @param x An abstract tensor with the values of the exponent.
 *
 * @return A light-weight object with the exponential of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::exp, Expr>
exp(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::exp, Expr>(x);
}

/**
 * @brief Return the natural logarithm, which is the inverse of the exponential
 * function (exp), element-wise.
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return A light weight-object with the natural logarithm of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::log, Expr>
log(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::log, Expr>(x);
}

/**
 * @brief Return the common (base-10) logarithm, element-wise.
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return A light-weight object with the common logarithm of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::log10, Expr>
log10(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::log10, Expr>(x);
}

/**
 * @brief Return the base-2 exponential, which is 2 raised to the power @a x:
 * @a 2^x, element-wise.
 *
 * @param x An abstract tensor with the values of the exponent.
 *
 * @return A light-weight object with the base-2 exponential of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::exp2, Expr>
exp2(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::exp2, Expr>(x);
}

/**
 * @brief Return the binary (base-2) logarithm, element-wise.
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return A light-weight object with the binary logarithm of each element in
 *         the tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::log2, Expr>
log2(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::log2, Expr>(x);
}

/**
 * @brief Return @a e raised to the power @a x minus one: @a e^x - 1,
 * element-wise. For values of @a x close to zero, expm1 is more accurate than
 * exp(x) - 1.
 *
 * @param x An abstract tensor with the values of the exponent.
 *
 * @return A light-weight object with the exponential minus one of each element
 *         in the tensor. This function does not create a new tensor, instead,
 *         an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::expm1, Expr>
expm1(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::expm1, Expr>(x);
}

/**
 * @brief Return the natural logarithm of one plus @a x, element-wise. For
 * values of @a x close to zero, log1p is more accurate than log(1 + x).
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return A light-weight object with the natural logarithm of (1 + x) for each
 *         element in the tensor. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::log1p, Expr>
log1p(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::log1p, Expr>(x);
}

/// Power functions.

/**
 * @brief Return @a x raised to the power @a y, element-wise.
 *
 * @param x An abstract tensor with the values of the base.
 * @param y An abstract tensor with the values of the exponent.
 *
 * @return A light-weight object with the result of raising @a x to the power
 *         @a y, element-wise. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::pow, Expr1, Expr2>
pow(const abstract_tensor<Expr1, T, Rank> &x,
    const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::pow, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::pow, Expr, detail::identity<T>>
pow(const abstract_tensor<Expr, T, Rank> &x,
    const typename detail::identity<T>::type &y) {
  return binary_expr<math::pow, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::pow, detail::identity<T>, Expr>
pow(const typename detail::identity<T>::type &x,
    const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::pow, detail::identity<T>, Expr>(x, y);
}

/**
 * @brief Return the square root, element-wise.
 *
 * @param x An abstract tensor with the values whose square root is computed.
 *
 * @return A light-weight object with the square root of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::sqrt, Expr>
sqrt(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::sqrt, Expr>(x);
}

/**
 * @brief Return the cubic root, element-wise.
 *
 * @param x An abstract tensor with the values whose cubic root is computed.
 *
 * @return A light-weight object with the cubic root of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::cbrt, Expr>
cbrt(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::cbrt, Expr>(x);
}

/// Rounding.

/**
 * @brief Rounds @a x upward. Return the smallest integral value that is not
 * less than @a x, element-wise.
 *
 * @param x An abstract tensor with the values to round up.
 *
 * @return A light-weight object with the ceiling of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::ceil, Expr>
ceil(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::ceil, Expr>(x);
}

/**
 * @brief Rounds @a x downward. Return the largest integral value that is not
 * greater than @a x, element-wise.
 *
 * @param x An abstract tensor with the values to round down.
 *
 * @return A light-weight object with the floor of each element in the tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::floor, Expr>
floor(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::floor, Expr>(x);
}

/**
 * @brief Rounds @a x toward zero. Return the nearest integral value that is not
 * larger in magnitude than @a x, element-wise.
 *
 * @param x An abstract tensor with the values to truncate.
 *
 * @return A light-weight object with the truncated value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::trunc, Expr>
trunc(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::trunc, Expr>(x);
}

/**
 * @brief Return the integral value that is nearest to @a x, element-wise.
 * Halfway cases are rounded away from zero.
 *
 * @param x An abstract tensor with the values to round.
 *
 * @return A light-weight object with the rounded value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::round, Expr>
round(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::round, Expr>(x);
}

/// Floating-point manipulation functions.

/**
 * @brief Descompose @a x into its binary significand and an integral power of
 * two, such that @a x = significand * 2^exponent, element-wise.
 *
 * @param x An abstract tensor with the values to be decomposed.
 *
 * @return A light-weight object with the binary significand and the exponent of
 *         each element in the tensor. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::frexp, Expr>
frexp(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::frexp, Expr>(x);
}

/**
 * @brief Return the result of multiplying @a x (the significand) by 2 raised to
 * the power of @a exp (the exponent), element-wise.
 *
 * @param x An abstract tensor with the values of the significand.
 * @param exp An abstract tensor with the values of the exponent.
 *
 * @return A light-weight object with the result of @a x*2^exp, element-wise.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::ldexp, Expr1, Expr2>
ldexp(const abstract_tensor<Expr1, T, Rank> &x,
      const abstract_tensor<Expr2, int, Rank> &exp) {
  return binary_expr<math::ldexp, Expr1, Expr2>(x, exp);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::ldexp, Expr, detail::identity<int>>
ldexp(const abstract_tensor<Expr, T, Rank> &x, int exp) {
  return binary_expr<math::ldexp, Expr, detail::identity<int>>(x, exp);
}

template <class Expr, class T, size_t Rank, detail::RequiresScalar<T> = 0>
inline binary_expr<math::ldexp, detail::identity<T>, Expr>
ldexp(const T &x, const abstract_tensor<Expr, int, Rank> &exp) {
  return binary_expr<math::ldexp, detail::identity<T>, Expr>(x, exp);
}

/**
 * @brief Return a value with the magnitude of @a x and the sign of @a y,
 * element-wise.
 *
 * @param x An abstract tensor with the values to change the sign of.
 * @param y An abstract tensor with the values to copy the sign from.
 *
 * @return A light-weight object with the values from the first tensor and the
 *         signs from the second tensor. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::copysign, Expr1, Expr2>
copysign(const abstract_tensor<Expr1, T, Rank> &x,
         const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::copysign, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::copysign, Expr, detail::identity<T>>
copysign(const abstract_tensor<Expr, T, Rank> &x,
         const typename detail::identity<T>::type &y) {
  return binary_expr<math::copysign, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::copysign, detail::identity<T>, Expr>
copysign(const typename detail::identity<T>::type &x,
         const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::copysign, detail::identity<T>, Expr>(x, y);
}

/**
 * @brief Return the next representable value after @a x in the direction of
 * @a y, element-wise.
 *
 * @param x An abstract tensor with the base values.
 * @param y An abstract tensor with the directions where to look for the next
 *          representable values.
 *
 * @return A light-weight object with the next representable value of each
 *         element in the first tensor in the direction of the second tensor.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::nextafter, Expr1, Expr2>
nextafter(const abstract_tensor<Expr1, T, Rank> &x,
          const abstract_tensor<Expr2, T, Rank> &y) {
  return binary_expr<math::nextafter, Expr1, Expr2>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::nextafter, Expr, detail::identity<T>>
nextafter(const abstract_tensor<Expr, T, Rank> &x,
          const typename detail::identity<T>::type &y) {
  return binary_expr<math::nextafter, Expr, detail::identity<T>>(x, y);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::nextafter, detail::identity<T>, Expr>
nextafter(const typename detail::identity<T>::type &x,
          const abstract_tensor<Expr, T, Rank> &y) {
  return binary_expr<math::nextafter, detail::identity<T>, Expr>(x, y);
}

/// Integer-valued functions.

/**
 * @brief Return the greatest common divisor, element-wise.
 *
 * @param m An abstract tensor with integer values.
 * @param n An abstract tensor with integer values.
 *
 * @return A light-weight object with the greatest common divisor of @a |m| and
 *         @a |n|, element-wise. If both @a m and @a n are zero, return zero.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::gcd, Expr1, Expr2>
gcd(const abstract_tensor<Expr1, T, Rank> &m,
    const abstract_tensor<Expr2, T, Rank> &n) {
  return binary_expr<math::gcd, Expr1, Expr2>(m, n);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::gcd, Expr, detail::identity<T>>
gcd(const abstract_tensor<Expr, T, Rank> &m,
    const typename detail::identity<T>::type &n) {
  return binary_expr<math::gcd, Expr, detail::identity<T>>(m, n);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::gcd, detail::identity<T>, Expr>
gcd(const typename detail::identity<T>::type &m,
    const abstract_tensor<Expr, T, Rank> &n) {
  return binary_expr<math::gcd, detail::identity<T>, Expr>(m, n);
}

/**
 * @brief Return the least common multiple, element-wise.
 *
 * @param m An abstract tensor with integer values.
 * @param n An abstract tensor with integer values.
 *
 * @return A light-weight object with the least common multiple of @a |m| and
 *         @a |n|, element-wise. If either @a m or @a n is zero, return zero.
 *         This function does not create a new tensor, instead, an expression
 *         object is returned (see lazy-evaluation).
 */
template <class Expr1, class Expr2, class T, size_t Rank>
inline binary_expr<math::lcm, Expr1, Expr2>
lcm(const abstract_tensor<Expr1, T, Rank> &m,
    const abstract_tensor<Expr2, T, Rank> &n) {
  return binary_expr<math::lcm, Expr1, Expr2>(m, n);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::lcm, Expr, detail::identity<T>>
lcm(const abstract_tensor<Expr, T, Rank> &m,
    const typename detail::identity<T>::type &n) {
  return binary_expr<math::lcm, Expr, detail::identity<T>>(m, n);
}

template <class Expr, class T, size_t Rank>
inline binary_expr<math::lcm, detail::identity<T>, Expr>
lcm(const typename detail::identity<T>::type &m,
    const abstract_tensor<Expr, T, Rank> &n) {
  return binary_expr<math::lcm, detail::identity<T>, Expr>(m, n);
}

/// Complex numbers.

/**
 * @brief Return the real part, element-wise.
 *
 * @param z An abstract tensor with complex values.
 *
 * @return A light-weight object with the real part of each element in the
 *         tensor. Non-complex types are treated as complex numbers with zero
 *         imaginary part component. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::real, Expr>
real(const abstract_tensor<Expr, T, Rank> &z) {
  return unary_expr<math::real, Expr>(z);
}

/**
 * @brief Return the imaginary part, element-wise.
 *
 * @param z An abstract tensor with complex values.
 *
 * @return A light-weight object with the imaginary part of each element in the
 *         tensor. Non-complex types are treated as complex numbers with zero
 *         imaginary part component. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::imag, Expr>
imag(const abstract_tensor<Expr, T, Rank> &z) {
  return unary_expr<math::imag, Expr>(z);
}

/**
 * @brief Return the complex conjugate, element-wise. The conjugate of a complex
 * number is obtained by changing the sign of its imaginary part.
 *
 * @param z An abstract tensor with the values whose complex conjugate is
 *          computed.
 *
 * @return A light-weight object with the complex conjugate of each element in
 *         the tensor. Non-complex types are treated as complex numbers with
 *         zero imaginary part component. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::conj, Expr>
conj(const abstract_tensor<Expr, T, Rank> &z) {
  return unary_expr<math::conj, Expr>(z);
}

/**
 * @brief Return the absolute value, element-wise.
 *
 * @param z An abstract tensor with the values whose absolute value is computed.
 *
 * @return A light-weight object with the absolute value of each element in the
 *         tensor. This function does not create a new tensor, instead, an
 *         expression object is returned (see lazy-evaluation).
 */

/**
 * @brief Return the phase angle (in radians) of a complex number, element-wise.
 *
 * @param z An abstract tensor with the values whose phase angle is computed.
 *
 * @return A light-weight object with the phase angle of each element in the
 *         tensor. Non-complex types are treated as complex numbers with zero
 *         imaginary part component. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::arg, Expr>
arg(const abstract_tensor<Expr, T, Rank> &z) {
  return unary_expr<math::arg, Expr>(z);
}

/// Clasification functions.

/**
 * @brief Return whether @a x is a finite value (neither infinite nor NaN),
 * element-wise.
 *
 * @param x An abstract tensor with floating-point values.
 *
 * @return A light-weight object with each element set to true where @a x is
 *         finite and false otherwise. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::isfinite, Expr>
isfinite(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::isfinite, Expr>(x);
}

/**
 * @brief Return whether @a x is an infinity value (either positive infinity
 * or negative infinity), element-wise.
 *
 * @param x An abstract tensor with floating-point values.
 *
 * @return A light-weight object with each element set to true where @a x is
 *         infinity and false otherwise. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::isinf, Expr>
isinf(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::isinf, Expr>(x);
}

/**
 * @brief Return whether @a x is a NaN (Not a Number) value, element-wise. The
 * NaN values are used to identify undefined or non-representable values for
 * floating-point numbers, such as the square root of negative numbers or the
 * result of 0/0.
 *
 * @param x An abstract tensor with floating-point values.
 *
 * @return A light-weight object with each element set to true where @a x is NaN
 *         and false otherwise. This function does not create a new tensor,
 *         instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::isnan, Expr>
isnan(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::isnan, Expr>(x);
}

/**
 * @brief Return whether the sign of @a x is negative, element-wise.
 *
 * @param x An abstract tensor with floating-point or integer values.
 *
 * @return A light-weight object with each element set to true where @a x is
 *         negative and false otherwise. This function does not create a new
 *         tensor, instead, an expression object is returned (see
 *         lazy-evaluation).
 */
template <class Expr, class T, size_t Rank>
inline unary_expr<math::signbit, Expr>
signbit(const abstract_tensor<Expr, T, Rank> &x) {
  return unary_expr<math::signbit, Expr>(x);
}
} // namespace numcpp

#endif // NUMCPP_MATH_H_INCLUDED
