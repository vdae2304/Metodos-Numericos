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

/** @file include/numcpp/functional/lazy_expression.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/functional.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_EXPRESSION_H_INCLUDED
#define NUMCPP_LAZY_EXPRESSION_H_INCLUDED

#include <tuple>
#include "numcpp/shape.h"
#include "numcpp/tensor/abstract_tensor.h"
#include "numcpp/iterators/flat_iterator.h"

namespace numcpp {
/**
 * @brief A light-weight object which stores the result of applying an unary
 * function to each element in a tensor object. This class represents an
 * expression rather than a container. Such expressions relies on lazy
 * evaluation, meaning that multiple expressions can be nested and the result
 * of the whole expression will be computed only at the end, when the whole
 * expression is evaluated or assigned to a tensor object.
 *
 * @tparam Function Type of the applied function.
 * @tparam Expression Type of the tensor where the function is applied.
 */
template <class Function, class Expression>
class unary_expr
    : public abstract_tensor<
          unary_expr<Function, Expression>,
          detail::result_of_t<Function, typename Expression::value_type>,
          Expression::rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, typename Expression::value_type>
      value_type;
  static constexpr size_t rank = Expression::rank;
  typedef void pointer;
  typedef value_type reference;
  typedef flat_iterator<const unary_expr<Function, Expression>> iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // Tensor where the function is applied.
  const Expression &m_arg;

public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying an
   * unary function to each element in a tensor.
   *
   * @param f The function to apply.
   * @param a Abstract tensor.
   */
  unary_expr(Function f,
             const abstract_tensor<Expression, typename Expression::value_type,
                                   Expression::rank> &a)
      : m_fun(f), m_arg(a.self()) {}

  unary_expr(const abstract_tensor<Expression, typename Expression::value_type,
                                   Expression::rank> &a)
      : unary_expr(Function(), a) {}

  /// Destructor.
  ~unary_expr() = default;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor.
   */
  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   */
  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The result of the function evaluation at the specified position in
   *         the tensor.
   */
  auto operator[](const index_type &index) const
      -> decltype(m_fun(m_arg[index])) {
    return m_fun(m_arg[index]);
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  shape_type shape() const { return m_arg.shape(); }

  size_type shape(size_type axis) const { return m_arg.shape(axis); }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }
};

/**
 * @brief A light-weight object which stores the result of applying a binary
 * function to each element in two tensor objects. This class represents an
 * expression rather than a container. Such expressions relies on lazy
 * evaluation, meaning that multiple expressions can be nested and the result
 * of the whole expression will be computed only at the end, when the whole
 * expression is evaluated or assigned to a tensor object.
 *
 * @tparam Function Type of the applied function.
 * @tparam LhsExpression Type of the first tensor where the function is
 *                       applied.
 * @tparam RhsExpression Type of the second tensor where the function is
 *                       applied.
 */
template <class Function, class LhsExpression, class RhsExpression>
class binary_expr
    : public abstract_tensor<
          binary_expr<Function, LhsExpression, RhsExpression>,
          detail::result_of_t<Function, typename LhsExpression::value_type,
                              typename RhsExpression::value_type>,
          LhsExpression::rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, typename LhsExpression::value_type,
                              typename RhsExpression::value_type>
      value_type;
  static constexpr size_t rank = LhsExpression::rank;
  typedef void pointer;
  typedef value_type reference;
  typedef flat_iterator<
      const binary_expr<Function, LhsExpression, RhsExpression>>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  const LhsExpression &m_arg1;

  // Second tensor argument.
  const RhsExpression &m_arg2;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * binary function to each element in two tensors.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   */
  binary_expr(
      Function f,
      const abstract_tensor<LhsExpression, typename LhsExpression::value_type,
                            LhsExpression::rank> &a,
      const abstract_tensor<RhsExpression, typename RhsExpression::value_type,
                            RhsExpression::rank> &b)
      : m_fun(f), m_arg1(a.self()), m_arg2(b.self()),
        m_shape(broadcast_shapes(m_arg1.shape(), m_arg2.shape())),
        m_size(m_shape.prod()) {}

  binary_expr(
      const abstract_tensor<LhsExpression, typename LhsExpression::value_type,
                            LhsExpression::rank> &a,
      const abstract_tensor<RhsExpression, typename RhsExpression::value_type,
                            RhsExpression::rank> &b)
      : binary_expr(Function(), a, b) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor.
   */
  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   */
  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The result of the function evaluation at the specified position in
   *         the tensor.
   */
  auto operator[](const index_type &index) const
      -> decltype(m_fun(m_arg1[index], m_arg2[index])) {
    return m_fun(m_arg1[detail::broadcast_index(index, m_arg1.shape())],
                 m_arg2[detail::broadcast_index(index, m_arg2.shape())]);
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_size; }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const {
    if (m_arg1.layout() == m_arg2.layout()) {
      return m_arg1.layout();
    } else {
      return default_layout;
    }
  }
};

