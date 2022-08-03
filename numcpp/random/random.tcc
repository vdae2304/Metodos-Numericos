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

#include <algorithm>
#include <numeric>

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
        m_rng.seed(s);
    }

    template <class bit_generator>
    template <class Sseq>
    void Generator<bit_generator>::seed(Sseq &s) {
        m_rng.seed(s);
    }

    /// Sample random data.

    template <class bit_generator>
    template <class OutputIterator, class Distribution>
    inline void Generator<bit_generator>::__sample_distribution(
        OutputIterator first, OutputIterator last, Distribution &rvs
    ) {
        while (first != last) {
            *first = rvs(m_rng);
            ++first;
        }
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::integers(T low, T high) {
        uniform_int_distribution<T> rvs(low, high);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::integers(
        T low, T high, size_t size
    ) {
        return this->integers<T>(low, high, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::integers(
        T low, T high, const shape_t<Rank> &size
    ) {
        uniform_int_distribution<T> rvs(low, high);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::random() {
        uniform_real_distribution<T> rvs;
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::random(size_t size) {
        return this->random<T>(make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::random(
        const shape_t<Rank> &size
    ) {
        uniform_real_distribution<T> rvs;
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T, class Tag>
    T Generator<bit_generator>::choice(
        const base_tensor<T, 1, Tag> &population
    ) {
        if (population.empty()) {
            throw std::invalid_argument("population cannot be empty");
        }
        uniform_int_distribution<size_t> rvs(0, population.size() - 1);
        return population[rvs(m_rng)];
    }

    template <class bit_generator>
    template <class T, class Tag, class TWeights, class TagWeights>
    T Generator<bit_generator>::choice(
        const base_tensor<T, 1, Tag> &population,
        const base_tensor<TWeights, 1, TagWeights> &weights
    ) {
        if (population.empty()) {
            throw std::invalid_argument("population cannot be empty");
        }
        if (population.size() != weights.size()) {
            char error[] = "population and weights must have same size";
            throw std::invalid_argument(error);
        }
        discrete_distribution<size_t> rvs(weights.begin(), weights.end());
        return population[rvs(m_rng)];
    }

    template <class bit_generator>
    template <class T, class Tag>
    inline tensor<T, 1> Generator<bit_generator>::choice(
        const base_tensor<T, 1, Tag> &population, size_t size,
        bool replace
    ) {
        return this->choice(population, make_shape(size), replace);
    }

    template <class bit_generator>
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> Generator<bit_generator>::choice(
        const base_tensor<T, 1, Tag> &population, const shape_t<Rank> &size,
        bool replace
    ) {
        if (population.empty()) {
            throw std::invalid_argument("population cannot be empty");
        }
        if (replace) {
            uniform_int_distribution<size_t> rvs(0, population.size() - 1);
            tensor<T, Rank> out(size);
            for (auto it = out.begin(); it != out.end(); ++it) {
                *it = population[rvs(m_rng)];
            }
            return out;
        }
        else {
            if (size.size() > population.size()) {
                char error[] = "cannot take a larger sample than population "
                               "when replace=false";
                throw std::invalid_argument(error);
            }
            size_t n = size.size();
            tensor<T, 1> out(population);
            for (size_t i = 0; i < n; ++i) {
                uniform_int_distribution<size_t> rvs(i, population.size() - 1);
                std::swap(out[i], out[rvs(m_rng)]);
            }
            return tensor<T, Rank>(size, out.begin());
        }
    }

    template <class bit_generator>
    template <class T, class Tag, class TWeights, class TagWeights>
    inline tensor<T, 1> Generator<bit_generator>::choice(
        const base_tensor<T, 1, Tag> &population, size_t size,
        const base_tensor<TWeights, 1, TagWeights> &weights,
        bool replace
    ) {
        return this->choice(population, make_shape(size), weights, replace);
    }

    template <class bit_generator>
    template <class T, size_t Rank, class Tag,
              class TWeights, class TagWeights>
    tensor<T, Rank> Generator<bit_generator>::choice(
        const base_tensor<T, 1, Tag> &population, const shape_t<Rank> &size,
        const base_tensor<TWeights, 1, TagWeights> &weights,
        bool replace
    ) {
        if (population.empty()) {
            throw std::invalid_argument("population cannot be empty");
        }
        if (population.shape() != weights.shape()) {
            char error[] = "population and weights must have same size";
            throw std::invalid_argument(error);
        }
        if (replace) {
            discrete_distribution<size_t> rvs(weights.begin(), weights.end());
            tensor<T, Rank> out(size);
            for (auto it = out.begin(); it != out.end(); ++it) {
                *it = population[rvs(m_rng)];
            }
            return out;
        }
        else {
            if (size.size() > population.size()) {
                char error[] = "cannot take a larger sample than population "
                               "when replace=false";
                throw std::invalid_argument(error);
            }
            size_t n = size.size();
            tensor<T, 1> out(population);
            tensor<TWeights, 1> w(weights);
            for (size_t i = 0; i < n; ++i) {
                discrete_distribution<size_t> rvs(w.begin() + i, w.end());
                size_t idx = i + rvs(m_rng);
                std::swap(out[i], out[idx]);
                w[idx] = w[i];
                w[i] = 0;
            }
            return tensor<T, Rank>(size, out.begin());
        }
    }

    /// Permutations.

    template <class bit_generator>
    template <class T, size_t Rank, class Tag>
    inline void Generator<bit_generator>::shuffle(
        base_tensor<T, Rank, Tag> &arg
    ) {
        std::shuffle(arg.begin(), arg.end(), m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::permutation(size_t n) {
        tensor<T, 1> out(n);
        std::iota(out.begin(), out.end(), 0);
        std::shuffle(out.begin(), out.end(), m_rng);
        return out;
    }

    template <class bit_generator>
    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> Generator<bit_generator>::permutation(
        const base_tensor<T, Rank, Tag> &arg
    ) {
        tensor<T, Rank> out(arg);
        std::shuffle(out.begin(), out.end(), m_rng);
        return out;
    }

    /// Distributions.

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::beta(T shape1, T shape2) {
        beta_distribution<T> rvs(shape1, shape2);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::beta(
        T shape1, T shape2, size_t size
    ) {
        return this->beta<T>(shape1, shape2, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::beta(
        T shape1, T shape2, const shape_t<Rank> &size
    ) {
        beta_distribution<T> rvs(shape1, shape2);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::binomial(size_t n, double prob) {
        binomial_distribution<T> rvs(n, prob);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::binomial(
        size_t n, double prob, size_t size
    ) {
        return this->binomial<T>(n, prob, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::binomial(
        size_t n, double prob, const shape_t<Rank> &size
    ) {
        binomial_distribution<T> rvs(n, prob);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::cauchy(T loc, T scale) {
        cauchy_distribution<T> rvs(loc, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::cauchy(
        T loc, T scale, size_t size
    ) {
        return this->cauchy<T>(loc, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::cauchy(
        T loc, T scale, const shape_t<Rank> &size
    ) {
        cauchy_distribution<T> rvs(loc, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::chisquare(T df) {
        chi_squared_distribution<T> rvs(df);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::chisquare(T df, size_t size) {
        return this->chisquare<T>(df, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::chisquare(
        T df, const shape_t<Rank> &size
    ) {
        chi_squared_distribution<T> rvs(df);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::exponential(T rate) {
        exponential_distribution<T> rvs(rate);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::exponential(
        T rate, size_t size
    ) {
        return this->exponential<T>(rate, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::exponential(
        T rate, const shape_t<Rank> &size
    ) {
        exponential_distribution<T> rvs(rate);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::fisher_f(T df1, T df2) {
        fisher_f_distribution<T> rvs(df1, df2);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::fisher_f(
        T df1, T df2, size_t size
    ) {
        return this->fisher_f<T>(df1, df2, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::fisher_f(
        T df1, T df2, const shape_t<Rank> &size
    ) {
        fisher_f_distribution<T> rvs(df1, df2);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::gamma(T shape, T scale) {
        gamma_distribution<T> rvs(shape, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::gamma(
        T shape, T scale, size_t size
    ) {
        return this->gamma<T>(shape, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::gamma(
        T shape, T scale, const shape_t<Rank> &size
    ) {
        gamma_distribution<T> rvs(shape, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::geometric(double prob) {
        geometric_distribution<T> rvs(prob);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::geometric(
        double prob, size_t size
    ) {
        return this->geometric<T>(prob, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::geometric(
        double prob, const shape_t<Rank> &size
    ) {
        geometric_distribution<T> rvs(prob);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::gumbel(T loc, T scale) {
        extreme_value_distribution<T> rvs(loc, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::gumbel(
        T loc, T scale, size_t size
    ) {
        return this->gumbel<T>(loc, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::gumbel(
        T loc, T scale, const shape_t<Rank> &size
    ) {
        extreme_value_distribution<T> rvs(loc, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::laplace(T loc, T scale) {
        laplace_distribution<T> rvs(loc, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::laplace(
        T loc, T scale, size_t size
    ) {
        return this->laplace<T>(loc, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::laplace(
        T loc, T scale, const shape_t<Rank> &size
    ) {
        laplace_distribution<T> rvs(loc, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::logistic(T loc, T scale) {
        logistic_distribution<T> rvs(loc, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::logistic(
        T loc, T scale, size_t size
    ) {
        return this->logistic<T>(loc, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::logistic(
        T loc, T scale, const shape_t<Rank> &size
    ) {
        logistic_distribution<T> rvs(loc, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::lognormal(T logmean, T logscale) {
        lognormal_distribution<T> rvs(logmean, logscale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::lognormal(
        T logmean, T logscale, size_t size
    ) {
        return this->lognormal<T>(logmean, logscale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::lognormal(
        T logmean, T logscale, const shape_t<Rank> &size
    ) {
        lognormal_distribution<T> rvs(logmean, logscale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::negative_binomial(
        size_t n, double prob
    ) {
        negative_binomial_distribution<T> rvs(n, prob);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::negative_binomial(
        size_t n, double prob, size_t size
    ) {
        return this->negative_binomial<T>(n, prob, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::negative_binomial(
        size_t n, double prob, const shape_t<Rank> &size
    ) {
        negative_binomial_distribution<T> rvs(n, prob);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::normal(T mean, T stddev) {
        normal_distribution<T> rvs(mean, stddev);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::normal(
        T mean, T stddev, size_t size
    ) {
        return this->normal<T>(mean, stddev, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::normal(
        T mean, T stddev, const shape_t<Rank> &size
    ) {
        normal_distribution<T> rvs(mean, stddev);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::pareto(T shape, T scale) {
        pareto_distribution<T> rvs(shape, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::pareto(
        T shape, T scale, size_t size
    ) {
        return this->pareto<T>(shape, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::pareto(
        T shape, T scale, const shape_t<Rank> &size
    ) {
        pareto_distribution<T> rvs(shape, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::poisson(double rate) {
        poisson_distribution<T> rvs(rate);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::poisson(
        double rate, size_t size
    ) {
        return this->poisson<T>(rate, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::poisson(
        double rate, const shape_t<Rank> &size
    ) {
        poisson_distribution<T> rvs(rate);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::rayleigh(T scale) {
        rayleigh_distribution<T> rvs(scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::rayleigh(
        T scale, size_t size
    ) {
        return this->rayleigh<T>(scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::rayleigh(
        T scale, const shape_t<Rank> &size
    ) {
        rayleigh_distribution<T> rvs(scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::student_t(T df) {
        student_t_distribution<T> rvs(df);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::student_t(
        T df, const size_t size
    ) {
        return this->student_t<T>(df, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::student_t(
        T df, const shape_t<Rank> &size
    ) {
        student_t_distribution<T> rvs(df);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::triangular(T lower, T mode, T right) {
        T breaks[3] = {lower, mode, right};
        T weights[3] = {T(0), T(1), T(0)};
        piecewise_linear_distribution<T> rvs(breaks, breaks + 3, weights);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::triangular(
        T lower, T mode, T right, size_t size
    ) {
        return this->triangular<T>(lower, mode, right, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::triangular(
        T lower, T mode, T right, const shape_t<Rank> &size
    ) {
        T breaks[3] = {lower, mode, right};
        T weights[3] = {T(0), T(1), T(0)};
        piecewise_linear_distribution<T> rvs(breaks, breaks + 3, weights);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::uniform(T low, T high) {
        uniform_real_distribution<T> rvs(low, high);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::uniform(
        T low, T high, size_t size
    ) {
        return this->uniform<T>(low, high, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::uniform(
        T low, T high, const shape_t<Rank> &size
    ) {
        uniform_real_distribution<T> rvs(low, high);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::wald(T mean, T scale) {
        inverse_gaussian_distribution<T> rvs(mean, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::wald(
        T mean, T scale, size_t size
    ) {
        return this->wald<T>(mean, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::wald(
        T mean, T scale, const shape_t<Rank> &size
    ) {
        inverse_gaussian_distribution<T> rvs(mean, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }

    template <class bit_generator>
    template <class T>
    inline T Generator<bit_generator>::weibull(T shape, T scale) {
        weibull_distribution<T> rvs(shape, scale);
        return rvs(m_rng);
    }

    template <class bit_generator>
    template <class T>
    inline tensor<T, 1> Generator<bit_generator>::weibull(
        T shape, T scale, size_t size
    ) {
        return this->weibull<T>(shape, scale, make_shape(size));
    }

    template <class bit_generator>
    template <class T, size_t Rank>
    inline tensor<T, Rank> Generator<bit_generator>::weibull(
        T shape, T scale, const shape_t<Rank> &size
    ) {
        weibull_distribution<T> rvs(shape, scale);
        tensor<T, Rank> out(size);
        this->__sample_distribution(out.begin(), out.end(), rvs);
        return out;
    }
}

#endif // NUMCPP_RANDOM_TCC_INCLUDED
