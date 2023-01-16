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
#include <numeric>
#include <sstream>
#include <stdexcept>

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
tensor_view<T, Rank> broadcast_to(T &val, const shape_t<Rank> &shape) {
  return tensor_view<T, Rank>(&val, shape, 0, shape_t<Rank>());
}

template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(tensor<T, Rank> &arg,
                                  const shape_t<Rank> &shape) {
  shape_t<Rank> strides = make_strides(arg.shape(), arg.layout());
  for (size_t i = 0; i < shape.ndim(); ++i) {
    if (arg.shape(i) != shape[i]) {
      if (arg.shape(i) != 1) {
        std::ostringstream error;
        error << "operands could not be broadcast together with shape "
              << arg.shape() << " and requested shape " << shape;
        throw std::invalid_argument(error.str());
      }
      strides[i] = 0;
    }
  }
  return tensor_view<T, Rank>(arg.data(), shape, 0, strides, arg.layout());
}

template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(tensor_view<T, Rank> arg,
                                  const shape_t<Rank> &shape) {
  shape_t<Rank> strides = arg.strides();
  for (size_t i = 0; i < shape.ndim(); ++i) {
    if (arg.shape(i) != shape[i]) {
      if (arg.shape(i) != 1) {
        std::ostringstream error;
        error << "operands could not be broadcast together with shape "
              << arg.shape() << " and requested shape " << shape;
        throw std::invalid_argument(error.str());
      }
      strides[i] = 0;
    }
  }
  return tensor_view<T, Rank>(arg.data(), shape, arg.offset(), strides,
                              arg.layout());
}

template <class T, size_t Rank>
inline tensor_view<T, Rank + 1> expand_dims(tensor<T, Rank> &arg, size_t axis) {
  return expand_dims(arg, make_shape(axis));
}

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank + N> expand_dims(tensor<T, Rank> &arg,
                                     const shape_t<N> &axes) {
  shape_t<Rank + N> shape;
  bool new_axis[Rank + N];
  std::fill_n(new_axis, Rank + N, false);
  for (size_t i = 0; i < axes.ndim(); ++i) {
    shape[axes[i]] = 1;
    new_axis[axes[i]] = true;
  }
  size_t n = 0;
  for (size_t i = 0; i < shape.ndim(); ++i) {
    if (!new_axis[i]) {
      shape[i] = arg.shape(n++);
    }
  }
  return tensor_view<T, Rank + N>(arg.data(), shape, arg.layout());
}

template <class T, size_t Rank>
inline tensor_view<T, Rank + 1> expand_dims(tensor_view<T, Rank> arg,
                                            size_t axis) {
  return expand_dims(arg, make_shape(axis));
}

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank + N> expand_dims(tensor_view<T, Rank> arg,
                                     const shape_t<N> &axes) {
  shape_t<Rank + N> shape, strides;
  bool new_axis[Rank + N];
  std::fill_n(new_axis, Rank + N, false);
  for (size_t i = 0; i < axes.ndim(); ++i) {
    shape[axes[i]] = 1;
    new_axis[axes[i]] = true;
  }
  size_t n = 0;
  for (size_t i = 0; i < shape.ndim(); ++i) {
    if (!new_axis[i]) {
      shape[i] = arg.shape(n);
      strides[i] = arg.strides(n++);
    }
  }
  return tensor_view<T, Rank + N>(arg.data(), shape, arg.offset(), strides,
                                  arg.layout());
}

template <class T, size_t Rank>
inline tensor_view<T, Rank - 1> squeeze(tensor<T, Rank> &arg, size_t axis) {
  return squeeze(arg, make_shape(axis));
}

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank - N> squeeze(tensor<T, Rank> &arg, const shape_t<N> &axes) {
  static_assert(N < Rank, "squeeze cannot take more arguments than the tensor "
                          "dimension");
  shape_t<Rank - N> shape;
  bool keep_axis[Rank];
  std::fill_n(keep_axis, Rank, true);
  for (size_t i = 0; i < axes.ndim(); ++i) {
    keep_axis[axes[i]] = false;
  }
  size_t n = 0;
  for (size_t i = 0; i < arg.ndim(); ++i) {
    if (keep_axis[i]) {
      shape[n++] = arg.shape(i);
    } else if (arg.shape(i) != 1) {
      throw std::invalid_argument("cannot select an axis to squeeze out which "
                                  "has size not equal to one");
    }
  }
  return tensor_view<T, Rank - N>(arg.data(), shape, arg.layout());
}

