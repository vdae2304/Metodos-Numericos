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

 /** @file include/numcpp/matrix/matrix.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_TCC_INCLUDED
#define NUMCPP_MATRIX_TCC_INCLUDED

#include <algorithm>
#include <iterator>

namespace numcpp {
    /// Constructors.

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix() {
        this->m_data = NULL;
        this->m_shape1 = 0;
        this->m_shape2 = 0;
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(size_t m, size_t n) {
        this->m_data = new T[m * n];
        this->m_shape1 = m;
        this->m_shape2 = n;
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(size_t m, size_t n, const T &val) {
        this->m_data = new T[m * n];
        this->m_shape1 = m;
        this->m_shape2 = n;
        std::fill_n(this->m_data, this->m_shape1 * this->m_shape2, val);
    }

    template <class T>
    template <class InputIterator, typename>
    base_matrix<T, matrix_tag>::base_matrix(
        InputIterator first, InputIterator last, size_t n
    ) {
        size_t size = std::distance(first, last);
        this->m_shape1 = size / n;
        this->m_shape2 = n;
        if (size % n != 0) {
            ++this->m_shape1;
        }
        this->m_data = new T[this->m_shape1 * this->m_shape2];
        std::copy(first, last, this->m_data);
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(
        const base_matrix<T, matrix_tag> &other
    ) {
        this->m_shape1 = other.m_shape1;
        this->m_shape2 = other.m_shape2;
        this->m_data = new T[this->m_shape1 * this->m_shape2];
        std::copy(
            other.m_data, other.m_data + other.m_shape1*other.m_shape2, 
            this->m_data
        );
    }

    template <class T>
    template <class Tag>
    base_matrix<T, matrix_tag>::base_matrix(const base_matrix<T, Tag> &other) {
        this->m_shape1 = other.rows();
        this->m_shape2 = other.cols();
        this->m_data = new T[this->m_shape1 * this->m_shape2];
        std::copy(other.begin(true), other.end(true), this->m_data);
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(
        base_matrix<T, matrix_tag> &&other
    ) {
        if (this != &other) {
            this->m_data = other.m_data;
            this->m_shape1 = other.m_shape1;
            this->m_shape2 = other.m_shape2;
            other.m_data = NULL;
            other.m_shape1 = 0;
            other.m_shape2 = 0;
        }
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(
        std::initializer_list< std::initializer_list<T> > il
    ) {
        this->m_shape1 = il.size();
        this->m_shape2 = 0;
        for (const std::initializer_list<T> &row : il) {
            this->m_shape2 = std::max(this->m_shape2, row.size());
        }
        this->m_data = new T[this->m_shape1 * this->m_shape2];
        T* rowptr = this->m_data;
        for (const std::initializer_list<T> &row : il) {
            std::copy(row.begin(), row.end(), rowptr);
            rowptr += this->m_shape2;
        }
    }

    /// Destructor.

    template <class T>
    base_matrix<T, matrix_tag>::~base_matrix() {
        delete[] this->m_data;
    }

    /// Iterators.

    template <class T>
    inline typename base_matrix<T, matrix_tag>::iterator 
    base_matrix<T, matrix_tag>::begin() {
        return base_matrix_iterator<T, matrix_tag>(this, 0, true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator 
    base_matrix<T, matrix_tag>::begin() const {
        return base_matrix_const_iterator<T, matrix_tag>(this, 0, true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::iterator 
    base_matrix<T, matrix_tag>::begin(bool row_major) {
        return base_matrix_iterator<T, matrix_tag>(this, 0, row_major);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator 
    base_matrix<T, matrix_tag>::begin(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(this, 0, row_major);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::iterator 
    base_matrix<T, matrix_tag>::end() {
        return base_matrix_iterator<T, matrix_tag>(
            this, this->m_shape1 * this->m_shape2, true
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator 
    base_matrix<T, matrix_tag>::end() const {
        return base_matrix_const_iterator<T, matrix_tag>(
            this, this->m_shape1 * this->m_shape2, true
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::iterator 
    base_matrix<T, matrix_tag>::end(bool row_major) {
        return base_matrix_iterator<T, matrix_tag>(
            this, this->m_shape1 * this->m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator 
    base_matrix<T, matrix_tag>::end(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(
            this, this->m_shape1 * this->m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::reverse_iterator 
    base_matrix<T, matrix_tag>::rbegin() {
        return typename base_matrix<T, matrix_tag>::reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator 
    base_matrix<T, matrix_tag>::rbegin() const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::reverse_iterator 
    base_matrix<T, matrix_tag>::rbegin(bool row_major) {
        return typename base_matrix<T, matrix_tag>::reverse_iterator(
            this->end(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator 
    base_matrix<T, matrix_tag>::rbegin(bool row_major) const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->end(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::reverse_iterator 
    base_matrix<T, matrix_tag>::rend() {
        return typename base_matrix<T, matrix_tag>::reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator 
    base_matrix<T, matrix_tag>::rend() const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::reverse_iterator 
    base_matrix<T, matrix_tag>::rend(bool row_major) {
        return typename base_matrix<T, matrix_tag>::reverse_iterator(
            this->begin(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator 
    base_matrix<T, matrix_tag>::rend(bool row_major) const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->begin(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cbegin() const {
        return base_matrix_const_iterator<T, matrix_tag>(this, 0, true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cbegin(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(this, 0, row_major);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cend() const {
        return base_matrix_const_iterator<T, matrix_tag>(
            this, this->m_shape1 * this->m_shape2, true
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cend(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(
            this, this->m_shape1 * this->m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator
    base_matrix<T, matrix_tag>::crbegin() const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->cend()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator
    base_matrix<T, matrix_tag>::crbegin(bool row_major) const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->cend(row_major)
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator
    base_matrix<T, matrix_tag>::crend() const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->cbegin()
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_reverse_iterator
    base_matrix<T, matrix_tag>::crend(bool row_major) const {
        return typename base_matrix<T, matrix_tag>::const_reverse_iterator(
            this->cbegin(row_major)
        );
    }

    /// Matrix indexing.

    template <class T>
    inline T& base_matrix<T, matrix_tag>::operator()(size_t i, size_t j) {
        __assert_within_bounds(this->m_shape1, this->m_shape2, i, j);
        return this->m_data[i*this->m_shape2 + j];
    }

    template <class T>
    inline const T& 
    base_matrix<T, matrix_tag>::operator()(size_t i, size_t j) const {
        __assert_within_bounds(this->m_shape1, this->m_shape2, i, j);
        return this->m_data[i*this->m_shape2 + j];
    }

    template <class T>
    base_matrix<T, matrix_view_tag> 
    base_matrix<T, matrix_tag>::operator()(slice slc1, slice slc2) {
        if (slc1.size() > 0 && slc2.size() > 0) {
            __assert_within_bounds(
                this->m_shape1, this->m_shape2, 
                slc1[slc1.size() - 1], slc2[slc2.size() - 1]
            );
        }
        return base_matrix<T, matrix_view_tag>(
            slc1.size(), slc2.size(), this->m_data, 
            this->m_shape2,
            slc1.start(), slc1.stride(),
            slc2.start(), slc2.stride()
        );
    }

    template <class T>
    const base_matrix<T, matrix_view_tag> 
    base_matrix<T, matrix_tag>::operator()(slice slc1, slice slc2) const {
        if (slc1.size() > 0 && slc2.size() > 0) {
            __assert_within_bounds(
                this->m_shape1, this->m_shape2, 
                slc1[slc1.size() - 1], slc2[slc2.size() - 1]
            );
        }
        return base_matrix<T, matrix_view_tag>(
            slc1.size(), slc2.size(), this->m_data, 
            this->m_shape2,
            slc1.start(), slc1.stride(),
            slc2.start(), slc2.stride()
        );
    }

    template <class T>
    base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::operator()(size_t i, slice slc) {
        if (slc.size() > 0) {
            __assert_within_bounds(
                this->m_shape1, this->m_shape2, i, slc[slc.size() - 1]
            );
        }
        return base_array<T, array_view_tag>(
            slc.size(), this->m_data, 
            i*this->m_shape2 + slc.start(), slc.stride()
        );
    }

    template <class T>
    const base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::operator()(size_t i, slice slc) const {
        if (slc.size() > 0) {
            __assert_within_bounds(
                this->m_shape1, this->m_shape2, i, slc[slc.size() - 1]
            );
        }
        return base_array<T, array_view_tag>(
            slc.size(), this->m_data, 
            i*this->m_shape2 + slc.start(), slc.stride()
        );
    }

    template <class T>
    base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::operator()(slice slc, size_t j) {
        if (slc.size() > 0) {
            __assert_within_bounds(
                this->m_shape1, this->m_shape2, slc[slc.size() - 1], j
            );
        }
        return base_array<T, array_view_tag>(
            slc.size(), this->m_data, 
            slc.start()*this->m_shape2 + j, this->m_shape2*slc.stride()
        );
    }

    template <class T>
    const base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::operator()(slice slc, size_t j) const {
        if (slc.size() > 0) {
            __assert_within_bounds(
                this->m_shape1, this->m_shape2, slc[slc.size() - 1], j
            );
        }
        return base_array<T, array_view_tag>(
            slc.size(), this->m_data, 
            slc.start()*this->m_shape2 + j, this->m_shape2*slc.stride()
        );
    }

    template <class T>
    template <class Tag>
    base_array<T, index_view_tag> 
    base_matrix<T, matrix_tag>::operator[](
        const base_array<std::pair<size_t, size_t>, Tag> &index
    ) {
        size_t *m_index = new size_t[index.size()];
        for (size_t k = 0; k < index.size(); ++k) {
            size_t i = index[k].first, j = index[k].second;
            m_index[k] = i*this->cols() + j;
            __assert_within_bounds(this->rows(), this->cols(), i, j);
        }
        return base_array<T, index_view_tag>(
            index.size(), this->data(), m_index, -1
        );
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag> base_matrix<T, matrix_tag>::operator[](
        const base_array<std::pair<size_t, size_t>, Tag> &index
    ) const {
        base_array<T, array_tag> subarray(index.size());
        for (size_t k = 0; k < index.size(); ++k) {
            subarray[k] = (*this)(index[k].first, index[k].second);
        }
        return subarray;
    }

    template <class T>
    template <class Tag>
    base_array<T, index_view_tag> 
    base_matrix<T, matrix_tag>::operator[](const base_matrix<bool, Tag> &mask) {
        __range_count_nonzero pred;
        size_t size = pred(mask.begin(), mask.end());
        size_t *m_index = new size_t[size];
        size_t n = 0;
        for (size_t i = 0; i < mask.rows(); ++i) {
            for (size_t j = 0; j < mask.cols(); ++j) {
                if (mask(i, j)) {
                    __assert_within_bounds(this->rows(), this->cols(), i, j);
                    m_index[n++] = i*this->cols() + j;
                }
            }
        }
        return base_array<T, index_view_tag>(size, this->data(), m_index, -1);
    }
    
    template <class T>
    template <class Tag>
    base_array<T, array_tag> 
    base_matrix<T, matrix_tag>::operator[](const base_matrix<bool, Tag> &mask) 
    const {
        __range_count_nonzero pred;
        base_array<T, array_tag> subarray(pred(mask.begin(), mask.end()));
        size_t n = 0;
        for (size_t i = 0; i < mask.rows(); ++i) {
            for (size_t j = 0; j < mask.cols(); ++j) {
                if (mask(i, j)) {
                    subarray[n++] = (*this)(i, j);
                }
            }
        }
        return subarray;
    }
    
    template <class T>
    inline size_t base_matrix<T, matrix_tag>::rows() const {
        return this->m_shape1;
    }

    template <class T>
    inline size_t base_matrix<T, matrix_tag>::cols() const {
        return this->m_shape2;
    }

    template <class T>
    inline size_t base_matrix<T, matrix_tag>::size() const {
        return this->m_shape1 * this->m_shape2;
    }

    template <class T>
    inline void base_matrix<T, matrix_tag>::resize(size_t m, size_t n) {
        if (this->m_shape1 * this->m_shape2 != m * n) {
            delete[] this->m_data;
            this->m_data = new T[m * n];
        }
        this->m_shape1 = m;
        this->m_shape2 = n;
    }

    template <class T>
    inline bool base_matrix<T, matrix_tag>::empty() const {
        return (this->m_shape1 == 0 || this->m_shape2 == 0);
    }

    template <class T>
    inline T* base_matrix<T, matrix_tag>::data() {
        return this->m_data;
    }

    template <class T>
    inline const T* base_matrix<T, matrix_tag>::data() const {
        return this->m_data;
    }

    /// Operator overloading.
    
    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator=(
        const base_matrix<T, matrix_tag> &other
    ) {
        this->resize(other.m_shape1, other.m_shape2);
        std::copy(
            other.m_data, other.m_data + other.m_shape1*other.m_shape2, 
            this->m_data
        );
        return *this;
    }
    
    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator=(const base_matrix<T, Tag> &other) {
        this->resize(other.rows(), other.cols());
        std::copy(other.begin(true), other.end(true), this->m_data);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator=(const T &val) {
        std::fill_n(this->m_data, this->m_shape1 * this->m_shape2, val);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator=(base_matrix<T, matrix_tag> &&other) {
        if (this != &other) {
            delete[] this->m_data;
            this->m_data = other.m_data;
            this->m_shape1 = other.m_shape1;
            this->m_shape2 = other.m_shape2;
            other.m_data = NULL;
            other.m_shape1 = 0;
            other.m_shape2 = 0;
        }
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator=(
        std::initializer_list< std::initializer_list<T> > il
    ) {
        size_t m = il.size(), n = 0;
        for (const std::initializer_list<T> &row : il) {
            n = std::max(n, row.size());
        }
        this->resize(m, n);
        T* rowptr = this->m_data;
        for (const std::initializer_list<T> &row : il) {
            std::copy(row.begin(), row.end(), rowptr);
            rowptr += this->m_shape2;
        }
        return *this;
    }

    /// Compound assignment operator.

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator+=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator-=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator*=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator/=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator%=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator&=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator|=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator^=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator<<=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator>>=(
        const base_matrix<T, matrix_tag> &rhs
    ) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator+=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator-=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator*=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator/=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator%=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator&=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator|=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator^=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator<<=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator>>=(const base_matrix<T, Tag> &rhs) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator+=(const T &val) {
        __apply_binary_function(__plus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator-=(const T &val) {
        __apply_binary_function(__minus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator*=(const T &val) {
        __apply_binary_function(__multiplies(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator/=(const T &val) {
        __apply_binary_function(__divides(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator%=(const T &val) {
        __apply_binary_function(__modulus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator&=(const T &val) {
        __apply_binary_function(__bit_and(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator|=(const T &val) {
        __apply_binary_function(__bit_or(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator^=(const T &val) {
        __apply_binary_function(__bit_xor(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator<<=(const T &val) {
        __apply_binary_function(__left_shift(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>& 
    base_matrix<T, matrix_tag>::operator>>=(const T &val) {
        __apply_binary_function(__right_shift(), *this, val, *this);
        return *this;
    }

    /// Public methods.

    template <class T>
    void base_matrix<T, matrix_tag>::apply(T f(T)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::apply(T f(const T&)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    template <class Function>
    void base_matrix<T, matrix_tag>::apply(Function f) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::argmax(size_t &i, size_t &j) const {
        __range_argmax pred;
        base_matrix_const_iterator<T, matrix_tag> it = 
            this->begin() + pred(this->begin(), this->end());
        i = it.row();
        j = it.col();
    }

    template <class T>
    base_array< size_t, lazy_axis_tag<__range_argmax, T, matrix_tag> >
    base_matrix<T, matrix_tag>::argmax(bool rowwise) const {
        typedef lazy_axis_tag<__range_argmax, T, matrix_tag> Closure;
        return base_array<size_t, Closure>(__range_argmax(), *this, rowwise);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::argmin(size_t &i, size_t &j) const {
        __range_argmin pred;
        base_matrix_const_iterator<T, matrix_tag> it = 
            this->begin() + pred(this->begin(), this->end());
        i = it.row();
        j = it.col();
    }

    template <class T>
    base_array< size_t, lazy_axis_tag<__range_argmin, T, matrix_tag> >
    base_matrix<T, matrix_tag>::argmin(bool rowwise) const {
        typedef lazy_axis_tag<__range_argmin, T, matrix_tag> Closure;
        return base_array<size_t, Closure>(__range_argmin(), *this, rowwise);
    }

    template <class T>
    template <class U>
    base_matrix< U, lazy_unary_tag<__identity, T, matrix_tag> > 
    base_matrix<T, matrix_tag>::astype() const {
        typedef lazy_unary_tag<__identity, T, matrix_tag> Closure;
        return base_matrix<U, Closure>(__identity(), *this);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::clamp(const T &a_min, const T &a_max) {
        __apply_unary_function(__clamp<T>(a_min, a_max), *this, *this);
    }

    template <class T>
    base_matrix< T, lazy_unary_tag<__conjugate, T, matrix_tag> > 
    base_matrix<T, matrix_tag>::conj() const {
        typedef lazy_unary_tag<__conjugate, T, matrix_tag> Closure;
        return base_matrix<T, Closure>(__conjugate(), *this);
    }

    template <class T>
    base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::diagonal(ptrdiff_t offset) {
        size_t size = 0, start, stride = this->cols() + 1;
        if (offset >= 0) {
            if (this->cols() > (size_t)offset) {
                size = std::min(this->rows(), this->cols() - offset);
            }
            start = offset;
        }
        else {
            if (this->rows() > (size_t)-offset) {
                size = std::min(this->rows() + offset, this->cols());
            }
            start = -offset * this->cols();
        }
        return base_array<T, array_view_tag>(size, this->m_data, start, stride);
    }

    template <class T>
    const base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::diagonal(ptrdiff_t offset) const {
        size_t size = 0, start, stride = this->cols() + 1;
        if (offset >= 0) {
            if (this->cols() > (size_t)offset) {
                size = std::min(this->rows(), this->cols() - offset);
            }
            start = offset;
        }
        else {
            if (this->rows() > (size_t)-offset) {
                size = std::min(this->rows() + offset, this->cols());
            }
            start = -offset * this->cols();
        }
        return base_array<T, array_view_tag>(size, this->m_data, start, stride);
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag> 
    base_matrix<T, matrix_tag>::dot(const base_array<T, Tag> &rhs) const {
        __assert_matmul_shapes(this->rows(), this->cols(), rhs.size(), 1);
        base_array<T, array_tag> out(this->rows(), T(0));
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->cols(); ++j) {
                out[i] += (*this)(i, j) * rhs[j];
            }
        }
        return out;
    }

    template <class T>
    template <class Tag>
    base_matrix<T, matrix_tag> 
    base_matrix<T, matrix_tag>::dot(const base_matrix<T, Tag> &rhs) const {
        __assert_matmul_shapes(this->rows(),this->cols(),rhs.rows(),rhs.cols());
        base_matrix<T, matrix_tag> out(this->rows(), rhs.cols(), T(0));
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < rhs.cols(); ++j) {
                for (size_t k = 0; k < this->cols(); ++k) {
                    out(i, j) += (*this)(i, k) * rhs(k, j);
                }
            }
        }
        return out;
    }

    template <class T>
    base_array<T, array_view_tag> base_matrix<T, matrix_tag>::flatten() {
        return base_array<T, array_view_tag>(this->size(), this->m_data);
    }

    template <class T>
    const base_array<T, array_view_tag> 
    base_matrix<T, matrix_tag>::flatten() const {
        return base_array<T, array_view_tag>(this->size(), this->m_data);
    }

    template <class T>
    base_matrix< 
        typename complex_traits<T>::value_type, 
        lazy_unary_tag<__imag_part, T, matrix_tag> 
    > 
    base_matrix<T, matrix_tag>::imag() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__imag_part, T, matrix_tag> Closure;
        return base_matrix<Rt, Closure>(__imag_part(), *this);
    }

    template <class T>
    T base_matrix<T, matrix_tag>::max() const {
        __range_max pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_max, T, matrix_tag> >
    base_matrix<T, matrix_tag>::max(bool rowwise) const {
        typedef lazy_axis_tag<__range_max, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_max(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_tag>::mean() const {
        __range_mean pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_mean, T, matrix_tag> >
    base_matrix<T, matrix_tag>::mean(bool rowwise) const {
        typedef lazy_axis_tag<__range_mean, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_mean(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_tag>::min() const {
        __range_min pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_min, T, matrix_tag> > 
    base_matrix<T, matrix_tag>::min(bool rowwise) const {
        typedef lazy_axis_tag<__range_min, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_min(), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_tag>::prod() const {
        __range_prod pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_prod, T, matrix_tag> >
    base_matrix<T, matrix_tag>::prod(bool rowwise) const {
        typedef lazy_axis_tag<__range_prod, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_prod(), *this, rowwise);
    }

    template <class T>
    base_matrix< 
        typename complex_traits<T>::value_type, 
        lazy_unary_tag<__real_part, T, matrix_tag> 
    > 
    base_matrix<T, matrix_tag>::real() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__real_part, T, matrix_tag> Closure;
        return base_matrix<Rt, Closure>(__real_part(), *this);
    }

    template <class T>
    T base_matrix<T, matrix_tag>::stddev(size_t ddof) const {
        __range_stddev pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_stddev, T, matrix_tag> >
    base_matrix<T, matrix_tag>::stddev(size_t ddof, bool rowwise) const {
        typedef lazy_axis_tag<__range_stddev, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_stddev(ddof), *this, rowwise);
    }

    template <class T>
    T base_matrix<T, matrix_tag>::sum() const {
        __range_sum pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_sum, T, matrix_tag> >
    base_matrix<T, matrix_tag>::sum(bool rowwise) const {
        typedef lazy_axis_tag<__range_sum, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_sum(), *this, rowwise);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::swap(base_matrix<T, matrix_tag> &other) {
        std::swap(this->m_data, other.m_data);
        std::swap(this->m_shape1, other.m_shape1);
        std::swap(this->m_shape2, other.m_shape2);
    }
    
    template <class T>
    base_matrix<T, matrix_view_tag> base_matrix<T, matrix_tag>::t() {
        return base_matrix<T, matrix_view_tag>(
            this->cols(), this->rows(), this->m_data, false
        );
    }

    template <class T>
    const base_matrix<T, matrix_view_tag> 
    base_matrix<T, matrix_tag>::t() const {
        return base_matrix<T, matrix_view_tag>(
            this->cols(), this->rows(), this->m_data, false
        );
    }

    template <class T>
    T base_matrix<T, matrix_tag>::var(size_t ddof) const {
        __range_var pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array< T, lazy_axis_tag<__range_var, T, matrix_tag> >
    base_matrix<T, matrix_tag>::var(size_t ddof, bool rowwise) const {
        typedef lazy_axis_tag<__range_var, T, matrix_tag> Closure;
        return base_array<T, Closure>(__range_var(ddof), *this, rowwise);
    }

    template <class T>
    base_matrix<T, matrix_view_tag> base_matrix<T, matrix_tag>::view() {
        return base_matrix<T, matrix_view_tag>(
            this->rows(), this->cols(), this->data()
        );
    }
}

#endif // NUMCPP_MATRIX_TCC_INCLUDED
