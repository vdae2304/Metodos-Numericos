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

/** @file include/numcpp/functional.h
 *  This header defines routines for functional programming.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_FUNCTIONAL_H_INCLUDED
#define NUMCPP_FUNCTIONAL_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/functional/lazy_expression.h"
#include "numcpp/functional/operators.h"

namespace numcpp {
/// Functional programming.

/**
 * @brief Apply a function element-wise.
 *
 * @param f The function to apply.
 * @param a An abstract tensor with the values where the function will be
 *          invoked.
 *
 * @return A light-weight object which stores the result of invoking the
 *         function on each element. This function does not create a new tensor,
 *         instead, an expression object is returned. The returned object uses
 *         lazy-evaluation, which means that the function is called only when
 *         required, i.e., when the whole expression is evaluated or assigned to
 *         a tensor object.
 */
template <class Function, class Expr, class T, size_t Rank>
unary_expr<Function, Expr> apply(Function &&f,
                                 const abstract_tensor<Expr, T, Rank> &a) {
  return unary_expr<Function, Expr>(std::forward<Function>(f), a);
}

/**
 * @brief Apply a function element-wise.
 *
 * @param out A location into which the result is stored.
 * @param f The function to apply.
 * @param a An abstract tensor with the values where the function will be
 *          invoked.
 *
 * @throw std::invalid_argument Thrown if the shape of @a out does not match the
 *                              shape of @a a.
 */
template <class OutExpr, class R, class Function, class Expr, class T,
          size_t Rank>
void apply(dense_tensor<OutExpr, R, Rank> &out, Function &&f,
           const abstract_tensor<Expr, T, Rank> &a);

/**
 * @brief Apply a binary function element-wise.
 *
 * @param f The function to apply.
 * @param a An abstract tensor with the values to pass as first argument.
 * @param b An abstract tensor with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument. Values
 *            are broadcasted to an appropriate shape.
 *
 * @return A light-weight object which stores the result of invoking the
 *         function on each element. This function does not create a new tensor,
 *         instead, an expression object is returned. The returned object uses
 *         lazy-evaluation, which means that the function is called only when
 *         required, i.e., when the whole expression is evaluated or assigned to
 *         a tensor object.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <class Function, class Expr1, class T, class Expr2, class U,
          size_t Rank>
binary_expr<Function, Expr1, Expr2>
apply2(Function &&f, const abstract_tensor<Expr1, T, Rank> &a,
       const abstract_tensor<Expr2, U, Rank> &b) {
  return binary_expr<Function, Expr1, Expr2>(std::forward<Function>(f), a, b);
}

template <class Function, class Expr, class T, class U, size_t Rank,
          detail::RequiresScalar<U> = 0>
binary_expr<Function, Expr, detail::identity<U>>
apply2(Function &&f, const abstract_tensor<Expr, T, Rank> &a, const U &val) {
  return binary_expr<Function, Expr, detail::identity<U>>(
      std::forward<Function>(f), a, val);
}

template <class Function, class Expr, class T, class U, size_t Rank,
          detail::RequiresScalar<T> = 0>
binary_expr<Function, detail::identity<T>, Expr>
apply2(Function &&f, const T &val, const abstract_tensor<Expr, U, Rank> &b) {
  return binary_expr<Function, detail::identity<T>, Expr>(
      std::forward<Function>(f), val, b);
}

/**
 * @brief Apply a binary function element-wise.
 *
 * @param out A location into which the result is stored.
 * @param f The function to apply.
 * @param a An abstract tensor with the values to pass as first argument.
 * @param b An abstract tensor with the values to pass as second argument.
 * @param val Value to use either as first argument or second argument. Values
 *            are broadcasted to an appropriate shape.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules, or if the shape of @a out does not match
 *                              the broadcasting shape.
 */
template <class OutExpr, class R, class Function, class Expr1, class T,
          class Expr2, class U, size_t Rank>
void apply2(dense_tensor<OutExpr, R, Rank> &out, Function &&f,
            const abstract_tensor<Expr1, T, Rank> &a,
            const abstract_tensor<Expr2, U, Rank> &b);

template <class OutExpr, class R, class Function, class Expr, class T, class U,
          size_t Rank, detail::RequiresScalar<U> = 0>
void apply2(dense_tensor<OutExpr, R, Rank> &out, Function &&f,
            const abstract_tensor<Expr, T, Rank> &a, const U &val);

