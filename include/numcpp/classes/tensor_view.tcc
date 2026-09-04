/*
 * File: include/numcpp/classes/tensor_view.tcc
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
template <integer_or_slice... Indices>
  requires((sizeof...(Indices) == Rank) &&
           (std::same_as<Indices, slice> || ...))
tensor_view<T, detail::slicing_rank<Indices...>>
tensor_view<T, Rank>::operator()(const Indices&... indices) {
  constexpr size_t N = detail::slicing_rank<Indices...>;
  shape_t<N> shape;
  index_t<N> strides;
  ptrdiff_t offset = __unpack_slices(m_shape, m_stride, shape.data(),
                                     strides.data(), indices...);
  return tensor_view<T, N>(m_data + offset, shape, strides);
}

template <class T, size_t Rank>
template <integer_or_slice... Indices>
  requires((sizeof...(Indices) == Rank) &&
           (std::same_as<Indices, slice> || ...))
tensor_view<const T, detail::slicing_rank<Indices...>>
tensor_view<T, Rank>::operator()(const Indices&... indices) const {
  constexpr size_t N = detail::slicing_rank<Indices...>;
  shape_t<N> shape;
  index_t<N> strides;
  ptrdiff_t offset = __unpack_slices(m_shape, m_stride, shape.data(),
                                     strides.data(), indices...);
  return tensor_view<const T, N>(m_data + offset, shape, strides);
}

/**
 * Returns true if the strides corresponds to a contiguous view in given
 * memory layout.
 */
template <size_t Rank>
bool __is_contiguous(const shape_t<Rank>& shape, const index_t<Rank>& strides,
                     layout_t layout) {
  size_t size = 1;
  for (size_t i = 0; i < Rank; ++i) {
    size_t k = (layout == layout_left) ? i : Rank - 1 - i;
    if (shape[k] != 1 && strides[k] != size) {
      return false;
    }
    size *= shape[k];
  }
  return true;
}

template <class T, size_t Rank>
inline layout_t tensor_view<T, Rank>::layout() const {
  layout_t layout = static_cast<layout_t>(0);
  if (__is_contiguous(m_shape, m_stride, layout_right)) {
    layout = layout | layout_right;
  }
  if (__is_contiguous(m_shape, m_stride, layout_left)) {
    layout = layout | layout_left;
  }
  return layout;
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
  if (!this->layout()) {
    throw std::runtime_error("cannot flatten a non-contiguous view");
  }
  return vector_view<T>(m_data, m_size);
}

template <class T, size_t Rank>
inline tensor_view<const T, 1> tensor_view<T, Rank>::flatten() const {
  if (!this->layout()) {
    throw std::runtime_error("cannot flatten a non-contiguous view");
  }
  return vector_view<const T>(m_data, m_size);
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
  if (!this->layout()) {
    throw std::runtime_error("cannot reshape a non-contiguous view");
  }
  return tensor_view<T, N>(m_data, shape, make_strides(shape, layout));
}

template <class T, size_t Rank>
template <size_t N>
tensor_view<const T, N> tensor_view<T, Rank>::reshape(const shape_t<N>& shape,
                                                      layout_t layout) const {
  if (m_size != shape.prod()) {
    std::ostringstream error;
    error << "cannot reshape tensor of shape " << m_shape << " into shape "
          << shape;
    throw std::invalid_argument(error.str());
  }
  if (!this->layout()) {
    throw std::runtime_error("cannot reshape a non-contiguous view");
  }
  return tensor_view<const T, N>(m_data, shape, make_strides(shape, layout));
}

template <class T, size_t Rank> tensor_view<T, Rank> tensor_view<T, Rank>::t() {
  shape_type shape = m_shape;
  index_type strides = m_stride;
  std::reverse(shape.begin(), shape.end());
  std::reverse(strides.begin(), strides.end());
  return tensor_view<T, Rank>(m_data, shape, strides);
}

template <class T, size_t Rank>
tensor_view<const T, Rank> tensor_view<T, Rank>::t() const {
  shape_type shape = m_shape;
  index_type strides = m_stride;
  std::reverse(shape.begin(), shape.end());
  std::reverse(strides.begin(), strides.end());
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
