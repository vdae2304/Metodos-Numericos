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

 /** @file include/numcpp/math/complexfwd.h
  *  This header defines function objects for several complex-valued
  *  mathematical functions.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_COMPLEXFWD_H_INCLUDED
#define NUMCPP_COMPLEXFWD_H_INCLUDED

#include <complex>
#include <utility>

namespace numcpp {

/// Namespace for mathematical functions.
namespace math {
    /**
     * @brief Function object implementing std::real.
     */
    struct real {
        template <class T>
        auto operator()(T&& z) const
         -> decltype(std::real(std::forward<T>(z))) {
            return std::real(std::forward<T>(z));
        }
    };

    /**
     * @brief Function object implementing std::imag.
     */
    struct imag {
        template <class T>
        auto operator()(T&& z) const
         -> decltype(std::imag(std::forward<T>(z))) {
            return std::imag(std::forward<T>(z));
        }
    };

    /**
     * @brief Function object implementing std::conj.
     */
    struct conj {
        template <class T>
        auto operator()(T&& z) const
         -> decltype(std::conj(std::forward<T>(z))) {
            return std::conj(std::forward<T>(z));
        }
    };

    /**
     * @brief Function object implementing std::arg.
     */
    struct arg {
        template <class T>
        auto operator()(T &&z) const
         -> decltype(std::arg(std::forward<T>(z))) {
            return std::arg(std::forward<T>(z));
        }
    };
}
}

#endif // NUMCPP_COMPLEXFWD_H_INCLUDED
