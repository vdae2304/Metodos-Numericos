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

 /** @file include/numcpp/broadcasting/reduce_iterator.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_REDUCE_ITERATOR_TCC_INCLUDED
#define NUMCPP_TENSOR_REDUCE_ITERATOR_TCC_INCLUDED

namespace numcpp {
    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>
    make_reduce_iterator(
        base_tensor<T, Rank, Tag> *ptr,
        const index_t<Rank> &indices,
        const shape_t<N> &axes,
        size_t flat
    ) {
        return base_tensor_reduce_iterator<T, Rank, Tag, N>(
            ptr, indices, axes, flat
        );
    }

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_reduce_iterator<T, Rank, Tag, N>::base_tensor_reduce_iterator()
     : m_ptr(NULL), m_indices(), m_axes(), m_flat(0) {}

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_reduce_iterator<T, Rank, Tag, N>::base_tensor_reduce_iterator(
        base_tensor<T, Rank, Tag> *ptr,
        const index_t<Rank> &indices,
        const shape_t<N> &axes,
        size_t flat
    ) : m_ptr(ptr), m_indices(indices), m_axes(axes), m_flat(flat) {}

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_reduce_iterator<T, Rank, Tag, N>::base_tensor_reduce_iterator(
        const base_tensor_reduce_iterator &other
    ) : m_ptr(other.m_ptr), m_indices(other.m_indices), m_axes(other.m_axes),
        m_flat(other.m_flat) {}

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator=(
        const base_tensor_reduce_iterator &other
    ) {
        m_ptr = other.m_ptr;
        m_indices = other.m_indices;
        m_axes = other.m_axes;
        m_flat = other.m_flat;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator++() {
        ++m_flat;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator--() {
        --m_flat;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator++(int) {
        base_tensor_reduce_iterator<T, Rank, Tag, N> it = *this;
        ++m_flat;
        return it;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator--(int) {
        base_tensor_reduce_iterator<T, Rank, Tag, N> it = *this;
        --m_flat;
        return it;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator+=(
        difference_type rhs
    ) {
        m_flat += rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator-=(
        difference_type rhs
    ) {
        m_flat -= rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline typename base_tensor_reduce_iterator<T, Rank, Tag, N>::reference
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator*() const {
        return m_ptr->operator[](this->coords());
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline typename base_tensor_reduce_iterator<T, Rank, Tag, N>::pointer
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator->() const {
        return &(m_ptr->operator[](this->coords()));
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline typename base_tensor_reduce_iterator<T, Rank, Tag, N>::reference
    base_tensor_reduce_iterator<T, Rank, Tag, N>::operator[](difference_type n)
    const {
        base_tensor_reduce_iterator<T, Rank, Tag, N> it = *this + n;
        return *it;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor<T, Rank, Tag>*
    base_tensor_reduce_iterator<T, Rank, Tag, N>::base() const {
        return m_ptr;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline size_t base_tensor_reduce_iterator<T, Rank, Tag, N>::index() const {
        return m_flat;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline index_t<Rank>
    base_tensor_reduce_iterator<T, Rank, Tag, N>::coords() const {
        shape_t<N> reduced_shape;
        for (size_t i = 0; i < N; ++i) {
            reduced_shape[i] = m_ptr->shape(m_axes[i]);
        }
        index_t<Rank> indices = m_indices;
        index_t<N> reduced_index = unravel_index(m_flat, reduced_shape);
        for (size_t i = 0; i < N; ++i) {
            indices[m_axes[i]] = reduced_index[i];
        }
        return indices;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline const shape_t<N>&
    base_tensor_reduce_iterator<T, Rank, Tag, N>::axes() const {
        return m_axes;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N> operator+(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    ) {
        base_tensor_reduce_iterator<T, Rank, Tag, N> it = lhs;
        return it += rhs;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N> operator+(
        ptrdiff_t lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        base_tensor_reduce_iterator<T, Rank, Tag, N> it = rhs;
        return it += lhs;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_reduce_iterator<T, Rank, Tag, N> operator-(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    ) {
        base_tensor_reduce_iterator<T, Rank, Tag, N> it = lhs;
        return it -= rhs;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline ptrdiff_t operator-(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return (ptrdiff_t)lhs.index() - (ptrdiff_t)rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator==(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator!=(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator<(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator>(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator<=(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator>=(
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    make_const_reduce_iterator(
        const base_tensor<T, Rank, Tag> *ptr,
        const index_t<Rank> &indices,
        const shape_t<N> &axes,
        size_t flat
    ) {
        return base_tensor_const_reduce_iterator<T, Rank, Tag, N>(
            ptr, indices, axes, flat
        );
    }

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    ::base_tensor_const_reduce_iterator()
     : m_ptr(NULL), m_indices(), m_axes(), m_flat(0) {}

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    ::base_tensor_const_reduce_iterator(
        const base_tensor<T, Rank, Tag> *ptr,
        const index_t<Rank> &indices,
        const shape_t<N> &axes,
        size_t flat
    ) : m_ptr(ptr), m_indices(indices), m_axes(axes), m_flat(flat) {}

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    ::base_tensor_const_reduce_iterator(
        const base_tensor_reduce_iterator<T, Rank, Tag, N> &other
    ) : m_ptr(other.base()), m_indices(other.coords()), m_axes(other.axes()),
        m_flat(other.index()) {}

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    ::base_tensor_const_reduce_iterator(
        const base_tensor_const_reduce_iterator &other
    ) : m_ptr(other.m_ptr), m_indices(other.m_indices), m_axes(other.m_axes),
        m_flat(other.m_flat) {}

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator=(
        const base_tensor_reduce_iterator<T, Rank, Tag, N> &other
    ) {
        m_ptr = other.base();
        m_indices = other.coords();
        m_axes = other.axes();
        m_flat = other.index();
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator=(
        const base_tensor_const_reduce_iterator &other
    ) {
        m_ptr = other.m_ptr,
        m_indices = other.m_indices;
        m_axes = other.m_axes;
        m_flat = other.m_flat;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator++() {
        ++m_flat;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator--() {
        --m_flat;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator++(int) {
        base_tensor_const_reduce_iterator<T, Rank, Tag, N> it = *this;
        ++m_flat;
        return it;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator--(int) {
        base_tensor_const_reduce_iterator<T, Rank, Tag, N> it = *this;
        --m_flat;
        return it;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator+=(
        difference_type rhs
    ) {
        m_flat += rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator-=(
        difference_type rhs
    ) {
        m_flat -= rhs;
        return *this;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline typename
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::reference
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator*() const {
        return m_ptr->operator[](this->coords());
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline typename base_tensor_const_reduce_iterator<T, Rank, Tag, N>::pointer
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator->() const {
        return &(m_ptr->operator[](this->coords()));
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline typename
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::reference
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::operator[](
        difference_type n
    ) const {
        base_tensor_const_reduce_iterator<T, Rank, Tag, N> it = *this + n;
        return *it;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline const base_tensor<T, Rank, Tag>*
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::base() const {
        return m_ptr;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline size_t base_tensor_const_reduce_iterator<T, Rank, Tag, N>::index()
    const {
        return m_flat;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline index_t<Rank>
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::coords() const {
        shape_t<N> reduced_shape;
        for (size_t i = 0; i < N; ++i) {
            reduced_shape[i] = m_ptr->shape(m_axes[i]);
        }
        index_t<Rank> indices = m_indices;
        index_t<N> reduced_index = unravel_index(m_flat, reduced_shape);
        for (size_t i = 0; i < N; ++i) {
            indices[m_axes[i]] = reduced_index[i];
        }
        return indices;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline const shape_t<N>&
    base_tensor_const_reduce_iterator<T, Rank, Tag, N>::axes() const {
        return m_axes;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N> operator+(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    ) {
        base_tensor_const_reduce_iterator<T, Rank, Tag, N> it = lhs;
        return it += rhs;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N> operator+(
        ptrdiff_t lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        base_tensor_const_reduce_iterator<T, Rank, Tag, N> it = rhs;
        return it += lhs;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline base_tensor_const_reduce_iterator<T, Rank, Tag, N> operator-(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    ) {
        base_tensor_const_reduce_iterator<T, Rank, Tag, N> it = lhs;
        return it -= rhs;
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline ptrdiff_t operator-(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() - rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator==(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() == rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator!=(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() != rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator<(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() < rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator>(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() > rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator<=(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() <= rhs.index();
    }

    template <class T, size_t Rank, class Tag, size_t N>
    inline bool operator>=(
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_reduce_iterator<T, Rank, Tag, N> &rhs
    ) {
        return lhs.index() >= rhs.index();
    }
}

// #include "numcpp/tensor/tensor_iterator.tcc"

#endif // NUMCPP_TENSOR_REDUCE_ITERATOR_TCC_INCLUDED
