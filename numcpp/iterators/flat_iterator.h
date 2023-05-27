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

/** @file include/numcpp/iterators/flat_iterator.h
 *  This header defines a generic iterator for tensor subclasses.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_ITERATOR_H_INCLUDED
#define NUMCPP_TENSOR_ITERATOR_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief A generic random access iterator for tensor subclasses.
 *
 * @tparam Container Tensor subclass.
 * @tparam T Value type.
 * @tparam Rank Dimension of the tensor.
 * @tparam Pointer Pointer type.
 * @tparam Reference Reference type.
 */
template <class Container, class T, size_t Rank, class Pointer = T *,
          class Reference = T &>
class flat_iterator {
public:
  /// Member types.
  typedef typename std::remove_cv<Container>::type container_type;
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef std::random_access_iterator_tag iterator_category;

  /// Constructors.

  /**
   * @brief Default constructor.
   */
  flat_iterator() : m_ptr(NULL), m_index(0), m_order(default_layout) {}

  /**
   * @brief Flat index constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param index Flat index into the tensor. Defaults to 0 (the start of the
   *              tensor).
   * @param order Order in which elements are iterated. Defaults to row-major
   *              order.
   */
  flat_iterator(Container *ptr, size_t index = 0,
                layout_t order = default_layout)
      : m_ptr(ptr), m_index(index), m_order(order) {}

  /**
   * @brief Copy constructor.
   */
  flat_iterator(const flat_iterator &other)
      : m_ptr(other.m_ptr), m_index(other.m_index), m_order(other.m_order) {}

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  flat_iterator &operator=(const flat_iterator &other) {
    m_ptr = other.m_ptr;
    m_index = other.m_index;
    m_order = other.m_order;
    return *this;
  }

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   */
  flat_iterator &operator++() {
    ++m_index;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   */
  flat_iterator &operator--() {
    --m_index;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   */
  flat_iterator operator++(int) {
    flat_iterator it = *this;
    ++m_index;
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   */
  flat_iterator operator--(int) {
    flat_iterator it = *this;
    --m_index;
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   */
  flat_iterator &operator+=(difference_type n) {
    m_index += n;
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   */
  flat_iterator &operator-=(difference_type n) {
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
    flat_iterator it = *this;
    it += n;
    return *it;
  }

  /// Public methods.

  /**
   * @brief Accesses the underlying tensor.
   */
  Container *base() const { return m_ptr; }

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
  // Pointer to the associated tensor subclass.
  Container *m_ptr;

  // Flat index.
  size_t m_index;

  // Layout order iteration.
  layout_t m_order;
};

/// Arithmetic operators for flat_iterator.

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline flat_iterator<Container, T, Rank, Pointer, Reference>
operator+(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
          ptrdiff_t rhs) {
  flat_iterator<Container, T, Rank, Pointer, Reference> it = lhs;
  return it += rhs;
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline flat_iterator<Container, T, Rank, Pointer, Reference>
operator+(ptrdiff_t lhs,
          const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  flat_iterator<Container, T, Rank, Pointer, Reference> it = rhs;
  return it += lhs;
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline flat_iterator<Container, T, Rank, Pointer, Reference>
operator-(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
          ptrdiff_t rhs) {
  flat_iterator<Container, T, Rank, Pointer, Reference> it = lhs;
  return it -= rhs;
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline ptrdiff_t
operator-(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
          const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
}

/// Relational operators for flat_iterator.

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline bool
operator==(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
           const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return lhs.index() == rhs.index();
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline bool
operator!=(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
           const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return lhs.index() != rhs.index();
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline bool
operator<(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
          const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return lhs.index() < rhs.index();
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline bool
operator>(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
          const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return lhs.index() > rhs.index();
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline bool
operator<=(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
           const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return lhs.index() <= rhs.index();
}

template <class Container, class T, size_t Rank, class Pointer, class Reference>
inline bool
operator>=(const flat_iterator<Container, T, Rank, Pointer, Reference> &lhs,
           const flat_iterator<Container, T, Rank, Pointer, Reference> &rhs) {
  return lhs.index() >= rhs.index();
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_ITERATOR_H_INCLUDED
