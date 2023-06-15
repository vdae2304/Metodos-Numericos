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

/** @file include/numcpp/routines/lazy_where.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/routines.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_WHERE_H_INCLUDED
#define NUMCPP_LAZY_WHERE_H_INCLUDED

namespace numcpp {
/**
 * @brief A light-weight object which selects elements from two tensors
 * depending on condition. This class represents an expression rather than a
 * container. Such expressions relies on short-circuit evaluation, meaning that
 * exactly one of the two tensors is evaluated at each position.
 *
 * @tparam Container Type of the tensor representing the condition.
 * @tparam Container1 Type of the tensor from which to choose where condition is
 *                    true.
 * @tparam Container2 Type of the tensor from which to choose where condition is
 *                    false.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Container, class Container1, class Container2, class T,
          size_t Rank>
class where_expr
    : public expression<where_expr<Container, Container1, Container2, T, Rank>,
                        T, Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<
      const where_expr<Container, Container1, Container2, T, Rank>, T, Rank,
      void, T>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const Container &m_cond;

  // True tensor argument.
  const Container1 &m_true;

  // False tensor argument.
  const Container2 &m_false;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  /**
   * @brief Constructs an expression which selects elements from two tensors
   * depending on condition.
   *
   * @param condition Where true, yield @a x, otherwise yield @a y.
   * @param x Values from which to choose where condition is true.
   * @param y Values from which to choose where condition is false.
   */
  where_expr(const expression<Container, bool, Rank> &condition,
             const expression<Container1, T, Rank> &x,
             const expression<Container2, T, Rank> &y)
      : m_cond(condition.self()), m_true(x.self()), m_false(y.self()),
        m_shape(broadcast_shapes(condition.shape(), x.shape(), y.shape())),
        m_size(m_shape.prod()) {}

  /// Destructor.
  ~where_expr() = default;

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
   * @brief Subscript operator. Returns the element at the given position.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The element at the specified position.
   */
  T operator[](const index_type &index) const {
    index_type i, j, k;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (m_cond.shape(axis) > 1) ? index[axis] : 0;
      j[axis] = (m_true.shape(axis) > 1) ? index[axis] : 0;
      k[axis] = (m_false.shape(axis) > 1) ? index[axis] : 0;
    }
    return m_cond[i] ? m_true[j] : m_false[k];
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
  layout_t layout() const { return m_cond.layout(); }
};

/**
 * @brief Partial specialization when the true argument is a tensor and the
 * false argument is a value. Values are broadcasted to an appropriate shape.
 */
template <class Container, class Container1, class T, size_t Rank>
class where_expr<Container, Container1, void, T, Rank>
    : public expression<where_expr<Container, Container1, void, T, Rank>, T,
                        Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const where_expr<Container, Container1, void, T, Rank>,
                        T, Rank, void, T>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const Container &m_cond;

  // True tensor argument.
  const Container1 &m_true;

  // False argument.
  T m_false;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  where_expr(const expression<Container, bool, Rank> &condition,
             const expression<Container1, T, Rank> &x, const T &y)
      : m_cond(condition.self()), m_true(x.self()), m_false(y),
        m_shape(broadcast_shapes(condition.shape(), x.shape())),
        m_size(m_shape.prod()) {}

  /// Destructor.
  ~where_expr() = default;

  /// Iterators.

  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  T operator[](const index_type &index) const {
    index_type i, j;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (m_cond.shape(axis) > 1) ? index[axis] : 0;
      j[axis] = (m_true.shape(axis) > 1) ? index[axis] : 0;
    }
    return m_cond[i] ? m_true[j] : m_false;
  }

  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  size_type size() const { return m_size; }

  layout_t layout() const { return m_cond.layout(); }
};

/**
 * @brief Partial specialization when the true argument is a value and the
 * false argument is a tensor. Values are broadcasted to an appropriate shape.
 */
template <class Container, class Container1, class T, size_t Rank>
class where_expr<Container, void, Container1, T, Rank>
    : public expression<where_expr<Container, void, Container1, T, Rank>, T,
                        Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const where_expr<Container, void, Container1, T, Rank>,
                        T, Rank, void, T>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const Container &m_cond;

  // True argument.
  T m_true;

  // False tensor argument.
  const Container1 &m_false;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  where_expr(const expression<Container, bool, Rank> &condition, const T &x,
             const expression<Container1, T, Rank> &y)
      : m_cond(condition.self()), m_true(x), m_false(y.self()),
        m_shape(broadcast_shapes(condition.shape(), y.shape())),
        m_size(m_shape.prod()) {}

  /// Destructor.
  ~where_expr() = default;

  /// Iterators.

  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  T operator[](const index_type &index) const {
    index_type i, j;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (m_cond.shape(axis) > 1) ? index[axis] : 0;
      j[axis] = (m_false.shape(axis) > 1) ? index[axis] : 0;
    }
    return m_cond[i] ? m_true : m_false[j];
  }

  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  size_type size() const { return m_size; }

  layout_t layout() const { return m_cond.layout(); }
};

/**
 * @brief Partial specialization when both true and false arguments are values.
 * Values are broadcasted to an appropriate shape.
 */
template <class Container, class T, size_t Rank>
class where_expr<Container, void, void, T, Rank>
    : public expression<where_expr<Container, void, void, T, Rank>, T, Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const where_expr<Container, void, void, T, Rank>, T,
                        Rank, void, T>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const Container &m_cond;

  // True argument.
  T m_true;

  // False argument.
  T m_false;

public:
  /// Constructors.

  where_expr(const expression<Container, bool, Rank> &condition, const T &x,
             const T &y)
      : m_cond(condition.self()), m_true(x), m_false(y) {}

  /// Destructor.
  ~where_expr() = default;

  /// Iterators.

  iterator begin() const { return this->begin(this->layout()); }

  iterator begin(layout_t order) const { return iterator(this, 0, order); }

  iterator end() const { return this->end(this->layout()); }

  iterator end(layout_t order) const {
    return iterator(this, this->size(), order);
  }

  /// Indexing.

  T operator[](const index_type &index) const {
    return m_cond[index] ? m_true : m_false;
  }

  shape_type shape() const { return m_cond.shape(); }

  size_type shape(size_type axis) const { return m_cond.shape(axis); }

  size_type size() const { return m_cond.size(); }

  layout_t layout() const { return m_cond.layout(); }
};
} // namespace numcpp

#endif // NUMCPP_LAZY_WHERE_H_INCLUDED
