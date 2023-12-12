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

/** @file include/numcpp/tensor/tensor.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_TCC_INCLUDED
#define NUMCPP_TENSOR_TCC_INCLUDED

#include <algorithm>
#include "numcpp/broadcasting/assert.h"

namespace numcpp {
/// Constructors.

template <class T, size_t Rank>
tensor<T, Rank>::tensor()
    : m_data(NULL), m_shape(), m_size(0), m_order(default_layout) {}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
tensor<T, Rank>::tensor(Sizes... sizes)
    : m_shape(sizes...), m_size(m_shape.prod()), m_order(default_layout) {
  m_data = new T[m_size];
}

template <class T, size_t Rank>
tensor<T, Rank>::tensor(const shape_type &shape, layout_t order)
    : m_shape(shape), m_size(shape.prod()), m_order(order) {
  m_data = new T[m_size];
}

template <class T, size_t Rank>
tensor<T, Rank>::tensor(const shape_type &shape, const T &val, layout_t order)
    : m_shape(shape), m_size(shape.prod()), m_order(order) {
  m_data = new T[m_size];
  std::fill_n(m_data, m_size, val);
}

template <class T, size_t Rank>
template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator>,
          detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
tensor<T, Rank>::tensor(InputIterator first, Sizes... sizes)
    : m_shape(sizes...), m_size(m_shape.prod()), m_order(default_layout) {
  m_data = new T[m_size];
  std::copy_n(first, m_size, m_data);
}

template <class T, size_t Rank>
template <class InputIterator, detail::RequiresInputIterator<InputIterator>>
tensor<T, Rank>::tensor(InputIterator first, const shape_type &shape,
                        layout_t order)
    : m_shape(shape), m_size(shape.prod()), m_order(order) {
  m_data = new T[m_size];
  std::copy_n(first, m_size, m_data);
}

template <class T, size_t Rank>
tensor<T, Rank>::tensor(const tensor &other)
    : m_shape(other.m_shape), m_size(other.m_size), m_order(other.m_order) {
  m_data = new T[m_size];
  std::copy_n(other.m_data, m_size, m_data);
}

template <class T, size_t Rank>
template <class Container, class U>
tensor<T, Rank>::tensor(const expression<Container, U, Rank> &other)
    : m_shape(other.shape()), m_size(other.size()), m_order(other.layout()) {
  m_data = new T[m_size];
  dense_tensor<tensor<T, Rank>, T, Rank>::operator=(other);
}

template <class T, size_t Rank>
template <class Container, class U>
tensor<T, Rank>::tensor(const expression<Container, U, Rank> &other,
                        layout_t order)
    : m_shape(other.shape()), m_size(other.size()), m_order(order) {
  m_data = new T[m_size];
  dense_tensor<tensor<T, Rank>, T, Rank>::operator=(other);
}

template <class T, size_t Rank>
tensor<T, Rank>::tensor(tensor &&other)
    : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
      m_order(other.m_order) {
  other.m_data = NULL;
  other.m_shape = shape_type();
  other.m_size = 0;
  other.m_order = default_layout;
}

template <class T, size_t Rank>
template <size_t Depth>
typename std::enable_if<(Depth == 1)>::type
tensor<T, Rank>::__initializer_list_shape(std::initializer_list<T> il,
                                          shape_type &shape) {
  shape[Rank - 1] = std::max(shape[Rank - 1], il.size());
}

template <class T, size_t Rank>
template <size_t Depth>
typename std::enable_if<(Depth > 1)>::type
tensor<T, Rank>::__initializer_list_shape(
    detail::nested_initializer_list_t<T, Depth> il, shape_type &shape) {
  shape[Rank - Depth] = std::max(shape[Rank - Depth], il.size());
  for (detail::nested_initializer_list_t<T, Depth - 1> inner_il : il) {
    __initializer_list_shape<Depth - 1>(inner_il, shape);
  }
}

template <class T, size_t Rank>
template <size_t Depth>
typename std::enable_if<(Depth == 1)>::type
tensor<T, Rank>::__fill_from_initializer_list(std::initializer_list<T> il,
                                              index_type &index) {
  index[Rank - 1] = 0;
  for (T val : il) {
    this->operator[](index) = val;
    ++index[Rank - 1];
  }
}

template <class T, size_t Rank>
template <size_t Depth>
typename std::enable_if<(Depth > 1)>::type
tensor<T, Rank>::__fill_from_initializer_list(
    detail::nested_initializer_list_t<T, Depth> il, index_type &index) {
  index[Rank - Depth] = 0;
  for (detail::nested_initializer_list_t<T, Depth - 1> inner_il : il) {
    __fill_from_initializer_list<Depth - 1>(inner_il, index);
    ++index[Rank - Depth];
  }
}

template <class T, size_t Rank>
tensor<T, Rank>::tensor(detail::nested_initializer_list_t<T, Rank> il)
    : m_shape(), m_size(0), m_order(row_major) {
  __initializer_list_shape<Rank>(il, m_shape);
  m_size = m_shape.prod();
  m_data = new T[m_size]();
  index_type index;
  __fill_from_initializer_list<Rank>(il, index);
}

/// Destructor.

template <class T, size_t Rank> tensor<T, Rank>::~tensor() { delete[] m_data; }

/// Indexing.

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline T &tensor<T, Rank>::operator()(Indices... indices) {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline const T &tensor<T, Rank>::operator()(Indices... indices) const {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
inline T &tensor<T, Rank>::operator[](const index_type &index) {
  detail::assert_within_bounds(m_shape, index);
  return m_data[ravel_index(index, m_shape, m_order)];
}

template <class T, size_t Rank>
inline const T &tensor<T, Rank>::operator[](const index_type &index) const {
  detail::assert_within_bounds(m_shape, index);
  return m_data[ravel_index(index, m_shape, m_order)];
}

#ifdef __cpp_multidimensional_subscript
template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline T &tensor<T, Rank>::operator[](Indices... indices) {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline const T &tensor<T, Rank>::operator[](Indices... indices) const {
  return this->operator[](index_type(indices...));
}
#endif // C++23

template <class T, size_t Rank>
template <size_t N>
void tensor<T, Rank>::__unpack_slices(size_t &, shape_t<N> &, ptrdiff_t &,
                                      shape_t<N> &) const {}

template <class T, size_t Rank>
template <size_t N, class... Indices>
void tensor<T, Rank>::__unpack_slices(size_t &size, shape_t<N> &shape,
                                      ptrdiff_t &offset, shape_t<N> &strides,
                                      size_t i, Indices... indices) const {
  size_t axis = Rank - 1 - sizeof...(Indices);
  detail::assert_within_bounds(m_shape[axis], i, axis);
  if (m_order == row_major) {
    __unpack_slices(size, shape, offset, strides, indices...);
  }
  offset += i * size;
  size *= m_shape[axis];
  if (m_order == column_major) {
    __unpack_slices(size, shape, offset, strides, indices...);
  }
}

template <class T, size_t Rank>
template <size_t N, class... Indices>
void tensor<T, Rank>::__unpack_slices(size_t &size, shape_t<N> &shape,
                                      ptrdiff_t &offset, shape_t<N> &strides,
                                      slice slc, Indices... indices) const {
  size_t axis = Rank - 1 - sizeof...(Indices);
  if (slc == slice()) {
    slc = slice(m_shape[axis]);
  } else if (slc.size() > 0 && slc.last() >= m_shape[axis]) {
    slc = slice(slc.start(), m_shape[axis], slc.stride());
  }
  size_t I = N - 1 - detail::slicing_rank<Indices...>::value;
  if (m_order == row_major) {
    __unpack_slices(size, shape, offset, strides, indices...);
  }
  shape[I] = slc.size();
  offset += slc.start() * size;
  strides[I] = slc.stride() * size;
  size *= m_shape[axis];
  if (m_order == column_major) {
    __unpack_slices(size, shape, offset, strides, indices...);
  }
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresSlicing<Indices...>>
tensor_view<T, detail::slicing_rank<Indices...>::value>
tensor<T, Rank>::operator()(Indices... indices) {
  constexpr size_t N = detail::slicing_rank<Indices...>::value;
  size_type size = 1;
  difference_type offset = 0;
  shape_t<N> shape, strides;
  __unpack_slices(size, shape, offset, strides, indices...);
  return tensor_view<T, N>(m_data, shape, offset, strides, m_order);
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresSlicing<Indices...>>
tensor_view<const T, detail::slicing_rank<Indices...>::value>
tensor<T, Rank>::operator()(Indices... indices) const {
  constexpr size_t N = detail::slicing_rank<Indices...>::value;
  size_type size = 1;
  difference_type offset = 0;
  shape_t<N> shape, strides;
  __unpack_slices(size, shape, offset, strides, indices...);
  return tensor_view<const T, N>(m_data, shape, offset, strides, m_order);
}

#ifdef __cpp_multidimensional_subscript
template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresSlicing<Indices...>>
inline tensor_view<T, detail::slicing_rank<Indices...>::value>
tensor<T, Rank>::operator[](Indices... indices) {
  return this->operator()(indices...);
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresSlicing<Indices...>>
inline tensor_view<const T, detail::slicing_rank<Indices...>::value>
tensor<T, Rank>::operator[](Indices... indices) const {
  return this->operator()(indices...);
}
#endif // C++23

template <class T, size_t Rank>
template <class Container, size_t N>
indirect_tensor<T, N> tensor<T, Rank>::operator[](
    const expression<Container, index_type, N> &indices) {
  indirect_tensor<T, N> subarray(m_data, indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    detail::assert_within_bounds(m_shape, indices[i]);
    subarray.iat(i) = ravel_index(indices[i], m_shape, m_order);
  }
  return subarray;
}

template <class T, size_t Rank>
template <class Container, size_t N>
tensor<T, N> tensor<T, Rank>::operator[](
    const expression<Container, index_type, N> &indices) const {
  tensor<T, N> subarray(indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    detail::assert_within_bounds(m_shape, indices[i]);
    subarray[i] = m_data[ravel_index(indices[i], m_shape, m_order)];
  }
  return subarray;
}

template <class T, size_t Rank>
template <class Container, class IntegralType, size_t N,
          detail::RequiresIntegral<IntegralType>>
indirect_tensor<T, N> tensor<T, Rank>::operator[](
    const expression<Container, IntegralType, N> &indices) {
  static_assert(Rank == 1, "Input tensor must be 1-dimensional");
  indirect_tensor<T, N> subarray(m_data, indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    detail::assert_within_bounds(m_size, indices[i]);
    subarray.iat(i) = indices[i];
  }
  return subarray;
}

template <class T, size_t Rank>
template <class Container, class IntegralType, size_t N,
          detail::RequiresIntegral<IntegralType>>
tensor<T, N> tensor<T, Rank>::operator[](
    const expression<Container, IntegralType, N> &indices) const {
  static_assert(Rank == 1, "Input tensor must be 1-dimensional");
  tensor<T, N> subarray(indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    detail::assert_within_bounds(m_size, indices[i]);
    subarray[i] = m_data[indices[i]];
  }
  return subarray;
}

template <class T, size_t Rank>
template <class Container>
indirect_tensor<T, 1>
tensor<T, Rank>::operator[](const expression<Container, bool, Rank> &mask) {
  detail::assert_mask_shape(m_shape, mask.shape());
  size_type size = std::count(mask.self().begin(), mask.self().end(), true);
  indirect_tensor<T, 1> subarray(m_data, size);
  size_t n = 0;
  for (index_type i : make_index_sequence_for(mask)) {
    if (mask[i]) {
      subarray.iat(n++) = ravel_index(i, m_shape, m_order);
    }
  }
  return subarray;
}

template <class T, size_t Rank>
template <class Container>
tensor<T, 1> tensor<T, Rank>::operator[](
    const expression<Container, bool, Rank> &mask) const {
  detail::assert_mask_shape(m_shape, mask.shape());
  size_type size = std::count(mask.self().begin(), mask.self().end(), true);
  tensor<T, 1> subarray(size);
  size_t n = 0;
  for (index_type i : make_index_sequence_for(mask)) {
    if (mask[i]) {
      subarray[n++] = m_data[ravel_index(i, m_shape, m_order)];
    }
  }
  return subarray;
}

template <class T, size_t Rank>
inline const shape_t<Rank> &tensor<T, Rank>::shape() const {
  return m_shape;
}

template <class T, size_t Rank>
inline size_t tensor<T, Rank>::shape(size_type axis) const {
  return m_shape[axis];
}

template <class T, size_t Rank> inline size_t tensor<T, Rank>::size() const {
  return m_size;
}

template <class T, size_t Rank> inline bool tensor<T, Rank>::empty() const {
  return (m_size == 0);
}

template <class T, size_t Rank> inline T *tensor<T, Rank>::data() {
  return m_data;
}

template <class T, size_t Rank> inline const T *tensor<T, Rank>::data() const {
  return m_data;
}

template <class T, size_t Rank>
inline layout_t tensor<T, Rank>::layout() const {
  return m_order;
}

template <class T, size_t Rank>
inline bool tensor<T, Rank>::is_contiguous() const {
  return true;
}

/// Assignment operator.

template <class T, size_t Rank>
tensor<T, Rank> &tensor<T, Rank>::operator=(const tensor &other) {
  this->resize(other.m_shape);
  dense_tensor<tensor<T, Rank>, T, Rank>::operator=(other);
  return *this;
}

template <class T, size_t Rank>
template <class Container, class U>
tensor<T, Rank> &
tensor<T, Rank>::operator=(const expression<Container, U, Rank> &other) {
  this->resize(other.shape());
  dense_tensor<tensor<T, Rank>, T, Rank>::operator=(other);
  return *this;
}

template <class T, size_t Rank>
tensor<T, Rank> &tensor<T, Rank>::operator=(const T &val) {
  std::fill_n(m_data, m_size, val);
  return *this;
}

template <class T, size_t Rank>
tensor<T, Rank> &tensor<T, Rank>::operator=(tensor &&other) {
  if (this != &other) {
    delete[] m_data;
    m_data = other.m_data;
    m_shape = other.m_shape;
    m_size = other.m_size;
    m_order = other.m_order;
    other.m_data = NULL;
    other.m_shape = shape_type();
    other.m_size = 0;
    other.m_order = default_layout;
  }
  return *this;
}

template <class T, size_t Rank>
tensor<T, Rank> &
tensor<T, Rank>::operator=(detail::nested_initializer_list_t<T, Rank> il) {
  shape_type shape;
  __initializer_list_shape<Rank>(il, shape);
  this->resize(shape);
  index_type index;
  __fill_from_initializer_list<Rank>(il, index);
  return *this;
}

/// Public methods.

template <class T, size_t Rank>
tensor_view<T, Rank - 1> tensor<T, Rank>::diagonal(difference_type k) {
  size_type axis1 = Rank - 2, axis2 = Rank - 1;
  shape_type m_stride = make_strides(m_shape, m_order);
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
tensor<T, Rank>::diagonal(difference_type k) const {
  size_type axis1 = Rank - 2, axis2 = Rank - 1;
  shape_type m_stride = make_strides(m_shape, m_order);
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
inline tensor_view<T, 1> tensor<T, Rank>::flatten() {
  return tensor_view<T, 1>(m_data, m_size);
}

template <class T, size_t Rank>
inline tensor_view<const T, 1> tensor<T, Rank>::flatten() const {
  return tensor_view<const T, 1>(m_data, m_size);
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresIntegral<Sizes...>>
inline tensor_view<T, sizeof...(Sizes)>
tensor<T, Rank>::reshape(Sizes... sizes) {
  return this->reshape(make_shape(sizes...));
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresIntegral<Sizes...>>
inline tensor_view<const T, sizeof...(Sizes)>
tensor<T, Rank>::reshape(Sizes... sizes) const {
  return this->reshape(make_shape(sizes...));
}

template <class T, size_t Rank>
template <size_t N>
inline tensor_view<T, N> tensor<T, Rank>::reshape(const shape_t<N> &shape) {
  return this->reshape(shape, m_order);
}

template <class T, size_t Rank>
template <size_t N>
inline tensor_view<const T, N>
tensor<T, Rank>::reshape(const shape_t<N> &shape) const {
  return this->reshape(shape, m_order);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<T, N> tensor<T, Rank>::reshape(const shape_t<N> &shape,
                                           layout_t order) {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  return tensor_view<T, N>(m_data, shape, order);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<const T, N> tensor<T, Rank>::reshape(const shape_t<N> &shape,
                                                 layout_t order) const {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  return tensor_view<const T, N>(m_data, shape, order);
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
inline void tensor<T, Rank>::resize(Sizes... sizes) {
  this->resize(shape_type(sizes...));
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
  layout_t order = (m_order == row_major) ? column_major : row_major;
  std::reverse(shape.data(), shape.data() + Rank);
  return tensor_view<T, Rank>(m_data, shape, order);
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor<T, Rank>::t() const {
  shape_type shape = m_shape;
  layout_t order = (m_order == row_major) ? column_major : row_major;
  std::reverse(shape.data(), shape.data() + Rank);
  return tensor_view<const T, Rank>(m_data, shape, order);
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
inline tensor_view<T, Rank> tensor<T, Rank>::t(Sizes... axes) {
  return this->t(shape_type(axes...));
}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
inline tensor_view<const T, Rank> tensor<T, Rank>::t(Sizes... axes) const {
  return this->t(shape_type(axes...));
}

template <class T, size_t Rank>
tensor_view<T, Rank> tensor<T, Rank>::t(const shape_type &axes) {
  shape_type shape, strides;
  shape_type m_stride = make_strides(m_shape, m_order);
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] = m_shape[axes[i]];
    strides[i] = m_stride[axes[i]];
  }
  return tensor_view<T, Rank>(m_data, shape, 0, strides, m_order);
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor<T, Rank>::t(const shape_type &axes) const {
  shape_type shape, strides;
  shape_type m_stride = make_strides(m_shape, m_order);
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] = m_shape[axes[i]];
    strides[i] = m_stride[axes[i]];
  }
  return tensor_view<const T, Rank>(m_data, shape, 0, strides, m_order);
}

template <class T, size_t Rank>
inline tensor_view<T, Rank> tensor<T, Rank>::view() {
  return tensor_view<T, Rank>(m_data, m_shape, m_order);
}

template <class T, size_t Rank>
inline tensor_view<const T, Rank> tensor<T, Rank>::view() const {
  return tensor_view<const T, Rank>(m_data, m_shape, m_order);
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_TCC_INCLUDED
