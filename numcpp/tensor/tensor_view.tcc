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

 /** @file include/numcpp/tensor/tensor_view.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_VIEW_TCC_INCLUDED
#define NUMCPP_TENSOR_VIEW_TCC_INCLUDED

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    /// Constructors.

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor()
     : m_data(NULL), m_size(0), m_shape(), m_offset(0), m_stride(),
       m_order(true)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(
        const shape_t<Rank> &shape, T *data, bool order
    ) : m_data(data), m_size(shape.size()), m_shape(shape),
        m_offset(0), m_stride(), m_order(order)
    {
        if (m_order) {
            m_stride[Rank - 1] = 1;
        }
        else {
            m_stride[0] = 1;
        }
        for (size_t i = 0; i < Rank - 1; ++i) {
            if (m_order) {
                size_t j = Rank - 1 - i;
                m_stride[j - 1] = m_stride[j] * m_shape[j];
            }
            else {
                m_stride[i + 1] = m_stride[i] * m_shape[i];
            }
        }
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(
        const shape_t<Rank> &shape, T *data,
        size_t offset, const shape_t<Rank> &strides, bool order
    ) : m_data(data), m_size(shape.size()), m_shape(shape),
        m_offset(offset), m_stride(strides), m_order(order)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(const base_tensor &other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape),
       m_offset(other.m_offset), m_stride(other.m_stride),
       m_order(other.m_order)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(base_tensor &&other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape),
       m_offset(other.m_offset), m_stride(other.m_stride),
       m_order(other.m_order)
    {
        other.m_data = NULL;
        other.m_size = 0;
        other.m_shape = shape_t<Rank>();
        other.m_offset = 0;
        other.m_stride = shape_t<Rank>();
        other.m_order = true;
    }

    /// Destructor.

    template <class T, size_t Rank>
    tensor_view<T, Rank>::~base_tensor() {}

    /// Indexing.

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...>>
    inline T& tensor_view<T, Rank>::operator()(Args... args) {
        return this->operator[](make_index(args...));
    }

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...>>
    inline const T& tensor_view<T, Rank>::operator()(Args... args) const {
        return this->operator[](make_index(args...));
    }

    template <class T, size_t Rank>
    T& tensor_view<T, Rank>::operator[](const index_t<Rank> &index) {
        assert_within_bounds(m_shape, index);
        size_t m_index = m_offset;
        for (size_t i = 0; i < Rank; ++i) {
            m_index += index[i] * m_stride[i];
        }
        return m_data[m_index];
    }

    template <class T, size_t Rank>
    const T& tensor_view<T, Rank>::operator[](const index_t<Rank> &index)
    const {
        assert_within_bounds(m_shape, index);
        size_t m_index = m_offset;
        for (size_t i = 0; i < Rank; ++i) {
            m_index += index[i] * m_stride[i];
        }
        return m_data[m_index];
    }

    template <class T, size_t Rank>
    inline T& tensor_view<T, Rank>::operator[](size_t i) {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        assert_within_bounds(m_size, i);
        return m_data[m_offset + i * m_stride[0]];
    }

    template <class T, size_t Rank>
    inline const T& tensor_view<T, Rank>::operator[](size_t i) const {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        assert_within_bounds(m_size, i);
        return m_data[m_offset + i * m_stride[0]];
    }

    template <class T, size_t Rank>
    constexpr size_t tensor_view<T, Rank>::ndim() const {
        return Rank;
    }

    template <class T, size_t Rank>
    inline const shape_t<Rank>& tensor_view<T, Rank>::shape() const {
        return m_shape;
    }

    template <class T, size_t Rank>
    inline size_t tensor_view<T, Rank>::shape(size_t axis) const {
        return m_shape[axis];
    }

    template <class T, size_t Rank>
    inline size_t tensor_view<T, Rank>::size() const {
        return m_size;
    }

    template <class T, size_t Rank>
    inline bool tensor_view<T, Rank>::empty() const {
        return (m_size == 0);
    }

    template <class T, size_t Rank>
    inline T* tensor_view<T, Rank>::data() {
        return m_data;
    }

    template <class T, size_t Rank>
    inline const T* tensor_view<T, Rank>::data() const {
        return m_data;
    }

    template <class T, size_t Rank>
    inline size_t tensor_view<T, Rank>::offset() const {
        return m_offset;
    }

    template <class T, size_t Rank>
    inline const shape_t<Rank>& tensor_view<T, Rank>::strides() const {
        return m_stride;
    }

    template <class T, size_t Rank>
    inline size_t tensor_view<T, Rank>::strides(size_t axis) const {
        return m_stride[axis];
    }

    template <class T, size_t Rank>
    inline bool tensor_view<T, Rank>::rowmajor() const {
        return m_order;
    }

    template <class T, size_t Rank>
    inline bool tensor_view<T, Rank>::colmajor() const {
        return !m_order;
    }

    /// Assignment operator.

    template <class T, size_t Rank>
    template <class U, class Tag>
    tensor_view<T, Rank>&
    tensor_view<T, Rank>::operator=(const base_tensor<U, Rank, Tag> &other) {
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
    tensor_view<T, Rank>& tensor_view<T, Rank>::operator=(const T &val) {
        std::fill_n(this->begin(), this->size(), val);
        return *this;
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank>&
    tensor_view<T, Rank>::operator=(base_tensor &&other) {
        if (this != &other) {
            m_data = other.m_data;
            m_size = other.m_size;
            m_shape = other.m_shape;
            m_offset = other.m_offset;
            m_stride = other.m_stride;
            m_order = other.m_order;
            other.m_data = NULL;
            other.m_size = 0;
            other.m_shape = shape_t<Rank>();
            other.m_offset = 0;
            other.m_stride = shape_t<Rank>();
            other.m_order = true;
        }
        return *this;
    }

    /// Public methods.

    template <class T, size_t Rank>
    tensor_view<T, 1> tensor_view<T, Rank>::diagonal(ptrdiff_t offset) {
        static_assert(Rank == 2, "Input must be 2 dimensional");
        size_t size = 0, start = 0, stride = 0;
        index_t<2> index = (offset >= 0) ? make_index(0, offset)
                                         : make_index(-offset, 0);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            start = m_offset + index[0] * m_stride[0] + index[1] * m_stride[1];
            stride = m_stride[0] + m_stride[1];
        }
        return tensor_view<T, 1>(size, m_data, start, stride);
    }

    template <class T, size_t Rank>
    tensor_view<const T, 1> tensor_view<T, Rank>::diagonal(ptrdiff_t offset)
    const {
        static_assert(Rank == 2, "Input must be 2 dimensional");
        size_t size = 0, start = 0, stride = 0;
        index_t<2> index = (offset >= 0) ? make_index(0, offset)
                                         : make_index(-offset, 0);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            start = m_offset + index[0] * m_stride[0] + index[1] * m_stride[1];
            stride = m_stride[0] + m_stride[1];
        }
        return tensor_view<const T, 1>(size, m_data, start, stride);
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<T, Rank - N>
    tensor_view<T, Rank>::squeeze(const shape_t<N> &axes) {
        static_assert(N < Rank, "Reduction dimension must be less than tensor"
                      " dimension");
        shape_t<Rank - N> new_shape, new_stride;
        bool keep_axis[Rank];
        std::fill_n(keep_axis, Rank, true);
        for (size_t i = 0; i < N; ++i) {
            keep_axis[axes[i]] = false;
        }
        size_t n = 0;
        for (size_t i = 0; i < Rank; ++i) {
            if (keep_axis[i]) {
                new_shape[n] = m_shape[i];
                new_stride[n++] = m_stride[i];
            }
            else if (m_shape[i] != 1) {
                char error[] = "cannot select an axis to squeeze out which has "
                               "size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<T, Rank - N>(
            new_shape, m_data, m_offset, new_stride, m_order
        );
    }

    template <class T, size_t Rank>
    template <class... Args, detail::RequiresIntegral<Args...> >
    inline tensor_view<T, Rank - sizeof...(Args)>
    tensor_view<T, Rank>::squeeze(Args... args) {
        return this->squeeze(make_shape(args...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<const T, Rank - N>
    tensor_view<T, Rank>::squeeze(const shape_t<N> &axes) const {
        static_assert(N < Rank, "Reduction dimension must be less than tensor"
                      " dimension");
        shape_t<Rank - N> new_shape, new_stride;
        bool keep_axis[Rank];
        std::fill_n(keep_axis, Rank, true);
        for (size_t i = 0; i < N; ++i) {
            keep_axis[axes[i]] = false;
        }
        size_t n = 0;
        for (size_t i = 0; i < Rank; ++i) {
            if (keep_axis[i]) {
                new_shape[n] = m_shape[i];
                new_stride[n++] = m_stride[i];
            }
            else if (m_shape[i] != 1) {
                char error[] = "cannot select an axis to squeeze out which has "
                               "size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<const T, Rank - N>(
            new_shape, m_data, m_offset, new_stride, m_order
        );
    }

    template <class T, size_t Rank>
    template <class... Args, detail::RequiresIntegral<Args...> >
    inline tensor_view<const T, Rank - sizeof...(Args)>
    tensor_view<T, Rank>::squeeze(Args... args) const {
        return this->squeeze(make_shape(args...));
    }

    template <class T, size_t Rank>
    void tensor_view<T, Rank>::swapaxes(size_t axis1, size_t axis2) {
        std::swap(m_shape[axis1], m_shape[axis2]);
        std::swap(m_stride[axis1], m_stride[axis2]);
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank> tensor_view<T, Rank>::t() {
        return tensor_view<T, Rank>(
            m_shape.transpose(), m_data, m_offset, m_stride.transpose(),
            !m_order
        );
    }

    template <class T, size_t Rank>
    tensor_view<const T, Rank> tensor_view<T, Rank>::t() const {
        return tensor_view<const T, Rank>(
            m_shape.transpose(), m_data, m_offset, m_stride.transpose(),
            !m_order
        );
    }
}

#endif // NUMCPP_TENSOR_VIEW_TCC_INCLUDED