template <class T, size_t Rank>
inline tensor_view<T, Rank - 1> squeeze(tensor_view<T, Rank> arg, size_t axis) {
  return squeeze(arg, make_shape(axis));
}

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank - N> squeeze(tensor_view<T, Rank> arg,
                                 const shape_t<N> &axes) {
  static_assert(N < Rank, "squeeze cannot take more arguments than the tensor "
                          "dimension");
  shape_t<Rank - N> shape, strides;
  bool keep_axis[Rank];
  std::fill_n(keep_axis, Rank, true);
  for (size_t i = 0; i < axes.ndim(); ++i) {
    keep_axis[axes[i]] = false;
  }
  size_t n = 0;
  for (size_t i = 0; i < arg.ndim(); ++i) {
    if (keep_axis[i]) {
      shape[n] = arg.shape(i);
      strides[n++] = arg.strides(i);
    } else if (arg.shape(i) != 1) {
      throw std::invalid_argument("cannot select an axis to squeeze out which "
                                  "has size not equal to one");
    }
  }
  return tensor_view<T, Rank - N>(arg.data(), shape, arg.offset(), strides,
                                  arg.layout());
}

/// Basic manipulation routines.

template <class U, class T, size_t Rank, class Tag>
inline base_tensor<U, Rank, lazy_unary_tag<cast_to<T, U>, T, Tag>>
astype(const base_tensor<T, Rank, Tag> &a) {
  typedef lazy_unary_tag<cast_to<T, U>, T, Tag> Closure;
  return base_tensor<U, Rank, Closure>(cast_to<T, U>(), a);
}

template <class T, size_t Rank, class Tag>
inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
copy(const base_tensor<T, Rank, Tag> &a) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  return tensor<Rt, Rank>(a);
}

template <class T, size_t Rank, class Tag>
inline tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
flatten(const base_tensor<T, Rank, Tag> &a) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  return tensor<Rt, 1>(a.begin(), a.size());
}

template <class T, size_t Rank, class Tag>
inline tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
flatten(const base_tensor<T, Rank, Tag> &a, layout_t order) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  return tensor<Rt, 1>(a.begin(order), a.size());
}

template <class T, class U, size_t Rank, class TagT, class TagU>
inline base_tensor<std::pair<T, U>, Rank,
                   lazy_binary_tag<detail::zip, T, TagT, U, TagU>>
zip(const base_tensor<T, Rank, TagT> &a, const base_tensor<U, Rank, TagU> &b) {
  typedef std::pair<T, U> Rt;
  typedef lazy_binary_tag<detail::zip, T, TagT, U, TagU> Closure;
  return base_tensor<Rt, Rank, Closure>(detail::zip(), a, b);
}

template <size_t I, class Tuple, size_t Rank, class Tag>
inline base_tensor<typename std::tuple_element<I, Tuple>::type, Rank,
                   lazy_unary_tag<detail::unzip<I>, Tuple, Tag>>
unzip(const base_tensor<Tuple, Rank, Tag> &a) {
  typedef typename std::tuple_element<I, Tuple>::type Rt;
  typedef lazy_unary_tag<detail::unzip<I>, Tuple, Tag> Closure;
  return base_tensor<Rt, Rank, Closure>(detail::unzip<I>(), a);
}

/// Concatenation.

namespace detail {
/**
 * @brief Computes the shape of the concatenation along the given axis. Throws a
 * std::invalid_argument exception if the shapes do not match along the
 * remaining axes.
 */
template <size_t Rank> void concatenation_shape(shape_t<Rank> &, size_t) {}

template <class T, size_t Rank, class Tag, class... Tensors>
void concatenation_shape(shape_t<Rank> &shape, size_t axis,
                         const base_tensor<T, Rank, Tag> &arg1,
                         const Tensors &...arg2) {
  for (size_t i = 0; i < shape.ndim(); ++i) {
    if (arg1.shape(i) != shape[i] && i != axis) {
      std::ostringstream error;
      error << "all the tensor dimensions for the concatenation axis must "
               "match exactly, but along dimension "
            << i << ", tensors have sizes " << shape[i] << " and "
            << arg1.shape(i);
      throw std::invalid_argument(error.str());
    }
  }
  shape[axis] += arg1.shape(axis);
  concatenation_shape(shape, axis, arg2...);
}

/**
 * @brief Concatenate tensors along the given axis.
 */
template <class T, size_t Rank>
void concatenate_impl(tensor<T, Rank> &, size_t, size_t) {}

template <class T, size_t Rank, class Tag, class... Tensors>
void concatenate_impl(tensor<T, Rank> &out, size_t axis, size_t offset,
                      const base_tensor<T, Rank, Tag> &arg1,
                      const Tensors &...arg2) {
  for (index_t<Rank> i : make_index_sequence_for(arg1)) {
    index_t<Rank> out_index = i;
    out_index[axis] += offset;
    out[out_index] = arg1[i];
  }
  concatenate_impl(out, axis, offset + arg1.shape(axis), arg2...);
}
} // namespace detail

