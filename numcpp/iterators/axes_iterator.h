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

/** @file include/numcpp/iterators/axes_iterator.h
 *  This header defines an iterator for reduction operations on tensor class.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED
#define NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief A random access iterator for tensor subclasses obtained by fixing
 * some of the axes and iterating over the remaining axes.
 *
 * @tparam Container Tensor subclass.
 * @tparam T Value type.
 * @tparam Rank Dimension of the tensor.
 * @tparam N Number of dimensions to fix.
 * @tparam Pointer Pointer type.
 * @tparam Reference Reference type.
 */
template <class Container, class T, size_t Rank, size_t N, class Pointer = T *,
          class Reference = T &>
class axes_iterator {
public:
  static_assert(N <= Rank, "The number of dimensions to iterate over cannot be"
                           " larger than the tensor dimension");

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
  axes_iterator() : m_ptr(NULL), m_index(), m_axes(), m_offset(0) {}

  /**
   * @brief Flat index constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param index An index_t object with the indices to fix.
   * @param axes A shape_t object with the axes to iterate over.
   * @param offset Flat index over the iterated axes. Defaults to 0.
   */
  axes_iterator(Container *ptr, const index_t<Rank> &index,
                const shape_t<N> &axes, size_t offset = 0)
      : m_ptr(ptr), m_index(index), m_axes(axes), m_offset(offset) {}

  /**
   * @brief Copy constructor.
   */
  axes_iterator(const axes_iterator &other)
      : m_ptr(other.m_ptr), m_index(other.m_index), m_axes(other.m_axes),
        m_offset(other.m_offset) {}

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  axes_iterator &operator=(const axes_iterator &other) {
    m_ptr = other.m_ptr;
    m_index = other.m_index;
    m_axes = other.m_axes;
    m_offset = other.m_offset;
    return *this;
  }

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   */
  axes_iterator &operator++() {
    ++m_offset;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   */
  axes_iterator &operator--() {
    --m_offset;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   */
  axes_iterator operator++(int) {
    axes_iterator it = *this;
    ++m_offset;
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   */
  axes_iterator operator--(int) {
    axes_iterator it = *this;
    --m_offset;
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   */
  axes_iterator &operator+=(difference_type n) {
    m_offset += n;
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   */
  axes_iterator &operator-=(difference_type n) {
    m_offset -= n;
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
    axes_iterator it = *this;
    it += n;
    return *it;
  }

  /// Public methods.

  /**
   * @brief Accesses the underlying tensor.
   */
  Container *base() const { return m_ptr; }

  /**
   * @brief Returns the current flat index over the iterated axes.
   */
  size_t index() const { return m_offset; }

  /**
   * @brief Returns an index_t object with the current coordinates.
   */
  index_t<Rank> coords() const {
    index_t<Rank> out_index = m_index;
    shape_t<N> shape;
    for (size_t i = 0; i < N; ++i) {
      shape[i] = m_ptr->shape(m_axes[i]);
    }
    index_t<N> compressed_index = unravel_index(m_offset, shape);
    for (size_t i = 0; i < N; ++i) {
      out_index[m_axes[i]] = compressed_index[i];
    }
    return out_index;
  }

  /**
   * @brief Returns a shape_t object with the axes to iterate over.
   */
  const shape_t<N> &axes() const { return m_axes; }

private:
  // Pointer to the associated tensor subclass.
  Container *m_ptr;

  // Indices to fix.
  index_t<Rank> m_index;

  // Axes to iterate over.
  shape_t<N> m_axes;

  // Flat index over the iterated axes.
  size_t m_offset;
};

/// Arithmetic operators for axes_iterator.

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline axes_iterator<Container, T, Rank, N, Pointer, Reference>
operator+(const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
          ptrdiff_t rhs) {
  axes_iterator<Container, T, Rank, N, Pointer, Reference> it = lhs;
  return it += rhs;
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline axes_iterator<Container, T, Rank, N, Pointer, Reference>
operator+(ptrdiff_t lhs,
          const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  axes_iterator<Container, T, Rank, N, Pointer, Reference> it = rhs;
  return it += lhs;
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline axes_iterator<Container, T, Rank, N, Pointer, Reference>
operator-(const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
          ptrdiff_t rhs) {
  axes_iterator<Container, T, Rank, N, Pointer, Reference> it = lhs;
  return it -= rhs;
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline ptrdiff_t
operator-(const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
          const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
}

/// Relational operators for axes_iterator.

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline bool operator==(
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return lhs.index() == rhs.index();
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline bool operator!=(
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return lhs.index() != rhs.index();
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline bool
operator<(const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
          const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return lhs.index() < rhs.index();
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline bool
operator>(const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
          const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return lhs.index() > rhs.index();
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline bool operator<=(
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return lhs.index() <= rhs.index();
}

template <class Container, class T, size_t Rank, size_t N, class Pointer,
          class Reference>
inline bool operator>=(
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &lhs,
    const axes_iterator<Container, T, Rank, N, Pointer, Reference> &rhs) {
  return lhs.index() >= rhs.index();
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED
