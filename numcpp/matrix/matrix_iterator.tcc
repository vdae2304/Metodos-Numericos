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

 /** @file include/numcpp/matrix/matrix_iterator.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_ITERATOR_TCC_INCLUDED
#define NUMCPP_MATRIX_ITERATOR_TCC_INCLUDED

#include <cstdio>
#include <stdexcept>

namespace numcpp {
    /// Helper function: Throws a std::out_of_range error if a pair of indices 
    /// is out of bounds.
    inline void __assert_within_bounds(
        size_t shape1, size_t shape2, size_t i, size_t j
    ) {
        if (i >= shape1 || j >= shape2) {
            char error[132];
            sprintf(
                error, "index (%zu, %zu) is out of bounds with shape "
                "(%zu, %zu)", i, j, shape1, shape2
            );
            throw std::out_of_range(error);
        }
    }
    
    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator() {
        this->m_ptr = NULL;
        this->m_index = 0;
        this->m_order = true;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator(
        base_matrix<T, Tag> *mat, size_t index, bool order
    ) {
        this->m_ptr = mat;
        this->m_index = index;
        this->m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator(
        base_matrix<T, Tag> *mat, size_t i, size_t j, bool order
    ) {
        this->m_ptr = mat;
        if (order) {
            this->m_index = i*mat->cols() + j;
        }
        else {
            this->m_index = j*mat->rows() + i;
        }
        this->m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator(
        const base_matrix_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
        this->m_order = other.m_order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>& 
    base_matrix_iterator<T, Tag>::operator=(
        const base_matrix_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
        this->m_order = other.m_order;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>& 
    base_matrix_iterator<T, Tag>::operator++() {
        ++this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>& 
    base_matrix_iterator<T, Tag>::operator--() {
        --this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag> 
    base_matrix_iterator<T, Tag>::operator++(int) {
        base_matrix_iterator<T, Tag> it = *this;
        ++this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag> 
    base_matrix_iterator<T, Tag>::operator--(int) {
        base_matrix_iterator<T, Tag> it = *this;
        --this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>& 
    base_matrix_iterator<T, Tag>::operator+=(ptrdiff_t rhs) {
        this->m_index += rhs;
        return *this;
    }
    
    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>& 
    base_matrix_iterator<T, Tag>::operator-=(ptrdiff_t rhs) {
        this->m_index -= rhs;
        return *this;
    }

    template <class T, class Tag>
    inline typename base_matrix_iterator<T, Tag>::reference 
    base_matrix_iterator<T, Tag>::operator*() const {
        return (*this->m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_iterator<T, Tag>::pointer 
    base_matrix_iterator<T, Tag>::operator->() const {
        return &(*this->m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_iterator<T, Tag>::reference 
    base_matrix_iterator<T, Tag>::operator[](ptrdiff_t n) const {
        base_matrix_iterator<T, Tag> it = *this + n;
        return *it;
    }

    template <class T, class Tag>
    inline base_matrix<T, Tag>* base_matrix_iterator<T, Tag>::base() const {
        return this->m_ptr;
    }

    template <class T, class Tag>
    inline size_t base_matrix_iterator<T, Tag>::index() const {
        return this->m_index;
    }

    template <class T, class Tag>
    inline size_t base_matrix_iterator<T, Tag>::row() const {
        if (this->m_order) {
            return this->m_index / this->m_ptr->cols();
        }
        else {
            return this->m_index % this->m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline size_t base_matrix_iterator<T, Tag>::col() const {
        if (this->m_order) {
            return this->m_index % this->m_ptr->cols();
        }
        else {
            return this->m_index / this->m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline bool base_matrix_iterator<T, Tag>::rowmajor() const {
        return this->m_order;
    }

    template <class T, class Tag>
    inline bool base_matrix_iterator<T, Tag>::colmajor() const {
        return !this->m_order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag> operator+(
        const base_matrix_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_matrix_iterator<T, Tag> it = lhs;
        return it += rhs;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_matrix_iterator<T, Tag> &rhs
    ) {
        base_matrix_iterator<T, Tag> it = rhs;
        return it += lhs;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag> operator-(
        const base_matrix_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_matrix_iterator<T, Tag> it = lhs;
        return it -= rhs;
    }

    template <class T, class Tag>
    inline ptrdiff_t operator-(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, class Tag>
    inline bool operator==(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, class Tag>
    inline bool operator!=(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<=(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>=(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator() {
        this->m_ptr = NULL;
        this->m_index = 0;
        this->m_order = true;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix<T, Tag> *mat, size_t index, bool order
    ) {
        this->m_ptr = mat;
        this->m_index = index;
        this->m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix<T, Tag> *mat, size_t i, size_t j, bool order
    ) {
        this->m_ptr = mat;
        if (order) {
            this->m_index = i*mat->cols() + j;
        }
        else {
            this->m_index = j*mat->rows() + i;
        }
        this->m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.base();
        this->m_index = other.index();
        this->m_order = other.order();
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix_const_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
        this->m_order = other.m_order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>& 
    base_matrix_const_iterator<T, Tag>::operator=(
        const base_matrix_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.base();
        this->m_index = other.index();
        this->m_order = other.order();
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>& 
    base_matrix_const_iterator<T, Tag>::operator=(
        const base_matrix_const_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
        this->m_order = other.m_order;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>& 
    base_matrix_const_iterator<T, Tag>::operator++() {
        ++this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>& 
    base_matrix_const_iterator<T, Tag>::operator--() {
        --this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag> 
    base_matrix_const_iterator<T, Tag>::operator++(int) {
        base_matrix_const_iterator<T, Tag> it = *this;
        ++this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag> 
    base_matrix_const_iterator<T, Tag>::operator--(int) {
        base_matrix_const_iterator<T, Tag> it = *this;
        --this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>& 
    base_matrix_const_iterator<T, Tag>::operator+=(ptrdiff_t rhs) {
        this->m_index += rhs;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>& 
    base_matrix_const_iterator<T, Tag>::operator-=(ptrdiff_t rhs) {
        this->m_index -= rhs;
        return *this;
    }

    template <class T, class Tag>
    inline typename base_matrix_const_iterator<T, Tag>::reference 
    base_matrix_const_iterator<T, Tag>::operator*() const {
        return (*this->m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_const_iterator<T, Tag>::pointer 
    base_matrix_const_iterator<T, Tag>::operator->() const {
        return &(*this->m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_const_iterator<T, Tag>::reference 
    base_matrix_const_iterator<T, Tag>::operator[](ptrdiff_t n) const {
        base_matrix_const_iterator<T, Tag> it = *this + n;
        return *it;
    }

    template <class T, class Tag>
    inline const base_matrix<T, Tag>* 
    base_matrix_const_iterator<T, Tag>::base() const {
        return this->m_ptr;
    }

    template <class T, class Tag>
    inline size_t base_matrix_const_iterator<T, Tag>::index() const {
        return this->m_index;
    }

    template <class T, class Tag>
    inline size_t base_matrix_const_iterator<T, Tag>::row() const {
        if (this->m_order) {
            return this->m_index / this->m_ptr->cols();
        }
        else {
            return this->m_index % this->m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline size_t base_matrix_const_iterator<T, Tag>::col() const {
        if (this->m_order) {
            return this->m_index % this->m_ptr->cols();
        }
        else {
            return this->m_index / this->m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline bool base_matrix_const_iterator<T, Tag>::rowmajor() const {
        return this->m_order;
    }

    template <class T, class Tag>
    inline bool base_matrix_const_iterator<T, Tag>::colmajor() const {
        return !this->m_order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag> operator+(
        const base_matrix_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_matrix_const_iterator<T, Tag> it = lhs;
        return it += rhs;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        base_matrix_const_iterator<T, Tag> it = rhs;
        return it += lhs;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag> operator-(
        const base_matrix_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_matrix_const_iterator<T, Tag> it = lhs;
        return it -= rhs;
    }

    template <class T, class Tag>
    inline ptrdiff_t operator-(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, class Tag>
    inline bool operator==(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, class Tag>
    inline bool operator!=(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<=(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>=(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }
}

#endif // NUMCPP_MATRIX_ITERATOR_TCC_INCLUDED