template <class T, size_t Rank, class Tag, class... Tensors>
inline tensor<T, Rank> concatenate(const base_tensor<T, Rank, Tag> &arg1,
                                   const Tensors &...arg2) {
  return concatenate(0, arg1, arg2...);
}

template <class T, size_t Rank, class Tag, class... Tensors>
tensor<T, Rank> concatenate(size_t axis, const base_tensor<T, Rank, Tag> &arg1,
                            const Tensors &...arg2) {
  shape_t<Rank> shape = arg1.shape();
  detail::concatenation_shape(shape, axis, arg2...);
  tensor<T, Rank> out(shape);
  detail::concatenate_impl(out, axis, 0, arg1, arg2...);
  return out;
}

namespace detail {
/**
 * @brief Expand the shape dimension by inserting a new axis.
 */
template <size_t Rank>
shape_t<Rank + 1> insert_axis(const shape_t<Rank> &shape, size_t axis,
                              size_t size = 0) {
  shape_t<Rank + 1> new_shape;
  for (size_t i = 0; i < axis; ++i) {
    new_shape[i] = shape[i];
  }
  new_shape[axis] = size;
  for (size_t i = axis; i < shape.ndim(); ++i) {
    new_shape[i + 1] = shape[i];
  }
  return new_shape;
}

/**
 * @brief Reduce the shape dimension by removing an axis.
 */
template <size_t Rank>
shape_t<Rank - 1> remove_axis(const shape_t<Rank> &shape, size_t axis) {
  shape_t<Rank - 1> new_shape;
  for (size_t i = 0; i < axis; ++i) {
    new_shape[i] = shape[i];
  }
  for (size_t i = axis + 1; i < shape.ndim(); ++i) {
    new_shape[i - 1] = shape[i];
  }
  return new_shape;
}

/**
 * @brief Stack tensors along the given axis.
 */
template <class T, size_t Rank>
void stack_impl(tensor<T, Rank> &, size_t, size_t) {}

template <class T, size_t Rank, class Tag, class... Tensors>
void stack_impl(tensor<T, Rank + 1> &out, size_t axis, size_t offset,
                const base_tensor<T, Rank, Tag> &arg1, const Tensors &...arg2) {
  shape_t<Rank> shape = detail::remove_axis(out.shape(), axis);
  if (shape != arg1.shape()) {
    throw std::invalid_argument("all the tensors must have the same shape");
  }
  for (index_t<Rank> i : make_index_sequence_for(arg1)) {
    index_t<Rank + 1> out_index = detail::insert_axis(i, axis, offset);
    out[out_index] = arg1[i];
  }
  stack_impl(out, axis, offset + 1, arg2...);
}
} // namespace detail

template <class T, size_t Rank, class Tag, class... Tensors>
inline tensor<T, Rank + 1> stack(const base_tensor<T, Rank, Tag> &arg1,
                                 const Tensors &...arg2) {
  return stack(0, arg1, arg2...);
}

template <class T, size_t Rank, class Tag, class... Tensors>
tensor<T, Rank + 1> stack(size_t axis, const base_tensor<T, Rank, Tag> &arg1,
                          const Tensors &...arg2) {
  shape_t<Rank + 1> shape = detail::insert_axis(arg1.shape(), axis, 1);
  shape[axis] += sizeof...(arg2);
  tensor<T, Rank + 1> out(shape);
  detail::stack_impl(out, axis, 0, arg1, arg2...);
  return out;
}

/// Tiling.

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
tile(const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &reps) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  shape_t<Rank> shape = a.shape();
  for (size_t i = 0; i < shape.ndim(); ++i) {
    shape[i] *= reps[i];
  }
  tensor<Rt, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    for (size_t i = 0; i < a_index.ndim(); ++i) {
      a_index[i] %= a.shape(i);
    }
    out[out_index] = a[a_index];
  }
  return out;
}

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
repeat(const base_tensor<T, Rank, Tag> &a, size_t reps, size_t axis) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  shape_t<Rank> shape = a.shape();
  shape[axis] *= reps;
  tensor<Rt, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    a_index[axis] /= reps;
    out[out_index] = a[a_index];
  }
  return out;
}

