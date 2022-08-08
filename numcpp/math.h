/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
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

#include "numcpp/tensor.h"
#include "numcpp/math/constants.h"
#include "numcpp/math/mathfwd.h"

namespace numcpp {
    /// Basic functions.
    using std::abs;
    using std::fmod;
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

    /// Basic functions.

    /**
     * @brief Return the absolute value, element-wise.
     *
     * @param x A tensor-like object with the values whose absolute value is
     *     computed.
     *
     * @return A light-weight object with the absolute value of each element in
     *     the tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::abs, T, Tag> >
    abs(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::abs, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::abs(), x);
    }

    /**
     * @brief Return the floating-point remainder of x/y, element-wise. The
     * floating-point remainder of x/y is x - n*y, where n is the truncated
     * value (i.e., rounded towards zero) of x/y.
     *
     * @param x A tensor-like object with the values of the quotient numerator.
     * @param y A tensor-like object with the values of the quotient
     *     denominator.
     *
     * @return A light-weight object with the remainder of x/y, element-wise.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::fmod, T, Tag1, T, Tag2> >
    fmod(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::fmod, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::fmod(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::fmod, T, Tag, T, scalar_tag>
    > fmod(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::fmod, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::fmod(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::fmod, T, scalar_tag, T, Tag>
    > fmod(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::fmod, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::fmod(), x, y);
    }

    /**
     * @brief Return the maximum value, element-wise. If one of the elements
     * being compared is a NaN, then the non-NaN element is returned.
     *
     * @param x A tensor-like object with floating point or integral values.
     * @param y A tensor-like object with floating point or integral values.
     *
     * @return A light-weight object with the element-wise maximum. This
     *     function does not create a new tensor, instead, an expression object
     *     is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::fmax, T, Tag1, T, Tag2> >
    fmax(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::fmax, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::fmax(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::fmax, T, Tag, T, scalar_tag>
    > fmax(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::fmax, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::fmax(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::fmax, T, scalar_tag, T, Tag>
    > fmax(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::fmax, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::fmax(), x, y);
    }

    /**
     * @brief Return the minimum value, element-wise. If one of the elements
     * being compared is a NaN, then the non-NaN element is returned.
     *
     * @param x A tensor-like object with floating point or integral values.
     * @param y A tensor-like object with floating point or integral values.
     *
     * @return A light-weight object with the element-wise minimum. This
     *     function does not create a new tensor, instead, an expression object
     *     is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::fmin, T, Tag1, T, Tag2> >
    fmin(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::fmin, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::fmin(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::fmin, T, Tag, T, scalar_tag>
    > fmin(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::fmin, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::fmin(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::fmin, T, scalar_tag, T, Tag>
    > fmin(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::fmin, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::fmin(), x, y);
    }

    /// Trigonometric functions.

    /**
     * @brief Return the cosine, element-wise.
     *
     * @param x A tensor-like object with the angles in radians.
     *
     * @return A light-weight object with the cosine of each element in the
     *     tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::cos, T, Tag> >
    cos(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::cos, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::cos(), x);
    }

    /**
     * @brief Return the sine, element-wise.
     *
     * @param x A tensor-like object with the angles in radians.
     *
     * @return A light-weight object with the sine of each element in the
     *     tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::sin, T, Tag> >
    sin(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::sin, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::sin(), x);
    }

    /**
     * @brief Return the tangent, element-wise.
     *
     * @param x A tensor-like object with the angles in radians.
     *
     * @return A light-weight object with the tangent of each element in the
     *     tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::tan, T, Tag> >
    tan(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::tan, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::tan(), x);
    }

    /**
     * @brief Return the hypotenuse of a right-angled triangle whose legs are x
     * and y, element-wise.
     *
     * @param x A tensor-like object with one of the legs of the right-angle
     *     triangles.
     * @param y A tensor-like object with one of the legs of the right-angle
     *     triangles.
     *
     * @return A light-weight object with the hypotenuse of the triangles.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::hypot, T, Tag1, T, Tag2> >
    hypot(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::hypot, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::hypot(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::hypot, T, Tag, T, scalar_tag>
    > hypot(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::hypot, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::hypot(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::hypot, T, scalar_tag, T, Tag>
    > hypot(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::hypot, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::hypot(), x, y);
    }

    /**
     * @brief Return the principal value of the arc cosine, element-wise.
     *
     * @param x A tensor-like object with the values whose arc cosine is
     *     computed.
     *
     * @return A light-weight object with the arc cosine, in radians, of each
     *     value in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::acos, T, Tag> >
    acos(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::acos, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::acos(), x);
    }

    /**
     * @brief Return the principal value of the arc sine, element-wise.
     *
     * @param x A tensor-like object with the values whose arc sine is
     *     computed.
     *
     * @return A light-weight object with the arc sine, in radians, of each
     *     value in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::asin, T, Tag> >
    asin(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::asin, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::asin(), x);
    }

    /**
     * @brief Return the principal value of the arc tangent, element-wise.
     *
     * @param x A tensor-like object with the values whose arc tangent is
     *     computed.
     *
     * @return A light-weight object with the arc tangent, in radians, of each
     *     value in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::atan, T, Tag> >
    atan(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::atan, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::atan(), x);
    }

    /**
     * @brief Return the principal value of the arc tangent of y/x,
     * element-wise. To compute the value, the function takes into account the
     * sign of both arguments in order to determine the quadrant.
     *
     * @param y A tensor-like object with the y-coordinates.
     * @param x A tensor-like object with the x-coordinates.
     *
     * @return A light-weight object with the arc tangent, in radians, of y/x.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::atan2, T, Tag1, T, Tag2> >
    atan2(
        const base_tensor<T, Rank, Tag1> &y,
        const base_tensor<T, Rank, Tag2> &x
    ) {
        typedef lazy_binary_tag<math::atan2, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::atan2(), y, x);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::atan2, T, Tag, T, scalar_tag>
    > atan2(
        const base_tensor<T, Rank, Tag> &y,
        const typename tensor<T, Rank>::value_type &x
    ) {
        typedef lazy_binary_tag<math::atan2, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::atan2(), y, x);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::atan2, T, scalar_tag, T, Tag>
    > atan2(
        const typename tensor<T, Rank>::value_type &y,
        const base_tensor<T, Rank, Tag> &x
    ) {
        typedef lazy_binary_tag<math::atan2, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::atan2(), y, x);
    }

    /**
     * @brief Convert angles from radians to degrees, element-wise.
     *
     * @param x A tensor-like object with the angles in radians.
     *
     * @return A light-weight object with the corresponding angles in degrees.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::degrees, T, Tag> >
    degrees(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::degrees, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::degrees(), x);
    }

    /**
     * @brief Convert angles from degrees to radians, element-wise.
     *
     * @param x A tensor-like object with the angles in degrees.
     *
     * @return A light-weight object with the corresponding angles in radians.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::radians, T, Tag> >
    radians(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::radians, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::radians(), x);
    }

    /// Hyperbolic functions.

    /**
     * @brief Return the hyperbolic cosine, element-wise.
     *
     * @param x A tensor-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic cosine of each element
     *     in the tensor. This function does not create a new tensor, instead,
     *     an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::cosh, T, Tag> >
    cosh(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::cosh, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::cosh(), x);
    }

    /**
     * @brief Return the hyperbolic sine, element-wise.
     *
     * @param x A tensor-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic sine of each element
     *     in the tensor. This function does not create a new tensor, instead,
     *     an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::sinh, T, Tag> >
    sinh(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::sinh, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::sinh(), x);
    }

    /**
     * @brief Return the hyperbolic tangent, element-wise.
     *
     * @param x A tensor-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic tangent of each
     *     element in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::tanh, T, Tag> >
    tanh(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::tanh, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::tanh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic cosine, element-wise.
     *
     * @param x A tensor-like object with the values whose inverse hyperbolic
     *     cosine is computed.
     *
     * @return A light-weight object with the inverse hyperbolic cosine of each
     *     element in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::acosh, T, Tag> >
    acosh(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::acosh, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::acosh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic sine, element-wise.
     *
     * @param x A tensor-like object with the values whose inverse hyperbolic
     *     sine is computed.
     *
     * @return A light-weight object with the inverse hyperbolic sine of each
     *     element in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::asinh, T, Tag> >
    asinh(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::asinh, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::asinh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic tangent, element-wise.
     *
     * @param x A tensor-like object with the values whose inverse hyperbolic
     *     tangent is computed.
     *
     * @return A light-weight object with the inverse hyperbolic tangent of
     *     each element in the tensor. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::atanh, T, Tag> >
    atanh(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::atanh, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::atanh(), x);
    }

    /// Exponential and logarithmic functions.

    /**
     * @brief Return the base-e exponential, which is e raised to the power x:
     * e^x, element-wise.
     *
     * @param x A tensor-like object with the values of the exponents.
     *
     * @return A light-weight object with the exponential of each element in
     *     the tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::exp, T, Tag> >
    exp(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::exp, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::exp(), x);
    }

    /**
     * @brief Return the natural logarithm, which is the inverse of the
     * exponential function (exp), element-wise.
     *
     * @param x A tensor-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light weight-object with the natural logarithm of each element
     *     in the tensor. This function does not create a new tensor, instead,
     *     an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::log, T, Tag> >
    log(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::log, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::log(), x);
    }

    /**
     * @brief Return the common (base-10) logarithm, element-wise.
     *
     * @param x A tensor-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the common logarithm of each element
     *     in the tensor. This function does not create a new tensor, instead,
     *     an  expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::log10, T, Tag> >
    log10(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::log10, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::log10(), x);
    }

    /**
     * @brief Return the base-2 exponential, which is 2 raised to the power x:
     * 2^x, element-wise.
     *
     * @param x A tensor-like object with the values of the exponents.
     *
     * @return A light-weight object with the base-2 exponential of each
     *     element in the tensor. This function does not create a new tensor,
     *     instead, an  expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::exp2, T, Tag> >
    exp2(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::exp2, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::exp2(), x);
    }

    /**
     * @brief Return the binary (base-2) logarithm, element-wise.
     *
     * @param x A tensor-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the binary logarithm of each element
     *     in the tensor. This function does not create a new tensor, instead,
     *     an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::log2, T, Tag> >
    log2(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::log2, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::log2(), x);
    }

    /**
     * @brief Return e raised to the power x minus one: e^x - 1, element-wise.
     * For values of x close to zero, expm1 is more accurate than exp(x) - 1.
     *
     * @param x A tensor-like object with the values of the exponent.
     *
     * @return A light-weight object with the exponential minus one of each
     *     element in the tensor. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::expm1, T, Tag> >
    expm1(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::expm1, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::expm1(), x);
    }

    /**
     * @brief Return the natural logarithm of one plus x, element-wise. For
     * values of x close to zero, log1p is more accurate than log(1 + x).
     *
     * @param x A tensor-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the natural logarithm of (1 + x) for
     *     each element in the tensor. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::log1p, T, Tag> >
    log1p(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::log1p, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::log1p(), x);
    }

    /// Power functions.

    /**
     * @brief Return x raised to the power y, element-wise.
     *
     * @param x A tensor-like object with the values of the base.
     * @param y A tensor-like object with the values of the exponent.
     *
     * @return A light-weight object with the result of rasing x to the power
     *     y, element-wise. This function does not create a new tensor,
     *     instead, an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::pow, T, Tag1, T, Tag2> >
    pow(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::pow, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::pow(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::pow, T, Tag, T, scalar_tag>
    > pow(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::pow, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::pow(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::pow, T, scalar_tag, T, Tag>
    > pow(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::pow, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::pow(), x, y);
    }

    /**
     * @brief Return the square root, element-wise.
     *
     * @param x A tensor-like object with the values whose square root is
     *     computed.
     *
     * @return A light-weight object with the square root of each element in
     *     the tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::sqrt, T, Tag>
    > sqrt(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::sqrt, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::sqrt(), x);
    }

    /**
     * @brief Return the cubic root, element-wise.
     *
     * @param x A tensor-like object with the values whose cubic root is
     *     computed.
     *
     * @return A light-weight object with the cubic root of each element in the
     *     tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::cbrt, T, Tag> >
    cbrt(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::cbrt, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::cbrt(), x);
    }

    /// Rounding.

    /**
     * @brief Rounds x upward. Return the smallest integral value that is not
     * less than x, element-wise.
     *
     * @param x A tensor-like object with the values to round up.
     *
     * @return A light-weight object with the ceiling of each element in the
     *     tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::ceil, T, Tag> >
    ceil(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::ceil, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::ceil(), x);
    }

    /**
     * @brief Rounds x downward. Return the largest integral value that is not
     * greater than x, element-wise.
     *
     * @param x A tensor-like object with the values to round down.
     *
     * @return A light-weight object with the floor of each element in the
     *     tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::floor, T, Tag> >
    floor(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::floor, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::floor(), x);
    }

    /**
     * @brief Rounds x toward zero. Return the nearest integral value that is
     * not larger in magnitude than x, element-wise.
     *
     * @param x A tensor-like object with the values to truncate.
     *
     * @return A light-weight object with the truncated value of each element
     *     in the tensor. This function does not create a new tensor, instead,
     *     an expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::trunc, T, Tag> >
    trunc(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::trunc, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::trunc(), x);
    }

    /**
     * @brief Return the integral value that is nearest to x, element-wise.
     * Halfway cases are rounded away from zero.
     *
     * @param x A tensor-like object with the values to round.
     *
     * @return A light-weight object with the rounded value of each element in
     *     the tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::round, T, Tag> >
    round(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::round, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::round(), x);
    }

    /// Floating-point manipulation functions.

    /**
     * @brief Descompose x into its binary significand and an integral power of
     * two, such that x = significand * 2^exponent, element-wise.
     *
     * @param x A tensor-like object with the values to be decomposed.
     *
     * @return A light-weight object with the binary significand and the
     *     exponent of each element in the tensor. This function does not
     *     create a new tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        std::pair<T, int>, Rank, lazy_unary_tag<math::frexp, T, Tag>
    > frexp(const base_tensor<T, Rank, Tag> &x) {
        typedef std::pair<T, int> Rt;
        typedef lazy_unary_tag<math::frexp, T, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(math::frexp(), x);
    }

    /**
     * @brief Return the result of multiplying x (the significand) by 2 raised
     * to the power of exp (the exponent), element-wise.
     *
     * @param x A tensor-like object with the values of the significand.
     * @param exp A tensor-like object with the values of the exponent.
     *
     * @return A light-weight object with the result of x*2^exp, element-wise.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::ldexp, T, Tag1, int, Tag2>
    > ldexp(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<int, Rank, Tag2> &exp
    ) {
        typedef lazy_binary_tag<math::ldexp, T, Tag1, int, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::ldexp(), x, exp);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::ldexp, T, Tag, int, scalar_tag>
    > ldexp(const base_tensor<T, Rank, Tag> &x, int exp) {
        typedef lazy_binary_tag<math::ldexp, T, Tag, int, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::ldexp(), x, exp);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::ldexp, T, scalar_tag, int, Tag>
    > ldexp(const T &x, const base_tensor<int, Rank, Tag> &exp) {
        typedef lazy_binary_tag<math::ldexp, T, scalar_tag, int, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::ldexp(), x, exp);
    }

    /**
     * @brief Return a value with the magnitude of x and the sign of y,
     * element-wise.
     *
     * @param x A tensor-like object with the values to change the sign of.
     * @param y A tensor-like object with the values to copy the sign from.
     *
     * @return A light-weight object with the values in the first tensor and
     *     the signs in the second tensor. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::copysign, T, Tag1, T, Tag2>
    > copysign(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::copysign, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::copysign(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::copysign, T, scalar_tag, T, Tag>
    > copysign(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::copysign, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::copysign(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::copysign, T, Tag, T, scalar_tag>
    > copysign(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::copysign, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::copysign(), x, y);
    }

    /**
     * @brief Return the next representable value after x in the direction of
     * y, element-wise.
     *
     * @param x A tensor-like object with the base values.
     * @param y A tensor-like object with the directions where to look for the
     *     next representable values.
     *
     * @return A light-weight object with the next representable value of each
     *     element in the first tensor in the direction of the second tensor.
     *     This function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::nextafter, T, Tag1, T, Tag2>
    > nextafter(
        const base_tensor<T, Rank, Tag1> &x,
        const base_tensor<T, Rank, Tag2> &y
    ) {
        typedef lazy_binary_tag<math::nextafter, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::nextafter(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::nextafter, T, scalar_tag, T, Tag>
    > nextafter(
        const typename tensor<T, Rank>::value_type &x,
        const base_tensor<T, Rank, Tag> &y
    ) {
        typedef lazy_binary_tag<math::nextafter, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::nextafter(), x, y);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::nextafter, T, Tag, T, scalar_tag>
    > nextafter(
        const base_tensor<T, Rank, Tag> &x,
        const typename tensor<T, Rank>::value_type &y
    ) {
        typedef lazy_binary_tag<math::nextafter, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::nextafter(), x, y);
    }

    /// Integer-valued functions.

    /**
     * @brief Return the greatest common divisor, element-wise.
     *
     * @param m A tensor-like object with integer values.
     * @param n A tensor-like object with integer values.
     *
     * @return A light-weight object with the greatest common divisor of |m|
     *     and |n|, element-wise. If both m and n are zero, return zero. This
     *     function does not create a new tensor, instead, an expression object
     *     is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::gcd, T, Tag1, T, Tag2> >
    gcd(
        const base_tensor<T, Rank, Tag1> &m,
        const base_tensor<T, Rank, Tag2> &n
    ) {
        typedef lazy_binary_tag<math::gcd, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::gcd(), m, n);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::gcd, T, Tag, T, scalar_tag>
    > gcd(
        const base_tensor<T, Rank, Tag> &m,
        const typename tensor<T, Rank>::value_type &n
    ) {
        typedef lazy_binary_tag<math::gcd, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::gcd(), m, n);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::gcd, T, scalar_tag, T, Tag>
    > gcd(
        const typename tensor<T, Rank>::value_type &m,
        const base_tensor<T, Rank, Tag> &n
    ) {
        typedef lazy_binary_tag<math::gcd, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::gcd(), m, n);
    }

    /**
     * @brief Return the least common multiple, element-wise.
     *
     * @param m A tensor-like object with integer values.
     * @param n A tensor-like object with integer values.
     *
     * @return A light-weight object with the least common multiple of |m| and
     *     |n|, element-wise. If either m or n is zero, return zero. This
     *     function does not create a new tensor, instead, an expression
     *     object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<math::lcm, T, Tag1, T, Tag2> >
    lcm(
        const base_tensor<T, Rank, Tag1> &m,
        const base_tensor<T, Rank, Tag2> &n
    ) {
        typedef lazy_binary_tag<math::lcm, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(math::lcm(), m, n);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::lcm, T, Tag, T, scalar_tag>
    > lcm(
        const base_tensor<T, Rank, Tag> &m,
        const typename tensor<T, Rank>::value_type &n
    ) {
        typedef lazy_binary_tag<math::lcm, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(math::lcm(), m, n);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<math::lcm, T, scalar_tag, T, Tag>
    > lcm(
        const typename tensor<T, Rank>::value_type &m,
        const base_tensor<T, Rank, Tag> &n
    ) {
        typedef lazy_binary_tag<math::lcm, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::lcm(), m, n);
    }

    /// Complex numbers.

    /**
     * @brief Return the real part, element-wise.
     *
     * @param z A tensor-like object with complex values.
     *
     * @return A light-weight object with the real part of each element in the
     *     tensor. Non-complex types are treated as complex numbers with zero
     *     imaginary part component. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::real, std::complex<T>, Tag>
    > real(const base_tensor<std::complex<T>, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::real, std::complex<T>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::real(), z);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::real, T, Tag> >
    real(const base_tensor<T, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::real, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::real(), z);
    }

    /**
     * @brief Return the imaginary part, element-wise.
     *
     * @param z A tensor-like object with complex values.
     *
     * @return A light-weight object with the imaginary part of each element in
     *     the tensor. Non-complex types are treated as complex numbers with
     *     zero imaginary part component. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::imag, std::complex<T>, Tag>
    > imag(const base_tensor<std::complex<T>, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::imag, std::complex<T>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::imag(), z);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::imag, T, Tag> >
    imag(const base_tensor<T, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::imag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::imag(), z);
    }

    /**
     * @brief Return the complex conjugate, element-wise. The conjugate of a
     * complex number is obtained by changing the sign of its imaginary part.
     *
     * @param z A tensor-like object with the values whose complex conjugate is
     *     computed.
     *
     * @return A light-weight object with the complex conjugate of each element
     *     in the tensor. Non-complex types are treated as complex numbers with
     *     zero imaginary part component. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::conj, T, Tag> >
    conj(const base_tensor<T, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::conj, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::conj(), z);
    }

    /**
     * @brief Return the absolute value, element-wise.
     *
     * @param z A tensor-like object with the values whose absolute value is
     *     computed.
     *
     * @return A light-weight object with the absolute value of each element in
     *     the tensor. This function does not create a new tensor, instead, an
     *     expression object is returned (see lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::abs, std::complex<T>, Tag>
    > abs(const base_tensor<std::complex<T>, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::abs, std::complex<T>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::abs(), z);
    }

    /**
     * @brief Return the phase angle (in radians) of a complex number,
     * element-wise.
     *
     * @param z A tensor-like object with the values whose phase angle is
     *     computed.
     *
     * @return A light-weight object with the phase angle of each element in
     *     the tensor. Non-complex types are treated as complex numbers with
     *     zero imaginary part component. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::arg, std::complex<T>, Tag>
    > arg(const base_tensor<std::complex<T>, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::arg, std::complex<T>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::arg(), z);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math::arg, T, Tag> >
    arg(const base_tensor<T, Rank, Tag> &z) {
        typedef lazy_unary_tag<math::arg, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math::arg(), z);
    }

    /// Clasification functions.

    /**
     * @brief Return whether x is a finite value (neither infinite nor NaN),
     * element-wise.
     *
     * @param x A tensor-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     finite and false otherwise. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<bool, Rank, lazy_unary_tag<math::isfinite, T, Tag> >
    isfinite(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::isfinite, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(math::isfinite(), x);
    }

    /**
     * @brief Return whether x is an infinity value (either positive infinity
     * or negative infinity), element-wise.
     *
     * @param x A tensor-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     infinity and false otherwise. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<bool, Rank, lazy_unary_tag<math::isinf, T, Tag> >
    isinf(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::isinf, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(math::isinf(), x);
    }

    /**
     * @brief Return whether x is a NaN (Not a Number) value, element-wise. The
     * NaN values are used to identify undefined or non-representable values
     * for floating-point numbers, such as the square root of negative numbers
     * or the result of 0/0.
     *
     * @param x A tensor-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     NaN and false otherwise. This function does not create a new
     *     tensor, instead, an expression object is returned (see
     *     lazy-evaluation).
     */
    template <class T, size_t Rank, class Tag>
    inline base_tensor<bool, Rank, lazy_unary_tag<math::isnan, T, Tag> >
    isnan(const base_tensor<T, Rank, Tag> &x) {
        typedef lazy_unary_tag<math::isnan, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(math::isnan(), x);
    }
}

#endif // NUMCPP_MATH_H_INCLUDED
