/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
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

#include "numcpp/tensor.h"
#include "numcpp/broadcasting/utilities.h"
#include "numcpp/broadcasting/lazy_reverse.h"
#include "numcpp/broadcasting/lazy_shift.h"

namespace numcpp {
    /// Broadcasting.

    /**
     * @brief Broadcast a value to a new shape.
     *
     * @details Return a tensor_view of given shape filled with val. This
     * function does not create a new tensor, instead, it broadcasts the given
     * value as a tensor of given shape. All the elements in the broadcasted
     * tensor refer to the same memory location, and thus shall not be
     * modified.
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
     * @details Return a tensor_view of given shape constructed by repeating
     * elements along each axis of size one. This function does not create a
     * new tensor, instead, it broadcasts the given tensor to a new shape. Some
     * elements in the broadcasted tensor may refer to the same memory
     * location, and thus shall not be modified.
     *
     * @param arg Reference to the tensor or tensor_view to broadcast.
     * @param shape The shape of the desired tensor.
     *
     * @return A view of the original tensor with the given shape.
     *
     * @throw std::invalid_argument Thrown if the tensor is not compatible with
     *     the new shape according to broadcasting rules.
     */
    template <class T, size_t Rank>
    tensor_view<T, Rank> broadcast_to(
        tensor<T, Rank> &arg, const shape_t<Rank> &shape
    );

    template <class T, size_t Rank>
    tensor_view<T, Rank> broadcast_to(
        tensor_view<T, Rank> arg, const shape_t<Rank> &shape
    );

    /**
     * @brief Expand the shape of a tensor by inserting new axes.
     *
     * @param arg Input tensor.
     * @param axes Position in the expanded axes where the new axis (or axes)
     *     is placed.
     *
     * @return A view of the original tensor with the number of dimensions
     *     increased.
     */
    template <class T, size_t Rank>
    tensor_view<T, Rank + 1> expand_dims(tensor<T, Rank> &arg, size_t axis);

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank + N> expand_dims(
        tensor<T, Rank> &arg, const shape_t<N> &axes
    );

    template <class T, size_t Rank>
    tensor_view<T, Rank + 1> expand_dims(tensor_view<T, Rank> arg, size_t axis);

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank + N> expand_dims(
        tensor_view<T, Rank> arg, const shape_t<N> &axes
    );

    /**
     * @brief Removes axes of length one.
     *
     * @param arg Input tensor.
     * @param axes Selects a subset of the entries of length one in the shape.
     *
     * @return A view of the original tensor with the axes removed.
     *
     * @throw std::invalid_argument Thrown if an axis with shape entry greater
     *     than one is selected.
     */
    template <class T, size_t Rank>
    tensor_view<T, Rank - 1> squeeze(tensor<T, Rank> &arg, size_t axis);

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank - N> squeeze(
        tensor<T, Rank> &arg, const shape_t<N> &axes
    );

    template <class T, size_t Rank>
    tensor_view<T, Rank - 1> squeeze(tensor_view<T, Rank> arg, size_t axis);

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank - N> squeeze(
        tensor_view<T, Rank> arg, const shape_t<N> &axes
    );

    /// Basic manipulation routines.