template <class T, size_t Rank, class Tag, class IntegralType, class TagReps,
          detail::RequiresIntegral<IntegralType>>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
repeat(const base_tensor<T, Rank, Tag> &a,
       const base_tensor<IntegralType, 1, TagReps> &reps, size_t axis) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  if (a.shape(axis) != reps.size()) {
    std::ostringstream error;
    error << "size of reps " << reps.size() << " doesn't match tensor size "
          << a.shape(axis) << " along dimension " << axis;
    throw std::invalid_argument(error.str());
  }
  shape_t<Rank> shape = a.shape();
  shape[axis] = std::accumulate(reps.begin(), reps.end(), 0);
  tensor<Rt, Rank> out(shape);
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

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
pad(const base_tensor<T, Rank, Tag> &arg, const shape_t<Rank> &before,
    const shape_t<Rank> &after) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  shape_t<Rank> shape = arg.shape();
  for (size_t i = 0; i < shape.ndim(); ++i) {
    shape[i] += before[i] + after[i];
  }
  tensor<Rt, Rank> out(shape);
  for (index_t<Rank> index : make_index_sequence_for(arg)) {
    index_t<Rank> out_index = index;
    for (size_t i = 0; i < index.ndim(); ++i) {
      out_index[i] += before[i];
    }
    out[out_index] = arg[index];
  }
  return out;
}

template <class T, size_t Rank, class Tag, class Function, class... Args>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
pad(const base_tensor<T, Rank, Tag> &arg, const shape_t<Rank> &before,
    const shape_t<Rank> &after, Function func, Args &&...args) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, Rank> out = std::move(pad(arg, before, after));
  shape_t<Rank> shape = out.shape();
  shape_t<Rank> strides = make_strides(shape);
  for (size_t axis = 0; axis < shape.ndim(); ++axis) {
    size_t size = shape[axis];
    shape[axis] = 1;
    for (index_t<Rank> index : make_index_sequence(shape)) {
      ptrdiff_t offset = 0;
      size_t stride = strides[axis];
      for (size_t i = 0; i < index.ndim(); ++i) {
        offset += index[i] * strides[i];
      }
      tensor_view<Rt, 1> view(out.data(), size, offset, stride);
      func(view, before[axis], after[axis], axis, std::forward<Args>(args)...);
    }
    shape[axis] = size;
  }
  return out;
}

/// Indexing routines.

template <size_t Rank, size_t N, class Tag>
inline base_tensor<
    size_t, N, lazy_unary_tag<detail::ravel_index<Rank>, index_t<Rank>, Tag>>
ravel_index(const base_tensor<index_t<Rank>, N, Tag> &index,
            const shape_t<Rank> &shape, layout_t order) {
  typedef size_t Rt;
  typedef lazy_unary_tag<detail::ravel_index<Rank>, index_t<Rank>, Tag> Closure;
  return base_tensor<Rt, N, Closure>(detail::ravel_index<Rank>(shape, order),
                                     index);
}

template <class IntegralType, size_t Rank, size_t N, class Tag,
          detail::RequiresIntegral<IntegralType>>
inline base_tensor<
    index_t<Rank>, N,
    lazy_unary_tag<detail::unravel_index<Rank>, IntegralType, Tag>>
unravel_index(const base_tensor<IntegralType, N, Tag> &index,
              const shape_t<Rank> &shape, layout_t order) {
  typedef index_t<Rank> Rt;
  typedef lazy_unary_tag<detail::unravel_index<Rank>, IntegralType, Tag>
      Closure;
  return base_tensor<Rt, N, Closure>(detail::unravel_index<Rank>(shape, order),
                                     index);
}

template <class T, size_t Rank, class Tag, size_t N, class TagIndex>
tensor<typename base_tensor<T, Rank, Tag>::value_type, N>
take(const base_tensor<T, Rank, Tag> &a,
     const base_tensor<index_t<Rank>, N, TagIndex> &indices) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  tensor<Rt, N> out(indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    out[i] = a[indices[i]];
  }
  return out;
}

template <class T, class Tag, class IntegralType, size_t N, class TagIndex,
          detail::RequiresIntegral<IntegralType>>
