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
    /// Broadcasting.

    template <class T, size_t Rank>
    tensor_view<T, Rank> broadcast_to(T &val, const shape_t<Rank> &shape) {
        return tensor_view<T, Rank>(shape, &val, 0, shape_t<Rank>());
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank> broadcast_to(
        tensor<T, Rank> &arg, const shape_t<Rank> &shape
    ) {
        shape_t<Rank> strides;
        size_t size = 1;
        for (size_t i = 0; i < shape.ndim(); ++i) {
            size_t j = shape.ndim() - 1 - i;
            if (arg.shape(j) == shape[j]) {
                strides[j] = size;
            }
            else if (arg.shape(j) != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shape "
                      << arg.shape() << " and requested shape " << shape;
                throw std::invalid_argument(error.str());
            }
            size *= arg.shape(j);
        }
        return tensor_view<T, Rank>(shape, arg.data(), 0, strides);
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank> broadcast_to(
        tensor_view<T, Rank> arg, const shape_t<Rank> &shape
    ) {
        shape_t<Rank> strides;
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (arg.shape(i) == shape[i]) {
                strides[i] = arg.strides(i);
            }
            else if (arg.shape(i) != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shape "
                      << arg.shape() << " and requested shape " << shape;
                throw std::invalid_argument(error.str());
            }
        }
        return tensor_view<T, Rank>(shape, arg.data(), arg.offset(), strides);
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank + 1> expand_dims(tensor<T, Rank> &arg, size_t axis) {
        return expand_dims(arg, make_shape(axis));
    }

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank + N> expand_dims(
        tensor<T, Rank> &arg, const shape_t<N> &axes
    ) {
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
        return tensor_view<T, Rank + N>(shape, arg.data());
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank + 1> expand_dims(
        tensor_view<T, Rank> arg, size_t axis
    ) {
        return expand_dims(arg, make_shape(axis));
    }

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank + N> expand_dims(
        tensor_view<T, Rank> arg, const shape_t<N> &axes
    ) {
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
        return tensor_view<T, Rank+N>(shape, arg.data(), arg.offset(), strides);
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank - 1> squeeze(tensor<T, Rank> &arg, size_t axis) {
        return squeeze(arg, make_shape(axis));
    }

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank - N> squeeze(
        tensor<T, Rank> &arg, const shape_t<N> &axes
    ) {
        static_assert(N < Rank, "Reduction dimension must be less than tensor"
                      " dimension");
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
            }
            else if (arg.shape(i) != 1) {
                char error[] = "cannot select an axis to squeeze out which has"
                    " size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<T, Rank - N>(shape, arg.data());
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank - 1> squeeze(tensor_view<T, Rank> arg, size_t axis) {
        return squeeze(arg, make_shape(axis));
    }

    template <size_t N, class T, size_t Rank>
    tensor_view<T, Rank - N> squeeze(
        tensor_view<T, Rank> arg, const shape_t<N> &axes
    ) {
        static_assert(N < Rank, "Reduction dimension must be less than tensor"
                      " dimension");
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
            }
            else if (arg.shape(i) != 1) {
                char error[] = "cannot select an axis to squeeze out which has"
                    " size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<T, Rank-N>(shape, arg.data(), arg.offset(), strides);
    }

    /// Basic manipulation routines.

    template <class U, class T, size_t Rank, class Tag>
    base_tensor<U, Rank, lazy_unary_tag<cast_to<U>, T, Tag> >
    astype(const base_tensor<T, Rank, Tag> &a) {
        typedef lazy_unary_tag<cast_to<U>, T, Tag> Closure;
        return base_tensor<U, Rank, Closure>(cast_to<U>(), a);
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, Rank>
    copy(const base_tensor<T, Rank, Tag> &a) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        return tensor<Rt, Rank>(a);
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    flatten(const base_tensor<T, Rank, Tag> &a) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        return tensor<Rt, 1>(a.size(), a.begin());
    }

    template <class T, size_t Rank, class Tag>
    tensor<typename base_tensor<T, Rank, Tag>::value_type, 1>
    flatten(const base_tensor<T, Rank, Tag> &a, bool row_major) {
        typedef typename base_tensor<T, Rank, Tag>::value_type Rt;
        return tensor<Rt, 1>(a.size(), a.begin(row_major));
    }

    template <class T, class U, size_t Rank, class TagT, class TagU>
    base_tensor<
        std::pair<T, U>, Rank, lazy_binary_tag<detail::zip, T, TagT, U, TagU>
    > zip(
        const base_tensor<T, Rank, TagT> &a,
        const base_tensor<U, Rank, TagU> &b
    ) {
        typedef std::pair<T, U> Rt;
        typedef lazy_binary_tag<detail::zip, T, TagT, U, TagU> Closure;
        return base_tensor<Rt, Rank, Closure>(detail::zip(), a, b);
    }

    template <size_t I, class T, class U, size_t Rank, class Tag>
    base_tensor<typename std::enable_if<(I == 0), T>::type, Rank,
                lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag> >
    unzip(const base_tensor<std::pair<T, U>, Rank, Tag> &a) {
        typedef lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag> Closure;
        return base_tensor<T, Rank, Closure>(detail::unzip<I>(), a);
    }

    template <size_t I, class T, class U, size_t Rank, class Tag>
    base_tensor<typename std::enable_if<(I == 1), U>::type, Rank,
                lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag> >
    unzip(const base_tensor<std::pair<T, U>, Rank, Tag> &a) {
        typedef lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag> Closure;
        return base_tensor<U, Rank, Closure>(detail::unzip<I>(), a);
    }

    /// Concatenation.

namespace detail {
    /**
     * @brief Computes the shape of the concatenation along the given axis.
     * Throws a std::invalid_argument exception if the shapes do not match
     * along the remaining axes.
     */
    template <size_t Rank>
    void concatenation_shape(shape_t<Rank>&, size_t) {}

    template <class T, size_t Rank, class Tag, class... Args>
    void concatenation_shape(
        shape_t<Rank> &shape, size_t axis,
        const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (arg1.shape(i) != shape[i] && i != axis) {
                std::ostringstream error;
                error << "all the tensor dimensions for the concatenation axis"
                      << " must match exactly, but along dimension " << i
                      << ", tensors have sizes " << shape[i] << " and "
                      << arg1.shape(i);
                throw std::invalid_argument(error.str());
            }
        }
        shape[axis] += arg1.shape(axis);
        concatenation_shape(shape, axis, arg2...);
    }

    /**
     * @brief Concatenates tensors along the given axis.
     */
    template <class T, size_t Rank>
    void concatenate_tensors(tensor<T, Rank>&, size_t, size_t) {}

    template <class T, size_t Rank, class Tag, class... Args>
    void concatenate_tensors(
        tensor<T, Rank> &out, size_t axis, size_t offset,
        const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        for (index_t<Rank> i : make_indices(arg1.shape())) {
            index_t<Rank> out_index = i;
            out_index[axis] += offset;
            out[out_index] = arg1[i];
        }
        concatenate_tensors(out, axis, offset + arg1.shape(axis), arg2...);
    }

    /**
     * @brief Stack tensors along the given axis.
     */
    template <class T, size_t Rank>
    void stack_tensors(tensor<T, Rank>&, size_t, size_t) {}

    template <class T, size_t Rank, class Tag, class... Args>
    void stack_tensors(
        tensor<T, Rank + 1> &out, size_t axis, size_t offset,
        const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        shape_t<Rank> shape = detail::remove_axis(out.shape(), axis);
        if (shape != arg1.shape()) {
            char error[] = "all the tensors must have the same shape";
            throw std::invalid_argument(error);
        }
        for (index_t<Rank> i : make_indices(arg1.shape())) {
            index_t<Rank + 1> out_index = detail::insert_axis(i, axis, offset);
            out[out_index] = arg1[i];
        }
        stack_tensors(out, axis, offset + 1, arg2...);
    }
}

    template <class T, size_t Rank, class Tag, class... Args>
    inline tensor<T, Rank> concatenate(
        const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        return concatenate(0, arg1, arg2...);
    }

    template <class T, size_t Rank, class Tag, class... Args>
    tensor<T, Rank> concatenate(
        size_t axis, const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        shape_t<Rank> shape = arg1.shape();
        detail::concatenation_shape(shape, axis, arg2...);
        tensor<T, Rank> out(shape);
        detail::concatenate_tensors(out, axis, 0, arg1, arg2...);
        return out;
    }

    template <class T, size_t Rank, class Tag, class... Args>
    inline tensor<T, Rank + 1> stack(
        const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        return stack(0, arg1, arg2...);
    }

    template <class T, size_t Rank, class Tag, class... Args>
    tensor<T, Rank + 1> stack(
        size_t axis, const base_tensor<T, Rank, Tag> &arg1, const Args&... arg2
    ) {
        shape_t<Rank + 1> shape = detail::insert_axis(arg1.shape(), axis);
        shape[axis] = 1 + sizeof...(arg2);
        tensor<T, Rank + 1> out(shape);
        detail::stack_tensors(out, axis, 0, arg1, arg2...);
        return out;
    }

    /// Tiling.

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> tile(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &reps
    ) {
        shape_t<Rank> shape = a.shape();
        for (size_t i = 0; i < shape.ndim(); ++i) {
            shape[i] *= reps[i];
        }
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index = out_index;
            for (size_t i = 0; i < a_index.ndim(); ++i) {
                a_index[i] %= a.shape(i);
            }
            out[out_index] = a[a_index];
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> repeat(
        const base_tensor<T, Rank, Tag> &a, size_t reps, size_t axis
    ) {
        shape_t<Rank> shape = a.shape();
        shape[axis] *= reps;
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index = out_index;
            a_index[axis] /= reps;
            out[out_index] = a[a_index];
        }
        return out;
    }

    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagReps,
              detail::RequiresIntegral<IntegralType> >
    tensor<T, Rank> repeat(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, 1, TagReps> &reps,
        size_t axis
    ) {
        if (a.shape(axis) != reps.size()) {
            std::ostringstream error;
            error << "size of reps " << reps.size() << " does not match tensor"
                  << " size " << a.shape(axis) << " along dimension " << axis;
            throw std::invalid_argument(error.str());
        }
        shape_t<Rank> shape = a.shape();
        shape[axis] = std::accumulate(reps.begin(), reps.end(), 0);
        tensor<T, Rank> out(shape);
        shape[axis] = 1;
        for (index_t<Rank> out_index : make_indices(shape)) {
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
    tensor<T, Rank> pad(
        const base_tensor<T, Rank, Tag> &arg,
        const shape_t<Rank> &before, const shape_t<Rank> &after
    ) {
        shape_t<Rank> shape = arg.shape();
        for (size_t i = 0; i < shape.ndim(); ++i) {
            shape[i] += before[i] + after[i];
        }
        tensor<T, Rank> out(shape);
        for (index_t<Rank> index : make_indices(arg.shape())) {
            index_t<Rank> out_index = index;
            for (size_t i = 0; i < index.ndim(); ++i) {
                out_index[i] += before[i];
            }
            out[out_index] = arg[index];
        }
        return out;
    }

    template <class T, size_t Rank, class Tag, class Function, class... Args>
    tensor<T, Rank> pad(
        const base_tensor<T, Rank, Tag> &arg,
        const shape_t<Rank> &before, const shape_t<Rank> &after,
        Function func, Args&&... args
    ) {
        tensor<T, Rank> out = std::move(pad(arg, before, after));
        shape_t<Rank> shape = out.shape(), strides;
        size_t size = 1;
        for (size_t i = 0; i < shape.ndim(); ++i) {
            size_t j = shape.ndim() - 1 - i;
            strides[j] = size;
            size *= shape[j];
        }
        for (size_t axis = 0; axis < shape.ndim(); ++axis) {
            size = shape[axis];
            shape[axis] = 1;
            for (index_t<Rank> index : make_indices(shape)) {
                size_t offset = 0, stride = strides[axis];
                for (size_t i = 0; i < index.ndim(); ++i) {
                    offset += index[i] * strides[i];
                }
                tensor_view<T, 1> view(size, out.data(), offset, stride);
                func(
                    view, before[axis], after[axis], axis,
                    std::forward<Args>(args)...
                );
            }
            shape[axis] = size;
        }
        return out;
    }

    template <class T>
    inline void pad_constant::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
    ) const {
        this->operator()(view, before, after, axis, T(), T());
    }

    template <class T>
    inline void pad_constant::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
        const typename tensor<T, 1>::value_type &val
    ) const {
        this->operator()(view, before, after, axis, val, val);
    }

    template <class T>
    void pad_constant::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t,
        const typename tensor<T, 1>::value_type &val_before,
        const typename tensor<T, 1>::value_type &val_after
    ) const {
        for (size_t i = 0; i < before; ++i) {
            view[i] = val_before;
        }
        for (size_t i = 0; i < after; ++i) {
            view[view.size() - 1 - i] = val_after;
        }
    }

    template <class T, class Tag>
    inline void pad_constant::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
        const base_tensor<T, 2, Tag> &values
    ) const {
        this->operator()(
            view, before, after, axis, values(axis, 0), values(axis, 1)
        );
    }

    template <class T>
    void pad_edge::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
    ) const {
        T edge_before = view[before];
        T edge_after = view[view.size() - 1 - after];
        pad_constant::operator()(
            view, before, after, axis, edge_before, edge_after
        );
    }

    template <class T>
    inline void pad_linear_ramp::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis
    ) const {
        this->operator()(view, before, after, axis, T(), T());
    }

    template <class T>
    inline void pad_linear_ramp::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
        const typename tensor<T, 1>::value_type &end
    ) const {
        this->operator()(view, before, after, axis, end, end);
    }

    template <class T>
    void pad_linear_ramp::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t,
        const typename tensor<T, 1>::value_type &end_before,
        const typename tensor<T, 1>::value_type &end_after
    ) const {
        T edge_before = view[before];
        T edge_after = view[view.size() - 1 - after];
        for (size_t i = 0; i < before; ++i) {
            double t = (double)i / (double)before;
            view[i] = (1 - t)*end_before + t*edge_before;
        }
        for (size_t i = 0; i < after; ++i) {
            double t = (double)i / (double)after;
            view[view.size() - 1 - i] = (1 - t)*end_after + t*edge_after;
        }
    }

    template <class T, class Tag>
    inline void pad_linear_ramp::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t axis,
        const base_tensor<T, 2, Tag> &end_values
    ) const {
        this->operator()(
            view, before, after, axis, end_values(axis, 0), end_values(axis, 1)
        );
    }

    template <class T>
    void pad_reflect::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t
    ) const {
        size_t size = view.size() - before - after;
        for (size_t i = 0; i < before; ++i) {
            size_t idx = 1 + (before - 1 - i) % (size - 1);
            size_t nblock = (before - 1 - i) / (size - 1);
            view[i] = (nblock % 2 == 0)
                ? view[before + idx]
                : view[before + size - 1 - idx];
        }
        for (size_t i = 0; i < after; ++i) {
            size_t idx = 1 + (after - 1 - i) % (size - 1);
            size_t nblock = (after - 1 - i) / (size - 1);
            view[view.size() - 1 - i] = (nblock % 2 == 0)
                ? view[before + size - 1 - idx]
                : view[before + idx];
        }
    }

    template <class T>
    void pad_symmetric::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t
    ) const {
        size_t size = view.size() - before - after;
        for (size_t i = 0; i < before; ++i) {
            size_t idx = (before - 1 - i) % size;
            size_t nblock = (before - 1 - i) / size;
            view[i] = (nblock % 2 == 0)
                ? view[before + idx]
                : view[before + size - 1 - idx];
        }
        for (size_t i = 0; i < after; ++i) {
            size_t idx = (after - 1 - i) % size;
            size_t nblock = (after - 1 - i) / size;
            view[view.size() - 1 - i] = (nblock % 2 == 0)
                ? view[before + size - 1 - idx]
                : view[before + idx];
        }
    }

    template <class T>
    void pad_wrap::operator()(
        tensor_view<T, 1> &view, size_t before, size_t after, size_t
    ) const {
        size_t size = view.size() - before - after;
        for (size_t i = 0; i < before; ++i) {
            size_t idx = (before - 1 - i) % size;
            view[i] = view[before + size - 1 - idx];
        }
        for (size_t i = 0; i < after; ++i) {
            size_t idx = (after - 1 - i) % size;
            view[view.size() - 1 - i] = view[before + idx];
        }
    }

    /// Indexing routines.

    template <size_t Rank, size_t N, class Tag>
    base_tensor<
        size_t, N,
        lazy_unary_tag<detail::ravel_index<Rank>, index_t<Rank>, Tag>
    > ravel_index(
        const base_tensor<index_t<Rank>, N, Tag> &index,
        const shape_t<Rank> &shape, bool order
    ) {
        typedef size_t Rt;
        typedef lazy_unary_tag<detail::ravel_index<Rank>, index_t<Rank>, Tag>
            Closure;
        return base_tensor<Rt, N, Closure>(
            detail::ravel_index<Rank>(shape, order), index
        );
    }

    template <class IntegralType, size_t Rank, size_t N, class Tag,
              detail::RequiresIntegral<IntegralType> >
    base_tensor<
        index_t<Rank>, N,
        lazy_unary_tag<detail::unravel_index<Rank>, IntegralType, Tag>
    > unravel_index(
        const base_tensor<IntegralType, N, Tag> &index,
        const shape_t<Rank> &shape, bool order
    ) {
        typedef index_t<Rank> Rt;
        typedef lazy_unary_tag<detail::unravel_index<Rank>, IntegralType, Tag>
            Closure;
        return base_tensor<Rt, N, Closure>(
            detail::unravel_index<Rank>(shape, order), index
        );
    }

    template <class T, size_t Rank, class Tag, size_t N, class TagIndex>
    tensor<T, N> take(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<index_t<Rank>, N, TagIndex> &indices
    ) {
        tensor<T, N> out(indices.shape());
        for (index_t<N> i : make_indices(indices.shape())) {
            out[i] = a[indices[i]];
        }
        return out;
    }

    template <class T, class Tag, class IntegralType, size_t N, class TagIndex,
              detail::RequiresIntegral<IntegralType> >
    tensor<T, N> take(
        const base_tensor<T, 1, Tag> &a,
        const base_tensor<IntegralType, N, TagIndex> &indices
    ) {
        tensor<T, N> out(indices.shape());
        for (index_t<N> i : make_indices(indices.shape())) {
            out[i] = a[indices[i]];
        }
        return out;
    }

    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagIndex,
              detail::RequiresIntegral<IntegralType> >
    tensor<T, Rank> take(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, 1, TagIndex> &indices,
        size_t axis
    ) {
        shape_t<Rank> shape = a.shape();
        shape[axis] = indices.size();
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index = out_index;
            a_index[axis] = indices[a_index[axis]];
            out[out_index] = a[a_index];
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank - 1> take(
        const base_tensor<T, Rank, Tag> &a, size_t index, size_t axis
    ) {
        static_assert(Rank > 1, "Source tensor must be at least 2-dimensional");
        shape_t<Rank - 1> shape = detail::remove_axis(a.shape(), axis);
        tensor<T, Rank - 1> out(shape);
        for (index_t<Rank - 1> out_index : make_indices(shape)) {
            index_t<Rank> a_index = detail::insert_axis(out_index, axis, index);
            out[out_index] = a[a_index];
        }
        return out;
    }

namespace detail {
    /**
     * @brief Throws a std::invalid_argument exception if the shape of indices
     * does not match the shape of a tensor for all dimensions other than axis.
     */
    template <size_t Rank>
    void assert_index_along_axis_shape(
        const shape_t<Rank> &shape, const shape_t<Rank> &indices_shape,
        size_t axis
    ) {
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (indices_shape[i] != shape[i] && i != axis) {
                std::ostringstream error;
                error << "index did not match indexed tensor along dimension "
                      << i << "; dimension is " << shape[i]
                      << " but corresponding index dimension is "
                      << indices_shape[i];
                throw std::invalid_argument(error.str());
            }
        }
    }

    /**
     * @brief Throws a std::invalid_argument exception if the shape of values
     * does not match the shape of indices.
     */
    template <size_t Rank>
    void assert_indexed_values_shape(
        const shape_t<Rank> &indices_shape, const shape_t<Rank> &values_shape
    ) {
        if (values_shape != indices_shape) {
            std::ostringstream error;
            error << "shape mismatch: value tensor of shape " << values_shape
                  << " does not match the indexing shape " << indices_shape;
            throw std::invalid_argument(error.str());
        }
    }
}

    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagIndex,
              detail::RequiresIntegral<IntegralType> >
    tensor<T, Rank> take_along_axis(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, Rank, TagIndex> &indices,
        size_t axis
    ) {
        detail::assert_index_along_axis_shape(a.shape(), indices.shape(), axis);
        tensor<T, Rank> out(indices.shape());
        for (index_t<Rank> out_index : make_indices(indices.shape())) {
            index_t<Rank> a_index = out_index;
            a_index[axis] = indices[out_index];
            out[out_index] = a[a_index];
        }
        return out;
    }

    template <class T, size_t Rank, class Tag, class TagIndex, class TagValue>
    void put(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
        const base_tensor<T, 1, TagValue> &values
    ) {
        detail::assert_indexed_values_shape(indices.shape(), values.shape());
        for (size_t i = 0; i < indices.size(); ++i) {
            a[indices[i]] = values[i];
        }
    }

    template <class T, size_t Rank, class Tag, class TagIndex>
    void put(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<index_t<Rank>, 1, TagIndex> &indices,
        const typename tensor<T, 1>::value_type &value
    ) {
        for (size_t i = 0; i < indices.size(); ++i) {
            a[indices[i]] = value;
        }
    }

    template <class T, class Tag, class IntegralType,
              class TagIndex, class TagValue,
              detail::RequiresIntegral<IntegralType> >
    void put(
        base_tensor<T, 1, Tag> &a,
        const base_tensor<IntegralType, 1, TagIndex> &indices,
        const base_tensor<T, 1, TagValue> &values
    ) {
        detail::assert_indexed_values_shape(indices.shape(), values.shape());
        for (size_t i = 0; i < indices.size(); ++i) {
            a[indices[i]] = values[i];
        }
    }

    template <class T, class Tag, class IntegralType, class TagIndex,
              detail::RequiresIntegral<IntegralType> >
    void put(
        base_tensor<T, 1, Tag> &a,
        const base_tensor<IntegralType, 1, TagIndex> &indices,
        const typename tensor<T, 1>::value_type &value
    ) {
        for (size_t i = 0; i < indices.size(); ++i) {
            a[indices[i]] = value;
        }
    }

    template <class T, size_t Rank, class Tag,
              class IntegralType, class TagIndex, class TagValue,
              detail::RequiresIntegral<IntegralType> >
    void put_along_axis(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<IntegralType, Rank, TagIndex> &indices,
        const base_tensor<T, Rank, TagValue> &values,
        size_t axis
    ) {
        detail::assert_index_along_axis_shape(a.shape(), indices.shape(), axis);
        detail::assert_indexed_values_shape(indices.shape(), values.shape());
        for (index_t<Rank> i : make_indices(indices.shape())) {
            index_t<Rank> a_index = i;
            a_index[axis] = indices[i];
            a[a_index] = values[i];
        }
    }