/**
 * @brief Partial specialization of binary_expr when the first argument is a
 * tensor and the second argument is a value. Values are broadcasted to an
 * appropriate shape.
 */
template <class Function, class Expression, class T>
class binary_expr<Function, Expression, detail::identity<T>>
    : public abstract_tensor<
          binary_expr<Function, Expression, detail::identity<T>>,
          detail::result_of_t<Function, typename Expression::value_type, T>,
          Expression::rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, typename Expression::value_type, T>
      value_type;
  static constexpr size_t rank = Expression::rank;
  typedef void pointer;
  typedef value_type reference;
  typedef flat_iterator<
      const binary_expr<Function, Expression, detail::identity<T>>>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  const Expression &m_arg;

  // Value to use as second argument.
  T m_val;

public:
  /// Constructors.

  binary_expr(Function f,
              const abstract_tensor<Expression, typename Expression::value_type,
                                    Expression::rank> &a,
              const T &val)
      : m_fun(f), m_arg(a.self()), m_val(val) {}

  binary_expr(const abstract_tensor<Expression, typename Expression::value_type,
                                    Expression::rank> &a,
              const T &val)
      : binary_expr(Function(), a, val) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Iterators.

  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  auto operator[](const index_type &index) const
      -> decltype(m_fun(m_arg[index], m_val)) {
    return m_fun(m_arg[index], m_val);
  }

  shape_type shape() const { return m_arg.shape(); }

  size_type shape(size_type axis) const { return m_arg.shape(axis); }

  size_type size() const { return m_arg.size(); }

  layout_t layout() const { return m_arg.layout(); }
};

/**
 * @brief Partial specialization of binary_expr when the first argument is a
 * value and the second argument is a tensor. Values are broadcasted to an
 * appropriate shape.
 */
template <class Function, class Expression, class T>
class binary_expr<Function, detail::identity<T>, Expression>
    : public abstract_tensor<
          binary_expr<Function, detail::identity<T>, Expression>,
          detail::result_of_t<Function, T, typename Expression::value_type>,
          Expression::rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, T, typename Expression::value_type>
      value_type;
  static constexpr size_t rank = Expression::rank;
  typedef void pointer;
  typedef value_type reference;
  typedef flat_iterator<
      const binary_expr<Function, detail::identity<T>, Expression>>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // Value to use as first argument.
  T m_val;

  // Second tensor argument.
  const Expression &m_arg;

public:
  /// Constructors.

  binary_expr(Function f, const T &val,
              const abstract_tensor<Expression, typename Expression::value_type,
                                    Expression::rank> &b)
      : m_fun(f), m_val(val), m_arg(b.self()) {}

  binary_expr(const T &val,
              const abstract_tensor<Expression, typename Expression::value_type,
                                    Expression::rank> &b)
      : binary_expr(Function(), val, b) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Iterators.

  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  auto operator[](const index_type &index) const
      -> decltype(m_fun(m_val, m_arg[index])) {
    return m_fun(m_val, m_arg[index]);
  }

  shape_type shape() const { return m_arg.shape(); }

  size_type shape(size_type axis) const { return m_arg.shape(axis); }

  size_type size() const { return m_arg.size(); }

  layout_t layout() const { return m_arg.layout(); }
};

/**
 * @brief A light-weight object which stores the result of applying a binary
 * function to all pairs of elements from two tensors. This class represents an
 * expression rather than a container. Such expressions relies on lazy
 * evaluation, meaning that multiple expressions can be nested and the result
 * of the whole expression will be computed only at the end, when the whole
 * expression is evaluated or assigned to a tensor object.
 *
 * @tparam Function Type of the applied function.
 * @tparam LhsExpression Type of the first tensor where the function is
 *                       applied.
 * @tparam RhsExpression Type of the second tensor where the function is
 *                       applied.
 */
template <class Function, class LhsExpression, class RhsExpression>
class outer_expr
    : public abstract_tensor<
          outer_expr<Function, LhsExpression, RhsExpression>,
          detail::result_of_t<Function, typename LhsExpression::value_type,
                              typename RhsExpression::value_type>,
          LhsExpression::rank + RhsExpression::rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, typename LhsExpression::value_type,
                              typename RhsExpression::value_type>
      value_type;
  static constexpr size_t rank = LhsExpression::rank + RhsExpression::rank;
  typedef void pointer;
  typedef value_type reference;
  typedef flat_iterator<
      const outer_expr<Function, LhsExpression, RhsExpression>>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  const LhsExpression &m_arg1;

  // Second tensor argument.
  const RhsExpression &m_arg2;

  // Output shape.
  shape_type m_shape;

  // Output size.
  size_type m_size;

