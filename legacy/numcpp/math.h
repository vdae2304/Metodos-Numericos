/*
 * This file is part of the NumCpp project.
 *
 * NumCpp is a package for scientific computing in C++. It is a C++ library
 * that provides an array and a matrix object, and an assortment of routines
 * for fast operations on arrays and matrices, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * The NumCpp package is inspired by the NumPy package for Python, although it
 * is not related to it or any of its parts.
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
#include "numcpp/math/constants.h"
#include "numcpp/math/mathfwd.h"

namespace numcpp {
    /// Basic functions.

    /**
     * @brief Return the absolute value, element-wise.
     *
     * @param x An array-like object with the values whose absolute value is
     *     computed.
     *
     * @return A light-weight object with the absolute value of each element in
     *     the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_abs, T, Tag> >
    abs(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_abs, T, Tag> Closure;
        return base_array<T, Closure>(__math_abs(), x);
    }

    /**
     * @brief Return the absolute value, element-wise.
     *
     * @param x A matrix-like object with the values whose absolute value is
     *     computed.
     *
     * @return A light-weight object with the absolute value of each element in
     *     the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_abs, T, Tag> >
    abs(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_abs, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_abs(), x);
    }

    /**
     * @brief Return the floating-point remainder of x/y, element-wise. The
     * floating-point remainder of x/y is x - n*y, where n is the truncated
     * value (i.e., rounded towards zero) of x/y.
     *
     * @param x An array-like object with the values of the quotient numerator.
     * @param y An array-like object with the values of the quotient
     *     denominator.
     *
     * @return A light-weight object with the remainder of x/y. Convertible to
     *     an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_fmod, T, Tag1, T, Tag2> >
    fmod(const base_array<T, Tag1> &x, const base_array<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_fmod, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_fmod(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_fmod, T, Tag, T, scalar_tag> >
    fmod(
        const base_array<T, Tag> &x,
        const typename base_array<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_fmod, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_fmod(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_fmod, T, scalar_tag, T, Tag> >
    fmod(
        const typename base_array<T, Tag>::value_type &x,
        const base_array<T, Tag> &y
    ) {
        typedef lazy_binary_tag<__math_fmod, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_fmod(), x, y);
    }

    /**
     * @brief Return the floating-point remainder of x/y, element-wise. The
     * floating-point remainder of x/y is x - n*y, where n is the truncated
     * value (i.e., rounded towards zero) of x/y.
     *
     * @param x A matrix-like object with the values of the quotient numerator.
     * @param y A matrix-like object with the values of the quotient
     *     denominator.
     *
     * @return A light-weight object with the remainder of x/y. Convertible to
     *     a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_fmod, T, Tag1, T, Tag2> >
    fmod(const base_matrix<T, Tag1> &x, const base_matrix<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_fmod, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_fmod(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_fmod, T, Tag, T, scalar_tag> >
    fmod(
        const base_matrix<T, Tag> &x,
        const typename base_matrix<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_fmod, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_fmod(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_fmod, T, scalar_tag, T, Tag> >
    fmod(
        const typename base_matrix<T, Tag>::value_type &x,
        const base_matrix<T, Tag> &y
    ) {
        typedef lazy_binary_tag<__math_fmod, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_fmod(), x, y);
    }

    /// Trigonometric functions.

    /**
     * @brief Return the cosine, element-wise.
     *
     * @param x An array-like object with the angles in radians.
     *
     * @return A light-weight object with the cosine of each element in the
     *     array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_cos, T, Tag> >
    cos(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_cos, T, Tag> Closure;
        return base_array<T, Closure>(__math_cos(), x);
    }

    /**
     * @brief Return the cosine, element-wise.
     *
     * @param x A matrix-like object with the angles in radians.
     *
     * @return A light-weight object with the cosine of each element in the
     *     matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_cos, T, Tag> >
    cos(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_cos, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_cos(), x);
    }

    /**
     * @brief Return the sine, element-wise.
     *
     * @param x An array-like object with the angles in radians.
     *
     * @return A light-weight object with the sine of each element in the
     *     array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_sin, T, Tag> >
    sin(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_sin, T, Tag> Closure;
        return base_array<T, Closure>(__math_sin(), x);
    }

    /**
     * @brief Return the sine, element-wise.
     *
     * @param x A matrix-like object with the angles in radians.
     *
     * @return A light-weight object with the sine of each element in the
     *     matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_sin, T, Tag> >
    sin(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_sin, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_sin(), x);
    }

    /**
     * @brief Return the tangent, element-wise.
     *
     * @param x An array-like object with the angles in radians.
     *
     * @return A light-weight object with the tangent of each element in the
     *     array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_tan, T, Tag> >
    tan(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_tan, T, Tag> Closure;
        return base_array<T, Closure>(__math_tan(), x);
    }

    /**
     * @brief Return the tangent, element-wise.
     *
     * @param x A matrix-like object with the angles in radians.
     *
     * @return A light-weight object with the tangent of each element in the
     *     matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_tan, T, Tag> >
    tan(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_tan, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_tan(), x);
    }

    /**
     * @brief Return the hypotenuse of a right-angled triangle whose legs are x
     * and y, element-wise.
     *
     * @param x An array-like object with one of the legs of the right-angle
     *     triangles.
     * @param y An array-like object with one of the legs of the right-angle
     *     triangles.
     *
     * @return A light-weight object with the hypotenuse of the triangles.
     *     Convertible to an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_hypot, T, Tag1, T, Tag2> >
    hypot(const base_array<T, Tag1> &x, const base_array<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_hypot, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_hypot(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_hypot, T, Tag, T, scalar_tag> >
    hypot(
        const base_array<T, Tag> &x,
        const typename base_array<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_hypot, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_hypot(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_hypot, T, scalar_tag, T, Tag> >
    hypot(
        const typename base_array<T, Tag>::value_type &x,
        const base_array<T, Tag> &y
    ) {
        typedef lazy_binary_tag<__math_hypot, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_hypot(), x, y);
    }

    /**
     * @brief Return the hypotenuse of a right-angled triangle whose legs are x
     * and y, element-wise.
     *
     * @param x A matrix-like object with one of the legs of the right-angle
     *     triangles.
     * @param y A matrix-like object with one of the legs of the right-angle
     *     triangles.
     *
     * @return A light-weight object with the hypotenuse of the triangles.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_hypot, T, Tag1, T, Tag2> >
    hypot(const base_matrix<T, Tag1> &x, const base_matrix<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_hypot, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_hypot(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_hypot, T, Tag, T, scalar_tag> >
    hypot(
        const base_matrix<T, Tag> &x,
        const typename base_matrix<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_hypot, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_hypot(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_hypot, T, scalar_tag, T, Tag> >
    hypot(
        const typename base_matrix<T, Tag>::value_type &x,
        const base_matrix<T, Tag> &y
    ) {
        typedef lazy_binary_tag<__math_hypot, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_hypot(), x, y);
    }

    /**
     * @brief Return the principal value of the arc cosine, element-wise.
     *
     * @param x An array-like object with the values whose arc cosine is
     *     computed.
     *
     * @return A light-weight object with the arc cosine, in radians, of each
     *     value in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_acos, T, Tag> >
    acos(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_acos, T, Tag> Closure;
        return base_array<T, Closure>(__math_acos(), x);
    }

    /**
     * @brief Return the principal value of the arc cosine, element-wise.
     *
     * @param x A matrix-like object with the values whose arc cosine is
     *     computed.
     *
     * @return A light-weight object with the arc cosine, in radians, of each
     *     value in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_acos, T, Tag> >
    acos(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_acos, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_acos(), x);
    }

    /**
     * @brief Return the principal value of the arc sine, element-wise.
     *
     * @param x An array-like object with the values whose arc sine is
     *     computed.
     *
     * @return A light-weight object with the arc sine, in radians, of each
     *     value in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_asin, T, Tag> >
    asin(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_asin, T, Tag> Closure;
        return base_array<T, Closure>(__math_asin(), x);
    }

    /**
     * @brief Return the principal value of the arc sine, element-wise.
     *
     * @param x A matrix-like object with the values whose arc sine is
     *     computed.
     *
     * @return A light-weight object with the arc sine, in radians, of each
     *     value in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_asin, T, Tag> >
    asin(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_asin, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_asin(), x);
    }

    /**
     * @brief Return the principal value of the arc tangent, element-wise.
     *
     * @param x An array-like object with the values whose arc tangent is
     *     computed.
     *
     * @return A light-weight object with the arc tangent, in radians, of each
     *     value in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_atan, T, Tag> >
    atan(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_atan, T, Tag> Closure;
        return base_array<T, Closure>(__math_atan(), x);
    }

    /**
     * @brief Return the principal value of the arc tangent, element-wise.
     *
     * @param x A matrix-like object with the values whose arc tangent is
     *     computed.
     *
     * @return A light-weight object with the arc tangent, in radians, of each
     *     value in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_atan, T, Tag> >
    atan(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_atan, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_atan(), x);
    }

    /**
     * @brief Return the principal value of the arc tangent of y/x,
     * element-wise. To compute the value, the function takes into account the
     * sign of both arguments in order to determine the quadrant.
     *
     * @param y An array-like object with the y-coordinates.
     * @param x An array-like object with the x-coordinates.
     *
     * @return A light-weight object with the arc tangent, in radians, of y/x.
     *     Convertible to an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_atan2, T, Tag1, T, Tag2> >
    atan2(const base_array<T, Tag1> &y, const base_array<T, Tag2> &x) {
        typedef lazy_binary_tag<__math_atan2, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_atan2(), y, x);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_atan2, T, Tag, T, scalar_tag> >
    atan2(
        const base_array<T, Tag> &y,
        const typename base_array<T, Tag>::value_type &x
    ) {
        typedef lazy_binary_tag<__math_atan2, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_atan2(), y, x);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_atan2, T, scalar_tag, T, Tag> >
    atan2(
        const typename base_array<T, Tag>::value_type &y,
        const base_array<T, Tag> &x
    ) {
        typedef lazy_binary_tag<__math_atan2, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_atan2(), y, x);
    }

    /**
     * @brief Return the principal value of the arc tangent of y/x,
     * element-wise. To compute the value, the function takes into account the
     * sign of both arguments in order to determine the quadrant.
     *
     * @param y A matrix-like object with the y-coordinates.
     * @param x A matrix-like object with the x-coordinates.
     *
     * @return A light-weight object with the arc tangent, in radians, of y/x.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_atan2, T, Tag1, T, Tag2> >
    atan2(const base_matrix<T, Tag1> &y, const base_matrix<T, Tag2> &x) {
        typedef lazy_binary_tag<__math_atan2, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_atan2(), y, x);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_atan2, T, Tag, T, scalar_tag> >
    atan2(
        const base_matrix<T, Tag> &y,
        const typename base_matrix<T, Tag>::value_type &x
    ) {
        typedef lazy_binary_tag<__math_atan2, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_atan2(), y, x);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_atan2, T, scalar_tag, T, Tag> >
    atan2(
        const typename base_matrix<T, Tag>::value_type &y,
        const base_matrix<T, Tag> &x
    ) {
        typedef lazy_binary_tag<__math_atan2, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_atan2(), y, x);
    }

    /**
     * @brief Convert angles from radians to degrees, element-wise.
     *
     * @param x An array-like object with the angles in radians.
     *
     * @return A light-weight object with the corresponding angles in degrees.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_degrees, T, Tag> >
    degrees(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_degrees, T, Tag> Closure;
        return base_array<T, Closure>(__math_degrees(), x);
    }

    /**
     * @brief Convert angles from radians to degrees, element-wise.
     *
     * @param x A matrix-like object with the angles in radians.
     *
     * @return A light-weight object with the corresponding angles in degrees.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_degrees, T, Tag> >
    degrees(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_degrees, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_degrees(), x);
    }

    /**
     * @brief Convert angles from degrees to radians, element-wise.
     *
     * @param x An array-like object with the angles in degrees.
     *
     * @return A light-weight object with the corresponding angles in radians.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_radians, T, Tag> >
    radians(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_radians, T, Tag> Closure;
        return base_array<T, Closure>(__math_radians(), x);
    }

    /**
     * @brief Convert angles from degrees to radians, element-wise.
     *
     * @param x A matrix-like object with the angles in degrees.
     *
     * @return A light-weight object with the corresponding angles in radians.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_radians, T, Tag> >
    radians(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_radians, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_radians(), x);
    }

    /// Hyperbolic functions.

    /**
     * @brief Return the hyperbolic cosine, element-wise.
     *
     * @param x An array-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic cosine of each element
     *     in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_cosh, T, Tag> >
    cosh(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_cosh, T, Tag> Closure;
        return base_array<T, Closure>(__math_cosh(), x);
    }

    /**
     * @brief Return the hyperbolic cosine, element-wise.
     *
     * @param x A matrix-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic cosine of each element
     *     in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_cosh, T, Tag> >
    cosh(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_cosh, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_cosh(), x);
    }

    /**
     * @brief Return the hyperbolic sine, element-wise.
     *
     * @param x An array-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic sine of each element
     *     in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_sinh, T, Tag> >
    sinh(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_sinh, T, Tag> Closure;
        return base_array<T, Closure>(__math_sinh(), x);
    }

    /**
     * @brief Return the hyperbolic sine, element-wise.
     *
     * @param x A matrix-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic sine of each element
     *     in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_sinh, T, Tag> >
    sinh(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_sinh, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_sinh(), x);
    }

    /**
     * @brief Return the hyperbolic tangent, element-wise.
     *
     * @param x An array-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic tangent of each
     *     element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_tanh, T, Tag> >
    tanh(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_tanh, T, Tag> Closure;
        return base_array<T, Closure>(__math_tanh(), x);
    }

    /**
     * @brief Return the hyperbolic tangent, element-wise.
     *
     * @param x A matrix-like object with the hyperbolic angles.
     *
     * @return A light-weight object with the hyperbolic tangent of each
     *     element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_tanh, T, Tag> >
    tanh(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_tanh, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_tanh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic cosine, element-wise.
     *
     * @param x An array-like object with the values whose inverse hyperbolic
     *     cosine is computed.
     *
     * @return A light-weight object with the inverse hyperbolic cosine of each
     *     element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_acosh, T, Tag> >
    acosh(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_acosh, T, Tag> Closure;
        return base_array<T, Closure>(__math_acosh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic cosine, element-wise.
     *
     * @param x A matrix-like object with the values whose inverse hyperbolic
     *     cosine is computed.
     *
     * @return A light-weight object with the inverse hyperbolic cosine of each
     *     element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_acosh, T, Tag> >
    acosh(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_acosh, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_acosh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic sine, element-wise.
     *
     * @param x An array-like object with the values whose inverse hyperbolic
     *     sine is computed.
     *
     * @return A light-weight object with the inverse hyperbolic sine of each
     *     element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_asinh, T, Tag> >
    asinh(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_asinh, T, Tag> Closure;
        return base_array<T, Closure>(__math_asinh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic sine, element-wise.
     *
     * @param x A matrix-like object with the values whose inverse hyperbolic
     *     sine is computed.
     *
     * @return A light-weight object with the inverse hyperbolic sine of each
     *     element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_asinh, T, Tag> >
    asinh(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_asinh, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_asinh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic tangent, element-wise.
     *
     * @param x An array-like object with the values whose inverse hyperbolic
     *     tangent is computed.
     *
     * @return A light-weight object with the inverse hyperbolic tangent of
     *     each element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_atanh, T, Tag> >
    atanh(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_atanh, T, Tag> Closure;
        return base_array<T, Closure>(__math_atanh(), x);
    }

    /**
     * @brief Return the inverse hyperbolic tangent, element-wise.
     *
     * @param x A matrix-like object with the values whose inverse hyperbolic
     *     tangent is computed.
     *
     * @return A light-weight object with the inverse hyperbolic tangent of
     *     each element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_atanh, T, Tag> >
    atanh(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_atanh, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_atanh(), x);
    }

    /// Exponential and logarithmic functions.

    /**
     * @brief Return the base-e exponential, which is e raised to the power x:
     * e^x, element-wise.
     *
     * @param x An array-like object with the values of the exponents.
     *
     * @return A light-weight object with the exponential of each element in
     *     the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_exp, T, Tag> >
    exp(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_exp, T, Tag> Closure;
        return base_array<T, Closure>(__math_exp(), x);
    }

    /**
     * @brief Return the base-e exponential, which is e raised to the power x:
     * e^x, element-wise.
     *
     * @param x A matrix-like object with the values of the exponents.
     *
     * @return A light-weight object with the exponential of each element in
     *     the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_exp, T, Tag> >
    exp(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_exp, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_exp(), x);
    }

    /**
     * @brief Return the natural logarithm, which is the inverse of the
     * exponential function (exp), element-wise.
     *
     * @param x An array-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light weight-object with the natural logarithm of each element
     *     in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_log, T, Tag> >
    log(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log, T, Tag> Closure;
        return base_array<T, Closure>(__math_log(), x);
    }

    /**
     * @brief Return the natural logarithm, which is the inverse of the
     * exponential function (exp), element-wise.
     *
     * @param x A matrix-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light weight-object with the natural logarithm of each element
     *     in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_log, T, Tag> >
    log(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_log(), x);
    }

    /**
     * @brief Return the common (base-10) logarithm, element-wise.
     *
     * @param x An array-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the common logarithm of each element
     *     in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_log10, T, Tag> >
    log10(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log10, T, Tag> Closure;
        return base_array<T, Closure>(__math_log10(), x);
    }

    /**
     * @brief Return the common (base-10) logarithm, element-wise.
     *
     * @param x A matrix-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the common logarithm of each element
     *     in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_log10, T, Tag> >
    log10(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log10, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_log10(), x);
    }

    /**
     * @brief Return the base-2 exponential, which is 2 raised to the power x:
     * 2^x, element-wise.
     *
     * @param x An array-like object with the values of the exponents.
     *
     * @return A light-weight object with the base-2 exponential of each
     *     element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_exp2, T, Tag> >
    exp2(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_exp2, T, Tag> Closure;
        return base_array<T, Closure>(__math_exp2(), x);
    }

    /**
     * @brief Return the base-2 exponential, which is 2 raised to the power x:
     * 2^x, element-wise.
     *
     * @param x A matrix-like object with the values of the exponents.
     *
     * @return A light-weight object with the base-2 exponential of each
     *     element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_exp2, T, Tag> >
    exp2(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_exp2, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_exp2(), x);
    }

    /**
     * @brief Return the binary (base-2) logarithm, element-wise.
     *
     * @param x An array-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the binary logarithm of each element
     *     in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_log2, T, Tag> >
    log2(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log2, T, Tag> Closure;
        return base_array<T, Closure>(__math_log2(), x);
    }

    /**
     * @brief Return the binary (base-2) logarithm, element-wise.
     *
     * @param x A matrix-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the binary logarithm of each element
     *     in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_log2, T, Tag> >
    log2(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log2, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_log2(), x);
    }

    /**
     * @brief Return e raised to the power x minus one: e^x - 1, element-wise.
     * For values of x close to zero, expm1 is more accurate than exp(x) - 1.
     *
     * @param x An array-like object with the values of the exponent.
     *
     * @return A light-weight object with the exponential minus one of each
     *     element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_expm1, T, Tag> >
    expm1(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_expm1, T, Tag> Closure;
        return base_array<T, Closure>(__math_expm1(), x);
    }

    /**
     * @brief Return e raised to the power x minus one: e^x - 1, element-wise.
     * For values of x close to zero, expm1 is more accurate than exp(x) - 1.
     *
     * @param x A matrix-like object with the values of the exponent.
     *
     * @return A light-weight object with the exponential minus one of each
     *     element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_expm1, T, Tag> >
    expm1(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_expm1, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_expm1(), x);
    }

    /**
     * @brief Return the natural logarithm of one plus x, element-wise. For
     * values of x close to zero, log1p is more accurate than log(1 + x).
     *
     * @param x An array-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the natural logarithm of (1 + x) for
     *     each element in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_log1p, T, Tag> >
    log1p(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log1p, T, Tag> Closure;
        return base_array<T, Closure>(__math_log1p(), x);
    }

    /**
     * @brief Return the natural logarithm of one plus x, element-wise. For
     * values of x close to zero, log1p is more accurate than log(1 + x).
     *
     * @param x A matrix-like object with the values whose logarithm is
     *     computed.
     *
     * @return A light-weight object with the natural logarithm of (1 + x) for
     *     each element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_log1p, T, Tag> >
    log1p(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_log1p, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_log1p(), x);
    }

    /// Power functions.

    /**
     * @brief Return x raised to the power y, element-wise.
     *
     * @param x An array-like object with the values of the base.
     * @param y An array-like object with the values of the exponent.
     *
     * @return A light-weight object with the result of rasing x to the power
     *     y. Convertible to an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_pow, T, Tag1, T, Tag2> >
    pow(const base_array<T, Tag1> &x, const base_array<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_pow, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_pow(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_pow, T, Tag, T, scalar_tag> >
    pow(
        const base_array<T, Tag> &x,
        const typename base_array<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_pow, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_pow(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_pow, T, scalar_tag, T, Tag> >
    pow(
        const typename base_array<T, Tag>::value_type &x,
        const base_array<T, Tag> &y
    ) {
        typedef lazy_binary_tag<__math_pow, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_pow(), x, y);
    }

    /**
     * @brief Return x raised to the power y, element-wise.
     *
     * @param x A matrix-like object with the values of the base.
     * @param y A matrix-like object with the values of the exponent.
     *
     * @return A light-weight object with the result of rasing x to the power
     *     y. Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_pow, T, Tag1, T, Tag2> >
    pow(const base_matrix<T, Tag1> &x, const base_matrix<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_pow, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_pow(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_pow, T, Tag, T, scalar_tag> >
    pow(
        const base_matrix<T, Tag> &x,
        const typename base_matrix<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_pow, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_pow(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_pow, T, scalar_tag, T, Tag> >
    pow(
        const typename base_matrix<T, Tag>::value_type &x,
        const base_matrix<T, Tag> &y
    ) {
        typedef lazy_binary_tag<__math_pow, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_pow(), x, y);
    }

    /**
     * @brief Return the square root, element-wise.
     *
     * @param x An array-like object with the values whose square root is
     *     computed.
     *
     * @return A light-weight object with the square root of each element in
     *     the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_sqrt, T, Tag> >
    sqrt(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_sqrt, T, Tag> Closure;
        return base_array<T, Closure>(__math_sqrt(), x);
    }

    /**
     * @brief Return the square root, element-wise.
     *
     * @param x A matrix-like object with the values whose square root is
     *     computed.
     *
     * @return A light-weight object with the square root of each element in
     *     the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_sqrt, T, Tag> >
    sqrt(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_sqrt, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_sqrt(), x);
    }

    /**
     * @brief Return the cubic root, element-wise.
     *
     * @param x An array-like object with the values whose cubic root is
     *     computed.
     *
     * @return A light-weight object with the cubic root of each element in the
     *     array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_cbrt, T, Tag> >
    cbrt(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_cbrt, T, Tag> Closure;
        return base_array<T, Closure>(__math_cbrt(), x);
    }

    /**
     * @brief Return the cubic root, element-wise.
     *
     * @param x A matrix-like object with the values whose cubic root is
     *     computed.
     *
     * @return A light-weight object with the cubic root of each element in the
     *     matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_cbrt, T, Tag> >
    cbrt(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_cbrt, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_cbrt(), x);
    }

    /// Rounding.

    /**
     * @brief Rounds x upward. Return the smallest integral value that is not
     * less than x, element-wise.
     *
     * @param x An array-like object with the values to round up.
     *
     * @return A light-weight object with the ceiling of each element in the
     *     array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_ceil, T, Tag> >
    ceil(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_ceil, T, Tag> Closure;
        return base_array<T, Closure>(__math_ceil(), x);
    }

    /**
     * @brief Rounds x upward. Return the smallest integral value that is not
     * less than x, element-wise.
     *
     * @param x A matrix-like object with the values to round up.
     *
     * @return A light-weight object with the ceiling of each element in the
     *     matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_ceil, T, Tag> >
    ceil(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_ceil, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_ceil(), x);
    }

    /**
     * @brief Rounds x downward. Return the largest integral value that is not
     * greater than x, element-wise.
     *
     * @param x An array-like object with the values to round down.
     *
     * @return A light-weight object with the floor of each element in the
     *     array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_floor, T, Tag> >
    floor(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_floor, T, Tag> Closure;
        return base_array<T, Closure>(__math_floor(), x);
    }

    /**
     * @brief Rounds x downward. Return the largest integral value that is not
     * greater than x, element-wise.
     *
     * @param x A matrix-like object with the values to round down.
     *
     * @return A light-weight object with the floor of each element in the
     *     matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_floor, T, Tag> >
    floor(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_floor, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_floor(), x);
    }

    /**
     * @brief Rounds x toward zero. Return the nearest integral value that is
     * not larger in magnitude than x, element-wise.
     *
     * @param x An array-like object with the values to truncate.
     *
     * @return A light-weight object with the truncated value of each element
     *     in the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_trunc, T, Tag> >
    trunc(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_trunc, T, Tag> Closure;
        return base_array<T, Closure>(__math_trunc(), x);
    }

    /**
     * @brief Rounds x toward zero. Return the nearest integral value that is
     * not larger in magnitude than x, element-wise.
     *
     * @param x A matrix-like object with the values to truncate.
     *
     * @return A light-weight object with the truncated value of each element
     *     in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_trunc, T, Tag> >
    trunc(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_trunc, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_trunc(), x);
    }

    /**
     * @brief Return the integral value that is nearest to x, element-wise.
     * Halfway cases are rounded away from zero.
     *
     * @param x An array-like object with the values to round.
     *
     * @return A light-weight object with the rounded value of each element in
     *     the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_round, T, Tag> >
    round(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_round, T, Tag> Closure;
        return base_array<T, Closure>(__math_round(), x);
    }

    /**
     * @brief Return the integral value that is nearest to x, element-wise.
     * Halfway cases are rounded away from zero.
     *
     * @param x A matrix-like object with the values to round.
     *
     * @return A light-weight object with the rounded value of each element in
     *     the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_round, T, Tag> >
    round(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_round, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_round(), x);
    }

    /// Floating-point manipulation functions.

    /**
     * @brief Descompose x into its binary significand and an integral power of
     * two, such that x = significand * 2^exponent, element-wise.
     *
     * @param x An array-like object with the values to be decomposed.
     *
     * @return A light-weight object with the binary significand and the
     *     exponent of each element in the array. Convertible to an array
     *     object.
     */
    template <class T, class Tag>
    base_array< std::pair<T, int>, lazy_unary_tag<__math_frexp, T, Tag> >
    frexp(const base_array<T, Tag> &x) {
        typedef std::pair<T, int> Rt;
        typedef lazy_unary_tag<__math_frexp, T, Tag> Closure;
        return base_array<Rt, Closure>(__math_frexp(), x);
    }

    /**
     * @brief Descompose x into its binary significand and an integral power of
     * two, such that x = significand * 2^exponent, element-wise.
     *
     * @param x A matrix-like object with the values to be decomposed.
     *
     * @return A light-weight object with the binary significand and the
     *     exponent of each element in the matrix. Convertible to a matrix
     *     object.
     */
    template <class T, class Tag>
    base_matrix< std::pair<T, int>, lazy_unary_tag<__math_frexp, T, Tag> >
    frexp(const base_matrix<T, Tag> &x) {
        typedef std::pair<T, int> Rt;
        typedef lazy_unary_tag<__math_frexp, T, Tag> Closure;
        return base_matrix<Rt, Closure>(__math_frexp(), x);
    }

    /**
     * @brief Return the result of multiplying x (the significand) by 2 raised
     * to the power of exp (the exponent), element-wise.
     *
     * @param x An array-like object with the values of the significand.
     * @param exp An array-like object with the values of the exponent.
     *
     * @return A light-weight object with the result of x*2^exp for each
     *     element in the array. Convertible to an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_ldexp, T, Tag1, int, Tag2> >
    ldexp(const base_array<T, Tag1> &x, const base_array<int, Tag2> &exp) {
        typedef lazy_binary_tag<__math_ldexp, T, Tag1, int, Tag2> Closure;
        return base_array<T, Closure>(__math_ldexp(), x, exp);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_ldexp, T, Tag, int, scalar_tag> >
    ldexp(
        const base_array<T, Tag> &x,
        const typename base_array<int, Tag>::value_type &exp
    ) {
        typedef lazy_binary_tag<__math_ldexp, T, Tag, int, scalar_tag> Closure;
        return base_array<T, Closure>(__math_ldexp(), x, exp);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_ldexp, T, scalar_tag, int, Tag> >
    ldexp(
        const typename base_array<T, Tag>::value_type &x,
        const base_array<int, Tag> &exp
    ) {
        typedef lazy_binary_tag<__math_ldexp, T, scalar_tag, int, Tag> Closure;
        return base_array<T, Closure>(__math_ldexp(), x, exp);
    }

    /**
     * @brief Return the result of multiplying x (the significand) by 2 raised
     * to the power of exp (the exponent), element-wise.
     *
     * @param x A matrix-like object with the values of the significand.
     * @param exp A matrix-like object with the values of the exponent.
     *
     * @return A light-weight object with the result of x*2^exp for each
     *     element in the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_ldexp, T, Tag1, int, Tag2> >
    ldexp(const base_matrix<T, Tag1> &x, const base_matrix<int, Tag2> &exp) {
        typedef lazy_binary_tag<__math_ldexp, T, Tag1, int, Tag2> Closure;
        return base_matrix<T, Closure>(__math_ldexp(), x, exp);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_ldexp, T, Tag, int, scalar_tag> >
    ldexp(
        const base_matrix<T, Tag> &x,
        const typename base_matrix<int, Tag>::value_type &exp
    ) {
        typedef lazy_binary_tag<__math_ldexp, T, Tag, int, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_ldexp(), x, exp);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_ldexp, T, scalar_tag, int, Tag> >
    ldexp(
        const typename base_matrix<T, Tag>::value_type &x,
        const base_matrix<int, Tag> &exp
    ) {
        typedef lazy_binary_tag<__math_ldexp, T, scalar_tag, int, Tag> Closure;
        return base_matrix<T, Closure>(__math_ldexp(), x, exp);
    }

    /**
     * @brief Return a value with the magnitude of x and the sign of y,
     * element-wise.
     *
     * @param x An array-like object with the values to change the sign of.
     * @param y An array-like object with the values to copy the sign from.
     *
     * @return A light-weight object with the values in the first array and the
     *     signs in the second array. Convertible to an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_copysign, T, Tag1, T, Tag2> >
    copysign(const base_array<T, Tag1> &x, const base_array<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_copysign, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_copysign(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_copysign, T, Tag, T, scalar_tag> >
    copysign(
        const base_array<T, Tag> &x,
        const typename base_array<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_copysign, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_copysign(), x, y);
    }

    /**
     * @brief Return a value with the magnitude of x and the sign of y,
     * element-wise.
     *
     * @param x A matrix-like object with the values to change the sign of.
     * @param y A matrix-like object with the values to copy the sign from.
     *
     * @return A light-weight object with the values in the first matrix and
     *     the signs in the second matrix. Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_copysign, T, Tag1, T, Tag2> >
    copysign(const base_matrix<T, Tag1> &x, const base_matrix<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_copysign, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_copysign(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_copysign, T, Tag, T, scalar_tag> >
    copysign(
        const base_matrix<T, Tag> &x,
        const typename base_matrix<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_copysign, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_copysign(), x, y);
    }

    /**
     * @brief Return the next representable value after x in the direction of
     * y, element-wise.
     *
     * @param x An array-like object with the base values.
     * @param y An array-like object with the directions where to look for the
     *     next representable values.
     *
     * @return A light-weight object with the next representable value of each
     *     element in the first array in the direction of the second array.
     *     Convertible to an array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_nextafter, T, Tag1, T, Tag2> >
    nextafter(const base_array<T, Tag1> &x, const base_array<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_nextafter, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_nextafter(), x, y);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_nextafter, T, Tag, T, scalar_tag> >
    nextafter(
        const base_array<T, Tag> &x,
        const typename base_array<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_nextafter, T,Tag, T,scalar_tag> Closure;
        return base_array<T, Closure>(__math_nextafter(), x, y);
    }

    /**
     * @brief Return the next representable value after x in the direction of
     * y, element-wise.
     *
     * @param x A matrix-like object with the base values.
     * @param y A matrix-like object with the directions where to look for the
     *     next representable values.
     *
     * @return A light-weight object with the next representable value of each
     *     element in the first matrix in the direction of the second matrix.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_nextafter, T, Tag1, T, Tag2> >
    nextafter(const base_matrix<T, Tag1> &x, const base_matrix<T, Tag2> &y) {
        typedef lazy_binary_tag<__math_nextafter, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_nextafter(), x, y);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_nextafter, T, Tag, T, scalar_tag> >
    nextafter(
        const base_matrix<T, Tag> &x,
        const typename base_matrix<T, Tag>::value_type &y
    ) {
        typedef lazy_binary_tag<__math_nextafter, T,Tag, T,scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_nextafter(), x, y);
    }

    /// Integer-valued functions.

    /**
     * @brief Return the greatest common divisor, element-wise.
     *
     * @param m An array-like object with integer values.
     * @param n An array-like object with integer values.
     *
     * @return A light-weight object with the greatest common divisor of |m|
     *     and |n|. If both m and n are zero, return zero. Convertible to an
     *     array object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_gcd, T, Tag1, T, Tag2> >
    gcd(const base_array<T, Tag1> &m, const base_array<T, Tag2> &n) {
        typedef lazy_binary_tag<__math_gcd, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_gcd(), m, n);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_gcd, T, Tag, T, scalar_tag> >
    gcd(
        const base_array<T, Tag> &m,
        const typename base_array<T, Tag>::value_type &n
    ) {
        typedef lazy_binary_tag<__math_gcd, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_gcd(), m, n);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_gcd, T, scalar_tag, T, Tag> >
    gcd(
        const typename base_array<T, Tag>::value_type &m,
        const base_array<T, Tag> &n
    ) {
        typedef lazy_binary_tag<__math_gcd, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_gcd(), m, n);
    }

    /**
     * @brief Return the greatest common divisor, element-wise.
     *
     * @param m A matrix-like object with integer values.
     * @param n A matrix-like object with integer values.
     *
     * @return A light-weight object with the greatest common divisor of |m|
     *     and |n|. If both m and n are zero, return zero. Convertible to a
     *     matrix object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_gcd, T, Tag1, T, Tag2> >
    gcd(const base_matrix<T, Tag1> &m, const base_matrix<T, Tag2> &n) {
        typedef lazy_binary_tag<__math_gcd, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_gcd(), m, n);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_gcd, T, Tag, T, scalar_tag> >
    gcd(
        const base_matrix<T, Tag> &m,
        const typename base_matrix<T, Tag>::value_type &n
    ) {
        typedef lazy_binary_tag<__math_gcd, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_gcd(), m, n);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_gcd, T, scalar_tag, T, Tag> >
    gcd(
        const typename base_matrix<T, Tag>::value_type &m,
        const base_matrix<T, Tag> &n
    ) {
        typedef lazy_binary_tag<__math_gcd, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_gcd(), m, n);
    }

    /**
     * @brief Return the least common multiple, element-wise.
     *
     * @param m An array-like object with integer values.
     * @param n An array-like object with integer values.
     *
     * @return A light-weight object with the least common multiple of |m| and
     *     |n|. If either m or n is zero, return zero. Convertible to an array
     *     object.
     */
    template <class T, class Tag1, class Tag2>
    base_array< T, lazy_binary_tag<__math_lcm, T, Tag1, T, Tag2> >
    lcm(const base_array<T, Tag1> &m, const base_array<T, Tag2> &n) {
        typedef lazy_binary_tag<__math_lcm, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__math_lcm(), m, n);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_lcm, T, Tag, T, scalar_tag> >
    lcm(
        const base_array<T, Tag> &m,
        const typename base_array<T, Tag>::value_type &n
    ) {
        typedef lazy_binary_tag<__math_lcm, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__math_lcm(), m, n);
    }

    template <class T, class Tag>
    base_array< T, lazy_binary_tag<__math_lcm, T, scalar_tag, T, Tag> >
    lcm(
        const typename base_array<T, Tag>::value_type &m,
        const base_array<T, Tag> &n
    ) {
        typedef lazy_binary_tag<__math_lcm, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__math_lcm(), m, n);
    }

    /**
     * @brief Return the least common multiple, element-wise.
     *
     * @param m A matrix-like object with integer values.
     * @param n A matrix-like object with integer values.
     *
     * @return A light-weight object with the least common multiple of |m| and
     *     |n|. If either m or n is zero, return zero. Convertible to a matrix
     *     object.
     */
    template <class T, class Tag1, class Tag2>
    base_matrix< T, lazy_binary_tag<__math_lcm, T, Tag1, T, Tag2> >
    lcm(const base_matrix<T, Tag1> &m, const base_matrix<T, Tag2> &n) {
        typedef lazy_binary_tag<__math_lcm, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__math_lcm(), m, n);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_lcm, T, Tag, T, scalar_tag> >
    lcm(
        const base_matrix<T, Tag> &m,
        const typename base_matrix<T, Tag>::value_type &n
    ) {
        typedef lazy_binary_tag<__math_lcm, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__math_lcm(), m, n);
    }

    template <class T, class Tag>
    base_matrix< T, lazy_binary_tag<__math_lcm, T, scalar_tag, T, Tag> >
    lcm(
        const typename base_matrix<T, Tag>::value_type &m,
        const base_matrix<T, Tag> &n
    ) {
        typedef lazy_binary_tag<__math_lcm, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_lcm(), m, n);
    }

    /// Complex numbers.

    /**
     * @brief Return the real part, element-wise.
     *
     * @param z An array-like object with complex values.
     *
     * @return A light-weight object with the real part of each element in the
     *     array. If T is not a complex type, return the elements unchanged.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_real, T, Tag>
    >
    real(const base_array<T, Tag> &z) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_real, T, Tag> Closure;
        return base_array<Rt, Closure>(__math_real(), z);
    }

    /**
     * @brief Return the real part, element-wise.
     *
     * @param z A matrix-like object with complex values.
     *
     * @return A light-weight object with the real part of each element in the
     *     matrix. If T is not a complex type, return the elements unchanged.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_real, T, Tag>
    >
    real(const base_matrix<T, Tag> &z) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_real, T, Tag> Closure;
        return base_matrix<Rt, Closure>(__math_real(), z);
    }

    /**
     * @brief Return the imaginary part, element-wise.
     *
     * @param z An array-like object with complex values.
     *
     * @return A light-weight object with the imaginary part of each element in
     *     the array. If T is not a complex type, return an array of zeros.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_imag, T, Tag>
    >
    imag(const base_array<T, Tag> &z) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_imag, T, Tag> Closure;
        return base_array<Rt, Closure>(__math_imag(), z);
    }

    /**
     * @brief Return the imaginary part, element-wise.
     *
     * @param z A matrix-like object with complex values.
     *
     * @return A light-weight object with the imaginary part of each element in
     *     the matrix. If T is not a complex type, return a matrix of zeros.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_imag, T, Tag>
    >
    imag(const base_matrix<T, Tag> &z) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_imag, T, Tag> Closure;
        return base_matrix<Rt, Closure>(__math_imag(), z);
    }

    /**
     * @brief Return the absolute value, element-wise.
     *
     * @param z An array-like object with the values whose absolute value is
     *     computed.
     *
     * @return A light-weight object with the absolute value of each element in
     *     the array. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_abs, std::complex<T>, Tag> >
    abs(const base_array<std::complex<T>, Tag> &z) {
        typedef lazy_unary_tag<__math_abs, std::complex<T>, Tag> Closure;
        return base_array<T, Closure>(__math_abs(), z);
    }

    /**
     * @brief Return the absolute value, element-wise.
     *
     * @param z A matrix-like object with the values whose absolute value is
     *     computed.
     *
     * @return A light-weight object with the absolute value of each element in
     *     the matrix. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_abs, std::complex<T>, Tag> >
    abs(const base_matrix<std::complex<T>, Tag> &z) {
        typedef lazy_unary_tag<__math_abs, std::complex<T>, Tag> Closure;
        return base_matrix<T, Closure>(__math_abs(), z);
    }

    /**
     * @brief Return the phase angle (in radians) of a complex number,
     * element-wise.
     *
     * @param z An array-like object with the values whose phase angle is
     *     computed.
     *
     * @return A light-weight object with the phase angle of each element in
     *     the array. If T is not a complex type, the function assumes the
     *     value has a zero imaginary part. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_arg, T, Tag>
    >
    arg(const base_array<T, Tag> &z) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_arg, T, Tag> Closure;
        return base_array<Rt, Closure>(__math_arg(), z);
    }

    /**
     * @brief Return the phase angle (in radians) of a complex number,
     * element-wise.
     *
     * @param z A matrix-like object with the values whose phase angle is
     *     computed.
     *
     * @return A light-weight object with the phase angle of each element in
     *     the matrix. If T is not a complex type, the function assumes the
     *     value has a zero imaginary part. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_arg, T, Tag>
    >
    arg(const base_matrix<T, Tag> &z) {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_arg, T, Tag> Closure;
        return base_matrix<Rt, Closure>(__math_arg(), z);
    }

    /**
     * @brief Return the complex conjugate, element-wise. The complex conjugate
     * of a complex number is obtained by changing the sign of its imaginary
     * part.
     *
     * @param z An array-like object with the values whose complex conjugate is
     *     computed.
     *
     * @return A light-weight object with the complex conjugate of each
     *     element in the array. If T is not a complex type, return the
     *     elements unchanged. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, lazy_unary_tag<__math_conj, T, Tag> >
    conj(const base_array<T, Tag> &z) {
        typedef lazy_unary_tag<__math_conj, T, Tag> Closure;
        return base_array<T, Closure>(__math_conj(), z);
    }

    /**
     * @brief Return the complex conjugate, element-wise. The complex conjugate
     * of a complex number is obtained by changing the sign of its imaginary
     * part.
     *
     * @param z A matrix-like object with the values whose complex conjugate is
     *     computed.
     *
     * @return A light-weight object with the complex conjugate of each
     *     element in the matrix. If T is not a complex type, return the
     *     elements unchanged. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, lazy_unary_tag<__math_conj, T, Tag> >
    conj(const base_matrix<T, Tag> &z) {
        typedef lazy_unary_tag<__math_conj, T, Tag> Closure;
        return base_matrix<T, Closure>(__math_conj(), z);
    }

    /// Clasification functions.

    /**
     * @brief Return whether x is a finite value (neither infinite nor NaN),
     * element-wise.
     *
     * @param x An array-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     finite and false otherwise. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< bool, lazy_unary_tag<__math_isfinite, T, Tag> >
    isfinite(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_isfinite, T, Tag> Closure;
        return base_array<bool, Closure>(__math_isfinite(), x);
    }

    /**
     * @brief Return whether x is a finite value (neither infinite nor NaN),
     * element-wise.
     *
     * @param x A matrix-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     finite and false otherwise. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< bool, lazy_unary_tag<__math_isfinite, T, Tag> >
    isfinite(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_isfinite, T, Tag> Closure;
        return base_matrix<bool, Closure>(__math_isfinite(), x);
    }

    /**
     * @brief Return whether x is an infinity value (either positive infinity
     * or negative infinity), element-wise.
     *
     * @param x An array-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     infinity and false otherwise. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< bool, lazy_unary_tag<__math_isinf, T, Tag> >
    isinf(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_isinf, T, Tag> Closure;
        return base_array<bool, Closure>(__math_isinf(), x);
    }

    /**
     * @brief Return whether x is an infinity value (either positive infinity
     * or negative infinity), element-wise.
     *
     * @param x A matrix-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     infinity and false otherwise. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< bool, lazy_unary_tag<__math_isinf, T, Tag> >
    isinf(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_isinf, T, Tag> Closure;
        return base_matrix<bool, Closure>(__math_isinf(), x);
    }

    /**
     * @brief Return whether x is a NaN (Not a Number) value, element-wise. The
     * NaN values are used to identify undefined or non-representable values
     * for floating-point numbers, such as the square root of negative numbers
     * or the result of 0/0.
     *
     * @param x An array-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     NaN and false otherwise. Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< bool, lazy_unary_tag<__math_isnan, T, Tag> >
    isnan(const base_array<T, Tag> &x) {
        typedef lazy_unary_tag<__math_isnan, T, Tag> Closure;
        return base_array<bool, Closure>(__math_isnan(), x);
    }

    /**
     * @brief Return whether x is a NaN (Not a Number) value, element-wise. The
     * NaN values are used to identify undefined or non-representable values
     * for floating-point numbers, such as the square root of negative numbers
     * or the result of 0/0.
     *
     * @param x A matrix-like object with floating point values.
     *
     * @return A light-weight object with each element set to true where x is
     *     NaN and false otherwise. Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< bool, lazy_unary_tag<__math_isnan, T, Tag> >
    isnan(const base_matrix<T, Tag> &x) {
        typedef lazy_unary_tag<__math_isnan, T, Tag> Closure;
        return base_matrix<bool, Closure>(__math_isnan(), x);
    }
}

#endif // NUMCPP_MATH_H_INCLUDED