template <class OutExpr, class R, class Function, class Expr, class T, class U,
          size_t Rank, detail::RequiresScalar<T> = 0>
void apply2(dense_tensor<OutExpr, R, Rank> &out, Function &&f, const T &val,
            const abstract_tensor<Expr, U, Rank> &b);

#if __cplusplus >= 201402L
/**
 * @brief Apply a function element-wise.
 *
 * @param f The function to apply.
 * @param a, b... Abstract tensors with the values to pass as each argument in
 *                the function.
 *
 * @return A light-weight object which stores the result of invoking the
 *         function on each element. This function does not create a new tensor,
 *         instead, an expression object is returned. The returned object uses
 *         lazy-evaluation, which means that the function is called only when
 *         required, i.e., when the whole expression is evaluated or assigned to
 *         a tensor object.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <class Function, class Expr1, class T, class... Expr2, class... U,
          size_t Rank>
element_wise_expr<Function, Expr1, Expr2...>
applyn(Function &&f, const abstract_tensor<Expr1, T, Rank> &a,
       const abstract_tensor<Expr2, U, Rank> &...b) {
  return element_wise_expr<Function, Expr1, Expr2...>(std::forward<Function>(f),
                                                      a, b...);
}

/**
 * @brief Apply a function element-wise.
 *
 * @param out A location into which the result is stored.
 * @param f The function to apply.
 * @param a, b... Abstract tensors with the values to pass as each argument in
 *                the function.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules, or if the shape of @a out does not match
 *                              the broadcasting shape.
 */
template <class OutExpr, class R, class Function, class Expr1, class T,
          class... Expr2, class... U, size_t Rank>
void applyn(dense_tensor<OutExpr, R, Rank> &out, Function &&f,
            const abstract_tensor<Expr1, T, Rank> &a,
            const abstract_tensor<Expr2, U, Rank> &...b);
#endif // C++14

/**
 * @brief Reduce the tensor's dimension by cumulatively applying a function to
 * all elements.
 *
 * @param f The function to apply. A binary function taking the current
 *          accumulated value as first argument and an element in the tensor as
 *          second argument, and returning a value.
 * @param a An abstract tensor with the values where the reduction will be
 *          performed.
 * @param init Initial value. If not provided, the first element is used.
 * @param where A boolean tensor which indicates the elements to include in the
 *              reduction.
 *
 * @return The result of accumulating all the elements in the tensor.
 *
 * @throw std::invalid_argument Thrown if the tensor is empty and @a init is not
 *                              provided.
 */
template <class Function, class Expr, class T, size_t Rank>
T reduce(Function &&f, const abstract_tensor<Expr, T, Rank> &a);

template <class Function, class Expr, class T, size_t Rank>
T reduce(Function &&f, const abstract_tensor<Expr, T, Rank> &a,
         typename detail::identity<T>::type init);

template <class Function, class Expr1, class T, size_t Rank, class Expr2>
T reduce(Function &&f, const abstract_tensor<Expr1, T, Rank> &a,
         typename detail::identity<T>::type init,
         const abstract_tensor<Expr2, bool, Rank> &where);

