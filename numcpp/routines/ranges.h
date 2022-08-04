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

 /** @file include/numcpp/tensor.h
  *  This header defines the tensor class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ROUTINES_ALGO_H_INCLUDED
#define NUMCPP_ROUTINES_ALGO_H_INCLUDED

#include "numcpp/broadcasting/operators.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <iterator>
#include <numeric>
#include <stdexcept>

namespace numcpp {

/// Namespace for implementation details.
namespace detail {
    /**
     * @brief Function object implementing clamp.
     */
    template <class T>
    struct clamp {
        /// Lower and upper boundary.
        T a_min, a_max;

        /**
         * @brief Constructor.
         *
         * @param a_min The lower boundary to clamp.
         * @param a_max The upper boundary to clamp.
         */
        clamp(const T &a_min, const T &a_max) : a_min(a_min), a_max(a_max) {}

        /**
         * @brief Clamp a value. Given an interval [a_min, a_max], values
         * smaller than a_min become a_min, and values larger than a_max become
         * a_max.
         *
         * @param val The value to clamp.
         *
         * @return The clamped value.
         */
        T operator()(const T &val) const {
            if (val < a_min) {
                return a_min;
            }
            else if (a_max < val) {
                return a_max;
            }
            else {
                return val;
            }
        }
    };

    /// Specialization of clamp for complex values.
    template <class T>
    struct clamp< std::complex<T> > {
        clamp<T> clamp_real, clamp_imag;

        clamp(const std::complex<T> &a_min, const std::complex<T> &a_max)
         : clamp_real(a_min.real(), a_max.real()),
           clamp_imag(a_min.imag(), a_max.imag())
          {}

        std::complex<T> operator()(const std::complex<T> &val) const {
            T x = clamp_real(val.real());
            T y = clamp_imag(val.imag());
            return std::complex<T>(x, y);
        }
    };
}

/// Namespace for range functions.
namespace ranges {
    /**
     * @brief Function object implementing all of range.
     */
    struct all {
        /**
         * @brief Returns true if all of the elements in the range
         * [first, last) evaluate to true or if [first, last) is an empty
         * range.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return true if all of the elements in the range evaluate to true
         *     and false otherwise.
         */
        template <class InputIterator>
        bool operator()(InputIterator first, InputIterator last) const {
            return (last == std::find(first, last, false));
        }
    };

    /**
     * @brief Function object implementing any of range.
     */
    struct any {
        /**
         * @brief Returns true if any of the elements in the range [first, last)
         * evaluate to true and if [first, last) is not an empty range.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return true if any of the elements in the range evaluate to true and
         *     false otherwise.
         */
        template <class InputIterator>
        bool operator()(InputIterator first, InputIterator last) const {
            return (last != std::find(first, last, true));
        }
    };

    /**
     * @brief Function object implementing maximum in range.
     */
    struct max {
        /**
         * @brief Returns the maximum value contained in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The maximum value in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            if (first == last) {
                char error[] = "attempt to get max of an empty sequence";
                throw std::invalid_argument(error);
            }
            return *std::max_element(first, last);
        }
    };

    /**
     * @brief Function object implementing minimum in range.
     */
    struct min {
        /**
         * @brief Returns the minimum value contained in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The minimum value in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            if (first == last) {
                char error[] = "attempt to get min of an empty sequence";
                throw std::invalid_argument(error);
            }
            return *std::min_element(first, last);
        }
    };

    /**
     * @brief Function object implementing argument maximum in range.
     */
    struct argmax {
        /**
         * @brief Returns the index of the maximum value contained in the range
         * [first, last). In case of multiple occurrences of the maximum value,
         * return the index corresponding to the first occurrence.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The index of the maximum value in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        size_t operator()(InputIterator first, InputIterator last) const {
            if (first == last) {
                char error[] = "attempt to get argmax of an empty sequence";
                throw std::invalid_argument(error);
            }
            return std::distance(first, std::max_element(first, last));
        }
    };

    /**
     * @brief Function object implementing argument minimum in range.
     */
    struct argmin {
        /**
         * @brief Returns the index of the minimum value contained in the range
         * [first, last). In case of multiple occurrences of the minimum value,
         * return the index corresponding to the first occurrence.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The index of the minimum value in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        size_t operator()(InputIterator first, InputIterator last) const {
            if (first == last) {
                char error[] = "attempt to get argmin of an empty sequence";
                throw std::invalid_argument(error);
            }
            return std::distance(first, std::min_element(first, last));
        }
    };

    /**
     * @brief Function object implementing sum of range.
     */
    struct sum {
        /**
         * @brief Returns the sum of the elements in the range [first, last) or
         * 0 if [first, last) is an empty range.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The sum of the elements in the range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            return std::accumulate(first, last, T(0), plus());
        }
    };

    /**
     * @brief Function object implementing product of range.
     */
    struct prod {
        /**
         * @brief Returns the product of the elements in the range
         * [first, last) or 1 if [first, last) is an empty range.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The product of the elements in the range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            return std::accumulate(first, last, T(1), multiplies());
        }
    };

    /**
     * @brief Function object implementing mean of range.
     */
    struct mean {
        /**
         * @brief Returns the average of the elements in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The average of the elements in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to get mean of an empty sequence";
                throw std::invalid_argument(error);
            }
            T val = std::accumulate(first, last, T(0), plus());
            val /= std::distance(first, last);
            return val;
        }
    };

    /**
     * @brief Function object implementing sample variance.
     */
    struct var {
        /// Delta degrees of freedom.
        size_t ddof;

        /**
         * @brief Constructor.
         *
         * @param ddof Delta degrees of freedom.
         */
        var(size_t ddof = 0) : ddof(ddof) {}

        /**
         * @brief Returns the sample variance of the elements in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The sample variance of the elements in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to get variance of an empty sequence";
                throw std::invalid_argument(error);
            }
            size_t size = std::distance(first, last);
            T val = T(0);
            T mean_val = std::accumulate(first, last, T(0), plus());
            mean_val /= size;
            while (first != last) {
                T deviation = std::abs(*first - mean_val);
                val += deviation * deviation;
                ++first;
            }
            val /= size - ddof;
            return val;
        }
    };

    /**
     * @brief Function object implementing sample standard deviation.
     */
    struct stddev : var {
        /**
         * @brief Constructor.
         *
         * @param ddof Delta degrees of freedom.
         */
        stddev(size_t ddof = 0) : var(ddof) {}

        /**
         * @brief Returns the sample standard deviation of the elements in the
         * range [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The sample standard deviation of the elements in the range.
         *
         * @throw std::invalid_argument Thrown if [first, last) is an empty
         *     range.
         */
        template <class InputIterator>
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            return std::sqrt(var::operator()(first, last));
        }
    };
}
}

#endif // NUMCPP_ROUTINES_ALGO_H_INCLUDED