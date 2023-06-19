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

/** @file include/numcpp/iterators/nested_index_sequence.h
 *  This header defines an iterator to retrieve nested indices.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_NESTED_INDEX_SEQUENCE_H_INCLUDED
#define NUMCPP_NESTED_INDEX_SEQUENCE_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief A nested_index_sequence is a class that identifies nested indices of a
 * tensor.
 *
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 * @tparam N Number of dimensions to iterate over.
 */
template <size_t Rank, size_t N> class nested_index_sequence {
  static_assert(N <= Rank, "The number of dimensions to iterate over cannot be"
                           " larger than the tensor dimension");

public:
  /// Member types.
  typedef index_t<Rank> value_type;

  class iterator {
  public:
    /// Member types.
    typedef ptrdiff_t difference_type;
    typedef index_t<Rank> value_type;
    typedef void pointer;
    typedef index_t<Rank> reference;
    typedef std::forward_iterator_tag iterator_category;

    /// Constructors.

    /**
     * @brief Default constructor.
     */
    iterator() : m_index(), m_shape(), m_axes(), m_offset(0) {}

    /**
     * @brief Flat index constructor.
     *
     * @param index Indices to fix.
     * @param shape Number of elements along the iterated axes.
     * @param axes Axes to iterate over.
     * @param offset Flat index over the iterated axes.
     */
    iterator(const index_t<Rank> &index, const shape_t<N> &shape,
             const shape_t<N> &axes, size_t offset = 0)
        : m_index(index), m_shape(shape), m_axes(axes), m_offset(offset) {}

    /**
     * @brief Copy constructor.
     */
    iterator(const iterator &other)
        : m_index(other.m_index), m_shape(other.m_shape), m_axes(other.m_axes),
          m_offset(other.m_offset) {}

    /// Assignment operator.

    /**
     * @brief Copy assignment.
     */
    iterator &operator=(const iterator &other) {
      m_index = other.m_index;
      m_shape = other.m_shape;
      m_axes = other.m_axes;
      m_offset = other.m_offset;
      return *this;
    }

    /// Operator overloading.

    /**
     * @brief Pre-increments the iterator by one.
     */
    iterator &operator++() {
      ++m_offset;
      return *this;
    }

    /**
     * @brief Post-increments the iterator by one.
     */
    iterator operator++(int) {
      iterator it = *this;
      ++m_offset;
      return it;
    }

    /**
     * @brief Return a reference to the current index.
     */
    index_t<Rank> operator*() const {
      index_t<Rank> out_index = m_index;
      size_t flat_index = m_offset;
      for (size_t i = 0; i < N; ++i) {
        size_t axis = m_axes[N - 1 - i];
        out_index[axis] = flat_index % m_shape[N - 1 - i];
        flat_index /= m_shape[N - 1 -i];
      }
      return out_index;
    }

    /// Relational operators.

    friend bool operator==(const iterator &lhs, const iterator &rhs) {
      return (lhs.m_offset == rhs.m_offset);
    }

    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return (lhs.m_offset != rhs.m_offset);
    }

  private:
    // Indices to fix.
    index_t<Rank> m_index;

    // Number of elements along the iterated axes.
    shape_t<N> m_shape;

    // Axes to iterate over.
    shape_t<N> m_axes;

    // Flat index over the iterated axes.
    size_t m_offset;
  };

  /**
   * @brief Constructor.
   *
   * @param shape Number of elements along each axis.
   * @param index An index_t object with the indices to fix.
   * @param axes A shape_t object with the axes to iterate over.
   */
  nested_index_sequence(const shape_t<Rank> &shape, const index_t<Rank> &index,
                        const shape_t<N> &axes)
      : m_index(index), m_axes(axes), m_size(1) {
    for (size_t i = 0; i < N; ++i) {
      m_shape[i] = shape[m_axes[i]];
      m_size *= m_shape[i];
    }
  }

  /**
   * @brief Return a forward iterator to the first index.
   */
  iterator begin() const { return iterator(m_index, m_shape, m_axes, 0); }

  /**
   * @brief Return a forward iterator to the past the end index.
   */
  iterator end() const { return iterator(m_index, m_shape, m_axes, m_size); }

  /**
   * @brief Return the number of elements along the iterated axes.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape along all axes.
   */
  const shape_t<N> &shape() const { return m_shape; }

  size_t shape(size_t axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements.
   */
  size_t size() const { return m_size; }

private:
  // Indices to fix.
  index_t<Rank> m_index;

  // Number of elements along the iterated axes.
  shape_t<N> m_shape;

  // Axes to iterate over.
  shape_t<N> m_axes;

  // Number of elements.
  size_t m_size;
};

/**
 * @brief Create a nested_index_sequence object deducing its templates from the
 * arguments.
 *
 * @param shape Number of elements along each axis.
 * @param a A tensor-like object to iterate over its indices.
 * @param index An index_t object with the indices to fix.
 * @param axes A shape_t object with the axes to iterate over.
 *
 * @return A nested_index_sequence object which iterates over the indices of a
 *         tensor along the specified axes. At each iteration, a new index is
 *         returned.
 */
template <size_t Rank, size_t N>
inline nested_index_sequence<Rank, N>
make_nested_index_sequence(const shape_t<Rank> &shape,
                           const index_t<Rank> &index, const shape_t<N> &axes) {
  return nested_index_sequence<Rank, N>(shape, index, axes);
}

template <size_t Rank>
inline nested_index_sequence<Rank, 1>
make_nested_index_sequence(const shape_t<Rank> &shape,
                           const index_t<Rank> &index, size_t axis) {
  return nested_index_sequence<Rank, 1>(shape, index, axis);
}

template <class Container, class T, size_t Rank, size_t N>
inline nested_index_sequence<Rank, N>
make_nested_index_sequence_for(const expression<Container, T, Rank> &a,
                               const index_t<Rank> &index,
                               const shape_t<N> &axes) {
  return nested_index_sequence<Rank, N>(a.shape(), index, axes);
}

template <class Container, class T, size_t Rank>
inline nested_index_sequence<Rank, 1>
make_nested_index_sequence_for(const expression<Container, T, Rank> &a,
                               const index_t<Rank> &index, size_t axis) {
  return nested_index_sequence<Rank, 1>(a.shape(), index, axis);
}
} // namespace numcpp

#endif // NUMCPP_NESTED_INDEX_SEQUENCE_H_INCLUDED
