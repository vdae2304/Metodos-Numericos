/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
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
 * transforms sequence of random bits into a sequence of numbers that follow a
 * specific probability distribution (such as uniform, normal or binomial).
 *
 * @tparam bit_generator Random number engine to use. Must meet the requirements
 *                       listed by Uniform Random Bit Generator and Random
 *                       Number Engine. Common examples are
 *                       @c std::default_random_engine and
 *                       @c std::random_device.
 *
 * @cite For more details, see
 * [Uniform Random Bit Generator]
 * (https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator)
 * [Random Number Engine]
 * (https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine)
 */
template <class bit_generator> class Generator {
public:
  /// Constructors.

  /**
   * @brief Default constructor. Constructs a Generator with default seed.
   */
  Generator();

  /**
   * @brief Seed constructor. Constructs a Generator from a seed value or a seed
   * sequence.
   *
   * @param s Seed value or seed sequence to use.
   */
  template <class Sseq> Generator(Sseq &&s);

  /// Destructor.
  ~Generator() = default;

  /// Seeding.

  /**
   * @brief Reinitializes the internal state of the random-number engine
   * using a new seed value.
   *
   * @param s Seed value or seed sequence to use.
   */
  template <class Sseq> void seed(Sseq &&s);

  /// Sample random data.

  /**
   * @brief Return random integers from @a low to @a high (inclusive).
   *
   * @param low Lowest integer to be drawn.
   * @param high Largest integer to be drawn.
   *
   * @return A random integer.
   */
  template <class T, class U>
  typename std::common_type<T, U>::type integers(T low, U high);

  /**
   * @brief Return a tensor of random integers from @a low to @a high
   * (inclusive).
   *
   * @param low Lowest integer to be drawn.
   * @param high Largest integer to be drawn.
   * @param size Output shape.
   *
   * @return A tensor of random integers.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename std::common_type<T, U>::type, 1> integers(T low, U high,
                                                            size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename std::common_type<T, U>::type, Rank>
  integers(T low, U high, const shape_t<Rank> &size);

  /**
   * @brief Return random floating-point numbers in the half-open interval
   * [0, 1).
   *
   * @return A random floating-point number.
   */
  template <class T = double> T random();

  /**
   * @brief Return a tensor of random floating-point numbers in the half-open
   * interval [0, 1).
   *
   * @param size Output shape.
   *
   * @return A tensor of random floating-point numbers.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T = double> tensor<T, 1> random(size_t size);

  template <class T = double, size_t Rank>
  tensor<T, Rank> random(const shape_t<Rank> &size);

  /**
   * @brief Generates a random sample from a given sequence.
   *
   * @param population A 1-dimensional tensor with the values to sample from.
   * @param weights A 1-dimensional tensor with the weights associated to each
   *                entry of @a population. If not provided, the sample assumes
   *                a uniform distribution over all entries of @a population.
   *
   * @return The generated random sample.
   *
   * @throw std::invalid_argument Thrown if the population size is 0 or if
   *                              @a population and @a weights have different
   *                              sizes.
   */
  template <class Container, class T>
  T choice(const expression<Container, T, 1> &population);

  template <class Container1, class T, class Container2, class U>
  T choice(const expression<Container1, T, 1> &population,
           const expression<Container2, U, 1> &weights);

  /**
   * @brief Generates a random sample from a given sequence.
   *
   * @param population A 1-dimensional tensor with the values to sample from.
   * @param size Output shape.
   * @param weights A 1-dimensional tensor with the weights associated to each
   *                entry of @a population. If not provided, the sample assumes
   *                a uniform distribution over all entries of @a population.
   * @param replace Whether the sample is with or without replacement. Default
   *                is true, meaning that a value can be selected multiple
   *                times.
   * @param shuffle Whether the sample is shuffled when sampling without
   *                replacement. Default is true.
   *
   * @return A tensor with the generated random samples.
   *
   * @throw std::invalid_argument Thrown if the population size is 0, if
   *                              @a population and @a weights have different
   *                              sizes or if @a replace=false and the sample
   *                              size is greater than the population size.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class Container, class T>
  tensor<T, 1> choice(const expression<Container, T, 1> &population,
                      size_t size, bool replace = true, bool shuffle = true);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> choice(const expression<Container, T, 1> &population,
                         const shape_t<Rank> &size, bool replace = true,
                         bool shuffle = true);

  template <class Container1, class T, class Container2, class U>
  tensor<T, 1> choice(const expression<Container1, T, 1> &population,
                      size_t size, const expression<Container2, U, 1> &weights,
                      bool replace = true);

  template <class Container1, class T, size_t Rank, class Container2, class U>
  tensor<T, Rank> choice(const expression<Container1, T, 1> &population,
                         const shape_t<Rank> &size,
                         const expression<Container2, U, 1> &weights,
                         bool replace = true);

  /// Permutations.

  /**
   * @brief Modify a tensor in-place by shuffling its contents.
   *
   * @param a The tensor to be shuffled.
   * @param axis Axis along which to shuffle. Defaults to Rank - 1, which means
   *             shuffle along the last axis.
   */
  template <class Container, class T, size_t Rank>
  void shuffle(dense_tensor<Container, T, Rank> &a, size_t axis = Rank - 1);

  /**
   * @brief Return a permuted range.
   *
   * @param n Randomly permute the range 0, 1, 2, ..., @a n - 1.
   *
   * @return The permuted range.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, detail::RequiresIntegral<T> = 0>
  tensor<T, 1> permutation(T n);

  /**
   * @brief Randomly permute a tensor.
   *
   * @param a Make a copy of the tensor and shuffle the elements randomly.
   * @param axis Axis along which to permute. If not provided, the flattened
   *             tensor is used.
   *
   * @return The permuted tensor.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class Container, class T, size_t Rank>
  tensor<T, 1> permutation(const expression<Container, T, Rank> &a);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> permutation(const expression<Container, T, Rank> &a,
                              size_t axis);

  /// Continuous distributions.

  /**
   * @brief Draw samples from a Beta distribution.
   *
   * @details The probability density function for the Beta distribution
   * is
   * @f[
   *   f(x;\alpha,\beta) = \frac{\Gamma(\alpha+\beta)}
   *     {\Gamma(\alpha)\Gamma(\beta)} x^{\alpha-1} (1-x)^{\beta-1}
   * @f]
   * for @f$0 \leq x \leq 1@f$, where @f$\alpha@f$ and @f$\beta@f$ are shape
   * parameters.
   *
   * @param shape1 Shape parameter. This shall be a positive value.
   * @param shape2 Shape parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type beta(T shape1, U shape2);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> beta(const expression<Container1, T, Rank> &shape1,
                       const expression<Container2, T, Rank> &shape2);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> beta(const expression<Container, T, Rank> &shape1,
                       typename Container::value_type shape2);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> beta(typename Container::value_type shape1,
                       const expression<Container, T, Rank> &shape2);

  /**
   * @brief Draw samples from a Beta distribution.
   *
   * @param shape1 Shape parameter. This shall be a positive value.
   * @param shape2 Shape parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> beta(T shape1, U shape2,
                                                       size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  beta(T shape1, U shape2, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Cauchy distribution.
   *
   * @details The probability density function for the Cauchy distribution is
   * @f[
   *   f(x;x_0,\gamma) = \frac{1}
   *     {\pi\gamma\left[1+\left(\frac{x-x_0}{\gamma}\right)^2\right]}
   * @f]
   * for all @a x, where @f$x_0@f$ and @f$\gamma@f$ are location and scale
   * parameters.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type cauchy(T loc, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> cauchy(const expression<Container1, T, Rank> &loc,
                         const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> cauchy(const expression<Container, T, Rank> &loc,
                         typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> cauchy(typename Container::value_type loc,
                         const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Cauchy distribution.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> cauchy(T loc, U scale,
                                                         size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  cauchy(T loc, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a chi-squared distribution.
   *
   * @details The probability density function for the chi-squared distribution
   * is
   * @f[
   *   f(x;k) = \frac{1}{2^{k/2}\Gamma(k/2)} x^{k/2-1} e^{-x/2}
   * @f]
   * for @f$x \geq 0@f$, where @a k is the degrees of freedom.
   *
   * @param df Degrees of freedom. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T> typename detail::promote<T>::type chisquare(T df);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> chisquare(const expression<Container, T, Rank> &df);

  /**
   * @brief Draw samples from a chi-squared distribution.
   *
   * @param df Degrees of freedom. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T>
  tensor<typename detail::promote<T>::type, 1> chisquare(T df, size_t size);

  template <class T, size_t Rank>
  tensor<typename detail::promote<T>::type, Rank>
  chisquare(T df, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from an exponential distribution.
   *
   * @details The probability density function for the exponential distribution
   * is
   * @f[
   *   f(x;\lambda) = \lambda e^{-\lambda x}
   * @f]
   * for @f$x \geq 0@f$, where @f$\lambda@f$ is the rate parameter.
   *
   * @param rate Rate parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T> typename detail::promote<T>::type exponential(T rate);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> exponential(const expression<Container, T, Rank> &rate);

  /**
   * @brief Draw samples from an exponential distribution.
   *
   * @param rate Rate parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T>
  tensor<typename detail::promote<T>::type, 1> exponential(T rate, size_t size);

  template <class T, size_t Rank>
  tensor<typename detail::promote<T>::type, Rank>
  exponential(T rate, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Fisher F-distribution.
   *
   * @details The probability density function for the F distribution is
   * @f[
   *   f(x;d_1,d_2) = \frac{\Gamma\left(\frac{d_1+d_2}{2}\right)}
   *     {\Gamma\left(\frac{d_1}{2}\right)\Gamma\left(\frac{d_2}{2}\right)}
   *     \frac{\left(\frac{d_1x}{d_2}\right)^{d_1/2}}
   *     {x\left(1+\frac{d_1x}{d_2}\right)^{(d_1+d_2)/2}}
   * @f]
   * for @f$x \geq 0@f$, where @f$d_1@f$ and @f$d_2@f$ are the degrees of
   * freedom.
   *
   * @param df1 Degrees of freedom. This shall be a positive value.
   * @param df2 Degrees of freedom. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type fisher_f(T df1, U df2);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> fisher_f(const expression<Container1, T, Rank> &df1,
                           const expression<Container2, T, Rank> &df2);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> fisher_f(const expression<Container, T, Rank> &df1,
                           typename Container::value_type df2);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> fisher_f(typename Container::value_type df1,
                           const expression<Container, T, Rank> &df2);

  /**
   * @brief Draw samples from a Fisher F-distribution.
   *
   * @param df1 Degrees of freedom. This shall be a positive value.
   * @param df2 Degrees of freedom. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> fisher_f(T df1, U df2,
                                                           size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  fisher_f(T df1, U df2, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Gamma distribution.
   *
   * @details The probability density function for the Gamma distribution is
   * @f[
   *   f(x;\alpha,\beta) = \frac{1}{\Gamma(\alpha)\beta^{\alpha}} x^{\alpha-1}
   *     e^{-x/\beta}
   * @f]
   * for @f$x > 0@f$, where @f$\alpha@f$ is the shape parameter and @f$\beta@f$
   * is the scale parameter.
   *
   * @param shape Shape parameter. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type gamma(T shape, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> gamma(const expression<Container1, T, Rank> &shape,
                        const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> gamma(const expression<Container, T, Rank> &shape,
                        typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> gamma(typename Container::value_type shape,
                        const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Gamma distribution.
   *
   * @param shape Shape parameter. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> gamma(T shape, U scale,
                                                        size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  gamma(T shape, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Gumbel distribution.
   *
   * @details The probability density function for the Gumbel distribution is
   * @f[
   *   f(x;a,b) = \frac{1}{b}e^{-z-e^{-z}}, \ z=\frac{x-a}{b}
   * @f]
   * for all @a x, where @a a is the location parameter and @a b is the scale
   * parameter.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type gumbel(T loc, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> gumbel(const expression<Container1, T, Rank> &loc,
                         const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> gumbel(const expression<Container, T, Rank> &loc,
                         typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> gumbel(typename Container::value_type loc,
                         const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Gumbel distribution.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> gumbel(T loc, U scale,
                                                         size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  gumbel(T loc, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Laplace distribution.
   *
   * @details The probability density function for the Laplace distribution is
   * @f[
   *   f(x;\mu,s) = \frac{1}{2s}\exp\left(-\frac{|x-\mu|}{s}\right)
   * @f]
   * for all @a x, where @f$\mu@f$ is the location parameter and @f$s@f$ is the
   * scale parameter.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type laplace(T loc, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> laplace(const expression<Container1, T, Rank> &loc,
                          const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> laplace(const expression<Container, T, Rank> &loc,
                          typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> laplace(typename Container::value_type loc,
                          const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Laplace distribution.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> laplace(T loc, U scale,
                                                          size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  laplace(T loc, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a logistic distribution.
   *
   * @details The probability density function for the logistic distribution is
   * @f[
   *   f(x;\mu,s) = \frac{e^{-(x-\mu)/2}}{s\left(1+e^{-(x-\mu)/s}\right)^2}
   * @f]
   * for all @a x, where @f$\mu@f$ is the location parameter and @f$s@f$ is the
   * scale parameter.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type logistic(T loc, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> logistic(const expression<Container1, T, Rank> &loc,
                           const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> logistic(const expression<Container, T, Rank> &loc,
                           typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> logistic(typename Container::value_type loc,
                           const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a logistic distribution.
   *
   * @param loc Location parameter.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> logistic(T loc, U scale,
                                                           size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  logistic(T loc, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a log-normal distribution.
   *
   * @details The probability density function for the log-normal distribution
   * is
   * @f[
   *   f(x;\mu,\sigma) = \frac{1}{x\sigma\sqrt{2\pi}}
   *     \exp\left(-\frac{(\log x-\mu)^2}{2\sigma^2}\right)
   * @f]
   * for @f$x > 0@f$, where @f$\mu@f$ and @f$\sigma@f$ are the mean and standard
   * deviation of the underlying normal distribution formed by the logaritm
   * transformation.
   *
   * @param logmean Mean of the underlying normal distribution.
   * @param logscale Standard deviation of the underlying normal distribution.
   *                 This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type lognormal(T logmean, U logscale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> lognormal(const expression<Container1, T, Rank> &logmean,
                            const expression<Container2, T, Rank> &logscale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> lognormal(const expression<Container, T, Rank> &logmean,
                            typename Container::value_type logscale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> lognormal(typename Container::value_type logmean,
                            const expression<Container, T, Rank> &logscale);

  /**
   * @brief Draw samples from a log-normal distribution.
   *
   * @param logmean Mean of the underlying normal distribution.
   * @param logscale Standard deviation of the underlying normal distribution.
   *                 This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1>
  lognormal(T logmean, U logscale, size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  lognormal(T logmean, U logscale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a normal distribution.
   *
   * @details The probability density function for the normal distribution is
   * @f[
   *   f(x;\mu,\sigma) = \frac{1}{\sigma\sqrt{2\pi}}
   *     \exp\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)
   * @f]
   * for all @a x, where @f$\mu@f$ and @f$\sigma@f$ are the mean and standard
   * deviation.
   *
   * @param mean Mean of the distribution.
   * @param stddev Standard deviation of the distribution. This shall be a
   *               positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type normal(T mean, U stddev);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> normal(const expression<Container1, T, Rank> &mean,
                         const expression<Container2, T, Rank> &stddev);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> normal(const expression<Container, T, Rank> &mean,
                         typename Container::value_type stddev);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> normal(typename Container::value_type mean,
                         const expression<Container, T, Rank> &stddev);

  /**
   * @brief Draw samples from a normal distribution.
   *
   * @param mean Mean of the distribution.
   * @param stddev Standard deviation of the distribution. This shall be a
   *               positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> normal(T mean, U stddev,
                                                         size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  normal(T mean, U stddev, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Pareto distribution.
   *
   * @details The probability density function for the Pareto distribution is
   * @f[
   *   f(x;\alpha,x_m) = \frac{\alpha x_m^{\alpha}}{x^{\alpha+1}}
   * @f]
   * for @f$x \geq x_m@f$, where @f$\alpha@f$ is the shape parameter and
   * @f$x_m@f$ is the scale parameter.
   *
   * @param shape Shape parameter. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type pareto(T shape, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> pareto(const expression<Container1, T, Rank> &shape,
                         const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> pareto(const expression<Container, T, Rank> &shape,
                         typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> pareto(typename Container::value_type shape,
                         const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Pareto distribution.
   *
   * @param shape Shape parameter. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> pareto(T shape, U scale,
                                                         size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  pareto(T shape, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Rayleigh distribution.
   *
   * @details The probability density function for the Rayleigh distribution is
   * @f[
   *   f(x;\sigma) = \frac{x}{\sigma^2}e^{-x^2/(2\sigma^2)}
   * @f]
   * for @f$x \geq 0@f$, where @f$\sigma@f$ is the scale parameter.
   *
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   *
   */
  template <class T> typename detail::promote<T>::type rayleigh(T scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> rayleigh(const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Rayleigh distribution.
   *
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T>
  tensor<typename detail::promote<T>::type, 1> rayleigh(T scale, size_t size);

  template <class T, size_t Rank>
  tensor<typename detail::promote<T>::type, Rank>
  rayleigh(T scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a standard normal distribution ( @a mean=0,
   * @a stddev=1 ).
   *
   * @return A sample from the distribution.
   */
  template <class T = double> T standard_normal();

  /**
   * @brief Draw samples from a standard normal distribution ( @a mean=0,
   * @a stddev = 1 ).
   *
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T = double> tensor<T, 1> standard_normal(size_t size);

  template <class T = double, size_t Rank>
  tensor<T, Rank> standard_normal(const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Student's t distribution.
   *
   * @details The probability density function for the Student's t distribution
   * is
   * @f[
   *   f(x;\nu) = \frac{1}{\sqrt{\pi\nu}}
   *     \frac{\Gamma\left(\frac{\nu+1}{2}\right)}
   *     {\Gamma\left(\frac{\nu}{2}\right)}
   *     \left(1+\frac{x^2}{\nu}\right)^{-(\nu+1)/2}
   * @f]
   * for all @a x, where @f$\nu@f$ is the degrees of freedom.
   *
   * @param df Degrees of freedom. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T> typename detail::promote<T>::type student_t(T df);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> student_t(const expression<Container, T, Rank> &df);

  /**
   * @brief Draw samples from a Student's t distribution.
   *
   * @param df Degrees of freedom. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T>
  tensor<typename detail::promote<T>::type, 1> student_t(T df, size_t size);

  template <class T, size_t Rank>
  tensor<typename detail::promote<T>::type, Rank>
  student_t(T df, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from an uniform distribution.
   *
   * @details The probability density function for the uniform distribution is
   * @f[
   *   f(x;a,b) = \frac{1}{b-a}
   * @f]
   * for @f$a \leq x < b@f$, where @a a and @a b are the lower and upper
   * boundaries of the distribution.
   *
   * @param low Lower boundary.
   * @param high Upper boundary.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type uniform(T low, U high);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> uniform(const expression<Container1, T, Rank> &low,
                          const expression<Container2, T, Rank> &high);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> uniform(const expression<Container, T, Rank> &low,
                          typename Container::value_type high);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> uniform(typename Container::value_type low,
                          const expression<Container, T, Rank> &high);

  /**
   * @brief Draw samples from an uniform distribution.
   *
   * @param low Lower boundary.
   * @param high Upper boundary.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> uniform(T low, U high,
                                                          size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  uniform(T low, U high, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Wald, or inverse Gaussian, distribution.
   *
   * @details The probability density function for the Wald distribution is
   * @f[
   *   f(x;\mu,\lambda) = \sqrt{\frac{\lambda}{2\pi x^3}}
   *     \exp\left(-\frac{\lambda(x-\mu)^2}{2\mu^2 x}\right)
   * @f]
   * for @f$x > 0@f$, where @f$\mu@f$ is the mean and @f$\lambda@f$ is the scale
   * parameter.
   *
   * @param mean Mean of the distribution. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type wald(T mean, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> wald(const expression<Container1, T, Rank> &mean,
                       const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> wald(const expression<Container, T, Rank> &mean,
                       typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> wald(typename Container::value_type mean,
                       const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Wald distribution.
   *
   * @param mean Mean of the distribution. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> wald(T mean, U scale,
                                                       size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  wald(T mean, U scale, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Weibull distribution.
   *
   * @details The probability density function for the Weibull distribution is
   * @f[
   *   f(x;a,b) = \frac{a}{b}\left(\frac{x}{b}\right)^{a-1}e^{-(x/b)^a}
   * @f]
   * for @f$x \geq 0@f$, where @a a is the shape parameter and @a b is the scale
   * parameter.
   *
   * @param shape Shape parameter. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  typename detail::promote<T, U>::type weibull(T shape, U scale);

  template <class Container1, class T, size_t Rank, class Container2>
  tensor<T, Rank> weibull(const expression<Container1, T, Rank> &shape,
                          const expression<Container2, T, Rank> &scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> weibull(const expression<Container, T, Rank> &shape,
                          typename Container::value_type scale);

  template <class Container, class T, size_t Rank>
  tensor<T, Rank> weibull(typename Container::value_type shape,
                          const expression<Container, T, Rank> &scale);

  /**
   * @brief Draw samples from a Weibull distribution.
   *
   * @param shape Shape parameter. This shall be a positive value.
   * @param scale Scale parameter. This shall be a positive value.
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T, class U>
  tensor<typename detail::promote<T, U>::type, 1> weibull(T shape, U scale,
                                                          size_t size);

  template <class T, class U, size_t Rank>
  tensor<typename detail::promote<T, U>::type, Rank>
  weibull(T shape, U scale, const shape_t<Rank> &size);

  /// Discrete distributions.

  /**
   * @brief Draw samples from a binomial distribution.
   *
   * @details The probability mass function for the binomial distribution is
   * @f[
   *   f(x;n,p) = \binom{n}{x} p^x (1-p)^{n-x}
   * @f]
   * for @f$x = 0, 1, 2, \ldots , n@f$, where @a n is the number of trials and
   * @a p is the probability of success.
   *
   * @tparam T An integer type.
   *
   * @param n Number of trials.
   * @param prob Probability of success. This shall be a value between 0 and 1
   *             (inclusive).
   *
   * @return A sample from the distribution.
   */
  template <class T> T binomial(T n, double prob);

  /**
   * @brief Draw samples from a binomial distribution.
   *
   * @tparam T An integer type.
   * @tparam Rank Dimension of the tensor.
   *
   * @param n Number of trials.
   * @param prob Probability of success. This shall be a value between 0 and 1
   *             (inclusive).
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T> tensor<T, 1> binomial(T n, double prob, size_t size);
  template <class T, size_t Rank>
  tensor<T, Rank> binomial(T n, double prob, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a geometric distribution.
   *
   * @details The probability mass function for the geometric distribution is
   * @f[
   *   f(x;p) = p(1-p)^x
   * @f]
   * for @f$x = 0, 1, 2, ...@f$, where @a p is the probability of success.
   *
   * @tparam T An integer type.
   *
   * @param prob Probability of success. This shall be a value between 0 and 1
   *             (inclusive).
   *
   * @return A sample from the distribution.
   */
  template <class T> T geometric(double prob);

  /**
   * @brief Draw samples from a geometric distribution.
   *
   * @tparam T An integer type.
   * @tparam Rank Dimension of the tensor.
   *
   * @param prob Probability of success. This shall be a value between 0 and 1
   *             (inclusive).
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T> tensor<T, 1> geometric(double prob, size_t size);
  template <class T, size_t Rank>
  tensor<T, Rank> geometric(double prob, const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a negative binomial distribution.
   *
   * @details The probability mass function for the negative binomial
   * distribution is
   * @f[
   *   f(x;n,p) = \binom{n+x-1}{x} p^{n} (1-p)^{x}
   * @f]
   * for @f$x = 0, 1, 2, ...@f$, where @a n is the number of successes before
   * the experiment is stopped and @a p is the probability of success.
   *
   * @tparam T An integer type.
   *
   * @param n Number of successes.
   * @param prob Probability of success. This shall be a value between 0 and 1
   *             (inclusive).
   *
   * @return A sample from the distribution.
   */
  template <class T> T negative_binomial(T n, double prob);

  /**
   * @brief Draw samples from a negative binomial distribution.
   *
   * @tparam T An integer type.
   * @tparam Rank Dimension of the tensor.
   *
   * @param n Number of successes.
   * @param prob Probability of success. This shall be a value between 0 and 1
   *             (inclusive).
   * @param size Output shape.
   *
   * @return A tensor with samples from the distribution.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T>
  tensor<T, 1> negative_binomial(T n, double prob, size_t size);
  template <class T, size_t Rank>
  tensor<T, Rank> negative_binomial(T n, double prob,
                                    const shape_t<Rank> &size);

  /**
   * @brief Draw samples from a Poisson distribution.
   *
   * @details The probability mass function for the Poisson distribution is
   * @f[
   *   f(x;\lambda) = \frac{\lambda^{x}}{x!} e^{-\lambda}
   * @f]
   * for @f$x = 0, 1, 2, ...@f$, where @f$\lambda@f$ is the rate parameter.
   *
   * @tparam T An integer type.
   *
   * @param rate Rate parameter. This shall be a positive value.
   *
   * @return A sample from the distribution.
   */
  template <class T> T poisson(double rate);

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
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class T> tensor<T, 1> poisson(double rate, size_t size);
  template <class T, size_t Rank>
  tensor<T, Rank> poisson(double rate, const shape_t<Rank> &size);

private:
  /**
   * @brief Sample values from a distribution.
   */
  template <class OutputIterator, class Distribution>
  void __sample_distribution(OutputIterator first, size_t n, Distribution &rvs);

  /**
   * @brief Sample element-wise values from a distribution with parameters.
   */
  template <class R, template <class> class Distribution, class Container,
            class T, size_t Rank>
  tensor<R, Rank>
  __sample_element_wise(Distribution<R> &rvs,
                        const expression<Container, T, Rank> &param);

  template <class R, template <class> class Distribution, class Container1,
            class T, class Container2, class U, size_t Rank>
  tensor<R, Rank>
  __sample_element_wise(Distribution<R> &rvs,
                        const expression<Container1, T, Rank> &param1,
                        const expression<Container2, U, Rank> &param2);

  template <class R, template <class> class Distribution, class Container,
            class T, class U, size_t Rank, detail::RequiresScalar<U> = 0>
  tensor<R, Rank>
  __sample_element_wise(Distribution<R> &rvs,
                        const expression<Container, T, Rank> &param1,
                        const U &param2);

  template <class R, template <class> class Distribution, class T,
            class Container, class U, size_t Rank,
            detail::RequiresScalar<T> = 0>
  tensor<R, Rank>
  __sample_element_wise(Distribution<R> &rvs, const T &param1,
                        const expression<Container, U, Rank> &param2);

  /**
   * @brief Sample @a n elements (with replacement) from the sequence
   * @a [first,last).
   */
  template <class RandomAccessIterator, class OutputIterator>
  void __sample_replacement(RandomAccessIterator first,
                            RandomAccessIterator last, OutputIterator out,
                            size_t n);

  template <class RandomAccessIterator1, class RandomAccessIterator2,
            class OutputIterator>
  void __sample_replacement(RandomAccessIterator1 first,
                            RandomAccessIterator1 last,
                            RandomAccessIterator2 weights, OutputIterator out,
                            size_t n);

  /**
   * @brief Sample @a n elements (without replacement) from the sequence
   * @a [first,last).
   */
  template <class InputIterator, class RandomAccessIterator>
  void __sample_no_replacement(InputIterator first, InputIterator last,
                               RandomAccessIterator out, size_t n);

  template <class RandomAccessIterator1, class RandomAccessIterator2,
            class OutputIterator>
  void __sample_no_replacement(RandomAccessIterator1 first,
                               RandomAccessIterator1 last,
                               RandomAccessIterator2 weights,
                               OutputIterator out, size_t n);

private:
  // Underlying uniform random number generator.
  bit_generator m_rng;
};

using default_rng = Generator<std::default_random_engine>;
} // namespace numcpp

#include "numcpp/random/random.tcc"

#endif // NUMCPP_RANDOM_H_INCLUDED
