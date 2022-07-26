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

 /** @file include/numcpp/routines/algo.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ROUTINES_ALGO_H_INCLUDED
#define NUMCPP_ROUTINES_ALGO_H_INCLUDED

#include <algorithm>
#include <cmath>
#include <complex>
#include <iterator>
#include <type_traits>
#include <stdexcept>

namespace numcpp {
    /// Unary and binary predicates.

    /// Function object implementing clamp.
    template <class T>
    struct __clamp {
        /// Lower and upper boundary.
        T a_min, a_max;

        /**
         * @brief Constructor.
         *
         * @param a_min The lower boundary to clamp.
         * @param a_max The upper boundary to clamp.
         */
        __clamp(const T &a_min, const T &a_max) : a_min(a_min), a_max(a_max) {}

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
    struct __clamp< std::complex<T> > {
        __clamp<T> clamp_real, clamp_imag;

        __clamp(const std::complex<T> &a_min, const std::complex<T> &a_max)
         : clamp_real(a_min.real(), a_max.real()),
           clamp_imag(a_min.imag(), a_max.imag())
          {}

        std::complex<T> operator()(const std::complex<T> &val) const {
            T x = clamp_real(val.real());
            T y = clamp_imag(val.imag());
            return std::complex<T>(x, y);
        }
    };

    /// Function object implementing isclose.
    template <class T>
    struct __isclose {
        /// Relative and absolute tolerance.
        T rtol, atol;

        /**
         * @brief Constructor.
         *
         * @param rtol Relative tolerance.
         * @param atol Absolute tolerance.
         */
        __isclose(const T &rtol, const T &atol) : rtol(rtol), atol(atol) {
            if (rtol < 0 || atol < 0) {
                char error[] = "tolerances must be greater or equal to zero";
                throw std::invalid_argument(error);
            }
        }

        /**
         * @brief Return if two numbers are equal within a tolerance.
         *
         * @param a A floating-point number.
         * @param b A floating-point number.
         *
         * @return True if the values are considered equal and false otherwise.
         */
        bool operator()(const T &a, const T &b) const {
            if (std::isfinite(a) && std::isfinite(b)) {
                T abs_diff = std::abs(a - b);
                T max_abs = std::max(std::abs(a), std::abs(b));
                return abs_diff <= std::max(rtol * max_abs, atol);
            }
            else if (std::isnan(a) || std::isnan(b)) {
                return false;
            }
            else {
                return std::signbit(a) == std::signbit(b);
            }
        }
    };

    /// Specialization of isclose for complex values.
    template <class T>
    struct __isclose< std::complex<T> > : __isclose<T> {
        __isclose(const T &rtol, const T &atol) : __isclose<T>(rtol, atol) {}

        bool operator()(const std::complex<T> &a, const std::complex<T> &b)
        const {
            return (__isclose<T>::operator()(a.real(), b.real()) &&
                    __isclose<T>::operator()(a.imag(), b.imag()));
        }
    };

    /// Range functions.

    /**
     * @brief Fills the range [first, last) with sequentially increasing
     * values, starting with start and adding steps of size step.
     *
     * @param first Output iterator to the initial position of the sequence.
     * @param last Output iterator to the final position of the sequence.
     * @param start Initial value.
     * @param step Step size.
     */
    template <class OutputIterator, class T>
    void __iota(
        OutputIterator first, OutputIterator last, const T &start, const T &step
    ) {
        T val = start;
        while (first != last) {
            *first = val;
            val = val + step;
            ++first;
        }
    }

    /**
     * @brief Returns the result of accumulating all the values in the range
     * [first, last).
     *
     * @param first Input iterator to the initial position of the sequence.
     * @param last Input iterator to the final position of the sequence.
     * @param init Initial value for the accumulator.
     * @param f Binary function taking an element of type T as first argument
     *     and an element in the range as second, and which returns a value
     *     that can be assigned to type T.
     *
     * @return The result of accumulating init and all the elements in the
     *     range [first, last).
     */
    template <class InputIterator, class T, class Function>
    T __reduce(
        InputIterator first, InputIterator last, const T &init, Function f
    ) {
        T val = init;
        while (first != last) {
            val = f(val, *first);
            ++first;
        }
        return val;
    }

    /**
     * @brief Assigns to every element in the range starting at result the
     * cumulative operation of the first elements in the range [first, last).
     *
     * @param first Input iterator to the initial position of the sequence.
     * @param last Input iterator to the final position of the sequence.
     * @param result Output iterator to the initial position of the destination
     *     sequence where the accumulates are stored.
     * @param f Binary function taking two elements of the type pointed by the
     *     InputIterator as arguments, and returning the result of the
     *     accumulated operation.
     *
     * @return An iterator pointing to past the last element of the destination
     *     sequence where resulting elements have been stored.
     */
    template <class InputIterator, class OutputIterator, class Function>
    OutputIterator __accumulate(
        InputIterator first, InputIterator last, OutputIterator result,
        Function f
    ) {
        typedef typename std::iterator_traits<InputIterator>::value_type T;
        if (first != last) {
            T val = *first;
            *result = val;
            ++first;
            ++result;
            while (first != last) {
                val = f(val, *first);
                *result = val;
                ++first;
                ++result;
            }
        }
        return result;
    }

    /// Range function objects.

    /// Function object implementing reduce.
    template <class Function>
    struct __range_reduce {
    private:
        /// Type traits to check if has identity.
        template <class F, typename = void>
        struct has_identity : std::false_type {};

        template <class F>
        struct has_identity<F, decltype(void(F::identity))> : std::true_type {};

    public:
        /// Underlying function.
        Function f;

        /**
         * @brief Constructor.
         *
         * @param f Binary function to apply.
         */
        __range_reduce(Function f = Function()) : f(f) {};

        /**
         * Returns the result of accumulating all the values in the range
         * [first, last).
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The result of accumulating all the elements in the range
         *     [first, last).
         */
        template <class InputIterator>
        typename std::enable_if<
            has_identity<Function>::value,
            typename std::iterator_traits<InputIterator>::value_type
        >::type operator()(InputIterator first, InputIterator last) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            return __reduce(first, last, T(f.identity), f);
        }

        template <class InputIterator>
        typename std::enable_if<
            !has_identity<Function>::value,
            typename std::iterator_traits<InputIterator>::value_type
        >::type operator()(InputIterator first, InputIterator last) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to call reduce on an empty sequence "
                "with no identity";
                throw std::invalid_argument(error);
            }
            T init = *first;
            ++first;
            return __reduce(first, last, init, f);
        }
    };

    /// Function object implementing maximum in range.
    struct __range_max {
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

    /// Function object implementing minimum in range.
    struct __range_min {
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

    /// Function object implementing argument maximum in range.
    struct __range_argmax {
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

    /// Function object implementing argument minimum in range.
    struct __range_argmin {
    template <class InputIterator>
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
        size_t operator()(InputIterator first, InputIterator last) const {
            if (first == last) {
                char error[] = "attempt to get argmin of an empty sequence";
                throw std::invalid_argument(error);
            }
            return std::distance(first, std::min_element(first, last));
        }
    };

    /// Function object implementing sum of range.
    struct __range_sum {
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
            return __reduce(first, last, T(0), __plus());
        }
    };

    /// Function object implementing product of range.
    struct __range_prod {
    template <class InputIterator>
        /**
         * @brief Returns the product of the elements in the range
         * [first, last) or 1 if [first, last) is an empty range.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return The product of the elements in the range.
         */
        typename std::iterator_traits<InputIterator>::value_type operator()(
            InputIterator first, InputIterator last
        ) const {
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            return __reduce(first, last, T(1), __multiplies());
        }
    };

    /// Function object implementing mean of range.
    struct __range_mean {
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
            T val = __reduce(first, last, T(0), __plus());
            val /= std::distance(first, last);
            return val;
        }
    };

    /// Function object implementing median of range.
    struct __range_median {
        /**
         * @brief Returns the median of the elements in the range
         * [first, last).
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
            T *arr = new T[size];
            std::copy(first, last, arr);
            std::nth_element(arr, arr + (size - 1)/2, arr + size);
            T val = arr[(size - 1)/2];
            if (size % 2 == 0) {
                val += *std::min_element(arr + size/2, arr + size);
                val /= 2;
            }
            delete[] arr;
            return val;
        }
    };

    /// Function object implementing sample variance.
    struct __range_var : __range_mean {
        /// Delta degrees of freedom.
        size_t ddof;

        /**
         * @brief Constructor.
         *
         * @param ddof Delta degrees of freedom.
         */
        __range_var(size_t ddof = 0) : ddof(ddof) {}

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
            T val = 0;
            T mean_val = __range_mean::operator()(first, last);
            while (first != last) {
                T deviation = std::abs(*first - mean_val);
                val += deviation * deviation;
                ++first;
            }
            val /= size - ddof;
            return val;
        }
    };

    /// Function object implementing sample standard deviation.
    struct __range_stddev : __range_var {
        /**
         * @brief Constructor.
         *
         * @param ddof Delta degrees of freedom.
         */
        __range_stddev(size_t ddof = 0) : __range_var(ddof) {}

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
            return std::sqrt(__range_var::operator()(first, last));
        }
    };

    /// Function object implementig quantile
    struct __range_quantile {
        /// Quantile to compute.
        double q;
        /// Method estimation
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
        __range_quantile(double q, const std::string &method = "linear")
         : q(q), method(method) {
            if (q < 0 || q > 1) {
                char error[] = "quantiles must be in the range [0, 1]";
                throw std::invalid_argument(error);
            }
            if (
                method != "lower" && method != "higher" &&
                method != "nearest" && method != "midpoint" &&
                method != "linear"
            ) {
                char error[] = "method must be one of \"lower\", \"higher\", "
                "\"nearest\", \"midpoint\" or \"linear\"";
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
            T *arr = new T[size];
            std::copy(first, last, arr);
            size_t ith = std::floor((size - 1) * q);
            std::nth_element(arr, arr + ith, arr + size);
            T lower = arr[ith];
            size_t jth = std::ceil((size - 1) * q);
            std::nth_element(arr, arr + jth, arr + size);
            T higher = arr[jth];
            delete[] arr;
            if (method == "lower") {
                return lower;
            }
            else if (method == "higher") {
                return higher;
            }
            else if (method == "nearest") {
                if (q - ith/(size - 1.0) < jth/(size - 1.0) - q) {
                    return lower;
                }
                else {
                    return higher;
                }
            }
            else if (method == "midpoint") {
                return (lower + higher) / 2;
            }
            else {
                double t = (size - 1) * q - ith;
                return (1 - t)*lower + t*higher;
            };
        }
    };

    /// Function object implementing all of range.
    struct __range_all {
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

    /// Function object implementing any of range.
    struct __range_any {
        /**
         * @brief Returns true if any of the elements in the range [first, last)
         * evaluate to true and if [first, last) is not an empty range.
         *
         * @param first Input iterator to the initial position of the sequence.
         * @param last Input iterator to the final position of the sequence.
         *
         * @return true if any of the elements in the range evaluate to true and
         * false otherwise.
         */
        template <class InputIterator>
        bool operator()(InputIterator first, InputIterator last) const {
            return (last != std::find(first, last, true));
        }
    };

    /// Function object implementing count non-zero in range.
    struct __range_count_nonzero {
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
            return std::distance(first, last) - std::count(first, last, T(0));
        }
    };
}

#endif // NUMCPP_ROUTINES_ALGO_H_INCLUDED
