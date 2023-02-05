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
 * @brief A random access iterator for base_tensor class obtained by fixing
 * some axes and iterating over the remaining axes.
 *
 * @tparam T Type of the elements contained in the base_tensor.
 * @tparam Rank Dimension of the base_tensor. It must be a positive integer.
 * @tparam Tag Type indicating which specialization of base_tensor refers to.
 * @tparam N Number of axes to fix. Must be less or equal to Rank.
 */
template <class T, size_t Rank, class Tag, size_t N>
class base_tensor_axes_iterator {
public:
  static_assert(N <= Rank, "Cannot fix more axes than the tensor dimension");

  /// Member types.
  typedef base_tensor<T, Rank, Tag> container_type;
  typedef ptrdiff_t difference_type;
  typedef typename container_type::value_type value_type;
  typedef typename container_type::pointer pointer;
  typedef typename container_type::reference reference;
  typedef std::random_access_iterator_tag iterator_category;

  /// Constructors.

  /**
   * @brief Default constructor.
   */
  base_tensor_axes_iterator() : m_ptr(NULL), m_indices(), m_axes(), m_flat(0){};

  /**
   * @brief Flat index constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param indices An index_t object with the indices to fix.
   * @param axes A shape_t object with the axes to iterate over.
   * @param flat Flat index over the iterated axes. Defaults to 0.
   */
  base_tensor_axes_iterator(container_type *ptr, const index_t<Rank> &indices,
                            const shape_t<N> &axes, size_t flat = 0)
      : m_ptr(ptr), m_indices(indices), m_axes(axes), m_flat(flat) {}

