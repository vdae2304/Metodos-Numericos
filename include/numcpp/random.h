/*
 * File: include/numcpp/random.h
 * Repository: https://github.com/vdae2304/Metodos-Numericos
 * 
 * Copyright (C) 2026 vdae2304
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NUMCPP_RANDOM_H_INCLUDED
#define NUMCPP_RANDOM_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"
#include "numcpp/expressions/random_gen.h"
#include "numcpp/math/distributions.h"

namespace numcpp {
namespace random {
/**
 * @brief Sample values from a given distribution.
 * 
 * @param rvs Random distribution.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor with a random sample. This function does not
 * create a new tensor, instead, a generator expression is returned. Each
 * evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <class Distribution, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline random_generator<Distribution, Generator, Rank> sample(
    Distribution rvs, const shape_t<Rank>& shape, Generator& urng) {
  return random_generator<Distribution, Generator, Rank>(rvs, shape, urng);
}

template <class Distribution, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline random_generator<Distribution, Generator, Rank> sample(
    Distribution rvs, const size_t (&shape)[Rank], Generator& urng) {
  return random_generator<Distribution, Generator, Rank>(rvs, make_shape(shape),
                                                         urng);
}

/**
 * @brief Return a tensor of random integers from @a a to @a b (inclusive).
 *
 * @param a Lowest integer to be drawn.
 * @param b Largest integer to be drawn.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random integers. This function does not create
 * a new tensor, instead, a generator expression is returned. Each evaluation
 * returns a different outcome and thus shall only be used to initialize a
 * dense tensor.
 */