    /**
     * @brief Cast each element in a tensor to a specified type.
     *
     * @tparam U Data type to which the elements are casted.
     * @param a Tensor-like object to cast.
     *
     * @return A light-weight object with the elements in the tensor casted to
     *     the specified type. This function does not create a new tensor,
     *     instead, it returns a readonly view of the tensor with its elements
     *     casted to the specified type.
     */
    template <class U, class T, size_t Rank, class Tag>
    base_tensor<U, Rank, lazy_unary_tag<cast_to<U>, T, Tag> >
    astype(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return a copy of the tensor.
     *
     * @param a Tensor-like object to copy.
     *
     * @return A copy of the tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    copy(const base_tensor<T, Rank, Tag> &a);

    /**
     * @brief Return a copy of the tensor flattened into one dimension.
     *
     * @param a Tensor-like object to flatten.
     * @param row_major Whether to flatten the elements in row-major order
     *     (i.e., from first axis to last axis) or column-major order (i.e.,
     *     from last axis to first axis). Defaults to the same order as stored
     *     in memory.
     *
     * @return The flattened tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    flatten(const base_tensor<T, Rank, Tag> &a);

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    flatten(const base_tensor<T, Rank, Tag> &a, bool row_major);

    /**
     * @brief Return a tensor of pairs with each pair constructed from the
     * corresponding elements in two tensors.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return A light-weight object of pairs constructed from the elements of
     *     two tensors. This function does not create a new tensor, instead, it
     *     returns a readonly view with the pairs constructed from both tensors.
     */
    template <class T, class U, size_t Rank, class TagT, class TagU>
    base_tensor<
        std::pair<T, U>, Rank, lazy_binary_tag<detail::zip, T, TagT, U, TagU>
    > zip(
        const base_tensor<T, Rank, TagT> &a,
        const base_tensor<U, Rank, TagU> &b
    );

    /**
     * @brief Return a tensor with each element constructed from taking the
     * I-th element of the corresponding pair in a tensor object.
     *
     * @tparam I Element index. It shall be 0 or 1.
     * @param a A tensor-like object of pairs.
     *
     * @return A light-weight object with the I-th element of each pair in the
     *     tensor. This function does not create a new tensor, instead, it
     *     returns a readonly view with the I-th element of each pair.
     */
    template <size_t I, class T, class U, size_t Rank, class Tag>
    base_tensor<
        typename std::enable_if<(I == 0), T>::type, Rank,
        lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag>
    > unzip(const base_tensor<std::pair<T, U>, Rank, Tag> &a);

    template <size_t I, class T, class U, size_t Rank, class Tag>
    base_tensor<
        typename std::enable_if<(I == 1), U>::type, Rank,
        lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag>
    > unzip(const base_tensor<std::pair<T, U>, Rank, Tag> &a);

    /// Concatenation.

    /**
     * @brief Concatenate one or more tensors.
     *
     * @param axis The axis along which the tensors will be concatenated.
     * @param arg1, arg2... Tensor-like objects to concatenate. The tensors
     *     must have the same data type and the same shape, except in the
     *     dimension corresponding to axis.
     *
     * @throw std::invalid_argument Thrown if the tensors do not have the same
     *     shape along all the axes other than axis.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag, class... Args>
    tensor<T, Rank> concatenate(
        size_t axis, const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    );

    /**
     * @brief Concatenate one or more tensors along a new axis.
     *
     * @param axis The axis in the result tensor along which the tensors will
     *     be stacked.
     * @param arg1, arg2... Tensor-like objects to stack. The tensors must have
     *     the same data type and the same shape.
     *
     * @throw std::invalid_argument Thrown if the tensors do not have the same
     *     shape.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag, class... Args>
    tensor<T, Rank + 1> stack(
        size_t axis, const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    );

    /// Tiling.

    /**
     * @brief Construct a tensor by repeating a the number of times given by
     * reps.
     *
     * @param a The input tensor.
     * @param reps The number of repetitions of a along each axis.
     *
     * @return The tiled tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> tile(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &reps
    );

    /**
     * @brief Repeat elements of a tensor.
     *
     * @param a The input tensor.
     * @param reps The number of repetitions of each element.
     * @param axis The axis along which to repeat values. Default is 0.
     *
     * @return A new tensor which has the same shape as a, except along the
     *     given axis.
     *
     * @throw std::invalid_argument Thrown if the size of reps does not match
     *     the size of a along the given axis.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> repeat(
        const base_tensor<T, Rank, Tag> &a, size_t reps, size_t axis = 0
    );

    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagReps,
              detail::RequiresIntegral<IntegralType> = true>
    tensor<T, Rank> repeat(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, 1, TagReps> &reps,
        size_t axis = 0
    );

    /// Rearranging elements.

    /**
     * @brief Reverse the order of the elements in a tensor along the given
     * axes.
     *
     * @param arg A tensor-like object to reverse.
     * @param axes Axis or axes along which to reverse over. The default is
     *     Rank - 1, which reverses along the last axis.
     *
     * @return A light-weight object with the elements in the tensor in
     *     reversed order. This function does not create a new tensor, instead,
     *     it returns a readonly view of the tensor with its elements in
     *     reversed order over the given axes.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_reverse_tag<Tag, 1> > reverse(
        const base_tensor<T, Rank, Tag> &arg, size_t axis = Rank - 1
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, lazy_reverse_tag<Tag, N> > reverse(
        const base_tensor<T, Rank, Tag> &arg, const shape_t<N> &axes
    );

    /**
     * @brief Shift the elements in a tensor along the given axes. The elements
     * are circularly shifted in such a way that the element at position count
     * becomes the first element.
     *
     * @param arg A tensor-like object to shift.
     * @param count Number of positions to shift the elements by along each
     *     axis.
     * @param axes Axis or axes along which the elements are shifted. The
     *     default is Rank - 1, which shifts along the last axis.
     *
     * @return A light-weight object with the elements in the tensor circularly
     *     shifted. This function does not create a new tensor, instead, it
     *     returns a readonly view of the tensor with its elements circularly
     *     shifted over the given axes.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, lazy_shift_tag<Tag, 1> > shift(
        const base_tensor<T, Rank, Tag> &arg,
        size_t count, size_t axis = Rank - 1
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor<T, Rank, lazy_shift_tag<Tag, N> > shift(
        const base_tensor<T, Rank, Tag> &arg,
        const index_t<N> &count, const shape_t<N> &axes
    );

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
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> pad(
        const base_tensor<T, Rank, Tag> &arg,
        const shape_t<Rank> &before, const shape_t<Rank> &after
    );

    /**
     * @brief Pad a tensor.
     *
     * @param arg The tensor to pad.
     * @param before Number of elements to pad at the beginning of each axis.
     * @param after Number of elements to pad at the end of each axis.
     * @param func Padding function. The signature of the padding function
     *     should be equivalent to the following:
     *         void func(tensor_view<T, 1> &view, size_t before, size_t after,
     *                   size_t axis, Args&&... args);
     *     where
     *     - view A one dimensional view already padded with uninitialized
     *       values.
     *     - before Number of elements padded at the beginning of view.
     *     - after Number of elements padded at the end of view.
     *     - axis The axis currently being calculated. The padding is done from
     *       the first axis to the last axis.
     * @param args... Additional arguments to pass to func.
     *
     * @return The padded tensor.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag, class Function, class... Args>
    tensor<T, Rank> pad(
        const base_tensor<T, Rank, Tag> &arg,
        const shape_t<Rank> &before, const shape_t<Rank> &after,
        Function func, Args&&... args
    );

    /**
     * @brief Pads with a constant value.
     *
     * @param args... The values to set the padded elements for each axis.
     * - If no arguments are passed, all the axes are padded with zeros.
     * - If a single value is passed, the same constant is used for all the
     *   axes.
     * - If two values are passed, the same before and after constants are used
     *   for each axis.
     * - If a matrix of values is passed, unique pad constants are used for
     *   each axis, where values(i, 0) and values(i, 1) are the before and
     *   after constants for axis i.
     */
    struct pad_constant {
        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
        ) const;

        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
            const typename tensor<T, 1>::value_type &val
        ) const;

        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
            const typename tensor<T, 1>::value_type &val_before,
            const typename tensor<T, 1>::value_type &val_after
        ) const;

        template <class T, class Tag>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
            const base_tensor<T, 2, Tag> &values
        ) const;
    };

    /**
     * @brief Pads with the edge values of the tensor.
     */
    struct pad_edge : pad_constant {
        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
        ) const;
    };

    /**
     * @brief Pads with the linear ramp between an end value and the tensor
     * edge value.
     *
     * @param args.. The values used for the ending values of the linear ramp.
     * - If no arguments are passed, all the end values are set to zero.
     * - If a single value is passed, the same value is used for all the axes.
     * - If two values are passed, the same before and after end values are
     *   used for each axis.
     * - If a matrix of values is passed, unique end values are used for each
     *   axis, where values(i, 0) and values(i, 1) are the before and after end
     *   values for axis i.
     */
    struct pad_linear_ramp {
        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
        ) const;

        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
            const typename tensor<T, 1>::value_type &end
        ) const;

        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t,
            const typename tensor<T, 1>::value_type &end_before,
            const typename tensor<T, 1>::value_type &end_after
        ) const;

        template <class T, class Tag>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
            const base_tensor<T, 2, Tag> &end_values
        ) const;
    };

    /**
     * @brief Pads with the reflection of the tensor mirrored on the first and
     * last values.
     */
    struct pad_reflect {
        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
        ) const;
    };

    /**
     * @brief Pads with the reflection of the tensor mirrored along the edge.
     */
    struct pad_symmetric {
        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
        ) const;
    };

    /**
     * @brief Pads with the wrap of the tensor along the axis. The first values
     * are used to pad the end and the end values are used to pad the
     * beginning.
     */
    struct pad_wrap {
        template <class T>
        void operator()(
            tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
        ) const;
    };

    /// Indexing routines.

    /**
     * @brief Converts a tuple of indices into a flat index, element-wise.
     *
     * @param index A tensor-like object with the indices to flatten.
     * @param shape The shape of the tensor used for raveling.
     * @param order Determines whether the indices should be viewed as indexing
     *     in row-major order (true) or column-major order (false). Defaults to
     *     row-major order.
     *
     * @return A light-weight object with the flattened indices. This function
     *     does not create a new tensor, instead, an expression object is
     *     returned.
     */
    template <size_t Rank, size_t N, class Tag>
    base_tensor<
        size_t, N,
        lazy_unary_tag<detail::ravel_index<Rank>, index_t<Rank>, Tag>
    > ravel_index(
        const base_tensor<index_t<Rank>, N, Tag> &index,
        const shape_t<Rank> &shape, bool order = true
    );

    /**
     * @brief Converts a flat index into a tuple of indices, element-wise.
     *
     * @param index A tensor-like object with the indices to unravel.
     * @param shape The shape of the tensor used for unraveling.
     * @param order Determines whether the indices should be viewed as indexing
     *     in row-major order (true) or column-major order (false). Defaults to
     *     row-major order.
     *
     * @return A light-weight object with the unraveled indices. This function
     *     does not create a new tensor, instead, an expression object is
     *     returned.
     */
    template <class IntegralType, size_t Rank, size_t N, class Tag,
              detail::RequiresIntegral<IntegralType> = true>
    base_tensor<
        index_t<Rank>, N,
        lazy_unary_tag<detail::unravel_index<Rank>, IntegralType, Tag>
    > unravel_index(
        const base_tensor<IntegralType, N, Tag> &index,
        const shape_t<Rank> &shape, bool order = true
    );

    /**
     * @brief Take elements from a tensor. For tensor class, a call such as
     * np::take(a, indices) is equivalent to a[indices], except that a copy is
     * always returned.
     *
     * @param a The source tensor.
     * @param indices A tensor-like object of index_t with the indices of the
     *     values to take. If the source tensor is one dimensional, a
     *     tensor-like object of integers can be used instead.
     *
     * @return A new tensor with the elements from a at the given positions.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag, size_t N, class TagIndex>
    tensor<T, N> take(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<index_t<Rank>, N, TagIndex> &indices
    );

    template <class T, class Tag, class IntegralType, size_t N, class TagIndex,
              detail::RequiresIntegral<IntegralType> = true>
    tensor<T, N> take(
        const base_tensor<T, 1, Tag> &a,
        const base_tensor<IntegralType, N, TagIndex> &indices
    );

    /**
     * @brief Take elements from a tensor along an axis.
     *
     * @param a The source tensor.
     * @param indices A tensor-like object with the indices of the values to
     *     take.
     * @param axis The axis over which to select values.
     *
     * @return A new tensor with the elements from a at the given positions. If
     *     a single index is given instead, the output tensor will have a
     *     dimension less than the source tensor. Otherwise, the output tensor
     *     and the source tensor will have the same dimension and the same
     *     shape, except for the axis over which the values are taken.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagIndex,
              detail::RequiresIntegral<IntegralType> = true>
    tensor<T, Rank> take(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, 1, TagIndex> &indices,
        size_t axis
    );

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank - 1> take(
        const base_tensor<T, Rank, Tag> &a, size_t index, size_t axis
    );

    /**
     * @brief Take values from the input tensor by matching 1d index and data
     * slices. Functions returning an index along an axis, like argsort and
     * argpartition, produce suitable indices for this function.
     *
     * @param a The source tensor.
     * @param indices A tensor-like object with the indices to take along each
     *     1d slice of a. This must match the shape of a for all dimensions
     *     other than axis.
     * @param axis The axis to take 1d slices along.
     *
     * @return A new tensor with the elements from a at the given positions.
     *
     * @throw std::invalid_argument Thrown if the shape of indices is not
     *     compatible with the shape of a.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagIndex,
              detail::RequiresIntegral<IntegralType> = true>
    tensor<T, Rank> take_along_axis(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, Rank, TagIndex> &indices,
        size_t axis
    );

    /**
     * @brief Replaces specified elements of a tensor with given values. For
     * tensor class, a call such as np::put(a, indices, values) is equivalent
     * to a[indices] = values.
     *
     * @param a Target tensor.
     * @param indices A tensor-like object of index_t with the target indices.
     *     If the source tensor is one dimensional, a tensor-like object of
     *     integers can be used instead.
     * @param values A single value or a tensor-like object with the values to
     *     put into a at target indices.
     *
     * @throw std::invalid_argument Thrown if indices and values have different
     *     shapes.
     */
    template <class T, size_t Rank, class Tag, class TagIndex, class TagValue>
    void put(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
        const base_tensor<T, 1, TagValue> &values
    );

    template <class T, size_t Rank, class Tag, class TagIndex>
    void put(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
        const typename tensor<T, 1>::value_type &value
    );

    template <class T, class Tag, class IntegralType,
              class TagIndex, class TagValue,
              detail::RequiresIntegral<IntegralType> = true>
    void put(
        base_tensor<T, 1, Tag> &a,
        const base_tensor<IntegralType, 1, TagIndex> &indices,
        const base_tensor<T, 1, TagValue> &values
    );

    template <class T, class Tag, class IntegralType, class TagIndex,
              detail::RequiresIntegral<IntegralType> = true>
    void put(
        base_tensor<T, 1, Tag> &a,
        const base_tensor<IntegralType, 1, TagIndex> &indices,
        const typename tensor<T, 1>::value_type &value
    );

    /**
     * @brief Put values into the destination tensor by matching 1d index and
     * data slices. Functions returning an index along an axis, like argsort
     * and argpartition, produce suitable indices for this function.
     *
     * @param a Destination tensor.
     * @param indices A tensor-like object with the indices to change along
     *     each 1d slice of a. This must match the shape of a for all
     *     dimensions other than axis.
     * @param values A tensor-like object with the values to insert at those
     *     indices. This must match the shape of indices.
     * @param axis The axis to take 1d slices along.
     *
     * @throw std::invalid_argument Thrown if the shapes of indices and values
     *     are not compatible with the shape of a.
     */
    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagIndex, class TagValue,
              detail::RequiresIntegral<IntegralType> = true>
    void put_along_axis(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, Rank, TagIndex> &indices,
        const base_tensor<T, Rank, TagValue> &values,
        size_t axis
    );

    /**
     * @brief Return the elements of a tensor that satisfy some condition. For
     * tensor class, a call such as np::extract(a, condition) is equivalent to
     * a[condition], except that a copy is always returned.
     *
     * @param a The input tensor.
     * @param condition A tensor-like object of bool whose true entries
     *     indicate the elements of a to extract.
     *
     * @return A new tensor with the elements from a where condition is true.
     *
     * @throw std::invalid_argument Thrown if a and condition have different
     *     shapes.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag, class TagCond>
    tensor<T, 1> extract(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition
    );

    /**
     * @brief Return the elements of a tensor that satisfy some condition along
     * given axis.
     *
     * @param a The input tensor.
     * @param condition A tensor-like object of bool whose true entries
     *     indicate the elements of a to extract.
     * @param axis The axis over which to select values.
     *
     * @return A new tensor with the elements from a where condition is true.
     *     The output tensor and the input tensor will have the same dimension
     *     and the same shape, except for the axis over which the values are
     *     extracted.
     *
     * @throw std::invalid_argument Thrown if the size of condition doesn't
     *     match the size of a along the given axis.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag, class TagCond>
    tensor<T, Rank> extract(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, 1, TagCond> &condition,
        size_t axis
    );

    /**
     * @brief Change elements of a tensor based on conditional and input
     * values.
     *
     * @param a Tensor to put data into.
     * @param condition Boolean mask tensor.
     * @param values A single value or a tensor-like object with the values to
     *     place in a. Only the first N elements are used, where N is the
     *     number of true values in condition.
     *
     * @throw std::invalid_argument Thrown if a and condition have different
     *     shapes.
     */
    template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
    void place(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, 1, TagValue> &values
    );

    template <class T, size_t Rank, class Tag, class TagCond>
    void place(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition,
        const typename tensor<T, 1>::value_type &value
    );

    /**
     * @brief Change elements of a tensor based on conditional and input
     * values. Similar to place, the difference is that place uses the first N
     * elements of values, where N is the number of true values in condition,
     * while putmask uses the elements where condition is true.
     *
     * @param a Tensor to put data into.
     * @param condition Boolean mask tensor.
     * @param values A tensor-like object with the values to put into a where
     *     condition is true.
     *
     * @throw std::invalid_argument Thrown if a, condition or values have
     *     different shapes.
     */
    template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
    void putmask(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagValue> &values
    );
}

#include "numcpp/broadcasting/broadcasting.tcc"

#endif // NUMCPP_BROADCASTING_H_INCLUDED
