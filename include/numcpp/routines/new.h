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

/** @file include/numcpp/routines/new.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/routines.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_NEW_H_INCLUDED
#define NUMCPP_NEW_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/iterators/flat_iterator.h"

namespace numcpp {
/**
 * @brief A light-weight object which always return the same constant value.
 *
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class T, size_t Rank>
class const_expr : public expression<const_expr<T, Rank>, T, Rank> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef const T *pointer;
  typedef const T &reference;
  typedef flat_iterator<const const_expr<T, Rank>, value_type, rank, pointer,
                        reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Value to return.
  T m_val;

public:
  /// Constructors.

  /**
   * @brief Constructs a const_expr.
   *
   * @param shape Number of elements along each axis.
   * @param val Value to return.
   */
  const_expr(const shape_type &shape, const T &val)
      : m_shape(shape), m_size(m_shape.prod()), m_val(val) {}

  /// Destructor.
  ~const_expr() = default;

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
  iterator begin(layout_t = default_layout) const { return iterator(this, 0); }

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
  iterator end(layout_t = default_layout) const {
    return iterator(this, this->size());
  }

  /// Indexing.

  /**
   * @brief Subscript operator. Returns a constant value.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return A constant value.
   */
  const T &operator[](const index_type &) const { return m_val; }

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
  layout_t layout() const { return default_layout; }
};

/**
 * @brief A light-weight object which stores a sequence of evenly spaced values.
 * This class represents a 1-dimensional expression.
 *
 * @tparam T Type of the elements contained in the tensor.
 */
template <class T>
class sequence_expr : public expression<sequence_expr<T>, T, 1> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = 1;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const sequence_expr<T>, value_type, rank, pointer,
                        reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<1> shape_type;
  typedef index_t<1> index_type;

private:
  // Starting value of the sequence.
  T m_start;

  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Step of the sequence.
  T m_step;

  // Base of the log scale, or NULL if the sequence is in linear scale.
  T *m_base;

public:
  /// Constructors.

  /**
   * @brief Constructs a sequence of evenly spaced values.
   *
   * @param start The starting value of the sequence.
   * @param size Number of values in the sequence.
   * @param step Spacing between values.
   * @param base The base of the log-scale. If not provided, the sequence is in
   *             linear scale.
   */
  sequence_expr(const T &start, size_type size, const T &step)
      : m_start(start), m_shape(size), m_size(size), m_step(step),
        m_base(NULL) {}

  sequence_expr(const T &start, size_type size, const T &step, const T &base)
      : m_start(start), m_shape(size), m_size(size), m_step(step),
        m_base(new T(base)) {}

  /// Destructor.
  ~sequence_expr() { delete m_base; };

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
  iterator begin(layout_t = default_layout) const { return iterator(this, 0); }

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
  iterator end(layout_t = default_layout) const {
    return iterator(this, this->size());
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
    size_type i = index[0];
    T val = m_start + T(i) * m_step;
    return (m_base != NULL) ? std::pow(*m_base, val) : val;
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
  layout_t layout() const { return default_layout; }
};

/**
 * @brief A light-weight object with ones on the diagonal and zeros elsewhere.
 *
 * @tparam T Type of the elements contained in the tensor.
 */
template <class T>
class identity_expr : public expression<identity_expr<T>, T, 2> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = 2;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const identity_expr<T>, value_type, rank, pointer,
                        reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<2> shape_type;
  typedef index_t<2> index_type;

private:
  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Offset from the main diagonal.
  difference_type m_offset;

public:
  /// Constructors.

  /**
   * @brief Constructs a matrix with ones on the diagonal and zeros elsewhere.
   *
   * @param shape Number of rows and columns.
   * @param k Offset of the diagonal from the main diagonal.
   */
  identity_expr(const shape_type &shape, difference_type k = 0)
      : m_shape(shape), m_size(shape.prod()), m_offset(k) {}

  /// Destructor.
  ~identity_expr() = default;

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
  iterator begin(layout_t = default_layout) const { return iterator(this, 0); }

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
  iterator end(layout_t = default_layout) const {
    return iterator(this, this->size());
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
    size_type i = index[0], j = index[1];
    if (m_offset >= 0) {
      return (i + m_offset == j) ? T(1) : T();
    } else {
      return (i == j - m_offset) ? T(1) : T();
    }
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
  layout_t layout() const { return default_layout; }
};

/**
 * @brief A light-weight object with its elements extracted from the diagonal
 * of a tensor. This class represents a readonly view of the elements in the
 * diagonal of another tensor.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor.
 */
template <class Container, class T, size_t Rank>
class diagonal_expr
    : public expression<diagonal_expr<Container, T, Rank>, T, Rank - 1> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank - 1;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const diagonal_expr<Container, T, Rank>, value_type,
                        rank, pointer, reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank - 1> shape_type;
  typedef index_t<Rank - 1> index_type;

private:
  // Elements to extract the diagonal from.
  const Container &m_arg;

  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Offset from the main diagonal.
  difference_type m_offset;

