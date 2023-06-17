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

/** @file include/numcpp/broadcasting/broadcasting.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_BROADCASTING_TCC_INCLUDED
#define NUMCPP_BROADCASTING_TCC_INCLUDED

#include <algorithm>
#include <cmath>
#include <numeric>
#include "numcpp/broadcasting/assert.h"
#include "numcpp/iterators/index_sequence.h"

namespace numcpp {
/// Tensor creation routines from existing data.

template <class InputIterator, detail::RequiresInputIterator<InputIterator>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type, 1>
asarray(InputIterator first, InputIterator last) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  size_t size = std::distance(first, last);
  return tensor<T, 1>(first, size);
}

template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator>,
          detail::RequiresIntegral<Sizes...>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type,
              sizeof...(Sizes)>
asarray(InputIterator first, Sizes... sizes) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  constexpr size_t Rank = sizeof...(Sizes);
  return tensor<T, Rank>(first, sizes...);
}

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asarray(InputIterator first, const shape_t<Rank> &shape, layout_t order) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  return tensor<T, Rank>(first, shape, order);
}

template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator>,
          detail::RequiresIntegral<Sizes...>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type,
              sizeof...(Sizes)>
ascontiguousarray(InputIterator first, Sizes... sizes) {
  return asarray(first, make_shape(sizes...), row_major);
}

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
ascontiguousarray(InputIterator first, const shape_t<Rank> &shape) {
  return asarray(first, shape, row_major);
}

template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator>,
          detail::RequiresIntegral<Sizes...>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type,
              sizeof...(Sizes)>
asfortranarray(InputIterator first, Sizes... sizes) {
  return asarray(first, make_shape(sizes...), column_major);
}

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asfortranarray(InputIterator first, const shape_t<Rank> &shape) {
  return asarray(first, shape, column_major);
}

namespace detail {
/**
 * @brief Throws a std::invalid_argument exception if the range
 * [first, first + size) containts NaNs or Infs.
 */
template <class InputIterator>
void assert_finite(InputIterator first, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    if (!std::isfinite(*first)) {
      throw std::invalid_argument("array must not contain infs or NaNs");
    }
    ++first;
  }
}
} // namespace detail

template <class InputIterator, detail::RequiresInputIterator<InputIterator>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type, 1>
asarray_chkfinite(InputIterator first, InputIterator last) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  size_t size = std::distance(first, last);
  detail::assert_finite(first, size);
  return tensor<T, 1>(first, size);
}

template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator>,
          detail::RequiresIntegral<Sizes...>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type,
              sizeof...(Sizes)>
asarray_chkfinite(InputIterator first, Sizes... sizes) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  constexpr size_t Rank = sizeof...(Sizes);
  shape_t<Rank> shape(sizes...);
  detail::assert_finite(first, shape.prod());
  return tensor<T, Rank>(first, shape);
}

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator>>
inline tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asarray_chkfinite(InputIterator first, const shape_t<Rank> &shape,
                  layout_t order) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  detail::assert_finite(first, shape.prod());
  return tensor<T, Rank>(first, shape, order);
}

/// Broadcasting.

template <class T, size_t Rank>
tensor_view<const T, Rank> broadcast_to(tensor<T, Rank> &a,
                                        const shape_t<Rank> &shape) {
  shape_t<Rank> strides = make_strides(a.shape(), a.layout());
  for (size_t i = 0; i < Rank; ++i) {
    if (a.shape(i) != shape[i]) {
      if (a.shape(i) != 1) {
        std::ostringstream error;
        error << "operands could not be broadcast together with shape "
              << a.shape() << " and requested shape " << shape;
        throw std::invalid_argument(error.str());
      }
      strides[i] = 0;
    }
  }
  return tensor_view<const T, Rank>(a.data(), shape, 0, strides, a.layout());
}

