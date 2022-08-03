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

 /** @file include/numcpp/broadcasting/utilities.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_RAVEL_H_INCLUDED
#define NUMCPP_MATRIX_RAVEL_H_INCLUDED

#include <utility>

namespace numcpp {
    /// Function object implementing zip.
    struct __zip {
        template <class T, class U>
        std::pair<T, U> operator()(const T &arg1, const U &arg2) const {
            return std::make_pair(arg1, arg2);
        }
    };

    /// Function object implementing unzip.
    template <size_t I>
    struct __unzip {
        template <class T, class U>
        typename std::tuple_element< I, std::pair<T, U> >::type
        operator()(const std::pair<T, U> &arg) const {
            return std::get<I>(arg);
        }
    };

    /// Function object implementing ravel_multi_index.
    struct __ravel_multi_index {
        /// Trailing dimension.
        size_t tda;

        /// Whether the elements are stored in row-major or column-major order.
        bool order;

        /**
         * @brief Constructor.
         *
         * @param m Number of rows.
         * @param n Number of columns.
         * @param row_major Whether the elements are assumed to be stored in
         *     row-major or column-major order. Defaults to row-major order.
         */
        __ravel_multi_index(size_t m, size_t n, bool row_major = true) {
            tda = row_major ? n : m;
            order = row_major;
        }

        /**
         * @brief Converts a pair of indices into a flat index.
         *
         * @param i, j A pair of indices.
         *
         * @return The flattened index.
         */
        size_t operator()(size_t i, size_t j) const {
            if (order) {
                return i * tda + j;
            }
            else {
                return j * tda + i;
            }
        }

        /**
         * @brief Converts a pair of indices into a flat index.
         *
         * @param multi_index A pair of indices.
         *
         * @return The flattened index.
         */
        size_t operator()(const index_t &multi_index) const {
            return this->operator()(multi_index.first, multi_index.second);
        }
    };

    /// Function object implementing unravel_index.
    struct __unravel_index {
        /// Trailing dimension.
        size_t tda;

        /// Whether the elements are stored in row-major or column-major order.
        bool order;

        /**
         * @brief Constructor.
         *
         * @param m Number of rows.
         * @param n Number of columns.
         * @param row_major Whether the elements are assumed to be stored in
         *     row-major or column-major order. Defaults to row-major order.
         */
        __unravel_index(size_t m, size_t n, bool row_major = true) {
            tda = row_major ? n : m;
            order = row_major;
        }

        /**
         * @brief Converts a flat index into a pair of indices.
         *
         * @param index A flat index.
         *
         * @return The unraveled index.
         */
        index_t operator()(size_t index) const {
            size_t i, j;
            if (order) {
                i = index / tda;
                j = index % tda;
            }
            else {
                i = index % tda;
                j = index / tda;
            }
            return index_t(i, j);
        }
    };
}

#endif // NUMCPP_MATRIX_RAVEL_H_INCLUDED
