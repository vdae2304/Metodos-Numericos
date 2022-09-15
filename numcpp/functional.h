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

 /** @file include/numcpp/functional.h
  *  This header defines routines for functional programming.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_FUNCTIONAL_H_INCLUDED
#define NUMCPP_FUNCTIONAL_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/functional/operators.h"
#include "numcpp/functional/lazy_tensor.h"
#include "numcpp/functional/lazy_outer.h"

namespace numcpp {
    /// Functional programming.

    /**
     * @brief Apply a function element-wise.
     *
     * @param f The function to apply.
     * @param arg A tensor-like object with the values where the function will
     *     be invoked.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function on each element. This function does not create a new
     *     tensor object, instead, an expression object is returned. The
     *     returned object uses lazy-evaluation, which means that the function
     *     is called only when required, i.e., when the whole expression is
     *     evaluated or assigned to a tensor object.
     */
    template <class Function, class T, size_t Rank, class Tag>
    base_tensor<
        detail::result_of_t<Function, T>, Rank,
        lazy_unary_tag<Function, T, Tag>
    > apply(Function &&f, const base_tensor<T, Rank, Tag> &arg);

    /**
     * @brief Apply a function element-wise.
     *
     * @param out A location into which the result is stored.
     * @param f The function to apply.
     * @param arg A tensor-like object with the values where the function will
     *     be invoked.
     *
     * @throw std::invalid_argument Thrown if the shape of out does not match
     *     the shape of arg and cannot be resized.
     */
    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg
    );

    /**
     * @brief Apply a function element-wise.
     *
     * @param f The function to apply.
     * @param arg1 First tensor-like object.
     * @param arg2 Second tensor-like object.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function on each element. This function does not create a new
     *     tensor object, instead, an expression object is returned. The
     *     returned object uses lazy-evaluation, which means that the function
     *     is called only when required, i.e., when the whole expression is
     *     evaluated or assigned to a tensor object.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules.
     */
    template <class Function, size_t Rank,
              class T, class TagT, class U, class TagU>
    base_tensor<
        detail::result_of_t<Function, T, U>, Rank,
        lazy_binary_tag<Function, T, TagT, U, TagU>
    > apply(
        Function &&f,
        const base_tensor<T, Rank, TagT> &arg1,
        const base_tensor<U, Rank, TagU> &arg2
    );

    template <class Function, size_t Rank, class T, class Tag, class U>
    base_tensor<
        detail::result_of_t<Function, T, U>, Rank,
        lazy_binary_tag<Function, T, Tag, U, scalar_tag>
    > apply(Function &&f, const base_tensor<T, Rank, Tag> &arg1, const U &val);

    template <class Function, size_t Rank, class T, class U, class Tag>
    base_tensor<
        detail::result_of_t<Function, T, U>, Rank,
        lazy_binary_tag<Function, T, scalar_tag, U, Tag>
    > apply(Function &&f, const T &val, const base_tensor<U, Rank, Tag> &arg2);

    /**
     * @brief Apply a function element-wise.
     *
     * @param out A location into which the result is stored.
     * @param f The function to apply.
     * @param arg1 First tensor-like object.
     * @param arg2 Second tensor-like object.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules, or if the
     *     shape of out does not match the broadcasting shape.
     */
    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class TagT, class U, class TagU>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f,
        const base_tensor<T, Rank, TagT> &arg1,
        const base_tensor<U, Rank, TagU> &arg2
    );

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag, class U>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg1, const U &val
    );

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class U, class Tag>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const T &val, const base_tensor<U, Rank, Tag> &arg2
    );

    /**
     * @brief Apply a function over multiple axes.
     *
     * @param f The function to apply. A function that accepts two iterators as
     *     arguments and returns a value. The iterators indicates the initial
     *     and final positions of the flattened dimensions. The range used is
     *     [first, last), which includes all the elements between first and
     *     last, including the element pointed by first but not the element
     *     pointed by last.
     * @param arg A tensor-like object with the values where the function will
     *     be invoked.
     * @param axes Axis or axes over which the function is applied.
     *
     * @return A new tensor with the result of invoking the function over the
     *     given axes. The output tensor has the same dimension as arg, but the
     *     axes over which the function is applied are left as dimensions of
     *     size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Function, class T, size_t Rank, class Tag>
    tensor<
        detail::result_of_t<
            Function,
            base_tensor_const_reduce_iterator<T, Rank, Tag, 1>,
            base_tensor_const_reduce_iterator<T, Rank, Tag, 1>
        >, Rank
    > apply_along_axis(
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    );

    template <class Function, class T, size_t Rank, class Tag, size_t N>
    tensor<
        detail::result_of_t<
            Function,
            base_tensor_const_reduce_iterator<T, Rank, Tag, N>,
            base_tensor_const_reduce_iterator<T, Rank, Tag, N>
        >, Rank
    > apply_over_axes(
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    );

    /**
     * @brief Apply a function over multiple axes.
     *
     * @param out A location into which the result is stored.
     * @param f The function to apply. A function that accepts two iterators as
     *     arguments and returns a value. The iterators indicates the initial
     *     and final positions of the flattened dimensions. The range used is
     *     [first, last), which includes all the elements between first and
     *     last, including the element pointed by first but not the element
     *     pointed by last.
     * @param arg A tensor-like object with the values where the function will
     *     be invoked.
     * @param axes Axis or axes over which the function is applied.
     *
     * @throw std::invalid_argument Thrown if the shape of out does not match
     *     the reduction shape and cannot be resized.
     */
    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void apply_along_axis(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    );

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag, size_t N>
    void apply_over_axes(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    );

    /**
     * @brief Reduce the tensor's dimension by applying a function to all
     * elements.
     *
     * @param f The function to apply. A binary function taking the current
     *     accumulated value as first argument and an element in the tensor as
     *     second argument, and returning a value.
     * @param arg A tensor-like object.
     *
     * @return The result of accumulating all the elements in the tensor.
     *
     * @note The initial value for the reduction is f.identity. If the function
     *     does not have identity, the first element in the tensor is used,
     *     and a std::invalid_argument exception is thrown if the tensor is
     *     empty.
     */
    template <class Function, class T, size_t Rank, class Tag>
    typename base_tensor<T, Rank, Tag>::value_type
    reduce(Function &&f, const base_tensor<T, Rank, Tag> &arg);

    /**
     * @brief Reduce the tensor's dimension by applying a function over
     * multiple axis.
     *
     * @param f The function to apply. A binary function taking the current
     *     accumulated value as first argument and an element in the tensor as
     *     second argument, and returning a value.
     * @param arg A tensor-like object.
     * @param axes Axis or axes along which the reduction is performed.
     *
     * @return A new tensor with the result of performing the reduction over
     *     the given axes. The output tensor has the same dimension as arg, but
     *     the axes which are reduced are left as dimensions of size one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Function, class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    reduce(Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis);

    template <class Function, class T, size_t Rank, class Tag, size_t N>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    reduce(
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    );

    /**
     * @brief Reduce the tensor's dimension by applying a function over
     * multiple axis.
     *
     * @param out A location into which the result is stored.
     * @param f The function to apply. A binary function taking the current
     *     accumulated value as first argument and an element in the tensor as
     *     second argument, and returning a value.
     * @param arg A tensor-like object.
     * @param axes Axis or axes along which the reduction is performed.
     *
     * @throw std::invalid_argument Thrown if the shape of out does not match
     *     the reduction shape and cannot be resized.
     */
    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void reduce(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    );

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag, size_t N>
    void reduce(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    );

    /**
     * @brief Accumulate the result of applying a function to all elements.
     *
     * @param f The function to apply. A binary function taking the current
     *     accumulated value as first argument and an element in the tensor as
     *     second argument, and returning a value.
     * @param arg A tensor-like object.
     * @param axis Axis along which to apply the accumulation. Default is zero.
     *
     * @return A new tensor with the accumulated values along the given axis.
     *     The output tensor has the same dimension and the same shape as arg.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Function, class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    accumulate(
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis = 0
    );

    /**
     * @brief Accumulate the result of applying a function to all elements.
     *
     * @param out A location into which the result is stored.
     * @param f The function to apply. A binary function taking the current
     *     accumulated value as first argument and an element in the tensor as
     *     second argument, and returning a value.
     * @param arg A tensor-like object.
     * @param axis Axis along which to apply the accumulation. Default is zero.
     *
     * @throw std::invalid_argument Thrown if the shape of out does not match
     *     the shape of arg and cannot be resized.
     */
    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void accumulate(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis = 0
    );

    /**
     * @brief Apply a function to all pairs (a, b) with a in arg1 and b in
     * arg2.
     *
     * @note Let M = arg1.ndim() and N = arg2.ndim(). The result of
     * outer(f, arg1, arg2) is a tensor of dimension M + N such that
     * @f[
     *     out(i_0, ..., i_{M-1}, j_0, ..., j_{N-1})
     *         = f(arg1(i_0, ..., i_{M-1}), arg2(j_0, ..., j_{N-1}))
     * @f]
     *
     * @param f The function to apply.
     * @param arg1 First tensor-like object.
     * @param arg2 Second tensor-like object.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function to all pairs of elements. This function does not create a
     *     new tensor object, instead, an expression object is returned. The
     *     returned object uses lazy-evaluation, which means that the function
     *     is called only when required, i.e., when the whole expression is
     *     evaluated or assigned to a tensor object.
     */
    template <class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    base_tensor<
        detail::result_of_t<Function, T, U>, M + N,
        lazy_outer_tag<Function, T, M, TagT, U, N, TagU>
    > outer(
        Function &&f,
        const base_tensor<T, M, TagT> &arg1,
        const base_tensor<U, N, TagU> &arg2
    );

    /**
     * @brief Apply a function to all pairs (a, b) with a in arg1 and b in
     * arg2.
     *
     * @param out A location into which the result is stored.
     * @param f The function to apply.
     * @param arg1 First tensor-like object.
     * @param arg2 Second tensor-like object.
     *
     * @throw std::invalid_argument Thrown if the shape of out does not match
     *     the concatenated shape of arg1 and arg2 and cannot be resized.
     */
    template <class R, class TagR,
              class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    void outer(
        base_tensor<R, M + N, TagR> &out,
        Function &&f,
        const base_tensor<T, M, TagT> &arg1,
        const base_tensor<U, N, TagU> &arg2
    );
}

#include "numcpp/functional/functional.tcc"

#endif // NUMCPP_FUNCTIONAL_H_INCLUDED
