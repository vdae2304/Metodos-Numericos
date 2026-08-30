/*
 * File: include/numcpp/iterators/flat_iterator.h
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

#ifndef NUMCPP_FLAT_ITERATOR_H_INCLUDED
#define NUMCPP_FLAT_ITERATOR_H_INCLUDED

#include <iterator>

namespace numcpp {
/**
 * @brief A generic random access iterator for tensor subclasses.
 *
 * @tparam Container Tensor subclass.
 * @tparam Reference Reference type.
 * @tparam Pointer Pointer type.
 */
template <class Container, class Reference, class Pointer = void>
class flat_iterator {
 public:
  /// Member types.
  typedef ptrdiff_t difference_type;
  typedef typename Container::value_type value_type;
  static constexpr size_t rank = Container::rank;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef std::random_access_iterator_tag iterator_category;

  /// Constructors.

  /**
   * @brief Default constructor.
   */
  flat_iterator()
      : m_ptr(nullptr), m_index{}, m_offset(0), m_layout(default_layout) {}

  /**
   * @brief Constructor.
   *
   * @param ptr Pointer to the tensor to iterate over.
   * @param offset Offset from beginning. Defaults to 0.
   * @param layout Layout in which elements are iterated. Defaults to
   * @ref default_layout.
   */
  flat_iterator(Container* ptr, difference_type offset = 0,
                layout_t layout = default_layout)
      : m_ptr(ptr),
        m_index(unravel_index(offset, ptr->shape(), layout)),
        m_offset(offset),
        m_layout(layout) {}

  /// Operator overloading.

  /**
   * @brief Pre-increments the iterator by one.
   * 
   * @note Time complexity: O(1) amortized. Worst case: O(rank)
   */
  flat_iterator& operator++() {
    for (size_t i = 0; i < rank; ++i) {
      size_t axis = (m_layout == layout_right) ? rank - 1 - i : i;
      ++m_index[axis];
      if (m_index[axis] < m_ptr->shape(axis)) break;
      m_index[axis] = 0;
    }
    ++m_offset;
    return *this;
  }

  /**
   * @brief Pre-decrements the iterator by one.
   * 
   * @note Time complexity: O(1) amortized. Worst case: O(rank)
   */
  flat_iterator& operator--() {
    for (size_t i = 0; i < rank; ++i) {
      size_t axis = (m_layout == layout_right) ? rank - 1 - i : i;
      if (m_index[axis] > 0) {
        --m_index[axis];
        break;
      }
      m_index[axis] = m_ptr->shape(axis) - 1;
    }
    --m_offset;
    return *this;
  }

  /**
   * @brief Post-increments the iterator by one.
   * 
   * @note Time complexity: O(1) amortized. Worst case: O(rank)
   */
  flat_iterator operator++(int) {
    flat_iterator it = *this;
    ++(*this);
    return it;
  }

  /**
   * @brief Post-decrements the iterator by one.
   * 
   * @note Time complexity: O(1) amortized. Worst case: O(rank)
   */
  flat_iterator operator--(int) {
    flat_iterator it = *this;
    --(*this);
    return it;
  }

  /**
   * @brief Advances the iterator by @a n.
   * 
   * @note Time complexity: O(rank) 
   */
  flat_iterator &operator+=(difference_type n) {
    if (n == 1) return ++(*this);
    if (n == -1) return --(*this);
    m_offset += n;
    m_index = unravel_index(m_offset, m_ptr->shape(), m_layout);
    return *this;
  }

  /**
   * @brief Advances the iterator by @a -n.
   * 
   * @note Time complexity: O(rank)
   */
  flat_iterator &operator-=(difference_type n) {
    return (*this) += -n;
  }

  /**
   * @brief Return a reference to the current element.
   */
  reference operator*() const { return (*m_ptr)[m_index]; }

  /**
   * @brief Return a pointer to the current element.
   */
  pointer operator->() const { return &(**this); }

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
  difference_type index() const { return m_offset; }

  /**
   * @brief Returns an index_t object with the current coordinates.
   */
  const index_t<rank>& coords() const { return m_index; }

  /**
   * @brief Returns the order in which elements are iterated.
   */
  layout_t layout() const { return m_layout; }

private:
  // Pointer to the tensor subclass.
  Container *m_ptr;

  // Current index.
  index_t<rank> m_index;

  // Offset from beginning.
  difference_type m_offset;

  // Layout in which elements are iterated.
  layout_t m_layout;
};

/// Arithmetic operators for flat_iterator.

template <class Container, class Reference, class Pointer>
inline flat_iterator<Container, Reference, Pointer> operator+(
    const flat_iterator<Container, Reference, Pointer>& lhs, ptrdiff_t rhs) {
  flat_iterator<Container, Reference, Pointer> it = lhs;
  return it += rhs;
}

template <class Container, class Reference, class Pointer>
inline flat_iterator<Container, Reference, Pointer> operator+(
    ptrdiff_t lhs, const flat_iterator<Container, Reference, Pointer>& rhs) {
  flat_iterator<Container, Reference, Pointer> it = rhs;
  return it += lhs;
}

template <class Container, class Reference, class Pointer>
inline flat_iterator<Container, Reference, Pointer> operator-(
    const flat_iterator<Container, Reference, Pointer>& lhs, ptrdiff_t rhs) {
  flat_iterator<Container, Reference, Pointer> it = lhs;
  return it -= rhs;
}

template <class Container, class Reference, class Pointer>
inline ptrdiff_t operator-(
    const flat_iterator<Container, Reference, Pointer>& lhs,
    const flat_iterator<Container, Reference, Pointer>& rhs) {
  return lhs.index() - rhs.index();
}

/// Relational operators for flat_iterator.

template <class Container, class Reference, class Pointer>
inline bool operator==(
    const flat_iterator<Container, Reference, Pointer>& lhs,
    const flat_iterator<Container, Reference, Pointer>& rhs) {
  return lhs.index() == rhs.index();
}

template <class Container, class Reference, class Pointer>
inline bool operator!=(
    const flat_iterator<Container, Reference, Pointer>& lhs,
    const flat_iterator<Container, Reference, Pointer>& rhs) {
  return !(lhs == rhs);
}

template <class Container, class Reference, class Pointer>
inline bool operator<(const flat_iterator<Container, Reference, Pointer>& lhs,
                      const flat_iterator<Container, Reference, Pointer>& rhs) {
  return lhs.index() < rhs.index();
}

template <class Container, class Reference, class Pointer>
inline bool operator>(const flat_iterator<Container, Reference, Pointer>& lhs,
                      const flat_iterator<Container, Reference, Pointer>& rhs) {
  return (rhs < lhs);
}

template <class Container, class Reference, class Pointer>
inline bool operator<=(
    const flat_iterator<Container, Reference, Pointer>& lhs,
    const flat_iterator<Container, Reference, Pointer>& rhs) {
  return !(rhs < lhs);
}

template <class Container, class Reference, class Pointer>
inline bool operator>=(
    const flat_iterator<Container, Reference, Pointer>& lhs,
    const flat_iterator<Container, Reference, Pointer>& rhs) {
  return !(lhs < rhs);
}
}  // namespace numcpp

#endif // NUMCPP_FLAT_ITERATOR_H_INCLUDED
