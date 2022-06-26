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
         * @param low Lowest integer to be drawn.
         * @param high Largest integer to be drawn.
         * 
         * @return A random integer.
         */
        template <class T>
        T integers(T low, T high);

        /**
         * @brief Return an array of random integers from low to high 
         * (inclusive).
         * 
         * @param low Lowest integer to be drawn.
         * @param high Largest integer to be drawn.
         * @param n Length of array.
         * 
         * @return An array of random integers.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> integers(T low, T high, size_t n);

        /**
         * @brief Return a matrix of random integers from low to high 
         * (inclusive).
         * 
         * @param low Lowest integer to be drawn.
         * @param high Largest integer to be drawn.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix of random integers.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> integers(T low, T high, size_t m, size_t n);

        /**
         * @brief Return random floating point numbers in the half-open 
         * interval [0, 1).
         * 
         * @return A random floating point number.
         */
        template <class T>
        T random();

        /**
         * @brief Return an array of random floating point numbers in the 
         * half-open interval [0, 1).
         * 
         * @param n Length of array.
         * 
         * @return An array of random floating point numbers.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> random(size_t n);

        /**
         * @brief Return a matrix of random floating point numbers in the 
         * half-open interval [0, 1).
         * 
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix of random floating point numbers.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> random(size_t m, size_t n);

        /**
         * @brief Generates a random sample from a given array.
         * 
         * @param arr An array-like object with the values to sample from.
         * @param weights An array-like object with the weights associated to 
         *     each entry of arr. If not provided, the sample assumes a uniform 
         *     distribution over all entries of arr.
         * 
         * @return The generated random sample.
         * 
         * @throw std::invalid_argument Thrown if the population size is 0 or 
         *     if the population and weights have different sizes.
         */
        template <class T, class Tag>
        T choice(const base_array<T, Tag> &arr);

        template <class T, class Tag, class TWeights, class TagWeights>
        T choice(
            const base_array<T, Tag> &arr, 
            const base_array<TWeights, TagWeights> &weights 
        );

        /**
         * @brief Generates a random sample from a given array.
         * 
         * @param arr An array-like object with the values to sample from.
         * @param size Number of samples to drawn.
         * @param weights An array-like object with the weights associated to 
         *     each entry of arr. If not provided, the sample assumes a uniform 
         *     distribution over all entries of arr.
         * @param replace Whether the sample is with or without replacement. 
         *     Default is true, meaning that a value can be selected multiple 
         *     times.
         * 
         * @return An array with the generated random samples.
         * 
         * @throw std::invalid_argument Thrown if the population size is 0, if 
         *     the population and weights have different sizes or if 
         *     replace = false and the sample size is greater than the 
         *     population size.
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T, class Tag>
        array<T> choice(
            const base_array<T, Tag> &arr, size_t size, bool replace = true
        );

        template <class T, class Tag, class TWeights, class TagWeights>
        array<T> choice(
            const base_array<T, Tag> &arr, size_t size, 
            const base_array<TWeights, TagWeights> &weights, bool replace = true
        );

        /// Permutations.

        /**
         * @brief Modify an array in-place by shuffling its contents.
         * 
         * @param arr The array to be shuffled.
         */
        template <class T>
        void shuffle(array<T> &arr);

        template <class T>
        void shuffle(array_view<T> arr);

        template <class T>
        void shuffle(index_view<T> arr);
        
        /**
         * @brief Randomly permute an array or return a permuted range.
         * 
         * @param n If an integer is given, randomly permute the range 
         *     0, 1, 2, ..., n - 1.
         * @param arr If an array is given, make a copy and shuffle the 
         *     elements randomly.
         * 
         * @return The permuted array or range.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> permutation(size_t n);

        template <class T, class Tag>
        array<T> permutation(const base_array<T, Tag> &arr);

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
         * @param shape1 Shape parameter. This shall be a positive value.
         * @param shape2 Shape parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T beta(const T &shape1, const T &shape2);

        /**
         * @brief Draw samples from a Beta distribution.
         * 
         * @param shape1 Shape parameter. This shall be a positive value.
         * @param shape2 Shape parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> beta(const T &shape1, const T &shape2, size_t n);

        /**
         * @brief Draw samples from a Beta distribution.
         * 
         * @param shape1 Shape parameter. This shall be a positive value.
         * @param shape2 Shape parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> beta(const T &shape1, const T &shape2, size_t m, size_t n);

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
         * @param size Number of trials.
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T binomial(size_t size, double prob);

        /**
         * @brief Draw samples from a binomial distribution.
         *  
         * @param size Number of trials.
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> binomial(size_t size, double prob, size_t n);

        /**
         * @brief Draw samples from a binomial distribution.
         *  
         * @param size Number of trials.
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> binomial(size_t size, double prob, size_t m, size_t n);

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
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T cauchy(const T &loc, const T &scale);

        /**
         * @brief Draw samples from a Cauchy distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> cauchy(const T &loc, const T &scale, size_t n);

        /**
         * @brief Draw samples from a Cauchy distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> cauchy(const T &loc, const T &scale, size_t m, size_t n);

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
         * @param df Degrees of freedom. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T chisquare(const T &df);

        /**
         * @brief Draw samples from a chi-squared distribution.
         * 
         * @param df Degrees of freedom. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> chisquare(const T &df, size_t n);

        /**
         * @brief Draw samples from a chi-squared distribution.
         * 
         * @param df Degrees of freedom. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> chisquare(const T &df, size_t m, size_t n);

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
         * @param rate Rate parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T exponential(const T &rate);

        /**
         * @brief Draw samples from an exponential distribution.
         * 
         * @param rate Rate parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> exponential(const T &rate, size_t n);

        /**
         * @brief Draw samples from an exponential distribution.
         * 
         * @param rate Rate parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> exponential(const T &rate, size_t m, size_t n);

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
         * @param df1 Degrees of freedom. This shall be a positive value.
         * @param df2 Degrees of freedom. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T fisher_f(const T &df1, const T &df2);

        /**
         * @brief Draw samples from a Fisher F-distribution.
         * 
         * @param df1 Degrees of freedom. This shall be a positive value.
         * @param df2 Degrees of freedom. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> fisher_f(const T &df1, const T &df2, size_t n);

        /**
         * @brief Draw samples from a Fisher F-distribution.
         * 
         * @param df1 Degrees of freedom. This shall be a positive value.
         * @param df2 Degrees of freedom. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> fisher_f(const T &df1, const T &df2, size_t m, size_t n);

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
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T gamma(const T &shape, const T &scale);

        /**
         * @brief Draw samples from a Gamma distribution.
         * 
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> gamma(const T &shape, const T &scale, size_t n);

        /**
         * @brief Draw samples from a Gamma distribution.
         * 
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> gamma(const T &shape, const T &scale, size_t m, size_t n);

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
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> geometric(double prob, size_t n);

        /**
         * @brief Draw samples from a geometric distribution.
         * 
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> geometric(double prob, size_t m, size_t n);

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
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T gumbel(const T &loc, const T &scale);

        /**
         * @brief Draw samples from a Gumbel distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> gumbel(const T &loc, const T &scale, size_t n);

        /**
         * @brief Draw samples from a Gumbel distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> gumbel(const T &loc, const T &scale, size_t m, size_t n);

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
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T laplace(const T &loc, const T &scale);

        /**
         * @brief Draw samples from a Laplace distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> laplace(const T &loc, const T &scale, size_t n);

        /**
         * @brief Draw samples from a Laplace distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> laplace(const T &loc, const T &scale, size_t m, size_t n);

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
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T logistic(const T &loc, const T &scale);

        /**
         * @brief Draw samples from a logistic distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> logistic(const T &loc, const T &scale, size_t n);

        /**
         * @brief Draw samples from a logistic distribution.
         * 
         * @param loc Location parameter.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> logistic(const T &loc, const T &scale, size_t m, size_t n);

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
         * @param logmean Mean of the underlying normal distribution.
         * @param logscale Standard deviation of the underlying normal 
         *     distribution. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T lognormal(const T &logmean, const T &logscale);

        /**
         * @brief Draw samples from a log-normal distribution.
         * 
         * @param logmean Mean of the underlying normal distribution.
         * @param logscale Standard deviation of the underlying normal 
         *     distribution. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> lognormal(const T &logmean, const T &logscale, size_t n);

        /**
         * @brief Draw samples from a log-normal distribution.
         * 
         * @param logmean Mean of the underlying normal distribution.
         * @param logscale Standard deviation of the underlying normal 
         *     distribution. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> lognormal(
            const T &logmean, const T &logscale, size_t m, size_t n
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
         * @param size Number of successes.
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T negative_binomial(size_t size, double prob);

        /**
         * @brief Draw samples from a negative binomial distribution.
         * 
         * @param size Number of successes.
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> negative_binomial(size_t size, double prob, size_t n);

        /**
         * @brief Draw samples from a negative binomial distribution.
         * 
         * @param size Number of successes.
         * @param prob Probability of success. This shall be a value between 0 
         *     and 1 (inclusive).
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> negative_binomial(
            size_t size, double prob, size_t m, size_t n
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
         * @param mean Mean of the distribution.
         * @param stddev Standard deviation of the distribution. This shall be 
         *     a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T normal(const T &mean, const T &stddev);

        /**
         * @brief Draw samples from a normal distribution.
         * 
         * @param mean Mean of the distribution.
         * @param stddev Standard deviation of the distribution. This shall be 
         *     a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> normal(const T &mean, const T &stddev, size_t n);

        /**
         * @brief Draw samples from a normal distribution.
         * 
         * @param mean Mean of the distribution.
         * @param stddev Standard deviation of the distribution. This shall be 
         *     a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> normal(const T &mean, const T &stddev, size_t m, size_t n);

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
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T pareto(const T &shape, const T &scale);

        /**
         * @brief Draw samples from a Pareto distribution.
         * 
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> pareto(const T &shape, const T &scale, size_t n);

        /**
         * @brief Draw samples from a Pareto distribution.
         * 
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> pareto(const T &shape, const T &scale, size_t m, size_t n);

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
         * @param rate Rate parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T poisson(double rate);

        /**
         * @brief Draw samples from a Poisson distribution.
         * 
         * @param rate Rate parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> poisson(double rate, size_t n);

        /**
         * @brief Draw samples from a Poisson distribution.
         * 
         * @param rate Rate parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> poisson(double rate, size_t m, size_t n);

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
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T rayleigh(const T &scale);

        /**
         * @brief Draw samples from a Rayleigh distribution.
         * 
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> rayleigh(const T &scale, size_t n);

        /**
         * @brief Draw samples from a Rayleigh distribution.
         * 
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> rayleigh(const T &scale, size_t m, size_t n);

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
         * @param df Degrees of freedom. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T student_t(const T &df);

        /**
         * @brief Draw samples from a Student's t distribution.
         * 
         * @param df Degrees of freedom. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> student_t(const T &df, size_t n);

        /**
         * @brief Draw samples from a Student's t distribution.
         * 
         * @param df Degrees of freedom. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> student_t(const T &df, size_t m, size_t n);

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
         * @param left Lower boundary.
         * @param mode Mode of the distribution.
         * @param right Upper boundary.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T triangular(const T &left, const T &mode, const T &right);

        /**
         * @brief Draw samples from a triangular distribution.
         * 
         * @param left Lower boundary.
         * @param mode Mode of the distribution.
         * @param right Upper boundary.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> triangular(
            const T &left, const T &mode, const T &right, size_t n
        );

        /**
         * @brief Draw samples from a triangular distribution.
         * 
         * @param left Lower boundary.
         * @param mode Mode of the distribution.
         * @param right Upper boundary.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> triangular(
            const T &left, const T &mode, const T &right, size_t m, size_t n
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
         * @param low Lower boundary.
         * @param high Upper boundary.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T uniform(const T &low, const T &high);

        /**
         * @brief Draw samples from an uniform distribution.
         * 
         * @param low Lower boundary.
         * @param high Upper boundary.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> uniform(const T &low, const T &high, size_t n);

        /**
         * @brief Draw samples from an uniform distribution.
         * 
         * @param low Lower boundary.
         * @param high Upper boundary.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> uniform(const T &low, const T &high, size_t m, size_t n);

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
         * @param mean Mean of the distribution. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T wald(const T &mean, const T& scale);

        /**
         * @brief Draw samples from a Wald distribution.
         * 
         * @param mean Mean of the distribution. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> wald(const T &mean, const T& scale, size_t n);

        /**
         * @brief Draw samples from a Wald distribution.
         * 
         * @param mean Mean of the distribution. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> wald(const T &mean, const T& scale, size_t m, size_t n);

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
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * 
         * @return A sample from the distribution.
         */
        template <class T>
        T weibull(const T &shape, const T &scale);

        /**
         * @brief Draw samples from a Weibull distribution.
         * 
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param n Length of array.
         * 
         * @return An array with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        array<T> weibull(const T &shape, const T &scale, size_t n);

        /**
         * @brief Draw samples from a Weibull distribution.
         * 
         * @param shape Shape parameter. This shall be a positive value.
         * @param scale Scale parameter. This shall be a positive value.
         * @param m Number of rows.
         * @param n Number of columns.
         * 
         * @return A matrix with samples from the distribution.
         * 
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class T>
        matrix<T> weibull(const T &shape, const T &scale, size_t m, size_t n);

    protected:
        /// Underlying uniform random number generator.
        bit_generator m_rng;
    };

    using default_rng = Generator<std::default_random_engine>;
}

#include "numcpp/random/random.tcc"

#endif // NUMCPP_RANDOM_H_INCLUDED
