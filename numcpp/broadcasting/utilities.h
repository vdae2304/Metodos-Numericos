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

 /** @file include/numcpp/broadcasting/utilities.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_UTILITIES_H_INCLUDED
#define NUMCPP_UTILITIES_H_INCLUDED

#include <utility>

namespace numcpp {

/// Namespace for implementation details.
namespace detail {
    /**
     * @brief Function object implementing zip.
     */
    struct zip {
        template <class T, class U>
        std::pair<T, U> operator()(const T &arg1, const U &arg2) const {
            return std::make_pair(arg1, arg2);
        }
    };

    /**
     * @brief Function object implementing unzip.
     */
    template <size_t I>
    struct unzip {
        template <class T, class U>
        typename std::tuple_element<I, std::pair<T, U> >::type
        operator()(const std::pair<T, U> &arg) const {
            return std::get<I>(arg);
        }
    };
}
}

#endif // NUMCPP_UTILITIES_H_INCLUDED