  /**
   * @brief Copy constructor.
   */
  base_tensor_axes_iterator(const base_tensor_axes_iterator &other)
      : m_ptr(other.m_ptr), m_indices(other.m_indices), m_axes(other.m_axes),
        m_flat(other.m_flat) {}

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  base_tensor_axes_iterator &operator=(const base_tensor_axes_iterator &other) {
    m_ptr = other.m_ptr;
    m_indices = other.m_indices;
    m_axes = other.m_axes;
    m_flat = other.m_flat;
    return *this;
  }

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   */
  base_tensor_axes_iterator &operator++() {
    ++m_flat;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   */
  base_tensor_axes_iterator &operator--() {
    --m_flat;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   */
  base_tensor_axes_iterator operator++(int) {
    base_tensor_axes_iterator it = *this;
    ++m_flat;
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   */
  base_tensor_axes_iterator operator--(int) {
    base_tensor_axes_iterator it = *this;
    --m_flat;
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   */
  base_tensor_axes_iterator &operator+=(difference_type n) {
    m_flat += n;
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   */
  base_tensor_axes_iterator &operator-=(difference_type n) {
    m_flat -= n;
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
    base_tensor_axes_iterator it = *this;
    it += n;
    return *it;
  }

  /// Public methods.

  /**
   * @brief Accesses the underlying tensor.
   */
  container_type *base() const { return m_ptr; }

  /**
   * @brief Returns the current flat index over the iterated axes.
   */
  size_t index() const { return m_flat; }

  /**
   * @brief Returns an index_t object with the current coordinates.
   */
  index_t<Rank> coords() const {
    index_t<Rank> out_index = m_indices;
    shape_t<N> shape;
    for (size_t i = 0; i < N; ++i) {
      shape[i] = m_ptr->shape(m_axes[i]);
    }
    index_t<N> index = unravel_index(m_flat, shape);
    for (size_t i = 0; i < N; ++i) {
      out_index[m_axes[i]] = index[i];
    }
    return out_index;
  }

  /**
   * @brief Returns a shape_t object with the axes to iterate over.
   */
  const shape_t<N> &axes() const { return m_axes; }

private:
  // Pointer to the base_tensor associated to the iterator.
  container_type *m_ptr;

  // Indices to fix.
  index_t<Rank> m_indices;

  // Axes to iterate over.
  shape_t<N> m_axes;

  // Current flat index over the iterated axes.
  size_t m_flat;
};

/// Arithmetic operators for base_tensor_axes_iterator.

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor_axes_iterator<T, Rank, Tag, N>
operator+(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
          ptrdiff_t rhs) {
  base_tensor_axes_iterator<T, Rank, Tag, N> it = lhs;
  return it += rhs;
}

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor_axes_iterator<T, Rank, Tag, N>
operator+(ptrdiff_t lhs,
          const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  base_tensor_axes_iterator<T, Rank, Tag, N> it = rhs;
  return it += lhs;
}

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor_axes_iterator<T, Rank, Tag, N>
operator-(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
          ptrdiff_t rhs) {
  base_tensor_axes_iterator<T, Rank, Tag, N> it = lhs;
  return it -= rhs;
}

template <class T, size_t Rank, class Tag, size_t N>
inline ptrdiff_t
operator-(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
          const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
}

/// Relational operators for base_tensor_axes_iterator.

template <class T, size_t Rank, class Tag, size_t N>
inline bool operator==(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
                       const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() == rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool operator!=(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
                       const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() != rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool operator<(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
                      const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() < rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool operator>(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
                      const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() > rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool operator<=(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
                       const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() <= rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool operator>=(const base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
                       const base_tensor_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() >= rhs.index();
}

/**
 * @brief A random access iterator for const-qualified base_tensor class
 * obtained by fixing some axes and iterating over the remaining axes.
 *
 * @tparam T Type of the elements contained in the base_tensor.
 * @tparam Rank Dimension of the base_tensor. It must be a positive integer.
 * @tparam Tag Type indicating which specialization of base_tensor refers to.
 * @tparam N Number of axes to fix. Must be less or equal to Rank.
 */
template <class T, size_t Rank, class Tag, size_t N>
class base_tensor_const_axes_iterator {
public:
  static_assert(N <= Rank, "Cannot fix more axes than the tensor dimension");

  /// Member types.
  typedef base_tensor<T, Rank, Tag> container_type;
  typedef ptrdiff_t difference_type;
  typedef typename container_type::value_type value_type;
  typedef typename container_type::const_pointer pointer;
  typedef typename container_type::const_reference reference;
  typedef std::random_access_iterator_tag iterator_category;

  /// Constructors.

  /**
   * @brief Default constructor.
   */
  base_tensor_const_axes_iterator()
      : m_ptr(NULL), m_indices(), m_axes(), m_flat(0) {}

  /**
   * @brief Flat index constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param indices An index_t object with the indices to fix.
   * @param axes A shape_t object with the axes to iterate over.
   * @param flat Flat index over the iterated axes. Defaults to 0.
   */
  base_tensor_const_axes_iterator(const container_type *ptr,
                                  const index_t<Rank> &indices,
                                  const shape_t<N> &axes, size_t flat = 0)
      : m_ptr(ptr), m_indices(indices), m_axes(axes), m_flat(flat) {}

  /**
   * @brief Copy constructor.
   */
  base_tensor_const_axes_iterator(
      const base_tensor_axes_iterator<T, Rank, Tag, N> &other)
      : m_ptr(other.base()), m_indices(other.coords()), m_axes(other.axes()),
        m_flat(other.index()) {}

  base_tensor_const_axes_iterator(const base_tensor_const_axes_iterator &other)
      : m_ptr(other.m_ptr), m_indices(other.m_indices), m_axes(other.m_axes),
        m_flat(other.m_flat) {}

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   */
  base_tensor_const_axes_iterator &
  operator=(const base_tensor_axes_iterator<T, Rank, Tag, N> &other) {
    m_ptr = other.base();
    m_indices = other.coords();
    m_axes = other.axes();
    m_flat = other.index();
    return *this;
  }

  base_tensor_const_axes_iterator &
  operator=(const base_tensor_const_axes_iterator &other) {
    m_ptr = other.m_ptr;
    m_indices = other.m_indices;
    m_axes = other.m_axes;
    m_flat = other.m_flat;
    return *this;
  }

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   */
  base_tensor_const_axes_iterator &operator++() {
    ++m_flat;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   */
  base_tensor_const_axes_iterator &operator--() {
    --m_flat;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   */
  base_tensor_const_axes_iterator operator++(int) {
    base_tensor_const_axes_iterator it = *this;
    ++m_flat;
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   */
  base_tensor_const_axes_iterator operator--(int) {
    base_tensor_const_axes_iterator it = *this;
    --m_flat;
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   */
  base_tensor_const_axes_iterator &operator+=(difference_type n) {
    m_flat += n;
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   */
  base_tensor_const_axes_iterator &operator-=(difference_type n) {
    m_flat -= n;
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
    base_tensor_const_axes_iterator it = *this;
    it += n;
    return *it;
  }

  /// Public methods.

  /**
   * @brief Accesses the underlying tensor.
   */
  const container_type *base() const { return m_ptr; }

  /**
   * @brief Returns the current flat index over the iterated axes.
   */
  size_t index() const { return m_flat; }

  /**
   * @brief Returns an index_t object with the current coordinates.
   */
  index_t<Rank> coords() const {
    index_t<Rank> out_index = m_indices;
    shape_t<N> shape;
    for (size_t i = 0; i < N; ++i) {
      shape[i] = m_ptr->shape(m_axes[i]);
    }
    index_t<N> index = unravel_index(m_flat, shape);
    for (size_t i = 0; i < N; ++i) {
      out_index[m_axes[i]] = index[i];
    }
    return out_index;
  }

  /**
   * @brief Returns a shape_t object with the axes to iterate over.
   */
  const shape_t<N> &axes() const { return m_axes; }

private:
  // Pointer to the base_tensor associated to the iterator.
  const container_type *m_ptr;

  // Indices to fix.
  index_t<Rank> m_indices;

  // Axes to iterate over.
  shape_t<N> m_axes;

  // Current flat index over the iterated axes.
  size_t m_flat;
};

/// Arithmetic operators for base_tensor_const_axes_iterator.

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor_const_axes_iterator<T, Rank, Tag, N>
operator+(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
          ptrdiff_t rhs) {
  base_tensor_const_axes_iterator<T, Rank, Tag, N> it = lhs;
  return it += rhs;
}

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor_const_axes_iterator<T, Rank, Tag, N>
operator+(ptrdiff_t lhs,
          const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  base_tensor_const_axes_iterator<T, Rank, Tag, N> it = rhs;
  return it += lhs;
}

template <class T, size_t Rank, class Tag, size_t N>
inline base_tensor_const_axes_iterator<T, Rank, Tag, N>
operator-(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
          ptrdiff_t rhs) {
  base_tensor_const_axes_iterator<T, Rank, Tag, N> it = lhs;
  return it -= rhs;
}

template <class T, size_t Rank, class Tag, size_t N>
inline ptrdiff_t
operator-(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
          const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() - rhs.index();
}

/// Relational operators for base_tensor_const_axes_iterator.

template <class T, size_t Rank, class Tag, size_t N>
inline bool
operator==(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
           const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() == rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool
operator!=(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
           const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() != rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool
operator<(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
          const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() < rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool
operator>(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
          const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() > rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool
operator<=(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
           const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() <= rhs.index();
}

template <class T, size_t Rank, class Tag, size_t N>
inline bool
operator>=(const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
           const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs) {
  return lhs.index() >= rhs.index();
}

/**
 * @brief Constructs a base_tensor_axes_iterator with its templates deduced from
 * the arguments.
 *
 * @param ptr Pointer to the tensor to iterate over.
 * @param indices An index_t object with the indices to fix.
 * @param axes A shape_t object with the axes to iterate over.
 * @param flat Flat index over the iterated axes. Defaults to 0.
 *
 * @return A random access iterator pointing to an element in the tensor. The
 *         returned iterator iterates only over the axes given by @a axes. The
 *         remaining axes are fixed at the positions given by @a indices.
 */
template <class T, size_t Rank, class Tag, size_t N>
base_tensor_axes_iterator<T, Rank, Tag, N>
make_axes_iterator(base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
                   const shape_t<N> &axes, size_t flat = 0) {
  typedef base_tensor_axes_iterator<T, Rank, Tag, N> iterator;
  return iterator(ptr, indices, axes, flat);
}

template <class T, size_t Rank, class Tag>
base_tensor_axes_iterator<T, Rank, Tag, 1>
make_axes_iterator(base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
                   size_t axis, size_t flat = 0) {
  typedef base_tensor_axes_iterator<T, Rank, Tag, 1> iterator;
  return iterator(ptr, indices, axis, flat);
}

/**
 * @brief Constructs a base_tensor_const_axes_iterator with its templates
 * deduced from the arguments.
 *
 * @param ptr Pointer to the tensor to iterate over.
 * @param indices An index_t object with the indices to fix.
 * @param axes A shape_t object with the axes to iterate over.
 * @param flat Flat index over the iterated axes. Defaults to 0.
 *
 * @return A const random access iterator pointing to an element in the tensor.
 *         The returned iterator iterates only over the axes given by @a axes.
 *         The remaining axes are fixed at the positions given by @a indices.
 */
template <class T, size_t Rank, class Tag, size_t N>
base_tensor_const_axes_iterator<T, Rank, Tag, N>
make_const_axes_iterator(const base_tensor<T, Rank, Tag> *ptr,
                         const index_t<Rank> &indices, const shape_t<N> &axes,
                         size_t flat = 0) {
  typedef base_tensor_const_axes_iterator<T, Rank, Tag, N> const_iterator;
  return const_iterator(ptr, indices, axes, flat);
}

template <class T, size_t Rank, class Tag>
base_tensor_const_axes_iterator<T, Rank, Tag, 1>
make_const_axes_iterator(const base_tensor<T, Rank, Tag> *ptr,
                         const index_t<Rank> &indices, size_t axis,
                         size_t flat = 0) {
  typedef base_tensor_const_axes_iterator<T, Rank, Tag, 1> const_iterator;
  return const_iterator(ptr, indices, axis, flat);
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED
