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
            return std::pair<T, U>(arg1, arg2);
        }
    };

    /**
     * @brief Function object implementing unzip.
     */
    template <size_t I>
    struct unzip;

    template <>
    struct unzip<0> {
        template <class T, class U>
        T operator()(const std::pair<T, U> &arg) const {
            return arg.first;
        }
    };

    template <>
    struct unzip<1> {
        template <class T, class U>
        U operator()(const std::pair<T, U> &arg) const {
            return arg.second;
        }
    };

    /**
     * @brief Function object implementing np::ravel_index.
     */
    template <size_t Rank>
    struct ravel_index {
        // Shape used for raveling.
        shape_t<Rank> shape;

        // Whether the indices should be viewed as indexing in row-major order
        // or column-major order
        bool order;

        /**
         * @brief Constructor.
         */
        ravel_index(const shape_t<Rank> &shape, bool order = true)
         : shape(shape), order(order) {}

        /**
         * @brief Converts a tuple of indices into a flat index.
         *
         * @param index A tuple of indices to flatten.
         */
        size_t operator()(const index_t<Rank> &index) const {
            return numcpp::ravel_index(index, shape, order);
        }
    };

    /**
     * @brief Function object implementing np::unravel_index.
     */
    template <size_t Rank>
    struct unravel_index {
        // Shape used for unraveling.
        shape_t<Rank> shape;

        // Whether the indices should be viewed as indexing in row-major order
        // or column-major order
        bool order;

        /**
         * @brief Constructor.
         */
        unravel_index(const shape_t<Rank> &shape, bool order = true)
         : shape(shape), order(order) {}

        /**
         * @brief Converts a flat index into a tuple of indices.
         *
         * @param index Index to unravel.
         */
        index_t<Rank> operator()(size_t index) const {
            return numcpp::unravel_index(index, shape, order);
        }
    };
}
}

#endif // NUMCPP_UTILITIES_H_INCLUDED