template <class T, size_t Rank>
tensor_view<const T, Rank> broadcast_to(tensor_view<T, Rank> &a,
                                        const shape_t<Rank> &shape) {
  shape_t<Rank> strides = a.strides();
  for (size_t i = 0; i < Rank; ++i) {
    if (a.shape(i) != shape[i]) {
      if (a.shape(i) != 1) {
        std::ostringstream error;
        error << "operands could not be broadcast together with shape "
              << a.shape() << " and requested shape " << shape;
        throw std::invalid_argument(error.str());
      }
      strides[i] = 0;
    }
  }
  return tensor_view<const T, Rank>(a.data(), shape, a.offset(), strides,
                                    a.layout());
}

template <class T, size_t Rank, class... Axes,
          detail::RequiresIntegral<Axes...>>
inline tensor_view<T, Rank + sizeof...(Axes)> expand_dims(tensor<T, Rank> &a,
                                                          Axes... axes) {
  return expand_dims(a, make_shape(axes...));
}

template <class T, size_t Rank, class... Axes,
          detail::RequiresIntegral<Axes...>>
inline tensor_view<T, Rank + sizeof...(Axes)>
expand_dims(tensor_view<T, Rank> &a, Axes... axes) {
  return expand_dims(a, make_shape(axes...));
}

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank + N> expand_dims(tensor<T, Rank> &a,
                                     const shape_t<N> &axes) {
  shape_t<Rank + N> shape = detail::insert_axes(a.shape(), axes, 1);
  return tensor_view<T, Rank + N>(a.data(), shape, a.layout());
}

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank + N> expand_dims(tensor_view<T, Rank> &a,
                                     const shape_t<N> &axes) {
  shape_t<Rank + N> shape = detail::insert_axes(a.shape(), axes, 1);
  shape_t<Rank + N> strides = detail::insert_axes(a.strides(), axes);
  return tensor_view<T, Rank + N>(a.data(), shape, a.offset(), strides,
                                  a.layout());
}

template <class T, size_t Rank, class... Axes,
          detail::RequiresIntegral<Axes...>>
inline tensor_view<T, Rank - sizeof...(Axes)> squeeze(tensor<T, Rank> &a,
                                                      Axes... axes) {
  return squeeze(a, make_shape(axes...));
}

template <class T, size_t Rank, class... Axes,
          detail::RequiresIntegral<Axes...>>
inline tensor_view<T, Rank - sizeof...(Axes)> squeeze(tensor_view<T, Rank> &a,
                                                      Axes... axes) {
  return squeeze(a, make_shape(axes...));
}

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank - N> squeeze(tensor<T, Rank> &a, const shape_t<N> &axes) {
  for (size_t i = 0; i < N; ++i) {
    if (a.shape(i) != 1) {
      throw std::invalid_argument("cannot select an axis to squeeze out which "
                                  "has size not equal to one");
    }
  }
  shape_t<Rank - N> shape = detail::remove_axes(a.shape(), axes);
  return tensor_view<T, Rank - N>(a.data(), shape, a.layout());
}

template <class T, size_t Rank, size_t N>
tensor_view<T, Rank - N> squeeze(tensor_view<T, Rank> &a,
                                 const shape_t<N> &axes) {
  for (size_t i = 0; i < N; ++i) {
    if (a.shape(i) != 1) {
      throw std::invalid_argument("cannot select an axis to squeeze out which "
                                  "has size not equal to one");
    }
  }
  shape_t<Rank - N> shape = detail::remove_axes(a.shape(), axes);
  shape_t<Rank - N> strides = detail::remove_axes(a.strides(), axes);
  return tensor_view<T, Rank - N>(a.data(), shape, a.offset(), strides,
                                  a.layout());
}

/// Basic manipulation routines.

template <class Container1, class T, size_t Rank, class Container2, class U>
void copyto(dense_tensor<Container1, T, Rank> &dest,
            const expression<Container2, U, Rank> &src) {
  detail::assert_output_shape(dest.shape(),
                              broadcast_shapes(dest.shape(), src.shape()));
  for (index_t<Rank> index : make_index_sequence_for(dest)) {
    index_t<Rank> i;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (src.shape(axis) > 1) ? index[axis] : 0;
    }
    dest[index] = src[i];
  }
}

