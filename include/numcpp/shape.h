/*
 * File: include/numcpp/shape.h
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

#ifndef NUMCPP_SHAPE_H_INCLUDED
#define NUMCPP_SHAPE_H_INCLUDED

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iosfwd>
#include "numcpp/enums/layout_t.h"

namespace numcpp {
using std::size_t;
using std::ptrdiff_t;

/**
 * @brief Base class for @ref shape_t and @ref index_t subclasses.
 *
 * @tparam T Value type. It must be an integer type.
 * @tparam Rank Dimension of the shape. It must be a positive integer.
 */
template <class T, size_t Rank>
struct basic_shape {
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef size_t size_type;

  /// No constructors/destructor defined to allow aggregate-initialization.

  /// Public methods.

  /**
   * @brief Return an iterator pointing to the first element in the shape.
   */
  T* begin() { return m_shape; }
  const T* begin() const { return m_shape; }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * shape. It does not point to any element, and thus shall not be
   * dereferenced.
   */
  T* end() { return m_shape + Rank; }
  const T* end() const { return m_shape + Rank; }

  /**
   * @brief Return a const_iterator pointing to the first element in the shape.
   */
  const T* cbegin() const { return m_shape; }

  /**
   * @brief Return a const_iterator pointing to the past-the-end element in the
   * shape.
   */
  const T* cend() const { return m_shape + Rank; }

  /**
   * @brief Return a pointer to the block of memory containing the elements of
   * the shape.
   */
  T* data() { return m_shape; }
  const T* data() const { return m_shape; }

  /**
   * @brief Return the product of the elements of the shape.
   */
  T prod() const {
    T size = 1;
    for (size_t i = 0; i < Rank; ++i) {
      size *= m_shape[i];
    }
    return size;
  }

  /// Operator overloading.

  /**
   * @brief Return the value along the i-th axis.
   *
   * @param i Axis index.
   *
   * @return The value along the i-th axis. If the shape is const-qualified,
   * the function returns a reference to const value_type. Otherwise, it
   * returns a reference to value_type.
   */
  T& operator[](size_type i) { return m_shape[i]; }
  const T& operator[](size_type i) const { return m_shape[i]; }

  // Shape elements.
  T m_shape[Rank];
};

/**
 * @brief A shape_t is a class that identifies the size of a tensor along each
 * dimension.
 */
template <size_t Rank>
using shape_t = basic_shape<size_t, Rank>;

/**
 * @brief An index_t is a class that identifies the position of the elements in
 * a tensor along each dimension.
 */
template <size_t Rank>
using index_t = basic_shape<ptrdiff_t, Rank>;

/**
 * @brief Create a @ref shape_t instance deducing its dimension from the number
 * of arguments.
 *
 * @param sizes... Size along each axis.
 *
 * @return A shape with the given values.
 */
template <std::integral... Sizes>
inline shape_t<sizeof...(Sizes)> make_shape(Sizes... sizes) {
  return shape_t<sizeof...(Sizes)>{static_cast<size_t>(sizes)...};
}

template <std::integral T, size_t Rank>
inline shape_t<Rank> make_shape(const T (&sizes)[Rank]) {
  shape_t<Rank> shape;
  std::copy_n(sizes, Rank, shape.data());
  return shape;
}

/**
 * @brief Create an @ref index_t instance deducing its dimension from the number
 * of arguments.
 *
 * @param indices... Index along each axis.
 *
 * @return An index with the given values.
 */
template <std::integral... Indices>
inline index_t<sizeof...(Indices)> make_index(Indices... indices) {
  return index_t<sizeof...(Indices)>{static_cast<ptrdiff_t>(indices)...};
}

template <std::integral T, size_t Rank>
inline index_t<Rank> make_index(const T (&indices)[Rank]) {
  index_t<Rank> index;
  std::copy_n(indices, Rank, index.data());
  return index;
}

/**
 * @brief Return a tuple of strides to offset a contiguous memory array as a
 * multidimensional array.
 * 
 * @details The elements in the array can be offset by
 *     data[index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
 * where data is the memory array.
 *
 * @param shape The shape of the tensor.
 * @param layout If set to @ref layout_right, the last dimension is contiguous.
 * If set to @ref layout_left, the first dimension is contiguous. Defaults to
 * @ref default_layout.
 *
 * @return The strides for each dimension.
 */
template <size_t Rank>
index_t<Rank> make_strides(const shape_t<Rank>& shape,
                           layout_t layout = default_layout) {
  index_t<Rank> strides;
  size_t size = 1;
  for (size_t i = 0; i < Rank; ++i) {
    size_t k = (layout == layout_left) ? i : Rank - 1 - i;
    strides[k] = size;
    size *= shape[k];
  }
  return strides;
}

