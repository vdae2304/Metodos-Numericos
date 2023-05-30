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

#if __cplusplus >= 201703L
namespace numcpp {
/**
 * @brief Class returned by @c vectorize. A vectorized function is a function
 * that operates on tensors in an element-by-element fashion.
 *
 * @tparam Function Function type.
 * @tparam Identity Identity type. A nullptr indicates there is no identity.
 */
template <class Function, class Identity> class vectorized_expr {
private:
  // Function to vectorize.
  Function m_fun;

  // Identity value.
  Identity m_identity;

public:
  /**
   * @brief Constructor.
   *
   * @param f Function to vectorize.
   * @param identity (Optional) Identity value.
   */
  vectorized_expr(Function f) : m_fun(f), m_identity() {}

  vectorized_expr(Function f, const Identity &identity)
      : m_fun(f), m_identity(identity) {}

  /**
   * @brief Return a pointer to the stored function.
   */
  Function *target() { return &m_fun; }
  const Function *target() const { return &m_fun; }

  /**
   * @brief Return whether the function has identity.
   */
  static constexpr bool has_identity() {
    return !std::is_same_v<Identity, nullptr_t>;
  }

  /**
   * @brief Return the identity value.
   */
  const Identity &identity() {
    static_assert(has_identity(),
                  "Function doesn't have an identity to return");
    return m_identity;
  }

  /**
   * @brief Set the identity value.
   */
  void identity(const Identity &identity) {
    static_assert(has_identity(), "Function doesn't have an identity to set");
    m_identity = identity;
  }

  /**
   * @brief Invokes the underlying function.
   */
  template <class T, detail::RequiresScalar<T> = 0>
  auto operator()(T &&a) -> decltype(m_fun(std::forward<T>(a))) {
    return m_fun(std::forward<T>(a));
  }

  template <class T, class U, detail::RequiresScalar<T> = 0,
            detail::RequiresScalar<U> = 0>
  auto operator()(T &&a, U &&b)
      -> decltype(m_fun(std::forward<T>(a), std::forward<U>(b))) {
    return m_fun(std::forward<T>(a), std::forward<U>(b));
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

  /**
   * @brief Reduce the tensor's dimension by cumulatively applying the function
   * to all elements.
   *
   * @param a A tensor-like object with the values where the reduction will be
   *          performed.
   * @param init Initial value. If not provided, the identity value is provided.
   * @param where A boolean tensor which indicates the elements to include in
   *              the reduction. For functions that do not have an identity
   *              defined, one has to pass in also @a init.
   *
   * @return The result of accumulating all the elements in the tensor.
   */
  template <class Container, class T, size_t Rank>
  T reduce(const expression<Container, T, Rank> &a) {
    if constexpr (has_identity()) {
      return numcpp::reduce(m_fun, a, m_identity);
    } else {
      return numcpp::reduce(m_fun, a);
    }
  }

  template <class Container, class T, size_t Rank>
  T reduce(const expression<Container, T, Rank> &a,
           typename Container::value_type init) {
    return numcpp::reduce(m_fun, a, init);
  }

  template <class Container1, class T, size_t Rank, class Container2>
  T reduce(const expression<Container1, T, Rank> &a,
           const expression<Container2, bool, Rank> &where) {
    static_assert(has_identity(), "Function doesn't have an identity, so to use"
                                  " a where mask one has to specify 'init'");
    return numcpp::reduce(m_fun, a, m_identity, where);
  }

  template <class Container1, class T, size_t Rank, class Container2>
  T reduce(const expression<Container1, T, Rank> &a,
           typename Container1::value_type init,
           const expression<Container2, bool, Rank> &where) {
    return numcpp::reduce(m_fun, a, init, where);
  }

  /**
   * @brief Reduce the tensor's dimension by cumulatively applying the function
   * over multiple axes.
   *
   * @param a A tensor-like object with the values where the reduction will be
   *          performed.
   * @param axes Axes along which the reduction is performed.
   * @param keepdims If set to @a keepdims, the axes which are reduced are left
   *                 as dimensions with size one. If set to @a dropdims, the
   *                 axes which are reduced are dropped. Defaults to
   *                 @a dropdims.
   * @param init Initial value. If not provided, the identity value is provided.
   * @param where A boolean tensor which indicates the elements to include in
   *              the reduction. For functions that do not have an identity
   *              defined, one has to pass in also @a init.
   *
   * @return A new tensor with the result of performing the reduction over the
   *         given axes.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class Container, class T, size_t Rank, size_t N>
  tensor<T, Rank - N> reduce(const expression<Container, T, Rank> &a,
                             const shape_t<N> &axes) {
    if constexpr (has_identity()) {
      return numcpp::reduce(m_fun, a, axes, m_identity);
    } else {
      return numcpp::reduce(m_fun, a, axes);
    }
  }

  template <class Container, class T, size_t Rank, size_t N>
  tensor<T, Rank - N> reduce(const expression<Container, T, Rank> &a,
                             const shape_t<N> &axes,
                             typename Container::value_type init) {
    return numcpp::reduce(m_fun, a, axes, init);
  }

  template <class Container1, class T, size_t Rank, size_t N, class Container2>
  tensor<T, Rank - N> reduce(const expression<Container1, T, Rank> &a,
                             const shape_t<N> &axes,
                             const expression<Container2, bool, Rank> &where) {
    static_assert(has_identity(), "Function doesn't have an identity, so to use"
                                  " a where mask one has to specify 'init'");
    return numcpp::reduce(m_fun, a, axes, m_identity, where);
  }

  template <class Container1, class T, size_t Rank, size_t N, class Container2>
  tensor<T, Rank - N> reduce(const expression<Container1, T, Rank> &a,
                             const shape_t<N> &axes,
                             typename Container1::value_type init,
                             const expression<Container2, bool, Rank> &where) {
    return numcpp::reduce(m_fun, a, axes, init, where);
  }

  template <class Container, class T, size_t Rank, size_t N>
  tensor<T, Rank> reduce(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, keepdims_t) {
    if constexpr (has_identity()) {
      return numcpp::reduce(m_fun, a, axes, keepdims, m_identity);
    } else {
      return numcpp::reduce(m_fun, a, axes, keepdims);
    }
  }

  template <class Container, class T, size_t Rank, size_t N>
  tensor<T, Rank> reduce(const expression<Container, T, Rank> &a,
                         const shape_t<N> &axes, keepdims_t,
                         typename Container::value_type init) {
    return numcpp::reduce(m_fun, a, axes, keepdims, init);
  }

  template <class Container1, class T, size_t Rank, size_t N, class Container2>
  tensor<T, Rank> reduce(const expression<Container1, T, Rank> &a,
                         const shape_t<N> &axes, keepdims_t,
                         const expression<Container2, bool, Rank> &where) {
    static_assert(has_identity(), "Function doesn't have an identity, so to use"
                                  " a where mask one has to specify 'init'");
    return numcpp::reduce(m_fun, a, axes, keepdims, m_identity, where);
  }

  template <class Container1, class T, size_t Rank, size_t N, class Container2>
  tensor<T, Rank> reduce(const expression<Container1, T, Rank> &a,
                         const shape_t<N> &axes, keepdims_t,
                         typename Container1::value_type init,
                         const expression<Container2, bool, Rank> &where) {
    return numcpp::reduce(m_fun, a, axes, keepdims, init, where);
  }

  template <class Container, class T, size_t Rank, size_t N>
  tensor<T, Rank - N> reduce(const expression<Container, T, Rank> &a,
                             const shape_t<N> &axes, dropdims_t) {
    if constexpr (has_identity()) {
      return numcpp::reduce(m_fun, a, axes, dropdims, m_identity);
    } else {
      return numcpp::reduce(m_fun, a, axes, dropdims);
    }
  }

  template <class Container, class T, size_t Rank, size_t N>
  tensor<T, Rank - N> reduce(const expression<Container, T, Rank> &a,
                             const shape_t<N> &axes, dropdims_t,
                             typename Container::value_type init) {
    return numcpp::reduce(m_fun, a, axes, dropdims, init);
  }

  template <class Container1, class T, size_t Rank, size_t N, class Container2>
  tensor<T, Rank - N> reduce(const expression<Container1, T, Rank> &a,
                             const shape_t<N> &axes, dropdims_t,
                             const expression<Container2, bool, Rank> &where) {
    static_assert(has_identity(), "Function doesn't have an identity, so to use"
                                  " a where mask one has to specify 'init'");
    return numcpp::reduce(m_fun, a, axes, dropdims, m_identity, where);
  }

  template <class Container1, class T, size_t Rank, size_t N, class Container2>
  tensor<T, Rank - N> reduce(const expression<Container1, T, Rank> &a,
                             const shape_t<N> &axes, dropdims_t,
                             typename Container1::value_type init,
                             const expression<Container2, bool, Rank> &where) {
    return numcpp::reduce(m_fun, a, axes, dropdims, init, where);
  }

  /**
   * @brief Accumulate the result of applying the function along an axis.
   *
   * @param a A tensor-like object with the values where the accumulation will
   *          be performed.
   * @param axis Axis along which to apply the accumulation. Default is zero.
   *
   * @return A new tensor with the accumulated values along the given axis.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class Container, class T, size_t Rank>
  tensor<T, Rank> accumulate(const expression<Container, T, Rank> &a,
                             size_t axis = 0) {
    return numcpp::accumulate(m_fun, a, axis);
  }

  /**
   * @brief Apply a function to all pairs @a (ai,bj) with @a ai in @a a and
   * @a bj in @a b.
   *
   * @param a A tensor-like object with the values to pass as first argument.
   * @param b A tensor-like object with the values to pass as second argument.
   *
   * @return A light-weight object which stores the result of invoking the
   *         function to all pairs of elements. This function does not create a
   *         new tensor, instead, an expression object is returned (see
   *         lazy-evaluation).
   */
  template <class Container1, class T, size_t Rank1, class Container2, class U,
            size_t Rank2>
  outer_expr<Function, Container1, T, Rank1, Container2, U, Rank2>
  outer(const expression<Container1, T, Rank1> &a,
        const expression<Container2, U, Rank2> &b) {
    return outer_expr<Function, Container1, T, Rank1, Container2, U, Rank2>(
        m_fun, a, b);
  }
};

/**
 * @brief Vectorize a function.
 * 
 * @param f Function to vectorize.
 * @param identity (Optional) Identity value.
 * 
 * @return A vectorized function that operates on tensors in an
 *         element-by-element fashion.
 */
template <class Function>
vectorized_expr<Function, nullptr_t> vectorize(Function &&f) {
  return vectorized_expr<Function, nullptr_t>(std::forward<Function>(f));
}

template <class Function, class T>
vectorized_expr<Function, T> vectorize(Function &&f, const T &identity) {
  return vectorized_expr<Function, T>(std::forward<Function>(f), identity);
}
} // namespace numcpp
#endif // C++17

#endif // NUMCPP_VECTORIZE_H_INCLUDED