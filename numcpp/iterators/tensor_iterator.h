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

/** @file include/numcpp/iterators/tensor_iterator.h
 *  This header defines a generic iterator for tensor class.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_ITERATOR_H_INCLUDED
#define NUMCPP_TENSOR_ITERATOR_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief A generic random access iterator for base_tensor class.
 *
 * @tparam T Type of the elements contained in the base_tensor.
 * @tparam Rank Dimension of the base_tensor. It must be a positive integer.
 * @tparam Tag Type indicating which specialization of base_tensor refers to.
 */
template <class T, size_t Rank, class Tag> class base_tensor_iterator {
public:
  /// Member types.
  typedef ptrdiff_t difference_type;
  typedef typename base_tensor<T, Rank, Tag>::value_type value_type;
  typedef typename base_tensor<T, Rank, Tag>::pointer pointer;
  typedef typename base_tensor<T, Rank, Tag>::reference reference;
  typedef std::random_access_iterator_tag iterator_category;

  /// Constructors.

  /**
   * @brief Default constructor.
   */
  base_tensor_iterator() : m_ptr(NULL), m_index(0), m_order(default_layout) {}

  /**
   * @brief Flat index constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param index Flat index into the tensor. Defaults to 0 (the start of the
   *              tensor).
   * @param order Order in which elements are iterated. Defaults to row-major
   *              order.
   */
  base_tensor_iterator(base_tensor<T, Rank, Tag> *ptr, size_t index = 0,
                       layout_t order = default_layout)
      : m_ptr(ptr), m_index(index), m_order(order) {}

  /**
   * @brief Copy constructor.
   */
  base_tensor_iterator(const base_tensor_iterator &other)
      : m_ptr(other.m_ptr), m_index(other.m_index), m_order(other.m_order) {}

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  base_tensor_iterator &operator=(const base_tensor_iterator &other) {
    m_ptr = other.m_ptr;
    m_index = other.m_index;
    m_order = other.m_order;
    return *this;
  }

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   */
  base_tensor_iterator &operator++() {
    ++m_index;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   */
  base_tensor_iterator &operator--() {
    --m_index;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   */
  base_tensor_iterator operator++(int) {
    base_tensor_iterator it = *this;
    ++m_index;
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   */
  base_tensor_iterator operator--(int) {
    base_tensor_iterator it = *this;
    --m_index;
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   */
  base_tensor_iterator &operator+=(difference_type n) {
    m_index += n;
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   */
  base_tensor_iterator &operator-=(difference_type n) {
    m_index -= n;
    return *this;
  }

  /**
   * @brief Return a reference to the current element.
   */
  reference operator*() const { return m_ptr->operator[](this->coords()); }

  /**
   * @brief Return a pointer to the current element.
   */
  pointer operator->() const { return &(this->operator*()); }

  /**
   * @brief Return a reference to the element located @a n positions away from
   * the current element.
   */
  reference operator[](difference_type n) const {
    base_tensor_iterator it = *this;
    it += n;
    return *it;
  }

  /// Public methods.

  /**
   * @brief Accesses the underlying tensor.
   */
  base_tensor<T, Rank, Tag> *base() const { return m_ptr; }

  /**
   * @brief Returns the current flat index.
   */
  size_t index() const { return m_index; }

  /**
   * @brief Returns an index_t object with the current coordinates.
   */
  index_t<Rank> coords() const {
    return unravel_index(m_index, m_ptr->shape(), m_order);
  }

  /**
   * @brief Returns the order in which elements are iterated.
   */
  layout_t layout() const { return m_order; }

private:
  // Pointer to the base_tensor associated to the iterator.
  base_tensor<T, Rank, Tag> *m_ptr;

  // Flat index associated to the iterator.
  size_t m_index;

  // Layout iteration.
  layout_t m_order;
};

/// Arithmetic operators for base_tensor_iterator.

template <class T, size_t Rank, class Tag>
inline base_tensor_iterator<T, Rank, Tag>
operator+(const base_tensor_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs) {
  base_tensor_iterator<T, Rank, Tag> it = lhs;
  return it += rhs;
}

template <class T, size_t Rank, class Tag>
inline base_tensor_iterator<T, Rank, Tag>
operator+(ptrdiff_t lhs, const base_tensor_iterator<T, Rank, Tag> &rhs) {
  base_tensor_iterator<T, Rank, Tag> it = rhs;
  return it += lhs;
}

template <class T, size_t Rank, class Tag>
inline base_tensor_iterator<T, Rank, Tag>
operator-(const base_tensor_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs) {
  base_tensor_iterator<T, Rank, Tag> it = lhs;
  return it -= rhs;
}

template <class T, size_t Rank, class Tag>
inline ptrdiff_t operator-(const base_tensor_iterator<T, Rank, Tag> &lhs,
                           const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
}

/// Relational operators for base_tensor_iterator.

template <class T, size_t Rank, class Tag>
inline bool operator==(const base_tensor_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() == rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator!=(const base_tensor_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() != rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator<(const base_tensor_iterator<T, Rank, Tag> &lhs,
                      const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() < rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator>(const base_tensor_iterator<T, Rank, Tag> &lhs,
                      const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() > rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator<=(const base_tensor_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() <= rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator>=(const base_tensor_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() >= rhs.index();
}

/**
 * @brief A generic random access iterator for const-qualified base_tensor
 * class.
 *
 * @tparam T Type of the elements contained in the base_tensor.
 * @tparam Rank Dimension of the base_tensor. It must be a positive integer.
 * @tparam Tag Type indicating which specialization of base_tensor refers to.
 */
template <class T, size_t Rank, class Tag> class base_tensor_const_iterator {
public:
  /// Member types.
  typedef ptrdiff_t difference_type;
  typedef typename base_tensor<T, Rank, Tag>::value_type value_type;
  typedef typename base_tensor<T, Rank, Tag>::const_pointer pointer;
  typedef typename base_tensor<T, Rank, Tag>::const_reference reference;
  typedef std::random_access_iterator_tag iterator_category;

  /// Constructors.

  /**
   * @brief Default constructor.
   */
  base_tensor_const_iterator()
      : m_ptr(NULL), m_index(0), m_order(default_layout) {}

  /**
   * @brief Flat index constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param index Flat index into the tensor. Defaults to 0 (the start of the
   *              tensor).
   * @param order Order in which elements are iterated. Defaults to row-major
   *              order.
   */
  base_tensor_const_iterator(const base_tensor<T, Rank, Tag> *ptr,
                             size_t index = 0, layout_t order = default_layout)
      : m_ptr(ptr), m_index(index), m_order(order) {}

  /**
   * @brief Copy constructor.
   */
  base_tensor_const_iterator(const base_tensor_iterator<T, Rank, Tag> &other)
      : m_ptr(other.base()), m_index(other.index()), m_order(other.layout()) {}

  base_tensor_const_iterator(const base_tensor_const_iterator &other)
      : m_ptr(other.m_ptr), m_index(other.m_index), m_order(other.m_order) {}

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  base_tensor_const_iterator &
  operator=(const base_tensor_iterator<T, Rank, Tag> &other) {
    m_ptr = other.base();
    m_index = other.index();
    m_order = other.layout();
    return *this;
  }

  base_tensor_const_iterator &
  operator=(const base_tensor_const_iterator &other) {
    m_ptr = other.m_ptr;
    m_index = other.m_index;
    m_order = other.m_order;
    return *this;
  }

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   */
  base_tensor_const_iterator &operator++() {
    ++m_index;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   */
  base_tensor_const_iterator &operator--() {
    --m_index;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   */
  base_tensor_const_iterator operator++(int) {
    base_tensor_const_iterator it = *this;
    ++m_index;
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   */
  base_tensor_const_iterator operator--(int) {
    base_tensor_const_iterator it = *this;
    --m_index;
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   */
  base_tensor_const_iterator &operator+=(difference_type n) {
    m_index += n;
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   */
  base_tensor_const_iterator &operator-=(difference_type n) {
    m_index -= n;
    return *this;
  }

  /**
   * @brief Return a reference to the current element.
   */
  reference operator*() const { return m_ptr->operator[](this->coords()); }

  /**
   * @brief Return a pointer to the current element.
   */
  pointer operator->() const { return &(this->operator*()); }

  /**
   * @brief Return a reference to the element located @a n positions away from
   * the current element.
   */
  reference operator[](difference_type n) const {
    base_tensor_const_iterator it = *this;
    it += n;
    return *it;
  }

  /// Public methods.

  /**
   * @brief Accesses the underlying tensor.
   */
  const base_tensor<T, Rank, Tag> *base() const { return m_ptr; }

  /**
   * @brief Returns the current flat index.
   */
  size_t index() const { return m_index; }

  /**
   * @brief Returns an index_t object with the current coordinates.
   */
  index_t<Rank> coords() const {
    return unravel_index(m_index, m_ptr->shape(), m_order);
  }

  /**
   * @brief Returns the order in which elements are iterated.
   */
  layout_t layout() const { return m_order; }

private:
  // Pointer to the base_tensor associated to the iterator.
  const base_tensor<T, Rank, Tag> *m_ptr;

  // Flat index associated to the iterator.
  size_t m_index;

  // Layout iteration.
  layout_t m_order;
};

/// Arithmetic operators for base_tensor_const_iterator.

template <class T, size_t Rank, class Tag>
inline base_tensor_const_iterator<T, Rank, Tag>
operator+(const base_tensor_const_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs) {
  base_tensor_const_iterator<T, Rank, Tag> it = lhs;
  return it += rhs;
}

template <class T, size_t Rank, class Tag>
inline base_tensor_const_iterator<T, Rank, Tag>
operator+(ptrdiff_t lhs, const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  base_tensor_const_iterator<T, Rank, Tag> it = rhs;
  return it += lhs;
}

template <class T, size_t Rank, class Tag>
inline base_tensor_const_iterator<T, Rank, Tag>
operator-(const base_tensor_const_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs) {
  base_tensor_const_iterator<T, Rank, Tag> it = lhs;
  return it -= rhs;
}

template <class T, size_t Rank, class Tag>
inline ptrdiff_t
operator-(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
          const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
}

/// Relational operators for base_tensor_const_iterator.

template <class T, size_t Rank, class Tag>
inline bool operator==(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() == rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator!=(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() != rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator<(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
                      const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() < rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator>(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
                      const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() > rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator<=(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() <= rhs.index();
}

template <class T, size_t Rank, class Tag>
inline bool operator>=(const base_tensor_const_iterator<T, Rank, Tag> &lhs,
                       const base_tensor_const_iterator<T, Rank, Tag> &rhs) {
  return lhs.index() >= rhs.index();
}

/**
 * @brief Constructs a base_tensor_iterator with its templates deduced from the
 * arguments.
 *
 * @param ptr Pointer to the tensor to iterate over.
 * @param index Flat index into the tensor. Defaults to 0 (the start of the
 *              tensor).
 * @param order Order in which elements are iterated. Defaults to row-major
 *              order.
 *
 * @return A random access iterator pointing to an element in the tensor.
 */
template <class T, size_t Rank, class Tag>
base_tensor_iterator<T, Rank, Tag>
make_tensor_iterator(base_tensor<T, Rank, Tag> *ptr, size_t index = 0,
                     layout_t order = default_layout) {
  return base_tensor_iterator<T, Rank, Tag>(ptr, index, order);
}

/**
 * @brief Constructs a base_tensor_const_iterator with its templates deduced
 * from the arguments.
 *
 * @param ptr Pointer to the tensor to iterate over.
 * @param index Flat index into the tensor. Defaults to 0 (the start of the
 *              tensor).
 * @param order Order in which elements are iterated. Defaults to row-major
 *              order.
 *
 * @return A const random access iterator pointing to an element in the tensor.
 */
template <class T, size_t Rank, class Tag>
base_tensor_const_iterator<T, Rank, Tag>
make_tensor_const_iterator(const base_tensor<T, Rank, Tag> *ptr,
                           size_t index = 0, layout_t order = default_layout) {
  return base_tensor_const_iterator<T, Rank, Tag>(ptr, index, order);
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_ITERATOR_H_INCLUDED
