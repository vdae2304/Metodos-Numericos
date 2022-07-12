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

 /** @file include/numcpp/math/mathfwd.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/math.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATHFWD_H_INCLUDED
#define NUMCPP_MATHFWD_H_INCLUDED

#include <cmath>
#include <complex>
#include <type_traits>

namespace numcpp {
    /// Basic functions
    using std::abs;
    using std::fmod;
    using std::max;
    using std::min;

    /// Trigonometric functions
    using std::cos;
    using std::sin;
    using std::tan;
    using std::acos;
    using std::asin;
    using std::atan;
    using std::atan2;
    using std::hypot;

    /// Hyperbolic functions
    using std::cosh;
    using std::sinh;
    using std::tanh;
    using std::acosh;
    using std::asinh;
    using std::atanh;

    /// Exponential and logarithmic functions
    using std::exp;
    using std::log;
    using std::log10;
    using std::exp2;
    using std::log2;
    using std::expm1;
    using std::log1p;

    /// Power functions
    using std::pow;
    using std::sqrt;
    using std::cbrt;

    /// Rounding
    using std::ceil;
    using std::floor;
    using std::trunc;
    using std::round;

    /// Floating-point manipulation functions
    using std::frexp;
    using std::ldexp;
    using std::copysign;
    using std::nextafter;

    /// Complex numbers
    using std::real;
    using std::imag;
    using std::arg;
    using std::conj;

    /// Clasification functions
    using std::isfinite;
    using std::isinf;
    using std::isnan;

    /// Function object implementing std::abs.
    struct __math_abs {
        template <class T>
        T operator()(const T &x) const {
            return std::abs(x);
        }

        template <class T>
        T operator()(const std::complex<T> &x) const {
            return std::abs(x);
        }
    };

    /// Function object implementing std::fmod.
    struct __math_fmod {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::fmod(x, y);
        }
    };

    /// Function object implementing std::cos.
    struct __math_cos {
        template <class T>
        T operator()(const T &x) const {
            return std::cos(x);
        }
    };

    /// Function object implementing std::sin.
    struct __math_sin {
        template <class T>
        T operator()(const T &x) const {
            return std::sin(x);
        }
    };

    /// Function object implementing std::tan.
    struct __math_tan {
        template <class T>
        T operator()(const T &x) const {
            return std::tan(x);
        }
    };

    /// Function object implementing std::acos.
    struct __math_acos {
        template <class T>
        T operator()(const T &x) const {
            return std::acos(x);
        }
    };

    /// Function object implementing std::asin.
    struct __math_asin {
        template <class T>
        T operator()(const T &x) const {
            return std::asin(x);
        }
    };

    /// Function object implementing std::atan.
    struct __math_atan {
        template <class T>
        T operator()(const T &x) const {
            return std::atan(x);
        }
    };

    /// Function object implementing std::atan2.
    struct __math_atan2 {
        template <class T>
        T operator()(const T &y, const T &x) const {
            return std::atan2(y, x);
        }
    };

    /// Function object implementing std::hypot.
    struct __math_hypot {
        template <class T>
        T operator()(const T &x, const T &y) const {
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
        return 180.0*x*inv_pi;
    }

    float degrees(float x) {
        float inv_pi = 0.318309886f;
        return 180.0f*x*inv_pi;
    }

    long double degrees(long double x) {
        long double inv_pi = 0.318309886183790671537767526745028L;
        return 180.0L*x*inv_pi;
    }

    template <class T>
    typename std::enable_if<std::is_integral<T>::value, double>::type
    degrees(T x) {
        return degrees((double)x);
    }

    /// Function object implementing numcpp::degrees.
    struct __math_degrees {
        template <class T>
        T operator()(const T &x) const {
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
        return pi*x/180.0;
    }

    float radians(float x) {
        float pi = 3.141592654f;
        return pi*x/180.0f;
    }

    long double radians(long double x) {
        long double pi = 3.141592653589793238462643383279502L;
        return pi*x/180.0L;
    }

    template <class T>
    typename std::enable_if<std::is_integral<T>::value, double>::type
    radians(T x) {
        return radians((double)x);
    }

    /// Function object implementing numcpp::radians.
    struct __math_radians {
        template <class T>
        T operator()(const T &x) const {
            return radians(x);
        }
    };

    /// Function object implementing std::cosh.
    struct __math_cosh {
        template <class T>
        T operator()(const T &x) const {
            return std::cosh(x);
        }
    };

    /// Function object implementing std::sinh.
    struct __math_sinh {
        template <class T>
        T operator()(const T &x) const {
            return std::sinh(x);
        }
    };

    /// Function object implementing std::tanh.
    struct __math_tanh {
        template <class T>
        T operator()(const T &x) const {
            return std::tanh(x);
        }
    };

    /// Function object implementing std::acosh.
    struct __math_acosh {
        template <class T>
        T operator()(const T &x) const {
            return std::acosh(x);
        }
    };

    /// Function object implementing std::asinh.
    struct __math_asinh {
        template <class T>
        T operator()(const T &x) const {
            return std::asinh(x);
        }
    };

    /// Function object implementing std::atanh.
    struct __math_atanh {
        template <class T>
        T operator()(const T &x) const {
            return std::atanh(x);
        }
    };

    /// Function object implementing std::exp.
    struct __math_exp {
        template <class T>
        T operator()(const T &x) const {
            return std::exp(x);
        }
    };

    /// Function object implementing std::log.
    struct __math_log {
        template <class T>
        T operator()(const T &x) const {
            return std::log(x);
        }
    };

    /// Function object implementing std::log10.
    struct __math_log10 {
        template <class T>
        T operator()(const T &x) const {
            return std::log10(x);
        }
    };

    /// Function object implementing std::exp2.
    struct __math_exp2 {
        template <class T>
        T operator()(const T &x) const {
            return std::exp2(x);
        }
    };

    /// Function object implementing std::log2.
    struct __math_log2 {
        template <class T>
        T operator()(const T &x) const {
            return std::log2(x);
        }
    };

    /// Function object implementing std::expm1.
    struct __math_expm1 {
        template <class T>
        T operator()(const T &x) const {
            return std::expm1(x);
        }
    };

    /// Function object implementing std::log1p.
    struct __math_log1p {
        template <class T>
        T operator()(const T &x) const {
            return std::log1p(x);
        }
    };

    /// Function object implementing std::pow.
    struct __math_pow {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::pow(x, y);
        }
    };

    /// Function object implementing std::sqrt.
    struct __math_sqrt {
        template <class T>
        T operator()(const T &x) const {
            return std::sqrt(x);
        }
    };

    /// Function object implementing std::cbrt.
    struct __math_cbrt {
        template <class T>
        T operator()(const T &x) const {
            return std::cbrt(x);
        }
    };

    /// Function object implementing std::ceil.
    struct __math_ceil {
        template <class T>
        T operator()(const T &x) const {
            return std::ceil(x);
        }
    };

    /// Function object implementing std::floor.
    struct __math_floor {
        template <class T>
        T operator()(const T &x) const {
            return std::floor(x);
        }
    };

    /// Function object implementing std::trunc.
    struct __math_trunc {
        template <class T>
        T operator()(const T &x) const {
            return std::trunc(x);
        }
    };

    /// Function object implementing std::round.
    struct __math_round {
        template <class T>
        T operator()(const T &x) const {
            return std::round(x);
        }
    };

    /// Function object implementing std::frexp.
    struct __math_frexp {
        template <class T>
        std::pair<T, int> operator()(const T &x) const {
            int exp;
            T mantissa = std::frexp(x, &exp);
            return std::make_pair(mantissa, exp);
        }
    };

    /// Function object implementing std::ldexp.
    struct __math_ldexp {
        template <class T>
        T operator()(const T &x, int exp) const {
            return std::ldexp(x, exp);
        }
    };

    /// Function object implementing std::copysign.
    struct __math_copysign {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::copysign(x, y);
        }
    };

    /// Function object implementing std::nextafter.
    struct __math_nextafter {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::nextafter(x, y);
        }
    };

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

    /// Function object implementing numcpp::gcd.
    struct __math_gcd {
        template <class T>
        T operator()(const T &m, const T &n) const {
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

    /// Function object implementing numcpp::lcm.
    struct __math_lcm {
        template <class T>
        T operator()(const T &m, const T &n) const {
            return lcm(m, n);
        }
    };

    /// Function object implementing arg.real().
    struct __math_real {
        template <class T>
        T operator()(const T &arg) const {
            return arg;
        }

        template <class T>
        T operator()(const std::complex<T> &arg) const {
            return arg.real();
        }
    };

    /// Function object implementing arg.imag().
    struct __math_imag {
        template <class T>
        T operator()(const T&) const {
            return 0;
        }

        template <class T>
        T operator()(const std::complex<T> &arg) const {
            return arg.imag();
        }
    };

    /// Function object implementing std::conj.
    struct __math_conj {
        template <class T>
        T operator()(const T &arg) const {
            return arg;
        }

        template <class T>
        std::complex<T> operator()(const std::complex<T> &arg) const {
            return std::conj(arg);
        }
    };

    /// Function object implementing std::arg.
    struct __math_arg {
        template <class T>
        T operator()(const T &z) const {
            return std::arg(z);
        }

        template <class T>
        T operator()(const std::complex<T> &z) const {
            return std::arg(z);
        }
    };

    /// Function object implementing std::isfinite.
    struct __math_isfinite {
        template <class T>
        bool operator()(const T &x) const {
            return std::isfinite(x);
        }
    };

    /// Function object implementing std::isinf.
    struct __math_isinf {
        template <class T>
        bool operator()(const T &x) const {
            return std::isinf(x);
        }
    };

    /// Function object implementing std::isnan.
    struct __math_isnan {
        template <class T>
        bool operator()(const T &x) const {
            return std::isnan(x);
        }
    };
}

#endif // NUMCPP_MATHFWD_H_INCLUDED
