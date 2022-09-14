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

 /** @file include/numcpp/tensor/indirect_tensor.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_INDIRECT_TENSOR_TCC_INCLUDED
#define NUMCPP_INDIRECT_TENSOR_TCC_INCLUDED

#include <algorithm>

namespace numcpp {
    /// Constructors.

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor()
     : m_data(NULL), m_size(0), m_shape(), m_indices(NULL), m_order(row_major),
       m_owner(false)
     {}

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(
        const shape_t<Rank> &shape, T *data, const size_t *indices,
        layout_t order, int mode
    ) : m_data(data), m_size(shape.size()), m_shape(shape), m_order(order)
    {
        if (mode > 0) {
            size_t *indptr = new size_t[m_size];
            std::copy_n(indices, m_size, indptr);
            m_indices = indptr;
            m_owner = true;
        }
        else {
            m_indices = indices;
            m_owner = (mode < 0);
        }
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(const base_tensor &other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape),
       m_order(other.m_order), m_owner(other.m_owner)
    {
        if (other.m_owner) {
            size_t *indptr = new size_t[m_size];
            std::copy_n(other.m_indices, m_size, indptr);
            m_indices = indptr;
        }
        else {
            m_indices = other.m_indices;
        }
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(base_tensor &&other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape),
       m_indices(other.m_indices), m_order(other.m_order),
       m_owner(other.m_owner)
    {
        other.m_data = NULL;
        other.m_size = 0;
        other.m_shape = shape_t<Rank>();
        other.m_indices = NULL;
        other.m_order = row_major;
        other.m_owner = false;
    }

    /// Destructor.

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::~base_tensor() {
        if (m_owner) {
            delete[] m_indices;
        }
    }

    /// Indexing.

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline T& indirect_tensor<T, Rank>::operator()(Index... index) {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline const T& indirect_tensor<T, Rank>::operator()(Index... index) const {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    inline T& indirect_tensor<T, Rank>::operator[](const index_t<Rank> &index) {
        detail::assert_within_bounds(m_shape, index);
        return m_data[m_indices[ravel_index(index, m_shape, m_order)]];
    }

    template <class T, size_t Rank>
    inline const T&
    indirect_tensor<T, Rank>::operator[](const index_t<Rank> &index) const {
        detail::assert_within_bounds(m_shape, index);
        return m_data[m_indices[ravel_index(index, m_shape, m_order)]];
    }

    template <class T, size_t Rank>
    inline T& indirect_tensor<T, Rank>::operator[](size_t i) {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        detail::assert_within_bounds(m_size, i);
        return m_data[m_indices[i]];
    }

    template <class T, size_t Rank>
    inline const T& indirect_tensor<T, Rank>::operator[](size_t i) const {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        detail::assert_within_bounds(m_size, i);
        return m_data[m_indices[i]];
    }

    template <class T, size_t Rank>
    constexpr size_t indirect_tensor<T, Rank>::ndim() {
        return Rank;
    }

    template <class T, size_t Rank>
    inline const shape_t<Rank>& indirect_tensor<T, Rank>::shape() const {
        return m_shape;
    }

    template <class T, size_t Rank>
    inline size_t indirect_tensor<T, Rank>::shape(size_t axis) const {
        return m_shape[axis];
    }

    template <class T, size_t Rank>
    inline size_t indirect_tensor<T, Rank>::size() const {
        return m_size;
    }

    template <class T, size_t Rank>
    inline bool indirect_tensor<T, Rank>::empty() const {
        return (m_size == 0);
    }

    template <class T, size_t Rank>
    inline T* indirect_tensor<T, Rank>::data() {
        return m_data;
    }

    template <class T, size_t Rank>
    inline const T* indirect_tensor<T, Rank>::data() const {
        return m_data;
    }

    template <class T, size_t Rank>
    inline const size_t* indirect_tensor<T, Rank>::indices() const {
        return m_indices;
    }

    template <class T, size_t Rank>
    inline layout_t indirect_tensor<T, Rank>::layout() const {
        return m_order;
    }

    template <class T, size_t Rank>
    inline bool indirect_tensor<T, Rank>::is_owner() const {
        return m_owner;
    }

    /// Assignment operator.

    template <class T, size_t Rank>
    template <class U, class Tag>
    indirect_tensor<T, Rank>& indirect_tensor<T, Rank>::operator=(
        const base_tensor<U, Rank, Tag> &other
    ) {
        if (this->shape() != other.shape()) {
            std::ostringstream error;
            error << "input shape " << other.shape() << " doesn't match the "
                  << "output shape " << this->shape();
            throw std::invalid_argument(error.str());
        }
        std::transform(
            other.begin(m_order), other.end(m_order), this->begin(),
            cast_to<T>()
        );
        return *this;
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>&
    indirect_tensor<T, Rank>::operator=(const T &val) {
        std::fill_n(this->begin(), this->size(), val);
        return *this;
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>&
    indirect_tensor<T, Rank>::operator=(base_tensor &&other) {
        if (this != &other) {
            if (m_owner) {
                delete[] m_indices;
            }
            m_data = other.m_data;
            m_size = other.m_size;
            m_shape = other.m_shape;
            m_indices = other.m_indices;
            m_order = other.m_order;
            m_owner = other.m_owner;
            other.m_data = NULL;
            other.m_size = 0;
            other.m_shape = shape_t<Rank>();
            other.m_indices = NULL;
            other.m_order = row_major;
            other.m_owner = false;
        }
        return *this;
    }
}

#endif // NUMCPP_INDIRECT_TENSOR_TCC_INCLUDED
