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

#include "numcpp/shape.h"
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
 * @tparam Container Type of the tensor where the function is applied.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Function, class Container, class T, size_t Rank>
class unary_expr : public expression<unary_expr<Function, Container, T, Rank>,
                                     detail::result_of_t<Function, T>, Rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, T> value_type;
  static constexpr size_t rank = Rank;
  typedef flat_iterator<const unary_expr<Function, Container, T, Rank>,
                        value_type, Rank, void, value_type>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // Tensor where the function is applied.
  const Container &m_arg;

public:
  /// Constructors.

  /**
   * @brief Constructs an expression which stores the result of applying an
   * unary function to each element in a tensor.
   *
   * @param f The function to apply.
   * @param a Tensor-like object.
   */
  unary_expr(Function f, const expression<Container, T, Rank> &a)
      : m_fun(f), m_arg(a.self()) {}

  unary_expr(const expression<Container, T, Rank> &a)
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
  iterator begin() const { return begin(this->layout()); }

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
  iterator end() const { return end(this->layout()); }

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
 * @tparam Container1 Type of the first tensor where the function is applied.
 * @tparam T Type of the elements contained in the first tensor.
 * @tparam Container2 Type of the second tensor where the function is applied.
 * @tparam U Type of the elements contained in the second tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Function, class Container1, class T, class Container2, class U,
          size_t Rank>
class binary_expr
    : public expression<
          binary_expr<Function, Container1, T, Container2, U, Rank>,
          detail::result_of_t<Function, T, U>, Rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, T, U> value_type;
  static constexpr size_t rank = Rank;
  typedef flat_iterator<
      const binary_expr<Function, Container1, T, Container2, U, Rank>,
      value_type, Rank, void, value_type>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  const Container1 &m_arg1;

  // Second tensor argument.
  const Container2 &m_arg2;

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
   * @param a First tensor-like argument.
   * @param b Second tensor-like argument.
   */
  binary_expr(Function f, const expression<Container1, T, Rank> &a,
              const expression<Container2, U, Rank> &b)
      : m_fun(f), m_arg1(a.self()), m_arg2(b.self()),
        m_shape(broadcast_shapes(m_arg1.shape(), m_arg2.shape())),
        m_size(m_shape.prod()) {}

  binary_expr(const expression<Container1, T, Rank> &a,
              const expression<Container2, U, Rank> &b)
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
  iterator begin() const { return begin(this->layout()); }

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
  iterator end() const { return end(this->layout()); }

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
    index_type i, j;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (m_arg1.shape(axis) > 1) ? index[axis] : 0;
      j[axis] = (m_arg2.shape(axis) > 1) ? index[axis] : 0;
    }
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

/**
 * @brief Partial specialization of binary_expr when the first argument is a
 * tensor and the second argument is a value. Values are broadcasted to an
 * appropriate shape.
 */
template <class Function, class Container, class T, class U, size_t Rank>
class binary_expr<Function, Container, T, void, U, Rank>
    : public expression<binary_expr<Function, Container, T, void, U, Rank>,
                        detail::result_of_t<Function, T, U>, Rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, T, U> value_type;
  static constexpr size_t rank = Rank;
  typedef flat_iterator<
      const binary_expr<Function, Container, T, void, U, Rank>, value_type,
      Rank, void, value_type>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  const Container &m_arg;

  // Value to use as second argument.
  U m_val;

public:
  /// Constructors.

  binary_expr(Function f, const expression<Container, T, Rank> &a, const U &val)
      : m_fun(f), m_arg(a.self()), m_val(val) {}

  binary_expr(const expression<Container, T, Rank> &a, const U &val)
      : binary_expr(Function(), a, val) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Iterators.

  iterator begin() const { return begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return end(this->layout()); }

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
template <class Function, class T, class Container, class U, size_t Rank>
class binary_expr<Function, void, T, Container, U, Rank>
    : public expression<binary_expr<Function, void, T, Container, U, Rank>,
                        detail::result_of_t<Function, T, U>, Rank> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, T, U> value_type;
  static constexpr size_t rank = Rank;
  typedef flat_iterator<
      const binary_expr<Function, void, T, Container, U, Rank>, value_type,
      Rank, void, value_type>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // Value to use as first argument.
  T m_val;

  // Second tensor argument.
  const Container &m_arg;

public:
  /// Constructors.

  binary_expr(Function f, const T &val, const expression<Container, U, Rank> &b)
      : m_fun(f), m_val(val), m_arg(b.self()) {}

  binary_expr(const T &val, const expression<Container, U, Rank> &b)
      : binary_expr(Function(), val, b) {}

  /// Destructor.
  ~binary_expr() = default;

  /// Iterators.

  iterator begin() const { return begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return end(this->layout()); }

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
 * @tparam Container1 Type of the first tensor where the function is applied.
 * @tparam T Type of the elements contained in the first tensor.
 * @tparam Rank1 Dimension of the first tensor.
 * @tparam Container2 Type of the second tensor where the function is applied.
 * @tparam U Type of the elements contained in the second tensor.
 * @tparam Rank2 Dimension of the second tensor.
 */
template <class Function, class Container1, class T, size_t Rank1,
          class Container2, class U, size_t Rank2>
class outer_expr
    : public expression<
          outer_expr<Function, Container1, T, Rank1, Container2, U, Rank2>,
          detail::result_of_t<Function, T, U>, Rank1 + Rank2> {
public:
  /// Member types.
  typedef detail::result_of_t<Function, T, U> value_type;
  static constexpr size_t rank = Rank1 + Rank2;
  typedef flat_iterator<
      const outer_expr<Function, Container1, T, Rank1, Container2, U, Rank2>,
      value_type, rank, void, value_type>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

private:
  // Function to apply.
  Function m_fun;

  // First tensor argument.
  const Container1 &m_arg1;

  // Second tensor argument.
  const Container2 &m_arg2;

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
   * @param a First tensor-like argument.
   * @param b Second tensor-like argument.
   */
  outer_expr(Function f, const expression<Container1, T, Rank1> &a,
             const expression<Container2, U, Rank2> &b)
      : m_fun(f), m_arg1(a.self()), m_arg2(b.self()),
        m_shape(shape_cat(m_arg1.shape(), m_arg2.shape())),
        m_size(a.size() * b.size()) {}

  outer_expr(const expression<Container1, T, Rank1> &a,
             const expression<Container2, U, Rank2> &b)
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
  iterator begin() const { return begin(this->layout()); }

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
  iterator end() const { return end(this->layout()); }

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
  value_type operator[](const index_type &index) const {
    index_t<Rank1> i;
    std::copy_n(index.data(), Rank1, i.data());
    index_t<Rank2> j;
    std::copy_n(index.data() + Rank1, Rank2, j.data());
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
} // namespace numcpp

#endif // NUMCPP_LAZY_EXPRESSION_H_INCLUDED
