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

 /** @file include/numcpp/random.h
  *  This header defines routines for sampling pseudo random numbers.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_RANDOM_H_INCLUDED
#define NUMCPP_RANDOM_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/random/distributions.h"

namespace numcpp {
    /**
     * @brief A Generator is a wrapper class of a random number engine that
     * transforms sequence of random bits into a sequence of numbers that
     * follow a specific probability distribution (such as uniform, normal or
     * binomial).
     *
     * @tparam bit_generator Random number engine to use. Must meet the
     *     requirements listed by Uniform Random Bit Generator @link
     *     https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator
     *     @endlink and Random Number Engine @link
     *     https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
     *     @endlink. Common examples are std::default_random_engine and
     *     std::random_device.
     */
    template <class bit_generator>
    class Generator {
    public:
        /// Constructors.

        /**
         * @brief Default constructor. Constructs a Generator with default
         * seed.
         */
        Generator();

        /**
         * @brief Seed constructor. Constructs a Generator from a seed value or
         * a seed sequence.
         *
         * @param s Seed value or seed sequence to use.
         */
        Generator(typename bit_generator::result_type s);

        template <class Sseq>
        Generator(Sseq &s);

        /// Destructor.
        ~Generator() = default;

        /// Seeding.

        /**
         * @brief Reinitializes the internal state of the random-number engine
         * using a new seed value.
         *
         * @param s Seed value or seed sequence to use.
         */
        void seed(typename bit_generator::result_type s);

        template <class Sseq>
        void seed(Sseq &s);

        /// Sample random data.

        /**
         * @brief Return random integers from low to high (inclusive).
         *
         * @tparam T An integer type.
         *
         * @param low Lowest integer to be drawn.
         * @param high Largest integer to be drawn.
         *
         * @return A random integer.
         */
        template <class T>
        T integers(T low, T high);

        /**
         * @brief Return a tensor of random integers from low to high
         * (inclusive).
         *
         * @tparam T An integer type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param low Lowest integer to be drawn.
         * @param high Largest integer to be drawn.
         * @param size Output shape.
         *
         * @return A tensor of random integers.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> integers(T low, T high, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> integers(T low, T high, const shape_t<Rank> &size);

        /**
         * @brief Return random floating point numbers in the half-open
         * interval [0, 1).
         *
         * @tparam T A floating-point type.
         *
         * @return A random floating point number.
         */
        template <class T>
        T random();

        /**
         * @brief Return a tensor of random floating point numbers in the
         * half-open interval [0, 1).
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param size Output shape.
         *
         * @return A tensor of random floating point numbers.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> random(size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> random(const shape_t<Rank> &size);

        /**
         * @brief Generates a random sample from a given sequence.
         *
         * @param population A 1-dimensional tensor with the values to sample
         *     from.
         * @param weights A 1-dimensional tensor with the weights associated
         *     to each entry of population. If not provided, the sample assumes
         *     a uniform distribution over all entries of population.
         *
         * @return The generated random sample.
         *
         * @throw std::invalid_argument Thrown if the population size is 0 or
         *     if population and weights have different sizes.
         */
        template <class T, class Tag>
        T choice(const base_tensor<T, 1, Tag> &population);

        template <class T, class Tag, class TWeights, class TagWeights>
        T choice(
            const base_tensor<T, 1, Tag> &population,
            const base_tensor<TWeights, 1, TagWeights> &weights
        );

        /**
         * @brief Generates a random sample from a given sequence.
         *
         * @param population A 1-dimensional tensor with the values to sample
         *     from.
         * @param size Output shape.
         * @param weights A 1-dimensional tensor with the weights associated
         *     to each entry of population. If not provided, the sample assumes
         *     a uniform distribution over all entries of population.
         * @param replace Whether the sample is with or without replacement.
         *     Default is true, meaning that a value can be selected multiple
         *     times.
         *
         * @return A tensor with the generated random samples.
         *
         * @throw std::invalid_argument Thrown if the population size is 0, if
         *     population and weights have different sizes or if
         *     replace = false and the sample size is greater than the
         *     population size.
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T, class Tag>
        tensor<T, 1> choice(
            const base_tensor<T, 1, Tag> &population, size_t size,
            bool replace = true
        );
        template <class T, size_t Rank, class Tag>
        tensor<T, Rank> choice(
            const base_tensor<T, 1, Tag> &population, const shape_t<Rank> &size,
            bool replace = true
        );

        template <class T, class Tag, class TWeights, class TagWeights>
        tensor<T, 1> choice(
            const base_tensor<T, 1, Tag> &population, size_t size,
            const base_tensor<TWeights, 1, TagWeights> &weights,
            bool replace = true
        );
        template <class T, size_t Rank, class Tag,
                  class TWeights, class TagWeights>
        tensor<T, Rank> choice(
            const base_tensor<T, 1, Tag> &population, const shape_t<Rank> &size,
            const base_tensor<TWeights, 1, TagWeights> &weights,
            bool replace = true
        );

        /// Permutations.

        /**
         * @brief Modify a tensor in-place by shuffling its contents.
         *
         * @param arg The tensor to be shuffled.
         * @param axis Axis along which to shuffle. Defaults to Rank - 1, which
         *     means shuffle along the last axis.
         */
        template <class T, size_t Rank, class Tag>
        void shuffle(base_tensor<T, Rank, Tag> &arg, size_t axis = Rank - 1);

        /**
         * @brief Return a permuted range.
         *
         * @param n Randomly permute the range 0, 1, 2, ..., n - 1.
         *
         * @return The permuted range.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> permutation(T n);

        /**
         * @brief Randomly permute a tensor.
         *
         * @param arg Make a copy of the tensor and shuffle the elements
         *     randomly.
         * @param axis Axis along which to permute. If not provided, the
         *     flattened tensor is used.
         *
         * @return The permuted tensor.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T, size_t Rank, class Tag>
        tensor<T, 1> permutation(const base_tensor<T, Rank, Tag> &arg);

        template <class T, size_t Rank, class Tag>
        tensor<T, Rank> permutation(
            const base_tensor<T, Rank, Tag> &arg, size_t axis
        );

        /// Distributions.

        /**
         * @brief Draw samples from a Beta distribution.
         *
         * @details The probability density function for the Beta distribution
         * is
         * @f[
         *     f(x;\alpha,\beta) = \frac{\Gamma(\alpha+\beta)}
         *         {\Gamma(\alpha)\Gamma(\beta)} x^{\alpha-1} (1-x)^{\beta-1}
         * @f]
         * for 0 <= x <= 1, where @f$\alpha@f$ and @f$\beta@f$ are shape
         * parameters.
         *
         * @tparam T A floating-point type.
         *
         * @param shape1 Shape parameter. This shall be a positive value.
         * @param shape2 Shape parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T beta(T shape1, T shape2);

        /**
         * @brief Draw samples from a Beta distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param shape1 Shape parameter. This shall be a positive value.
         * @param shape2 Shape parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> beta(T shape1, T shape2, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> beta(T shape1, T shape2, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a binomial distribution.
         *
         * @details The probability mass function for the binomial distribution
         * is
         * @f[
         *     f(x;n,p) = \binom{n}{x} p^x (1-p)^{n-x}
         * @f]
         * for x = 0, 1, 2, ... , n, where n is the number of trials and p is
         * the probability of success.
         *
         * @tparam T An integer type.
         *
         * @param n Number of trials.
         * @param prob Probability of success. This shall be a value between 0
         *     and 1 (inclusive).
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T binomial(T n, double prob);

        /**
         * @brief Draw samples from a binomial distribution.
         *
         * @tparam T An integer type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param n Number of trials.
         * @param prob Probability of success. This shall be a value between 0
         *     and 1 (inclusive).
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> binomial(T n, double prob, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> binomial(T n, double prob, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Cauchy distribution.
         *
         * @details The probability density function for the Cauchy
         * distribution is
         * @f[
         *     f(x;x_0,\gamma) = \frac{1}
         *         {\pi\gamma\left[1+\left(\frac{x-x_0}{\gamma}\right)^2\right]}
         * @f]
         * for all x, where @f$x_0@f$ and @f$\gamma@f$ are location and scale
         * parameters.
         *
         * @tparam T A floating-point type.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T cauchy(T loc, T scale);

        /**
         * @brief Draw samples from a Cauchy distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> cauchy(T loc, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> cauchy(T loc, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a chi-squared distribution.
         *
         * @details The probability density function for the chi-squared
         * distribution is
         * @f[
         *     f(x;k) = \frac{1}{2^{k/2}\Gamma(k/2)} x^{k/2-1} e^{-x/2}
         * @f]
         * for x >= 0, where k is the degrees of freedom.
         *
         * @tparam T A floating-point type.
         *
         * @param df Degrees of freedom. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T chisquare(T df);

        /**
         * @brief Draw samples from a chi-squared distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param df Degrees of freedom. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> chisquare(T df, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> chisquare(T df, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from an exponential distribution.
         *
         * @details The probability density function for the exponential
         * distribution is
         * @f[
         *     f(x;\lambda) = \lambda e^{-\lambda x}
         * @f]
         * for x >= 0, where @f$\lambda@f$ is the rate parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param rate Rate parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T exponential(T rate);

        /**
         * @brief Draw samples from an exponential distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param rate Rate parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> exponential(T rate, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> exponential(T rate, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Fisher F-distribution.
         *
         * @details The probability density function for the F distribution is
         * @f[
         *     f(x;d_1,d_2) = \frac{\Gamma\left(\frac{d_1+d_2}{2}\right)}
         *         {\Gamma\left(\frac{d_1}{2}\right)\Gamma\left(\frac{d_2}{2}\right)}
         *         \frac{\left(\frac{d_1x}{d_2}\right)^{d_1/2}}
         *         {x\left(1+\frac{d_1x}{d_2}\right)^{(d_1+d_2)/2}}
         * @f]
         * for x >= 0, where d1 and d2 are the degrees of freedom.
         *
         * @tparam T A floating-point type.
         *
         * @param df1 Degrees of freedom. This shall be a positive value.
         * @param df2 Degrees of freedom. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T fisher_f(T df1, T df2);

        /**
         * @brief Draw samples from a Fisher F-distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param df1 Degrees of freedom. This shall be a positive value.
         * @param df2 Degrees of freedom. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> fisher_f(T df1, T df2, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> fisher_f(T df1, T df2, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Gamma distribution.
         *
         * @details The probability density function for the Gamma distribution
         * is
         * @f[
         *     f(x;\alpha,\beta) = \frac{1}{\Gamma(\alpha)\beta^{\alpha}}
         *         x^{\alpha-1} e^{-x/\beta}
         * @f]
         * for x > 0, where @f$\alpha@f$ is the shape parameter and @f$\beta@f$
         * is the scale parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T gamma(T shape, T scale);

        /**
         * @brief Draw samples from a Gamma distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> gamma(T shape, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> gamma(T shape, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a geometric distribution.
         *
         * @details The probability mass function for the geometric
         * distribution is
         * @f[
         *     f(x;p) = p(1-p)^x
         * @f]
         * for x = 0, 1, 2, ..., where p is the probability of success.
         *
         * @tparam T An integer type.
         *
         * @param prob Probability of success. This shall be a value between 0
         *     and 1 (inclusive).
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T geometric(double prob);

        /**
         * @brief Draw samples from a geometric distribution.
         *
         * @tparam T An integer type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param prob Probability of success. This shall be a value between 0
         *     and 1 (inclusive).
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> geometric(double prob, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> geometric(double prob, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Gumbel distribution.
         *
         * @details The probability density function for the Gumbel
         * distribution is
         * @f[
         *     f(x;a,b) = \frac{1}{b}e^{-z-e^{-z}}, \ z=\frac{x-a}{b}
         * @f]
         * for all x, where a is the location parameter and b is the scale
         * parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T gumbel(T loc, T scale);

        /**
         * @brief Draw samples from a Gumbel distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> gumbel(T loc, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> gumbel(T loc, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Laplace distribution.
         *
         * @details The probability density function for the Laplace
         * distribution is
         * @f[
         *     f(x;\mu,s) = \frac{1}{2s}\exp\left(-\frac{|x-\mu|}{s}\right)
         * @f]
         * for all x, where @f$\mu@f$ is the location parameter and s is the
         * scale parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T laplace(T loc, T scale);

        /**
         * @brief Draw samples from a Laplace distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> laplace(T loc, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> laplace(T loc, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a logistic distribution.
         *
         * @details The probability density function for the logistic
         * distribution is
         * @f[
         *     f(x;\mu,s) = \frac{e^{-(x-\mu)/2}}
         *         {s\left(1+e^{-(x-\mu)/s}\right)^2}
         * @f]
         * for all x, where @f$\mu@f$ is the location parameter and s is the
         * scale parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T logistic(T loc, T scale);

        /**
         * @brief Draw samples from a logistic distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> logistic(T loc, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> logistic(T loc, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a log-normal distribution.
         *
         * @details The probability density function for the log-normal
         * distribution is
         * @f[
         *     f(x;\mu,\sigma) = \frac{1}{x\sigma\sqrt{2\pi}}
         *         \exp\left(-\frac{(\log x-\mu)^2}{2\sigma^2}\right)
         * @f]
         * for x > 0, where @f$\mu@f$ and @f$\sigma@f$ are the mean and
         * standard deviation of the underlying normal distribution formed by
         * the logaritm transformation.
         *
         * @tparam T A floating-point type.
         *
         * @param logmean Mean of the underlying normal distribution.
         * @param logscale Standard deviation of the underlying normal
         *     distribution. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T lognormal(T logmean, T logscale);

        /**
         * @brief Draw samples from a log-normal distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param logmean Mean of the underlying normal distribution.
         * @param logscale Standard deviation of the underlying normal
         *     distribution. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> lognormal(T logmean, T logscale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> lognormal(
            T logmean, T logscale, const shape_t<Rank> &size
        );

        /**
         * @brief Draw samples from a negative binomial distribution.
         *
         * @details The probability mass function for the negative binomial
         * distribution is
         * @f[
         *     f(x;n,p) = \binom{n+x-1}{x} p^{n} (1-p)^{x}
         * @f]
         * for x = 0, 1, 2, ..., where n is the number of successes before the
         * experiment is stopped and p is the probability of success.
         *
         * @tparam T An integer type.
         *
         * @param n Number of successes.
         * @param prob Probability of success. This shall be a value between 0
         *     and 1 (inclusive).
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T negative_binomial(T n, double prob);

        /**
         * @brief Draw samples from a negative binomial distribution.
         *
         * @tparam T An integer type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param n Number of successes.
         * @param prob Probability of success. This shall be a value between 0
         *     and 1 (inclusive).
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> negative_binomial(T n, double prob, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> negative_binomial(
            T n, double prob, const shape_t<Rank> &size
        );

        /**
         * @brief Draw samples from a normal distribution.
         *
         * @details The probability density function for the normal
         * distribution is
         * @f[
         *     f(x;\mu,\sigma) = \frac{1}{\sigma\sqrt{2\pi}}
         *         \exp\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)
         * @f]
         * for all x, where @f$\mu@f$ and @f$\sigma@f$ are the mean and
         * standard deviation.
         *
         * @tparam T A floating-point type.
         *
         * @param mean Mean of the distribution.
         * @param stddev Standard deviation of the distribution. This shall be
         *     a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T normal(T mean, T stddev);

        /**
         * @brief Draw samples from a normal distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param mean Mean of the distribution.
         * @param stddev Standard deviation of the distribution. This shall be
         *     a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> normal(T mean, T stddev, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> normal(T mean, T stddev, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Pareto distribution.
         *
         * @details The probability density function for the Pareto
         * distribution is
         * @f[
         *     f(x;\alpha,x_m) = \frac{\alpha x_m^{\alpha}}{x^{\alpha+1}}
         * @f]
         * for x >= x_m, where @f$\alpha@f$ is the shape parameter and
         * @f$x_m@f$ is the scale parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T pareto(T shape, T scale);

        /**
         * @brief Draw samples from a Pareto distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> pareto(T shape, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> pareto(T shape, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Poisson distribution.
         *
         * @details The probability mass function for the Poisson distribution
         * is
         * @f[
         *     f(x;\lambda) = \frac{\lambda^{x}}{x!} e^{-\lambda}
         * @f]
         * for x = 0, 1, 2, ..., where @f$\lambda@f$ is the rate parameter.
         *
         * @tparam T An integer type.
         *
         * @param rate Rate parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T poisson(double rate);

        /**
         * @brief Draw samples from a Poisson distribution.
         *
         * @tparam T An integer type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param rate Rate parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> poisson(double rate, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> poisson(double rate, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Rayleigh distribution.
         *
         * @details The probability density function for the Rayleigh
         * distribution is
         * @f[
         *     f(x;\sigma) = \frac{x}{\sigma^2}e^{-x^2/(2\sigma^2)}
         * @f]
         * for x >= 0, where @f$\sigma@f$ is the scale parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T rayleigh(T scale);

        /**
         * @brief Draw samples from a Rayleigh distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> rayleigh(T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> rayleigh(T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a standard normal distribution (mean = 0,
         * stddev = 1).
         *
         * @tparam T A floating-point type.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T standard_normal();

        /**
         * @brief Draw samples from a standard normal distribution (mean = 0,
         * stddev = 1).
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> standard_normal(size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> standard_normal(const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Student's t distribution.
         *
         * @details The probability density function for the Student's t
         * distribution is
         * @f[
         *     f(x;\nu) = \frac{1}{\sqrt{\pi\nu}}
         *         \frac{\Gamma\left(\frac{\nu+1}{2}\right)}
         *         {\Gamma\left(\frac{\nu}{2}\right)}
         *         \left(1+\frac{x^2}{\nu}\right)^{-(\nu+1)/2}
         * @f]
         * for all x, where @f$\nu@f$ is the degrees of freedom.
         *
         * @tparam T A floating-point type.
         *
         * @param df Degrees of freedom. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T student_t(T df);

        /**
         * @brief Draw samples from a Student's t distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param df Degrees of freedom. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> student_t(T df, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> student_t(T df, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a triangular distribution.
         *
         * @details The probability density function for the triangular
         * distribution is
         * @f[
         *     f(x;a,c,b) = \begin{cases}
         *         \frac{2(x-a)}{(b-a)(c-a)}, & a \leq x \leq c,\\
         *         \frac{2(b-x)}{(b-a)(b-c)}, & c \leq x \leq b,\\
         *         0, & \text{otherwise}
         *     \end{cases}
         * @f]
         * where a is the lower boundary, b is the upper boundary, and c is the
         * mode of the distribution.
         *
         * @tparam T A floating-point type.
         *
         * @param left Lower boundary.
         * @param mode Mode of the distribution.
         * @param right Upper boundary.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T triangular(T left, T mode, T right);

        /**
         * @brief Draw samples from a triangular distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param left Lower boundary.
         * @param mode Mode of the distribution.
         * @param right Upper boundary.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> triangular(T left, T mode, T right, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> triangular(
            T left, T mode, T right, const shape_t<Rank> &size
        );

        /**
         * @brief Draw samples from an uniform distribution.
         *
         * @details The probability density function for the uniform
         * distribution is
         * @f[
         *   f(x;a,b) = \frac{1}{b-a}
         * @f]
         * for a <= x < b, where a and b are the lower and upper boundaries of
         * the distribution.
         *
         * @tparam T A floating-point type.
         *
         * @param low Lower boundary.
         * @param high Upper boundary.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T uniform(T low, T high);

        /**
         * @brief Draw samples from an uniform distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param low Lower boundary.
         * @param high Upper boundary.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> uniform(T low, T high, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> uniform(T low, T high, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Wald, or inverse Gaussian, distribution.
         *
         * @details The probability density function for the Wald distribution
         * is
         * @f[
         *     f(x;\mu,\lambda) = \sqrt{\frac{\lambda}{2\pi x^3}}
         *         \exp\left(-\frac{\lambda(x-\mu)^2}{2\mu^2 x}\right)
         * @f]
         * for x > 0, where @f$\mu@f$ is the mean and @f$\lambda@f$ is the
         * scale parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param mean Mean of the distribution. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T wald(T mean, T scale);

        /**
         * @brief Draw samples from a Wald distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param mean Mean of the distribution. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> wald(T mean, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> wald(T mean, T scale, const shape_t<Rank> &size);

        /**
         * @brief Draw samples from a Weibull distribution.
         *
         * @details The probability density function for the Weibull
         * distribution is
         * @f[
         *     f(x;a,b) = \frac{a}{b}\left(\frac{x}{b}\right)^{a-1}e^{-(x/b)^a}
         * @f]
         * for x >= 0, where a is the shape parameter and b is the scale
         * parameter.
         *
         * @tparam T A floating-point type.
         *
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         *
         * @return A sample from the distribution.
         */
        template <class T>
        T weibull(T shape, T scale);

        /**
         * @brief Draw samples from a Weibull distribution.
         *
         * @tparam T A floating-point type.
         * @tparam Rank Dimension of the tensor.
         *
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param size Output shape.
         *
         * @return A tensor with samples from the distribution.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class T>
        tensor<T, 1> weibull(T shape, T scale, size_t size);
        template <class T, size_t Rank>
        tensor<T, Rank> weibull(T shape, T scale, const shape_t<Rank> &size);

    private:
        /**
         * @brief Sample values from a distribution.
         */
        template <class OutputIterator, class Distribution>
        void __sample_distribution(
            OutputIterator first, size_t size, Distribution &rvs
        );

        // Underlying uniform random number generator.
        bit_generator m_rng;
    };

    using default_rng = Generator<std::default_random_engine>;
}

#include "numcpp/random/random.tcc"

#endif // NUMCPP_RANDOM_H_INCLUDED
