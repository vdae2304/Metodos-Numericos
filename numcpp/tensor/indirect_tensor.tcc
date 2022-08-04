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
     : m_data(NULL), m_size(0), m_shape(), m_index(NULL), m_order(true),
       m_owner(false)
     {}

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(
        const shape_t<Rank> &shape, T *data, size_t *index,
        bool order, int mode
    ) : m_data(data), m_size(shape.size()), m_shape(shape), m_order(order)
    {
        if (mode > 0) {
            m_index = new size_t[m_size];
            std::copy_n(index, m_size, m_index);
            m_owner = true;
        }
        else {
            m_index = index;
            m_owner = (mode < 0);
        }
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(
        const shape_t<Rank> &shape, T *data, const size_t *index,
        bool order
    ) : m_data(data), m_size(shape.size()), m_shape(shape), m_order(order)
    {
        m_index = new size_t[m_size];
        std::copy_n(index, m_size, m_index);
        m_owner = true;
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(const base_tensor &other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape),
       m_order(other.m_order)
    {
        if (other.m_owner) {
            m_index = new size_t[m_size];
            std::copy_n(other.m_index, m_size, m_index);
            m_owner = true;
        }
        else {
            m_index = other.m_index;
        }
        m_owner = other.m_owner;
    }

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::base_tensor(base_tensor &&other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape),
       m_index(other.m_index), m_order(other.m_order), m_owner(other.m_owner)
    {
        other.m_data = NULL;
        other.m_size = 0;
        other.m_shape = index_t<Rank>();
        other.m_index = NULL;
        other.m_order = true;
        other.m_owner = false;
    }

    /// Destructor.

    template <class T, size_t Rank>
    indirect_tensor<T, Rank>::~base_tensor() {
        if (m_owner) {
            delete[] m_index;
        }
    }

    /// Indexing.

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...>>
    inline T& indirect_tensor<T, Rank>::operator()(Args... args) {
        return this->operator[](make_index(args...));
    }

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...>>
    inline const T& indirect_tensor<T, Rank>::operator()(Args... args) const {
        return this->operator[](make_index(args...));
    }

    template <class T, size_t Rank>
    inline T& indirect_tensor<T, Rank>::operator[](const index_t<Rank> &index) {
        assert_within_bounds(m_shape, index);
        return m_data[m_index[ravel_index(index, m_shape, m_order)]];
    }

    template <class T, size_t Rank>
    inline const T&
    indirect_tensor<T, Rank>::operator[](const index_t<Rank> &index) const {
        assert_within_bounds(m_shape, index);
        return m_data[m_index[ravel_index(index, m_shape, m_order)]];
    }

    template <class T, size_t Rank>
    inline T& indirect_tensor<T, Rank>::operator[](size_t i) {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        assert_within_bounds(m_size, i);
        return m_data[m_index[i]];
    }

    template <class T, size_t Rank>
    inline const T& indirect_tensor<T, Rank>::operator[](size_t i) const {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        assert_within_bounds(m_size, i);
        return m_data[m_index[i]];
    }

    template <class T, size_t Rank>
    constexpr size_t indirect_tensor<T, Rank>::ndim() const {
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
    inline size_t* indirect_tensor<T, Rank>::index() {
        return m_index;
    }

    template <class T, size_t Rank>
    inline const size_t* indirect_tensor<T, Rank>::index() const {
        return m_index;
    }

    template <class T, size_t Rank>
    inline bool indirect_tensor<T, Rank>::rowmajor() const {
        return m_order;
    }

    template <class T, size_t Rank>
    inline bool indirect_tensor<T, Rank>::colmajor() const {
        return !m_order;
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
        std::copy(other.begin(m_order), other.end(m_order), this->begin());
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
                delete[] m_index;
            }
            m_data = other.m_data;
            m_size = other.m_size;
            m_shape = other.m_shape;
            m_index = other.m_index;
            m_order = other.m_order;
            m_owner = other.m_owner;
            other.m_data = NULL;
            other.m_size = 0;
            other.m_shape = index_t<Rank>();
            other.m_index = NULL;
            other.m_order = true;
            other.m_owner = false;
        }
        return *this;
    }
}

#endif // NUMCPP_INDIRECT_TENSOR_TCC_INCLUDED
