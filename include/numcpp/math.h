/*
 * File: include/numcpp/math.h
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

#ifndef NUMCPP_MATH_H_INCLUDED
#define NUMCPP_MATH_H_INCLUDED

#include "numcpp/expressions/unary_expr.h"
#include "numcpp/expressions/binary_expr.h"
#include "numcpp/math/constants.h"
#include "numcpp/math/degrees.h"
#include "numcpp/math/gcd.h"
#include "numcpp/math/mathfwd.h"

namespace numcpp {
/// Basic functions.
using std::abs;
using std::fmax;
using std::fmin;
using std::fmod;
using std::modf;

/// Trigonometric functions.
using std::acos;
using std::asin;
using std::atan;
using std::atan2;
using std::cos;
using std::hypot;
using std::sin;
using std::tan;

/// Hyperbolic functions.
using std::acosh;
using std::asinh;
using std::atanh;
using std::cosh;
using std::sinh;
using std::tanh;

/// Exponential and logarithmic functions.
using std::exp;
using std::exp2;
using std::expm1;
using std::log;
using std::log10;
using std::log1p;
using std::log2;

/// Power functions.
using std::cbrt;
using std::pow;
using std::sqrt;

/// Rounding.
using std::ceil;
using std::floor;
using std::round;
using std::trunc;

/// Floating-point manipulation functions.
using std::copysign;
using std::frexp;
using std::ldexp;
using std::nextafter;

/// Complex numbers.
using std::arg;
using std::conj;
using std::imag;
using std::real;

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
 * @return An abstract tensor with the absolute value of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto abs(Expr&& x)
    -> decltype(apply(math::abs(), std::forward<Expr>(x))) {
  return apply(math::abs(), std::forward<Expr>(x));
}

/**
 * @brief Return the floating-point remainder of @a x/y, element-wise. The
 * floating-point remainder of @a x/y is @a x-n*y, where @a n is the truncated
 * value (i.e., rounded towards zero) of @a x/y.
 *
 * @param x An abstract tensor with the values of the quotient numerator.
 * @param y An abstract tensor with the values of the quotient denominator.
 *
 * @return An abstract tensor with the remainder of @a x/y, element-wise. This
 * function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto fmod(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::fmod(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::fmod(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto fmod(Expr&& x, const T& y)
    -> decltype(apply2(math::fmod(), std::forward<Expr>(x), y)) {
  return apply2(math::fmod(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto fmod(const T& x, Expr&& y)
    -> decltype(apply2(math::fmod(), x, std::forward<Expr>(y))) {
  return apply2(math::fmod(), x, std::forward<Expr>(y));
}

/**
 * @brief Descompose @a x into integral and fractional parts, element-wise.
 *
 * @param x An abstract tensor with the values to be decomposed.
 *
 * @return An abstract tensor with the integral and fractional parts of each
 * element in the tensor. This function does not create a new tensor, instead,
 * an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto modf(Expr&& x)
    -> decltype(apply(math::modf(), std::forward<Expr>(x))) {
  return apply(math::modf(), std::forward<Expr>(x));
}

/**
 * @brief Return the maximum value, element-wise. If one of the elements being
 * compared is a NaN, then the non-NaN element is returned.
 *
 * @param x An abstract tensor with floating-point or integer values.
 * @param y An abstract tensor with floating-point or integer values.
 *
 * @return An abstract tensor with the element-wise maximum value. This function
 * does not create a new tensor, instead, an expression object is returned (see
 * lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto fmax(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::fmax(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::fmax(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto fmax(Expr&& x, const T& y)
    -> decltype(apply2(math::fmax(), std::forward<Expr>(x), y)) {
  return apply2(math::fmax(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto fmax(const T& x, Expr&& y)
    -> decltype(apply2(math::fmax(), x, std::forward<Expr>(y))) {
  return apply2(math::fmax(), x, std::forward<Expr>(y));
}

/**
 * @brief Return the minimum value, element-wise. If one of the elements being
 * compared is a NaN, then the non-NaN element is returned.
 *
 * @param x An abstract tensor with floating-point or integer values.
 * @param y An abstract tensor with floating-point or integer values.
 *
 * @return An abstract tensor with the element-wise minimum value. This function
 * does not create a new tensor, instead, an expression object is returned (see
 * lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto fmin(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::fmin(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::fmin(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto fmin(Expr&& x, const T& y)
    -> decltype(apply2(math::fmin(), std::forward<Expr>(x), y)) {
  return apply2(math::fmin(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto fmin(const T& x, Expr&& y)
    -> decltype(apply2(math::fmin(), x, std::forward<Expr>(y))) {
  return apply2(math::fmin(), x, std::forward<Expr>(y));
}

/// Trigonometric functions.

/**
 * @brief Return the cosine, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return An abstract tensor with the cosine of each element in the tensor.
 * This function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto cos(Expr&& x)
    -> decltype(apply(math::cos(), std::forward<Expr>(x))) {
  return apply(math::cos(), std::forward<Expr>(x));
}

/**
 * @brief Return the sine, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return An abstract tensor with the sine of each element in the tensor. This
 * function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto sin(Expr&& x)
    -> decltype(apply(math::sin(), std::forward<Expr>(x))) {
  return apply(math::sin(), std::forward<Expr>(x));
}

/**
 * @brief Return the tangent, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return An abstract tensor with the tangent of each element in the tensor.
 * This function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto tan(Expr&& x)
    -> decltype(apply(math::tan(), std::forward<Expr>(x))) {
  return apply(math::tan(), std::forward<Expr>(x));
}

/**
 * @brief Return the hypotenuse of a right-angled triangle whose legs are @a x
 * and @a y, element-wise.
 *
 * @param x An abstract tensor with one of the legs of the right-angle triangle.
 * @param y An abstract tensor with one of the legs of the right-angle triangle.
 *
 * @return An abstract tensor with the hypotenuse of the triangles,
 * element-wise. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto hypot(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::hypot(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::hypot(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto hypot(Expr&& x, const T& y)
    -> decltype(apply2(math::hypot(), std::forward<Expr>(x), y)) {
  return apply2(math::hypot(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto hypot(const T& x, Expr&& y)
    -> decltype(apply2(math::hypot(), x, std::forward<Expr>(y))) {
  return apply2(math::hypot(), x, std::forward<Expr>(y));
}

/**
 * @brief Return the principal value of the arc cosine, element-wise.
 *
 * @param x An abstract tensor with the values whose arc cosine is computed.
 *
 * @return An abstract tensor with the arc cosine, in radians, of each element
 * in the tensor. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto acos(Expr&& x)
    -> decltype(apply(math::acos(), std::forward<Expr>(x))) {
  return apply(math::acos(), std::forward<Expr>(x));
}

/**
 * @brief Return the principal value of the arc sine, element-wise.
 *
 * @param x An abstract tensor with the values whose arc sine is computed.
 *
 * @return An abstract tensor with the arc sine, in radians, of each element in
 * the tensor. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto asin(Expr&& x)
    -> decltype(apply(math::asin(), std::forward<Expr>(x))) {
  return apply(math::asin(), std::forward<Expr>(x));
}

/**
 * @brief Return the principal value of the arc tangent, element-wise.
 *
 * @param x An abstract tensor with the values whose arc tangent is computed.
 *
 * @return An abstract tensor with the arc tangent, in radians, of each element
 * in the tensor. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto atan(Expr&& x)
    -> decltype(apply(math::atan(), std::forward<Expr>(x))) {
  return apply(math::atan(), std::forward<Expr>(x));
}

/**
 * @brief Return the principal value of the arc tangent of @a y/x, element-wise.
 * To compute the value, the function takes into account the sign of both
 * arguments in order to determine the quadrant.
 *
 * @param y An abstract tensor with the @a y -coordinates.
 * @param x An abstract tensor with the @a x -coordinates.
 *
 * @return An abstract tensor with the arc tangent, in radians, of @a y/x ,
 * element-wise. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto atan2(LhsExpr&& y, RhsExpr&& x)
    -> decltype(apply2(math::atan2(), std::forward<LhsExpr>(y),
                       std::forward<RhsExpr>(x))) {
  return apply2(math::atan2(), std::forward<LhsExpr>(y),
                std::forward<RhsExpr>(x));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto atan2(Expr&& y, const T& x)
    -> decltype(apply2(math::atan2(), std::forward<Expr>(y), x)) {
  return apply2(math::atan2(), std::forward<Expr>(y), x);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto atan2(const T& y, Expr&& x)
    -> decltype(apply2(math::atan2(), y, std::forward<Expr>(x))) {
  return apply2(math::atan2(), y, std::forward<Expr>(x));
}

/**
 * @brief Convert angles from radians to degrees, element-wise.
 *
 * @param x An abstract tensor with the angles in radians.
 *
 * @return An abstract tensor with the corresponding angle in degrees for each
 * element in the tensor. This function does not create a new tensor, instead,
 * an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto degrees(Expr&& x)
    -> decltype(apply(math::degrees(), std::forward<Expr>(x))) {
  return apply(math::degrees(), std::forward<Expr>(x));
}

/**
 * @brief Convert angles from degrees to radians, element-wise.
 *
 * @param x An abstract tensor with the angles in degrees.
 *
 * @return An abstract tensor with the corresponding angle in radians for each
 * element in the tensor. This function does not create a new tensor, instead,
 * an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto radians(Expr&& x)
    -> decltype(apply(math::radians(), std::forward<Expr>(x))) {
  return apply(math::radians(), std::forward<Expr>(x));
}

/// Hyperbolic functions.

/**
 * @brief Return the hyperbolic cosine, element-wise.
 *
 * @param x An abstract tensor with the hyperbolic angles.
 *
 * @return An abstract tensor with the hyperbolic cosine of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto cosh(Expr&& x)
    -> decltype(apply(math::cosh(), std::forward<Expr>(x))) {
  return apply(math::cosh(), std::forward<Expr>(x));
}

/**
 * @brief Return the hyperbolic sine, element-wise.
 *
 * @param x An abstract tensor with the hyperbolic angles.
 *
 * @return An abstract tensor with the hyperbolic sine of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto sinh(Expr&& x)
    -> decltype(apply(math::sinh(), std::forward<Expr>(x))) {
  return apply(math::sinh(), std::forward<Expr>(x));
}

/**
 * @brief Return the hyperbolic tangent, element-wise.
 *
 * @param x An abstract tensor with the hyperbolic angles.
 *
 * @return An abstract tensor with the hyperbolic tangent of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto tanh(Expr&& x)
    -> decltype(apply(math::tanh(), std::forward<Expr>(x))) {
  return apply(math::tanh(), std::forward<Expr>(x));
}

/**
 * @brief Return the inverse hyperbolic cosine, element-wise.
 *
 * @param x An abstract tensor with the values whose inverse hyperbolic cosine
 * is computed.
 *
 * @return An abstract tensor with the inverse hyperbolic cosine of each element
 * in the tensor. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto acosh(Expr&& x)
    -> decltype(apply(math::acosh(), std::forward<Expr>(x))) {
  return apply(math::acosh(), std::forward<Expr>(x));
}

/**
 * @brief Return the inverse hyperbolic sine, element-wise.
 *
 * @param x An abstract tensor with the values whose inverse hyperbolic sine is
 * computed.
 *
 * @return An abstract tensor with the inverse hyperbolic sine of each element
 * in the tensor. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto asinh(Expr&& x)
    -> decltype(apply(math::asinh(), std::forward<Expr>(x))) {
  return apply(math::asinh(), std::forward<Expr>(x));
}

/**
 * @brief Return the inverse hyperbolic tangent, element-wise.
 *
 * @param x An abstract tensor with the values whose inverse hyperbolic tangent
 * is computed.
 *
 * @return An abstract tensor with the inverse hyperbolic tangent of each
 * element in the tensor. This function does not create a new tensor, instead,
 * an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto atanh(Expr&& x)
    -> decltype(apply(math::atanh(), std::forward<Expr>(x))) {
  return apply(math::atanh(), std::forward<Expr>(x));
}

/// Exponential and logarithmic functions.

/**
 * @brief Return the base-e exponential, which is @a e raised to the power @a x:
 * @a e^x, element-wise.
 *
 * @param x An abstract tensor with the values of the exponent.
 *
 * @return An abstract tensor with the exponential of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto exp(Expr&& x)
    -> decltype(apply(math::exp(), std::forward<Expr>(x))) {
  return apply(math::exp(), std::forward<Expr>(x));
}

/**
 * @brief Return the natural logarithm, which is the inverse of the exponential
 * function (exp), element-wise.
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return An abstract tensor with the natural logarithm of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto log(Expr&& x)
    -> decltype(apply(math::log(), std::forward<Expr>(x))) {
  return apply(math::log(), std::forward<Expr>(x));
}

/**
 * @brief Return the common (base-10) logarithm, element-wise.
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return An abstract tensor with the common logarithm of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto log10(Expr&& x)
    -> decltype(apply(math::log10(), std::forward<Expr>(x))) {
  return apply(math::log10(), std::forward<Expr>(x));
}

/**
 * @brief Return the base-2 exponential, which is 2 raised to the power @a x:
 * @a 2^x, element-wise.
 *
 * @param x An abstract tensor with the values of the exponent.
 *
 * @return An abstract tensor with the base-2 exponential of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto exp2(Expr&& x)
    -> decltype(apply(math::exp2(), std::forward<Expr>(x))) {
  return apply(math::exp2(), std::forward<Expr>(x));
}

/**
 * @brief Return the binary (base-2) logarithm, element-wise.
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return An abstract tensor with the binary logarithm of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto log2(Expr&& x)
    -> decltype(apply(math::log2(), std::forward<Expr>(x))) {
  return apply(math::log2(), std::forward<Expr>(x));
}

/**
 * @brief Return @a e raised to the power @a x minus one: @a e^x - 1,
 * element-wise. For values of @a x close to zero, expm1 is more accurate than
 * exp(x) - 1.
 *
 * @param x An abstract tensor with the values of the exponent.
 *
 * @return An abstract tensor with the exponential minus one of each element in
 * the tensor. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto expm1(Expr&& x)
    -> decltype(apply(math::expm1(), std::forward<Expr>(x))) {
  return apply(math::expm1(), std::forward<Expr>(x));
}

/**
 * @brief Return the natural logarithm of one plus @a x, element-wise. For
 * values of @a x close to zero, log1p is more accurate than log(1 + x).
 *
 * @param x An abstract tensor with the values whose logarithm is computed.
 *
 * @return An abstract tensor with the natural logarithm of (1 + x) of each
 * element in the tensor. This function does not create a new tensor, instead,
 * an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto log1p(Expr&& x)
    -> decltype(apply(math::log1p(), std::forward<Expr>(x))) {
  return apply(math::log1p(), std::forward<Expr>(x));
}

/// Power functions.

/**
 * @brief Return @a x raised to the power @a y, element-wise.
 *
 * @param x An abstract tensor with the values of the base.
 * @param y An abstract tensor with the values of the exponent.
 *
 * @return An abstract tensor with the result of raising @a x to the power @a y,
 * element-wise. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto pow(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::pow(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::pow(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto pow(Expr&& x, const T& y)
    -> decltype(apply2(math::pow(), std::forward<Expr>(x), y)) {
  return apply2(math::pow(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto pow(const T& x, Expr&& y)
    -> decltype(apply2(math::pow(), x, std::forward<Expr>(y))) {
  return apply2(math::pow(), x, std::forward<Expr>(y));
}

/**
 * @brief Return the square root, element-wise.
 *
 * @param x An abstract tensor with the values whose square root is computed.
 *
 * @return An abstract tensor with the square root of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto sqrt(Expr&& x)
    -> decltype(apply(math::sqrt(), std::forward<Expr>(x))) {
  return apply(math::sqrt(), std::forward<Expr>(x));
}

/**
 * @brief Return the cubic root, element-wise.
 *
 * @param x An abstract tensor with the values whose cubic root is computed.
 *
 * @return An abstract tensor with the cubic root of each element in the tensor.
 * This function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto cbrt(Expr&& x)
    -> decltype(apply(math::cbrt(), std::forward<Expr>(x))) {
  return apply(math::cbrt(), std::forward<Expr>(x));
}

/// Rounding.

/**
 * @brief Rounds @a x upward. Return the smallest integral value that is not
 * less than @a x, element-wise.
 *
 * @param x An abstract tensor with the values to round up.
 *
 * @return An abstract tensor with the ceiling of each element in the tensor.
 * This function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto ceil(Expr&& x)
    -> decltype(apply(math::ceil(), std::forward<Expr>(x))) {
  return apply(math::ceil(), std::forward<Expr>(x));
}

/**
 * @brief Rounds @a x downward. Return the largest integral value that is not
 * greater than @a x, element-wise.
 *
 * @param x An abstract tensor with the values to round down.
 *
 * @return An abstract tensor with the floor of each element in the tensor. This
 * function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto floor(Expr&& x)
    -> decltype(apply(math::floor(), std::forward<Expr>(x))) {
  return apply(math::floor(), std::forward<Expr>(x));
}

/**
 * @brief Rounds @a x toward zero. Return the nearest integral value that is not
 * larger in magnitude than @a x, element-wise.
 *
 * @param x An abstract tensor with the values to truncate.
 *
 * @return An abstract tensor with the truncated value of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto trunc(Expr&& x)
    -> decltype(apply(math::trunc(), std::forward<Expr>(x))) {
  return apply(math::trunc(), std::forward<Expr>(x));
}

/**
 * @brief Return the integral value that is nearest to @a x, element-wise.
 * Halfway cases are rounded away from zero.
 *
 * @param x An abstract tensor with the values to round.
 *
 * @return An abstract tensor with the rounded value of each element in the
 * tensor. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto round(Expr&& x)
    -> decltype(apply(math::round(), std::forward<Expr>(x))) {
  return apply(math::round(), std::forward<Expr>(x));
}

/// Floating-point manipulation functions.

/**
 * @brief Descompose @a x into its binary significand and an integral power of
 * two, such that @a x = significand * 2^exponent, element-wise.
 *
 * @param x An abstract tensor with the values to be decomposed.
 *
 * @return An abstract tensor with the binary significand and the exponent of
 * each element in the tensor. This function does not create a new tensor,
 * instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto frexp(Expr&& x)
    -> decltype(apply(math::frexp(), std::forward<Expr>(x))) {
  return apply(math::frexp(), std::forward<Expr>(x));
}

/**
 * @brief Return the result of multiplying @a x (the significand) by 2 raised to
 * the power of @a exp (the exponent), element-wise.
 *
 * @param x An abstract tensor with the values of the significand.
 * @param exp An abstract tensor with the values of the exponent.
 *
 * @return An abstract tensor with the result of @a x*2^exp, element-wise. This
 * function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto ldexp(LhsExpr&& x, RhsExpr&& exp)
    -> decltype(apply2(math::ldexp(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(exp))) {
  return apply2(math::ldexp(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(exp));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto ldexp(Expr&& x, const T& exp)
    -> decltype(apply2(math::ldexp(), std::forward<Expr>(x), exp)) {
  return apply2(math::ldexp(), std::forward<Expr>(x), exp);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto ldexp(const T& x, Expr&& exp)
    -> decltype(apply2(math::ldexp(), x, std::forward<Expr>(exp))) {
  return apply2(math::ldexp(), x, std::forward<Expr>(exp));
}

/**
 * @brief Return a value with the magnitude of @a x and the sign of @a y,
 * element-wise.
 *
 * @param x An abstract tensor with the values to change the sign of.
 * @param y An abstract tensor with the values to copy the sign from.
 *
 * @return An abstract tensor with the values from the first tensor and the
 * signs from the second tensor. This function does not create a new tensor,
 * instead, an expression object is returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto copysign(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::copysign(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::copysign(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto copysign(Expr&& x, const T& y)
    -> decltype(apply2(math::copysign(), std::forward<Expr>(x), y)) {
  return apply2(math::copysign(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto copysign(const T& x, Expr&& y)
    -> decltype(apply2(math::copysign(), x, std::forward<Expr>(y))) {
  return apply2(math::copysign(), x, std::forward<Expr>(y));
}

/**
 * @brief Return the next representable value after @a x in the direction of
 * @a y, element-wise.
 *
 * @param x An abstract tensor with the base values.
 * @param y An abstract tensor with the directions where to look for the next
 * representable values.
 *
 * @return An abstract tensor with the next representable value of each element
 * in the first tensor in the direction of the second tensor. This function does
 * not create a new tensor, instead, an expression object is returned (see
 * lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto nextafter(LhsExpr&& x, RhsExpr&& y)
    -> decltype(apply2(math::nextafter(), std::forward<LhsExpr>(x),
                       std::forward<RhsExpr>(y))) {
  return apply2(math::nextafter(), std::forward<LhsExpr>(x),
                std::forward<RhsExpr>(y));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto nextafter(Expr&& x, const T& y)
    -> decltype(apply2(math::nextafter(), std::forward<Expr>(x), y)) {
  return apply2(math::nextafter(), std::forward<Expr>(x), y);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto nextafter(const T& x, Expr&& y)
    -> decltype(apply2(math::nextafter(), x, std::forward<Expr>(y))) {
  return apply2(math::nextafter(), x, std::forward<Expr>(y));
}

/// Integer-valued functions.

/**
 * @brief Return the greatest common divisor, element-wise.
 *
 * @param m An abstract tensor with integer values.
 * @param n An abstract tensor with integer values.
 *
 * @return An abstract tensor with the greatest common divisor of @a |m| and
 * @a |n|, element-wise. If both @a m and @a n are zero, return zero. This
 * function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto gcd(LhsExpr&& m, RhsExpr&& n)
    -> decltype(apply2(math::gcd(), std::forward<LhsExpr>(m),
                       std::forward<RhsExpr>(n))) {
  return apply2(math::gcd(), std::forward<LhsExpr>(m),
                std::forward<RhsExpr>(n));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto gcd(Expr&& m, const T& n)
    -> decltype(apply2(math::gcd(), std::forward<Expr>(m), n)) {
  return apply2(math::gcd(), std::forward<Expr>(m), n);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto gcd(const T& m, Expr&& n)
    -> decltype(apply2(math::gcd(), m, std::forward<Expr>(n))) {
  return apply2(math::gcd(), m, std::forward<Expr>(n));
}

/**
 * @brief Return the least common multiple, element-wise.
 *
 * @param m An abstract tensor with integer values.
 * @param n An abstract tensor with integer values.
 *
 * @return An abstract tensor with the least common multiple of @a |m| and
 * @a |n|, element-wise. If either @a m or @a n is zero, return zero. This
 * function does not create a new tensor, instead, an expression object is
 * returned (see lazy-evaluation).
 */
