/*
 * File: include/numcpp/iterators/index_sequence.h
 * Repository: https://github.com/vdae2304/Metodos-Numericos
 * 
 * Copyright (C) 2026 vdae2304
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NUMCPP_INDEX_SEQUENCE_H_INCLUDED
#define NUMCPP_INDEX_SEQUENCE_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief An index_sequence is a class that identifies the indices of a tensor.
 *
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <size_t Rank>
class index_sequence {
 public:
  /// Member types.
  typedef index_t<Rank> value_type;
  typedef index_t<Rank> reference;
  typedef void pointer;
  typedef size_t size_type;

  class iterator {
   public:
    /// Member types.
    typedef ptrdiff_t difference_type;
    typedef typename index_sequence::value_type value_type;
    typedef typename index_sequence::pointer pointer;
    typedef typename index_sequence::reference reference;
    typedef std::input_iterator_tag iterator_category;

    /// Constructors.

    /**
     * @brief Constructor.
     *
     * @param ptr Pointer to index sequence.
     * @param offset Current offset. Defaults to 0.
     */
    iterator(const index_sequence* ptr, difference_type offset = 0)
        : m_ptr(ptr), m_offset(offset) {
      for (size_t i = 0; i < Rank; ++i) {
        size_t axis = m_ptr->m_order[i];
        m_index[axis] = offset % m_ptr->shape(axis);
        offset /= m_ptr->shape(axis);
      }
    }

    /// Operator overloading.

    /**
     * @brief Pre-increments the iterator by one.
     * 
     * @note Time complexity: O(1) amortized. Worst case: O(Rank)
     */
    iterator& operator++() {
      for (size_t i = 0; i < Rank; ++i) {
        size_t axis = m_ptr->m_order[i];
        ++m_index[axis];
        if (m_index[axis] < m_ptr->shape(axis)) break;
        m_index[axis] = 0;
      }
      ++m_offset;
      return *this;
    }

    /**
     * @brief Post-increments the iterator by one.
     * 
     * @note Time complexity: O(1) amortized. Worst case: O(Rank)
     */
    iterator operator++(int) {
      iterator it = *this;
      ++(*this);
      return it;
    }

    /**
     * @brief Return the current index.
     */
    reference operator*() const { return m_index; }

    /// Relational operators.

    friend bool operator==(const iterator& lhs, const iterator& rhs) {
      return (lhs.m_offset == rhs.m_offset);
    }

    friend bool operator!=(const iterator& lhs, const iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    // Pointer to associated index_sequence.
    const index_sequence* m_ptr;

    // Current index.
    value_type m_index;

    // Current offset.
    difference_type m_offset;
  };

  /**
   * @brief Layout constructor.
   *
   * @param shape Number of elements along each axis.
   * @param layout Memory layout in which elements are iterated. If set to
   * @ref layout_right, the last dimension is varying the fastest. If set to
   * @ref layout_left, the first dimension is varying the fastest.
   */
  index_sequence(const shape_t<Rank>& shape, layout_t layout = default_layout)
      : m_shape(shape), m_size(shape.prod()) {
    if (layout == no_layout) layout = default_layout;
    for (size_t i = 0; i < Rank; ++i) {
      m_order[i] = (layout == layout_right) ? Rank - i - 1 : i;
    }
  }

  /**
   * @brief Order constructor.
   *
   * @param shape Number of elements along each axis.
   * @param order A permutation of {0, 1, 2, ..., Rank} specifying the order in
   * which elements shall be iterated, starting from the axis which is varying
   * the fastest.
   */
  index_sequence(const shape_t<Rank>& shape, const index_t<Rank>& order)
      : m_shape(shape), m_size(shape.prod()), m_order(order) {}

  /**
   * @brief Return an input iterator to the first index.
   */
  iterator begin() const { return iterator(this, 0); }

  /**
   * @brief Return an input iterator to the past the end index.
   */
  iterator end() const { return iterator(this, m_size); }

  /**
   * @brief Return the number of elements along each axis.
   */
  const shape_t<Rank>& shape() const { return m_shape; }

  /**
   * @brief Return the number of elements along given axis.
   */
  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements.
   */
  size_type size() const { return m_size; }

 private:
  // Number of elements along each axis.
  shape_t<Rank> m_shape;

  // Number of elements.
  size_type m_size;

  // Order in which elements shall be iterated.
  index_t<Rank> m_order;
};

/**
 * @brief Create an index_sequence.
 *
 * @param shape Number of elements along each axis. It can be a @ref shape_t
 * object or the elements of the shape passed as separate arguments.
 * @param layout Memory layout in which indices are computed. If set to
 * @ref layout_right, the last dimension is contiguous. If set to
 * @ref layout_left, the first dimension is contiguous.
 * @param order A permutation of {0, 1, 2, ..., Rank} specifying the order in
 * which elements shall be iterated, starting from the axis which is varying
 * the fastest.
 *
 * @return An index_sequence object which iterates over the indices of a tensor.
 * At each iteration, a new index is returned.
 */
template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
inline index_sequence<sizeof...(Sizes)> make_index_sequence(Sizes... sizes) {
  return index_sequence<sizeof...(Sizes)>({static_cast<size_t>(sizes)...});
}

template <size_t Rank>
inline index_sequence<Rank> make_index_sequence(
    const shape_t<Rank>& shape, layout_t layout = default_layout) {
  return index_sequence<Rank>(shape, layout);
}

template <size_t Rank>
inline index_sequence<Rank> make_index_sequence(const shape_t<Rank>& shape,
                                                const index_t<Rank> order) {
  return index_sequence<Rank>(shape, order);
}

/**
 * @brief Create an index_sequence for given tensor.
 *
 * @param a An abstract tensor to iterate over.
 * @param layout Memory layout in which indices are computed. If set to
 * @ref layout_right, the last dimension is contiguous. If set to
 * @ref layout_left, the first dimension is contiguous.
 * @param order A permutation of {0, 1, 2, ..., Rank} specifying the order in
 * which elements shall be iterated, starting from the axis which is varying
 * the fastest.
 *
 * @return An index_sequence object which iterates over the indices of a tensor.
 * At each iteration, a new index is returned.
 */
template <class Expr, class T, size_t Rank>
inline index_sequence<Rank> make_index_sequence_for(
    const abstract_tensor<Expr, T, Rank>& a, layout_t layout = default_layout) {
  return index_sequence<Rank>(a.self().shape(), layout);
}

template <class Expr, class T, size_t Rank>
inline index_sequence<Rank> make_index_sequence_for(
    const abstract_tensor<Expr, T, Rank>& a, const index_t<Rank> order) {
  return index_sequence<Rank>(a.self().shape(), order);
}
}  // namespace numcpp

#endif  // NUMCPP_INDEX_SEQUENCE_H_INCLUDED
