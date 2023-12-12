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

/** @file include/numcpp/functional/vectorize.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/functional.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_VECTORIZE_H_INCLUDED
#define NUMCPP_VECTORIZE_H_INCLUDED

#include "numcpp/functional/lazy_expression.h"

namespace numcpp {
/**
 * @brief Class returned by @c vectorize. A vectorized function is a wrapper for
 * a function that operates on tensors in an element-by-element fashion,
 * supporting broadcasting.
 *
 * @tparam Function Function type.
 */
template <class Function> class vectorized_expr {
private:
  // Function to vectorize.
  Function m_fun;

public:
  /**
   * @brief Constructor.
   *
   * @param f Function to vectorize.
   */
  vectorized_expr() : m_fun() {}

  vectorized_expr(Function f) : m_fun(f) {}

  /**
   * @brief Assigns a new function.
   *
   * @param other Another vectorized function object to copy.
   * @param f Function to vectorize.
   */
  vectorized_expr &operator=(const vectorized_expr &other) {
    m_fun = other.m_fun;
    return *this;
  }

  vectorized_expr &operator=(Function f) {
    m_fun = f;
    return *this;
  }

  /**
   * @brief Return a pointer to the stored function.
   */
  Function *target() { return &m_fun; }
  const Function *target() const { return &m_fun; }

  /**
   * @brief Invokes the function.
   *
   * @param args... Parameters to pass to the stored function.
   *
   * @return The return value of the invocation.
   */
  template <class... T>
  auto operator()(T &&...args) -> decltype(m_fun(std::forward<T>(args)...)) {
    return m_fun(std::forward<T>(args)...);
  }

  /**
   * @brief Apply the function element-wise.
   *
   * @param a A tensor-like object with the values where the function will be
   *          invoked.
   *
   * @return A light-weight object which stores the result of invoking the
   *         function on each element. This function does not create a new
   *         tensor, instead, an expression object is returned (see
   *         lazy-evaluation).
   */
  template <class Container, class T, size_t Rank>
  unary_expr<Function, Container, T, Rank>
  operator()(const expression<Container, T, Rank> &a) {
    return unary_expr<Function, Container, T, Rank>(m_fun, a);
  }

  /**
   * @brief Apply the function element-wise.
   *
   * @param a A tensor-like object with the values to pass as first argument.
   * @param b A tensor-like object with the values to pass as second argument.
   * @param val Value to use either as first argument or second argument. Values
   *            are broadcasted to an appropriate shape.
   *
   * @return A light-weight object which stores the result of invoking the
   *         function on each element. This function does not create a new
   *         tensor, instead, an expression object is returned (see
   *         lazy-evalaution).
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   */
  template <class Container1, class T, class Container2, class U, size_t Rank>
  binary_expr<Function, Container1, T, Container2, U, Rank>
  operator()(const expression<Container1, T, Rank> &a,
             const expression<Container2, U, Rank> &b) {
    return binary_expr<Function, Container1, T, Container2, U, Rank>(m_fun, a,
                                                                     b);
  }

  template <class Container, class T, class U, size_t Rank,
            detail::RequiresScalar<U> = 0>
  binary_expr<Function, Container, T, void, U, Rank>
  operator()(const expression<Container, T, Rank> &a, const U &val) {
    return binary_expr<Function, Container, T, void, U, Rank>(m_fun, a, val);
  }

  template <class T, class Container, class U, size_t Rank,
            detail::RequiresScalar<T> = 0>
  binary_expr<Function, void, T, Container, U, Rank>
  operator()(const T &val, const expression<Container, U, Rank> &b) {
    return binary_expr<Function, void, T, Container, U, Rank>(m_fun, val, b);
  }

#if __cplusplus >= 201402L
  /**
   * @brief Apply the function element-wise.
   *
   * @param a, b... Tensor-like objects with the values to pass. Since C++14,
   *                this function accepts any number of arguments.
   *
   * @return A light-weight object which stores the result of invoking the
   *         function on each element. This function does not create a new
   *         tensor, instead, an expression object is returned (see
   *         lazy-evalaution).
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   */
  template <class Container1, class T, class... Container2, class... U,
            size_t Rank>
  element_wise_expr<Function, Container1, Container2...>
  operator()(const expression<Container1, T, Rank> &a,
             const expression<Container2, U, Rank> &...b) {
    return element_wise_expr<Function, Container1, Container2...>(m_fun, a,
                                                                  b...);
  }
#endif // C++14
};
} // namespace numcpp

#endif // NUMCPP_VECTORIZE_H_INCLUDED
