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

/** @file include/numcpp/math/complexfwd.h
 *  This header defines function objects for several complex-valued mathematical
 *  functions.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_COMPLEXFWD_H_INCLUDED
#define NUMCPP_COMPLEXFWD_H_INCLUDED

#include <complex>

namespace numcpp {
/// Namespace for mathematical functions.
namespace math {
/**
 * @brief Function object implementing @c std::real.
 */
struct real {
  template <class T> T operator()(const std::complex<T> &z) const {
    return z.real();
  }

  template <class T> T operator()(T x) const { return x; }
};

/**
 * @brief Function object implementing @c std::imag.
 */
struct imag {
  template <class T> T operator()(const std::complex<T> &z) const {
    return z.imag();
  }

  template <class T> T operator()(T x) const { return T(); }
};

/**
 * @brief Function object implementing @c std::conj.
 */
struct conj {
  template <class T>
  std::complex<T> operator()(const std::complex<T> &z) const {
    return std::conj(z);
  }

  template <class T> T operator()(T x) const { return x; }
};

/**
 * @brief Function object implementing @c std::arg.
 */
struct arg {
  template <class T> T operator()(const std::complex<T> &z) const {
    return std::arg(z);
  }

  template <class T> auto operator()(T x) const -> decltype(std::arg(x)) {
    return std::arg(x);
  }
};
} // namespace math
} // namespace numcpp

#endif // NUMCPP_COMPLEXFWD_H_INCLUDED