/**
 * @brief Reduce the tensor's dimension by cumulatively applying a function over
 * multiple axes.
 *
 * @param f The function to apply. A binary function taking the current
 *          accumulated value as first argument and an element in the tensor as
 *          second argument, and returning a value.
 * @param a An abstract tensor with the values where the reduction will be
 *          performed.
 * @param axes Axes along which the reduction is performed.
 * @param keepdims If set to @a keepdims, the axes which are reduced are left as
 *                 dimensions with size one. If set to @a dropdims, the axes
 *                 which are reduced are dropped. Defaults to @a dropdims.
 * @param init Initial value. If not provided, the first element is used.
 * @param where A boolean tensor which indicates the elements to include in the
 *              reduction.
 *
 * @return A new tensor with the result of performing the reduction over the
 *         given axes.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Function, class Expr, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f,
                           const abstract_tensor<Expr, T, Rank> &a,
                           const shape_t<N> &axes);

template <class Function, class Expr, class T, size_t Rank, size_t N>
tensor<T, Rank - N>
reduce(Function &&f, const abstract_tensor<Expr, T, Rank> &a,
       const shape_t<N> &axes, typename detail::identity<T>::type init);

template <class Function, class Expr1, class T, size_t Rank, size_t N,
          class Expr2>
tensor<T, Rank - N>
reduce(Function &&f, const abstract_tensor<Expr1, T, Rank> &a,
       const shape_t<N> &axes, typename detail::identity<T>::type init,
       const abstract_tensor<Expr2, bool, Rank> &where);

template <class Function, class Expr, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const abstract_tensor<Expr, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t);

template <class Function, class Expr, class T, size_t Rank, size_t N>
tensor<T, Rank> reduce(Function &&f, const abstract_tensor<Expr, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t,
                       typename detail::identity<T>::type init);

template <class Function, class Expr1, class T, size_t Rank, size_t N,
          class Expr2>
tensor<T, Rank> reduce(Function &&f, const abstract_tensor<Expr1, T, Rank> &a,
                       const shape_t<N> &axes, keepdims_t,
                       typename detail::identity<T>::type init,
                       const abstract_tensor<Expr2, bool, Rank> &where);

template <class Function, class Expr, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f,
                           const abstract_tensor<Expr, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t);

template <class Function, class Expr, class T, size_t Rank, size_t N>
tensor<T, Rank - N> reduce(Function &&f,
                           const abstract_tensor<Expr, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t,
                           typename detail::identity<T>::type init);

template <class Function, class Expr1, class T, size_t Rank, size_t N,
          class Expr2>
tensor<T, Rank - N> reduce(Function &&f,
                           const abstract_tensor<Expr1, T, Rank> &a,
                           const shape_t<N> &axes, dropdims_t,
                           typename detail::identity<T>::type init,
                           const abstract_tensor<Expr2, bool, Rank> &where);

/**
 * @brief Accumulate the result of applying a function along an axis.
 *
 * @param f The function to apply. A binary function taking the current
 *          accumulated value as first argument and an element in the tensor as
 *          second argument, and returning a value.
 * @param a An abstract tensor with the values where the accumulation will be
 *          performed.
 * @param axis Axis along which to apply the accumulation. Default is zero.
 *
 * @return A new tensor with the accumulated values along the given axis.
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Function, class Expr, class T, size_t Rank>
tensor<T, Rank> accumulate(Function &&f,
                           const abstract_tensor<Expr, T, Rank> &a,
                           size_t axis = 0);

/**
 * @brief Apply a function to all pairs @a (ai,bj) with @a ai in @a a and
 * @a bj in @a b.
 *
 * @details Let @a a and @a b be tensors of dimension @a M and @a N,
 * respectively. The result of @c outer is a tensor of dimension @a M+N such
 * that
 * @f[
 *   out(i_0, ..., i_{M-1}, j_0, ..., j_{N-1})
 *     = f(a(i_0, ..., i_{M-1}), b(j_0, ..., j_{N-1}))
 * @f]
 *
 * @param f The function to apply.
 * @param a An abstract tensor with the values to pass as first argument.
 * @param b An abstract tensor with the values to pass as second argument.
 *
 * @return A light-weight object which stores the result of invoking the
 *         function to all pairs of elements. This function does not create a
 *         new tensor, instead, an expression object is returned. The returned
 *         object uses lazy-evaluation, which means that the function is called
 *         only when required, i.e., when the whole expression is evaluated or
 *         assigned to a tensor object.
 */
template <class Function, class Expr1, class T, size_t Rank1, class Expr2,
          class U, size_t Rank2>
outer_expr<Function, Expr1, Expr2>
outer(Function &&f, const abstract_tensor<Expr1, T, Rank1> &a,
      const abstract_tensor<Expr2, U, Rank2> &b) {
  return outer_expr<Function, Expr1, Expr2>(std::forward<Function>(f), a, b);
}

/**
 * @brief Apply a function to all pairs @a (ai,bj) with @a ai in @a a and
 * @a bj in @a b.
 *
 * @param out A location into which the result is stored.
 * @param f The function to apply.
 * @param a An abstract tensor with the values to pass as first argument.
 * @param b An abstract tensor with the values to pass as second argument.
 *
 * @throw std::invalid_argument Thrown if the shape of @a out does not match the
 *                              concatenated shape of @a a and @a b.
 */
template <class OutExpr, class R, class Function, class Expr1, class T,
          size_t Rank1, class Expr2, class U, size_t Rank2>
void outer(dense_tensor<OutExpr, R, Rank1 + Rank2> &out, Function &&f,
           const abstract_tensor<Expr1, T, Rank1> &a,
           const abstract_tensor<Expr2, U, Rank2> &b);
} // namespace numcpp

#include "numcpp/functional/functional.tcc"

#endif // NUMCPP_FUNCTIONAL_H_INCLUDED