template <class Container, class T, size_t Rank>
inline tensor<T, Rank> copy(const expression<Container, T, Rank> &a) {
  return copy(a, a.layout());
}

template <class Container, class T, size_t Rank>
inline tensor<T, Rank> copy(const expression<Container, T, Rank> &a,
                            layout_t order) {
  tensor<T, Rank> out(a.shape(), order);
  copyto(out, a);
  return out;
}

template <class Container, class T, size_t Rank>
inline tensor<T, 1> flatten(const expression<Container, T, Rank> &a) {
  return flatten(a, a.layout());
}

template <class Container, class T, size_t Rank>
tensor<T, 1> flatten(const expression<Container, T, Rank> &a, layout_t order) {
  tensor<T, 1> out(a.size());
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence(a.shape(), order)) {
    out[n++] = a[i];
  }
  return out;
}

/// Concatenation.

namespace detail {
/**
 * @brief Computes the shape of the concatenation along the given axis. Throws a
 * std::invalid_argument exception if the shapes do not match along the
 * remaining axes.
 */
template <size_t Rank> void concatenation_shape(shape_t<Rank> &, size_t) {}

template <class Container1, class T, size_t Rank, class... Container2>
void concatenation_shape(shape_t<Rank> &out_shape, size_t axis,
                         const expression<Container1, T, Rank> &a,
                         const Container2 &...b) {
  for (size_t i = 0; i < Rank; ++i) {
    if (a.shape(i) != out_shape[i] && i != axis) {
      std::ostringstream error;
      error << "all the tensor dimensions for the concatenation axis must "
               "match exactly, but along dimension "
            << i << ", tensors have sizes " << out_shape[i] << " and "
            << a.shape(i);
      throw std::invalid_argument(error.str());
    }
  }
  out_shape[axis] += a.shape(axis);
  concatenation_shape(out_shape, axis, b...);
}

/**
 * @brief Concatenate tensors along the given axis.
 */
template <class T, size_t Rank>
void concatenate_impl(tensor<T, Rank> &, size_t, size_t) {}

template <class Container1, class T, size_t Rank, class... Container2>
void concatenate_impl(tensor<T, Rank> &out, size_t axis, size_t offset,
                      const expression<Container1, T, Rank> &a,
                      const Container2 &...b) {
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    index_t<Rank> out_index = i;
    out_index[axis] += offset;
    out[out_index] = a[i];
  }
  offset += a.shape(axis);
  concatenate_impl(out, axis, offset, b...);
}

/**
 * @brief Stack tensors along the given axis.
 */
template <class T, size_t Rank>
void stack_impl(tensor<T, Rank> &, size_t, size_t) {}

template <class Container1, class T, size_t Rank, class... Container2>
void stack_impl(tensor<T, Rank + 1> &out, size_t axis, size_t offset,
                const expression<Container1, T, Rank> &a,
                const Container2 &...b) {
  shape_t<Rank> shape = detail::remove_axes(out.shape(), axis);
  if (shape != a.shape()) {
    throw std::invalid_argument("all the tensors must have the same shape");
  }
  for (index_t<Rank> i : make_index_sequence_for(a)) {
    index_t<Rank + 1> out_index = detail::insert_axes(i, axis, offset);
    out[out_index] = a[i];
  }
  stack_impl(out, axis, offset + 1, b...);
}
} // namespace detail

template <class Container1, class T, size_t Rank, class... Container2>
inline tensor<T, Rank> concatenate(const expression<Container1, T, Rank> &a,
                                   const Container2 &...b) {
  return concatenate<0>(a, b...);
}

template <size_t Axis, class Container1, class T, size_t Rank,
          class... Container2>
tensor<T, Rank> concatenate(const expression<Container1, T, Rank> &a,
                            const Container2 &...b) {
  shape_t<Rank> shape = a.shape();
  detail::concatenation_shape(shape, Axis, b...);
  tensor<T, Rank> out(shape);
  detail::concatenate_impl(out, Axis, 0, a, b...);
  return out;
}

