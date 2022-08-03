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

 /** @file include/numcpp/array/owning_array.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/array.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_TCC_INCLUDED
#define NUMCPP_ARRAY_TCC_INCLUDED

#include <algorithm>
#include <iterator>

namespace numcpp {
    /// Constructors.

    template <class T>
    base_array<T, array_tag>::base_array() {
        m_data = NULL;
        m_size = 0;
    }

    template <class T>
    base_array<T, array_tag>::base_array(size_t n) {
        m_data = new T[n];
        m_size = n;
    }

    template <class T>
    base_array<T, array_tag>::base_array(size_t n, const T &val) {
        m_data = new T[n];
        m_size = n;
        std::fill_n(m_data, m_size, val);
    }

    template <class T>
    template <class InputIterator, typename>
    base_array<T, array_tag>::base_array(
        InputIterator first, InputIterator last
    ) {
        m_size = std::distance(first, last);
        m_data = new T[m_size];
        std::copy(first, last, m_data);
    }

    template <class T>
    base_array<T, array_tag>::base_array(
        const base_array<T, array_tag> &other
    ) {
        m_size = other.m_size;
        m_data = new T[m_size];
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag>::base_array(const base_array<T, Tag> &other) {
        m_size = other.size();
        m_data = new T[m_size];
        std::copy(other.begin(), other.end(), m_data);
    }

    template <class T>
    base_array<T, array_tag>::base_array(base_array<T, array_tag> &&other) {
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = NULL;
        other.m_size = 0;
    }

    template <class T>
    base_array<T, array_tag>::base_array(std::initializer_list<T> il) {
        m_size = il.size();
        m_data = new T[m_size];
        std::copy(il.begin(), il.end(), m_data);
    }

    /// Destructor.

    template <class T>
    base_array<T, array_tag>::~base_array() {
        delete[] m_data;
    }

    /// Iterators.

    template <class T>
    inline typename base_array<T, array_tag>::iterator
    base_array<T, array_tag>::begin() {
        return base_array_iterator<T, array_tag>(this, 0);
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_iterator
    base_array<T, array_tag>::begin() const {
        return base_array_const_iterator<T, array_tag>(this, 0);
    }

    template <class T>
    inline typename base_array<T, array_tag>::iterator
    base_array<T, array_tag>::end() {
        return base_array_iterator<T, array_tag>(this, m_size);
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_iterator
    base_array<T, array_tag>::end() const {
        return base_array_const_iterator<T, array_tag>(this, m_size);
    }

    template <class T>
    inline typename base_array<T, array_tag>::reverse_iterator
    base_array<T, array_tag>::rbegin() {
        return typename base_array<T, array_tag>::reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_reverse_iterator
    base_array<T, array_tag>::rbegin() const {
        return typename base_array<T, array_tag>::const_reverse_iterator(
            this->end()
        );
    }

    template <class T>
    inline typename base_array<T, array_tag>::reverse_iterator
    base_array<T, array_tag>::rend() {
        return typename base_array<T, array_tag>::reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_reverse_iterator
    base_array<T, array_tag>::rend() const {
        return typename base_array<T, array_tag>::const_reverse_iterator(
            this->begin()
        );
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_iterator
    base_array<T, array_tag>::cbegin() const {
        return base_array_const_iterator<T, array_tag>(this, 0);
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_iterator
    base_array<T, array_tag>::cend() const {
        return base_array_const_iterator<T, array_tag>(this, m_size);
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_reverse_iterator
    base_array<T, array_tag>::crbegin() const {
        return typename base_array<T, array_tag>::const_reverse_iterator(
            this->cend()
        );
    }

    template <class T>
    inline typename base_array<T, array_tag>::const_reverse_iterator
    base_array<T, array_tag>::crend() const {
        return typename base_array<T, array_tag>::const_reverse_iterator(
            this->cbegin()
        );
    }

    /// Array indexing.

    template <class T>
    inline T& base_array<T, array_tag>::operator[](size_t i) {
        __assert_within_bounds(m_size, i);
        return m_data[i];
    }

    template <class T>
    inline const T& base_array<T, array_tag>::operator[](size_t i) const {
        __assert_within_bounds(m_size, i);
        return m_data[i];
    }

    template <class T>
    base_array<T, array_view_tag>
    base_array<T, array_tag>::operator[](slice slc) {
        if (slc.size() > 0) {
            __assert_within_bounds(m_size, slc[slc.size() - 1]);
        }
        return base_array<T, array_view_tag>(
            slc.size(), m_data, slc.start(), slc.stride()
        );
    }

    template <class T>
    const base_array<T, array_view_tag>
    base_array<T, array_tag>::operator[](slice slc) const {
        if (slc.size() > 0) {
            __assert_within_bounds(m_size, slc[slc.size() - 1]);
        }
        return base_array<T, array_view_tag>(
            slc.size(), m_data, slc.start(), slc.stride()
        );
    }

    template <class T>
    template <class Tag>
    base_array<T, index_view_tag>
    base_array<T, array_tag>::operator[](const base_array<size_t, Tag> &index) {
        size_t *m_index = new size_t[index.size()];
        for (size_t i = 0; i < index.size(); ++i) {
            m_index[i] = index[i];
            __assert_within_bounds(m_size, m_index[i]);
        }
        return base_array<T, index_view_tag>(index.size(), m_data, m_index, -1);
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag>
    base_array<T, array_tag>::operator[](const base_array<size_t, Tag> &index)
    const {
        base_array<T, array_tag> subarray(index.size());
        for (size_t i = 0; i < index.size(); ++i) {
            subarray[i] = (*this)[index[i]];
        }
        return subarray;
    }

    template <class T>
    template <class Tag>
    base_array<T, index_view_tag>
    base_array<T, array_tag>::operator[](const base_array<bool, Tag> &mask) {
        size_t size = std::count(mask.begin(), mask.end(), true);
        size_t *m_index = new size_t[size];
        size_t n = 0;
        for (size_t i = 0; i < mask.size(); ++i) {
            if (mask[i]) {
                __assert_within_bounds(m_size, i);
                m_index[n++] = i;
            }
        }
        return base_array<T, index_view_tag>(size, m_data, m_index, -1);
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag>
    base_array<T, array_tag>::operator[](const base_array<bool, Tag> &mask)
    const {
        size_t size = std::count(mask.begin(), mask.end(), true);
        base_array<T, array_tag> subarray(size);
        size_t n = 0;
        for (size_t i = 0; i < mask.size(); ++i) {
            if (mask[i]) {
                subarray[n++] = (*this)[i];
            }
        }
        return subarray;
    }

    template <class T>
    inline size_t base_array<T, array_tag>::size() const {
        return m_size;
    }

    template <class T>
    inline void base_array<T, array_tag>::resize(size_t n) {
        if (m_size != n) {
            delete[] m_data;
            m_data = new T[n];
            m_size = n;
        }
    }

    template <class T>
    inline bool base_array<T, array_tag>::empty() const {
        return (m_size == 0);
    }

    template <class T>
    inline T* base_array<T, array_tag>::data() {
        return m_data;
    }

    template <class T>
    inline const T* base_array<T, array_tag>::data() const {
        return m_data;
    }

    /// Operator overloading

    template <class T>
    inline base_array<T, array_tag>& base_array<T, array_tag>::operator=(
        const base_array<T, array_tag> &other
    ) {
        this->resize(other.m_size);
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator=(const base_array<T, Tag> &other) {
        this->resize(other.size());
        std::copy(other.begin(), other.end(), m_data);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator=(const T &val) {
        std::fill_n(m_data, m_size, val);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator=(base_array<T, array_tag> &&other) {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_size = other.m_size;
            other.m_data = NULL;
            other.m_size = 0;
        }
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator=(std::initializer_list<T> il) {
        this->resize(il.size());
        std::copy(il.begin(), il.end(), m_data);
        return *this;
    }

    /// Compound assignment operator.

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator+=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator-=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator*=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator/=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator%=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator&=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator|=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator^=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator<<=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator>>=(const base_array<T, array_tag> &rhs) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator+=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__plus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator-=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__minus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator*=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__multiplies(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator/=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__divides(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator%=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__modulus(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator&=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__bit_and(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator|=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__bit_or(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator^=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__bit_xor(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator<<=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__left_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    template <class Tag>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator>>=(const base_array<T, Tag> &rhs) {
        __apply_binary_function(__right_shift(), *this, rhs, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator+=(const T &val) {
        __apply_binary_function(__plus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator-=(const T &val) {
        __apply_binary_function(__minus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator*=(const T &val) {
        __apply_binary_function(__multiplies(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator/=(const T &val) {
        __apply_binary_function(__divides(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator%=(const T &val) {
        __apply_binary_function(__modulus(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator&=(const T &val) {
        __apply_binary_function(__bit_and(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator|=(const T &val) {
        __apply_binary_function(__bit_or(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator^=(const T &val) {
        __apply_binary_function(__bit_xor(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator<<=(const T &val) {
        __apply_binary_function(__left_shift(), *this, val, *this);
        return *this;
    }

    template <class T>
    inline base_array<T, array_tag>&
    base_array<T, array_tag>::operator>>=(const T &val) {
        __apply_binary_function(__right_shift(), *this, val, *this);
        return *this;
    }

    /// Public methods.

    template <class T>
    void base_array<T, array_tag>::apply(T f(T)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    void base_array<T, array_tag>::apply(T f(const T&)) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    template <class Function>
    void base_array<T, array_tag>::apply(Function f) {
        __apply_unary_function(f, *this, *this);
    }

    template <class T>
    size_t base_array<T, array_tag>::argmax() const {
        __range_argmax pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    size_t base_array<T, array_tag>::argmin() const {
        __range_argmin pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array<size_t, array_tag>
    base_array<T, array_tag>::argpartition(size_t kth) const {
        return this->argpartition(kth, __less());
    }

    template <class T>
    template <class Compare>
    base_array<size_t, array_tag>
    base_array<T, array_tag>::argpartition(size_t kth, Compare comp) const {
        __assert_within_bounds(this->size(), kth);
        base_array<size_t, array_tag> index(this->size());
        __iota(index.begin(), index.end(), 0, 1);
        auto comparator = [this, comp](size_t i, size_t j) {
            return comp((*this)[i], (*this)[j]);
        };
        std::nth_element(
            index.begin(), index.begin() + kth, index.end(), comparator
        );
        return index;
    }

    template <class T>
    base_array<size_t, array_tag> base_array<T, array_tag>::argsort() const {
        return this->argsort(__less());
    }

    template <class T>
    template <class Compare>
    base_array<size_t, array_tag>
    base_array<T, array_tag>::argsort(Compare comp, bool stable) const {
        base_array<size_t, array_tag> index(this->size());
        __iota(index.begin(), index.end(), 0, 1);
        auto comparator = [this, comp](size_t i, size_t j) {
            return comp((*this)[i], (*this)[j]);
        };
        if (stable) {
            std::stable_sort(index.begin(), index.end(), comparator);
        }
        else {
            std::sort(index.begin(), index.end(), comparator);
        }
        return index;
    }

    template <class T>
    template <class U>
    base_array< U, lazy_unary_tag<__identity, T, array_tag> >
    base_array<T, array_tag>::astype() const {
        typedef lazy_unary_tag<__identity, T, array_tag> Closure;
        return base_array<U, Closure>(__identity(), *this);
    }

    template <class T>
    void base_array<T, array_tag>::clamp(const T &a_min, const T &a_max) {
        __apply_unary_function(__clamp<T>(a_min, a_max), *this, *this);
    }

    template <class T>
    base_array< T, lazy_unary_tag<__math_conj, T, array_tag> >
    base_array<T, array_tag>::conj() const {
        typedef lazy_unary_tag<__math_conj, T, array_tag> Closure;
        return base_array<T, Closure>(__math_conj(), *this);
    }

    template <class T>
    template <class Tag>
    T base_array<T, array_tag>::dot(const base_array<T, Tag> &rhs) const {
        __assert_equal_length(this->size(), rhs.size());
        T val = 0;
        for (size_t i = 0; i < this->size(); ++i) {
            val += (*this)[i] * rhs[i];
        }
        return val;
    }

    template <class T>
    template <class Tag>
    base_array<T, array_tag>
    base_array<T, array_tag>::dot(const base_matrix<T, Tag> &rhs) const {
        __assert_matmul_shapes(1, this->size(), rhs.rows(), rhs.cols());
        base_array<T, array_tag> out(rhs.cols(), T(0));
        for (size_t i = 0; i < rhs.rows(); ++i) {
            for (size_t j = 0; j < rhs.cols(); ++j) {
                out[j] += (*this)[i] * rhs(i, j);
            }
        }
        return out;
    }

    template <class T>
    base_array<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_imag, T, array_tag>
    >
    base_array<T, array_tag>::imag() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_imag, T, array_tag> Closure;
        return base_array<Rt, Closure>(__math_imag(), *this);
    }

    template <class T>
    void base_array<T, array_tag>::imag(
        const typename complex_traits<T>::value_type &val
    ) {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i].imag(val);
        }
    }

    template <class T>
    template <class Tag>
    void base_array<T, array_tag>::imag(
        const base_array<typename complex_traits<T>::value_type, Tag> &arr
    ) {
        __assert_equal_length(this->size(), arr.size());
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i].imag(arr[i]);
        }
    }

    template <class T>
    T base_array<T, array_tag>::max() const {
        __range_max pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_tag>::mean() const {
        __range_mean pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_tag>::min() const {
        __range_min pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    void base_array<T, array_tag>::partition(size_t kth) {
        __assert_within_bounds(this->size(), kth);
        std::nth_element(this->begin(), this->begin() + kth, this->end());
    }

    template <class T>
    template <class Compare>
    void base_array<T, array_tag>::partition(size_t kth, Compare comp) {
        __assert_within_bounds(this->size(), kth);
        std::nth_element(this->begin(), this->begin() + kth, this->end(), comp);
    }

    template <class T>
    T base_array<T, array_tag>::prod() const {
        __range_prod pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array<
        typename complex_traits<T>::value_type,
        lazy_unary_tag<__math_real, T, array_tag>
    >
    base_array<T, array_tag>::real() const {
        typedef typename complex_traits<T>::value_type Rt;
        typedef lazy_unary_tag<__math_real, T, array_tag> Closure;
        return base_array<Rt, Closure>(__math_real(), *this);
    }

    template <class T>
    void base_array<T, array_tag>::real(
        const typename complex_traits<T>::value_type &val
    ) {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i].real(val);
        }
    }

    template <class T>
    template <class Tag>
    void base_array<T, array_tag>::real(
        const base_array<typename complex_traits<T>::value_type, Tag> &arr
    ) {
        __assert_equal_length(this->size(), arr.size());
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i].real(arr[i]);
        }
    }

    template <class T>
    void base_array<T, array_tag>::reverse() {
        std::reverse(this->begin(), this->end());
    }

    template <class T>
    void base_array<T, array_tag>::sort() {
        std::sort(this->begin(), this->end());
    }

    template <class T>
    template <class Compare>
    void base_array<T, array_tag>::sort(Compare comp, bool stable) {
        if (stable) {
            std::stable_sort(this->begin(), this->end(), comp);
        }
        else {
            std::sort(this->begin(), this->end(), comp);
        }
    }

    template <class T>
    T base_array<T, array_tag>::stddev(size_t ddof) const {
        __range_stddev pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    T base_array<T, array_tag>::sum() const {
        __range_sum pred;
        return pred(this->begin(), this->end());
    }

    template <class T>
    void base_array<T, array_tag>::swap(base_array<T, array_tag> &other) {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }

    template <class T>
    T base_array<T, array_tag>::var(size_t ddof) const {
        __range_var pred(ddof);
        return pred(this->begin(), this->end());
    }

    template <class T>
    base_array<T, array_view_tag> base_array<T, array_tag>::view() {
        return base_array<T, array_view_tag>(this->size(), this->data());
    }
}

#endif // NUMCPP_ARRAY_TCC_INCLUDED
