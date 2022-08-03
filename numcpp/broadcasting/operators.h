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

 /** @file include/numcpp/broadcasting/operators.h
  *  This header defines function objects for operators.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_OPERATORS_H_INCLUDED
#define NUMCPP_OPERATORS_H_INCLUDED

#include <complex>
#include <utility>

namespace numcpp {
    /**
     * @brief Function object that returns arg unchanged.
     */
    struct identity {
        template <class T>
        constexpr T&& operator()(T &&arg) const {
            return std::forward<T>(arg);
        }
    };

    /**
     * @brief Function object implementing +arg.
     */
    struct unary_plus {
        template <class T>
        constexpr auto operator()(T &&arg) const
         -> decltype(+std::forward<T>(arg)) {
            return +std::forward<T>(arg);
        }
    };

    /**
     * @brief Function object implementing -arg.
     */
    struct negate {
        template <class T>
        constexpr auto operator()(T &&arg) const
         -> decltype(-std::forward<T>(arg)) {
            return -std::forward<T>(arg);
        }
    };

    /**
     * @brief Function object implementing lhs + rhs.
     */
    struct plus {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) + std::forward<U>(rhs)) {
            return std::forward<T>(lhs) + std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs - rhs.
     */
    struct minus {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) - std::forward<U>(rhs)) {
            return std::forward<T>(lhs) - std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs * rhs.
     */
    struct multiplies {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) * std::forward<U>(rhs)) {
            return std::forward<T>(lhs) * std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs / rhs.
     */
    struct divides {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) / std::forward<U>(rhs)) {
            return std::forward<T>(lhs) / std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs % rhs.
     */
    struct modulus {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) % std::forward<U>(rhs)) {
            return std::forward<T>(lhs) % std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing ~arg.
     */
    struct bit_not {
        template <class T>
        constexpr auto operator()(T &&arg) const
         -> decltype(~std::forward<T>(arg)) {
            return ~std::forward<T>(arg);
        }
    };

    /**
     * @brief Function object implementing lhs & rhs.
     */
    struct bit_and {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) & std::forward<U>(rhs)) {
            return std::forward<T>(lhs) & std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs | rhs.
     */
    struct bit_or {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) | std::forward<U>(rhs)) {
            return std::forward<T>(lhs) | std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs ^ rhs.
     */
    struct bit_xor {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) ^ std::forward<U>(rhs)) {
            return std::forward<T>(lhs) ^ std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs << rhs.
     */
    struct left_shift {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) << std::forward<U>(rhs)) {
            return std::forward<T>(lhs) << std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs >> rhs.
     */
    struct right_shift {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) >> std::forward<U>(rhs)) {
            return std::forward<T>(lhs) >> std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing !arg.
     */
    struct logical_not {
        template <class T>
        constexpr auto operator()(T &&arg) const
         -> decltype(!std::forward<T>(arg)) {
            return !std::forward<T>(arg);
        }
    };

    /**
     * @brief Function object implementing lhs && rhs.
     */
    struct logical_and {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) && std::forward<U>(rhs)) {
            return std::forward<T>(lhs) && std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs || rhs.
     */
    struct logical_or {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) || std::forward<U>(rhs)) {
            return std::forward<T>(lhs) || std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs == rhs.
     */
    struct equal_to {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) == std::forward<U>(rhs)) {
            return std::forward<T>(lhs) == std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs != rhs.
     */
    struct not_equal_to {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) != std::forward<U>(rhs)) {
            return std::forward<T>(lhs) != std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs < rhs.
     */
    struct less {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) < std::forward<U>(rhs)) {
            return std::forward<T>(lhs) < std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs > rhs.
     */
    struct greater {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) > std::forward<U>(rhs)) {
            return std::forward<T>(lhs) > std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs <= rhs.
     */
    struct less_equal {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) <= std::forward<U>(rhs)) {
            return std::forward<T>(lhs) <= std::forward<U>(rhs);
        }
    };

    /**
     * @brief Function object implementing lhs <= rhs.
     */
    struct greater_equal {
        template <class T, class U>
        constexpr auto operator()(T &&lhs, U &&rhs) const
         -> decltype(std::forward<T>(lhs) >= std::forward<U>(rhs)) {
            return std::forward<T>(lhs) >= std::forward<U>(rhs);
        }
    };

/// Namespace for implementation details.
namespace detail {
    /**
     * @brief Type traits for complex numbers.
     */
    template <class T>
    struct complex_traits {
        typedef T value_type;
    };

    template <class T>
    struct complex_traits< std::complex<T> > {
        typedef T value_type;
    };

    template <class T>
    using complex_value_type = typename complex_traits<T>::value_type;
}

    /**
     * @brief Function object returning real part.
     */
    struct math_real {
        template <class T>
        T operator()(T &&x) const {
            return x;
        }

        template <class T>
        T operator()(const std::complex<T> &z) const {
            return z.real();
        }
    };

    /**
     * @brief Function object returning imaginary part.
     */
    struct math_imag {
        template <class T>
        T operator()(T&&) const {
            return T();
        }

        template <class T>
        T operator()(const std::complex<T> &z) const {
            return z.imag();
        }
    };

    /**
     * @brief Function object implementing std::conj.
     */
    struct math_conj {
        template <class T>
        T operator()(T &&x) const {
            return x;
        }

        template <class T>
        std::complex<T> operator()(const std::complex<T> &z) const {
            return std::conj(z);
        }
    };
}

#endif // NUMCPP_OPERATORS_H_INCLUDED