template <class Container1, class T, size_t Rank, class... Container2>
tensor<T, Rank + 1> stack(const expression<Container1, T, Rank> &a,
                          const Container2 &...b) {
  return stack<0>(a, b...);
}

template <size_t Axis, class Container1, class T, size_t Rank,
          class... Container2>
tensor<T, Rank + 1> stack(const expression<Container1, T, Rank> &a,
                          const Container2 &...b) {
  shape_t<Rank + 1> shape = detail::insert_axes(a.shape(), Axis, 1);
  shape[Axis] += sizeof...(b);
  tensor<T, Rank + 1> out(shape);
  detail::stack_impl(out, Axis, 0, a, b...);
  return out;
}

/// Tiling.

template <class Container, class T, size_t Rank>
tensor<T, Rank> tile(const expression<Container, T, Rank> &a,
                     const shape_t<Rank> &reps) {
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] *= reps[i];
  }
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    for (size_t i = 0; i < Rank; ++i) {
      a_index[i] %= a.shape(i);
    }
    out[out_index] = a[a_index];
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<T, Rank> repeat(const expression<Container, T, Rank> &a, size_t reps,
                       size_t axis) {
  shape_t<Rank> shape = a.shape();
  shape[axis] *= reps;
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    a_index[axis] /= reps;
    out[out_index] = a[a_index];
  }
  return out;
}

template <class Container1, class T, size_t Rank, class Container2,
          class IntegralType, detail::RequiresIntegral<IntegralType>>
tensor<T, Rank> repeat(const expression<Container1, T, Rank> &a,
                       const expression<Container2, IntegralType, 1> &reps,
                       size_t axis) {
  detail::assert_output_shape(a.shape(axis), reps.size());
  shape_t<Rank> shape = a.shape();
  shape[axis] =
      std::accumulate(reps.self().begin(), reps.self().end(), IntegralType(0));
  tensor<T, Rank> out(shape);
  shape[axis] = 1;
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    for (size_t i = 0; i < reps.size(); ++i) {
      for (IntegralType times = 0; times < reps[i]; ++times) {
        out[out_index] = a[a_index];
        ++out_index[axis];
      }
      ++a_index[axis];
    }
  }
  return out;
}

/// Padding.

template <class Container, class T, size_t Rank>
tensor<T, Rank> pad(const expression<Container, T, Rank> &a,
                    const shape_t<Rank> &before, const shape_t<Rank> &after) {
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < Rank; ++i) {
    shape[i] += before[i] + after[i];
  }
  tensor<T, Rank> out(shape);
  for (index_t<Rank> index : make_index_sequence_for(a)) {
    index_t<Rank> out_index = index;
    for (size_t i = 0; i < Rank; ++i) {
      out_index[i] += before[i];
    }
    out[out_index] = a[index];
  }
  return out;
}

template <class Container, class T, size_t Rank, class Function, class... Args>
tensor<T, Rank> pad(const expression<Container, T, Rank> &a,
                    const shape_t<Rank> &before, const shape_t<Rank> &after,
                    Function func, Args &&...args) {
  tensor<T, Rank> out = pad(a, before, after);
  shape_t<Rank> shape = out.shape();
  shape_t<Rank> strides = make_strides(shape);
  for (size_t axis = 0; axis < Rank; ++axis) {
    size_t size = shape[axis];
    shape[axis] = 1;
    for (index_t<Rank> index : make_index_sequence(shape)) {
      ptrdiff_t offset = ravel_index(index, out.shape());
      size_t stride = strides[axis];
      tensor_view<T, 1> view(out.data(), size, offset, stride);
      func(view, before[axis], after[axis], axis, std::forward<Args>(args)...);
    }
    shape[axis] = size;
  }
  return out;
}

/// Indexing routines.

