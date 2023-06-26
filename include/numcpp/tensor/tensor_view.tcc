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

/** @file include/numcpp/tensor/tensor_view.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_VIEW_TCC_INCLUDED
#define NUMCPP_TENSOR_VIEW_TCC_INCLUDED

#include <algorithm>
#include "numcpp/broadcasting/assert.h"

namespace numcpp {
/// Constructors.

template <class T, size_t Rank>
tensor_view<T, Rank>::tensor_view()
    : m_data(NULL), m_shape(), m_size(0), m_stride(), m_order(default_layout) {}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
tensor_view<T, Rank>::tensor_view(T *data, Sizes... sizes)
    : m_data(data), m_shape(sizes...), m_size(m_shape.prod()),
      m_stride(make_strides(m_shape)), m_order(default_layout) {}

template <class T, size_t Rank>
tensor_view<T, Rank>::tensor_view(T *data, const shape_type &shape,
                                  layout_t order)
    : m_data(data), m_shape(shape), m_size(shape.prod()),
      m_stride(make_strides(shape, order)), m_order(order) {}

template <class T, size_t Rank>
tensor_view<T, Rank>::tensor_view(T *data, const shape_type &shape,
                                  difference_type offset,
                                  const shape_type &strides, layout_t order)
    : m_data(data + offset), m_shape(shape), m_size(shape.prod()),
      m_stride(strides), m_order(order) {}

template <class T, size_t Rank>
tensor_view<T, Rank>::tensor_view(const tensor_view &other)
    : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
      m_stride(other.m_stride), m_order(other.m_order) {}

template <class T, size_t Rank>
tensor_view<T, Rank>::tensor_view(tensor_view &&other)
    : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
      m_stride(other.m_stride), m_order(other.m_order) {
  other.m_data = NULL;
  other.m_shape = shape_type();
  other.m_size = 0;
  other.m_stride = shape_type();
  other.m_order = default_layout;
}

/// Destructor.

template <class T, size_t Rank> tensor_view<T, Rank>::~tensor_view() {}

/// Indexing.

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline T &tensor_view<T, Rank>::operator()(Indices... indices) {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline const T &tensor_view<T, Rank>::operator()(Indices... indices) const {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
T &tensor_view<T, Rank>::operator[](const index_type &index) {
  detail::assert_within_bounds(m_shape, index);
  size_type flat_index = 0;
  for (size_t i = 0; i < Rank; ++i) {
    flat_index += index[i] * m_stride[i];
  }
  return m_data[flat_index];
}

template <class T, size_t Rank>
const T &tensor_view<T, Rank>::operator[](const index_type &index) const {
  detail::assert_within_bounds(m_shape, index);
  size_type flat_index = 0;
  for (size_t i = 0; i < Rank; ++i) {
    flat_index += index[i] * m_stride[i];
  }
  return m_data[flat_index];
}

#ifdef __cpp_multidimensional_subscript
template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline T &tensor_view<T, Rank>::operator[](Indices... indices) {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline const T &tensor_view<T, Rank>::operator[](Indices... indices) const {
  return this->operator[](index_type(indices...));
}
#endif // C++23

template <class T, size_t Rank>
inline const shape_t<Rank> &tensor_view<T, Rank>::shape() const {
  return m_shape;
}

template <class T, size_t Rank>
inline size_t tensor_view<T, Rank>::shape(size_type axis) const {
  return m_shape[axis];
}

template <class T, size_t Rank>
inline size_t tensor_view<T, Rank>::size() const {
  return m_size;
}

template <class T, size_t Rank>
inline bool tensor_view<T, Rank>::empty() const {
  return (m_size == 0);
}

template <class T, size_t Rank> inline T *tensor_view<T, Rank>::data() {
  return m_data;
}

template <class T, size_t Rank>
inline const T *tensor_view<T, Rank>::data() const {
  return m_data;
}

template <class T, size_t Rank>
inline const shape_t<Rank> &tensor_view<T, Rank>::strides() const {
  return m_stride;
}

template <class T, size_t Rank>
inline size_t tensor_view<T, Rank>::strides(size_type axis) const {
  return m_stride[axis];
}

template <class T, size_t Rank>
inline layout_t tensor_view<T, Rank>::layout() const {
  return m_order;
}

template <class T, size_t Rank>
inline bool tensor_view<T, Rank>::is_contiguous() const {
  return (m_stride == make_strides(m_shape, m_order));
}

/// Assignment operator.

template <class T, size_t Rank>
tensor_view<T, Rank> &
tensor_view<T, Rank>::operator=(const tensor_view &other) {
  dense_tensor<tensor_view<T, Rank>, typename std::remove_cv<T>::type,
               Rank>::operator=(other);
  return *this;
}

template <class T, size_t Rank>
template <class Container, class U>
tensor_view<T, Rank> &
tensor_view<T, Rank>::operator=(const expression<Container, U, Rank> &other) {
  dense_tensor<tensor_view<T, Rank>, typename std::remove_cv<T>::type,
               Rank>::operator=(other);
  return *this;
}

template <class T, size_t Rank>
tensor_view<T, Rank> &tensor_view<T, Rank>::operator=(const T &val) {
  dense_tensor<tensor_view<T, Rank>, typename std::remove_cv<T>::type,
               Rank>::operator=(val);
  return *this;
}

template <class T, size_t Rank>
tensor_view<T, Rank> &tensor_view<T, Rank>::operator=(tensor_view &&other) {
  if (this != &other) {
    m_data = other.m_data;
    m_shape = other.m_shape;
    m_size = other.m_size;
    m_stride = other.m_stride;
    m_order = other.m_order;
    other.m_data = NULL;
    other.m_shape = shape_type();
    other.m_size = 0;
    other.m_stride = shape_type();
    other.m_order = default_layout;
  }
  return *this;
}

/// Public methods.

template <class T, size_t Rank>
tensor_view<T, Rank - 1> tensor_view<T, Rank>::diagonal(difference_type k) {
  size_type axis1 = Rank - 2, axis2 = Rank - 1;
  shape_t<Rank - 1> shape = detail::remove_axes(m_shape, axis2);
  shape_t<Rank - 1> strides = detail::remove_axes(m_stride, axis2);
  difference_type offset = 0;
  shape[axis1] = 0;
  strides[axis1] += m_stride[axis2];
  if (k >= 0) {
    if (size_type(k) < m_shape[axis2]) {
      shape[axis1] = std::min(m_shape[axis1], m_shape[axis2] - k);
      offset = k * m_stride[axis2];
    }
  } else {
    if (size_type(-k) < m_shape[axis1]) {
      shape[axis1] = std::min(m_shape[axis1] + k, m_shape[axis2]);
      offset = -k * m_stride[axis1];
    }
  }
  return tensor_view<T, Rank - 1>(m_data, shape, offset, strides, m_order);
}

template <class T, size_t Rank>
tensor_view<const T, Rank - 1>
tensor_view<T, Rank>::diagonal(difference_type k) const {
  size_type axis1 = Rank - 2, axis2 = Rank - 1;
  shape_t<Rank - 1> shape = detail::remove_axes(m_shape, axis2);
  shape_t<Rank - 1> strides = detail::remove_axes(m_stride, axis2);
  difference_type offset = 0;
  shape[axis1] = 0;
  strides[axis1] += m_stride[axis2];
  if (k >= 0) {
    if (size_type(k) < m_shape[axis2]) {
      shape[axis1] = std::min(m_shape[axis1], m_shape[axis2] - k);
      offset = k * m_stride[axis2];
    }
  } else {
    if (size_type(-k) < m_shape[axis1]) {
      shape[axis1] = std::min(m_shape[axis1] + k, m_shape[axis2]);
      offset = -k * m_stride[axis1];
    }
  }
  return tensor_view<const T, Rank - 1>(m_data, shape, offset, strides,
                                        m_order);
}

template <class T, size_t Rank>
inline tensor_view<T, 1> tensor_view<T, Rank>::flatten() {
  if (!this->is_contiguous()) {
    throw std::runtime_error("cannot flatten a non-contiguous view");
  }
  return tensor_view<T, 1>(m_data, m_size);
}

template <class T, size_t Rank>
inline tensor_view<const T, 1> tensor_view<T, Rank>::flatten() const {
  if (!this->is_contiguous()) {
    throw std::runtime_error("cannot flatten a non-contiguous view");
  }
  return tensor_view<const T, 1>(m_data, m_size);
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresIntegral<Sizes...>>
inline tensor_view<T, sizeof...(Sizes)>
tensor_view<T, Rank>::reshape(Sizes... sizes) {
  return this->reshape(make_shape(sizes...));
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresIntegral<Sizes...>>
inline tensor_view<const T, sizeof...(Sizes)>
tensor_view<T, Rank>::reshape(Sizes... sizes) const {
  return this->reshape(make_shape(sizes...));
}

template <class T, size_t Rank>
template <size_t N>
inline tensor_view<T, N>
tensor_view<T, Rank>::reshape(const shape_t<N> &shape) {
  return this->reshape(shape, m_order);
}

template <class T, size_t Rank>
template <size_t N>
inline tensor_view<const T, N>
tensor_view<T, Rank>::reshape(const shape_t<N> &shape) const {
  return this->reshape(shape, m_order);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<T, N> tensor_view<T, Rank>::reshape(const shape_t<N> &shape,
                                                layout_t order) {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  if (!this->is_contiguous()) {
    throw std::runtime_error("cannot reshape a non-contiguous view");
  }
  return tensor_view<T, N>(m_data, shape, order);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<const T, N> tensor_view<T, Rank>::reshape(const shape_t<N> &shape,
                                                      layout_t order) const {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  if (!this->is_contiguous()) {
    throw std::runtime_error("cannot reshape a non-contiguous view");
  }
  return tensor_view<const T, N>(m_data, shape, order);
}

template <class T, size_t Rank>
inline void tensor_view<T, Rank>::swapaxes(size_type axis1, size_type axis2) {
  std::swap(m_shape[axis1], m_shape[axis2]);
  std::swap(m_stride[axis1], m_stride[axis2]);
}

template <class T, size_t Rank>
inline tensor_view<T, Rank> tensor_view<T, Rank>::t() {
  shape_type shape = m_shape;
  shape_type strides = m_stride;
  layout_t order = (m_order == row_major) ? column_major : row_major;
  std::reverse(shape.data(), shape.data() + Rank);
  std::reverse(strides.data(), strides.data() + Rank);
  return tensor_view<T, Rank>(m_data, shape, 0, strides, order);
}

template <class T, size_t Rank>
inline tensor_view<const T, Rank> tensor_view<T, Rank>::t() const {
  shape_type shape = m_shape;
  shape_type strides = m_stride;
  layout_t order = (m_order == row_major) ? column_major : row_major;
  std::reverse(shape.data(), shape.data() + Rank);
  std::reverse(strides.data(), strides.data() + Rank);
  return tensor_view<const T, Rank>(m_data, shape, 0, strides, order);
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_VIEW_TCC_INCLUDED
