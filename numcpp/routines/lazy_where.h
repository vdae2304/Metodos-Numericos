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
/// Forward declarations.
template <class TagCond, class TagTrue, class TagFalse> struct lazy_where_tag;

/**
 * @brief A light-weight object which selects elements from two tensor objects
 * depending on condition. This class represents an expression rather than a
 * container. Such expressions relies on short-circuit evaluation, meaning that
 * only one of the two tensors is evaluated at each position.
 *
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 * @tparam TagCond Type of the base_tensor container representing the condition.
 * @tparam TagTrue Type of the base_tensor container from which to choose where
 *                 condition is true.
 * @tparam TagFalse Type of the base_tensor container from which to choose where
 *                  condition is false.
 */
template <class T, size_t Rank, class TagCond, class TagTrue, class TagFalse>
class base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, TagFalse>> {
public:
  /// Member types.
  typedef typename std::remove_cv<T>::type value_type;
  typedef T reference;
  typedef T const_reference;
  typedef nullptr_t pointer;
  typedef nullptr_t const_pointer;
  typedef base_tensor_const_iterator<T, Rank,
                                     lazy_where_tag<TagCond, TagTrue, TagFalse>>
      iterator;
  typedef base_tensor_const_iterator<T, Rank,
                                     lazy_where_tag<TagCond, TagTrue, TagFalse>>
      const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const base_tensor<bool, Rank, TagCond> &m_cond;

  // First tensor argument.
  const base_tensor<T, Rank, TagTrue> &m_true;

  // Second tensor argument.
  const base_tensor<T, Rank, TagFalse> &m_false;

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
  base_tensor(const base_tensor<bool, Rank, TagCond> &condition,
              const base_tensor<T, Rank, TagTrue> &x,
              const base_tensor<T, Rank, TagFalse> &y)
      : m_cond(condition), m_true(x), m_false(y),
        m_shape(broadcast_shapes(condition.shape(), x.shape(), y.shape())),
        m_size(m_shape.prod()) {}

  /// Destructor.
  ~base_tensor() = default;

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
  const_iterator begin() const {
    return const_iterator(this, 0, this->layout());
  }

  const_iterator begin(layout_t order) const {
    return const_iterator(this, 0, order);
  }

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
  const_iterator end() const {
    return const_iterator(this, this->size(), this->layout());
  }

  const_iterator end(layout_t order) const {
    return const_iterator(this, this->size(), order);
  }

  /// Indexing.

  /**
   * @brief Call operator. Returns the element at the given position.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The element at the specified position.
   */
  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  value_type operator()(Indices... indices) const {
    return this->operator[](index_type(indices...));
  }

  /**
   * @brief Subscript operator. Returns the element at the given position.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor.
   *
   * @return The element at the specified position.
   */
  value_type operator[](const index_type &index) const {
    return m_cond[detail::broadcast_index(index, m_cond.shape())]
               ? m_true[detail::broadcast_index(index, m_true.shape())]
               : m_false[detail::broadcast_index(index, m_false.shape())];
  }

  /**
   * @brief Return the dimension of the tensor.
   */
  static constexpr size_type ndim() { return Rank; }

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

  /// Public methods.

  /**
   * @brief Return a copy of the tensor.
   */
  tensor<value_type, Rank> copy() const {
    return tensor<value_type, Rank>(*this);
  }
};

/**
 * @brief Partial specialization when the true argument is a tensor and the
 * false argument is a value. Values are broadcasted to an appropriate shape.
 */
template <class T, size_t Rank, class TagCond, class TagTrue>
class base_tensor<T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag>> {
public:
  /// Member types.
  typedef typename std::remove_cv<T>::type value_type;
  typedef T reference;
  typedef T const_reference;
  typedef nullptr_t pointer;
  typedef nullptr_t const_pointer;
  typedef base_tensor_const_iterator<
      T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag>>
      iterator;
  typedef base_tensor_const_iterator<
      T, Rank, lazy_where_tag<TagCond, TagTrue, scalar_tag>>
      const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const base_tensor<bool, Rank, TagCond> &m_cond;

  // First tensor argument.
  const base_tensor<T, Rank, TagTrue> &m_true;

  // Second argument.
  T m_false;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  base_tensor(const base_tensor<bool, Rank, TagCond> &condition,
              const base_tensor<T, Rank, TagTrue> &x, const T &y)
      : m_cond(condition), m_true(x), m_false(y),
        m_shape(broadcast_shapes(condition.shape(), x.shape())),
        m_size(m_shape.prod()) {}

  /// Destructor.
  ~base_tensor() = default;

  /// Iterators.

  const_iterator begin() const {
    return const_iterator(this, 0, this->layout());
  }

  const_iterator begin(layout_t order) const {
    return const_iterator(this, 0, order);
  }

  const_iterator end() const {
    return const_iterator(this, this->size(), this->layout());
  }

  const_iterator end(layout_t order) const {
    return const_iterator(this, this->size(), order);
  }

  /// Indexing.

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  value_type operator()(Indices... indices) const {
    return this->operator[](index_type(indices...));
  }