tensor<typename base_tensor<T, 1, Tag>::value_type, N>
take(const base_tensor<T, 1, Tag> &a,
     const base_tensor<IntegralType, N, TagIndex> &indices) {
  typedef typename base_tensor<T, 1, Tag>::value_type Rt;
  tensor<Rt, N> out(indices.shape(), indices.layout());
  for (index_t<N> i : make_index_sequence_for(indices)) {
    out[i] = a[indices[i]];
  }
  return out;
}

template <class T, size_t Rank, class Tag, class IntegralType, class TagIndex,
          detail::RequiresIntegral<IntegralType>>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
take(const base_tensor<T, Rank, Tag> &a,
     const base_tensor<IntegralType, 1, TagIndex> &indices, size_t axis) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  shape_t<Rank> shape = a.shape();
  shape[axis] = indices.size();
  tensor<Rt, Rank> out(shape);
  for (index_t<Rank> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = out_index;
    a_index[axis] = indices[a_index[axis]];
    out[out_index] = a[a_index];
  }
  return out;
}

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank - 1>
take(const base_tensor<T, Rank, Tag> &a, size_t index, size_t axis) {
  static_assert(Rank > 1, "Source tensor must be at least 2-dimensional");
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  shape_t<Rank - 1> shape = detail::remove_axis(a.shape(), axis);
  tensor<Rt, Rank - 1> out(shape);
  for (index_t<Rank - 1> out_index : make_index_sequence(shape)) {
    index_t<Rank> a_index = detail::insert_axis(out_index, axis, index);
    out[out_index] = a[a_index];
  }
  return out;
}

namespace detail {
/**
 * @brief Throws a std::invalid_argument exception if the shape of indices does
 * not match the shape of a tensor for all dimensions other than axis.
 */
template <size_t Rank>
void assert_index_along_axis_shape(const shape_t<Rank> &shape,
                                   const shape_t<Rank> &indices_shape,
                                   size_t axis) {
  for (size_t i = 0; i < shape.ndim(); ++i) {
    if (indices_shape[i] != shape[i] && i != axis) {
      std::ostringstream error;
      error << "index did not match indexed tensor along dimension " << i
            << "; dimension is " << shape[i]
            << " but corresponding index dimension is " << indices_shape[i];
      throw std::invalid_argument(error.str());
    }
  }
}

/**
 * @brief Throws a std::invalid_argument exception if the shape of values does
 * not match the shape of indices.
 */
template <size_t Rank>
void assert_indexed_values_shape(const shape_t<Rank> &indices_shape,
                                 const shape_t<Rank> &values_shape) {
  if (values_shape != indices_shape) {
    std::ostringstream error;
    error << "shape mismatch: value tensor of shape " << values_shape
          << " doesn't match the indexing shape " << indices_shape;
    throw std::invalid_argument(error.str());
  }
}
} // namespace detail

template <class T, size_t Rank, class Tag, class IntegralType, class TagIndex,
          detail::RequiresIntegral<IntegralType>>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
take_along_axis(const base_tensor<T, Rank, Tag> &a,
                const base_tensor<IntegralType, Rank, TagIndex> &indices,
                size_t axis) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  detail::assert_index_along_axis_shape(a.shape(), indices.shape(), axis);
  tensor<Rt, Rank> out(indices.shape(), indices.layout());
  for (index_t<Rank> out_index : make_index_sequence_for(indices)) {
    index_t<Rank> a_index = out_index;
    a_index[axis] = indices[out_index];
    out[out_index] = a[a_index];
  }
  return out;
}

template <class T, size_t Rank, class Tag, class TagIndex, class TagValue>
void put(base_tensor<T, Rank, Tag> &a,
         const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
         const base_tensor<T, 1, TagValue> &values) {
  detail::assert_indexed_values_shape(indices.shape(), values.shape());
  for (size_t i = 0; i < indices.size(); ++i) {
    a[indices[i]] = values[i];
  }
}

template <class T, size_t Rank, class Tag, class TagIndex>
void put(base_tensor<T, Rank, Tag> &a,
         const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
         const typename base_tensor<T, Rank, Tag>::value_type &value) {
  for (size_t i = 0; i < indices.size(); ++i) {
    a[indices[i]] = value;
  }
}

template <class T, class Tag, class IntegralType, class TagIndex,
          class TagValue, detail::RequiresIntegral<IntegralType>>
void put(base_tensor<T, 1, Tag> &a,
         const base_tensor<IntegralType, 1, TagIndex> &indices,
         const base_tensor<T, 1, TagValue> &values) {
  detail::assert_indexed_values_shape(indices.shape(), values.shape());
  for (size_t i = 0; i < indices.size(); ++i) {
    a[indices[i]] = values[i];
  }
}

