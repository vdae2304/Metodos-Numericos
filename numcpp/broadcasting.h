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

/** @file include/numcpp/broadcasting.h
 *  This header defines routines for broadcasting and advanced indexing.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_BROADCASTING_H_INCLUDED
#define NUMCPP_BROADCASTING_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/broadcasting/utilities.h"

namespace numcpp {
/// Broadcasting.

/**
 * @brief Broadcast a value to a new shape.
 *
 * @details Return a @c tensor_view of given shape filled with @a val. This
 * function does not create a new tensor, instead, it broadcasts the given value
 * as a tensor of given shape. All the elements in the broadcasted tensor refer
 * to the same memory location, and thus shall not be modified.
 *
 * @param val Reference to the fill value.
 * @param shape The shape of the desired tensor.
 *
 * @return A view of the original value with the given shape.
 */
template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(T &val, const shape_t<Rank> &shape);

/**
 * @brief Broadcast a tensor to a new shape.
 *
 * @details Return a @c tensor_view of given shape constructed by repeating
 * elements along each axis of size one. This function does not create a new
 * tensor, instead, it broadcasts the given tensor to a new shape. Some elements
 * in the broadcasted tensor may refer to the same memory location, and thus
 * shall not be modified.
 *
 * @param arg Reference to the tensor or tensor_view to broadcast.
 * @param shape The shape of the desired tensor.
 *
 * @return A view of the original tensor with the given shape.
 *
 * @throw std::invalid_argument Thrown if the tensor is not compatible with the
 *                              new shape according to broadcasting rules.
 */
template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(tensor<T, Rank> &arg,
                                  const shape_t<Rank> &shape);

template <class T, size_t Rank>
tensor_view<T, Rank> broadcast_to(tensor_view<T, Rank> arg,
                                  const shape_t<Rank> &shape);

/**
 * @brief Expand the shape of a tensor by inserting new axes.
 *
 * @param arg Input tensor.
 * @param axes Position in the expanded axes where the new axis (or axes) is
 *             placed.
 *
 * @return A view of the original tensor with the number of dimensions
 *         increased.
 */