/**
 * @brief Converts a tuple of indices into a flat index.
 *
 * @param index A tuple of indices to flatten.
 * @param shape The shape of the tensor used for raveling.
 * @param layout If set to @ref layout_right, the last dimension is contiguous.
 * If set to @ref layout_left, the first dimension is contiguous. Defaults to
 * @ref default_layout.
 *
 * @return The flattened index.
 */
template <size_t Rank>
ptrdiff_t ravel_index(const index_t<Rank>& index, const shape_t<Rank>& shape,
                      layout_t layout = default_layout) {
  ptrdiff_t offset = 0;
  size_t size = 1;
  for (size_t i = 0; i < Rank; ++i) {
    size_t k = (layout == layout_left) ? i : Rank - 1 - i;
    offset += size * index[k];
    size *= shape[k];
  }
  return offset;
}

/**
 * @brief Converts a flat index into a tuple of indices.
 *
 * @param offset Index to unravel.
 * @param shape The shape of the tensor used for unraveling.
 * @param layout If set to @ref layout_right, the last dimension is contiguous.
 * If set to @ref layout_left, the first dimension is contiguous. Defaults to
 * @ref default_layout.
 *
 * @return The unraveled index.
 */
template <size_t Rank>
index_t<Rank> unravel_index(ptrdiff_t offset, const shape_t<Rank>& shape,
                            layout_t layout = default_layout) {
  index_t<Rank> index;
  for (size_t i = 0; i < Rank; ++i) {
    size_t k = (layout == layout_left) ? i : Rank - 1 - i;
    index[k] = offset % shape[k];
    offset /= shape[k];
  }
  return index;
}

/**
 * @brief Broadcast input shapes into a common shape.
 *
 * @details Two dimensions are said to be compatible if
 *   - they are equal or,
 *   - one of them is 1
 * The size of the resulting broadcasting is the size that is not 1 along each
 * axis of the shapes.
 *
 * @param shapes... The shapes to be broadcast against each other. The shapes
 * must have the same dimension.
 *
 * @return Broadcasted shape.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 * cannot be broadcasted according to broadcasting rules.
 */
template <size_t Rank, size_t... Ranks>
shape_t<Rank> broadcast_shapes(const shape_t<Rank> &shape1,
                               const shape_t<Ranks> &...shapes);

/**
 * @brief Constructs a shape as the concatenation of one or more shapes.
 *
 * @param shapes... The shapes to concatenate.
 *
 * @return The concatenated shape.
 */
template <class T, size_t Rank, size_t... Ranks>
basic_shape<T, (Rank + ... + Ranks)> shape_cat(
    const basic_shape<T, Rank>& shape1, const basic_shape<T, Ranks>&... shapes);

/**
 * @brief Compares if two shapes are equal. Returns true if they have the same
 * dimension and the same size along each axis.
 */
template <class T, size_t Rank1, size_t Rank2>
inline bool operator==(const basic_shape<T, Rank1>& shape1,
                       const basic_shape<T, Rank2>& shape2) {
  return (Rank1 == Rank2) &&
         std::equal(shape1.begin(), shape1.end(), shape2.begin());
}

/**
 * @brief Compares if two shapes are not equal. Returns true if they have
 * different dimensions or if they have different sizes along an axis.
 */
template <class T, size_t Rank1, size_t Rank2>
inline bool operator!=(const basic_shape<T, Rank1> &shape1,
                       const basic_shape<T, Rank2> &shape2) {
  return !(shape1 == shape2);
}

/**
 * @brief Reads a shape from input stream. 
 * 
 * @details For a 1-dimensional shape, the supported formats are:
 *   - size
 *   - (size,)
 * For a n-dimensional shape, n > 1, the supported formats are:
 *   - (size_1, size_2, ..., size_n)
 *
 * If an error occurs, calls istr.setstate(std::ios_base::failbit).
 *
 * @param istr Input stream object.
 * @param shape Shape to be extracted from the input stream.
 *
 * @return istr
 */
template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits>& operator>>(
    std::basic_istream<charT, traits> &istr, basic_shape<T, Rank> &shape);

/**
 * @brief Writes to output stream a shape in the form
 * (size_1, size_2, ..., size_n)
 *
 * @param ostr Output stream object.
 * @param shape Shape to be inserted into the output stream.
 *
 * @return ostr
 */
template <class charT, class traits, class T, size_t Rank>
std::basic_ostream<charT, traits>& operator<<(
    std::basic_ostream<charT, traits> &ostr, const basic_shape<T, Rank> &shape);
} // namespace numcpp

#include "numcpp/classes/shape.tcc"

#endif // NUMCPP_SHAPE_H_INCLUDED
