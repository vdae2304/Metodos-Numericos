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

 /** @file include/numcpp/config.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_CONFIG_H_INCLUDED
#define NUMCPP_CONFIG_H_INCLUDED

#if __cplusplus < 201103L
#error This file requires compiler and library support for the ISO C++ 2011 \
standard. This support must be enabled with the -std=c++11 or -std=gnu++11 \
compiler options.
#else

#include <complex>
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace numcpp {
    using std::size_t;
    using std::ptrdiff_t;
    using std::nullptr_t;
    typedef std::pair<size_t, size_t> index_t;

    /// Forward declarations.
    struct scalar_tag;

    template <class T, class Tag> class base_array;
    struct array_tag;
    struct array_view_tag;
    struct index_view_tag;

    template <class T>
    using array = base_array<T, array_tag>;
    template <class T>
    using array_view = base_array<T, array_view_tag>;
    template <class T>
    using index_view = base_array<T, index_view_tag>;

    template <class T, class Tag> class base_matrix;
    struct matrix_tag;
    struct matrix_view_tag;

    template <class T>
    using matrix = base_matrix<T, matrix_tag>;
    template <class T>
    using matrix_view = base_matrix<T, matrix_view_tag>;

    template <class Function, class T, class Tag>
    struct lazy_unary_tag;
    template <class Function, class T1, class Tag1, class T2, class Tag2>
    struct lazy_binary_tag;
    template <class Function, class T, class Tag>
    struct lazy_axis_tag;

    /// Concept for input iterator templates.
    template <class InputIterator>
    using RequiresInputIterator = typename std::enable_if<
        std::is_convertible<
            typename std::iterator_traits<InputIterator>::iterator_category,
            std::input_iterator_tag
        >::value
    >::type;

    /// Type traits for complex types.
    template <class T>
    struct complex_traits {
        typedef T value_type;
    };

    template <class T>
    struct complex_traits< std::complex<T> > {
        typedef T value_type;
    };

    /// Asserts whether two arrays have equal lengths.
    inline void __assert_equal_length(size_t size1, size_t size2) {
        if (size1 != size2) {
            char error[100];
            sprintf(
                error, "operands could not be broadcast together with shapes "
                "(%zu,) (%zu,)", size1, size2
            );
            throw std::invalid_argument(error);
        }
    }

    /// Asserts whether two matrices have equal number of rows and columns.
    inline void __assert_equal_shape(
        size_t nrows1, size_t ncols1, size_t nrows2, size_t ncols2
    ) {
        if (nrows1 != nrows2 || ncols1 != ncols2) {
            char error[150];
            sprintf(
                error, "operands could not be broadcast together with shapes "
                "(%zu, %zu) (%zu, %zu)", nrows1, ncols1, nrows2, ncols2
            );
            throw std::invalid_argument(error);
        }
    }

    /// Assert whether an index is within the bounds of an array.
    inline void __assert_within_bounds(size_t size, size_t i) {
        if (i >= size) {
            char error[80];
            sprintf(error, "index %zu is out of bounds with size %zu", i, size);
            throw std::out_of_range(error);
        }
    }

    /// Asserts whether a pair of indices is within the bounds of a matrix.
    inline void __assert_within_bounds(
        size_t nrows, size_t ncols, size_t i, size_t j
    ) {
        if (i >= nrows || j >= ncols) {
            char error[130];
            sprintf(
                error, "index (%zu, %zu) is out of bounds with shape "
                "(%zu, %zu)", i, j, nrows, ncols
            );
            throw std::out_of_range(error);
        }
    }

    /// Asserts whether two matrices have appropriate shapes for matrix
    /// multiplication.
    inline void __assert_matmul_shapes(
        size_t nrows1, size_t ncols1, size_t nrows2, size_t ncols2
    ) {
        if (ncols1 != nrows2) {
            char error[180];
            sprintf(
                error, "matmul: Number of columns in left operand is not equal "
                "to number of rows in right operand: (%zu, %zu) (%zu, %zu)",
                nrows1, ncols1, nrows2, ncols2
            );
            throw std::invalid_argument(error);
        }
    }

    /// Function object that returns arg unchanged.
    struct __identity {
        template <class T>
        T operator()(const T &arg) const {
            return arg;
        }
    };

    /// Function object implementing +arg.
    struct __unary_plus {
        template <class T>
        T operator()(const T &arg) const {
            return +arg;
        }
    };

    /// Function object implementing -arg.
    struct __negate {
        template <class T>
        T operator()(const T &arg) const {
            return -arg;
        }
    };

    /// Function object implementing lhs + rhs.
    struct __plus {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs + rhs;
        }
    };

    /// Function object implementing lhs - rhs.
    struct __minus {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs - rhs;
        }
    };

    /// Function object implementing lhs * rhs.
    struct __multiplies {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs * rhs;
        }
    };

    /// Function object implementing lhs / rhs.
    struct __divides {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs / rhs;
        }
    };

    /// Function object implementing lhs % rhs.
    struct __modulus {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs % rhs;
        }
    };

    /// Function object implementing ~arg.
    struct __bit_not {
        template <class T>
        T operator()(const T &arg) const {
            return ~arg;
        }
    };

    /// Function object implementing lhs & rhs.
    struct __bit_and {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs & rhs;
        }
    };

    /// Function object implementing lhs | rhs.
    struct __bit_or {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs | rhs;
        }
    };

    /// Function object implementing lhs ^ rhs.
    struct __bit_xor {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs ^ rhs;
        }
    };

    /// Function object implementing lhs << rhs.
    struct __left_shift {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs << rhs;
        }
    };

    /// Function object implementing lhs >> rhs.
    struct __right_shift {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return lhs >> rhs;
        }
    };

    /// Function object implementing !arg.
    struct __logical_not {
        template <class T>
        bool operator()(const T &arg) const {
            return !arg;
        }
    };

    /// Function object implementing lhs && rhs.
    struct __logical_and {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs && rhs;
        }
    };

    /// Function object implementing lhs || rhs.
    struct __logical_or {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs || rhs;
        }
    };

    /// Function object implementing lhs == rhs.
    struct __equal_to {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs == rhs;
        }
    };

    /// Function object implementing lhs != rhs.
    struct __not_equal_to {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs != rhs;
        }
    };

    /// Function object implementing lhs < rhs.
    struct __less {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
    };

    /// Function object implementing lhs > rhs.
    struct __greater {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs > rhs;
        }
    };

    /// Function object implementing lhs <= rhs.
    struct __less_equal {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs <= rhs;
        }
    };

    /// Function object implementing lhs >= rhs.
    struct __greater_equal {
        template <class T>
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs >= rhs;
        }
    };

    /// Function object implementing std::conj.
    struct __conjugate {
        template <class T>
        T operator()(const T &arg) const {
            return arg;
        }

        template <class T>
        std::complex<T> operator()(const std::complex<T> &arg) const {
            return std::conj(arg);
        }
    };

    /// Function object implementing arg.real().
    struct __real_part {
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
    struct __imag_part {
        template <class T>
        T operator()(const T&) const {
            return 0;
        }

        template <class T>
        T operator()(const std::complex<T> &arg) const {
            return arg.imag();
        }
    };
}

#endif // C++11
#endif // NUMCPP_CONFIG_H_INCLUDED
