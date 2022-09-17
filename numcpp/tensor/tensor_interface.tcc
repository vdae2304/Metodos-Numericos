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

#include "numcpp/routines/ranges.h"
#include "numcpp/iterators/index_sequence.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    /// Iterators.

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin() {
        type& self = static_cast<type&>(*this);
        return iterator(static_cast<type*>(this), 0, self.layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin() const {
        const type& self = static_cast<const type&>(*this);
        return const_iterator(static_cast<const type*>(this), 0, self.layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin(layout_t order) {
        return iterator(static_cast<type*>(this), 0, order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin(layout_t order) const {
        return const_iterator(static_cast<const type*>(this), 0, order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end() {
        type& self = static_cast<type&>(*this);
        return iterator(static_cast<type*>(this), self.size(), self.layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end() const {
        const type& self = static_cast<const type&>(*this);
        return const_iterator(
            static_cast<const type*>(this), self.size(), self.layout()
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end(layout_t order) {
        type& self = static_cast<type&>(*this);
        return iterator(static_cast<type*>(this), self.size(), order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end(layout_t order) const {
        const type& self = static_cast<const type&>(*this);
        return const_iterator(
            static_cast<const type*>(this), self.size(), order
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cbegin() const {
        const type& self = static_cast<const type&>(*this);
        return const_iterator(static_cast<const type*>(this), 0, self.layout());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cbegin(layout_t order) const {
        return const_iterator(static_cast<const type*>(this), 0, order);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cend() const {
        const type& self = static_cast<const type&>(*this);
        return const_iterator(
            static_cast<const type*>(this), self.size(), self.layout()
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cend(layout_t order) const {
        const type& self = static_cast<const type&>(*this);
        return const_iterator(
            static_cast<const type*>(this), self.size(), order
        );
    }

    /// Compound assignment operator.

namespace detail {
    /**
     * @brief Broadcast an index to match the given shape.
     */
    template <size_t Rank>
    index_t<Rank> broadcast_index(
        index_t<Rank> index, const shape_t<Rank> &shape
    ) {
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (shape[i] == 1) {
                index[i] = 0;
            }
        }
        return index;
    }
}

    template <class T, size_t Rank, class Tag>
    template <class Function, class TagOp>
    base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::apply_binary_function(
        Function f, const base_tensor<T, Rank, TagOp> &rhs
    ) {
        type& self = static_cast<type&>(*this);
        shape_t<Rank> shape = self.shape();
        shape_t<Rank> common_shape = broadcast_shapes(shape, rhs.shape());
        if (shape != common_shape) {
            std::ostringstream error;
            error << "non-broadcastable output operand with shape " << shape
                  << " doesn't match the broadcast shape " << common_shape;
            throw std::invalid_argument(error.str());
        }
        for (index_t<Rank> i : make_indices(shape)) {
            self[i] = f(self[i], rhs[detail::broadcast_index(i, rhs.shape())]);
        }
        return self;
    }

    template <class T, size_t Rank, class Tag>
    template <class Function>
    base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::apply_binary_function(
        Function f, const T &val
    ) {
        type& self = static_cast<type&>(*this);
        for (index_t<Rank> i : make_indices(self.shape())) {
            self[i] = f(self[i], val);
        }
        return self;
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
        const type& self = static_cast<const type&>(*this);
        index_sequence<Rank> indices(self.shape());
        tensor<index_t<Rank>, 1> out(indices.begin(), self.size());
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(self[i], self[j]);
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
        const type& self = static_cast<const type&>(*this);
        index_sequence<Rank> indices(self.shape());
        tensor<index_t<Rank>, 1> out(indices.begin(), self.size());
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(self[i], self[j]);
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
        const type& self = static_cast<const type&>(*this);
        return base_tensor<U, Rank, Closure>(cast_to<U>(), self);
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
        const type& self = static_cast<const type&>(*this);
        return tensor<Rt, Rank>(self);
    }

    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::nonzero() const {
        const type& self = static_cast<const type&>(*this);
        size_t size = self.size();
        size -= std::count(this->begin(), this->end(), T());
        tensor<index_t<Rank>, 1> out(size);
        size_t n = 0;
        for (index_t<Rank> i : make_indices(self.shape())) {
            if (self[i] != T()) {
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
        type& self = static_cast<type&>(*this);
        shape_t<Rank> shape = self.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_axes_iterator(&self, index, axis, 0);
            auto last = make_axes_iterator(&self, index, axis, size);
            std::nth_element(first, first + kth, last, comp);
        }
    }

    template <class T, size_t Rank, class Tag>
    void tensor_interface<T, Rank, Tag>::reverse(size_t axis) {
        type& self = static_cast<type&>(*this);
        shape_t<Rank> shape = self.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_axes_iterator(&self, index, axis, 0);
            auto last = make_axes_iterator(&self, index, axis, size);
            std::reverse(first, last);
        }
    }

    template <class T, size_t Rank, class Tag>
    void tensor_interface<T, Rank, Tag>::shift(size_t count, size_t axis) {
        type& self = static_cast<type&>(*this);
        shape_t<Rank> shape = self.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_axes_iterator(&self, index, axis, 0);
            auto last = make_axes_iterator(&self, index, axis, size);
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
        type& self = static_cast<type&>(*this);
        shape_t<Rank> shape = self.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            auto first = make_axes_iterator(&self, index, axis, 0);
            auto last = make_axes_iterator(&self, index, axis, size);
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
        const type& self = static_cast<const type&>(*this);
        shape_t<Rank> shape = self.shape();
        size_t size = shape[axis];
        shape[axis] = 1;
        tensor<R, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            out[out_index] = f(
                make_const_axes_iterator(&self, out_index, axis, 0),
                make_const_axes_iterator(&self, out_index, axis, size)
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
        const type& self = static_cast<const type&>(*this);
        shape_t<Rank> shape = self.shape();
        size_t size = 1;
        for (size_t i = 0; i < axes.ndim(); ++i) {
            size *= shape[axes[i]];
            shape[axes[i]] = 1;
        }
        tensor<R, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            out[out_index] = f(
                make_const_axes_iterator(&self, out_index, axes, 0),
                make_const_axes_iterator(&self, out_index, axes, size)
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
    inline tensor<bool, Rank>
    tensor_interface<T, Rank, Tag>::all(size_t axis) const {
        return this->apply_along_axis<bool>(ranges::all(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<bool, Rank>
    tensor_interface<T, Rank, Tag>::all(const shape_t<N> &axes) const {
        return this->apply_over_axes<bool>(ranges::all(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline bool tensor_interface<T, Rank, Tag>::any() const {
        ranges::any pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<bool, Rank>
    tensor_interface<T, Rank, Tag>::any(size_t axis) const {
        return this->apply_along_axis<bool>(ranges::any(), axis);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<bool, Rank>
    tensor_interface<T, Rank, Tag>::any(const shape_t<N> &axes) const {
        return this->apply_over_axes<bool>(ranges::any(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> tensor_interface<T, Rank, Tag>::argmax() const {
        const type& self = static_cast<const type&>(*this);
        ranges::argmax pred;
        size_t index = pred(this->begin(), this->end());
        return unravel_index(index, self.shape(), self.layout());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank>
    tensor_interface<T, Rank, Tag>::argmax(size_t axis) const {
        return this->apply_along_axis<size_t>(ranges::argmax(), axis);
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> tensor_interface<T, Rank, Tag>::argmin() const {
        const type& self = static_cast<const type&>(*this);
        ranges::argmin pred;
        size_t index = pred(this->begin(), this->end());
        return unravel_index(index, self.shape(), self.layout());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank>
    tensor_interface<T, Rank, Tag>::argmin(size_t axis) const {
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

    /// Complex numbers.

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::real, std::complex<T>, Tag>
    > complex_interface<std::complex<T>, Rank, Tag>::real() const {
        typedef lazy_unary_tag<math::real, std::complex<T>, Tag> Closure;
        const type& self = static_cast<const type&>(*this);
        return base_tensor<T, Rank, Closure>(math::real(), self);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::real, const std::complex<T>, Tag>
    > complex_interface<const std::complex<T>, Rank, Tag>::real() const {
        typedef lazy_unary_tag<math::real, const std::complex<T>, Tag> Closure;
        const type& self = static_cast<const type&>(*this);
        return base_tensor<T, Rank, Closure>(math::real(), self);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagReal>
    void complex_interface<std::complex<T>, Rank, Tag>::real(
        const base_tensor<T, Rank, TagReal> &arg
    ) {
        type& self = static_cast<type&>(*this);
        if (self.shape() != arg.shape()) {
            std::ostringstream error;
            error << "input shape " << arg.shape() << " doesn't match the "
                  << "output shape " << self.shape();
            throw std::invalid_argument(error.str());
        }
        for (index_t<Rank> i : make_indices(self.shape())) {
            self[i].real(arg[i]);
        }
    }

    template <class T, size_t Rank, class Tag>
    void complex_interface<std::complex<T>, Rank, Tag>::real(const T &val) {
        type& self = static_cast<type&>(*this);
        for (index_t<Rank> i : make_indices(self.shape())) {
            self[i].real(val);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::imag, std::complex<T>, Tag>
    > complex_interface<std::complex<T>, Rank, Tag>::imag() const {
        typedef lazy_unary_tag<math::imag, std::complex<T>, Tag> Closure;
        const type& self = static_cast<const type&>(*this);
        return base_tensor<T, Rank, Closure>(math::imag(), self);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_unary_tag<math::imag, const std::complex<T>, Tag>
    > complex_interface<const std::complex<T>, Rank, Tag>::imag() const {
        typedef lazy_unary_tag<math::imag, const std::complex<T>, Tag> Closure;
        const type& self = static_cast<const type&>(*this);
        return base_tensor<T, Rank, Closure>(math::imag(), self);
    }

    template <class T, size_t Rank, class Tag>
    template <class TagImag>
    void complex_interface<std::complex<T>, Rank, Tag>::imag(
        const base_tensor<T, Rank, TagImag> &arg
    ) {
        type& self = static_cast<type&>(*this);
        if (self.shape() != arg.shape()) {
            std::ostringstream error;
            error << "input shape " << arg.shape() << " doesn't match the "
                  << "output shape " << self.shape();
            throw std::invalid_argument(error.str());
        }
        for (index_t<Rank> i : make_indices(self.shape())) {
            self[i].imag(arg[i]);
        }
    }

    template <class T, size_t Rank, class Tag>
    void complex_interface<std::complex<T>, Rank, Tag>::imag(const T &val) {
        type& self = static_cast<type&>(*this);
        for (index_t<Rank> i : make_indices(self.shape())) {
            self[i].imag(val);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        std::complex<T>, Rank,
        lazy_unary_tag<math::conj, std::complex<T>, Tag>
    > complex_interface<std::complex<T>, Rank, Tag>::conj() const {
        typedef std::complex<T> Rt;
        typedef lazy_unary_tag<math::conj, std::complex<T>, Tag> Closure;
        const type& self = static_cast<const type&>(*this);
        return base_tensor<Rt, Rank, Closure>(math::conj(), self);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        std::complex<T>, Rank,
        lazy_unary_tag<math::conj, const std::complex<T>, Tag>
    > complex_interface<const std::complex<T>, Rank, Tag>::conj() const {
        typedef std::complex<T> Rt;
        typedef lazy_unary_tag<math::conj, const std::complex<T>, Tag> Closure;
        const type& self = static_cast<const type&>(*this);
        return base_tensor<Rt, Rank, Closure>(math::conj(), self);
    }
}

#endif // NUMCPP_TENSOR_INTERFACE_TCC_INCLUDED
