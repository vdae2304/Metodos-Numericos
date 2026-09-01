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
#include "numcpp/utilities/assert.h"

namespace numcpp {
/// Public methods.

/**
 * @brief Constructs the shape, strides and offset to construct a tensor_view
 * from slicing.
 * 
 * @param in_shape The shape of the input tensor.
 * @param in_strides The strides of the input tensor.
 * @param out_shape The memory array of the output shape.
 * @param out_strides The memory array of the output strides.
 * 
 * @return The output offset.
 */
template <size_t Rank>
ptrdiff_t __unpack_slices(const shape_t<Rank>& in_shape,
                          const index_t<Rank>& in_strides, size_t* out_shape,
                          ptrdiff_t* out_strides) {
  return 0;
}

/**
 * @overload
 * 
 * @param slc Current slice.
 */
template <size_t Rank, class... Slices>
ptrdiff_t __unpack_slices(const shape_t<Rank>& in_shape,
                          const index_t<Rank>& in_strides, size_t* out_shape,
                          ptrdiff_t* out_strides, slice slc,
                          const Slices&... args) {
  size_t axis = Rank - 1 - sizeof...(Slices);
  slc = slc.bind_array(in_shape[axis]);
  detail::assert_within_bounds(in_shape[axis], slc.start(), axis);

  *out_shape = 0;
  *out_strides = slc.stride() * in_strides[axis];
  if (slc.stride() > 0 && slc.start() < slc.stop()) {
    *out_shape = 1 + (slc.stop() - slc.start() - 1) / slc.stride();
  } else if (slc.stride() < 0 && slc.start() > slc.stop()) {
    *out_shape = 1 + (slc.stop() - slc.start() + 1) / slc.stride();
  }

  return slc.start() * in_strides[axis] +
         __unpack_slices(in_shape, in_strides, out_shape + 1, out_strides + 1,
                         args...);
}

/**
 * @overload
 * 
 * @param idx Current index.
 */
template <size_t Rank, class... Slices>
ptrdiff_t __unpack_slices(const shape_t<Rank>& in_shape,
                          const index_t<Rank>& in_strides, size_t* out_shape,
                          ptrdiff_t* out_strides, ptrdiff_t idx,
                          const Slices&... args) {
  size_t axis = Rank - 1 - sizeof...(Slices);
  if (idx < 0) {
    idx += in_shape[axis];
  }
  detail::assert_within_bounds(in_shape[axis], idx, axis);
  return idx * in_strides[axis] +
         __unpack_slices(in_shape, in_strides, out_shape, out_strides, args...);
}

template <class T, size_t Rank>
template <class... Indices,
          detail::requires_all<detail::n_arguments<Rank, Indices...>,
                               detail::has_slicing<Indices...> > >
tensor_view<T, detail::slicing_rank<Indices...>::value>
tensor_view<T, Rank>::operator()(const Indices&... indices) {
  constexpr size_t N = detail::slicing_rank<Indices...>::value;
  shape_t<N> shape;
  index_t<N> strides;
  ptrdiff_t offset = __unpack_slices(m_shape, m_stride, shape.data(),
                                     strides.data(), indices...);
  return tensor_view<T, N>(m_data + offset, shape, strides);
}

template <class T, size_t Rank>
template <class... Indices,
          detail::requires_all<detail::n_arguments<Rank, Indices...>,
                               detail::has_slicing<Indices...> > >
tensor_view<const T, detail::slicing_rank<Indices...>::value>
tensor_view<T, Rank>::operator()(const Indices&... indices) const {
  constexpr size_t N = detail::slicing_rank<Indices...>::value;
  shape_t<N> shape;
  index_t<N> strides;
  ptrdiff_t offset = __unpack_slices(m_shape, m_stride, shape.data(),
                                     strides.data(), indices...);
  return tensor_view<const T, N>(m_data + offset, shape, strides);
}

/**
 * Returns true if the strides can be flattened into a 1-dimensional strided
 * view.
 */
template <size_t Rank>
bool __is_almost_contiguous(const shape_t<Rank>& shape,
                            const index_t<Rank>& strides, layout_t layout) {
  ptrdiff_t current = (layout == layout_right) ? strides[Rank - 1] : strides[0];
  for (size_t i = 0; i < Rank; ++i) {
    size_t k = (layout == layout_right) ? Rank - 1 - i : i;
    if (shape[k] != 1 && strides[k] != current) {
      return false;
    }
    current *= shape[k];
  }
  return true;
}

template <class T, size_t Rank>
inline layout_t tensor_view<T, Rank>::layout() const {
  if (__is_almost_contiguous(m_shape, m_stride, layout_right)) {
    return layout_right;
  }
  if (__is_almost_contiguous(m_shape, m_stride, layout_left)) {
    return layout_left;
  }
  return no_layout;
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor_view<T, Rank>::broadcast_to(
    const shape_type& shape) const {
  index_type strides = m_stride;
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
inline tensor_view<T, 1> tensor_view<T, Rank>::flatten() {
  switch (this->layout()) {
    case layout_right:
      return tensor_view<T, 1>(m_data, {m_size}, {m_stride[Rank - 1]});
    case layout_left:
      return tensor_view<T, 1>(m_data, {m_size}, {m_stride[0]});
    default:
      throw std::runtime_error("view cannot be flattened");
  }
}

template <class T, size_t Rank>
inline tensor_view<const T, 1> tensor_view<T, Rank>::flatten() const {
  switch (this->layout()) {
    case layout_right:
      return tensor_view<const T, 1>(m_data, {m_size}, {m_stride[Rank - 1]});
    case layout_left:
      return tensor_view<const T, 1>(m_data, {m_size}, {m_stride[0]});
    default:
      throw std::runtime_error("view cannot be flattened");
  }
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<T, N> tensor_view<T, Rank>::reshape(const shape_t<N> &shape,
                                                layout_t layout) {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  ptrdiff_t base_stride;
  switch (this->layout()) {
    case layout_right:
      if (layout == no_layout) layout = layout_right;
      base_stride = m_stride[Rank - 1];
      break;
    case layout_left:
      if (layout == no_layout) layout = layout_left;
      base_stride = m_stride[0];
      break;
    default:
      throw std::runtime_error("view cannot reshaped");
  }
  index_t<N> strides = make_strides(shape, layout);
  for (size_t i = 0; i < Rank; ++i) {
    strides[i] *= base_stride;
  }
  return tensor_view<T, N>(m_data, shape, strides);
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<const T, N> tensor_view<T, Rank>::reshape(const shape_t<N> &shape,
                                                      layout_t layout) const {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  ptrdiff_t base_stride;
  switch (this->layout()) {
    case layout_right:
      if (layout == no_layout) layout = layout_right;
      base_stride = m_stride[Rank - 1];
      break;
    case layout_left:
      if (layout == no_layout) layout = layout_left;
      base_stride = m_stride[0];
      break;
    default:
      throw std::runtime_error("view cannot reshaped");
  }
  index_t<N> strides = make_strides(shape, layout);
  for (size_t i = 0; i < Rank; ++i) {
    strides[i] *= base_stride;
  }
  return tensor_view<const T, N>(m_data, shape, strides);
}

template <class T, size_t Rank> tensor_view<T, Rank> tensor_view<T, Rank>::t() {
  shape_type shape = m_shape;
  index_type strides = m_stride;
  std::reverse(shape.data(), shape.data() + Rank);
  std::reverse(strides.data(), strides.data() + Rank);
  return tensor_view<T, Rank>(m_data, shape, strides);
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor_view<T, Rank>::t() const {
  shape_type shape = m_shape;
  index_type strides = m_stride;
  std::reverse(shape.data(), shape.data() + Rank);
  std::reverse(strides.data(), strides.data() + Rank);
  return tensor_view<const T, Rank>(m_data, shape, strides);
}

template <class T, size_t Rank>
tensor_view<T, Rank> tensor_view<T, Rank>::t(const shape_type &axes) {
  shape_type shape;
  index_type strides;
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] = m_shape[axes[i]];
    strides[i] = m_stride[axes[i]];
  }
  return tensor_view<T, Rank>(m_data, shape, strides);
}

template <class T, size_t Rank>
tensor_view<const T, Rank>
tensor_view<T, Rank>::t(const shape_type &axes) const {
  shape_type shape;
  index_type strides;
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] = m_shape[axes[i]];
    strides[i] = m_stride[axes[i]];
  }
  return tensor_view<const T, Rank>(m_data, shape, strides);
}
} // namespace numcpp

#endif // NUMCPP_TENSOR_VIEW_TCC_INCLUDED