namespace detail {
    /**
     * @brief Throws a std::invalid_argument exception if the shape of the
     * boolean mask does not match the shape of a tensor.
     */
    template <size_t Rank>
    void assert_mask_shape(
        const shape_t<Rank> &shape, const shape_t<Rank> &mask_shape
    ) {
        if (shape != mask_shape) {
            std::ostringstream error;
            error << "boolean index did not match indexed tensor; shape is "
                  << shape << " but corresponding boolean shape is "
                  << mask_shape;
            throw std::invalid_argument(error.str());
        }
    }

    /**
     * @brief Throws a std::invalid_argument exception if the size of the
     * boolean mask does not match the size of a tensor along the given axis.
     */
    void assert_mask_shape(size_t size, size_t mask_size, size_t axis) {
        if (size != mask_size) {
            std::ostringstream error;
            error << "boolean index did not match indexed tensor along "
                  << "dimension " << axis << "; dimension is " << size
                  << " but corresponding boolean dimension is " << mask_size;
            throw std::invalid_argument(error.str());
        }
    }
}

    template <class T, size_t Rank, class Tag, class TagCond>
    tensor<T, 1> compress(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition
    ) {
        detail::assert_mask_shape(a.shape(), condition.shape());
        size_t size = std::count(condition.begin(), condition.end(), true);
        tensor<T, 1> out(size);
        size_t n = 0;
        for (index_t<Rank> i : make_indices(condition.shape())) {
            if (condition[i]) {
                out[n++] = a[i];
            }
        }
        return out;
    }

    template <class T, size_t Rank, class Tag, class TagCond>
    tensor<T, Rank> compress(
        const base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, 1, TagCond> &condition,
        size_t axis
    ) {
        detail::assert_mask_shape(a.shape(axis), condition.size(), axis);
        shape_t<Rank> shape = a.shape();
        shape[axis] = std::count(condition.begin(), condition.end(), true);
        tensor<T, Rank> out(shape);
        shape[axis] = 1;
        for (index_t<Rank> out_index : make_indices(shape)) {
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
    void place(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, 1, TagValue> &values
    ) {
        detail::assert_mask_shape(a.shape(), condition.shape());
        size_t n = 0;
        for (index_t<Rank> i : make_indices(condition.shape())) {
            if (condition[i]) {
                a[i] = values[n++];
            }
        }
    }

    template <class T, size_t Rank, class Tag, class TagCond>
    void place(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition,
        const typename tensor<T, 1>::value_type &value
    ) {
        detail::assert_mask_shape(a.shape(), condition.shape());
        for (index_t<Rank> i : make_indices(condition.shape())) {
            if (condition[i]) {
                a[i] = value;
            }
        }
    }

    template <class T, size_t Rank, class Tag, class TagCond, class TagValue>
    void putmask(
        base_tensor<T, Rank, Tag> &a,
        const base_tensor<bool, Rank, TagCond> &condition,
        const base_tensor<T, Rank, TagValue> &values
    ) {
        detail::assert_mask_shape(a.shape(), condition.shape());
        detail::assert_indexed_values_shape(condition.shape(), values.shape());
        for (index_t<Rank> i : make_indices(condition.shape())) {
            if (condition[i]) {
                a[i] = values[i];
            }
        }
    }
}

#endif // NUMCPP_BROADCASTING_TCC_INCLUDED