template <class LhsExpr, class RhsExpr, detail::RequiresTensor<LhsExpr> = 0,
          detail::RequiresTensor<RhsExpr> = 0>
inline auto lcm(LhsExpr&& m, RhsExpr&& n)
    -> decltype(apply2(math::lcm(), std::forward<LhsExpr>(m),
                       std::forward<RhsExpr>(n))) {
  return apply2(math::lcm(), std::forward<LhsExpr>(m),
                std::forward<RhsExpr>(n));
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto lcm(Expr&& m, const T& n)
    -> decltype(apply2(math::lcm(), std::forward<Expr>(m), n)) {
  return apply2(math::lcm(), std::forward<Expr>(m), n);
}

template <class Expr, class T, detail::RequiresTensor<Expr> = 0,
          detail::RequiresScalar<T> = 0>
inline auto lcm(const T& m, Expr&& n)
    -> decltype(apply2(math::lcm(), m, std::forward<Expr>(n))) {
  return apply2(math::lcm(), m, std::forward<Expr>(n));
}

/// Complex numbers.

/**
 * @brief Return the real part, element-wise.
 *
 * @param z An abstract tensor with complex values.
 *
 * @return An abstract tensor with the real part of each element in the tensor.
 * Non-complex types are treated as complex numbers with zero imaginary part
 * component. This function does not create a new tensor, instead, an expression
 * object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto real(Expr&& z)
    -> decltype(apply(math::real(), std::forward<Expr>(z))) {
  return apply(math::real(), std::forward<Expr>(z));
}

template <class T, size_t Rank>
inline tensor_view<T, Rank> real(tensor<std::complex<T>, Rank>& z) {
  T* data = reinterpret_cast<T*>(z.data());
  shape_t<Rank> shape = z.shape();
  index_t<Rank> strides = make_strides(shape, z.layout());
  for (size_t i = 0; i < Rank; ++i) {
    strides[i] *= 2;
  }
  return tensor_view<T, Rank>(data, shape, strides);
}

template <class T, size_t Rank>
inline tensor_view<T, Rank> real(tensor_view<std::complex<T>, Rank>& z) {
  T* data = reinterpret_cast<T*>(z.data());
  shape_t<Rank> shape = z.shape();
  index_t<Rank> strides = z.strides();
  for (size_t i = 0; i < Rank; ++i) {
    strides[i] *= 2;
  }
  return tensor_view<T, Rank>(data, shape, strides);
}

/**
 * @brief Return the imaginary part, element-wise.
 *
 * @param z An abstract tensor with complex values.
 *
 * @return An abstract tensor with the imaginary part of each element in the
 * tensor. Non-complex types are treated as complex numbers with zero imaginary
 * part component. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto imag(Expr&& z)
    -> decltype(apply(math::imag(), std::forward<Expr>(z))) {
  return apply(math::imag(), std::forward<Expr>(z));
}

template <class T, size_t Rank>
inline tensor_view<T, Rank> imag(tensor<std::complex<T>, Rank>& z) {
  T* data = reinterpret_cast<T*>(z.data());
  shape_t<Rank> shape = z.shape();
  index_t<Rank> strides = make_strides(shape, z.layout());
  for (size_t i = 0; i < Rank; ++i) {
    strides[i] *= 2;
  }
  return tensor_view<T, Rank>(data + 1, shape, strides);
}

template <class T, size_t Rank>
inline tensor_view<T, Rank> imag(tensor_view<std::complex<T>, Rank>& z) {
  T* data = reinterpret_cast<T*>(z.data());
  shape_t<Rank> shape = z.shape();
  index_t<Rank> strides = z.strides();
  for (size_t i = 0; i < Rank; ++i) {
    strides[i] *= 2;
  }
  return tensor_view<T, Rank>(data + 1, shape, strides);
}

/**
 * @brief Return the complex conjugate, element-wise. The conjugate of a complex
 * number is obtained by changing the sign of its imaginary part.
 *
 * @param z An abstract tensor with the values whose complex conjugate is
 * computed.
 *
 * @return An abstract tensor with the complex conjugate of each element in the
 * tensor. Non-complex types are treated as complex numbers with zero imaginary
 * part component. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto conj(Expr&& z)
    -> decltype(apply(math::conj(), std::forward<Expr>(z))) {
  return apply(math::conj(), std::forward<Expr>(z));
}

/**
 * @brief Return the phase angle (in radians) of a complex number, element-wise.
 *
 * @param z An abstract tensor with the values whose phase angle is computed.
 *
 * @return An abstract tensor with the phase angle of each element in the
 * tensor. Non-complex types are treated as complex numbers with zero imaginary
 * part component. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto arg(Expr&& z)
    -> decltype(apply(math::arg(), std::forward<Expr>(z))) {
  return apply(math::arg(), std::forward<Expr>(z));
}

/// Clasification functions.

/**
 * @brief Return whether @a x is a finite value (neither infinite nor NaN),
 * element-wise.
 *
 * @param x An abstract tensor with floating-point values.
 *
 * @return An abstract tensor with each element set to true where @a x is finite
 * and false otherwise. This function does not create a new tensor, instead, an
 * expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto isfinite(Expr&& x)
    -> decltype(apply(math::isfinite(), std::forward<Expr>(x))) {
  return apply(math::isfinite(), std::forward<Expr>(x));
}

/**
 * @brief Return whether @a x is an infinity value (either positive infinity
 * or negative infinity), element-wise.
 *
 * @param x An abstract tensor with floating-point values.
 *
 * @return An abstract tensor with each element set to true where @a x is
 * infinity and false otherwise. This function does not create a new tensor,
 * instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto isinf(Expr&& x)
    -> decltype(apply(math::isinf(), std::forward<Expr>(x))) {
  return apply(math::isinf(), std::forward<Expr>(x));
}

/**
 * @brief Return whether @a x is a NaN (Not a Number) value, element-wise. The
 * NaN values are used to identify undefined or non-representable values for
 * floating-point numbers, such as the square root of negative numbers or the
 * result of 0/0.
 *
 * @param x An abstract tensor with floating-point values.
 *
 * @return An abstract tensor with each element set to true where @a x is
 * NaN and false otherwise. This function does not create a new tensor, instead,
 * an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto isnan(Expr&& x)
    -> decltype(apply(math::isnan(), std::forward<Expr>(x))) {
  return apply(math::isnan(), std::forward<Expr>(x));
}

/**
 * @brief Return whether the sign of @a x is negative, element-wise.
 *
 * @param x An abstract tensor with floating-point or integer values.
 *
 * @return An abstract tensor with each element set to true where @a x is
 * negative and false otherwise. This function does not create a new tensor,
 * instead, an expression object is returned (see lazy-evaluation).
 */
template <class Expr, detail::RequiresTensor<Expr> = 0>
inline auto signbit(Expr&& x)
    -> decltype(apply(math::signbit(), std::forward<Expr>(x))) {
  return apply(math::signbit(), std::forward<Expr>(x));
}
}  // namespace numcpp

#endif  // NUMCPP_MATH_H_INCLUDED