public:
  /// Constructors.

  /**
   * @brief Constructs a tensor with its elements extracted from the diagonal of
   * another tensor.
   *
   * @param a Tensor with the elements to extract the diagonal from.
   * @param k Offset of the diagonal from the main diagonal.
   */
  diagonal_expr(const expression<Container, T, Rank> &a, difference_type k = 0)
      : m_arg(a.self()), m_offset(k) {
    size_type axis1 = Rank - 2, axis2 = Rank - 1;
    m_shape = detail::remove_axes(a.shape(), axis2);
    m_shape[axis1] = 0;
    if (k >= 0) {
      if (size_type(k) < a.shape(axis2)) {
        m_shape[axis1] = std::min(a.shape(axis1), a.shape(axis2) - k);
      }
    } else {
      if (size_type(-k) < a.shape(axis1)) {
        m_shape[axis1] = std::min(a.shape(axis1) + k, a.shape(axis2));
      }
    }
    m_size = m_shape.prod();
  }

  /// Destructor.
  ~diagonal_expr() = default;

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
    size_type axis1 = Rank - 2, axis2 = Rank - 1;
    index_t<Rank> a_index = detail::insert_axes(index, axis2);
    if (m_offset >= 0) {
      a_index[axis1] = index[axis1];
      a_index[axis2] = index[axis1] + m_offset;
    } else {
      a_index[axis1] = index[axis1] - m_offset;
      a_index[axis2] = index[axis1];
    }
    return m_arg[a_index];
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
  layout_t layout() const { return m_arg.layout(); }
};

/**
 * @brief A light-weight object with given values on the diagonal and zeros
 * elsewhere. This class represents a readonly view of the elements of another
 * tensor arranged as a 2-dimensional expression.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 */
template <class Container, class T>
class diagonal_expr<Container, T, 1>
    : public expression<diagonal_expr<Container, T, 1>, T, 2> {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = 2;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const diagonal_expr<Container, T, 1>, value_type, rank,
                        pointer, reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<2> shape_type;
  typedef index_t<2> index_type;

private:
  // Elements on the diagonal.
  const Container &m_arg;

  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Offset from the main diagonal.
  difference_type m_offset;

public:
  /// Constructors.

  /**
   * @brief Constructs a diagonal matrix with given values on the diagonal and
   * zeros elsewhere.
   *
   * @param a 1-dimensional tensor with the elements on the diagonal.
   * @param k Offset of the diagonal from the main diagonal.
   */
  diagonal_expr(const expression<Container, T, 1> &a, difference_type k = 0)
      : m_arg(a.self()), m_offset(k) {
    m_shape[0] = (k >= 0) ? m_arg.size() + k : m_arg.size() - k;
    m_shape[1] = m_shape[0];
    m_size = m_shape.prod();
  }

  /// Destructor.
  ~diagonal_expr() = default;

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
  iterator begin(layout_t = default_layout) const { return iterator(this, 0); }

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
  iterator end(layout_t = default_layout) const {
    return iterator(this, this->size());
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
    size_type i = index[0], j = index[1];
    if (m_offset >= 0) {
      return (i + m_offset == j) ? m_arg[i] : T();
    } else {
      return (i == j - m_offset) ? m_arg[j] : T();
    }
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
  layout_t layout() const { return default_layout; }
};

/**
 * @brief A light-weight object with given values below the diagonal and zeros
 * elsewhere (a lower triangular matrix), or with given values above the
 * diagonal and zeros elsewhere (an upper diagonal matrix). This class
 * represents a readonly view of the elements of another tensor.
 *
 * @tparam Container Type of the tensor whose elements are referenced.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor.
 */
template <class Container, class T, size_t Rank>
class triangular_expr
    : public expression<triangular_expr<Container, T, Rank>, T, Rank> {
public:
  static_assert(Rank >= 2, "Input tensor must be at least 2-dimensional");

  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef void pointer;
  typedef T reference;
  typedef flat_iterator<const triangular_expr<Container, T, Rank>, value_type,
                        rank, pointer, reference>
      iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Elements below/above the diagonal.
  const Container &m_arg;

  // Whether to extract the lower or upper triangle.
  bool m_lower;

  // Offset from the main diagonal.
  difference_type m_offset;

public:
  /// Constructors.

  /**
   * @brief Constructs a triangular matrix with given values below/above the
   * diagonal and zeros elsewhere.
   *
   * @param a Matrix from which to extract the lower/upper triangle.
   * @param lower Whether to extract the lower triangle or the upper triangle.
   * @param k Offset of the diagonal from the main diagonal.
   */
  triangular_expr(const expression<Container, T, Rank> &a, bool lower = true,
                  difference_type k = 0)
      : m_arg(a.self()), m_lower(lower), m_offset(k) {}

  /// Destructor.
  ~triangular_expr() = default;

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
    size_type axis1 = Rank - 2, axis2 = Rank - 1;
    size_type i = index[axis1], j = index[axis2];
    if (m_offset >= 0) {
      if (m_lower) {
        return (j <= i + m_offset) ? m_arg[index] : T();
      } else {
        return (j >= i + m_offset) ? m_arg[index] : T();
      }
    } else {
      if (m_lower) {
        return (j - m_offset <= i) ? m_arg[index] : T();
      } else {
        return (j - m_offset >= i) ? m_arg[index] : T();
      }
    }
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
} // namespace numcpp

#endif // NUMCPP_NEW_H_INCLUDED
