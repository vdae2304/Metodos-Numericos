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
#include "numcpp/iterators/axes_iterator.h"

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
template <class Distribution, class Container, class T, size_t Rank>
tensor<typename Distribution::result_type, Rank>
Generator<bit_generator>::__sample_element_wise(
    Distribution &rvs, const expression<Container, T, Rank> &param) {
  typedef typename Distribution::result_type Rt;
  typedef typename Distribution::param_type param_type;
  tensor<Rt, Rank> out(param.shape());
  for (index_t<Rank> index : make_index_sequence_for(out)) {
    out[index] = rvs(m_rng, param_type(param[index]));
  }
  return out;
}

template <class bit_generator>
template <class Distribution, class Container1, class T, class Container2,
          class U, size_t Rank>
tensor<typename Distribution::result_type, Rank>
Generator<bit_generator>::__sample_element_wise(
    Distribution &rvs, const expression<Container1, T, Rank> &param1,
    const expression<Container2, U, Rank> &param2) {
  typedef typename Distribution::result_type Rt;
  typedef typename Distribution::param_type param_type;
  tensor<Rt, Rank> out(broadcast_shapes(param1.shape(), param2.shape()));
  for (index_t<Rank> index : make_index_sequence_for(out)) {
    index_t<Rank> i, j;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (param1.shape(axis) > 1) ? index[axis] : 0;
      j[axis] = (param2.shape(axis) > 1) ? index[axis] : 0;
    }
    out[index] = rvs(m_rng, param_type(param1[i], param2[j]));
  }
  return out;
}

template <class bit_generator>
template <class Distribution, class Container, class T, class U, size_t Rank,
          detail::RequiresScalar<U>>
tensor<typename Distribution::result_type, Rank>
Generator<bit_generator>::__sample_element_wise(
    Distribution &rvs, const expression<Container, T, Rank> &param1,
    const U &param2) {
  typedef typename Distribution::result_type Rt;
  typedef typename Distribution::param_type param_type;
  tensor<Rt, Rank> out(param1.shape());
  for (index_t<Rank> index : make_index_sequence_for(out)) {
    out[index] = rvs(m_rng, param_type(param1[index], param2));
  }
  return out;
}

template <class bit_generator>
template <class Distribution, class T, class Container, class U, size_t Rank,
          detail::RequiresScalar<T>>
