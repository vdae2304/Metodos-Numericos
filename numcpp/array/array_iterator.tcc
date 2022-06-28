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

 /** @file include/numcpp/array/array_iterator.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/array.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_ITERATOR_TCC_INCLUDED
#define NUMCPP_ARRAY_ITERATOR_TCC_INCLUDED

namespace numcpp {
    template <class T, class Tag>
    inline base_array_iterator<T, Tag>::base_array_iterator() {
        this->m_ptr = NULL;
        this->m_index = 0;
    }
    
    template <class T, class Tag>
    inline base_array_iterator<T, Tag>::base_array_iterator(
        base_array<T, Tag> *arr, size_t i
    ) {
        this->m_ptr = arr;
        this->m_index = i;
    }
    
    template <class T, class Tag>
    inline base_array_iterator<T, Tag>::base_array_iterator(
        const base_array_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag>& base_array_iterator<T, Tag>::operator=(
        const base_array_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag>& 
    base_array_iterator<T, Tag>::operator++() {
        ++this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag>& 
    base_array_iterator<T, Tag>::operator--() {
        --this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag> 
    base_array_iterator<T, Tag>::operator++(int) {
        base_array_iterator<T, Tag> it = *this;
        ++this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag> 
    base_array_iterator<T, Tag>::operator--(int) {
        base_array_iterator<T, Tag> it = *this;
        --this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag>& 
    base_array_iterator<T, Tag>::operator+=(ptrdiff_t rhs) {
        this->m_index += rhs;
        return *this;
    }
    
    template <class T, class Tag>
    inline base_array_iterator<T, Tag>& 
    base_array_iterator<T, Tag>::operator-=(ptrdiff_t rhs) {
        this->m_index -= rhs;
        return *this;
    }

    template <class T, class Tag>
    inline typename base_array_iterator<T, Tag>::reference 
    base_array_iterator<T, Tag>::operator*() const {
        return (*this->m_ptr)[this->m_index];
    }

    template <class T, class Tag>
    inline typename base_array_iterator<T, Tag>::pointer 
    base_array_iterator<T, Tag>::operator->() const {
        return &(*this->m_ptr)[this->m_index];
    }

    template <class T, class Tag>
    inline typename base_array_iterator<T, Tag>::reference 
    base_array_iterator<T, Tag>::operator[](ptrdiff_t n) const {
        return (*this->m_ptr)[this->m_index + n];
    }

    template <class T, class Tag>
    inline base_array<T, Tag>* base_array_iterator<T, Tag>::base() const {
        return this->m_ptr;
    }

    template <class T, class Tag>
    inline size_t base_array_iterator<T, Tag>::index() const {
        return this->m_index;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag> operator+(
        const base_array_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_array_iterator<T, Tag> it = lhs;
        return it += rhs;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_array_iterator<T, Tag> &rhs
    ) {
        base_array_iterator<T, Tag> it = rhs;
        return it += lhs;
    }

    template <class T, class Tag>
    inline base_array_iterator<T, Tag> operator-(
        const base_array_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_array_iterator<T, Tag> it = lhs;
        return it -= rhs;
    }

    template <class T, class Tag>
    inline ptrdiff_t operator-(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, class Tag>
    inline bool operator==(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, class Tag>
    inline bool operator!=(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<=(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>=(
        const base_array_iterator<T, Tag> &lhs, 
        const base_array_iterator<T, Tag> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>::base_array_const_iterator() {
        this->m_ptr = NULL;
        this->m_index = 0;
    }
    
    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>::base_array_const_iterator(
        const base_array<T, Tag> *arr, size_t i
    ) {
        this->m_ptr = arr;
        this->m_index = i;
    }
    
    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>::base_array_const_iterator(
        const base_array_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.base();
        this->m_index = other.index();
    }
    
    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>::base_array_const_iterator(
        const base_array_const_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>& 
    base_array_const_iterator<T, Tag>::operator=(
        const base_array_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.base();
        this->m_index = other.index();
        return *this;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>& 
    base_array_const_iterator<T, Tag>::operator=(
        const base_array_const_iterator<T, Tag> &other
    ) {
        this->m_ptr = other.m_ptr;
        this->m_index = other.m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>& 
    base_array_const_iterator<T, Tag>::operator++() {
        ++this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>& 
    base_array_const_iterator<T, Tag>::operator--() {
        --this->m_index;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag> 
    base_array_const_iterator<T, Tag>::operator++(int) {
        base_array_const_iterator<T, Tag> it = *this;
        ++this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag> 
    base_array_const_iterator<T, Tag>::operator--(int) {
        base_array_const_iterator<T, Tag> it = *this;
        --this->m_index;
        return it;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>& 
    base_array_const_iterator<T, Tag>::operator+=(ptrdiff_t rhs) {
        this->m_index += rhs;
        return *this;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag>& 
    base_array_const_iterator<T, Tag>::operator-=(ptrdiff_t rhs) {
        this->m_index -= rhs;
        return *this;
    }

    template <class T, class Tag>
    inline typename base_array_const_iterator<T, Tag>::reference 
    base_array_const_iterator<T, Tag>::operator*() const {
        return (*this->m_ptr)[this->m_index];
    }

    template <class T, class Tag>
    inline typename base_array_const_iterator<T, Tag>::pointer 
    base_array_const_iterator<T, Tag>::operator->() const {
        return &(*this->m_ptr)[this->m_index];
    }

    template <class T, class Tag>
    inline typename base_array_const_iterator<T, Tag>::reference 
    base_array_const_iterator<T, Tag>::operator[](ptrdiff_t n) const {
        return (*this->m_ptr)[this->m_index + n];
    }

    template <class T, class Tag>
    inline const base_array<T, Tag>*
    base_array_const_iterator<T, Tag>::base() const {
        return this->m_ptr;
    }

    template <class T, class Tag>
    inline size_t base_array_const_iterator<T, Tag>::index() const {
        return this->m_index;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag> operator+(
        const base_array_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_array_const_iterator<T, Tag> it = lhs;
        return it += rhs;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_array_const_iterator<T, Tag> &rhs
    ) {
        base_array_const_iterator<T, Tag> it = rhs;
        return it += lhs;
    }

    template <class T, class Tag>
    inline base_array_const_iterator<T, Tag> operator-(
        const base_array_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_array_const_iterator<T, Tag> it = lhs;
        return it -= rhs;
    }

    template <class T, class Tag>
    inline ptrdiff_t operator-(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, class Tag>
    inline bool operator==(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, class Tag>
    inline bool operator!=(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, class Tag>
    inline bool operator<=(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, class Tag>
    inline bool operator>=(
        const base_array_const_iterator<T, Tag> &lhs, 
        const base_array_const_iterator<T, Tag> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }
}

#endif // NUMCPP_ARRAY_ITERATOR_TCC_INCLUDED
