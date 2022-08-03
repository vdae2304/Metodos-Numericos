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

 /** @file include/numcpp/tensor/tensor_iterator.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_ITERATOR_TCC_INCLUDED
#define NUMCPP_TENSOR_ITERATOR_TCC_INCLUDED

namespace numcpp {
    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag> make_tensor_iterator(
        base_tensor<T, Rank, Tag> *ptr, size_t index, bool order
    ) {
        return base_tensor_iterator<T, Rank, Tag>(ptr, index, order);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag>::base_tensor_iterator()
     : m_ptr(NULL), m_index(0), m_order(true) {}

    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag>::base_tensor_iterator(
        base_tensor<T, Rank, Tag> *ptr, size_t index, bool order
    ) : m_ptr(ptr), m_index(index), m_order(order) {}

    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag>::base_tensor_iterator(
        const base_tensor_iterator &other
    ) : m_ptr(other.m_ptr), m_index(other.m_index), m_order(other.m_order) {}

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>&
    base_tensor_iterator<T, Rank, Tag>::operator=(
        const base_tensor_iterator &other
    ) {
        m_ptr = other.m_ptr;
        m_index = other.m_index;
        m_order = other.m_order;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>&
    base_tensor_iterator<T, Rank, Tag>::operator++() {
        ++m_index;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>&
    base_tensor_iterator<T, Rank, Tag>::operator--() {
        --m_index;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    base_tensor_iterator<T, Rank, Tag>::operator++(int) {
        base_tensor_iterator<T, Rank, Tag> it = *this;
        ++m_index;
        return it;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>
    base_tensor_iterator<T, Rank, Tag>::operator--(int) {
        base_tensor_iterator<T, Rank, Tag> it = *this;
        --m_index;
        return it;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>&
    base_tensor_iterator<T, Rank, Tag>::operator+=(ptrdiff_t rhs) {
        m_index += rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag>&
    base_tensor_iterator<T, Rank, Tag>::operator-=(ptrdiff_t rhs) {
        m_index -= rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline typename base_tensor_iterator<T, Rank, Tag>::reference
    base_tensor_iterator<T, Rank, Tag>::operator*() const {
        return m_ptr->operator[](this->coords());
    }

    template <class T, size_t Rank, class Tag>
    inline typename base_tensor_iterator<T, Rank, Tag>::pointer
    base_tensor_iterator<T, Rank, Tag>::operator->() const {
        return &(m_ptr->operator[](this->coords()));
    }

    template <class T, size_t Rank, class Tag>
    inline typename base_tensor_iterator<T, Rank, Tag>::reference
    base_tensor_iterator<T, Rank, Tag>::operator[](ptrdiff_t n) const {
        return m_ptr->operator[](
            unravel_index(m_index + n, m_ptr->shape(), m_order)
        );
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, Tag>*
    base_tensor_iterator<T, Rank, Tag>::base() const {
        return m_ptr;
    }

    template <class T, size_t Rank, class Tag>
    inline size_t base_tensor_iterator<T, Rank, Tag>::index() const {
        return m_index;
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> base_tensor_iterator<T, Rank, Tag>::coords() const {
        return unravel_index(m_index, m_ptr->shape(), m_order);
    }

    template <class T, size_t Rank, class Tag>
    inline bool base_tensor_iterator<T, Rank, Tag>::rowmajor() const {
        return m_order;
    }

    template <class T, size_t Rank, class Tag>
    inline bool base_tensor_iterator<T, Rank, Tag>::colmajor() const {
        return !m_order;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag> operator+(
        const base_tensor_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_tensor_iterator<T, Rank, Tag> it = lhs;
        return it += rhs;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag> operator+(
        ptrdiff_t lhs, const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        base_tensor_iterator<T, Rank, Tag> it = rhs;
        return it += lhs;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_iterator<T, Rank, Tag> operator-(
        const base_tensor_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_tensor_iterator<T, Rank, Tag> it = lhs;
        return it -= rhs;
    }

    template <class T, size_t Rank, class Tag>
    inline ptrdiff_t operator-(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator==(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator!=(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator<(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator>(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator<=(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator>=(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag> make_tensor_const_iterator(
        const base_tensor<T, Rank, Tag> *ptr, size_t index, bool order
    ) {
        return base_tensor_const_iterator<T, Rank, Tag>(ptr, index, order);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag>::base_tensor_const_iterator()
     : m_ptr(NULL), m_index(0), m_order(true) {}

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag>::base_tensor_const_iterator(
        const base_tensor<T, Rank, Tag> *ptr, size_t index, bool order
    ) : m_ptr(ptr), m_index(index), m_order(order) {}

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag>::base_tensor_const_iterator(
        const base_tensor_iterator<T, Rank, Tag> &other
    ) : m_ptr(other.base()), m_index(other.index()), m_order(other.rowmajor())
     {}

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag>::base_tensor_const_iterator(
        const base_tensor_const_iterator &other
    ) : m_ptr(other.m_ptr), m_index(other.m_index), m_order(other.m_order) {}

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>&
    base_tensor_const_iterator<T, Rank, Tag>::operator=(
        const base_tensor_iterator<T, Rank, Tag> &other
    ) {
        m_ptr = other.base();
        m_index = other.index();
        m_order = other.rowmajor();
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>&
    base_tensor_const_iterator<T, Rank, Tag>::operator=(
        const base_tensor_const_iterator &other
    ) {
        m_ptr = other.m_ptr;
        m_index = other.m_index;
        m_order = other.m_order;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>&
    base_tensor_const_iterator<T, Rank, Tag>::operator++() {
        ++m_index;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>&
    base_tensor_const_iterator<T, Rank, Tag>::operator--() {
        --m_index;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    base_tensor_const_iterator<T, Rank, Tag>::operator++(int) {
        base_tensor_const_iterator<T, Rank, Tag> it = *this;
        ++m_index;
        return it;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>
    base_tensor_const_iterator<T, Rank, Tag>::operator--(int) {
        base_tensor_const_iterator<T, Rank, Tag> it = *this;
        --m_index;
        return it;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>&
    base_tensor_const_iterator<T, Rank, Tag>::operator+=(ptrdiff_t rhs) {
        m_index += rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag>&
    base_tensor_const_iterator<T, Rank, Tag>::operator-=(ptrdiff_t rhs) {
        m_index -= rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag>
    inline typename base_tensor_const_iterator<T, Rank, Tag>::reference
    base_tensor_const_iterator<T, Rank, Tag>::operator*() const {
        return m_ptr->operator[](this->coords());
    }

    template <class T, size_t Rank, class Tag>
    inline typename base_tensor_const_iterator<T, Rank, Tag>::pointer
    base_tensor_const_iterator<T, Rank, Tag>::operator->() const {
        return &(m_ptr->operator[](this->coords()));
    }

    template <class T, size_t Rank, class Tag>
    inline typename base_tensor_const_iterator<T, Rank, Tag>::reference
    base_tensor_const_iterator<T, Rank, Tag>::operator[](ptrdiff_t n) const {
        return m_ptr->operator[](
            unravel_index(m_index + n, m_ptr->shape(), m_order)
        );
    }

    template <class T, size_t Rank, class Tag>
    inline const base_tensor<T, Rank, Tag>*
    base_tensor_const_iterator<T, Rank, Tag>::base() const {
        return m_ptr;
    }

    template <class T, size_t Rank, class Tag>
    inline size_t base_tensor_const_iterator<T, Rank, Tag>::index() const {
        return m_index;
    }

    template <class T, size_t Rank, class Tag>
    inline index_t<Rank> base_tensor_const_iterator<T, Rank, Tag>::coords()
    const {
        return unravel_index(m_index, m_ptr->shape(), m_order);
    }

    template <class T, size_t Rank, class Tag>
    inline bool base_tensor_const_iterator<T, Rank, Tag>::rowmajor() const {
        return m_order;
    }

    template <class T, size_t Rank, class Tag>
    inline bool base_tensor_const_iterator<T, Rank, Tag>::colmajor() const {
        return !m_order;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag> operator+(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_tensor_const_iterator<T, Rank, Tag> it = lhs;
        return it += rhs;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag> operator+(
        ptrdiff_t lhs, const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        base_tensor_const_iterator<T, Rank, Tag> it = rhs;
        return it += lhs;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor_const_iterator<T, Rank, Tag> operator-(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    ) {
        base_tensor_const_iterator<T, Rank, Tag> it = lhs;
        return it -= rhs;
    }

    template <class T, size_t Rank, class Tag>
    inline ptrdiff_t operator-(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator==(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator!=(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator<(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator>(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator<=(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, size_t Rank, class Tag>
    inline bool operator>=(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }
}

#endif // NUMCPP_TENSOR_ITERATOR_TCC_INCLUDED