public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * binary function to all pairs of elements.
   *
   * @param f The function to apply.
   * @param a First argument.
   * @param b Second argument.
   */
  outer_expr(
      Function f,
      const abstract_tensor<LhsExpression, typename LhsExpression::value_type,
                            LhsExpression::rank> &a,
      const abstract_tensor<RhsExpression, typename RhsExpression::value_type,
                            RhsExpression::rank> &b)
      : m_fun(f), m_arg1(a.self()), m_arg2(b.self()),
        m_shape(shape_cat(m_arg1.shape(), m_arg2.shape())),
        m_size(a.size() * b.size()) {}

  outer_expr(
      const abstract_tensor<LhsExpression, typename LhsExpression::value_type,
                            LhsExpression::rank> &a,
      const abstract_tensor<RhsExpression, typename RhsExpression::value_type,
                            RhsExpression::rank> &b)
      : outer_expr(Function(), a, b) {}

  /// Destructor.
  ~outer_expr() = default;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor.
   */
  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   */
  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The result of the function evaluation at the specified position in
   *         the tensor.
   */
  auto operator[](const index_type &index) const
      -> decltype(m_fun(m_arg1[typename LhsExpression::index_type()],
                        m_arg2[typename RhsExpression::index_type()])) {
    typename LhsExpression::index_type i;
    typename RhsExpression::index_type j;
    std::copy_n(index.data(), m_arg1.rank, i.data());
    std::copy_n(index.data() + m_arg1.rank, m_arg2.rank, j.data());
    return m_fun(m_arg1[i], m_arg2[j]);
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_size; }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const {
    if (m_arg1.layout() == m_arg2.layout()) {
      return m_arg1.layout();
    } else {
      return default_layout;
    }
  }
};

#if __cplusplus >= 201402L
/**
 * @brief A light-weight object which stores the result of applying a function
 * element-wise. Unlike unary_expr and binary_expr, element_wise_expr accepts
 * any number of arguments. However, arguments must be tensors rather than
 * scalar values. This class represents an expression rather than a container.
 * Such expressions relies on lazy evaluation, meaning that multiple expressions
 * can be nested and the result of the whole expression will be computed only at
 * the end, when the whole expression is evaluated or assigned to a tensor
 * object.
 *
 * @tparam Function Type of the applied function.
 * @tparam LhsExpression Type of the first tensor where the function is
 *                       applied.
 * @tparam RhsExpression... Type of the remaining tensors where the function is
 *                          applied.
 */
template <class Function, class LhsExpression, class... RhsExpression>
class element_wise_expr
    : public abstract_tensor<
          element_wise_expr<Function, LhsExpression, RhsExpression...>,
          detail::result_of_t<Function, typename LhsExpression::value_type,
                              typename RhsExpression::value_type...>,
          LhsExpression::rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, typename LhsExpression::value_type,
                              typename RhsExpression::value_type...>
      value_type;
  static constexpr size_t rank = LhsExpression::rank;
  typedef void pointer;
  typedef value_type reference;
  typedef flat_iterator<
      const element_wise_expr<Function, LhsExpression, RhsExpression...>>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // Tensor arguments.
  std::tuple<const LhsExpression &, const RhsExpression &...> m_args;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying a
   * function element-wise.
   *
   * @param f The function to apply.
   * @param a, b... Tensor arguments.
   */
  element_wise_expr(
      Function f,
      const abstract_tensor<LhsExpression, typename LhsExpression::value_type,
                            LhsExpression::rank> &a,
      const abstract_tensor<RhsExpression, typename RhsExpression::value_type,
                            RhsExpression::rank> &...b)
      : m_fun(f), m_args(a.self(), b.self()...),
        m_shape(broadcast_shapes(a.shape(), b.shape()...)),
        m_size(m_shape.prod()) {}

  element_wise_expr(
      const abstract_tensor<LhsExpression, typename LhsExpression::value_type,
                            LhsExpression::rank> &a,
      const abstract_tensor<RhsExpression, typename RhsExpression::value_type,
                            RhsExpression::rank> &...b)
      : element_wise_expr(Function(), a, b...) {}

  /// Destructor.
  ~element_wise_expr() = default;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor.
   */
  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   */
  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns the result of applying the function to
   * an element in the tensor.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The result of the function evaluation at the specified position in
   *         the tensor.
   */
  auto operator[](const index_type &index) const {
    return __at(index,
                std::make_index_sequence<1 + sizeof...(RhsExpression)>());
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const { return m_size; }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return std::get<0>(m_args).layout(); }

private:
  /**
   * @brief Broadcasts an index and returns the element at the specified
   * position.
   */
  template <class Expression, class T, size_t Rank>
  T __broadcast(const abstract_tensor<Expression, T, Rank> &a,
                index_type index) const {
    static_assert(Rank == rank, "Dimension must be the same for all operators");
    for (size_t axis = 0; axis < rank; ++axis) {
      if (a.shape(axis) == 1) {
        index[axis] = 0;
      }
    }
    return a[index];
  }

  /**
   * @brief Implementation of subscript operator.
   */
  template <size_t... Is>
  auto __at(const index_type &index, std::index_sequence<Is...>) const {
    return m_fun(std::get<Is>(m_args)[detail::broadcast_index(
        index, std::get<Is>(m_args).shape())]...);
  }
};
#endif // C++14
} // namespace numcpp

#endif // NUMCPP_LAZY_EXPRESSION_H_INCLUDED
