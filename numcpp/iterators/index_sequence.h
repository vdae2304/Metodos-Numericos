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

/** @file include/numcpp/iterators/index_sequence.h
 *  This header defines an iterator to retrieve indices.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_INDEX_SEQUENCE_H_INCLUDED
#define NUMCPP_INDEX_SEQUENCE_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief An index_sequence is a class that identifies the indices of a tensor.
 *
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <size_t Rank> class index_sequence {
public:
  /// Member types.
  typedef index_t<Rank> value_type;

  class iterator {
  public:
    /// Member types.
    typedef ptrdiff_t difference_type;
    typedef index_t<Rank> value_type;
    typedef std::forward_iterator_tag iterator_category;

    /// Constructors.

    /**
     * @brief Default constructor.
     */
    iterator() : m_shape(), m_index(0), m_order(default_layout) {}

    /**
     * @brief Flat index constructor.
     *
     * @param shape Number of elements along each axis.
     * @param index Flat index into the tensor.
     * @param order Order in which elements are iterated.
     */
    iterator(const shape_t<Rank> &shape, size_t index = 0,
             layout_t order = default_layout)
        : m_shape(shape), m_index(index), m_order(order) {}

    /**
     * @brief Copy constructor.
     */
    iterator(const iterator &other)
        : m_shape(other.m_shape), m_index(other.m_index),
          m_order(other.m_order) {}

    /// Assignment operator.

    /**
     * @brief Copy assignment.
     */
    iterator &operator=(const iterator &other) {
      m_shape = other.m_shape;
      m_index = other.m_index;
      m_order = other.m_order;
      return *this;
    }

    /// Operator overloading.

    /**
     * @brief Pre-increments the iterator by one.
     */
    iterator &operator++() {
      ++m_index;
      return *this;
    }

    /**
     * @brief Post-increments the iterator by one.
     */
    iterator operator++(int) {
      iterator it = *this;
      ++m_index;
      return it;
    }

    /**
     * @brief Return a reference to the current index.
     */
    index_t<Rank> operator*() const {
      return unravel_index(m_index, m_shape, m_order);
    }

    /// Relational operators.

    friend bool operator==(const iterator &lhs, const iterator &rhs) {
      return (lhs.m_index == rhs.m_index);
    }

    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return (lhs.m_index != rhs.m_index);
    }

  private:
    // Number of elements along each axis.
    shape_t<Rank> m_shape;

    // Flat index into the tensor.
    size_t m_index;

    // Order in which elements are iterated.
    layout_t m_order;
  };

  /**
   * @brief Constructor.
   *
   * @param shape Number of elements along each axis. It can be a shape_t object
   *              or the elements of the shape passed as separate arguments.
   * @param order Order in which elements are iterated. Defaults to row-major
   *              order.
   */
  template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...> = 0,
            detail::RequiresIntegral<Sizes...> = 0>
  index_sequence(Sizes... sizes)
      : m_shape(sizes...), m_size(m_shape.prod()), m_order(default_layout) {}

  index_sequence(const shape_t<Rank> &shape, layout_t order = default_layout)
      : m_shape(shape), m_size(shape.prod()), m_order(order) {}

  /**
   * @brief Return a forward iterator to the first index.
   */
  iterator begin() const { return iterator(m_shape, 0, m_order); }

  /**
   * @brief Return a forward iterator to the past the end index.
   */
  iterator end() const { return iterator(m_shape, m_size, m_order); }

  /**
   * @brief Return the number of elements along each axis.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape along all axes.
   */
  const shape_t<Rank> &shape() const { return m_shape; }

  size_t shape(size_t axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements.
   */
  size_t size() const { return m_size; }

  /**
   * @brief Return the memory layout in which indices are computed.
   */
  layout_t layout() const { return m_order; }

private:
  // Number of elements along each axis.
  shape_t<Rank> m_shape;

  // Number of elements.
  size_t m_size;

  // Order in which elements are iterated.
  layout_t m_order;
};

/**
 * @brief Create an index_sequence.
 *
 * @param shape Number of elements along each axis. It can be a shape_t object
 *              or the elements of the shape passed as separate arguments.
 * @param order Order in which elements are iterated. Defaults to row-major
 *              order.
 *
 * @return An index_sequence object which iterates over the indices of a tensor.
 *         At each iteration, a new index is returned.
 */
template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
inline index_sequence<sizeof...(Sizes)> make_index_sequence(Sizes... sizes) {
  return index_sequence<sizeof...(Sizes)>(sizes...);
}

template <size_t Rank>
inline index_sequence<Rank>
make_index_sequence(const shape_t<Rank> &shape,
                    layout_t order = default_layout) {
  return index_sequence<Rank>(shape, order);
}

/**
 * @brief Create an index_sequence.
 *
 * @param a A tensor-like object to iterate over.
 * @param order Order in which elements are iterated. Defaults to the same
 *              layout as of @a a.
 *
 *
 * @return An index_sequence object which iterates over the indices of a tensor.
 *         At each iteration, a new index is returned.
 */
template <class Container, class T, size_t Rank>
inline index_sequence<Rank>
make_index_sequence_for(const expression<Container, T, Rank> &a) {
  return index_sequence<Rank>(a.shape(), a.layout());
}

template <class Container, class T, size_t Rank>
inline index_sequence<Rank>
make_index_sequence_for(const expression<Container, T, Rank> &a,
                        layout_t order) {
  return index_sequence<Rank>(a.shape(), order);
}
} // namespace numcpp

#endif // NUMCPP_INDEX_SEQUENCE_H_INCLUDED