template <class Container1, class T, size_t Rank, class Container2, size_t N>
tensor<T, N> take(const expression<Container1, T, Rank> &a,
                  const expression<Container2, index_t<Rank>, N> &indices) {
  tensor<T, N> out(indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    out[i] = a[indices[i]];
  }
  return out;
}

template <class Container1, class T, class Container2, class IntegralType,
          size_t N, detail::RequiresIntegral<IntegralType>>
tensor<T, N> take(const expression<Container1, T, 1> &a,
                  const expression<Container2, IntegralType, N> &indices) {
  tensor<T, N> out(indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    out[i] = a[indices[i]];
  }
  return out;
}

template <class Container1, class T, size_t Rank, class Container2,
          class IntegralType, detail::RequiresIntegral<IntegralType>>
tensor<T, Rank> take(const expression<Container1, T, Rank> &a,
                     const expression<Container2, IntegralType, 1> &indices,
                     size_t axis) {
  shape_t<Rank> shape = a.shape();
  shape[axis] = indices.size();
  tensor<T, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    a_index[axis] = indices[a_index[axis]];
    out[out_index] = a[a_index];
  }
  return out;
}

template <class Container, class T, size_t Rank>
tensor<T, Rank - 1> take(const expression<Container, T, Rank> &a, size_t index,
                         size_t axis) {
  shape_t<Rank - 1> shape = detail::remove_axes(a.shape(), axis);
  tensor<T, Rank - 1> out(shape);
  for (index_t<Rank - 1> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = detail::insert_axes(out_index, axis, index);
    out[out_index] = a[a_index];
  }
  return out;
}

template <class Container1, class T, size_t Rank, class Container2,
          class IntegralType, detail::RequiresIntegral<IntegralType>>
tensor<T, Rank>
take_along_axis(const expression<Container1, T, Rank> &a,
                const expression<Container2, IntegralType, Rank> &indices,
                size_t axis) {
  detail::assert_index_along_axis_shape(a.shape(), indices.shape(), axis);
  tensor<T, Rank> out(indices.shape(), indices.layout());
  for (index_t<Rank> out_index : make_index_sequence_for(indices)) {
    index_t<Rank> a_index = out_index;
    a_index[axis] = indices[out_index];
    out[out_index] = a[a_index];
  }
  return out;
}

template <class Container1, class T, size_t Rank, class Container2, size_t N,
          class Container3>
void put(dense_tensor<Container1, T, Rank> &a,
         const expression<Container2, index_t<Rank>, N> &indices,
         const expression<Container3, T, N> &values) {
  detail::assert_output_shape(
      indices.shape(), broadcast_shapes(indices.shape(), values.shape()));
  for (index_t<N> index : make_index_sequence_for(indices)) {
    index_t<N> i;
    for (size_t axis = 0; axis < N; ++axis) {
      i[axis] = (values.shape(axis) > 1) ? index[axis] : 0;
    }
    a[indices[index]] = values[i];
  }
}

template <class Container1, class T, size_t Rank, class Container2, size_t N>
void put(dense_tensor<Container1, T, Rank> &a,
         const expression<Container2, index_t<Rank>, N> &indices,
         const typename dense_tensor<Container1, T, Rank>::value_type &value) {
  for (index_t<N> i : make_index_sequence_for(indices)) {
    a[indices[i]] = value;
  }
}

template <class Container1, class T, class Container2, class IntegralType,
          size_t N, class Container3, detail::RequiresIntegral<IntegralType>>
void put(dense_tensor<Container1, T, 1> &a,
         const expression<Container2, IntegralType, N> &indices,
         const expression<Container3, T, N> &values) {
  detail::assert_output_shape(
      indices.shape(), broadcast_shapes(indices.shape(), values.shape()));
  for (index_t<N> index : make_index_sequence_for(indices)) {
    index_t<N> i;
    for (size_t axis = 0; axis < N; ++axis) {
      i[axis] = (values.shape(axis) > 1) ? index[axis] : 0;
    }
    a[indices[index]] = values[i];
  }
}

