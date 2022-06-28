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
#include <stdexcept>

namespace numcpp {
    /// Unary and binary predicates.

    /// Function object implementing std::max.
    struct __math_max {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::max(x, y);
        }
    };

    /// Function object implementing std::min.
    struct __math_min {
        template <class T>
        T operator()(const T &x, const T &y) const {
            return std::min(x, y);
        }
    };

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
        __clamp(const T &a_min, const T &a_max) {
            this->a_min = a_min;
            this->a_max = a_max;
        }

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
            if (val < this->a_min) {
                return this->a_min;
            }
            else if (this->a_max < val) {
                return this->a_max;
            }
            else {
                return val;
            }
        }
    };

    /// Specialization of clamp for complex values.
    template <class T>
    struct __clamp< std::complex<T> > {
        __clamp<T> real, imag;

        __clamp(const std::complex<T> &a_min, const std::complex<T> &a_max)
         : real(a_min.real(), a_max.real()), imag(a_min.imag(), a_max.imag())
          {}

        std::complex<T> operator()(const std::complex<T> &val) const {
            T x = this->real(val.real());
            T y = this->imag(val.imag());
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
        __isclose(const T &rtol, const T &atol) {
            if (rtol < 0 || atol < 0) {
                char error[] = "tolerances must be greater or equal to zero";
                throw std::invalid_argument(error);
            }
            this->rtol = rtol;
            this->atol = atol;
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
                return abs_diff <= std::max(this->rtol*max_abs, this->atol);
            }
            else if (std::isnan(a) || std::isnan(b)) {
                return false;
            }
            else {
                return std::signbit(a) == std::signbit(b);
            }
        }
    };

    /// Range function objects.

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
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to get max of an empty sequence";
                throw std::invalid_argument(error);
            }
            T val = *first;
            while (++first != last) {
                if (val < *first) {
                    val = *first;
                }
            }
            return val;
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
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to get min of an empty sequence";
                throw std::invalid_argument(error);
            }
            T val = *first;
            while (++first != last) {
                if (*first < val) {
                    val = *first;
                }
            }
            return val;
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
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to get argmax of an empty sequence";
                throw std::invalid_argument(error);
            }
            size_t index = 0;
            T val = *first;
            for (size_t i = 1; ++first != last; ++i) {
                if (val < *first) {
                    index = i;
                    val = *first;
                }
            }
            return index;
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
            typedef typename std::iterator_traits<InputIterator>::value_type T;
            if (first == last) {
                char error[] = "attempt to get argmin of an empty sequence";
                throw std::invalid_argument(error);
            }
            size_t index = 0;
            T val = *first;
            for (size_t i = 1; ++first != last; ++i) {
                if (*first < val) {
                    index = i;
                    val = *first;
                }
            }
            return index;
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
            T val = 0;
            while (first != last) {
                val = val + *first;
                ++first;
            }
            return val;
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
            T val = 1;
            while (first != last) {
                val = val * *first;
                ++first;
            }
            return val;
        }
    };

    /// Function object implementing mean of range.
    struct __range_mean : __range_sum {
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
            T val = __range_sum::operator()(first, last);
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
                __range_min pred;
                val += pred(arr + size/2, arr + size);
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
        __range_var(size_t ddof = 0) {
            this->ddof = ddof;
        }

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
                val += deviation*deviation;
                ++first;
            }
            val /= size - this->ddof;
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
        __range_quantile(double q, const std::string &method = "linear") {
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
            this->q = q;
            this->method = method;
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
            size_t ith = std::floor((size - 1) * this->q);
            std::nth_element(arr, arr + ith, arr + size);
            T lower = arr[ith];
            size_t jth = std::ceil((size - 1) * this->q);
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
                if (this->q - ith/(size - 1.0) < jth/(size - 1.0) - this->q) {
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
                double t = (size - 1) * this->q - ith;
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
            while (first != last) {
                if (!*first) {
                    return false;
                }
                ++first;
            }
            return true;
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
            while (first != last) {
                if (*first) {
                    return true;
                }
                ++first;
            }
            return false;
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
            size_t count = 0;
            T zero = 0;
            while (first != last) {
                if (*first != zero) {
                    ++count;
                }
                ++first;
            }
            return count;
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
            val += step;
            ++first;
        }
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
}

#endif // NUMCPP_ROUTINES_ALGO_H_INCLUDED