template <class T, size_t Rank>
tensor_view<T, Rank + 1> expand_dims(tensor<T, Rank> &arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank + N> expand_dims(tensor<T, Rank> &arg,
                                     const shape_t<N> &axes);

template <class T, size_t Rank>
tensor_view<T, Rank + 1> expand_dims(tensor_view<T, Rank> arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank + N> expand_dims(tensor_view<T, Rank> arg,
                                     const shape_t<N> &axes);

/**
 * @brief Removes axes of length one.
 *
 * @param arg Input tensor.
 * @param axes Selects a subset of the entries of length one in the shape.
 *
 * @return A view of the original tensor with the axes removed.
 *
 * @throw std::invalid_argument Thrown if an axis with shape entry greater than
 *                              one is selected.
 */
template <class T, size_t Rank>
tensor_view<T, Rank - 1> squeeze(tensor<T, Rank> &arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank - N> squeeze(tensor<T, Rank> &arg, const shape_t<N> &axes);

template <class T, size_t Rank>
tensor_view<T, Rank - 1> squeeze(tensor_view<T, Rank> arg, size_t axis);

template <size_t N, class T, size_t Rank>
tensor_view<T, Rank - N> squeeze(tensor_view<T, Rank> arg,
                                 const shape_t<N> &axes);

/// Tensor creation routines from existing data.

/**
 * @brief Convert the input to a tensor. The data-type and dimension of the
 * tensor is deduced from the input data.
 *
 * @param first Input iterator to the initial position in a range.
 * @param last Input iterator to the final position in a range. The range used
 *             is @a [first,last), which includes all the elements between
 *             @a first and @a last, including the element pointed by @a first
 *             but not the element pointed by @a last.
 * @param shape Number of elements along each axis. It can be a shape_t object
 *              or the elements of the shape passed as separate arguments.
 * @param order Memory layout in which elements are stored. In row-major order,
 *              the last dimension is contiguous. In column-major order, the
 *              first dimension is contiguous. Defaults to row-major order.
 *
 * @return Tensor interpretation of the input data.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class InputIterator, detail::RequiresInputIterator<InputIterator> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type, 1>
asarray(InputIterator first, InputIterator last);

template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator> = 0,
          detail::RequiresIntegral<Sizes...> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
asarray(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asarray(InputIterator first, const shape_t<Rank> &shape,
        layout_t order = default_layout);

/**
 * @brief Return a tensor laid out in C/C++ (row-major) order in memory.
 *
 * @param first Input iterator to the initial position in a range.
 * @param shape Number of elements along each axis. It can be a shape_t object
 *              or the elements of the shape passed as separate arguments.
 *
 * @return Tensor interpretation of the input data in row-major order.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator> = 0,
          detail::RequiresIntegral<Sizes...> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
ascontiguousarray(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
ascontiguousarray(InputIterator first, const shape_t<Rank> &shape);

/**
 * @brief Return a tensor laid out in Fortran/Matlab (column-major) order in
 * memory.
 *
 * @param first Input iterator to the initial position in a range.
 * @param shape Number of elements along each axis. It can be a shape_t object
 *              or the elements of the shape passed as separate arguments.
 *
 * @return Tensor interpretation of the input data in column-major order.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator> = 0,
          detail::RequiresIntegral<Sizes...> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
asfortranarray(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asfortranarray(InputIterator first, const shape_t<Rank> &shape);

/**
 * @brief Convert the input to a tensor, checking for NaNs or Infs.
 *
 * @param first Input iterator to the initial position in a range.
 * @param last Input iterator to the final position in a range. The range used
 *             is @a [first,last), which includes all the elements between
 *             @a first and @a last, including the element pointed by @a first
 *             but not the element pointed by @a last.
 * @param shape Number of elements along each axis. It can be a shape_t object
 *              or the elements of the shape passed as separate arguments.
 * @param order Memory layout in which elements are stored. In row-major order,
 *              the last dimension is contiguous. In column-major order, the
 *              first dimension is contiguous. Defaults to row-major order.
 *
 * @return Tensor interpretation of the input data.
 *
 * @throw std::invalid_argument Thrown if input data contains NaN (Not a Number)
 *                              or Inf (Infinity).
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class InputIterator, detail::RequiresInputIterator<InputIterator> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type, 1>
asarray_chkfinite(InputIterator first, InputIterator last);

template <class InputIterator, class... Sizes,
          detail::RequiresInputIterator<InputIterator> = 0,
          detail::RequiresIntegral<Sizes...> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type,
       sizeof...(Sizes)>
asarray_chkfinite(InputIterator first, Sizes... sizes);

template <class InputIterator, size_t Rank,
          detail::RequiresInputIterator<InputIterator> = 0>
tensor<typename std::iterator_traits<InputIterator>::value_type, Rank>
asarray_chkfinite(InputIterator first, const shape_t<Rank> &shape,
                  layout_t order = default_layout);

/// Basic manipulation routines.

/**
 * @brief Cast each element in a tensor to a specified type.
 *
 * @tparam U Data type to which the elements are casted.
 * @param a Tensor-like object to cast.
 *
 * @return A light-weight object with the elements in the tensor casted to the
 *         specified type. This function does not create a new tensor, instead,
 *         it returns a readonly view of the tensor with its elements casted to
 *         the specified type.
 */
template <class U, class T, size_t Rank, class Tag>
base_tensor<U, Rank, lazy_unary_tag<cast_to<T, U>, T, Tag>>
astype(const base_tensor<T, Rank, Tag> &a);

/**
 * @brief Return a copy of the tensor.
 *
 * @param a Tensor-like object to copy.
 *
 * @return A new tensor with a copy of each of the elements in the tensor.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
copy(const base_tensor<T, Rank, Tag> &a);

/**
 * @brief Return a copy of the tensor flattened into one dimension.
 *
 * @param a Tensor-like object to flatten.
 * @param order Whether to flatten the elements in row-major order or
 *              column-major order. The default is to use the same layout as
 *              stored in memory.
 *
 * @return The flattened tensor.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
flatten(const base_tensor<T, Rank, Tag> &a);

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
flatten(const base_tensor<T, Rank, Tag> &a, layout_t order);

/**
 * @brief Return a tensor of pairs with each pair constructed from the
 * corresponding elements in two tensors.
 *
 * @param a First tensor-like argument.
 * @param b Second tensor-like argument.
 *
 * @return A light-weight object with the pairs constructed from the elements of
 *         two tensors. This function does not create a new tensor, instead, it
 *         returns a readonly view with the pairs constructed from both tensors.
 */
template <class T, class U, size_t Rank, class TagT, class TagU>
base_tensor<std::pair<T, U>, Rank,
            lazy_binary_tag<detail::zip, T, TagT, U, TagU>>
zip(const base_tensor<T, Rank, TagT> &a, const base_tensor<U, Rank, TagU> &b);

/**
 * @brief Return a tensor with each element constructed from taking the I-th
 * element of the corresponding tuple in a tensor.
 *
 * @tparam I Element index.
 * @param a A tensor-like object of tuple-like values. Any class overloading
 *          @c std::tuple_element and @c std::get, such as @c std::pair,
 *          @c std::tuple and @c std::array, is considered a tuple.
 *
 * @return A light-weight object with the I-th element of each tuple-like in the
 *         tensor. This function does not create a new tensor, instead, it
 *         returns a readonly view with the I-th element of each tuple.
 */
template <size_t I, class Tuple, size_t Rank, class Tag>
base_tensor<typename std::tuple_element<I, Tuple>::type, Rank,
            lazy_unary_tag<detail::unzip<I>, Tuple, Tag>>
unzip(const base_tensor<Tuple, Rank, Tag> &a);

/// Concatenation.

/**
 * @brief Concatenate one or more tensors.
 *
 * @param axis The axis along which the tensors will be concatenated. Default is
 *             0.
 * @param arg1, arg2... Tensor-like objects to concatenate. The tensors must
 *                      have the same data type and the same shape, except in
 *                      the dimension corresponding to @a axis.
 *
 * @return The concatenated tensors.
 *
 * @throw std::invalid_argument Thrown if the tensors do not have the same shape
 *                              along all the axes other than @a axis.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class... Tensors>
tensor<T, Rank> concatenate(const base_tensor<T, Rank, Tag> &arg1,
                            const Tensors &...arg2);

template <class T, size_t Rank, class Tag, class... Tensors>
tensor<T, Rank> concatenate(size_t axis, const base_tensor<T, Rank, Tag> &arg1,
                            const Tensors &...arg2);

/**
 * @brief Concatenate one or more tensors along a new axis.
 *
 * @param axis The axis in the result tensor along which the tensors will be
 *             stacked. Default is 0.
 * @param arg1, arg2... Tensor-like objects to stack. The tensors must have the
 *                      same data type and the same shape.
 *
 * @return The stacked tensors.
 *
 * @throw std::invalid_argument Thrown if the tensors do not have the same
 *                              shape.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class... Tensors>
tensor<T, Rank + 1> stack(const base_tensor<T, Rank, Tag> &arg1,
                          const Tensors &...arg2);

template <class T, size_t Rank, class Tag, class... Tensors>
tensor<T, Rank + 1> stack(size_t axis, const base_tensor<T, Rank, Tag> &arg1,
                          const Tensors &...arg2);

/// Tiling.

/**
 * @brief Construct a tensor by repeating @a a the number of times given by
 * @a reps.
 *
 * @param a The input tensor.
 * @param reps The number of repetitions of @a a along each axis.
 *
 * @return The tiled tensor.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
tile(const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &reps);

/**
 * @brief Repeat elements of a tensor.
 *
 * @param a The input tensor.
 * @param reps The number of repetitions of each element.
 * @param axis The axis along which to repeat values. Default is 0.
 *
 * @return A new tensor which has the same shape as @a a, except along the given
 *         axis.
 *
 * @throw std::invalid_argument Thrown if the size of @a reps does not match the
 *                              size of @a a along the given axis.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
repeat(const base_tensor<T, Rank, Tag> &a, size_t reps, size_t axis = 0);

template <class T, size_t Rank, class Tag, class IntegralType, class TagReps,
          detail::RequiresIntegral<IntegralType> = 0>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
repeat(const base_tensor<T, Rank, Tag> &a,
       const base_tensor<IntegralType, 1, TagReps> &reps, size_t axis = 0);

/// Padding.

/**
 * @brief Pad a tensor.
 *
 * @param arg The tensor to pad.
 * @param before Number of elements to pad at the beginning of each axis.
 * @param after Number of elements to pad at the end of each axis.
 *
 * @return The padded tensor. Padded elements are left uninitialized.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
pad(const base_tensor<T, Rank, Tag> &arg, const shape_t<Rank> &before,
    const shape_t<Rank> &after);

/**
 * @brief Pad a tensor.
 *
 * @param arg The tensor to pad.
 * @param before Number of elements to pad at the beginning of each axis.
 * @param after Number of elements to pad at the end of each axis.
 * @param func Padding function. For tensors with rank greater than 1, the
 * padding of later axes depends on the padding of previous axes. The signature
 * of the padding function should be equivalent to the following:
 *   void func(tensor_view<T, 1> &view, size_t before, size_t after,
 *             size_t axis, Args&&... args);
 *   where
 *   - @a view A 1-dimensional view already padded with uninitialized values.
 *   - @a before Number of elements padded at the beginning of @a view.
 *   - @a after Number of elements padded at the end of @a view.
 *   - @a axis The axis currently being calculated.
 * @param args... Additional arguments to pass to @a func.
 *
 * @return The padded tensor.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class Function, class... Args>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
pad(const base_tensor<T, Rank, Tag> &arg, const shape_t<Rank> &before,
    const shape_t<Rank> &after, Function func, Args &&...args);

/// Indexing routines.

/**
 * @brief Converts a tuple of indices into a flat index, element-wise.
 *
 * @param index A tensor-like object with the indices to flatten.
 * @param shape The shape of the tensor used for raveling.
 * @param order Determines whether the indices should be viewed as indexing in
 *              row-major or column-major order. Defaults to row-major order.
 *
 * @return A light-weight object with the flattened indices. This function does
 *         not create a new tensor, instead, an expression object is returned.
 */
template <size_t Rank, size_t N, class Tag>
base_tensor<size_t, N,
            lazy_unary_tag<detail::ravel_index<Rank>, index_t<Rank>, Tag>>
ravel_index(const base_tensor<index_t<Rank>, N, Tag> &index,
            const shape_t<Rank> &shape, layout_t order = default_layout);

/**
 * @brief Converts a flat index into a tuple of indices, element-wise.
 *
 * @param index A tensor-like object with the indices to unravel.
 * @param shape The shape of the tensor used for unraveling.
 * @param order Determines whether the indices should be viewed as indexing in
 *              row-major or column-major order. Defaults to row-major order.
 *
 * @return A light-weight object with the unraveled indices. This function does
 *         not create a new tensor, instead, an expression object is returned.
 */
template <class IntegralType, size_t Rank, size_t N, class Tag,
          detail::RequiresIntegral<IntegralType> = 0>
base_tensor<index_t<Rank>, N,
            lazy_unary_tag<detail::unravel_index<Rank>, IntegralType, Tag>>
unravel_index(const base_tensor<IntegralType, N, Tag> &index,
              const shape_t<Rank> &shape, layout_t order = default_layout);

/**
 * @brief Take elements from a tensor. For @c tensor class, a call such as
 * @c np::take(a,indices) is equivalent to @c a[indices], except that a copy is
 * always returned.
 *
 * @param a The source tensor.
 * @param indices A tensor-like object of @c index_t with the indices of the
 *                values to take. If the source tensor is 1-dimensional, a
 *                tensor-like object of integers can be used instead.
 *
 * @return A new tensor with the elements from @a a at the given positions.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, size_t N, class TagIndex>
tensor<typename base_tensor<T, Rank, Tag>::value_type, N>
take(const base_tensor<T, Rank, Tag> &a,
     const base_tensor<index_t<Rank>, N, TagIndex> &indices);

template <class T, class Tag, class IntegralType, size_t N, class TagIndex,
          detail::RequiresIntegral<IntegralType> = 0>
tensor<typename base_tensor<T, 1, Tag>::value_type, N>
take(const base_tensor<T, 1, Tag> &a,
     const base_tensor<IntegralType, N, TagIndex> &indices);

/**
 * @brief Take elements from a tensor along an axis.
 *
 * @param a The source tensor.
 * @param indices A tensor-like object with the indices of the values to take.
 * @param axis The axis over which to select values.
 *
 * @return A new tensor with the elements from @a a at the given positions. If a
 *         single index is given, the output tensor will have a dimension less
 *         than the source tensor. Otherwise, the output tensor and the source
 *         tensor will have the same dimension and the same shape, except for
 *         the axis over which the values are taken.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class IntegralType, class TagIndex,
          detail::RequiresIntegral<IntegralType> = 0>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
take(const base_tensor<T, Rank, Tag> &a,
     const base_tensor<IntegralType, 1, TagIndex> &indices, size_t axis);

template <class T, size_t Rank, class Tag>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank - 1>
take(const base_tensor<T, Rank, Tag> &a, size_t index, size_t axis);

/**
 * @brief Take values from the input tensor by matching 1d index and data
 * slices. Functions returning an index along an axis, like @c argsort and
 * @c argpartition, produce suitable indices for this function.
 *
 * @param a The source tensor.
 * @param indices A tensor-like object with the indices to take along each 1d
 *                slice of @a a. This must match the shape of @a a for all
 *                dimensions other than @a axis.
 * @param axis The axis to take 1d slices along.
 *
 * @return A new tensor with the elements from @a a at the given positions.
 *
 * @throw std::invalid_argument Thrown if the shape of @a indices is not
 *                              compatible with the shape of @a a.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class IntegralType, class TagIndex,
          detail::RequiresIntegral<IntegralType> = 0>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
take_along_axis(const base_tensor<T, Rank, Tag> &a,
                const base_tensor<IntegralType, Rank, TagIndex> &indices,
                size_t axis);

/**
 * @brief Replaces specified elements of a tensor with given values. For
 * @c tensor class, a call such as @c np::put(a,indices,values) is equivalent to
 * @c a[indices]=values.
 *
 * @param a Target tensor.
 * @param indices A tensor-like object of @c index_t with the target indices. If
 *                the source tensor is 1-dimensional, a tensor-like object of
 *                integers can be used instead.
 * @param values A single value or a tensor-like object with the values to put
 *               into @a a at target indices.
 *
 * @throw std::invalid_argument Thrown if @a indices and @a values have
 *                              different shapes.
 */
template <class T, size_t Rank, class Tag, class TagIndex, class TagValue>
void put(base_tensor<T, Rank, Tag> &a,
         const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
         const base_tensor<T, 1, TagValue> &values);

template <class T, size_t Rank, class Tag, class TagIndex>
void put(base_tensor<T, Rank, Tag> &a,
         const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
         const typename base_tensor<T, Rank, Tag>::value_type &value);

template <class T, class Tag, class IntegralType, class TagIndex,
          class TagValue, detail::RequiresIntegral<IntegralType> = 0>
void put(base_tensor<T, 1, Tag> &a,
         const base_tensor<IntegralType, 1, TagIndex> &indices,
         const base_tensor<T, 1, TagValue> &values);

template <class T, class Tag, class IntegralType, class TagIndex,
          detail::RequiresIntegral<IntegralType> = 0>
void put(base_tensor<T, 1, Tag> &a,
         const base_tensor<IntegralType, 1, TagIndex> &indices,
         const typename base_tensor<T, 1, Tag>::value_type &value);

/**
 * @brief Put values into the destination tensor by matching 1d index and data
 * slices. Functions returning an index along an axis, like @c argsort and
 * @c argpartition, produce suitable indices for this function.
 *
 * @param a Destination tensor.
 * @param indices A tensor-like object with the indices to change along each 1d
 *                slice of @a a. This must match the shape of @a a for all
 *                dimensions other than @a axis.
 * @param values A tensor-like object with the values to insert at those
 *               indices. This must match the shape of @a indices.
 * @param axis The axis to take 1d slices along.
 *
 * @throw std::invalid_argument Thrown if the shapes of @a indices and @a values
 *                              are not compatible with the shape of @a a.
 */
template <class T, size_t Rank, class Tag, class IntegralType, class TagIndex,
          class TagValue, detail::RequiresIntegral<IntegralType> = 0>
void put_along_axis(base_tensor<T, Rank, Tag> &a,
                    const base_tensor<IntegralType, Rank, TagIndex> &indices,
                    const base_tensor<T, Rank, TagValue> &values, size_t axis);

/**
 * @brief Return the elements of a tensor that satisfy some condition. For
 * @c tensor class, a call such as @c np::compress(a,condition) is equivalent to
 * @c a[condition], except that a copy is always returned.
 *
 * @param a The input tensor.
 * @param condition A tensor-like object of @c bool whose true entries indicate
 *                  the elements of @a a to return.
 *
 * @return A new tensor with the elements from @a a where @a condition is true.
 *
 * @throw std::invalid_argument Thrown if @a a and @a condition have different
 *                              shapes.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class TagCond>
tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
compress(const base_tensor<T, Rank, Tag> &a,
         const base_tensor<bool, Rank, TagCond> &condition);

/**
 * @brief Return the elements of a tensor that satisfy some condition along
 * given axis.
 *
 * @param a The input tensor.
 * @param condition A tensor-like object of @c bool whose true entries indicate
 *                  the elements of @a a to return.
 * @param axis The axis over which to select values.
 *
 * @return A new tensor with the elements from @a a where @a condition is true.
 *         The output tensor and the input tensor will have the same dimension
 *         and the same shape, except for the axis over which the values are
 *         selected.
 *
 * @throw std::invalid_argument Thrown if the size of @a condition does not
 *                              match the size of @a a along the given axis.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank, class Tag, class TagCond>
tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
compress(const base_tensor<T, Rank, Tag> &a,
         const base_tensor<bool, 1, TagCond> &condition, size_t axis);

/**
 * @brief Change elements of a tensor based on conditional and input values.
 *
 * @param a Tensor to put data into.
 * @param condition Boolean mask tensor.
 * @param values A single value or a tensor-like object with the values to place
 *               in @a a. Only the first @a N elements are used, where @a N is
 *               the number of true values in @a condition.
 *
 * @throw std::invalid_argument Thrown if @a a and @a condition have different
 *                              shapes.
 */
template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
void place(base_tensor<T, Rank, Tag> &a,
           const base_tensor<bool, Rank, TagCond> &condition,
           const base_tensor<T, 1, TagValue> &values);

template <class T, size_t Rank, class Tag, class TagCond>
void place(base_tensor<T, Rank, Tag> &a,
           const base_tensor<bool, Rank, TagCond> &condition,
           const typename base_tensor<T, Rank, Tag>::value_type &value);

/**
 * @brief Change elements of a tensor based on conditional and input values.
 * Similar to @c place, the difference is that @c place uses the first @a N
 * elements of @a values, where @a N is the number of true values in
 * @a condition, while @c putmask uses the elements where @a condition is true.
 *
 * @param a Tensor to put data into.
 * @param condition Boolean mask tensor.
 * @param values A tensor-like object with the values to put into @a a where
 *               @a condition is true.
 *
 * @throw std::invalid_argument Thrown if @a a, @a condition or @a values have
 *                              different shapes.
 */
template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
void putmask(base_tensor<T, Rank, Tag> &a,
             const base_tensor<bool, Rank, TagCond> &condition,
             const base_tensor<T, Rank, TagValue> &values);
} // namespace numcpp

#include "numcpp/broadcasting/broadcasting.tcc"

#endif // NUMCPP_BROADCASTING_H_INCLUDED
