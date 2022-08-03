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
        m_data = NULL;
        m_shape1 = 0;
        m_shape2 = 0;
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(size_t m, size_t n) {
        m_data = new T[m * n];
        m_shape1 = m;
        m_shape2 = n;
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(size_t m, size_t n, const T &val) {
        m_data = new T[m * n];
        m_shape1 = m;
        m_shape2 = n;
        std::fill_n(m_data, m_shape1 * m_shape2, val);
    }

    template <class T>
    template <class InputIterator, typename>
    base_matrix<T, matrix_tag>::base_matrix(
        InputIterator first, InputIterator last, size_t n
    ) {
        size_t size = std::distance(first, last);
        m_shape1 = size / n;
        m_shape2 = n;
        if (size % n != 0) {
            ++m_shape1;
        }
        m_data = new T[m_shape1 * m_shape2];
        std::copy(first, last, m_data);
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(
        const base_matrix<T, matrix_tag> &other
    ) {
        m_shape1 = other.m_shape1;
        m_shape2 = other.m_shape2;
        m_data = new T[m_shape1 * m_shape2];
        std::copy(
            other.m_data, other.m_data + other.m_shape1 * other.m_shape2,
            m_data
        );
    }

    template <class T>
    template <class Tag>
    base_matrix<T, matrix_tag>::base_matrix(const base_matrix<T, Tag> &other) {
        m_shape1 = other.rows();
        m_shape2 = other.cols();
        m_data = new T[m_shape1 * m_shape2];
        std::copy(other.begin(true), other.end(true), m_data);
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(
        base_matrix<T, matrix_tag> &&other
    ) {
        m_data = other.m_data;
        m_shape1 = other.m_shape1;
        m_shape2 = other.m_shape2;
        other.m_data = NULL;
        other.m_shape1 = 0;
        other.m_shape2 = 0;
    }

    template <class T>
    base_matrix<T, matrix_tag>::base_matrix(
        std::initializer_list< std::initializer_list<T> > il
    ) {
        m_shape1 = il.size();
        m_shape2 = 0;
        for (const std::initializer_list<T> &row : il) {
            m_shape2 = std::max(m_shape2, row.size());
        }
        m_data = new T[m_shape1 * m_shape2];
        T* rowptr = m_data;
        for (const std::initializer_list<T> &row : il) {
            std::copy(row.begin(), row.end(), rowptr);
            rowptr += m_shape2;
        }
    }

    /// Destructor.

    template <class T>
    base_matrix<T, matrix_tag>::~base_matrix() {
        delete[] m_data;
    }

    /// Iterators.

    template <class T>
    inline typename base_matrix<T, matrix_tag>::iterator
    base_matrix<T, matrix_tag>::begin() {
        return this->begin(true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::begin() const {
        return this->begin(true);
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
        return this->end(true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::end() const {
        return this->end(true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::iterator
    base_matrix<T, matrix_tag>::end(bool row_major) {
        return base_matrix_iterator<T, matrix_tag>(
            this, m_shape1 * m_shape2, row_major
        );
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::end(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(
            this, m_shape1 * m_shape2, row_major
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
        return this->cbegin(true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cbegin(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(this, 0, row_major);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cend() const {
        return this->cend(true);
    }

    template <class T>
    inline typename base_matrix<T, matrix_tag>::const_iterator
    base_matrix<T, matrix_tag>::cend(bool row_major) const {
        return base_matrix_const_iterator<T, matrix_tag>(
            this, m_shape1 * m_shape2, row_major
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
        __assert_within_bounds(m_shape1, m_shape2, i, j);
        return m_data[i * m_shape2 + j];
    }

    template <class T>
    inline const T&
    base_matrix<T, matrix_tag>::operator()(size_t i, size_t j) const {
        __assert_within_bounds(m_shape1, m_shape2, i, j);
        return m_data[i * m_shape2 + j];
    }

    template <class T>
    inline T&
    base_matrix<T, matrix_tag>::operator[](const index_t &index) {
        return (*this)(index.first, index.second);
    }

    template <class T>
    inline const T&
    base_matrix<T, matrix_tag>::operator[](const index_t &index) const {
        return (*this)(index.first, index.second);
    }

    template <class T>
    base_matrix<T, matrix_view_tag>
    base_matrix<T, matrix_tag>::operator()(slice slc1, slice slc2) {
        if (slc1.size() > 0 && slc2.size() > 0) {
            __assert_within_bounds(
                m_shape1, m_shape2, slc1[slc1.size() - 1], slc2[slc2.size() - 1]
            );
        }
        size_t offset = slc1.start() * m_shape2 + slc2.start();
        size_t stride1 = slc1.stride() * m_shape2;
        size_t stride2 = slc2.stride();
        return base_matrix<T, matrix_view_tag>(
            slc1.size(), slc2.size(), m_data, offset, stride1, stride2
        );
    }

    template <class T>
    const base_matrix<T, matrix_view_tag>
    base_matrix<T, matrix_tag>::operator()(slice slc1, slice slc2) const {
        if (slc1.size() > 0 && slc2.size() > 0) {
            __assert_within_bounds(
                m_shape1, m_shape2, slc1[slc1.size() - 1], slc2[slc2.size() - 1]
            );
        }
        size_t offset = slc1.start() * m_shape2 + slc2.start();
        size_t stride1 = slc1.stride() * m_shape2;
        size_t stride2 = slc2.stride();
        return base_matrix<T, matrix_view_tag>(
            slc1.size(), slc2.size(), m_data, offset, stride1, stride2
        );
    }

    template <class T>
    base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::operator()(size_t i, slice slc) {
        if (slc.size() > 0) {
            __assert_within_bounds(m_shape1, m_shape2, i, slc[slc.size() - 1]);
        }
        size_t offset = i * m_shape2 + slc.start();
        size_t stride = slc.stride();
        return base_array<T, array_view_tag>(slc.size(), m_data, offset,stride);
    }

    template <class T>
    const base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::operator()(size_t i, slice slc) const {
        if (slc.size() > 0) {
            __assert_within_bounds(m_shape1, m_shape2, i, slc[slc.size() - 1]);
        }
        size_t offset = i * m_shape2 + slc.start();
        size_t stride = slc.stride();
        return base_array<T, array_view_tag>(slc.size(), m_data, offset,stride);
    }

    template <class T>
    base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::operator()(slice slc, size_t j) {
        if (slc.size() > 0) {
            __assert_within_bounds(m_shape1, m_shape2, slc[slc.size() - 1], j);
        }
        size_t offset = slc.start() * m_shape2 + j;
        size_t stride = slc.stride() * m_shape2;
        return base_array<T, array_view_tag>(slc.size(), m_data, offset,stride);
    }

    template <class T>
    const base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::operator()(slice slc, size_t j) const {
        if (slc.size() > 0) {
            __assert_within_bounds(m_shape1, m_shape2, slc[slc.size() - 1], j);
        }
        size_t offset = slc.start() * m_shape2 + j;
        size_t stride = slc.stride() * m_shape2;
        return base_array<T, array_view_tag>(slc.size(), m_data, offset,stride);
    }

    template <class T>
    template <class Tag>
    base_array<T, index_view_tag> base_matrix<T, matrix_tag>::operator[](
        const base_array<index_t, Tag> &index
    ) {
        size_t *m_index = new size_t[index.size()];
        for (size_t k = 0; k < index.size(); ++k) {
            size_t i = index[k].first, j = index[k].second;
            __assert_within_bounds(m_shape1, m_shape2, i, j);
            m_index[k] = i * m_shape2 + j;
        }
        return base_array<T, index_view_tag>(index.size(), m_data, m_index, -1);
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag> base_matrix<T, matrix_tag>::operator[](
        const base_array<index_t, Tag> &index
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
        size_t size = std::count(mask.begin(), mask.end(), true);
        size_t *m_index = new size_t[size];
        size_t n = 0;
        for (size_t i = 0; i < mask.rows(); ++i) {
            for (size_t j = 0; j < mask.cols(); ++j) {
                if (mask(i, j)) {
                    __assert_within_bounds(m_shape1, m_shape2, i, j);
                    m_index[n++] = i * m_shape2 + j;
                }
            }
        }
        return base_array<T, index_view_tag>(size, m_data, m_index, -1);
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag>
    base_matrix<T, matrix_tag>::operator[](const base_matrix<bool, Tag> &mask)
    const {
        size_t size = std::count(mask.begin(), mask.end(), true);
        base_array<T, array_tag> subarray(size);
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
        return m_shape1;
    }

    template <class T>
    inline size_t base_matrix<T, matrix_tag>::cols() const {
        return m_shape2;
    }

    template <class T>
    inline size_t base_matrix<T, matrix_tag>::size() const {
        return m_shape1 * m_shape2;
    }

    template <class T>
    inline void base_matrix<T, matrix_tag>::resize(size_t m, size_t n) {
        if (m_shape1 * m_shape2 != m * n) {
            delete[] m_data;
            m_data = new T[m * n];
        }
        m_shape1 = m;
        m_shape2 = n;
    }

    template <class T>
    inline bool base_matrix<T, matrix_tag>::empty() const {
        return (m_shape1 == 0 || m_shape2 == 0);
    }

    template <class T>
    inline T* base_matrix<T, matrix_tag>::data() {
        return m_data;
    }

    template <class T>
    inline const T* base_matrix<T, matrix_tag>::data() const {
        return m_data;
    }

    template <class T>
    inline bool base_matrix<T, matrix_tag>::rowmajor() const {
        return true;
    }

    template <class T>
    inline bool base_matrix<T, matrix_tag>::colmajor() const {
        return false;
    }

    /// Operator overloading.

    template <class T>
    inline base_matrix<T, matrix_tag>&
    base_matrix<T, matrix_tag>::operator=(
        const base_matrix<T, matrix_tag> &other
    ) {
        this->resize(other.m_shape1, other.m_shape2);
        std::copy(
            other.m_data, other.m_data + other.m_shape1 * other.m_shape2,
            m_data
        );
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_matrix<T, matrix_tag>&
    base_matrix<T, matrix_tag>::operator=(const base_matrix<T, Tag> &other) {
        this->resize(other.rows(), other.cols());
        std::copy(other.begin(true), other.end(true), m_data);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>&
    base_matrix<T, matrix_tag>::operator=(const T &val) {
        std::fill_n(m_data, m_shape1 * m_shape2, val);
        return *this;
    }

    template <class T>
    inline base_matrix<T, matrix_tag>&
    base_matrix<T, matrix_tag>::operator=(base_matrix<T, matrix_tag> &&other) {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_shape1 = other.m_shape1;
            m_shape2 = other.m_shape2;
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
        T* rowptr = m_data;
        for (const std::initializer_list<T> &row : il) {
            std::copy(row.begin(), row.end(), rowptr);
            rowptr += m_shape2;
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
    index_t base_matrix<T, matrix_tag>::argmax() const {
        __range_argmax pred;
        base_matrix_const_iterator<T, matrix_tag> it =
            this->begin() + pred(this->begin(), this->end());
        return index_t(it.row(), it.col());
    }

    template <class T>
    base_array< size_t, lazy_axis_tag<__range_argmax, T, matrix_tag> >
    base_matrix<T, matrix_tag>::argmax(bool rowwise) const {
        typedef lazy_axis_tag<__range_argmax, T, matrix_tag> Closure;
        return base_array<size_t, Closure>(__range_argmax(), *this, rowwise);
    }

    template <class T>
    index_t base_matrix<T, matrix_tag>::argmin() const {
        __range_argmin pred;
        base_matrix_const_iterator<T, matrix_tag> it =
            this->begin() + pred(this->begin(), this->end());
        return index_t(it.row(), it.col());
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
    base_matrix< T, lazy_unary_tag<__math_conj, T, matrix_tag> >
    base_matrix<T, matrix_tag>::conj() const {
        typedef lazy_unary_tag<__math_conj, T, matrix_tag> Closure;
        return base_matrix<T, Closure>(__math_conj(), *this);
    }

    template <class T>
    base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::diagonal(ptrdiff_t offset) {
        size_t size = 0, start = 0, stride = this->cols() + 1;
        if (offset >= 0 && this->cols() > (size_t)offset) {
            size = std::min(this->rows(), this->cols() - offset);
            start = offset;
        }
        else if (offset < 0 && this->rows() > (size_t)-offset) {
            size = std::min(this->rows() + offset, this->cols());
            start = -offset * this->cols();
        }
        return base_array<T, array_view_tag>(size, m_data, start, stride);
    }

    template <class T>
    const base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::diagonal(ptrdiff_t offset) const {
        size_t size = 0, start = 0, stride = this->cols() + 1;
        if (offset >= 0 && this->cols() > (size_t)offset) {
            size = std::min(this->rows(), this->cols() - offset);
            start = offset;
        }
        else if (offset < 0 && this->rows() > (size_t)-offset) {
            size = std::min(this->rows() + offset, this->cols());
            start = -offset * this->cols();
        }
        return base_array<T, array_view_tag>(size, m_data, start, stride);
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
        return base_array<T, array_view_tag>(this->size(), m_data);
    }

    template <class T>
    const base_array<T, array_view_tag>
    base_matrix<T, matrix_tag>::flatten() const {
        return base_array<T, array_view_tag>(this->size(), m_data);
    }

    template <class T>
    base_matrix<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_imag, T, matrix_tag>
    >
    base_matrix<T, matrix_tag>::imag() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_imag, T, matrix_tag> Closure;
        return base_matrix<Rt, Closure>(__math_imag(), *this);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::imag(
        const typename complex_traits<T>::value_type &val
    ) {
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->cols(); ++j) {
                (*this)(i, j).imag(val);
            }
        }
    }

    template <class T>
    template <class Tag>
    void base_matrix<T, matrix_tag>::imag(
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
    void base_matrix<T, matrix_tag>::partition(size_t kth, bool rowwise) {
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
    void base_matrix<T, matrix_tag>::partition(
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
        lazy_unary_tag<__math_real, T, matrix_tag>
    >
    base_matrix<T, matrix_tag>::real() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_real, T, matrix_tag> Closure;
        return base_matrix<Rt, Closure>(__math_real(), *this);
    }

    template <class T>
    void base_matrix<T, matrix_tag>::real(
        const typename complex_traits<T>::value_type &val
    ) {
        for (size_t i = 0; i < this->rows(); ++i) {
            for (size_t j = 0; j < this->cols(); ++j) {
                (*this)(i, j).real(val);
            }
        }
    }

    template <class T>
    template <class Tag>
    void base_matrix<T, matrix_tag>::real(
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
    void base_matrix<T, matrix_tag>::reverse(bool rowwise) {
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
    void base_matrix<T, matrix_tag>::sort(bool rowwise) {
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
    void base_matrix<T, matrix_tag>::sort(
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
        std::swap(m_data, other.m_data);
        std::swap(m_shape1, other.m_shape1);
        std::swap(m_shape2, other.m_shape2);
    }

    template <class T>
    base_matrix<T, matrix_view_tag> base_matrix<T, matrix_tag>::t() {
        return base_matrix<T, matrix_view_tag>(
            m_shape2, m_shape1, m_data, false
        );
    }

    template <class T>
    const base_matrix<T, matrix_view_tag>
    base_matrix<T, matrix_tag>::t() const {
        return base_matrix<T, matrix_view_tag>(
            m_shape2, m_shape1, m_data, false
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
