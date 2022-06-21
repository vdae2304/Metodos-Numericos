/*
 * This file is part of the NumCpp project.
 *
 * NumCpp is a package for scientific computing in C++. It is a C++ library 
 * that provides an array and a matrix object, and an assortment of routines 
 * for fast operations on arrays and matrices, including mathematical, logical, 
 * sorting, selecting, I/O and much more.
 *
 * The NumCpp package is inspired by the NumPy package for Python, although it
 * is not related to it or any of its parts.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/array/array_view.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/array.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_VIEW_TCC_INCLUDED
#define NUMCPP_ARRAY_VIEW_TCC_INCLUDED

#include <algorithm>
#include <cstdio>
#include <stdexcept>

namespace numcpp {
    /// Constructors.

    template <class T>
    base_array<T, array_view_tag>::base_array() {
        this->m_data = NULL;
        this->m_size = 0;
        this->m_offset = 0;
        this->m_stride = 1;
    }

    template <class T>
    base_array<T, array_view_tag>::base_array(size_t n, T *data) {
        this->m_data = data;
        this->m_size = n;
        this->m_offset = 0;
        this->m_stride = 1;
    }

    template <class T>
    base_array<T, array_view_tag>::base_array(
        size_t n, T *data, size_t offset, size_t stride
    ) {
        this->m_data = data;
        this->m_size = n;
        this->m_offset = offset;
        this->m_stride = stride;
    }

    template <class T>
    base_array<T, array_view_tag>::base_array(
        const base_array<T, array_view_tag> &other
    ) {
        this->m_data = other.m_data;
        this->m_size = other.m_size;
        this->m_offset = other.m_offset;
        this->m_stride = other.m_stride;
    }

    template <class T>
    base_array<T, array_view_tag>::base_array(
        base_array<T, array_view_tag> &&other
    ) {
        this->m_data = other.m_data;
        this->m_size = other.m_size;
        this->m_offset = other.m_offset;
        this->m_stride = other.m_stride;
        other.m_data = NULL;
        other.m_size = 0;
        other.m_offset = 0;
        other.m_stride = 1;
    }

    /// Destructor.

    template <class T>
    base_array<T, array_view_tag>::~base_array() {}

    /// Iterators.

    template <class T>
    inline typename base_array<T, array_view_tag>::iterator
    base_array<T, array_view_tag>::begin() {
        return base_array_iterator<T, array_view_tag>(this, 0);
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_iterator
    base_array<T, array_view_tag>::begin() const {
        return base_array_const_iterator<T, array_view_tag>(this, 0);
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::iterator
    base_array<T, array_view_tag>::end() {
        return base_array_iterator<T, array_view_tag>(this, this->m_size);
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_iterator
    base_array<T, array_view_tag>::end() const {
        return base_array_const_iterator<T, array_view_tag>(this, this->m_size);
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::reverse_iterator 
    base_array<T, array_view_tag>::rbegin() {
        return typename base_array<T, array_view_tag>::reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_reverse_iterator 
    base_array<T, array_view_tag>::rbegin() const {
        return typename base_array<T, array_view_tag>::const_reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::reverse_iterator 
    base_array<T, array_view_tag>::rend() {
        return typename base_array<T, array_view_tag>::reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_reverse_iterator 
    base_array<T, array_view_tag>::rend() const {
        return typename base_array<T, array_view_tag>::const_reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_iterator
    base_array<T, array_view_tag>::cbegin() const {
        return base_array_const_iterator<T, array_view_tag>(this, 0);
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_iterator
    base_array<T, array_view_tag>::cend() const {
        return base_array_const_iterator<T, array_view_tag>(this, this->m_size);
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_reverse_iterator 
    base_array<T, array_view_tag>::crbegin() const {
        return typename base_array<T, array_view_tag>::const_reverse_iterator(
            this->cend()
        );
    }

    template <class T>
    inline typename base_array<T, array_view_tag>::const_reverse_iterator 
    base_array<T, array_view_tag>::crend() const {
        return typename base_array<T, array_view_tag>::const_reverse_iterator(
            this->cbegin()
        );
    }

    /// Array indexing.

    template <class T>
    inline T& base_array<T, array_view_tag>::operator[](size_t i) {
        __assert_within_bounds(this->m_size, i);
        return this->m_data[this->m_offset + i*this->m_stride];
    }

    template <class T>
    const T& base_array<T, array_view_tag>::operator[](size_t i) const {
        __assert_within_bounds(this->m_size, i);
        return this->m_data[this->m_offset + i*this->m_stride];
    }

    template <class T>
    inline size_t base_array<T, array_view_tag>::size() const {
        return this->m_size;
    }

    template <class T>
    inline bool base_array<T, array_view_tag>::empty() const {
        return (this->m_size == 0);
    }

    template <class T>
    inline T* base_array<T, array_view_tag>::data() {
        return this->m_data;
    }

    template <class T>
    inline const T* base_array<T, array_view_tag>::data() const {
        return this->m_data;
    }

    /// Assignment operator.

    /// Helper function: Throws a std::invalid_argument exception if two 
    /// arrays have different lengths.
    inline void __assert_equal_length(size_t lhs_size, size_t rhs_size) {
        if (lhs_size != rhs_size) {
            char error[110];
            sprintf(
                error, "operands could not be broadcast together with shapes "
                "(%zu,) (%zu,)", lhs_size, rhs_size
            );
            throw std::invalid_argument(error);
        }
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator=(
        const base_array<T, array_view_tag> &other
    ) {
        __assert_equal_length(this->size(), other.size());
        std::copy(other.begin(), other.end(), this->begin());
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator=(
        const base_array<T, Tag> &other
    ) {
        __assert_equal_length(this->size(), other.size());
        std::copy(other.begin(), other.end(), this->begin());
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator=(const T &val) {
        std::fill_n(this->begin(), this->size(), val);
        return *this;
    }
    
    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator=(
        base_array<T, array_view_tag> &&other
    ) {
        if (this != &other) {
            this->m_data = other.m_data;
            this->m_size = other.m_size;
            this->m_offset = other.m_offset;
            this->m_stride = other.m_stride;
            other.m_data = NULL;
            other.m_size = 0;
            other.m_offset = 0;
            other.m_stride = 1;
        }
        return *this;
    }

    /// Compound assignment operator.

    /// Helper function: Apply a binary function element-wise.
    template <
        class Function, 
        class T1, class Tag1, class T2, class Tag2, 
        class R, class TagR
    >
    inline void __apply_binary_function(
        Function f, 
        const base_array<T1, Tag1> &lhs, const base_array<T2, Tag2> &rhs, 
        base_array<R, TagR> &out
    ) {
        __assert_equal_length(lhs.size(), rhs.size());
        for (size_t i = 0; i < lhs.size(); ++i) {
            out[i] = f(lhs[i], rhs[i]);
        }
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator+=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator-=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator*=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator/=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator%=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator&=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator|=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator^=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator<<=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator>>=(
        const base_array<T, array_view_tag> &rhs
    ) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator+=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator-=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator*=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator/=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator%=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator&=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator|=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator^=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator<<=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator>>=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    /// Helper function: Apply a binary function element-wise. Values are 
    /// broadcasted into an appropriate size.
    template <
        class Function, 
        class T1, class Tag1, class T2, 
        class R, class TagR
    >
    inline void __apply_binary_function(
        Function f, const base_array<T1, Tag1> &lhs, const T2 &val, 
        base_array<R, TagR> &out
    ) {
        for (size_t i = 0; i < lhs.size(); ++i) {
            out[i] = f(lhs[i], val);
        }
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator+=(const T &val) {
        __apply_binary_function(__plus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator-=(const T &val) {
        __apply_binary_function(__minus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator*=(const T &val) {
        __apply_binary_function(__multiplies(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator/=(const T &val) {
        __apply_binary_function(__divides(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator%=(const T &val) {
        __apply_binary_function(__modulus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator&=(const T &val) {
        __apply_binary_function(__bit_and(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator|=(const T &val) {
        __apply_binary_function(__bit_or(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator^=(const T &val) {
        __apply_binary_function(__bit_xor(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator<<=(const T &val) {
        __apply_binary_function(__left_shift(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_view_tag>& 
    base_array<T, array_view_tag>::operator>>=(const T &val) {
        __apply_binary_function(__right_shift(), *this, val, *this);
        return *this;
    }

    /// Public methods.

    /// Helper function: Apply an unary function to each element in the given 
    /// array.
    template <class Function, class T, class Tag, class R, class TagR>
    inline void __apply_unary_function(
        Function f, const base_array<T, Tag> &arr, base_array<R, TagR> &out
    ) {
        for (size_t i = 0; i < arr.size(); ++i) {
            out[i] = f(arr[i]);
        }
    }

    template <class T>
    void base_array<T, array_view_tag>::apply(T f(T)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    void base_array<T, array_view_tag>::apply(T f(const T&)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    template <class Function>
    void base_array<T, array_view_tag>::apply(Function f) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    size_t base_array<T, array_view_tag>::argmax() const {
        __range_argmax pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    size_t base_array<T, array_view_tag>::argmin() const {
        __range_argmin pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    template <class U>
    base_array< U, lazy_unary_tag<__identity, T, array_view_tag> > 
    base_array<T, array_view_tag>::astype() const {
        typedef lazy_unary_tag<__identity, T, array_view_tag> Closure;
        return base_array<U, Closure>(__identity(), *this);
    }

    template <class T>
    void base_array<T, array_view_tag>::clamp(const T &a_min, const T &a_max) {
        __apply_unary_function(__clamp<T>(a_min, a_max), *this, *this);
    }

    template <class T>
    base_array< T, lazy_unary_tag<__conjugate, T, array_view_tag> > 
    base_array<T, array_view_tag>::conj() const {
        typedef lazy_unary_tag<__conjugate, T, array_view_tag> Closure;
        return base_array<T, Closure>(__conjugate(), *this);
    }

    template <class T>
    base_array< 
        typename complex_traits<T>::value_type, 
        lazy_unary_tag<__imag_part, T, array_view_tag> 
    > 
    base_array<T, array_view_tag>::imag() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__imag_part, T, array_view_tag> Closure;
        return base_array<Rt, Closure>(__imag_part(), *this);
    }

    template <class T>
    T base_array<T, array_view_tag>::max() const {
        __range_max pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_view_tag>::mean() const {
        __range_mean pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_view_tag>::min() const {
        __range_min pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    void base_array<T, array_view_tag>::partition(size_t kth) {
        __assert_within_bounds(this->size(), kth);
        std::nth_element(this->begin(), this->begin() + kth, this->end());
    }

    template <class T>
    template <class Compare>
    void base_array<T, array_view_tag>::partition(size_t kth, Compare comp) {
        __assert_within_bounds(this->size(), kth);
        std::nth_element(this->begin(), this->begin() + kth, this->end(), comp);
    }

    template <class T>
    T base_array<T, array_view_tag>::prod() const {
        __range_prod pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< 
        typename complex_traits<T>::value_type, 
        lazy_unary_tag<__real_part, T, array_view_tag> 
    > 
    base_array<T, array_view_tag>::real() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__real_part, T, array_view_tag> Closure;
        return base_array<Rt, Closure>(__real_part(), *this);
    }

    template <class T>
    void base_array<T, array_view_tag>::reverse() {
        std::reverse(this->begin(), this->end());
    }

    template <class T>
    void base_array<T, array_view_tag>::sort() {
        std::sort(this->begin(), this->end());
    }

    template <class T>
    template <class Compare>
    void base_array<T, array_view_tag>::sort(Compare comp, bool stable) {
        if (stable) {
            std::stable_sort(this->begin(), this->end(), comp);
        }
        else {
            std::sort(this->begin(), this->end(), comp);
        }
    }

    template <class T>
    T base_array<T, array_view_tag>::stddev(size_t ddof) const {
        __range_stddev pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_view_tag>::sum() const {
        __range_sum pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_view_tag>::var(size_t ddof) const {
        __range_var pred(ddof);
        return pred(this->begin(), this->end());
    }
}

#endif // NUMCPP_ARRAY_VIEW_TCC_INCLUDED