template <class T, class Tag, class IntegralType, class TagIndex,
          detail::RequiresIntegral<IntegralType>>
void put(base_tensor<T, 1, Tag> &a,
         const base_tensor<IntegralType, 1, TagIndex> &indices,
         const typename base_tensor<T, 1, Tag>::value_type &value) {
  for (size_t i = 0; i < indices.size(); ++i) {
    a[indices[i]] = value;
  }
}

template <class T, size_t Rank, class Tag, class IntegralType, class TagIndex,
          class TagValue, detail::RequiresIntegral<IntegralType>>
void put_along_axis(base_tensor<T, Rank, Tag> &a,
                    const base_tensor<IntegralType, Rank, TagIndex> &indices,
                    const base_tensor<T, Rank, TagValue> &values, size_t axis) {
  detail::assert_index_along_axis_shape(a.shape(), indices.shape(), axis);
  detail::assert_indexed_values_shape(indices.shape(), values.shape());
  for (index_t<Rank> i : make_index_sequence_for(indices)) {
    index_t<Rank> a_index = i;
    a_index[axis] = indices[i];
    a[a_index] = values[i];
  }
}

namespace detail {
/**
 * @brief Throws a std::invalid_argument exception if the shape of the boolean
 * mask does not match the shape of a tensor.
 */
template <size_t Rank>
void assert_mask_shape(const shape_t<Rank> &shape,
                       const shape_t<Rank> &mask_shape) {
  if (shape != mask_shape) {
    std::ostringstream error;
    error << "boolean index did not match indexed tensor; shape is " << shape
          << " but corresponding boolean shape is " << mask_shape;
    throw std::invalid_argument(error.str());
  }
}

/**
 * @brief Throws a std::invalid_argument exception if the size of the boolean
 * mask does not match the size of a tensor along the given axis.
 */
void assert_mask_shape(size_t size, size_t mask_size, size_t axis) {
  if (size != mask_size) {
    std::ostringstream error;
    error << "boolean index did not match indexed tensor along dimension "
          << axis << "; dimension is " << size
          << " but corresponding boolean dimension is " << mask_size;
    throw std::invalid_argument(error.str());
  }
}
} // namespace detail

template <class T, size_t Rank, class Tag, class TagCond>
tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
compress(const base_tensor<T, Rank, Tag> &a,
         const base_tensor<bool, Rank, TagCond> &condition) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  detail::assert_mask_shape(a.shape(), condition.shape());
  size_t size = std::count(condition.begin(), condition.end(), true);
  tensor<Rt, 1> out(size);
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      out[n++] = a[i];
    }
  }
  return out;
}

template <class T, size_t Rank, class Tag, class TagCond>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
compress(const base_tensor<T, Rank, Tag> &a,
         const base_tensor<bool, 1, TagCond> &condition, size_t axis) {
  typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
  detail::assert_mask_shape(a.shape(axis), condition.size(), axis);
  shape_t<Rank> shape = a.shape();
  shape[axis] = std::count(condition.begin(), condition.end(), true);
  tensor<Rt, Rank> out(shape);
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

template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
void place(base_tensor<T, Rank, Tag> &a,
           const base_tensor<bool, Rank, TagCond> &condition,
           const base_tensor<T, 1, TagValue> &values) {
  detail::assert_mask_shape(a.shape(), condition.shape());
  size_t n = 0;
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      a[i] = values[n++];
    }
  }
}

template <class T, size_t Rank, class Tag, class TagCond>
void place(base_tensor<T, Rank, Tag> &a,
           const base_tensor<bool, Rank, TagCond> &condition,
           const typename base_tensor<T, Rank, Tag>::value_type &value) {
  detail::assert_mask_shape(a.shape(), condition.shape());
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      a[i] = value;
    }
  }
}

template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
void putmask(base_tensor<T, Rank, Tag> &a,
             const base_tensor<bool, Rank, TagCond> &condition,
             const base_tensor<T, Rank, TagValue> &values) {
  detail::assert_mask_shape(a.shape(), condition.shape());
  detail::assert_indexed_values_shape(condition.shape(), values.shape());
  for (index_t<Rank> i : make_index_sequence_for(condition)) {
    if (condition[i]) {
      a[i] = values[i];
    }
  }
}
} // namespace numcpp

#endif // NUMCPP_BROADCASTING_TCC_INCLUDED
