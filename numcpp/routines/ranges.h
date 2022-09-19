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

 /** @file include/numcpp/routines/ranges.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_RANGES_H_INCLUDED
#define NUMCPP_RANGES_H_INCLUDED

#include <algorithm>
#include <cmath>
#include <complex>
#include <iterator>
#include <numeric>
#include <stdexcept>

namespace numcpp {

/// Namespace for range functions.
namespace ranges {
    /**
     * @brief Function object implementing std::max.
     */
    struct maximum {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return std::max(lhs, rhs);
        }
    };

    /**
     * @brief Function object implementing std::min.
     */
    struct minimum {
        template <class T>
        T operator()(const T &lhs, const T &rhs) const {
            return std::min(lhs, rhs);
        }
    };

    /**
     * @brief Function object implementing clamp.
     */
    template <class T>
    struct clamp {
        // Lower and upper boundary.
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

    /**
     * @brief Specialization of clamp for complex values.
     */
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

    /**
     * @brief Function object implementing isclose.
     */
    template <class T>
    struct isclose {
        // Relative and absolute tolerances.
        T rtol, atol;

        /**
         * @brief Constructor.
         *
         * @param rtol Relative tolerance.
         * @param atol Absolute tolerance.
         */
        isclose(const T &rtol, const T &atol) : rtol(rtol), atol(atol) {
            if (rtol < 0 || atol < 0) {
                char error[] = "tolerances must be non-negative";
                throw std::invalid_argument(error);
            }
        }

        /**
         * @brief Return if two numbers are equal within a tolerance.
         *
         * @param a A floating-point number.
         * @param b A floating-point number.
         *
         * @return true if the values are considered equal and false otherwise.
         */
        bool operator()(const T &a, const T &b) const {
            if (std::isfinite(a) && std::isfinite(b)) {
                T max_abs = std::fmax(std::abs(a), std::abs(b));
                return std::abs(a - b) <= std::fmax(rtol * max_abs, atol);
            }
            else if (std::isnan(a) || std::isnan(b)) {
                return false;
            }
            else {
                return std::signbit(a) == std::signbit(b);
            }
        }
    };

    /**
     * @brief Specialization of isclose for complex values.
     */
    template <class T>
    struct isclose< std::complex<T> > : isclose<T> {
        isclose(const T &rtol, const T &atol) : isclose<T>(rtol, atol) {}

        bool operator()(const std::complex<T> &a, const std::complex<T> &b)
        const {
            return (isclose<T>::operator()(a.real(), b.real()) &&
                    isclose<T>::operator()(a.imag(), b.imag()));
        }
    };

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
     * @brief Function object implementing count non-zero of range.
     */
    struct count_nonzero {
        /**
         * @brief Returns the number of non-zero elements in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The number of non-zero elements in the range.
         */
        template <class InputIterator>
        size_t operator()(InputIterator first, InputIterator last) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            return std::distance(first, last) - std::count(first, last, T());
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
     * @brief Function object implementing median of range.
     */
    struct median {
        /**
         * @brief Returns the median of the elements in the range [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The median of the elements in the range.
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
                char error[] = "attempt to get median of an empty sequence";
                throw std::invalid_argument(error);
            }
            size_t size = std::distance(first, last);
            T *buffer = new T[size];
            std::copy(first, last, buffer);
            std::nth_element(buffer, buffer + (size - 1) / 2, buffer + size);
            T val = buffer[(size - 1) / 2];
            if (size % 2 == 0) {
                val += *std::min_element(buffer + size / 2, buffer + size);
                val /= 2;
            }
            delete[] buffer;
            return val;
        }
    };

    /**
     * @brief Function object implementing sample variance.
     */
    struct var {
        // Whether to use a biased estimator.
        bool bias;

        /**
         * @brief Constructor.
         *
         * @param bias If bias is true, then normalization is by n. Otherwise,
         *     normalization is by n - 1.
         */
        var(bool bias = false) : bias(bias) {}

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
            val /= size - 1 + bias;
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
         * @param bias If bias is true, then normalization is by n. Otherwise,
         *     normalization is by n - 1.
         */
        stddev(bool bias = false) : var(bias) {}

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

    /**
     * @brief Function object implementing quantile of range.
     */
    struct quantile {
        // Quantile to compute.
        double q;

        // Method estimation.
        std::string method;

        /**
         * @brief Constructor.
         *
         * @param q Quantile to compute, which must be between 0 and 1
         *     (inclusive).
         * @param method This parameter specifies the method to use for
         *     estimating the quantile. Must be one of "lower", "higher",
         *     "nearest", "midpoint" or "linear".
         */
        quantile(double q, const std::string &method = "linear")
         : q(q), method(method) {
            if (q < 0 || q > 1) {
                char error[] = "quantiles must be in the range [0, 1]";
                throw std::invalid_argument(error);
            }
            if (
                method != "lower" && method != "higher" &&
                method != "midpoint" && method != "nearest" &&
                method != "linear"
            ) {
                char error[] = "method must be one of \"lower\", \"higher\", "
                    "\"midpoint\", \"nearest\" or \"linear\"";
                throw std::invalid_argument(error);
            }
        }

        /**
         * @brief Returns the q-th quantile of the elements in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The quantile of the elements in the range.
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
                char error[] = "attempt to get quantile of an empty sequence";
                throw std::invalid_argument(error);
            }
            size_t size = std::distance(first, last);
            T *buffer = new T[size];
            std::copy(first, last, buffer);
            size_t ith = std::floor((size - 1) * q);
            std::nth_element(buffer, buffer + ith, buffer + size);
            T lower = buffer[ith];
            size_t jth = std::ceil((size - 1) * q);
            std::nth_element(buffer, buffer + jth, buffer + size);
            T higher = buffer[jth];
            delete[] buffer;
            double t;
            if (method == "lower") {
                t = 0;
            }
            else if (method == "higher") {
                t = 1;
            }
            else if (method == "midpoint") {
                t = 0.5;
            }
            else if (method == "nearest") {
                t = std::round((size - 1) * q - ith);
            }
            else if (method == "linear") {
                t = (size - 1) * q - ith;
            };
            return (1 - t) * lower + t * higher;
        }
    };
}
}

#endif // NUMCPP_RANGES_H_INCLUDED
