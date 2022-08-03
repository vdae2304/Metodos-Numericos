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

 /** @file include/numcpp/math/mathfwd.h
  *  This header defines function objects for several mathematical functions.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATHFWD_H_INCLUDED
#define NUMCPP_MATHFWD_H_INCLUDED

#include <cmath>
#include <complex>
#include <type_traits>
#include <utility>

namespace numcpp {
    /// Basic functions.

    /**
     * @brief Function object implementing std::abs.
     */
    struct math_abs {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::abs(std::forward<T>(x))) {
            return std::abs(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::max.
     */
    struct math_max {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::max(x, y);
        }
    };

    /**
     * @brief Function object implementing std::min.
     */
    struct math_min {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::min(x, y);
        }
    };

    /**
     * @brief Function object implementing std::fmod.
     */
    struct math_fmod {
        template <class T, class U>
        auto operator()(T x, U y) const -> decltype(std::fmod(x, y)) {
            return std::fmod(x, y);
        }
    };

    /// Trigonometric functions.

    /**
     * @brief Function object implementing std::cos.
     */
    struct math_cos {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::cos(std::forward<T>(x))) {
            return std::cos(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::sin.
     */
    struct math_sin {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::sin(std::forward<T>(x))) {
            return std::sin(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::tan.
     */
    struct math_tan {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::tan(std::forward<T>(x))) {
            return std::tan(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::acos.
     */
    struct math_acos {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::acos(std::forward<T>(x))) {
            return std::acos(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::asin.
     */
    struct math_asin {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::asin(std::forward<T>(x))) {
            return std::asin(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::atan.
     */
    struct math_atan {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::atan(std::forward<T>(x))) {
            return std::atan(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::atan2.
     */
    struct math_atan2 {
        template <class T, class U>
        auto operator()(T y, U x) const -> decltype(std::atan2(y, x)) {
            return std::atan2(y, x);
        }
    };

    /**
     * @brief Function object implementing std::hypot.
     */
    struct math_hypot {
        template <class T, class U>
        auto operator()(T x, U y) const -> decltype(std::hypot(x, y)) {
            return std::hypot(x, y);
        }
    };

    /**
     * @brief Convert angles from radians to degrees.
     *
     * @param x Angle in radians.
     *
     * @return The corresponding angle in degrees.
     */
    double degrees(double x) {
        double inv_pi = 0.31830988618379067;
        return 180.0 * x * inv_pi;
    }

    float degrees(float x) {
        float inv_pi = 0.318309886f;
        return 180.0f * x * inv_pi;
    }

    long double degrees(long double x) {
        long double inv_pi = 0.318309886183790671537767526745028L;
        return 180.0L * x * inv_pi;
    }

    template <class T>
    typename std::enable_if<std::is_integral<T>::value, double>::type
    degrees(T x) {
        return degrees((double)x);
    }

    /**
     * @brief Function object implementing numcpp::degrees.
     */
    struct math_degrees {
        template <class T>
        auto operator()(T x) const -> decltype(degrees(x)) {
            return degrees(x);
        }
    };

    /**
     * @brief Convert angles from degrees to radians.
     *
     * @param x Angle in degrees.
     *
     * @return The corresponding angle in radians.
     */
    double radians(double x) {
        double pi = 3.14159265358979324;
        return pi * x / 180.0;
    }

    float radians(float x) {
        float pi = 3.141592654f;
        return pi * x / 180.0f;
    }

    long double radians(long double x) {
        long double pi = 3.141592653589793238462643383279502L;
        return pi * x / 180.0L;
    }

    template <class T>
    typename std::enable_if<std::is_integral<T>::value, double>::type
    radians(T x) {
        return radians((double)x);
    }

    /**
     * @brief Function object implementing numcpp::radians.
     */
    struct math_radians {
        template <class T>
        auto operator()(T x) const -> decltype(radians(x)) {
            return radians(x);
        }
    };

    /// Hyperbolic functions.

    /**
     * @brief Function object implementing std::cosh.
     */
    struct math_cosh {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::cosh(std::forward<T>(x))) {
            return std::cosh(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::sinh.
     */
    struct math_sinh {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::sinh(std::forward<T>(x))) {
            return std::sinh(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::tanh.
     */
    struct math_tanh {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::tanh(std::forward<T>(x))) {
            return std::tanh(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::acosh.
     */
    struct math_acosh {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::acosh(std::forward<T>(x))) {
            return std::acosh(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::asinh.
     */
    struct math_asinh {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::asinh(std::forward<T>(x))) {
            return std::asinh(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::atanh.
     */
    struct math_atanh {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::atanh(std::forward<T>(x))) {
            return std::atanh(std::forward<T>(x));
        }
    };

    /// Exponential and logarithmic functions.

    /**
     * @brief Function object implementing std::exp.
     */
    struct math_exp {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::exp(std::forward<T>(x))) {
            return std::exp(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::log.
     */
    struct math_log {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::log(std::forward<T>(x))) {
            return std::log(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::log10.
     */
    struct math_log10 {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::log10(std::forward<T>(x))) {
            return std::log10(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::exp2.
     */
    struct math_exp2 {
        template <class T>
        auto operator()(T x) const -> decltype(std::exp2(x)) {
            return std::exp2(x);
        }
    };

    /**
     * @brief Function object implementing std::log2.
     */
    struct math_log2 {
        template <class T>
        auto operator()(T x) const -> decltype(std::log2(x)) {
            return std::log2(x);
        }
    };

    /**
     * @brief Function object implementing std::expm1.
     */
    struct math_expm1 {
        template <class T>
        auto operator()(T x) const -> decltype(std::expm1(x)) {
            return std::expm1(x);
        }
    };

    /**
     * @brief Function object implementing std::log1p.
     */
    struct math_log1p {
        template <class T>
        auto operator()(T x) const -> decltype(std::log1p(x)) {
            return std::log1p(x);
        }
    };

    /// Power functions.

    /**
     * @brief Function object implementing std::pow.
     */
    struct math_pow {
        template <class T, class U>
        auto operator()(T &&x, U &&y) const
         -> decltype(std::pow(std::forward<T>(x), std::forward<U>(y))) {
            return std::pow(std::forward<T>(x), std::forward<U>(y));
        }
    };

    /**
     * @brief Function object implementing std::sqrt.
     */
    struct math_sqrt {
        template <class T>
        auto operator()(T &&x) const
         -> decltype(std::sqrt(std::forward<T>(x))) {
            return std::sqrt(std::forward<T>(x));
        }
    };

    /**
     * @brief Function object implementing std::cbrt.
     */
    struct math_cbrt {
        template <class T>
        auto operator()(T x) const -> decltype(std::cbrt(x)) {
            return std::cbrt(x);
        }
    };

    /// Rounding.

    /**
     * @brief Function object implementing std::ceil.
     */
    struct math_ceil {
        template <class T>
        auto operator()(T x) const -> decltype(std::ceil(x)) {
            return std::ceil(x);
        }
    };

    /**
     * @brief Function object implementing std::floor.
     */
    struct math_floor {
        template <class T>
        auto operator()(T x) const -> decltype(std::floor(x)) {
            return std::floor(x);
        }
    };

    /**
     * @brief Function object implementing std::trunc.
     */
    struct math_trunc {
        template <class T>
        auto operator()(T x) const -> decltype(std::trunc(x)) {
            return std::trunc(x);
        }
    };

    /**
     * @brief Function object implementing std::round.
     */
    struct math_round {
        template <class T>
        auto operator()(T x) const -> decltype(std::round(x)) {
            return std::round(x);
        }
    };

    /// Floating-point manipulation functions.

    /**
     * @brief Function object implementing std::frexp.
     */
    struct math_frexp {
        template <class T>
        auto operator()(T x) const
         -> std::pair<decltype(std::frexp(x, std::declval<int*>())), int> {
            int exp;
            auto mantissa = std::frexp(x, &exp);
            return std::make_pair(mantissa, exp);
        }
    };

    /**
     * @brief Function object implementing std::ldexp.
     */
    struct math_ldexp {
        template <class T>
        auto operator()(T x, int exp) const -> decltype(std::ldexp(x, exp)) {
            return std::ldexp(x, exp);
        }
    };

    /**
     * @brief Function object implementing std::copysign.
     */
    struct math_copysign {
        template <class T, class U>
        auto operator()(T x, U y) const -> decltype(std::copysign(x, y)) {
            return std::copysign(x, y);
        }
    };

    /**
     * @brief Function object implementing std::nextafter.
     */
    struct math_nextafter {
        template <class T, class U>
        auto operator()(T x, U y) const -> decltype(std::nextafter(x, y)) {
            return std::nextafter(x, y);
        }
    };

    /// Integer-valued functions.

    /**
     * @brief Return the greatest common divisor of |m| and |n|.
     *
     * @param m An integer value.
     * @param n An integer value.
     *
     * @return The greatest common divisor of |m| and |n|. If both m and n are
     *     zero, return zero.
     */
    template <class T1, class T2>
    typename std::common_type<
        typename std::enable_if<std::is_integral<T1>::value, T1>::type,
        typename std::enable_if<std::is_integral<T2>::value, T2>::type
    >::type
    gcd(T1 m, T2 n) {
        typedef typename std::common_type<T1, T2>::type T;
        while (n != 0) {
            T mod = m % n;
            m = n;
            n = mod;
        }
        return std::abs(m);
    }

    /**
     * @brief Function object implementing numcpp::gcd.
     */
    struct math_gcd {
        template <class T, class U>
        auto operator()(T m, U n) const -> decltype(gcd(m, n)) {
            return gcd(m, n);
        }
    };

    /**
     * @brief Return the least common multiple of |m| and |n|.
     *
     * @param m An integer value.
     * @param n An integer value.
     *
     * @return The least common multiple of |m| and |n|. If either m or n is
     *     zero, return zero.
     */
    template <class T1, class T2>
    typename std::common_type<
        typename std::enable_if<std::is_integral<T1>::value, T1>::type,
        typename std::enable_if<std::is_integral<T2>::value, T2>::type
    >::type
    lcm(T1 m, T2 n) {
        if (m != 0 && n != 0) {
            return (std::abs(m) / gcd(m, n)) * std::abs(n);
        }
        else {
            return 0;
        }
    }

    /**
     * @brief Function object implementing numcpp::lcm.
     */
    struct math_lcm {
        template <class T, class U>
        auto operator()(T m, U n) const -> decltype(lcm(m, n)) {
            return lcm(m, n);
        }
    };

    /// Complex numbers.

    /**
     * @brief Function object implementing std::arg.
     */
    struct math_arg {
        template <class T>
        auto operator()(T &&z) const
         -> decltype(std::arg(std::forward<T>(z))) {
            return std::arg(std::forward<T>(z));
        }
    };

    /// Clasification functions.

    /**
     * @brief Function object implementing std::isfinite.
     */
    struct math_isfinite {
        template <class T>
        bool operator()(T x) const {
            return std::isfinite(x);
        }
    };

    /**
     * @brief Function object implementing std::isinf.
     */
    struct math_isinf {
        template <class T>
        bool operator()(T x) const {
            return std::isinf(x);
        }
    };

    /**
     * @brief Function object implementing std::isnan.
     */
    struct math_isnan {
        template <class T>
        bool operator()(T x) const {
            return std::isnan(x);
        }
    };
}

#endif // NUMCPP_MATHFWD_H_INCLUDED