template <std::integral T, std::integral U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto integers(T a, U b, const shape_t<Rank>& shape, Generator& urng) {
  typedef std::common_type_t<T, U> Rt;
  std::uniform_int_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

template <std::integral T, std::integral U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto integers(T a, U b, const size_t (&shape)[Rank], Generator& urng) {
  typedef std::common_type_t<T, U> Rt;
  std::uniform_int_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

/**
 * @brief Return a tensor of random floating-point numbers in the half-open
 * interval [0, 1).
 *
 * @param shape Output shape.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <std::floating_point T = double, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto random(const shape_t<Rank>& shape, Generator& urng) {
  std::uniform_real_distribution<T> rvs;
  return sample(rvs, shape, urng);
}

template <std::floating_point T = double, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto random(const size_t (&shape)[Rank], Generator& urng) {
  std::uniform_real_distribution<T> rvs;
  return sample(rvs, shape, urng);
}

/// Continuous distributions.

/**
 * @brief Draw samples from a Beta distribution.
 *
 * @details The probability density function for the Beta distribution is
 * @f[
 *   f(x;\alpha,\beta) = \frac{\Gamma(\alpha+\beta)}
 *     {\Gamma(\alpha)\Gamma(\beta)} x^{\alpha-1} (1-x)^{\beta-1}
 * @f]
 * for 0 <= @a x <= 1, where @a alpha and @a beta are shape parameters.
 *
 * @param alpha Shape parameter. This shall be a positive value.
 * @param beta Shape parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto beta(T alpha, U beta, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  beta_distribution<Rt> rvs(alpha, beta);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto beta(T alpha, U beta, const size_t (&shape)[Rank],
                 Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  beta_distribution<Rt> rvs(alpha, beta);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Cauchy distribution.
 *
 * @details The probability density function for the Cauchy distribution is
 * @f[
 *   f(x;a,b) = \frac{1}
 *     {b\pi\left[1+\left(\frac{x-a}{b}\right)^2\right]}
 * @f]
 * for all @a x, where @a a and @a b are location and scale parameters.
 *
 * @param a Location parameter.
 * @param b Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto cauchy(T a, U b, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::cauchy_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto cauchy(T a, U b, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::cauchy_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a chi-squared distribution.
 *
 * @details The probability density function for the chi-squared distribution is
 * @f[
 *   f(x;n) = \frac{1}{2^{n/2}\Gamma(n/2)} x^{n/2-1} e^{-x/2}
 * @f]
 * for @a x >= 0, where @a n is the degrees of freedom.
 *
 * @param n Degrees of freedom. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto chisquare(T n, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<T> Rt;
  std::chi_squared_distribution<Rt> rvs(n);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto chisquare(T n, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<T> Rt;
  std::chi_squared_distribution<Rt> rvs(n);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from an exponential distribution.
 *
 * @details The probability density function for the exponential distribution is
 * @f[
 *   f(x;\lambda) = \lambda e^{-\lambda x}
 * @f]
 * for @a x >= 0, where @a lambda is the rate parameter.
 *
 * @param lambda Rate parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto exponential(T lambda, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<T> Rt;
  std::exponential_distribution<Rt> rvs(lambda);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto exponential(T lambda, const size_t (&shape)[Rank],
                        Generator& urng) {
  typedef detail::promote_t<T> Rt;
  std::exponential_distribution<Rt> rvs(lambda);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Fisher F-distribution.
 *
 * @details The probability density function for the F distribution is
 * @f[
 *   f(x;m,n) = \frac{\Gamma\left(\frac{m+n}{2}\right)}
 *     {\Gamma\left(\frac{m}{2}\right)\Gamma\left(\frac{n}{2}\right)}
 *     \frac{\left(\frac{m}{n}\right)^{m/2}}
 *     {x\left(1+\frac{mx}{n}\right)^{(m+n)/2}}
 * @f]
 * for @a x >= 0, where @a m and @a n are the degrees of freedom.
 *
 * @param m Degrees of freedom. This shall be a positive value.
 * @param n Degrees of freedom. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto fisher_f(T m, U n, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::fisher_f_distribution<Rt> rvs(m, n);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto fisher_f(T m, U n, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::fisher_f_distribution<Rt> rvs(m, n);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Gamma distribution.
 *
 * @details The probability density function for the Gamma distribution is
 * @f[
 *   f(x;\alpha,\beta) = \frac{1}{\Gamma(\alpha)\beta^{\alpha}} x^{\alpha-1}
 *     e^{-x/\beta}
 * @f]
 * for @a x > 0, where @a alpha is the shape parameter and @a beta is the scale
 * parameter.
 *
 * @param alpha Shape parameter. This shall be a positive value.
 * @param beta Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto gamma(T alpha, U beta, const shape_t<Rank>& shape,
                  Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::gamma_distribution<Rt> rvs(alpha, beta);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto gamma(T alpha, U beta, const size_t (&shape)[Rank],
                  Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::gamma_distribution<Rt> rvs(alpha, beta);
  return sample(rvs, shape, urng);
}

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
 * @param a Location parameter.
 * @param b Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto gumbel(T a, U b, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::extreme_value_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto gumbel(T a, U b, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::extreme_value_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Laplace distribution.
 *
 * @details The probability density function for the Laplace distribution is
 * @f[
 *   f(x;\mu,s) = \frac{1}{2s}\exp\left(-\frac{|x-\mu|}{s}\right)
 * @f]
 * for all @a x, where @a mu is the location parameter and @a s is the scale
 * parameter.
 *
 * @param mu Location parameter.
 * @param s Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto laplace(T mu, U s, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  laplace_distribution<Rt> rvs(mu, s);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto laplace(T mu, U s, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  laplace_distribution<Rt> rvs(mu, s);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a logistic distribution.
 *
 * @details The probability density function for the logistic distribution is
 * @f[
 *   f(x;\mu,s) = \frac{e^{-(x-\mu)/2}}{s\left(1+e^{-(x-\mu)/s}\right)^2}
 * @f]
 * for all @a x, where @a mu is the location parameter and @a s is the scale
 * parameter.
 *
 * @param mu Location parameter.
 * @param s Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto logistic(T mu, U s, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  logistic_distribution<Rt> rvs(mu, s);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto logistic(T mu, U s, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  logistic_distribution<Rt> rvs(mu, s);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a log-normal distribution.
 *
 * @details The probability density function for the log-normal distribution is
 * @f[
 *   f(x;m,s) = \frac{1}{xs\sqrt{2\pi}}
 *     \exp\left(-\frac{(\log x-m)^2}{2s^2}\right)
 * @f]
 * for @a x > 0, where @a m and @a s are the mean and standard deviation of the
 * underlying normal distribution formed by the logarithm transformation.
 *
 * @param m Mean of the underlying normal distribution.
 * @param s Standard deviation of the underlying normal distribution. This
 * shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto lognormal(T m, U s, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::lognormal_distribution<Rt> rvs(m, s);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto lognormal(T m, U s, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::lognormal_distribution<Rt> rvs(m, s);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a normal distribution.
 *
 * @details The probability density function for the normal distribution is
 * @f[
 *   f(x;\mu,\sigma) = \frac{1}{\sigma\sqrt{2\pi}}
 *     \exp\left(-\frac{(x-\mu)^2}{2\sigma^2}\right)
 * @f]
 * for all @a x, where @a mu and @a sigma are the mean and standard deviation.
 *
 * @param mean Mean of the distribution.
 * @param stddev Standard deviation of the distribution. This shall be a
 * positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto normal(T mean, U stddev, const shape_t<Rank>& shape,
                   Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::normal_distribution<Rt> rvs(mean, stddev);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto normal(T mean, U stddev, const size_t (&shape)[Rank],
                   Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::normal_distribution<Rt> rvs(mean, stddev);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Pareto distribution.
 *
 * @details The probability density function for the Pareto distribution is
 * @f[
 *   f(x;\alpha,x_m) = \frac{\alpha x_m^{\alpha}}{x^{\alpha+1}}
 * @f]
 * for @a x >= x_m, where @a alpha is the shape parameter and @a x_m is the
 * scale parameter.
 *
 * @param alpha Shape parameter. This shall be a positive value.
 * @param xm Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto pareto(T alpha, U xm, const shape_t<Rank>& shape,
                   Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  pareto_distribution<Rt> rvs(alpha, xm);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto pareto(T alpha, U xm, const size_t (&shape)[Rank],
                   Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  pareto_distribution<Rt> rvs(alpha, xm);
  return sample(rvs, shape, urng);
}

 /**
 * @brief Draw samples from a Rayleigh distribution.
 *
 * @details The probability density function for the Rayleigh distribution is
 * @f[
 *   f(x;\sigma) = \frac{x}{\sigma^2}e^{-x^2/(2\sigma^2)}
 * @f]
 * for @a x >= 0, where @a sigma is the scale parameter.
 *
 * @param sigma Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 *
 */
template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto rayleigh(T sigma, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<T> Rt;
  rayleigh_distribution<Rt> rvs(sigma);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto rayleigh(T sigma, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<T> Rt;
  rayleigh_distribution<Rt> rvs(sigma);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a standard normal distribution ( @a mean=0,
 * @a stddev=1 ).
 * 
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <std::floating_point T = double, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto standard_normal(const shape_t<Rank>& shape, Generator& urng) {
  std::normal_distribution<T> rvs;
  return sample(rvs, shape, urng);
}

template <std::floating_point T = double, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto standard_normal(const size_t (&shape)[Rank], Generator& urng) {
  std::normal_distribution<T> rvs;
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Student's t distribution.
 *
 * @details The probability density function for the Student's t distribution is
 * @f[
 *   f(x;n) = \frac{1}{\sqrt{n\pi}}
 *     \frac{\Gamma\left(\frac{n+1}{2}\right)}
 *     {\Gamma\left(\frac{n}{2}\right)}
 *     \left(1+\frac{x^2}{n}\right)^{-(n+1)/2}
 * @f]
 * for all @a x, where @a n is the degrees of freedom.
 *
 * @param n Degrees of freedom. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto student_t(T n, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<T> Rt;
  std::student_t_distribution<Rt> rvs(n);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto student_t(T n, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<T> Rt;
  std::student_t_distribution<Rt> rvs(n);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from an uniform distribution.
 *
 * @details The probability density function for the uniform distribution is
 * @f[
 *   f(x;a,b) = \frac{1}{b-a}
 * @f]
 * for a <= @a x < b, where @a a and @a b are the lower and upper boundaries of
 * the distribution.
 *
 * @param a Lower boundary.
 * @param b Upper boundary.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto uniform(T a, U b, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::uniform_real_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto uniform(T a, U b, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::uniform_real_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Wald, or inverse Gaussian, distribution.
 *
 * @details The probability density function for the Wald distribution is
 * @f[
 *   f(x;\mu,\lambda) = \sqrt{\frac{\lambda}{2\pi x^3}}
 *     \exp\left(-\frac{\lambda(x-\mu)^2}{2\mu^2 x}\right)
 * @f]
 * for @a x > 0, where @a mu is the mean and @a lambda is the scale parameter.
 *
 * @param mu Mean of the distribution. This shall be a positive value.
 * @param lambda Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto wald(T mu, U lambda, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  inverse_gaussian_distribution<Rt> rvs(mu, lambda);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto wald(T mu, U lambda, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  inverse_gaussian_distribution<Rt> rvs(mu, lambda);
  return sample(rvs, shape, urng);
}

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
 * @param a Shape parameter. This shall be a positive value.
 * @param b Scale parameter. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random floating-point numbers. This function
 * does not create a new tensor, instead, a generator expression is returned.
 * Each evaluation returns a different outcome and thus shall only be used to
 * initialize a dense tensor.
 */
template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto weibull(T a, U b, const shape_t<Rank>& shape, Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::weibull_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

template <detail::arithmetic T, detail::arithmetic U, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto weibull(T a, U b, const size_t (&shape)[Rank], Generator& urng) {
  typedef detail::promote_t<std::common_type_t<T, U>> Rt;
  std::weibull_distribution<Rt> rvs(a, b);
  return sample(rvs, shape, urng);
}

/// Discrete distributions.

/**
 * @brief Draw samples from a Bernoulli distribution.
 *
 * @details The probability mass function for the Bernoulli distribution is
 * @f[
 *   f(x;p) = \begin{cases}
 *     1-p, & x=0,\\
 *     p, & x=1,
 *   \end{cases}
 * @f]
 * where @a p is the probability of success.
 *
 * @param p Probability of success. This shall be a value between 0 and 1
 * (inclusive).
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random integers. This function does not create
 * a new tensor, instead, a generator expression is returned. Each evaluation
 * returns a different outcome and thus shall only be used to initialize a
 * dense tensor.
 */
template <size_t Rank, std::uniform_random_bit_generator Generator>
inline auto bernoulli(double p, const shape_t<Rank>& shape, Generator& urng) {
  std::bernoulli_distribution rvs(p);
  return sample(rvs, shape, urng);
}

template <size_t Rank, std::uniform_random_bit_generator Generator>
inline auto bernoulli(double p, const size_t (&shape)[Rank], Generator& urng) {
  std::bernoulli_distribution rvs(p);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a binomial distribution.
 *
 * @details The probability mass function for the binomial distribution is
 * @f[
 *   f(x;t,p) = \binom{t}{x} p^x (1-p)^{t-x}
 * @f]
 * for @a x = 0, 1, 2, ... , t, where @a t is the number of trials and @a p is
 * the probability of success.
 *
 * @param t Number of trials.
 * @param p Probability of success. This shall be a value between 0 and 1
 * (inclusive).
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random integers. This function does not create
 * a new tensor, instead, a generator expression is returned. Each evaluation
 * returns a different outcome and thus shall only be used to initialize a
 * dense tensor.
 */
template <std::integral T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto binomial(T t, double p, const shape_t<Rank>& shape,
                     Generator& urng) {
  std::binomial_distribution<T> rvs(t, p);
  return sample(rvs, shape, urng);
}

template <std::integral T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto binomial(T t, double p, const size_t (&shape)[Rank],
                     Generator& urng) {
  std::binomial_distribution<T> rvs(t, p);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a geometric distribution.
 *
 * @details The probability mass function for the geometric distribution is
 * @f[
 *   f(x;p) = p(1-p)^x
 * @f]
 * for @a x = 0, 1, 2, ..., where @a p is the probability of success.
 *
 * @param p Probability of success. This shall be a value between 0 and 1
 * (inclusive).
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random integers. This function does not create
 * a new tensor, instead, a generator expression is returned. Each evaluation
 * returns a different outcome and thus shall only be used to initialize a
 * dense tensor.
 */
template <std::integral T = int, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto geometric(double p, const shape_t<Rank>& shape, Generator& urng) {
  std::geometric_distribution<T> rvs(p);
  return sample(rvs, shape, urng);
}

template <std::integral T = int, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto geometric(double p, const size_t (&shape)[Rank], Generator& urng) {
  std::geometric_distribution<T> rvs(p);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a negative binomial distribution.
 *
 * @details The probability mass function for the negative binomial
 * distribution is
 * @f[
 *   f(x;k,p) = \binom{k+x-1}{x} p^{k} (1-p)^{x}
 * @f]
 * for @a x = 0, 1, 2, ..., where @a k is the number of successes before the
 * experiment is stopped and @a p is the probability of success.
 *
 * @param k Number of successes.
 * @param p Probability of success. This shall be a value between 0 and 1
 * (inclusive).
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random integers. This function does not create
 * a new tensor, instead, a generator expression is returned. Each evaluation
 * returns a different outcome and thus shall only be used to initialize a
 * dense tensor.
 */
template <std::integral T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto negative_binomial(T k, double p, const shape_t<Rank>& shape,
                              Generator& urng) {
  std::negative_binomial_distribution<T> rvs(k, p);
  return sample(rvs, shape, urng);
}

template <std::integral T, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto negative_binomial(T k, double p, const size_t (&shape)[Rank],
                              Generator& urng) {
  std::negative_binomial_distribution<T> rvs(k, p);
  return sample(rvs, shape, urng);
}

/**
 * @brief Draw samples from a Poisson distribution.
 *
 * @details The probability mass function for the Poisson distribution is
 * @f[
 *   f(x;\lambda) = \frac{\lambda^{x}}{x!} e^{-\lambda}
 * @f]
 * for @a x = 0, 1, 2, ..., where @a lambda is the mean.
 *
 * @param mean Mean of the distribution. This shall be a positive value.
 * @param shape Output shape.
 * @param urng Uniform random bit generator.
 *
 * @return An abstract tensor of random integers. This function does not create
 * a new tensor, instead, a generator expression is returned. Each evaluation
 * returns a different outcome and thus shall only be used to initialize a
 * dense tensor.
 */
template <std::integral T = int, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto poisson(double mean, const shape_t<Rank>& shape, Generator& urng) {
  std::poisson_distribution<T> rvs(mean);
  return sample(rvs, shape, urng);
}

template <std::integral T = int, size_t Rank,
          std::uniform_random_bit_generator Generator>
inline auto poisson(double mean, const size_t (&shape)[Rank], Generator& urng) {
  std::poisson_distribution<T> rvs(mean);
  return sample(rvs, shape, urng);
}
} // namspace random
} // namespace numcpp

#endif // NUMCPP_RANDOM_H_INCLUDED
