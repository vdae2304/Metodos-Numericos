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

 /** @file include/numcpp/matrix/matrix_view.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_VIEW_TCC_INCLUDED
#define NUMCPP_MATRIX_VIEW_TCC_INCLUDED

#include <algorithm>

namespace numcpp {
    /// Constructors.

    template <class T>
    base_matrix<T, matrix_view_tag>::base_matrix() {
        this->m_data = NULL;
        this->m_shape1 = 0;
        this->m_shape2 = 0;
        this->m_tda = 0;
        this->m_offset1 = 0;
        this->m_stride1 = 1;
        this->m_offset2 = 0;
        this->m_stride2 = 1;
        this->m_order = true;
    }

    template <class T>
    base_matrix<T, matrix_view_tag>::base_matrix(
        size_t m, size_t n, T *data, bool row_major
    ) {
        this->m_data = data;
        this->m_shape1 = m;
        this->m_shape2 = n;
        this->m_tda = row_major ? n : m;
        this->m_offset1 = 0;
        this->m_stride1 = 1;
        this->m_offset2 = 0;
        this->m_stride2 = 1;
        this->m_order = row_major;
    }

    template <class T>
    base_matrix<T, matrix_view_tag>::base_matrix(
        size_t m, size_t n, T *data, size_t tda,
        size_t offset1, size_t stride1, 
        size_t offset2, size_t stride2, 
        bool row_major
    ) {
        this->m_data = data;
        this->m_shape1 = m;
        this->m_shape2 = n;
        this->m_tda = tda;
        this->m_offset1 = offset1;
        this->m_stride1 = stride1;
        this->m_offset2 = offset2;
        this->m_stride2 = stride2;
        this->m_order = row_major;
    }

    template <class T>
    base_matrix<T, matrix_view_tag>::base_matrix(
        const base_matrix<T, matrix_view_tag> &other
    ) {
        this->m_data = other.m_data;
        this->m_shape1 = other.m_shape1;
        this->m_shape2 = other.m_shape2;
        this->m_tda = other.m_tda;
        this->m_offset1 = other.m_offset1;
        this->m_stride1 = other.m_stride1;
        this->m_offset2 = other.m_offset2;
        this->m_stride2 = other.m_stride2;
        this->m_order = other.m_order;
    }

    template <class T>
    base_matrix<T, matrix_view_tag>::base_matrix(
        base_matrix<T, matrix_view_tag> &&other
    ) {
        this->m_data = other.m_data;
        this->m_shape1 = other.m_shape1;
        this->m_shape2 = other.m_shape2;
        this->m_tda = other.m_tda;
        this->m_offset1 = other.m_offset1;
        this->m_stride1 = other.m_stride1;
        this->m_offset2 = other.m_offset2;
        this->m_stride2 = other.m_stride2;
        this->m_order = other.m_order;
        other.m_data = NULL;
        other.m_shape1 = 0;
        other.m_shape2 = 0;
        other.m_tda = 0;
        other.m_offset1 = 0;
        other.m_stride1 = 1;
        other.m_offset2 = 0;
        other.m_stride2 = 1;
        other.m_order = true;
    }

    /// Destructor.

    template <class T>
    base_matrix<T, matrix_view_tag>::~base_matrix() {}

    /// Iterators.

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::iterator 
    base_matrix<T, matrix_view_tag>::begin() {
        return base_matrix_iterator<T, matrix_view_tag>(this, 0, this->m_order);
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator 
    base_matrix<T, matrix_view_tag>::begin() const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, 0, this->m_order
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::iterator 
    base_matrix<T, matrix_view_tag>::begin(bool row_major) {
        return base_matrix_iterator<T, matrix_view_tag>(this, 0, row_major);
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator 
    base_matrix<T, matrix_view_tag>::begin(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, 0, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::iterator 
    base_matrix<T, matrix_view_tag>::end() {
        return base_matrix_iterator<T, matrix_view_tag>(
            this, this->m_shape1 * this->m_shape2, this->m_order
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator 
    base_matrix<T, matrix_view_tag>::end() const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, this->m_shape1 * this->m_shape2, this->m_order
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::iterator 
    base_matrix<T, matrix_view_tag>::end(bool row_major) {
        return base_matrix_iterator<T, matrix_view_tag>(
            this, this->m_shape1 * this->m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator 
    base_matrix<T, matrix_view_tag>::end(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, this->m_shape1 * this->m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::reverse_iterator 
    base_matrix<T, matrix_view_tag>::rbegin() {
        return typename base_matrix<T, matrix_view_tag>::reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator 
    base_matrix<T, matrix_view_tag>::rbegin() const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::reverse_iterator 
    base_matrix<T, matrix_view_tag>::rbegin(bool row_major) {
        return typename base_matrix<T, matrix_view_tag>::reverse_iterator(
            this->end(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator 
    base_matrix<T, matrix_view_tag>::rbegin(bool row_major) const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->end(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::reverse_iterator 
    base_matrix<T, matrix_view_tag>::rend() {
        return typename base_matrix<T, matrix_view_tag>::reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator 
    base_matrix<T, matrix_view_tag>::rend() const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::reverse_iterator 
    base_matrix<T, matrix_view_tag>::rend(bool row_major) {
        return typename base_matrix<T, matrix_view_tag>::reverse_iterator(
            this->begin(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator 
    base_matrix<T, matrix_view_tag>::rend(bool row_major) const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->begin(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator
    base_matrix<T, matrix_view_tag>::cbegin() const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, 0, this->m_order
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator
    base_matrix<T, matrix_view_tag>::cbegin(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, 0, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator
    base_matrix<T, matrix_view_tag>::cend() const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, this->m_shape1 * this->m_shape2, this->m_order
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_iterator
    base_matrix<T, matrix_view_tag>::cend(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_view_tag>(
            this, this->m_shape1 * this->m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator
    base_matrix<T, matrix_view_tag>::crbegin() const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->cend()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator
    base_matrix<T, matrix_view_tag>::crbegin(bool row_major) const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->cend(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator
    base_matrix<T, matrix_view_tag>::crend() const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->cbegin()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_view_tag>::const_reverse_iterator
    base_matrix<T, matrix_view_tag>::crend(bool row_major) const {
        return typename base_matrix<T, matrix_view_tag>::const_reverse_iterator(
            this->cbegin(row_major)
        );
    }

    /// Matrix indexing.

    template <class T>
    inline T& base_matrix<T, matrix_view_tag>::operator()(size_t i, size_t j) {
        __assert_within_bounds(this->m_shape1, this->m_shape2, i, j);
        i = this->m_offset1 + i*this->m_stride1;
        j = this->m_offset2 + j*this->m_stride2;
        if (this->m_order) {
            return this->m_data[i*this->m_tda + j];
        }
        else {
            return this->m_data[j*this->m_tda + i];
        }
    }

    template <class T>
    inline const T& 
    base_matrix<T, matrix_view_tag>::operator()(size_t i, size_t j) const {
        __assert_within_bounds(this->m_shape1, this->m_shape2, i, j);
        i = this->m_offset1 + i*this->m_stride1;
        j = this->m_offset2 + j*this->m_stride2;
        if (this->m_order) {
            return this->m_data[i*this->m_tda + j];
        }
        else {
            return this->m_data[j*this->m_tda + i];
        }
    }

    template <class T>
    inline size_t base_matrix<T, matrix_view_tag>::rows() const {
        return this->m_shape1;
    }

    template <class T>
    inline size_t base_matrix<T, matrix_view_tag>::cols() const {
        return this->m_shape2;
    }

    template <class T>
    inline size_t base_matrix<T, matrix_view_tag>::size() const {
        return this->m_shape1 * this->m_shape2;
    }

    template <class T>
    inline bool base_matrix<T, matrix_view_tag>::empty() const {
        return (this->m_shape1 == 0 || this->m_shape2 == 0);
    }

    template <class T>
    inline T* base_matrix<T, matrix_view_tag>::data() {
        return this->m_data;
    }

    template <class T>
    inline const T* base_matrix<T, matrix_view_tag>::data() const {
        return this->m_data;
    }

    template <class T>
    inline bool base_matrix<T, matrix_view_tag>::rowmajor() const {
        return this->m_order;
    }

    template <class T>
    inline bool base_matrix<T, matrix_view_tag>::colmajor() const {
        return !this->m_order;
    }

    /// Assignment operator.
    
    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator=(
        const base_matrix<T, matrix_view_tag> &other
    ) {
        __assert_equal_shape(
            this->rows(), this->cols(), other.rows(), other.cols()
        );
        std::copy(
            other.begin(this->rowmajor()), other.end(this->rowmajor()), 
            this->begin()
        );
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator=(
        const base_matrix<T, Tag> &other
    ) {
        __assert_equal_shape(
            this->rows(), this->cols(), other.rows(), other.cols()
        );
        std::copy(
            other.begin(this->rowmajor()), other.end(this->rowmajor()), 
            this->begin()
        );
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator=(const T &val) {
        std::fill_n(this->begin(), this->size(), val);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator=(
        base_matrix<T, matrix_view_tag> &&other
    ) {
        if (this != &other) {
            this->m_data = other.m_data;
            this->m_shape1 = other.m_shape1;
            this->m_shape2 = other.m_shape2;
            this->m_tda = other.m_tda;
            this->m_offset1 = other.m_offset1;
            this->m_stride1 = other.m_stride1;
            this->m_offset2 = other.m_offset2;
            this->m_stride2 = other.m_stride2;
            this->m_order = other.m_order;
            other.m_data = NULL;
            other.m_shape1 = 0;
            other.m_shape2 = 0;
            other.m_tda = 0;
            other.m_offset1 = 0;
            other.m_stride1 = 1;
            other.m_offset2 = 0;
            other.m_stride2 = 1;
            other.m_order = true;
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
        const base_matrix<T1, Tag1> &lhs, const base_matrix<T2, Tag2> &rhs, 
        base_matrix<R, TagR> &out
    ) {
        __assert_equal_shape(lhs.rows(), lhs.cols(), rhs.rows(), rhs.cols());
        for (size_t i = 0; i < lhs.rows(); ++i) {
            for (size_t j = 0; j < lhs.cols(); ++j) {
                out(i, j) = f(lhs(i, j), rhs(i, j));
            }
        }
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator+=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator-=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator*=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator/=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator%=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator&=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator|=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator^=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator<<=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator>>=(
        const base_matrix<T, matrix_view_tag> &rhs
    ) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator+=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator-=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator*=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator/=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator%=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator&=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator|=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator^=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator<<=(
        const base_matrix<T, Tag> &rhs
    ) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator>>=(
        const base_matrix<T, Tag> &rhs
    ) {
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
        Function f, const base_matrix<T1, Tag1> &lhs, const T2 &val, 
        base_matrix<R, TagR> &out
    ) {
        for (size_t i = 0; i < lhs.rows(); ++i) {
            for (size_t j = 0; j < lhs.cols(); ++j) {
                out(i, j) = f(lhs(i, j), val);
            }
        }
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator+=(const T &val) {
        __apply_binary_function(__plus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator-=(const T &val) {
        __apply_binary_function(__minus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator*=(const T &val) {
        __apply_binary_function(__multiplies(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator/=(const T &val) {
        __apply_binary_function(__divides(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator%=(const T &val) {
        __apply_binary_function(__modulus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator&=(const T &val) {
        __apply_binary_function(__bit_and(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator|=(const T &val) {
        __apply_binary_function(__bit_or(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator^=(const T &val) {
        __apply_binary_function(__bit_xor(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator<<=(const T &val) {
        __apply_binary_function(__left_shift(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_view_tag>& 
    base_matrix<T, matrix_view_tag>::operator>>=(const T &val) {
        __apply_binary_function(__right_shift(), *this, val, *this);
        return *this;
    }

    /// Public methods.

    /// Helper function: Apply an unary function to each element in the given 
    /// matrix.
    template <class Function, class T, class Tag, class R, class TagR>
    void __apply_unary_function(
        Function f, const base_matrix<T, Tag> &mat, base_matrix<R, TagR> &out
    ) {
        for (size_t i = 0; i < mat.rows(); ++i) {
            for (size_t j = 0; j < mat.cols(); ++j) {
                out(i, j) = f(mat(i, j));
            }
        }
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::apply(T f(T)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::apply(T f(const T&)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    template <class Function>
    void base_matrix<T, matrix_view_tag>::apply(Function f) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::argmax(size_t &i, size_t &j) const {
        __range_argmax pred;
        base_matrix_const_iterator<T, matrix_view_tag> it = 
            this->begin() + pred(this->begin(), this->end());
        i = it.row();
        j = it.col();
    }

    template <class T>
    base_array< size_t, lazy_axis_tag<__range_argmax, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::argmax(bool rowwise) const {
        typedef lazy_axis_tag<__range_argmax, T, matrix_view_tag> Closure;
        return base_array<size_t, Closure>(__range_argmax(), *this, rowwise);
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::argmin(size_t &i, size_t &j) const {
        __range_argmin pred;
        base_matrix_const_iterator<T, matrix_view_tag> it = 
            this->begin() + pred(this->begin(), this->end());
        i = it.row();
        j = it.col();
    }

    template <class T>
    base_array< size_t, lazy_axis_tag<__range_argmin, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::argmin(bool rowwise) const {
        typedef lazy_axis_tag<__range_argmin, T, matrix_view_tag> Closure;
        return base_array<size_t, Closure>(__range_argmin(), *this, rowwise);
    }

    template <class T>
    template <class U>
    base_matrix< U, lazy_unary_tag<__identity, T, matrix_view_tag> > 
    base_matrix<T, matrix_view_tag>::astype() const {
        typedef lazy_unary_tag<__identity, T, matrix_view_tag> Closure;
        return base_matrix<U, Closure>(__identity(), *this);
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::clamp(
        const T &a_min, const T &a_max
    ) {
        __apply_unary_function(__clamp<T>(a_min, a_max), *this, *this);
    }

    template <class T>
    base_matrix< T, lazy_unary_tag<__conjugate, T, matrix_view_tag> > 
    base_matrix<T, matrix_view_tag>::conj() const {
        typedef lazy_unary_tag<__conjugate, T, matrix_view_tag> Closure;
        return base_matrix<T, Closure>(__conjugate(), *this);
    }

    template <class T>
    base_matrix< 
        typename complex_traits<T>::value_type, 
        lazy_unary_tag<__imag_part, T, matrix_view_tag> 
    > 
    base_matrix<T, matrix_view_tag>::imag() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__imag_part, T, matrix_view_tag> Closure;
        return base_matrix<Rt, Closure>(__imag_part(), *this);
    }

    template <class T>
    template <class Tag>
    void base_matrix<T, matrix_view_tag>::imag(
        const base_matrix<typename complex_traits<T>::value_type, Tag> &mat
    ) {
        __assert_equal_shape(this->rows(),this->cols(), mat.rows(),mat.cols());
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->cols(); ++j) {
                (*this)(i, j).imag(mat(i, j));
            }
        }
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::max() const {
        __range_max pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_max, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::max(bool rowwise) const {
        typedef lazy_axis_tag<__range_max, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_max(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::mean() const {
        __range_mean pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_mean, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::mean(bool rowwise) const {
        typedef lazy_axis_tag<__range_mean, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_mean(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::min() const {
        __range_min pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::partition(size_t kth, bool rowwise) {
        size_t size = rowwise ? this->rows() : this->cols();
        size_t tda = rowwise ? this->cols() : this->rows();
        __assert_within_bounds(tda, kth);
        for (size_t i = 0; i < size; ++i) {
            std::nth_element(
                this->begin(rowwise) + i*tda, 
                this->begin(rowwise) + i*tda + kth, 
                this->begin(rowwise) + (i + 1)*tda
            );
        }
    }

    template <class T>
    template <class Compare>
    void base_matrix<T, matrix_view_tag>::partition(
        size_t kth, bool rowwise, Compare comp
    ) {
        size_t size = rowwise ? this->rows() : this->cols();
        size_t tda = rowwise ? this->cols() : this->rows();
        __assert_within_bounds(tda, kth);
        for (size_t i = 0; i < size; ++i) {
            std::nth_element(
                this->begin(rowwise) + i*tda, 
                this->begin(rowwise) + i*tda + kth, 
                this->begin(rowwise) + (i + 1)*tda, 
                comp
            );
        }
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_min, T, matrix_view_tag> > 
    base_matrix<T, matrix_view_tag>::min(bool rowwise) const {
        typedef lazy_axis_tag<__range_min, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_min(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::prod() const {
        __range_prod pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_prod, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::prod(bool rowwise) const {
        typedef lazy_axis_tag<__range_prod, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_prod(), *this, rowwise);
    }

    template <class T>
    base_matrix< 
        typename complex_traits<T>::value_type, 
        lazy_unary_tag<__real_part, T, matrix_view_tag> 
    > 
    base_matrix<T, matrix_view_tag>::real() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__real_part, T, matrix_view_tag> Closure;
        return base_matrix<Rt, Closure>(__real_part(), *this);
    }

    template <class T>
    template <class Tag>
    void base_matrix<T, matrix_view_tag>::real(
        const base_matrix<typename complex_traits<T>::value_type, Tag> &mat
    ) {
        __assert_equal_shape(this->rows(),this->cols(), mat.rows(),mat.cols());
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->cols(); ++j) {
                (*this)(i, j).real(mat(i, j));
            }
        }
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::reverse(bool rowwise) {
        size_t size = rowwise ? this->rows() : this->cols();
        size_t tda = rowwise ? this->cols() : this->rows();
        for (size_t i = 0; i < size; ++i) {
            std::reverse(
                this->begin(rowwise) + i*tda, 
                this->begin(rowwise) + (i + 1)*tda
            );
        }
    }

    template <class T>
    void base_matrix<T, matrix_view_tag>::sort(bool rowwise) {
        size_t size = rowwise ? this->rows() : this->cols();
        size_t tda = rowwise ? this->cols() : this->rows();
        for (size_t i = 0; i < size; ++i) {
            std::sort(
                this->begin(rowwise) + i*tda, 
                this->begin(rowwise) + (i + 1)*tda
            );
        }
    }

    template <class T>
    template <class Compare>
    void base_matrix<T, matrix_view_tag>::sort(
        bool rowwise, Compare comp, bool stable
    ) {
        size_t size = rowwise ? this->rows() : this->cols();
        size_t tda = rowwise ? this->cols() : this->rows();
        for (size_t i = 0; i < size; ++i) {
            if (stable) {
                std::stable_sort(
                    this->begin(rowwise) + i*tda, 
                    this->begin(rowwise) + (i + 1)*tda, 
                    comp
                );
            }
            else {
                std::sort(
                    this->begin(rowwise) + i*tda, 
                    this->begin(rowwise) + (i + 1)*tda, 
                    comp
                );
            }
        }
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::stddev(size_t ddof) const {
        __range_stddev pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_stddev, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::stddev(size_t ddof, bool rowwise) const {
        typedef lazy_axis_tag<__range_stddev, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_stddev(ddof), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::sum() const {
        __range_sum pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_sum, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::sum(bool rowwise) const {
        typedef lazy_axis_tag<__range_sum, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_sum(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_view_tag>::var(size_t ddof) const {
        __range_var pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_var, T, matrix_view_tag> >
    base_matrix<T, matrix_view_tag>::var(size_t ddof, bool rowwise) const {
        typedef lazy_axis_tag<__range_var, T, matrix_view_tag> Closure;
        return base_array<T, Closure>(__range_var(ddof), *this, rowwise);
    }
}

#endif // NUMCPP_MATRIX_VIEW_TCC_INCLUDED
