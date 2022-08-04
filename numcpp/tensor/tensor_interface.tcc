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
    const base_tensor<T, Rank, Tag>* tensor_interface<T, Rank, Tag>::base()
    const {
        return static_cast<const base_tensor<T, Rank, Tag>*>(this);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin() {
        return this->begin(this->base()->rowmajor());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin() const {
        return this->begin(this->base()->rowmajor());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin(bool row_major) {
        return make_tensor_iterator(this->base(), 0, row_major);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::begin(bool row_major) const {
        return make_tensor_const_iterator(this->base(), 0, row_major);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end() {
        return this->end(this->base()->rowmajor());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end() const {
        return this->end(this->base()->rowmajor());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end(bool row_major) {
        return make_tensor_iterator(
            this->base(), this->base()->size(), row_major
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::end(bool row_major) const {
        return make_tensor_const_iterator(
            this->base(), this->base()->size(), row_major
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cbegin() const {
        return this->cbegin(this->base()->rowmajor());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cbegin(bool row_major) const {
        return make_tensor_const_iterator(this->base(), 0, row_major);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cend() const {
        return this->cend(this->base()->rowmajor());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    tensor_interface<T, Rank, Tag>::cend(bool row_major) const {
        return make_tensor_const_iterator(
            this->base(), this->base()->size(), row_major
        );
    }

    /// Compound assignment operator.

    template <class T, size_t Rank, class Tag>
    template <class Function, class U, class TagU>
    base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::apply_binary_function(
        Function f, const base_tensor<U, Rank, TagU> &rhs
    ) {
        typedef typename tensor_interface<T, Rank, Tag>::iterator iterator;
        shape_t<Rank> output_shape = this->base()->shape();
        shape_t<Rank> common_shape = broadcast_shapes(output_shape,rhs.shape());
        if (output_shape != common_shape) {
            std::ostringstream error;
            error << "non-broadcastable output operand with shape "
                  << output_shape << " doesn't match the broadcast shape "
                  << common_shape;
            throw std::invalid_argument(error.str());
        }
        iterator first = this->begin(), last = this->end();
        while (first != last) {
            index_t<Rank> index = first.coords();
            for (size_t i = 0; i < Rank; ++i) {
                index[i] = (rhs.shape(i) == 1) ? 0 : index[i];
            }
            *first = f(*first, rhs[index]);
            ++first;
        }
        return *this->base();
    }

    template <class T, size_t Rank, class Tag>
    template <class Function>
    base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::apply_binary_function(
        Function f, const T &val
    ) {
        typedef typename tensor_interface<T, Rank, Tag>::iterator iterator;
        iterator first1 = this->begin(), last1 = this->end();
        while (first1 != last1) {
            *first1 = f(*first1, val);
            ++first1;
        }
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator+=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(plus(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator-=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(minus(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator*=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(multiplies(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator/=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(divides(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator%=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(modulus(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator&=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(bit_and(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator|=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(bit_or(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator^=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(bit_xor(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator<<=(
        const base_tensor<U, Rank, TagU> &rhs
    ) {
        return this->apply_binary_function(left_shift(), rhs);
    }

    template <class T, size_t Rank, class Tag>
    template <class U, class TagU>
    inline base_tensor<T, Rank, Tag>&
    tensor_interface<T, Rank, Tag>::operator>>=(
        const base_tensor<U, Rank, TagU> &rhs
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
    inline void tensor_interface<T, Rank, Tag>::apply(Function f) {
        std::transform(this->begin(), this->end(), this->begin(), f);
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
        size_t size = shape.size();
        tensor<index_t<Rank>, 1> out(size);
        for (size_t i = 0; i < size; ++i) {
            out[i] = unravel_index(i, shape);
        }
        auto comparator = [&](const index_t<Rank> &i, const index_t<Rank> &j) {
            return comp(this->base()->operator[](i),
                        this->base()->operator[](j));
        };
        std::nth_element(out.begin(), out.begin() + kth, out.end(), comparator);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::argsort(
        bool stable
    ) const {
        return this->argsort(less(), stable);
    }

    template <class T, size_t Rank, class Tag>
    template <class Compare>
    tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::argsort(
        Compare comp, bool stable
    ) const {
        shape_t<Rank> shape = this->base()->shape();
        size_t size = shape.size();
        tensor<index_t<Rank>, 1> out(size);
        for (size_t i = 0; i < size; ++i) {
            out[i] = unravel_index(i, shape);
        }
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
    inline base_tensor<U, Rank, lazy_unary_tag<identity, T, Tag> >
    tensor_interface<T, Rank, Tag>::astype() const {
        typedef lazy_unary_tag<identity, T, tensor_tag> Closure;
        return base_tensor<U, Rank, Closure>(identity(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<math_conj, T, Tag> >
    tensor_interface<T, Rank, Tag>::conj() const {
        typedef lazy_unary_tag<math_conj, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(math_conj(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::clamp(
        const T &a_min, const T &a_max
    ) {
        this->apply(detail::clamp<T>(a_min, a_max));
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<typename std::remove_cv<T>::type, Rank>
    tensor_interface<T, Rank, Tag>::copy() const {
        typedef typename std::remove_cv<T>::type Rt;
        return tensor<Rt, Rank>(*this->base());
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<detail::complex_value_type<T>, Rank,
                       lazy_unary_tag<math_imag, T, Tag> >
    tensor_interface<T, Rank, Tag>::imag() const {
        typedef detail::complex_value_type<T> Rt;
        typedef lazy_unary_tag<math_imag, T, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(math_imag(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    template <class TagArg>
    void tensor_interface<T, Rank, Tag>::imag(
        const base_tensor<detail::complex_value_type<T>, Rank, TagArg> &arg
    ) {
        typedef typename tensor_interface<T, Rank, Tag>::iterator iterator;
        if (this->shape() != arg.shape()) {
            std::ostringstream error;
            error << "input shape " << arg.shape() << " doesn't match the "
                  << "output shape " << this->shape();
            throw std::invalid_argument(error.str());
        }
        auto first = arg.begin(this->base()->rowmajor());
        for (iterator it = this->begin(); it != this->end(); ++it) {
            it->imag(*first);
            ++first;
        }
    }

    template <class T, size_t Rank, class Tag>
    void tensor_interface<T, Rank, Tag>::imag(
        const detail::complex_value_type<T> &val
    ) {
        typedef typename tensor_interface<T, Rank, Tag>::iterator iterator;
        for (iterator it = this->begin(); it != this->end(); ++it) {
            it->imag(val);
        }
    }

    template <class T, size_t Rank, class Tag>
    tensor<index_t<Rank>, 1> tensor_interface<T, Rank, Tag>::nonzero() const {
        typedef typename tensor_interface<T,Rank,Tag>::const_iterator iterator;
        size_t size = this->base()->size();
        size -= std::count(this->begin(), this->end(), T(0));
        tensor<index_t<Rank>, 1> out(size);
        size_t n = 0;
        for (iterator it = this->begin(); it != this->end(); ++it) {
            if (*it != T(0)) {
                out[n++] = it.coords();
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
        typedef base_tensor_reduce_iterator<T, Rank, Tag, 1> iterator;
        shape_t<Rank> shape = this->base()->shape();
        assert_within_bounds(shape, kth, axis);
        size_t n = shape[axis];
        shape[axis] = 1;
        size_t size = shape.size();
        for (size_t i = 0; i < size; ++i) {
            index_t<Rank> indices = unravel_index(i, shape);
            shape_t<1> axes(axis);
            iterator first = make_reduce_iterator(this->base(),indices,axes,0);
            iterator last = make_reduce_iterator(this->base(),indices,axes,n);
            std::nth_element(first, first + kth, last, comp);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<detail::complex_value_type<T>, Rank,
                       lazy_unary_tag<math_real, T, Tag> >
    tensor_interface<T, Rank, Tag>::real() const {
        typedef detail::complex_value_type<T> Rt;
        typedef lazy_unary_tag<math_real, T, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(math_real(), *this->base());
    }

    template <class T, size_t Rank, class Tag>
    template <class TagArg>
    void tensor_interface<T, Rank, Tag>::real(
        const base_tensor<detail::complex_value_type<T>, Rank, TagArg> &arg
    ) {
        typedef typename tensor_interface<T, Rank, Tag>::iterator iterator;
        if (this->shape() != arg.shape()) {
            std::ostringstream error;
            error << "input shape " << arg.shape() << " doesn't match the "
                  << "output shape " << this->shape();
            throw std::invalid_argument(error.str());
        }
        auto first = arg.begin(this->base()->rowmajor());
        for (iterator it = this->begin(); it != this->end(); ++it) {
            it->real(*first);
            ++first;
        }
    }

    template <class T, size_t Rank, class Tag>
    void tensor_interface<T, Rank, Tag>::real(
        const detail::complex_value_type<T> &val
    ) {
        typedef typename tensor_interface<T, Rank, Tag>::iterator iterator;
        for (iterator it = this->begin(); it != this->end(); ++it) {
            it->real(val);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::reverse(size_t axis) {
        typedef base_tensor_reduce_iterator<T, Rank, Tag, 1> iterator;
        shape_t<Rank> shape = this->base()->shape();
        size_t n = shape[axis];
        shape[axis] = 1;
        size_t size = shape.size();
        for (size_t i = 0; i < size; ++i) {
            index_t<Rank> indices = unravel_index(i, shape);
            shape_t<1> axes(axis);
            iterator first = make_reduce_iterator(this->base(),indices,axes,0);
            iterator last = make_reduce_iterator(this->base(),indices,axes,n);
            std::reverse(first, last);
        }
    }

    template <class T, size_t Rank, class Tag>
    inline void tensor_interface<T, Rank, Tag>::sort(size_t axis, bool stable) {
        this->sort(axis, less(), stable);
    }

    template <class T, size_t Rank, class Tag>
    template <class Compare>
    void tensor_interface<T, Rank, Tag>::sort(
        size_t axis, Compare comp, bool stable
    ) {
        typedef base_tensor_reduce_iterator<T, Rank, Tag, 1> iterator;
        shape_t<Rank> shape = this->base()->shape();
        size_t n = shape[axis];
        shape[axis] = 1;
        size_t size = shape.size();
        for (size_t i = 0; i < size; ++i) {
            index_t<Rank> indices = unravel_index(i, shape);
            shape_t<1> axes(axis);
            iterator first = make_reduce_iterator(this->base(),indices,axes,0);
            iterator last = make_reduce_iterator(this->base(),indices,axes,n);
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
    template <class R, class Function, size_t N>
    tensor<R, Rank> tensor_interface<T, Rank, Tag>::reduce(
        Function f, const shape_t<N> &axes
    ) const {
        typedef typename tensor<R, Rank>::iterator iterator;
        static_assert(N <= Rank, "Reduction dimension must be less or equal to"
                      " tensor dimension");
        size_t size = 1;
        shape_t<Rank> shape = this->base()->shape();
        for (size_t i = 0; i < N; ++i) {
            size *= shape[axes[i]];
            shape[axes[i]] = 1;
        }
        tensor<R, Rank> out(shape);
        for (iterator it = out.begin(); it != out.end(); ++it) {
            index_t<Rank> indices = it.coords();
            *it = f(
                make_const_reduce_iterator(this->base(), indices, axes, 0),
                make_const_reduce_iterator(this->base(), indices, axes, size)
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
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::all(size_t axes)
    const {
        return this->all(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::all(
        const shape_t<N> &axes
    ) const {
        return this->reduce<bool>(ranges::all(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline bool tensor_interface<T, Rank, Tag>::any() const {
        ranges::any pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::any(size_t axes)
    const {
        return this->any(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<bool, Rank> tensor_interface<T, Rank, Tag>::any(
        const shape_t<N> &axes
    ) const {
        return this->reduce<bool>(ranges::any(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> tensor_interface<T, Rank, Tag>::argmax() const {
        ranges::argmax pred;
        size_t index = pred(this->begin(), this->end());
        bool order = this->base()->rowmajor();
        return unravel_index(index, this->base()->shape(), order);
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank> tensor_interface<T, Rank, Tag>::argmax(
        size_t axis
    ) const {
        return this->reduce<size_t>(ranges::argmax(), make_shape(axis));
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> tensor_interface<T, Rank, Tag>::argmin() const {
        ranges::argmin pred;
        size_t index = pred(this->begin(), this->end());
        bool order = this->base()->rowmajor();
        return unravel_index(index, this->base()->shape(), order);
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<size_t, Rank> tensor_interface<T, Rank, Tag>::argmin(
        size_t axis
    ) const {
        return this->reduce<size_t>(ranges::argmin(), make_shape(axis));
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::max() const {
        ranges::max pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::max(size_t axes)
    const {
        return this->max(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::max(
        const shape_t<N> &axes
    ) const {
        return this->reduce<T>(ranges::max(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::mean() const {
        ranges::mean pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::mean(size_t axes)
    const {
        return this->mean(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::mean(
        const shape_t<N> &axes
    ) const {
        return this->reduce<T>(ranges::mean(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::min() const {
        ranges::min pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::min(size_t axes)
    const {
        return this->min(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::min(
        const shape_t<N> &axes
    ) const {
        return this->reduce<T>(ranges::min(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::prod() const {
        ranges::prod pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::prod(size_t axes)
    const {
        return this->prod(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::prod(
        const shape_t<N> &axes
    ) const {
        return this->reduce<T>(ranges::prod(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::stddev(size_t ddof) const {
        ranges::stddev pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::stddev(
        size_t axes, size_t ddof
    ) const {
        return this->stddev(make_shape(axes), ddof);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::stddev(
        const shape_t<N> &axes, size_t ddof
    ) const {
        return this->reduce<T>(ranges::stddev(ddof), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::sum() const {
        ranges::sum pred;
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::sum(size_t axes)
    const {
        return this->sum(make_shape(axes));
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::sum(
        const shape_t<N> &axes
    ) const {
        return this->reduce<T>(ranges::sum(), axes);
    }

    template <class T, size_t Rank, class Tag>
    inline T tensor_interface<T, Rank, Tag>::var(size_t ddof) const {
        ranges::var pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T, size_t Rank, class Tag>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::var(
        size_t axes, size_t ddof
    ) const {
        return this->var(make_shape(axes), ddof);
    }

    template <class T, size_t Rank, class Tag>
    template <size_t N>
    inline tensor<T, Rank> tensor_interface<T, Rank, Tag>::var(
        const shape_t<N> &axes, size_t ddof
    ) const {
        return this->reduce<T>(ranges::var(ddof), axes);
    }
}

#endif // NUMCPP_TENSOR_INTERFACE_TCC_INCLUDED
