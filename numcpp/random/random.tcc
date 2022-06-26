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

 /** @file include/numcpp/random/random.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/random.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_RANDOM_TCC_INCLUDED
#define NUMCPP_RANDOM_TCC_INCLUDED

#include "numcpp/routines/algo.h"

namespace numcpp {
    /// Constructors.

    template <class bit_generator>
    Generator<bit_generator>::Generator() : m_rng() {}

    template <class bit_generator>
    Generator<bit_generator>::Generator(typename bit_generator::result_type s)
     : m_rng(s) {}

    template <class bit_generator>
    template <class Sseq>
    Generator<bit_generator>::Generator(Sseq &s) : m_rng(s) {}

    /// Seeding.

    template <class bit_generator>
    void Generator<bit_generator>::seed(typename bit_generator::result_type s) {
        this->m_rng.seed(s);
    }

    template <class bit_generator>
    template <class Sseq>
    void Generator<bit_generator>::seed(Sseq &s) {
        this->m_rng.seed(s);
    }

    /// Sample random data.

    /// Helper function: Fill a range with values sampled from a distribution.
    template <class OutputIterator, class Distribution, class RandomState>
    void __sample_distribution(
        OutputIterator first, OutputIterator last, Distribution &rvs, 
        RandomState &rng
    ) {
        while (first != last) {
            *first = rvs(rng);
            ++first;
        }
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::integers(T low, T high) {
        uniform_int_distribution<T> rvs(low, high);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::integers(T low, T high, size_t n) {
        uniform_int_distribution<T> rvs(low, high);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::integers(
        T low, T high, size_t m, size_t n
    ) {
        uniform_int_distribution<T> rvs(low, high);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::random() {
        uniform_real_distribution<T> rvs;
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::random(size_t n) {
        uniform_real_distribution<T> rvs;
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::random(size_t m, size_t n) {
        uniform_real_distribution<T> rvs;
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T, class Tag>
    T Generator<bit_generator>::choice(const base_array<T, Tag> &arr) {
        if (arr.empty()) {
            throw std::invalid_argument("arr cannot be empty");
        }
        uniform_int_distribution<size_t> rvs(0, arr.size() - 1);
        return arr[rvs(this->m_rng)];
    }

    template <class bit_generator>
    template <class T, class Tag, class TWeights, class TagWeights>
    T Generator<bit_generator>::choice(
        const base_array<T, Tag> &arr, 
        const base_array<TWeights, TagWeights> &weights 
    ) {
        if (arr.empty()) {
            throw std::invalid_argument("arr cannot be empty");
        }
        if (arr.size() != weights.size()) {
            throw std::invalid_argument("arr and weights must have same size");
        }
        discrete_distribution<size_t> rvs(weights.begin(), weights.end());
        return arr[rvs(this->m_rng)];
    }

    template <class bit_generator>
    template <class T, class Tag>
    array<T> Generator<bit_generator>::choice(
        const base_array<T, Tag> &arr, size_t size, bool replace
    ) {
        if (arr.empty()) {
            throw std::invalid_argument("arr cannot be empty");
        }
        if (replace) {
            uniform_int_distribution<size_t> rvs(0, arr.size() - 1);
            array<T> out(size);
            for (size_t i = 0; i < size; ++i) {
                out[i] = arr[rvs(this->m_rng)];
            }
            return out;
        }
        else {
            if (size > arr.size()) {
                char error[] = "cannot take a larger sample than population "
                               "when replace=false";
                throw std::invalid_argument(error);
            }
            array<T> out(arr);
            for (size_t i = 0; i < size; ++i) {
                uniform_int_distribution<size_t> rvs(i, arr.size() - 1);
                std::swap(out[i], out[rvs(this->m_rng)]);
            }
            return out[slice(size)];
        }
    }

    template <class bit_generator>
    template <class T, class Tag, class TWeights, class TagWeights>
    array<T> Generator<bit_generator>::choice(
        const base_array<T, Tag> &arr, size_t size, 
        const base_array<TWeights, TagWeights> &weights, bool replace 
    ) {
        if (arr.empty()) {
            throw std::invalid_argument("arr cannot be empty");
        }
        if (arr.size() != weights.size()) {
            throw std::invalid_argument("arr and weights must have same size");
        }
        if (replace) {
            discrete_distribution<size_t> rvs(weights.begin(), weights.end());
            array<T> out(size);
            for (size_t i = 0; i < size; ++i) {
                out[i] = arr[rvs(this->m_rng)];
            }
            return out;
        }
        else {
            if (size > arr.size()) {
                char error[] = "cannot take a larger sample than population "
                               "when replace=false";
                throw std::invalid_argument(error);
            }
            array<T> out(arr);
            array<TWeights> w(weights);
            for (size_t i = 0; i < size; ++i) {
                discrete_distribution<size_t> rvs(w.begin() + i, w.end());
                size_t idx = i + rvs(this->m_rng);
                std::swap(out[i], out[idx]);
                w[idx] = w[i];
                w[i] = 0;
            }
            return out[slice(size)];
        }
    }

    /// Permutations.

    template <class bit_generator>
    template <class T>
    void Generator<bit_generator>::shuffle(array<T> &arr) {
        std::shuffle(arr.begin(), arr.end(), this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    void Generator<bit_generator>::shuffle(array_view<T> arr) {
        std::shuffle(arr.begin(), arr.end(), this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    void Generator<bit_generator>::shuffle(index_view<T> arr) {
        std::shuffle(arr.begin(), arr.end(), this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::permutation(size_t n) {
        array<T> out(n);
        __iota(out.begin(), out.end(), 0, 1);
        std::shuffle(out.begin(), out.end(), this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T, class Tag>
    array<T> Generator<bit_generator>::permutation(
        const base_array<T, Tag> &arr
    ) {
        array<T> out(arr);
        std::shuffle(out.begin(), out.end(), this->m_rng);
        return out;
    }

    /// Distributions.

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::beta(const T &shape1, const T &shape2) {
        beta_distribution<T> rvs(shape1, shape2);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::beta(
        const T &shape1, const T &shape2, size_t n
    ) {
        beta_distribution<T> rvs(shape1, shape2);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::beta(
        const T &shape1, const T &shape2, size_t m, size_t n
    ) {
        beta_distribution<T> rvs(shape1, shape2);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::binomial(size_t size, double prob) {
        binomial_distribution<size_t> rvs(size, prob);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::binomial(
        size_t size, double prob, size_t n
    ) {
        binomial_distribution<size_t> rvs(size, prob);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::binomial(
        size_t size, double prob, size_t m, size_t n
    ) {
        binomial_distribution<size_t> rvs(size, prob);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::cauchy(const T &loc, const T &scale) {
        cauchy_distribution<T> rvs(loc, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::cauchy(
        const T &loc, const T &scale, size_t n
    ) {
        cauchy_distribution<T> rvs(loc, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::cauchy(
        const T &loc, const T &scale, size_t m, size_t n
    ) {
        cauchy_distribution<T> rvs(loc, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::chisquare(const T &df) {
        chi_squared_distribution<T> rvs(df);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::chisquare(const T &df, size_t n) {
        chi_squared_distribution<T> rvs(df);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::chisquare(
        const T &df, size_t m, size_t n
    ) {
        chi_squared_distribution<T> rvs(df);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::exponential(const T &rate) {
        exponential_distribution<T> rvs(rate);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::exponential(const T &rate, size_t n) {
        exponential_distribution<T> rvs(rate);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::exponential(
        const T &rate, size_t m, size_t n
    ) {
        exponential_distribution<T> rvs(rate);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::fisher_f(const T &df1, const T &df2) {
        fisher_f_distribution<T> rvs(df1, df2);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::fisher_f(
        const T &df1, const T &df2, size_t n
    ) {
        fisher_f_distribution<T> rvs(df1, df2);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::fisher_f(
        const T &df1, const T &df2, size_t m, size_t n
    ) {
        fisher_f_distribution<T> rvs(df1, df2);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::gamma(const T &shape, const T &scale) {
        gamma_distribution<T> rvs(shape, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::gamma(
        const T &shape, const T &scale, size_t n
    ) {
        gamma_distribution<T> rvs(shape, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::gamma(
        const T &shape, const T &scale, size_t m, size_t n
    ) {
        gamma_distribution<T> rvs(shape, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::geometric(double prob) {
        geometric_distribution<size_t> rvs(prob);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::geometric(double prob, size_t n) {
        geometric_distribution<size_t> rvs(prob);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::geometric(
        double prob, size_t m, size_t n
    ) {
        geometric_distribution<size_t> rvs(prob);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::gumbel(const T &loc, const T &scale) {
        extreme_value_distribution<T> rvs(loc, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::gumbel(
        const T &loc, const T &scale, size_t n
    ) {
        extreme_value_distribution<T> rvs(loc, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::gumbel(
        const T &loc, const T &scale, size_t m, size_t n
    ) {
        extreme_value_distribution<T> rvs(loc, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::laplace(const T &loc, const T &scale) {
        laplace_distribution<T> rvs(loc, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::laplace(
        const T &loc, const T &scale, size_t n
    ) {
        laplace_distribution<T> rvs(loc, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::laplace(
        const T &loc, const T &scale, size_t m, size_t n
    ) {
        laplace_distribution<T> rvs(loc, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::logistic(const T &loc, const T &scale) {
        logistic_distribution<T> rvs(loc, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::logistic(
        const T &loc, const T &scale, size_t n
    ) {
        logistic_distribution<T> rvs(loc, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::logistic(
        const T &loc, const T &scale, size_t m, size_t n
    ) {
        logistic_distribution<T> rvs(loc, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::lognormal(const T &logmean, const T &logscale) {
        lognormal_distribution<T> rvs(logmean, logscale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::lognormal(
        const T &logmean, const T &logscale, size_t n
    ) {
        lognormal_distribution<T> rvs(logmean, logscale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::lognormal(
        const T &logmean, const T &logscale, size_t m, size_t n
    ) {
        lognormal_distribution<T> rvs(logmean, logscale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::negative_binomial(size_t size, double prob) {
        negative_binomial_distribution<size_t> rvs(size, prob);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::negative_binomial(
        size_t size, double prob, size_t n
    ) {
        negative_binomial_distribution<size_t> rvs(size, prob);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::negative_binomial(
        size_t size, double prob, size_t m, size_t n
    ) {
        negative_binomial_distribution<size_t> rvs(size, prob);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::normal(const T &mean, const T &stddev) {
        normal_distribution<T> rvs(mean, stddev);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::normal(
        const T &mean, const T &stddev, size_t n
    ) {
        normal_distribution<T> rvs(mean, stddev);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::normal(
        const T &mean, const T &stddev, size_t m, size_t n
    ) {
        normal_distribution<T> rvs(mean, stddev);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::pareto(const T &shape, const T &scale) {
        pareto_distribution<T> rvs(shape, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::pareto(
        const T &shape, const T &scale, size_t n
    ) {
        pareto_distribution<T> rvs(shape, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::pareto(
        const T &shape, const T &scale, size_t m, size_t n
    ) {
        pareto_distribution<T> rvs(shape, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::poisson(double rate) {
        poisson_distribution<size_t> rvs(rate);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::poisson(double rate, size_t n) {
        poisson_distribution<size_t> rvs(rate);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::poisson(
        double rate, size_t m, size_t n
    ) {
        poisson_distribution<size_t> rvs(rate);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::rayleigh(const T &scale) {
        rayleigh_distribution<T> rvs(scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::rayleigh(
        const T &scale, size_t n
    ) {
        rayleigh_distribution<T> rvs(scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::rayleigh(
        const T &scale, size_t m, size_t n
    ) {
        rayleigh_distribution<T> rvs(scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::student_t(const T &df) {
        student_t_distribution<T> rvs(df);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::student_t(const T &df, size_t n) {
        student_t_distribution<T> rvs(df);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::student_t(
        const T &df, size_t m, size_t n
    ) {
        student_t_distribution<T> rvs(df);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::triangular(
        const T &lower, const T &mode, const T &right
    ) {
        array<T> breaks{lower, mode, right};
        array<T> weights{T(0), T(1), T(0)};
        piecewise_linear_distribution<T> rvs(
            breaks.begin(), breaks.end(), weights.begin()
        );
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::triangular(
        const T &lower, const T &mode, const T &right, size_t n
    ) {
        array<T> breaks{lower, mode, right};
        array<T> weights{T(0), T(1), T(0)};
        piecewise_linear_distribution<T> rvs(
            breaks.begin(), breaks.end(), weights.begin()
        );
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::triangular(
        const T &lower, const T &mode, const T &right, size_t m, size_t n
    ) {
        array<T> breaks{lower, mode, right};
        array<T> weights{T(0), T(1), T(0)};
        piecewise_linear_distribution<T> rvs(
            breaks.begin(), breaks.end(), weights.begin()
        );
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::uniform(const T &low, const T &high) {
        uniform_real_distribution<T> rvs(low, high);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::uniform(
        const T &low, const T &high, size_t n
    ) {
        uniform_real_distribution<T> rvs(low, high);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::uniform(
        const T &low, const T &high, size_t m, size_t n
    ) {
        uniform_real_distribution<T> rvs(low, high);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::wald(const T &mean, const T& scale) {
        inverse_gaussian_distribution<T> rvs(mean, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::wald(
        const T &mean, const T& scale, size_t n
    ) {
        inverse_gaussian_distribution<T> rvs(mean, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::wald(
        const T &mean, const T& scale, size_t m, size_t n
    ) {
        inverse_gaussian_distribution<T> rvs(mean, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    T Generator<bit_generator>::weibull(const T &shape, const T &scale) {
        weibull_distribution<T> rvs(shape, scale);
        return rvs(this->m_rng);
    }

    template <class bit_generator>
    template <class T>
    array<T> Generator<bit_generator>::weibull(
        const T &shape, const T &scale, size_t n
    ) {
        weibull_distribution<T> rvs(shape, scale);
        array<T> out(n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T>
    matrix<T> Generator<bit_generator>::weibull(
        const T &shape, const T &scale, size_t m, size_t n
    ) {
        weibull_distribution<T> rvs(shape, scale);
        matrix<T> out(m, n);
        __sample_distribution(out.begin(), out.end(), rvs, this->m_rng);
        return out;
    }
}

#endif // NUMCPP_RANDOM_TCC_INCLUDED
