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

 /** @file include/numcpp/math/constants.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/math.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATH_CONSTANTS_H_INCLUDED
#define NUMCPP_MATH_CONSTANTS_H_INCLUDED

#include <limits>

namespace numcpp {
    template <class T>
    class constants {
    public:
        /// Floating point representation of (positive) infinity.
        static constexpr T inf = std::numeric_limits<T>::infinity();

        /// Floating point representation of quiet NaN (Not a Number).
        static constexpr T nan = std::numeric_limits<T>::quiet_NaN();

        /// Euler's constant, base of natural logarithm.
        static constexpr T e = T(2.7182818284590452353602874713526625L);

        /// Pi, ratio of a circle's circunferemce to its diameter.
        static constexpr T pi = T(3.1415926535897932384626433832795029L);

        /// Golden ratio.
        static constexpr T golden = T(1.6180339887498948482045868343656381L);

        /// Euler–Mascheroni's constant.
        static constexpr T egamma = T(0.5772156649015328606065120900824024L);
    };
};

#endif // NUMCPP_MATH_CONSTANTS_H_INCLUDED