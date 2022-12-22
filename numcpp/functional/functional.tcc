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

 /** @file include/numcpp/functional/functional.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/functional.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_FUNCTIONAL_TCC_INCLUDED
#define NUMCPP_FUNCTIONAL_TCC_INCLUDED

namespace numcpp {
    /// Functional programming.

namespace detail {
    /**
     * @brief Resizes a tensor if it is not a subclass. Otherwise, throws an
     * std::invalid_argument exception.
     */
    template <class T, size_t Rank>
    inline void resize(tensor<T, Rank> &arg, const shape_t<Rank> &shape) {
        arg.resize(shape);
    }

    template <class T, size_t Rank, class Tag>
    void resize(base_tensor<T, Rank, Tag> &arg, const shape_t<Rank> &shape) {
        if (arg.shape() != shape) {
            std::ostringstream error;
            error << "non-broadcastable output operand with shape "
                  << arg.shape() << " doesn't match the broadcast shape "
                  << shape;
            throw std::invalid_argument(error.str());
        }
    }
}

    template <class Function, class T, size_t Rank, class Tag>
    inline base_tensor<
        detail::result_of_t<Function, T>, Rank,
        lazy_unary_tag<Function, T, Tag>
    > apply(Function &&f, const base_tensor<T, Rank, Tag> &arg) {
        typedef detail::result_of_t<Function, T> Rt;
        typedef lazy_unary_tag<Function, T, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(std::forward<Function>(f), arg);
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg
    ) {
        detail::resize(out, arg.shape());
        for (index_t<Rank> i : make_index_sequence(out.shape())) {
            out[i] = std::forward<Function>(f)(arg[i]);
        }
    }

    template <class Function, size_t Rank,
              class T, class TagT, class U, class TagU>
    inline base_tensor<
        detail::result_of_t<Function, T, U>, Rank,
        lazy_binary_tag<Function, T, TagT, U, TagU>
    > apply(
        Function &&f,
        const base_tensor<T, Rank, TagT> &arg1,
        const base_tensor<U, Rank, TagU> &arg2
    ) {
        typedef detail::result_of_t<Function, T, U> Rt;
        typedef lazy_binary_tag<Function, T, TagT, U, TagU> Closure;
        return base_tensor<Rt, Rank, Closure>(
            std::forward<Function>(f), arg1, arg2
        );
    }

    template <class Function, size_t Rank, class T, class Tag, class U>
    inline base_tensor<
        detail::result_of_t<Function, T, U>, Rank,
        lazy_binary_tag<Function, T, Tag, U, scalar_tag>
    > apply(Function &&f, const base_tensor<T, Rank, Tag> &arg1, const U &val) {
        typedef detail::result_of_t<Function, T, U> Rt;
        typedef lazy_binary_tag<Function, T, Tag, U, scalar_tag> Closure;
        return base_tensor<Rt, Rank, Closure>(
            std::forward<Function>(f), arg1, val
        );
    }

    template <class Function, size_t Rank, class T, class U, class Tag>
    inline base_tensor<
        detail::result_of_t<Function, T, U>, Rank,
        lazy_binary_tag<Function, T, scalar_tag, U, Tag>
    > apply(Function &&f, const T &val, const base_tensor<U, Rank, Tag> &arg2) {
        typedef detail::result_of_t<Function, T, U> Rt;
        typedef lazy_binary_tag<Function, T, scalar_tag, U, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(
            std::forward<Function>(f), val, arg2
        );
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class TagT, class U, class TagU>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f,
        const base_tensor<T, Rank, TagT> &arg1,
        const base_tensor<U, Rank, TagU> &arg2
    ) {
        detail::resize(out, broadcast_shapes(arg1.shape(), arg2.shape()));
        for (index_t<Rank> i : make_index_sequence(out.shape())) {
            out[i] = std::forward<Function>(f)(
                arg1[detail::broadcast_index(i, arg1.shape())],
                arg2[detail::broadcast_index(i, arg2.shape())]
            );
        }
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag, class U>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg1, const U &val
    ) {
        detail::resize(out, arg1.shape());
        for (index_t<Rank> i : make_index_sequence(out.shape())) {
            out[i] = std::forward<Function>(f)(arg1[i], val);
        }
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class U, class Tag>
    void apply(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const T &val, const base_tensor<U, Rank, Tag> &arg2
    ) {
        detail::resize(out, arg2.shape());
        for (index_t<Rank> i : make_index_sequence(out.shape())) {
            out[i] = std::forward<Function>(f)(val, arg2[i]);
        }
    }

    template <class Function, class T, size_t Rank, class Tag>
    inline tensor<
        detail::result_of_t<
            Function,
            base_tensor_const_axes_iterator<T, Rank, Tag, 1>,
            base_tensor_const_axes_iterator<T, Rank, Tag, 1>
        >, Rank
    > apply_along_axis(
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    ) {
        typedef detail::result_of_t<
            Function,
            base_tensor_const_axes_iterator<T, Rank, Tag, 1>,
            base_tensor_const_axes_iterator<T, Rank, Tag, 1>
        > Rt;
        tensor<Rt, Rank> out;
        apply_along_axis(out, std::forward<Function>(f), arg, axis);
        return out;
    }

    template <class Function, class T, size_t Rank, class Tag, size_t N>
    inline tensor<
        detail::result_of_t<
            Function,
            base_tensor_const_axes_iterator<T, Rank, Tag, N>,
            base_tensor_const_axes_iterator<T, Rank, Tag, N>
        >, Rank
    > apply_over_axes(
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    ) {
        typedef detail::result_of_t<
            Function,
            base_tensor_const_axes_iterator<T, Rank, Tag, N>,
            base_tensor_const_axes_iterator<T, Rank, Tag, N>
        > Rt;
        tensor<Rt, Rank> out;
        apply_over_axes(out, std::forward<Function>(f), arg, axes);
        return out;
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void apply_along_axis(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    ) {
        shape_t<Rank> shape = arg.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        detail::resize(out, shape);
        for (index_t<Rank> out_index : make_index_sequence(shape)) {
            out[out_index] = std::forward<Function>(f)(
                make_const_axes_iterator(&arg, out_index, axis, 0),
                make_const_axes_iterator(&arg, out_index, axis, size)
            );
        }
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag, size_t N>
    void apply_over_axes(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    ) {
        static_assert(N <= Rank, "Reduction dimension must be less or equal to"
                      " tensor dimension");
        shape_t<Rank> shape = arg.shape();
        size_t size = 1;
        for (size_t i = 0; i < axes.ndim(); ++i) {
            size *= shape[axes[i]];
            shape[axes[i]] = 1;
        }
        detail::resize(out, shape);
        for (index_t<Rank> out_index : make_index_sequence(shape)) {
            out[out_index] = std::forward<Function>(f)(
                make_const_axes_iterator(&arg, out_index, axes, 0),
                make_const_axes_iterator(&arg, out_index, axes, size)
            );
        }
    }

namespace detail {
    /**
     * @brief Checks whether a function has identity.
     */
    template <class F, typename = void>
    struct has_identity : std::false_type {};

    template <class F>
    struct has_identity<F, decltype(void(F::identity))> : std::true_type {};

    /**
     * @brief Returns the result of accumulating all the values in the range
     * [first, last).
     */
    template <class InputIterator, class Function>
    typename std::iterator_traits<InputIterator>::value_type reduce_impl(
        InputIterator first, InputIterator last, Function f, std::true_type
    ) {
        typedef typename std::iterator_traits<InputIterator>::value_type T;
        return std::accumulate(first, last, T(f.identity), f);
    }

    template <class InputIterator, class Function>
    typename std::iterator_traits<InputIterator>::value_type reduce_impl(
        InputIterator first, InputIterator last, Function f, std::false_type
    ) {
        typedef typename std::iterator_traits<InputIterator>::value_type T;
        if (first == last) {
            char error[] = "attempt to call reduce on an empty sequence with"
                " no identity";
            throw std::invalid_argument(error);
        }
        T init = *first;
        ++first;
        return std::accumulate(first, last, init, f);
    }
}

    template <class Function, class T, size_t Rank, class Tag>
    inline typename base_tensor<T, Rank, Tag>::value_type
    reduce(Function &&f, const base_tensor<T, Rank, Tag> &arg) {
        return detail::reduce_impl(
            arg.begin(), arg.end(),
            std::forward<Function>(f), detail::has_identity<Function>()
        );
    }

    template <class Function, class T, size_t Rank, class Tag>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    reduce(Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        reduce(out, std::forward<Function>(f), arg, axis);
        return out;
    }

    template <class Function, class T, size_t Rank, class Tag, size_t N>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    reduce(
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        reduce(out, std::forward<Function>(f), arg, axes);
        return out;
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void reduce(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    ) {
        shape_t<Rank> shape = arg.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        detail::resize(out, shape);
        for (index_t<Rank> out_index : make_index_sequence(shape)) {
            out[out_index] = detail::reduce_impl(
                make_const_axes_iterator(&arg, out_index, axis, 0),
                make_const_axes_iterator(&arg, out_index, axis, size),
                std::forward<Function>(f), detail::has_identity<Function>()
            );
        }
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag, size_t N>
    void reduce(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg,
        const shape_t<N> &axes
    ) {
        static_assert(N <= Rank, "Reduction dimension must be less or equal to"
                      " tensor dimension");
        shape_t<Rank> shape = arg.shape();
        size_t size = 1;
        for (size_t i = 0; i < axes.ndim(); ++i) {
            size *= shape[axes[i]];
            shape[axes[i]] = 1;
        }
        detail::resize(out, shape);
        for (index_t<Rank> out_index : make_index_sequence(shape)) {
            out[out_index] = detail::reduce_impl(
                make_const_axes_iterator(&arg, out_index, axes, 0),
                make_const_axes_iterator(&arg, out_index, axes, size),
                std::forward<Function>(f), detail::has_identity<Function>()
            );
        }
    }

    template <class Function, class T, size_t Rank, class Tag>
    inline tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    accumulate(
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    ) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        tensor<Rt, Rank> out;
        accumulate(out, std::forward<Function>(f), arg, axis);
        return out;
    }

    template <class R, size_t Rank, class TagR,
              class Function,
              class T, class Tag>
    void accumulate(
        base_tensor<R, Rank, TagR> &out,
        Function &&f, const base_tensor<T, Rank, Tag> &arg, size_t axis
    ) {
        shape_t<Rank> shape = arg.shape();
        detail::resize(out, shape);
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> out_index : make_index_sequence(shape)) {
            std::partial_sum(
                make_const_axes_iterator(&arg, out_index, axis, 0),
                make_const_axes_iterator(&arg, out_index, axis, size),
                make_axes_iterator(&out, out_index, axis, 0),
                std::forward<Function>(f)
            );
        }
    }

    template <class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    inline base_tensor<
        detail::result_of_t<Function, T, U>, M + N,
        lazy_outer_tag<Function, T, M, TagT, U, N, TagU>
    > outer(
        Function &&f,
        const base_tensor<T, M, TagT> &arg1,
        const base_tensor<U, N, TagU> &arg2
    ) {
        typedef detail::result_of_t<Function, T, U> Rt;
        typedef lazy_outer_tag<Function, T, M, TagT, U, N, TagU> Closure;
        return base_tensor<Rt, M + N, Closure>(
            std::forward<Function>(f), arg1, arg2
        );
    }

    template <class R, class TagR,
              class Function,
              class T, size_t M, class TagT,
              class U, size_t N, class TagU>
    void outer(
        base_tensor<R, M + N, TagR> &out,
        Function &&f,
        const base_tensor<T, M, TagT> &arg1,
        const base_tensor<U, N, TagU> &arg2
    ) {
        detail::resize(out, shape_cat(arg1.shape(), arg2.shape()));
        for (index_t<M> i : make_index_sequence(arg1.shape())) {
            for (index_t<N> j : make_index_sequence(arg2.shape())) {
                out[shape_cat(i, j)] =
                    std::forward<Function>(f)(arg1[i], arg2[j]);
            }
        }
    }
}

#endif // NUMCPP_FUNCTIONAL_TCC_INCLUDED
