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

namespace numcpp {
    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator() {
        m_ptr = NULL;
        m_index = 0;
        m_order = true;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator(
        base_matrix<T, Tag> *mat, size_t index, bool order
    ) {
        m_ptr = mat;
        m_index = index;
        m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator(
        base_matrix<T, Tag> *mat, size_t i, size_t j, bool order
    ) {
        m_ptr = mat;
        if (order) {
            m_index = i * mat->cols() + j;
        }
        else {
            m_index = j * mat->rows() + i;
        }
        m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>::base_matrix_iterator(
        const base_matrix_iterator<T, Tag> &other
    ) {
        m_ptr = other.m_ptr;
        m_index = other.m_index;
        m_order = other.m_order;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>&
    base_matrix_iterator<T, Tag>::operator=(
        const base_matrix_iterator<T, Tag> &other
    ) {
        m_ptr = other.m_ptr;
        m_index = other.m_index;
        m_order = other.m_order;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>&
    base_matrix_iterator<T, Tag>::operator++() {
        ++m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>&
    base_matrix_iterator<T, Tag>::operator--() {
        --m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>
    base_matrix_iterator<T, Tag>::operator++(int) {
        base_matrix_iterator<T, Tag> it = *this;
        ++m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>
    base_matrix_iterator<T, Tag>::operator--(int) {
        base_matrix_iterator<T, Tag> it = *this;
        --m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>&
    base_matrix_iterator<T, Tag>::operator+=(ptrdiff_t rhs) {
        m_index += rhs;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_iterator<T, Tag>&
    base_matrix_iterator<T, Tag>::operator-=(ptrdiff_t rhs) {
        m_index -= rhs;
        return *this;
    }

    template <class T, class Tag>
    inline typename base_matrix_iterator<T, Tag>::reference
    base_matrix_iterator<T, Tag>::operator*() const {
        return (*m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_iterator<T, Tag>::pointer
    base_matrix_iterator<T, Tag>::operator->() const {
        return &(*m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_iterator<T, Tag>::reference
    base_matrix_iterator<T, Tag>::operator[](ptrdiff_t n) const {
        base_matrix_iterator<T, Tag> it = *this + n;
        return *it;
    }

    template <class T, class Tag>
    inline base_matrix<T, Tag>* base_matrix_iterator<T, Tag>::base() const {
        return m_ptr;
    }

    template <class T, class Tag>
    inline size_t base_matrix_iterator<T, Tag>::index() const {
        return m_index;
    }

    template <class T, class Tag>
    inline size_t base_matrix_iterator<T, Tag>::row() const {
        if (m_order) {
            return m_index / m_ptr->cols();
        }
        else {
            return m_index % m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline size_t base_matrix_iterator<T, Tag>::col() const {
        if (m_order) {
            return m_index % m_ptr->cols();
        }
        else {
            return m_index / m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline bool base_matrix_iterator<T, Tag>::rowmajor() const {
        return m_order;
    }

    template <class T, class Tag>
    inline bool base_matrix_iterator<T, Tag>::colmajor() const {
        return !m_order;
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
        m_ptr = NULL;
        m_index = 0;
        m_order = true;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix<T, Tag> *mat, size_t index, bool order
    ) {
        m_ptr = mat;
        m_index = index;
        m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix<T, Tag> *mat, size_t i, size_t j, bool order
    ) {
        m_ptr = mat;
        if (order) {
            m_index = i * mat->cols() + j;
        }
        else {
            m_index = j * mat->rows() + i;
        }
        m_order = order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix_iterator<T, Tag> &other
    ) {
        m_ptr = other.base();
        m_index = other.index();
        m_order = other.rowmajor();
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>::base_matrix_const_iterator(
        const base_matrix_const_iterator<T, Tag> &other
    ) {
        m_ptr = other.m_ptr;
        m_index = other.m_index;
        m_order = other.m_order;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>&
    base_matrix_const_iterator<T, Tag>::operator=(
        const base_matrix_iterator<T, Tag> &other
    ) {
        m_ptr = other.base();
        m_index = other.index();
        m_order = other.rowmajor();
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>&
    base_matrix_const_iterator<T, Tag>::operator=(
        const base_matrix_const_iterator<T, Tag> &other
    ) {
        m_ptr = other.m_ptr;
        m_index = other.m_index;
        m_order = other.m_order;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>&
    base_matrix_const_iterator<T, Tag>::operator++() {
        ++m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>&
    base_matrix_const_iterator<T, Tag>::operator--() {
        --m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>
    base_matrix_const_iterator<T, Tag>::operator++(int) {
        base_matrix_const_iterator<T, Tag> it = *this;
        ++m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>
    base_matrix_const_iterator<T, Tag>::operator--(int) {
        base_matrix_const_iterator<T, Tag> it = *this;
        --m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>&
    base_matrix_const_iterator<T, Tag>::operator+=(ptrdiff_t rhs) {
        m_index += rhs;
        return *this;
    }

    template <class T, class Tag>
    inline base_matrix_const_iterator<T, Tag>&
    base_matrix_const_iterator<T, Tag>::operator-=(ptrdiff_t rhs) {
        m_index -= rhs;
        return *this;
    }

    template <class T, class Tag>
    inline typename base_matrix_const_iterator<T, Tag>::reference
    base_matrix_const_iterator<T, Tag>::operator*() const {
        return (*m_ptr)(this->row(), this->col());
    }

    template <class T, class Tag>
    inline typename base_matrix_const_iterator<T, Tag>::pointer
    base_matrix_const_iterator<T, Tag>::operator->() const {
        return &(*m_ptr)(this->row(), this->col());
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
        return m_ptr;
    }

    template <class T, class Tag>
    inline size_t base_matrix_const_iterator<T, Tag>::index() const {
        return m_index;
    }

    template <class T, class Tag>
    inline size_t base_matrix_const_iterator<T, Tag>::row() const {
        if (m_order) {
            return m_index / m_ptr->cols();
        }
        else {
            return m_index % m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline size_t base_matrix_const_iterator<T, Tag>::col() const {
        if (m_order) {
            return m_index % m_ptr->cols();
        }
        else {
            return m_index / m_ptr->rows();
        }
    }

    template <class T, class Tag>
    inline bool base_matrix_const_iterator<T, Tag>::rowmajor() const {
        return m_order;
    }

    template <class T, class Tag>
    inline bool base_matrix_const_iterator<T, Tag>::colmajor() const {
        return !m_order;
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
