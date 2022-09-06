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

 /** @file include/numcpp/tensor/tensor_interface.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_INTERFACE_TCC_INCLUDED
#define NUMCPP_TENSOR_INTERFACE_TCC_INCLUDED

#include "numcpp/iterators/index_sequence.h"
#include "numcpp/routines/ranges.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    /// Iterators.

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, Tag>* tensor_interface<T, Rank, Tag>::base() {
        return static_cast<base_tensor<T, Rank, Tag>*>(this);
    }

    template <class T, size_t Rank, class Tag>
    const base_tensor<T, Rank, Tag>*
    tensor_interface<T, Rank, Tag>::base() const {
        return static_cast<const base_tensor<T, Rank, Tag>*>(this);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin() {
        return this->begin(this->base()->layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin() const {
        return this->begin(this->base()->layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin(layout_t order) {
        return make_tensor_iterator(this->base(), 0, order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin(layout_t order) const {
        return make_tensor_const_iterator(this->base(), 0, order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end() {
        return this->end(this->base()->layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end() const {
        return this->end(this->base()->layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end(layout_t order) {
        return make_tensor_iterator(this->base(), this->base()->size(), order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end(layout_t order) const {
        return make_tensor_const_iterator(
            this->base(), this->base()->size(), order
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cbegin() const {
        return this->cbegin(this->base()->layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cbegin(layout_t order) const {
        return make_tensor_const_iterator(this->base(), 0, order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cend() const {
        return this->cend(this->base()->layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cend(layout_t order) const {
        return make_tensor_const_iterator(
            this->base(), this->base()->size(), order
        );
    }

    /// Compound assignment operator.

namespace detail {
    /**
     * @brief Broadcast an index to match the given shape.
     */
    template <size_t Rank>
    index_t<Rank> broadcast_index(
        const index_t<Rank> &index, const shape_t<Rank> &shape
    ) {
        index_t<Rank> out_index = index;
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (shape[i] == 1) {
                out_index[i] = 0;
            }
        }
        return out_index;
    }
}

    template <class T, size_t Rank, class Tag>
    template <class Function, class TagOp>
    base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::apply_binary_function(
        Function f, const base_tensor<T, Rank, TagOp> &rhs
    ) {
        shape_t<Rank> shape = this->base()->shape();
        shape_t<Rank> common_shape = broadcast_shapes(shape, rhs.shape());
        if (shape != common_shape) {
            std::ostringstream error;
            error << "non-broadcastable output operand with shape " << shape
                  << " doesn't match the broadcast shape " << common_shape;
            throw std::invalid_argument(error.str());
        }
        for (index_t<Rank> i : make_indices(shape)) {
            this->base()->operator[](i) = f(
                this->base()->operator[](i),
                rhs[detail::broadcast_index(i, rhs.shape())]
            );
        }
        return *this->base();
    }

    template <class T, size_t Rank, class Tag>
    template <class Function>
    base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::apply_binary_function(
        Function f, const T &val
    ) {
        shape_t<Rank> shape = this->base()->shape();
        for (index_t<Rank> i : make_indices(shape)) {
            this->base()->operator[](i) = f(this->base()->operator[](i), val);
        }
        return *this->base();
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator+=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(plus(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator-=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(minus(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator*=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(multiplies(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator/=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(divides(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator%=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(modulus(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator&=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(bit_and(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator|=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(bit_or(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator^=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(bit_xor(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator<<=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(left_shift(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagOp>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator>>=(
        const base_tensor<T, Rank, TagOp> &rhs
    ) {
        return this->apply_binary_function(right_shift(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator+=(const T &val) {
        return this->apply_binary_function(plus(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator-=(const T &val) {
        return this->apply_binary_function(minus(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator*=(const T &val) {
        return this->apply_binary_function(multiplies(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator/=(const T &val) {
        return this->apply_binary_function(divides(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator%=(const T &val) {
        return this->apply_binary_function(modulus(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator&=(const T &val) {
        return this->apply_binary_function(bit_and(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator|=(const T &val) {
        return this->apply_binary_function(bit_or(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator^=(const T &val) {
        return this->apply_binary_function(bit_xor(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator<<=(const T &val) {
        return this->apply_binary_function(left_shift(), val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator>>=(const T &val) {
        return this->apply_binary_function(right_shift(), val);
    }

    /// Public methods.

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::apply(T f(T)) {
        std::transform(this->begin(), this->end(), this->begin(), f);
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::apply(T f(const T&)) {
        std::transform(this->begin(), this->end(), this->begin(), f);
    }

    template <class T, size_t Rank, class Tag>
    template <class Function>
    inline void tensor_interface<T, Rank, Tag>::apply(Function &&f) {
        std::transform(
            this->begin(), this->end(), this->begin(),
            std::forward<Function>(f)
        );
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<index_t<Rank>, 1>
    tensor_interface<T, Rank, Tag>::argpartition(size_t kth) const {
        return this->argpartition(kth, less());
    }

    template <class T, size_t Rank, class Tag>
    template <class Compare>
    tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::argpartition(
        size_t kth, Compare comp
    ) const {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = this->base()->size();
        tensor<index_t<Rank>, 1> out(size, make_indices(shape).begin());
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(this->base()->operator[](i),
                        this->base()->operator[](j));
        };
        std::nth_element(out.begin(), out.begin() + kth, out.end(), comparator);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<index_t<Rank>, 1>
    tensor_interface<T, Rank, Tag>::argsort() const {
        return this->argsort(less());
    }

    template <class T, size_t Rank, class Tag>
    template <class Compare>
    tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::argsort(
        Compare comp, bool stable
    ) const {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = this->base()->size();
        tensor<index_t<Rank>, 1> out(size, make_indices(shape).begin());
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(this->base()->operator[](i),
                        this->base()->operator[](j));
        };
        if (stable) {
            std::stable_sort(out.begin(), out.end(), comparator);
        }
        else {
            std::sort(out.begin(), out.end(), comparator);
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    template <class U>
    inline base_tensor<U, Rank, lazy_unary_tag<cast_to<U>, T, Tag> >
    tensor_interface<T, Rank, Tag>::astype() const {
        typedef lazy_unary_tag<cast_to<U>, T, Tag> Closure;
        return base_tensor<U, Rank, Closure>(cast_to<U>(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::clamp(
        const T &a_min, const T &a_max
    ) {
        this->apply(ranges::clamp<T>(a_min, a_max));
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::copy() const {
        typedef typename std::remove_cv<T>::type Rt;
        return tensor<Rt, Rank>(*this->base());
    }

    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::nonzero() const {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = this->base()->size();
        size -= std::count(this->begin(), this->end(), T());
        tensor<index_t<Rank>, 1> out(size);
        size_t n = 0;
        for (index_t<Rank> i : make_indices(shape)) {
            if (this->base()->operator[](i) != T()) {
                out[n++] = i;
            }
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::partition(
        size_t kth, size_t axis
    ) {
        this->partition(kth, axis, less());
    }

    template <class T, size_t Rank, class Tag>
    template <class Compare>
    void tensor_interface<T, Rank, Tag>::partition(
        size_t kth, size_t axis, Compare comp
    ) {
        shape_t<Rank> shape = this->base()->shape();
        detail::assert_within_bounds(shape, kth, axis);
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_reduce_iterator(this->base(), index, axis, 0);
            auto last = make_reduce_iterator(this->base(), index, axis, size);
            std::nth_element(first, first + kth, last, comp);
        }
    }

    template <class T, size_t Rank, class Tag>
    void tensor_interface<T, Rank, Tag>::reverse(size_t axis) {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_reduce_iterator(this->base(), index, axis, 0);
            auto last = make_reduce_iterator(this->base(), index, axis, size);
            std::reverse(first, last);
        }
    }

    template <class T, size_t Rank, class Tag>
    void tensor_interface<T, Rank, Tag>::shift(size_t count, size_t axis) {
        shape_t<Rank> shape = this->base()->shape();
        detail::assert_within_bounds(shape, count, axis);
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_reduce_iterator(this->base(), index, axis, 0);
            auto last = make_reduce_iterator(this->base(), index, axis, size);
            std::rotate(first, first + count, last);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::sort(size_t axis) {
        this->sort(axis, less());
    }

    template <class T, size_t Rank, class Tag>
    template <class Compare>
    void tensor_interface<T, Rank, Tag>::sort(
        size_t axis, Compare comp, bool stable
    ) {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_reduce_iterator(this->base(), index, axis, 0);
            auto last = make_reduce_iterator(this->base(), index, axis, size);
            if (stable) {
                std::stable_sort(first, last, comp);
            }
            else {
                std::sort(first, last, comp);
            }
        }
    }

    /// Reductions.

    template <class T, size_t Rank, class Tag>
    template <class R, class Function>
    tensor<R, Rank> tensor_interface<T, Rank, Tag>::apply_along_axis(
        Function f, size_t axis
    ) const {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        tensor<R, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            out[out_index] = f(
                make_const_reduce_iterator(this->base(), out_index, axis, 0),
                make_const_reduce_iterator(this->base(), out_index, axis, size)
            );
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    template <class R, class Function, size_t N>
    tensor<R, Rank> tensor_interface<T, Rank, Tag>::apply_over_axes(
        Function f, const shape_t<N> &axes
    ) const {
        static_assert(N <= Rank, "Reduction dimension must be less or equal to"
                      " tensor dimension");
        shape_t<Rank> shape = this->base()->shape();
        size_t size = 1;
        for (size_t i = 0; i < axes.ndim(); ++i) {
            size *= shape[axes[i]];
            shape[axes[i]] = 1;
        }
        tensor<R, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            out[out_index] = f(
                make_const_reduce_iterator(this->base(), out_index, axes, 0),
                make_const_reduce_iterator(this->base(), out_index, axes, size)
            );
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline bool tensor_interface<T, Rank, Tag>::all() const {
        ranges::all pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::all(size_t axis)
    const {
        return this->apply_along_axis<bool>(ranges::all(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::all(
        const shape_t<N> &axes
    ) const {
        return this->apply_over_axes<bool>(ranges::all(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline bool tensor_interface<T, Rank, Tag>::any() const {
        ranges::any pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::any(size_t axis)
    const {
        return this->apply_along_axis<bool>(ranges::any(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::any(
        const shape_t<N> &axes
    ) const {
        return this->apply_over_axes<bool>(ranges::any(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> tensor_interface<T, Rank, Tag>::argmax() const {
        ranges::argmax pred;
        size_t index = pred(this->begin(), this->end());
        layout_t order = this->base()->layout();
        return unravel_index(index, this->base()->shape(), order);
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank> tensor_interface<T, Rank, Tag>::argmax(
        size_t axis
    ) const {
        return this->apply_along_axis<size_t>(ranges::argmax(), axis);
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> tensor_interface<T, Rank, Tag>::argmin() const {
        ranges::argmin pred;
        size_t index = pred(this->begin(), this->end());
        layout_t order = this->base()->layout();
        return unravel_index(index, this->base()->shape(), order);
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank> tensor_interface<T, Rank, Tag>::argmin(
        size_t axis
    ) const {
        return this->apply_along_axis<size_t>(ranges::argmin(), axis);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::max() const {
        ranges::max pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::max(size_t axis) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::max(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::max(const shape_t<N> &axes) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::max(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::mean() const {
        ranges::mean pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::mean(size_t axis) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::mean(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::mean(const shape_t<N> &axes) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::mean(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::min() const {
        ranges::min pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::min(size_t axis) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::min(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::min(const shape_t<N> &axes) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::min(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::prod() const {
        ranges::prod pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::prod(size_t axis) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::prod(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::prod(const shape_t<N> &axes) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::prod(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::stddev(bool bias) const {
        ranges::stddev pred(bias);
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::stddev(size_t axis, bool bias) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::stddev(bias), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::stddev(const shape_t<N> &axes, bool bias)
    const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::stddev(bias), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::sum() const {
        ranges::sum pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::sum(size_t axis) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::sum(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::sum(const shape_t<N> &axes) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::sum(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline typename std::remove_cv<T>::type
    tensor_interface<T, Rank, Tag>::var(bool bias) const {
        ranges::var pred(bias);
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::var(size_t axis, bool bias) const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_along_axis<Rt>(ranges::var(bias), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::var(const shape_t<N> &axes, bool bias)
    const {
        typedef typename std::remove_cv<T>::type Rt;
        return this->apply_over_axes<Rt>(ranges::var(bias), axes);
    }
}

#endif // NUMCPP_TENSOR_INTERFACE_TCC_INCLUDED