  value_type operator[](const index_type &index) const {
    return m_cond[detail::broadcast_index(index, m_cond.shape())]
               ? m_true[detail::broadcast_index(index, m_true.shape())]
               : m_false;
  }

  static constexpr size_type ndim() { return Rank; }

  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  size_type size() const { return m_size; }

  layout_t layout() const { return m_cond.layout(); }

  /// Public methods.

  tensor<value_type, Rank> copy() const {
    return tensor<value_type, Rank>(*this);
  }
};

/**
 * @brief Partial specialization when the true argument is a value and the
 * false argument is a tensor. Values are broadcasted to an appropriate shape.
 */
template <class T, size_t Rank, class TagCond, class TagFalse>
class base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse>> {
public:
  /// Member types.
  typedef typename std::remove_cv<T>::type value_type;
  typedef T reference;
  typedef T const_reference;
  typedef nullptr_t pointer;
  typedef nullptr_t const_pointer;
  typedef base_tensor_const_iterator<
      T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse>>
      iterator;
  typedef base_tensor_const_iterator<
      T, Rank, lazy_where_tag<TagCond, scalar_tag, TagFalse>>
      const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const base_tensor<bool, Rank, TagCond> &m_cond;

  // First tensor argument.
  T m_true;

  // Second argument.
  const base_tensor<T, Rank, TagFalse> &m_false;

  // Common shape.
  shape_type m_shape;

  // Common size.
  size_type m_size;

public:
  /// Constructors.

  base_tensor(const base_tensor<bool, Rank, TagCond> &condition, const T &x,
              const base_tensor<T, Rank, TagFalse> &y)
      : m_cond(condition), m_true(x), m_false(y),
        m_shape(broadcast_shapes(condition.shape(), y.shape())),
        m_size(m_shape.prod()) {}

  /// Destructor.
  ~base_tensor() = default;

  /// Iterators.

  const_iterator begin() const {
    return const_iterator(this, 0, this->layout());
  }

  const_iterator begin(layout_t order) const {
    return const_iterator(this, 0, order);
  }

  const_iterator end() const {
    return const_iterator(this, this->size(), this->layout());
  }

  const_iterator end(layout_t order) const {
    return const_iterator(this, this->size(), order);
  }

  /// Indexing.

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  value_type operator()(Indices... indices) const {
    return this->operator[](index_type(indices...));
  }

  value_type operator[](const index_type &index) const {
    return m_cond[detail::broadcast_index(index, m_cond.shape())]
               ? m_true
               : m_false[detail::broadcast_index(index, m_false.shape())];
  }

  static constexpr size_type ndim() { return Rank; }

  const shape_type &shape() const { return m_shape; }

  size_type shape(size_type axis) const { return m_shape[axis]; }

  size_type size() const { return m_size; }

  layout_t layout() const { return m_cond.layout(); }

  /// Public methods.

  tensor<value_type, Rank> copy() const {
    return tensor<value_type, Rank>(*this);
  }
};

/**
 * @brief Partial specialization when both true and false arguments are values.
 * Values are broadcasted to an appropriate shape.
 */
template <class T, size_t Rank, class TagCond>
class base_tensor<T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag>> {
public:
  /// Member types.
  typedef typename std::remove_cv<T>::type value_type;
  typedef T reference;
  typedef T const_reference;
  typedef nullptr_t pointer;
  typedef nullptr_t const_pointer;
  typedef base_tensor_const_iterator<
      T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag>>
      iterator;
  typedef base_tensor_const_iterator<
      T, Rank, lazy_where_tag<TagCond, scalar_tag, scalar_tag>>
      const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Condition tensor argument.
  const base_tensor<bool, Rank, TagCond> &m_cond;

  // First tensor argument.
  T m_true;

  // Second argument.
  T m_false;

public:
  /// Constructors.

  base_tensor(const base_tensor<bool, Rank, TagCond> &condition, const T &x,
              const T &y)
      : m_cond(condition), m_true(x), m_false(y) {}

  /// Destructor.
  ~base_tensor() = default;

  /// Iterators.

  const_iterator begin() const {
    return const_iterator(this, 0, this->layout());
  }

  const_iterator begin(layout_t order) const {
    return const_iterator(this, 0, order);
  }

  const_iterator end() const {
    return const_iterator(this, this->size(), this->layout());
  }

  const_iterator end(layout_t order) const {
    return const_iterator(this, this->size(), order);
  }

  /// Indexing.

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  value_type operator()(Indices... indices) const {
    return this->operator[](index_type(indices...));
  }

  value_type operator[](const index_type &index) const {
    return m_cond[index] ? m_true : m_false;
  }

  static constexpr size_type ndim() { return Rank; }

  auto shape() const -> decltype(m_cond.shape()) { return m_cond.shape(); }

  size_type shape(size_type axis) const { return m_cond.shape(axis); }

  size_type size() const { return m_cond.size(); }

  layout_t layout() const { return m_cond.layout(); }

  /// Public methods.

  tensor<value_type, Rank> copy() const {
    return tensor<value_type, Rank>(*this);
  }
};
} // namespace numcpp

#endif // NUMCPP_LAZY_WHERE_H_INCLUDED
