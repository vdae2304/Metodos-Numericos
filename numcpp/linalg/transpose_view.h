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

/** @file include/numcpp/linalg/transpose_view.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/linalg.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TRANSPOSE_VIEW_H_INCLUDED
#define NUMCPP_TRANSPOSE_VIEW_H_INCLUDED

namespace numcpp {
/// Forward declarations.
template <class Tag> struct transpose_tag;
template <class Tag> struct conj_transpose_tag;

/**
 * @brief A light-weight object which stores the elements of a tensor with its
 * axes reversed or permuted. This class represents a view of the elements of
 * another tensor rather than a new tensor.
 *
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 * @tparam Tag Type of the base_tensor container to transpose.
 */
template <class T, size_t Rank, class Tag>
class base_tensor<T, Rank, transpose_tag<Tag>> {
public:
  /// Member types.
  typedef typename base_tensor<T, Rank, Tag>::value_type value_type;
  typedef typename base_tensor<T, Rank, Tag>::const_reference reference;
  typedef typename base_tensor<T, Rank, Tag>::const_reference const_reference;
  typedef typename base_tensor<T, Rank, Tag>::const_pointer pointer;
  typedef typename base_tensor<T, Rank, Tag>::const_pointer const_pointer;
  typedef base_tensor_const_iterator<T, Rank, transpose_tag<Tag>> iterator;
  typedef base_tensor_const_iterator<T, Rank, transpose_tag<Tag>>
      const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Tensor object to transpose.
  const base_tensor<T, Rank, Tag> &m_arg;

  // Permuted shape.
  shape_type m_shape;

  // Permutation to apply.
  shape_type m_axes;

public:
  /// Constructors.

  /**
   * @brief Constructs a view which reverse or permute the axes of a tensor.
   *
   * @param arg Tensor to transpose.
   * @param axes A permutation of (0, 1, ..., Rank - 1). Defaults to
   *             (Rank - 1, ..., 1, 0).
   */
  base_tensor(const base_tensor<T, Rank, Tag> &arg) : m_arg(arg) {
    for (size_t i = 0; i < m_axes.ndim(); ++i) {
      m_axes[i] = m_axes.ndim() - 1 - i;
      m_shape[i] = arg.shape(m_axes[i]);
    }
  }

  base_tensor(const base_tensor<T, Rank, Tag> &arg, const shape_type &axes)
      : m_arg(arg), m_axes(axes) {
    for (size_t i = 0; i < m_axes.ndim(); ++i) {
      m_shape[i] = arg.shape(m_axes[i]);
    }
  }

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
  const_reference operator()(Indices... indices) const {
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
  const_reference operator[](const index_type &index) const {
    index_type out_index;
    for (size_t i = 0; i < index.ndim(); ++i) {
      out_index[i] = index[m_axes[i]];
    }
    return m_arg[out_index];
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
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }

  /// Public methods.

  /**
   * @brief Return a copy of the tensor.
   */
  tensor<value_type, Rank> copy() const {
    return tensor<value_type, Rank>(*this);
  }
};

/**
 * @brief A light-weight object which stores the complex conjugate of the
 * elements in a tensor with its axes reversed or permuted. This class
 * represents a view of the elements of another tensor rather than a new tensor.
 *
 * @tparam T Type of the real and imaginary components in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 * @tparam Tag Type of the base_tensor container to transpose.
 */
template <class T, size_t Rank, class Tag>
class base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag>> {
public:
  /// Member types.
  typedef std::complex<T> value_type;
  typedef std::complex<T> reference;
  typedef std::complex<T> const_reference;
  typedef nullptr_t pointer;
  typedef nullptr_t const_pointer;
  typedef base_tensor_const_iterator<std::complex<T>, Rank,
                                     conj_transpose_tag<Tag>>
      iterator;
  typedef base_tensor_const_iterator<std::complex<T>, Rank,
                                     conj_transpose_tag<Tag>>
      const_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

private:
  // Tensor object to conjugate transpose.
  const base_tensor<std::complex<T>, Rank, Tag> &m_arg;

  // Permuted shape.
  shape_type m_shape;

  // Permutation to apply.
  shape_type m_axes;

public:
  /// Constructors.

  /**
   * @brief Constructs a view which reverse or permute the axes of a tensor.
   *
   * @param arg Tensor to conjugate transpose.
   * @param axes A permutation of (0, 1, ..., Rank - 1). Defaults to
   *             (Rank - 1, ..., 1, 0).
   */
  base_tensor(const base_tensor<std::complex<T>, Rank, Tag> &arg) : m_arg(arg) {
    for (size_t i = 0; i < m_axes.ndim(); ++i) {
      m_axes[i] = m_axes.ndim() - 1 - i;
      m_shape[i] = arg.shape(m_axes[i]);
    }
  }

  base_tensor(const base_tensor<std::complex<T>, Rank, Tag> &arg,
              const shape_type &axes)
      : m_arg(arg), m_axes(axes) {
    for (size_t i = 0; i < m_axes.ndim(); ++i) {
      m_shape[i] = arg.shape(m_axes[i]);
    }
  }

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
    index_type out_index;
    for (size_t i = 0; i < index.ndim(); ++i) {
      out_index[i] = index[m_axes[i]];
    }
    return std::conj(m_arg[out_index]);
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
  size_type size() const { return m_arg.size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_arg.layout(); }

  /// Public methods.

  /**
   * @brief Return a copy of the tensor.
   */
  tensor<value_type, Rank> copy() const {
    return tensor<value_type, Rank>(*this);
  }
};
} // namespace numcpp

#endif // NUMCPP_TRANSPOSE_VIEW_H_INCLUDED
