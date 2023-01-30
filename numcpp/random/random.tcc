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
template <class Sseq>
Generator<bit_generator>::Generator(Sseq &&s) : m_rng(std::forward<Sseq>(s)) {}

/// Seeding.

template <class bit_generator>
template <class Sseq>
void Generator<bit_generator>::seed(Sseq &&s) {
  m_rng.seed(std::forward<Sseq>(s));
}

/// Sample random data.

template <class bit_generator>
template <class OutputIterator, class Distribution>
inline void
Generator<bit_generator>::__sample_distribution(OutputIterator first, size_t n,
                                                Distribution &rvs) {
  for (size_t i = 0; i < n; ++i) {
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
inline tensor<T, 1> Generator<bit_generator>::integers(T low, T high,
                                                       size_t size) {
  return this->integers<T>(low, high, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::integers(T low, T high, const shape_t<Rank> &size) {
  uniform_int_distribution<T> rvs(low, high);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, Rank>
Generator<bit_generator>::random(const shape_t<Rank> &size) {
  uniform_real_distribution<T> rvs;
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class Tag>
typename base_tensor<T, 1, Tag>::value_type
Generator<bit_generator>::choice(const base_tensor<T, 1, Tag> &population) {
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  uniform_int_distribution<size_t> rvs(0, population.size() - 1);
  return population[rvs(m_rng)];
}

template <class bit_generator>
template <class T, class Tag, class W, class TagW>
typename base_tensor<T, 1, Tag>::value_type
Generator<bit_generator>::choice(const base_tensor<T, 1, Tag> &population,
                                 const base_tensor<W, 1, TagW> &weights) {
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  if (population.size() != weights.size()) {
    throw std::invalid_argument(
        "population and weights must have the same size");
  }
  discrete_distribution<size_t> rvs(weights.begin(), weights.end());
  return population[rvs(m_rng)];
}

template <class bit_generator>
template <class RandomAccessIterator, class OutputIterator>
void Generator<bit_generator>::__sample_replacement(RandomAccessIterator first,
                                                    RandomAccessIterator last,
                                                    OutputIterator out,
                                                    size_t n) {
  size_t size = std::distance(first, last);
  uniform_int_distribution<size_t> rvs(0, size - 1);
  for (size_t i = 0; i < n; ++i) {
    *out = first[rvs(m_rng)];
    ++out;
  }
}

template <class bit_generator>
template <class InputIterator, class RandomAccessIterator>
void Generator<bit_generator>::__sample_no_replacement(InputIterator first,
                                                       InputIterator last,
                                                       RandomAccessIterator out,
                                                       size_t n) {
  // Reservoir sampling algorithm.
  typedef uniform_int_distribution<size_t>::param_type param_type;
  uniform_int_distribution<size_t> rvs;
  size_t size = 0;
  while (size < n) {
    out[size++] = *first;
    ++first;
  }
  while (first != last) {
    size_t i = rvs(m_rng, param_type(0, size));
    if (i < n) {
      out[i] = *first;
    }
    ++first;
    ++size;
  }
}

template <class bit_generator>
template <class T, class Tag>
inline tensor<typename base_tensor<T, 1, Tag>::value_type, 1>
Generator<bit_generator>::choice(const base_tensor<T, 1, Tag> &population,
                                 size_t size, bool replace, bool shuffle) {
  return this->choice(population, make_shape(size), replace, shuffle);
}

template <class bit_generator>
template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, 1, Tag>::value_type, Rank>
Generator<bit_generator>::choice(const base_tensor<T, 1, Tag> &population,
                                 const shape_t<Rank> &size, bool replace,
                                 bool shuffle) {
  typedef typename base_tensor<T, 1, Tag>::value_type Rt;
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  if (!replace && size.prod() > population.size()) {
    throw std::invalid_argument(
        "cannot take a larger sample than population when replace=false");
  }
  tensor<Rt, Rank> out(size);
  if (replace) {
    __sample_replacement(population.begin(), population.end(), out.data(),
                         out.size());
  } else {
    __sample_no_replacement(population.begin(), population.end(), out.data(),
                            out.size());
    if (shuffle) {
      std::shuffle(out.data(), out.data() + out.size(), m_rng);
    }
  }
  return out;
}

template <class bit_generator>
template <class RandomAccessIterator1, class RandomAccessIterator2,
          class OutputIterator>
void Generator<bit_generator>::__sample_replacement(
    RandomAccessIterator1 first, RandomAccessIterator1 last,
    RandomAccessIterator2 weights, OutputIterator out, size_t n) {
  size_t size = std::distance(first, last);
  discrete_distribution<size_t> rvs(weights, weights + size);
  for (size_t i = 0; i < n; ++i) {
    *out = first[rvs(m_rng)];
    ++out;
  }
}

template <class bit_generator>
template <class RandomAccessIterator1, class RandomAccessIterator2,
          class OutputIterator>
void Generator<bit_generator>::__sample_no_replacement(
    RandomAccessIterator1 first, RandomAccessIterator1 last,
    RandomAccessIterator2 weights, OutputIterator out, size_t n) {
  typedef discrete_distribution<size_t>::param_type param_type;
  discrete_distribution<size_t> rvs;
  size_t size = std::distance(first, last);
  double *w = new double[size];
  std::copy(weights, weights + size, w);
  for (size_t i = 0; i < n; ++i) {
    size_t k = rvs(m_rng, param_type(w, w + size));
    *out = first[k];
    w[k] = 0;
    ++out;
  }
  delete[] w;
}

template <class bit_generator>
template <class T, class Tag, class W, class TagW>
inline tensor<typename base_tensor<T, 1, Tag>::value_type, 1>
Generator<bit_generator>::choice(const base_tensor<T, 1, Tag> &population,
                                 size_t size,
                                 const base_tensor<W, 1, TagW> &weights,
                                 bool replace) {
  return this->choice(population, make_shape(size), weights, replace);
}

template <class bit_generator>
template <class T, size_t Rank, class Tag, class W, class TagW>
tensor<typename base_tensor<T, 1, Tag>::value_type, Rank>
Generator<bit_generator>::choice(const base_tensor<T, 1, Tag> &population,
                                 const shape_t<Rank> &size,
                                 const base_tensor<W, 1, TagW> &weights,
                                 bool replace) {
  typedef typename base_tensor<T, 1, Tag>::value_type Rt;
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  if (population.shape() != weights.shape()) {
    throw std::invalid_argument(
        "population and weights must have the same size");
  }
  if (!replace && size.prod() > population.size()) {
    throw std::invalid_argument(
        "cannot take a larger sample than population when replace=false");
  }
  tensor<Rt, Rank> out(size);
  if (replace) {
    __sample_replacement(population.begin(), population.end(), weights.begin(),
                         out.data(), out.size());
  } else {
    __sample_no_replacement(population.begin(), population.end(),
                            weights.begin(), out.data(), out.size());
  }
  return out;
}

/// Permutations.

template <class bit_generator>
template <class T, size_t Rank, class Tag>
void Generator<bit_generator>::shuffle(base_tensor<T, Rank, Tag> &arg,
                                       size_t axis) {
  shape_t<Rank> shape = arg.shape();
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    auto first = make_axes_iterator(&arg, index, axis, 0);
    auto last = make_axes_iterator(&arg, index, axis, size);
    std::shuffle(first, last, m_rng);
  }
}

template <class bit_generator>
template <class T>
inline tensor<T, 1> Generator<bit_generator>::permutation(T n) {
  size_t size = (n > 0) ? n : 0;
  tensor<T, 1> out(size);
  std::iota(out.begin(), out.end(), T(0));
  std::shuffle(out.begin(), out.end(), m_rng);
  return out;
}

template <class bit_generator>
template <class T, size_t Rank, class Tag>
inline tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
Generator<bit_generator>::permutation(const base_tensor<T, Rank, Tag> &arg) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, 1> out(arg.begin(), arg.size());
  std::shuffle(out.begin(), out.end(), m_rng);
  return out;
}

template <class bit_generator>
template <class T, size_t Rank, class Tag>
inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
Generator<bit_generator>::permutation(const base_tensor<T, Rank, Tag> &arg,
                                      size_t axis) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out(arg);
  this->shuffle(out, axis);
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
inline tensor<T, 1> Generator<bit_generator>::beta(T shape1, T shape2,
                                                   size_t size) {
  return this->beta<T>(shape1, shape2, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::beta(T shape1, T shape2, const shape_t<Rank> &size) {
  beta_distribution<T> rvs(shape1, shape2);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T>
inline T Generator<bit_generator>::binomial(T n, double prob) {
  binomial_distribution<T> rvs(n, prob);
  return rvs(m_rng);
}

template <class bit_generator>
template <class T>
inline tensor<T, 1> Generator<bit_generator>::binomial(T n, double prob,
                                                       size_t size) {
  return this->binomial<T>(n, prob, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::binomial(T n, double prob,
                                   const shape_t<Rank> &size) {
  binomial_distribution<T> rvs(n, prob);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::cauchy(T loc, T scale,
                                                     size_t size) {
  return this->cauchy<T>(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::cauchy(T loc, T scale, const shape_t<Rank> &size) {
  cauchy_distribution<T> rvs(loc, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, Rank>
Generator<bit_generator>::chisquare(T df, const shape_t<Rank> &size) {
  chi_squared_distribution<T> rvs(df);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::exponential(T rate, size_t size) {
  return this->exponential<T>(rate, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::exponential(T rate, const shape_t<Rank> &size) {
  exponential_distribution<T> rvs(rate);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::fisher_f(T df1, T df2,
                                                       size_t size) {
  return this->fisher_f<T>(df1, df2, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::fisher_f(T df1, T df2, const shape_t<Rank> &size) {
  fisher_f_distribution<T> rvs(df1, df2);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::gamma(T shape, T scale,
                                                    size_t size) {
  return this->gamma<T>(shape, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::gamma(T shape, T scale, const shape_t<Rank> &size) {
  gamma_distribution<T> rvs(shape, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::geometric(double prob,
                                                        size_t size) {
  return this->geometric<T>(prob, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::geometric(double prob, const shape_t<Rank> &size) {
  geometric_distribution<T> rvs(prob);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::gumbel(T loc, T scale,
                                                     size_t size) {
  return this->gumbel<T>(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::gumbel(T loc, T scale, const shape_t<Rank> &size) {
  extreme_value_distribution<T> rvs(loc, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::laplace(T loc, T scale,
                                                      size_t size) {
  return this->laplace<T>(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::laplace(T loc, T scale, const shape_t<Rank> &size) {
  laplace_distribution<T> rvs(loc, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::logistic(T loc, T scale,
                                                       size_t size) {
  return this->logistic<T>(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::logistic(T loc, T scale, const shape_t<Rank> &size) {
  logistic_distribution<T> rvs(loc, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::lognormal(T logmean, T logscale,
                                                        size_t size) {
  return this->lognormal<T>(logmean, logscale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::lognormal(T logmean, T logscale,
                                    const shape_t<Rank> &size) {
  lognormal_distribution<T> rvs(logmean, logscale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T>
inline T Generator<bit_generator>::negative_binomial(T n, double prob) {
  negative_binomial_distribution<T> rvs(n, prob);
  return rvs(m_rng);
}

template <class bit_generator>
template <class T>
inline tensor<T, 1>
Generator<bit_generator>::negative_binomial(T n, double prob, size_t size) {
  return this->negative_binomial<T>(n, prob, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::negative_binomial(T n, double prob,
                                            const shape_t<Rank> &size) {
  negative_binomial_distribution<T> rvs(n, prob);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::normal(T mean, T stddev,
                                                     size_t size) {
  return this->normal<T>(mean, stddev, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::normal(T mean, T stddev, const shape_t<Rank> &size) {
  normal_distribution<T> rvs(mean, stddev);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::pareto(T shape, T scale,
                                                     size_t size) {
  return this->pareto<T>(shape, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::pareto(T shape, T scale, const shape_t<Rank> &size) {
  pareto_distribution<T> rvs(shape, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::poisson(double rate,
                                                      size_t size) {
  return this->poisson<T>(rate, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::poisson(double rate, const shape_t<Rank> &size) {
  poisson_distribution<T> rvs(rate);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::rayleigh(T scale, size_t size) {
  return this->rayleigh<T>(scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::rayleigh(T scale, const shape_t<Rank> &size) {
  rayleigh_distribution<T> rvs(scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T>
inline T Generator<bit_generator>::standard_normal() {
  normal_distribution<T> rvs;
  return rvs(m_rng);
}

template <class bit_generator>
template <class T>
inline tensor<T, 1> Generator<bit_generator>::standard_normal(size_t size) {
  return this->standard_normal<T>(make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::standard_normal(const shape_t<Rank> &size) {
  normal_distribution<T> rvs;
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::student_t(T df, size_t size) {
  return this->student_t<T>(df, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::student_t(T df, const shape_t<Rank> &size) {
  student_t_distribution<T> rvs(df);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::triangular(T lower, T mode,
                                                         T right, size_t size) {
  return this->triangular<T>(lower, mode, right, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::triangular(T lower, T mode, T right,
                                     const shape_t<Rank> &size) {
  T breaks[3] = {lower, mode, right};
  T weights[3] = {T(0), T(1), T(0)};
  piecewise_linear_distribution<T> rvs(breaks, breaks + 3, weights);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::uniform(T low, T high,
                                                      size_t size) {
  return this->uniform<T>(low, high, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::uniform(T low, T high, const shape_t<Rank> &size) {
  uniform_real_distribution<T> rvs(low, high);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::wald(T mean, T scale,
                                                   size_t size) {
  return this->wald<T>(mean, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::wald(T mean, T scale, const shape_t<Rank> &size) {
  inverse_gaussian_distribution<T> rvs(mean, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
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
inline tensor<T, 1> Generator<bit_generator>::weibull(T shape, T scale,
                                                      size_t size) {
  return this->weibull<T>(shape, scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::weibull(T shape, T scale, const shape_t<Rank> &size) {
  weibull_distribution<T> rvs(shape, scale);
  tensor<T, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}
} // namespace numcpp

#endif // NUMCPP_RANDOM_TCC_INCLUDED