tensor<typename Distribution::result_type, Rank>
Generator<bit_generator>::__sample_element_wise(
    Distribution &rvs, const T &param1,
    const expression<Container, U, Rank> &param2) {
  typedef typename Distribution::result_type Rt;
  typedef typename Distribution::param_type param_type;
  tensor<Rt, Rank> out(param2.shape());
  for (index_t<Rank> index : make_index_sequence_for(out)) {
    out[index] = rvs(m_rng, param_type(param1, param2[index]));
  }
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename std::common_type<T, U>::type
Generator<bit_generator>::integers(T low, U high) {
  typedef typename std::common_type<T, U>::type Rt;
  uniform_int_distribution<Rt> rvs(low, high);
  return rvs(m_rng);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename std::common_type<T, U>::type, 1>
Generator<bit_generator>::integers(T low, U high, size_t size) {
  return this->integers(low, high, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename std::common_type<T, U>::type, Rank>
Generator<bit_generator>::integers(T low, U high, const shape_t<Rank> &size) {
  typedef typename std::common_type<T, U>::type Rt;
  uniform_int_distribution<Rt> rvs(low, high);
  tensor<Rt, Rank> out(size);
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
template <class Container, class T>
T Generator<bit_generator>::choice(
    const expression<Container, T, 1> &population) {
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  uniform_int_distribution<size_t> rvs(0, population.size() - 1);
  return population[rvs(m_rng)];
}

template <class bit_generator>
template <class Container1, class T, class Container2, class U>
T Generator<bit_generator>::choice(
    const expression<Container1, T, 1> &population,
    const expression<Container2, U, 1> &weights) {
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  if (population.size() != weights.size()) {
    throw std::invalid_argument(
        "population and weights must have the same size");
  }
  discrete_distribution<size_t> rvs(weights.self().begin(),
                                    weights.self().end());
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
template <class Container, class T>
inline tensor<T, 1>
Generator<bit_generator>::choice(const expression<Container, T, 1> &population,
                                 size_t size, bool replace, bool shuffle) {
  return this->choice(population, make_shape(size), replace, shuffle);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
tensor<T, Rank>
Generator<bit_generator>::choice(const expression<Container, T, 1> &population,
                                 const shape_t<Rank> &size, bool replace,
                                 bool shuffle) {
  if (population.size() == 0) {
    throw std::invalid_argument("population cannot be empty");
  }
  if (!replace && size.prod() > population.size()) {
    throw std::invalid_argument(
        "cannot take a larger sample than population when replace=false");
  }
  tensor<T, Rank> out(size);
  if (replace) {
    __sample_replacement(population.self().begin(), population.self().end(),
                         out.data(), out.size());
  } else {
    __sample_no_replacement(population.self().begin(), population.self().end(),
                            out.data(), out.size());
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
template <class Container1, class T, class Container2, class U>
inline tensor<T, 1> Generator<bit_generator>::choice(
    const expression<Container1, T, 1> &population, size_t size,
    const expression<Container2, U, 1> &weights, bool replace) {
  return this->choice(population, make_shape(size), weights, replace);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2, class U>
inline tensor<T, Rank> Generator<bit_generator>::choice(
    const expression<Container1, T, 1> &population, const shape_t<Rank> &size,
    const expression<Container2, U, 1> &weights, bool replace) {
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
  tensor<T, Rank> out(size);
  if (replace) {
    __sample_replacement(population.self().begin(), population.self().end(),
                         weights.self().begin(), out.data(), out.size());
  } else {
    __sample_no_replacement(population.self().begin(), population.self().end(),
                            weights.self().begin(), out.data(), out.size());
  }
  return out;
}

/// Permutations.

template <class bit_generator>
template <class Container, class T, size_t Rank>
void Generator<bit_generator>::shuffle(dense_tensor<Container, T, Rank> &a,
                                       size_t axis) {
  shape_t<Rank> shape = a.shape();
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    typedef axes_iterator<Container, T, Rank, 1> iterator;
    iterator first(&a.self(), index, axis, 0);
    iterator last(&a.self(), index, axis, size);
    std::shuffle(first, last, m_rng);
  }
}

template <class bit_generator>
template <class T, detail::RequiresIntegral<T>>
inline tensor<T, 1> Generator<bit_generator>::permutation(T n) {
  size_t size = (n > 0) ? n : 0;
  tensor<T, 1> out(size);
  std::iota(out.data(), out.data() + out.size(), T(0));
  std::shuffle(out.data(), out.data() + out.size(), m_rng);
  return out;
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, 1>
Generator<bit_generator>::permutation(const expression<Container, T, Rank> &a) {
  tensor<T, 1> out(a.self().begin(), a.size());
  std::shuffle(out.data(), out.data() + out.size(), m_rng);
  return out;
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::permutation(const expression<Container, T, Rank> &a,
                                      size_t axis) {
  tensor<T, Rank> out(a);
  this->shuffle(out, axis);
  return out;
}

/// Continuous distributions.

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::beta(T shape1, U shape2) {
  typedef typename detail::promote<T, U>::type Rt;
  beta_distribution<Rt> rvs(shape1, shape2);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::beta(const expression<Container1, T, Rank> &shape1,
                               const expression<Container2, T, Rank> &shape2) {
  beta_distribution<T> rvs;
  return __sample_element_wise(rvs, shape1, shape2);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::beta(const expression<Container, T, Rank> &shape1,
                               typename Container::value_type shape2) {
  beta_distribution<T> rvs;
  return __sample_element_wise(rvs, shape1, shape2);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::beta(typename Container::value_type shape1,
                               const expression<Container, T, Rank> &shape2) {
  beta_distribution<T> rvs;
  return __sample_element_wise(rvs, shape1, shape2);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::beta(T shape1, U shape2, size_t size) {
  return this->beta(shape1, shape2, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::beta(T shape1, U shape2, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  beta_distribution<Rt> rvs(shape1, shape2);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::cauchy(T loc, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  cauchy_distribution<Rt> rvs(loc, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::cauchy(const expression<Container1, T, Rank> &loc,
                                 const expression<Container2, T, Rank> &scale) {
  cauchy_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::cauchy(const expression<Container, T, Rank> &loc,
                                 typename Container::value_type scale) {
  cauchy_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::cauchy(typename Container::value_type loc,
                                 const expression<Container, T, Rank> &scale) {
  cauchy_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::cauchy(T loc, U scale, size_t size) {
  return this->cauchy(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::cauchy(T loc, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  cauchy_distribution<Rt> rvs(loc, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T>
inline typename detail::promote<T>::type
Generator<bit_generator>::chisquare(T df) {
  typedef typename detail::promote<T>::type Rt;
  chi_squared_distribution<Rt> rvs(df);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::chisquare(const expression<Container, T, Rank> &df) {
  chi_squared_distribution<T> rvs;
  return __sample_element_wise(rvs, df);
}

template <class bit_generator>
template <class T>
inline tensor<typename detail::promote<T>::type, 1>
Generator<bit_generator>::chisquare(T df, size_t size) {
  return this->chisquare(df, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<typename detail::promote<T>::type, Rank>
Generator<bit_generator>::chisquare(T df, const shape_t<Rank> &size) {
  typedef typename detail::promote<T>::type Rt;
  chi_squared_distribution<Rt> rvs(df);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T>
inline typename detail::promote<T>::type
Generator<bit_generator>::exponential(T rate) {
  typedef typename detail::promote<T>::type Rt;
  exponential_distribution<Rt> rvs(rate);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::exponential(
    const expression<Container, T, Rank> &rate) {
  exponential_distribution<T> rvs;
  return __sample_element_wise(rvs, rate);
}

template <class bit_generator>
template <class T>
inline tensor<typename detail::promote<T>::type, 1>
Generator<bit_generator>::exponential(T rate, size_t size) {
  return this->exponential(rate, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<typename detail::promote<T>::type, Rank>
Generator<bit_generator>::exponential(T rate, const shape_t<Rank> &size) {
  typedef typename detail::promote<T>::type Rt;
  exponential_distribution<Rt> rvs(rate);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::fisher_f(T df1, U df2) {
  typedef typename detail::promote<T, U>::type Rt;
  fisher_f_distribution<Rt> rvs(df1, df2);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::fisher_f(const expression<Container1, T, Rank> &df1,
                                   const expression<Container2, T, Rank> &df2) {
  fisher_f_distribution<T> rvs;
  return __sample_element_wise(rvs, df1, df2);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::fisher_f(const expression<Container, T, Rank> &df1,
                                   typename Container::value_type df2) {
  fisher_f_distribution<T> rvs;
  return __sample_element_wise(rvs, df1, df2);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::fisher_f(typename Container::value_type df1,
                                   const expression<Container, T, Rank> &df2) {
  fisher_f_distribution<T> rvs;
  return __sample_element_wise(rvs, df1, df2);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::fisher_f(T df1, U df2, size_t size) {
  return this->fisher_f(df1, df2, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::fisher_f(T df1, U df2, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  fisher_f_distribution<Rt> rvs(df1, df2);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::gamma(T shape, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  gamma_distribution<Rt> rvs(shape, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::gamma(const expression<Container1, T, Rank> &shape,
                                const expression<Container2, T, Rank> &scale) {
  gamma_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::gamma(const expression<Container, T, Rank> &shape,
                                typename Container::value_type scale) {
  gamma_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::gamma(typename Container::value_type shape,
                                const expression<Container, T, Rank> &scale) {
  gamma_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::gamma(T shape, U scale, size_t size) {
  return this->gamma(shape, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::gamma(T shape, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  gamma_distribution<Rt> rvs(shape, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::gumbel(T loc, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  extreme_value_distribution<Rt> rvs(loc, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::gumbel(const expression<Container1, T, Rank> &loc,
                                 const expression<Container2, T, Rank> &scale) {
  extreme_value_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::gumbel(const expression<Container, T, Rank> &loc,
                                 typename Container::value_type scale) {
  extreme_value_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::gumbel(typename Container::value_type loc,
                                 const expression<Container, T, Rank> &scale) {
  extreme_value_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::gumbel(T loc, U scale, size_t size) {
  return this->gumbel(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::gumbel(T loc, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  extreme_value_distribution<Rt> rvs(loc, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::laplace(T loc, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  laplace_distribution<Rt> rvs(loc, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::laplace(
    const expression<Container1, T, Rank> &loc,
    const expression<Container2, T, Rank> &scale) {
  laplace_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::laplace(const expression<Container, T, Rank> &loc,
                                  typename Container::value_type scale) {
  laplace_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::laplace(typename Container::value_type loc,
                                  const expression<Container, T, Rank> &scale) {
  laplace_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::laplace(T loc, U scale, size_t size) {
  return this->laplace(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::laplace(T loc, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  laplace_distribution<Rt> rvs(loc, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::logistic(T loc, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  logistic_distribution<Rt> rvs(loc, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::logistic(
    const expression<Container1, T, Rank> &loc,
    const expression<Container2, T, Rank> &scale) {
  logistic_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::logistic(const expression<Container, T, Rank> &loc,
                                   typename Container::value_type scale) {
  logistic_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::logistic(
    typename Container::value_type loc,
    const expression<Container, T, Rank> &scale) {
  logistic_distribution<T> rvs;
  return __sample_element_wise(rvs, loc, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::logistic(T loc, U scale, size_t size) {
  return this->logistic(loc, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::logistic(T loc, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  logistic_distribution<Rt> rvs(loc, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::lognormal(T logmean, U logscale) {
  typedef typename detail::promote<T, U>::type Rt;
  lognormal_distribution<Rt> rvs(logmean, logscale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::lognormal(
    const expression<Container1, T, Rank> &logmean,
    const expression<Container2, T, Rank> &logscale) {
  lognormal_distribution<T> rvs;
  return __sample_element_wise(rvs, logmean, logscale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::lognormal(
    const expression<Container, T, Rank> &logmean,
    typename Container::value_type logscale) {
  lognormal_distribution<T> rvs;
  return __sample_element_wise(rvs, logmean, logscale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::lognormal(
    typename Container::value_type logmean,
    const expression<Container, T, Rank> &logscale) {
  lognormal_distribution<T> rvs;
  return __sample_element_wise(rvs, logmean, logscale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::lognormal(T logmean, U logscale, size_t size) {
  return this->lognormal(logmean, logscale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::lognormal(T logmean, U logscale,
                                    const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  lognormal_distribution<Rt> rvs(logmean, logscale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::normal(T mean, U stddev) {
  typedef typename detail::promote<T, U>::type Rt;
  normal_distribution<Rt> rvs(mean, stddev);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::normal(
    const expression<Container1, T, Rank> &mean,
    const expression<Container2, T, Rank> &stddev) {
  normal_distribution<T> rvs;
  return __sample_element_wise(rvs, mean, stddev);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::normal(const expression<Container, T, Rank> &mean,
                                 typename Container::value_type stddev) {
  normal_distribution<T> rvs;
  return __sample_element_wise(rvs, mean, stddev);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::normal(typename Container::value_type mean,
                                 const expression<Container, T, Rank> &stddev) {
  normal_distribution<T> rvs;
  return __sample_element_wise(rvs, mean, stddev);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::normal(T mean, U stddev, size_t size) {
  return this->normal(mean, stddev, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::normal(T mean, U stddev, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  normal_distribution<Rt> rvs(mean, stddev);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::pareto(T shape, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  pareto_distribution<Rt> rvs(shape, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::pareto(const expression<Container1, T, Rank> &shape,
                                 const expression<Container2, T, Rank> &scale) {
  pareto_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::pareto(const expression<Container, T, Rank> &shape,
                                 typename Container::value_type scale) {
  pareto_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::pareto(typename Container::value_type shape,
                                 const expression<Container, T, Rank> &scale) {
  pareto_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::pareto(T shape, U scale, size_t size) {
  return this->pareto(shape, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::pareto(T shape, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  pareto_distribution<Rt> rvs(shape, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T>
inline typename detail::promote<T>::type
Generator<bit_generator>::rayleigh(T scale) {
  typedef typename detail::promote<T>::type Rt;
  rayleigh_distribution<Rt> rvs(scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::rayleigh(
    const expression<Container, T, Rank> &scale) {
  rayleigh_distribution<T> rvs;
  return __sample_element_wise(rvs, scale);
}

template <class bit_generator>
template <class T>
inline tensor<typename detail::promote<T>::type, 1>
Generator<bit_generator>::rayleigh(T scale, size_t size) {
  return this->rayleigh(scale, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<typename detail::promote<T>::type, Rank>
Generator<bit_generator>::rayleigh(T scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T>::type Rt;
  rayleigh_distribution<Rt> rvs(scale);
  tensor<Rt, Rank> out(size);
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
inline typename detail::promote<T>::type
Generator<bit_generator>::student_t(T df) {
  typedef typename detail::promote<T>::type Rt;
  student_t_distribution<Rt> rvs(df);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::student_t(const expression<Container, T, Rank> &df) {
  student_t_distribution<T> rvs;
  return __sample_element_wise(rvs, df);
}

template <class bit_generator>
template <class T>
inline tensor<typename detail::promote<T>::type, 1>
Generator<bit_generator>::student_t(T df, size_t size) {
  return this->student_t(df, make_shape(size));
}

template <class bit_generator>
template <class T, size_t Rank>
inline tensor<typename detail::promote<T>::type, Rank>
Generator<bit_generator>::student_t(T df, const shape_t<Rank> &size) {
  typedef typename detail::promote<T>::type Rt;
  student_t_distribution<Rt> rvs(df);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::uniform(T low, U high) {
  typedef typename detail::promote<T, U>::type Rt;
  uniform_real_distribution<Rt> rvs(low, high);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::uniform(const expression<Container1, T, Rank> &low,
                                  const expression<Container2, T, Rank> &high) {
  uniform_real_distribution<T> rvs;
  return __sample_element_wise(rvs, low, high);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::uniform(const expression<Container, T, Rank> &low,
                                  typename Container::value_type high) {
  uniform_real_distribution<T> rvs;
  return __sample_element_wise(rvs, low, high);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::uniform(typename Container::value_type low,
                                  const expression<Container, T, Rank> &high) {
  uniform_real_distribution<T> rvs;
  return __sample_element_wise(rvs, low, high);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::uniform(T low, U high, size_t size) {
  return this->uniform(low, high, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::uniform(T low, U high, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  uniform_real_distribution<Rt> rvs(low, high);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::wald(T mean, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  inverse_gaussian_distribution<Rt> rvs(mean, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank>
Generator<bit_generator>::wald(const expression<Container1, T, Rank> &mean,
                               const expression<Container2, T, Rank> &scale) {
  inverse_gaussian_distribution<T> rvs;
  return __sample_element_wise(rvs, mean, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::wald(const expression<Container, T, Rank> &mean,
                               typename Container::value_type scale) {
  inverse_gaussian_distribution<T> rvs;
  return __sample_element_wise(rvs, mean, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::wald(typename Container::value_type mean,
                               const expression<Container, T, Rank> &scale) {
  inverse_gaussian_distribution<T> rvs;
  return __sample_element_wise(rvs, mean, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::wald(T mean, U scale, size_t size) {
  return this->wald(mean, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::wald(T mean, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  inverse_gaussian_distribution<Rt> rvs(mean, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, class U>
inline typename detail::promote<T, U>::type
Generator<bit_generator>::weibull(T shape, U scale) {
  typedef typename detail::promote<T, U>::type Rt;
  weibull_distribution<Rt> rvs(shape, scale);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::weibull(
    const expression<Container1, T, Rank> &shape,
    const expression<Container2, T, Rank> &scale) {
  weibull_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::weibull(const expression<Container, T, Rank> &shape,
                                  typename Container::value_type scale) {
  weibull_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::weibull(typename Container::value_type shape,
                                  const expression<Container, T, Rank> &scale) {
  weibull_distribution<T> rvs;
  return __sample_element_wise(rvs, shape, scale);
}

template <class bit_generator>
template <class T, class U>
inline tensor<typename detail::promote<T, U>::type, 1>
Generator<bit_generator>::weibull(T shape, U scale, size_t size) {
  return this->weibull(shape, scale, make_shape(size));
}

template <class bit_generator>
template <class T, class U, size_t Rank>
inline tensor<typename detail::promote<T, U>::type, Rank>
Generator<bit_generator>::weibull(T shape, U scale, const shape_t<Rank> &size) {
  typedef typename detail::promote<T, U>::type Rt;
  weibull_distribution<Rt> rvs(shape, scale);
  tensor<Rt, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

/// Discrete distributions.

template <class bit_generator>
inline bool Generator<bit_generator>::bernoulli(double prob) {
  bernoulli_distribution rvs(prob);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container, size_t Rank>
inline tensor<bool, Rank> Generator<bit_generator>::bernoulli(
    const expression<Container, double, Rank> &prob) {
  bernoulli_distribution rvs;
  return __sample_element_wise(rvs, prob);
}

template <class bit_generator>
inline tensor<bool, 1> Generator<bit_generator>::bernoulli(double prob,
                                                           size_t size) {
  return this->bernoulli(prob, make_shape(size));
}

template <class bit_generator>
template <size_t Rank>
inline tensor<bool, Rank>
Generator<bit_generator>::bernoulli(double prob, const shape_t<Rank> &size) {
  bernoulli_distribution rvs(prob);
  tensor<bool, Rank> out(size);
  __sample_distribution(out.data(), out.size(), rvs);
  return out;
}

template <class bit_generator>
template <class T, detail::RequiresIntegral<T>>
inline T Generator<bit_generator>::binomial(T n, double prob) {
  binomial_distribution<T> rvs(n, prob);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::binomial(
    const expression<Container1, T, Rank> &n,
    const expression<Container2, double, Rank> &prob) {
  binomial_distribution<T> rvs;
  return __sample_element_wise(rvs, n, prob);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank>
Generator<bit_generator>::binomial(const expression<Container, T, Rank> &n,
                                   double prob) {
  binomial_distribution<T> rvs;
  return __sample_element_wise(rvs, n, prob);
}

template <class bit_generator>
template <class Container, class T, size_t Rank, detail::RequiresIntegral<T>>
inline tensor<T, Rank> Generator<bit_generator>::binomial(
    T n, const expression<Container, double, Rank> &prob) {
  binomial_distribution<T> rvs;
  return __sample_element_wise(rvs, n, prob);
}

template <class bit_generator>
template <class T>
inline tensor<T, 1> Generator<bit_generator>::binomial(T n, double prob,
                                                       size_t size) {
  return this->binomial(n, prob, make_shape(size));
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
inline T Generator<bit_generator>::geometric(double prob) {
  geometric_distribution<T> rvs(prob);
  return rvs(m_rng);
}

template <class bit_generator>
template <class T, class Container, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::geometric(
    const expression<Container, double, Rank> &prob) {
  geometric_distribution<T> rvs;
  return __sample_element_wise(rvs, prob);
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
template <class T, detail::RequiresIntegral<T>>
inline T Generator<bit_generator>::negative_binomial(T n, double prob) {
  negative_binomial_distribution<T> rvs(n, prob);
  return rvs(m_rng);
}

template <class bit_generator>
template <class Container1, class T, size_t Rank, class Container2>
inline tensor<T, Rank> Generator<bit_generator>::negative_binomial(
    const expression<Container1, T, Rank> &n,
    const expression<Container2, double, Rank> &prob) {
  negative_binomial_distribution<T> rvs;
  return __sample_element_wise(rvs, n, prob);
}

template <class bit_generator>
template <class Container, class T, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::negative_binomial(
    const expression<Container, T, Rank> &n, double prob) {
  negative_binomial_distribution<T> rvs;
  return __sample_element_wise(rvs, n, prob);
}

template <class bit_generator>
template <class Container, class T, size_t Rank, detail::RequiresIntegral<T>>
inline tensor<T, Rank> Generator<bit_generator>::negative_binomial(
    T n, const expression<Container, double, Rank> &prob) {
  negative_binomial_distribution<T> rvs;
  return __sample_element_wise(rvs, n, prob);
}

template <class bit_generator>
template <class T>
inline tensor<T, 1>
Generator<bit_generator>::negative_binomial(T n, double prob, size_t size) {
  return this->negative_binomial(n, prob, make_shape(size));
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
inline T Generator<bit_generator>::poisson(double rate) {
  poisson_distribution<T> rvs(rate);
  return rvs(m_rng);
}

template <class bit_generator>
template <class T, class Container, size_t Rank>
inline tensor<T, Rank> Generator<bit_generator>::poisson(
    const expression<Container, double, Rank> &rate) {
  poisson_distribution<T> rvs;
  return __sample_element_wise(rvs, rate);
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
} // namespace numcpp

#endif // NUMCPP_RANDOM_TCC_INCLUDED
