/*
 * File: include/numcpp/classes/tensor.tcc
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

#ifndef NUMCPP_TENSOR_TCC_INCLUDED
#define NUMCPP_TENSOR_TCC_INCLUDED

#include "numcpp/utilities/assert.h"

namespace numcpp {
/// Constructors.

/**
 * Return the shape of a nested initializer list.
 */
template <class T>
void __initializer_list_shape(const std::initializer_list<T>& il,
                              size_t* shape) {
  *shape = std::max(*shape, il.size());
}

template <class T>
void __initializer_list_shape(
    const std::initializer_list<std::initializer_list<T>>& il, size_t* shape) {
  *shape = std::max(*shape, il.size());
  for (const std::initializer_list<T>& inner : il) {
    __initializer_list_shape(inner, shape + 1);
  }
}

/**
 * Copy elements from an initializer list to an array.
 */
template <class T>
T* __copy_initializer_list(const std::initializer_list<T>& il,
                           T* data, const ptrdiff_t* strides) {
  for (const T& val : il) {
    *data = val;
    data += *strides;
  }
  return data;
}

template <class T, class U>
T* __copy_initializer_list(
    const std::initializer_list<std::initializer_list<U>>& il,
    T* data, const ptrdiff_t* strides) {
  for (const std::initializer_list<U> &inner : il) {
    __copy_initializer_list(inner, data, strides + 1);
    data += *strides;
  }
  return data;
}

template <class T, size_t Rank>
tensor<T, Rank>::tensor(
    typename detail::nested_initializer_list<T, Rank>::type il)
    : m_shape{}, m_size(0), m_data(nullptr), m_layout(layout_right) {
  __initializer_list_shape(il, m_shape.data());
  m_size = m_shape.prod();
  m_data = new T[m_size]();
  index_type strides = make_strides(m_shape, layout_right);
  __copy_initializer_list(il, m_data, strides.data());
}

/// Public methods.

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor<T, Rank>::broadcast_to(
    const shape_type& shape) const {
  index_type strides = make_strides(m_shape, m_layout);
  for (size_t i = 0; i < Rank; ++i) {
    if (m_shape[i] == 1) {
      strides[i] = 0;
    } else if (m_shape[i] != shape[i]) {
      std::ostringstream error;
      error << "operands could not be broadcast together with shape " << m_shape
            << " and requested shape " << shape;
      throw std::invalid_argument(error.str());
    }
  }
  return tensor_view<const T, Rank>(m_data, shape, strides);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<T, N> tensor<T, Rank>::reshape(const shape_t<N> &shape,
                                           layout_t layout) {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  return tensor_view<T, N>(m_data, shape, layout);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<const T, N> tensor<T, Rank>::reshape(const shape_t<N> &shape,
                                                 layout_t layout) const {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  return tensor_view<const T, N>(m_data, shape, layout);
}

template <class T, size_t Rank>
inline void tensor<T, Rank>::resize(const shape_type &shape) {
  if (m_size != shape.prod()) {
    delete[] m_data;
    m_size = shape.prod();
    m_data = new T[m_size];
  }
  m_shape = shape;
}

template <class T, size_t Rank> tensor_view<T, Rank> tensor<T, Rank>::t() {
  shape_type shape = m_shape;
  layout_t layout = (m_layout == layout_left) ? layout_right : layout_left;
  std::reverse(shape.begin(), shape.end());
  return tensor_view<T, Rank>(m_data, shape, layout);
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor<T, Rank>::t() const {
  shape_type shape = m_shape;
  layout_t layout = (m_layout == layout_left) ? layout_right : layout_left;
  std::reverse(shape.begin(), shape.end());
  return tensor_view<const T, Rank>(m_data, shape, layout);
}

template <class T, size_t Rank>
tensor_view<T, Rank> tensor<T, Rank>::t(const shape_type &axes) {
  shape_type shape;
  index_type strides, m_stride = make_strides(m_shape, m_layout);
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] = m_shape[axes[i]];
    strides[i] = m_stride[axes[i]];
  }
  return tensor_view<T, Rank>(m_data, shape, strides);
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor<T, Rank>::t(const shape_type &axes) const {
  shape_type shape;
  index_type strides, m_stride = make_strides(m_shape, m_layout);
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] = m_shape[axes[i]];
    strides[i] = m_stride[axes[i]];
  }
  return tensor_view<const T, Rank>(m_data, shape, strides);
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_TCC_INCLUDED