template <class Container1, class T, class Container2, class IntegralType,
          size_t N, detail::RequiresIntegral<IntegralType>>
void put(dense_tensor<Container1, T, 1> &a,
         const expression<Container2, IntegralType, N> &indices,
         const typename dense_tensor<Container1, T, 1>::value_type &value) {
  for (index_t<N> i : make_index_sequence_for(indices)) {
    a[indices[i]] = value;
  }
}

template <class Container1, class T, size_t Rank, class Container2,
          class IntegralType, class Container3,
          detail::RequiresIntegral<IntegralType>>
void put_along_axis(dense_tensor<Container1, T, Rank> &a,
                    const expression<Container2, IntegralType, Rank> &indices,
                    const expression<Container3, T, Rank> &values,
                    size_t axis) {
  detail::assert_index_along_axis_shape(a.shape(), indices.shape(), axis);
  detail::assert_output_shape(
      indices.shape(), broadcast_shapes(indices.shape(), values.shape()));
  for (index_t<Rank> index : make_index_sequence_for(indices)) {
    index_t<Rank> i;
    for (size_t axis = 0; axis < Rank; ++axis) {
      i[axis] = (values.shape(axis) > 1) ? index[axis] : 0;
    }
    index[axis] = indices[index];
    a[index] = values[i];
  }
}

template <class Container1, class T, size_t Rank, class Container2>
tensor<T, 1> compress(const expression<Container1, T, Rank> &a,
                      const expression<Container2, bool, Rank> &condition) {
  detail::assert_mask_shape(a.shape(), condition.shape());
  size_t size =
      std::count(condition.self().begin(), condition.self().end(), true);
  tensor<T, 1> out(size);
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      out[n++] = a[i];
    }
  }
  return out;
}

template <class Container1, class T, size_t Rank, class Container2>
tensor<T, Rank> compress(const expression<Container1, T, Rank> &a,
                         const expression<Container2, bool, 1> &condition,
                         size_t axis) {
  detail::assert_mask_shape(a.shape(axis), condition.size(), axis);
  shape_t<Rank> shape = a.shape();
  shape[axis] =
      std::count(condition.self().begin(), condition.self().end(), true);
  tensor<T, Rank> out(shape);
  shape[axis] = 1;
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    for (size_t i = 0; i < condition.size(); ++i) {
      if (condition[i]) {
        a_index[axis] = i;
        out[out_index] = a[a_index];
        ++out_index[axis];
      }
    }
  }
  return out;
}

template <class Container1, class T, size_t Rank, class Container2,
          class Container3>
void place(dense_tensor<Container1, T, Rank> &a,
           const expression<Container2, bool, Rank> &condition,
           const expression<Container3, T, 1> &values) {
  detail::assert_mask_shape(a.shape(), condition.shape());
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      a[i] = values[n++];
    }
  }
}

template <class Container1, class T, size_t Rank, class Container2>
void place(
    dense_tensor<Container1, T, Rank> &a,
    const expression<Container2, bool, Rank> &condition,
    const typename dense_tensor<Container1, T, Rank>::value_type &value) {
  detail::assert_mask_shape(a.shape(), condition.shape());
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      a[i] = value;
    }
  }
}

template <class Container1, class T, size_t Rank, class Container2,
          class Container3>
void putmask(dense_tensor<Container1, T, Rank> &a,
             const expression<Container2, bool, Rank> &condition,
             const expression<Container3, T, Rank> &values) {
  detail::assert_output_shape(a.shape(),
                              broadcast_shapes(a.shape(), values.shape()));
  detail::assert_mask_shape(a.shape(), condition.shape());
  for (index_t<Rank> index : make_index_sequence_for(condition)) {
    if (condition[index]) {
      index_t<Rank> i;
      for (size_t axis = 0; axis < Rank; ++axis) {
        i[axis] = (values.shape(axis) > 1) ? index[axis] : 0;
      }
      a[index] = values[i];
    }
  }
}
} // namespace numcpp

#endif // NUMCPP_BROADCASTING_TCC_INCLUDED
