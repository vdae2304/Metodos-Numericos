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
     : m_data(NULL), m_shape(), m_size(0), m_offset(0), m_stride(),
       m_order(row_major)
     {}

    template <class T, size_t Rank>
    template <class... Sizes,
              detail::RequiresNArguments<Rank, Sizes...>,
              detail::RequiresIntegral<Sizes...> >
    tensor_view<T, Rank>::base_tensor(T *data, Sizes... sizes)
     : m_data(data), m_shape(sizes...), m_size(m_shape.prod()),
       m_offset(0), m_stride(make_strides(m_shape)), m_order(row_major)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(
        T *data, const shape_t<Rank> &shape, layout_t order
    ) : m_data(data), m_shape(shape), m_size(shape.prod()),
        m_offset(0), m_stride(make_strides(shape, order)), m_order(order)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(
        T *data, const shape_t<Rank> &shape,
        ptrdiff_t offset, const shape_t<Rank> &strides,
        layout_t order
    ) : m_data(data), m_shape(shape), m_size(shape.prod()),
        m_offset(offset), m_stride(strides), m_order(order)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(const base_tensor &other)
     : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
       m_offset(other.m_offset), m_stride(other.m_stride),
       m_order(other.m_order)
     {}

    template <class T, size_t Rank>
    tensor_view<T, Rank>::base_tensor(base_tensor &&other)
     : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
       m_offset(other.m_offset), m_stride(other.m_stride),
       m_order(other.m_order)
    {
        other.m_data = NULL;
        other.m_shape = shape_t<Rank>();
        other.m_size = 0;
        other.m_offset = 0;
        other.m_stride = shape_t<Rank>();
        other.m_order = row_major;
    }

    /// Destructor.

    template <class T, size_t Rank>
    tensor_view<T, Rank>::~base_tensor() {}

    /// Indexing.

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline T& tensor_view<T, Rank>::operator()(Index... index) {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline const T& tensor_view<T, Rank>::operator()(Index... index) const {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    T& tensor_view<T, Rank>::operator[](const index_t<Rank> &index) {
        detail::assert_within_bounds(m_shape, index);
        ptrdiff_t flat_index = m_offset;
        for (size_t i = 0; i < Rank; ++i) {
            flat_index += index[i] * m_stride[i];
        }
        return m_data[flat_index];
    }

    template <class T, size_t Rank>
    const T& tensor_view<T, Rank>::operator[](const index_t<Rank> &index)
    const {
        detail::assert_within_bounds(m_shape, index);
        ptrdiff_t flat_index = m_offset;
        for (size_t i = 0; i < Rank; ++i) {
            flat_index += index[i] * m_stride[i];
        }
        return m_data[flat_index];
    }

#ifdef __cpp_multidimensional_subscript
    template <class T, size_t Rank>
    template <class... Index,
                detail::RequiresNArguments<Rank, Index...>,
                detail::RequiresIntegral<Index...> >
    inline T& tensor_view<T, Rank>::operator[](Index... index) {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    template <class... Index,
                detail::RequiresNArguments<Rank, Index...>,
                detail::RequiresIntegral<Index...> >
    inline const T& tensor_view<T, Rank>::operator[](Index... index) const {
        return this->operator[](make_index(index...));
    }
#endif // C++23

    template <class T, size_t Rank>
    constexpr size_t tensor_view<T, Rank>::ndim() {
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
    inline ptrdiff_t tensor_view<T, Rank>::offset() const {
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
    inline layout_t tensor_view<T, Rank>::layout() const {
        return m_order;
    }

    template <class T, size_t Rank>
    inline bool tensor_view<T, Rank>::is_contiguous() const {
        return (m_stride == make_strides(m_shape, m_order));
    }

    /// Assignment operator.

    template <class T, size_t Rank>
    tensor_view<T, Rank>&
    tensor_view<T, Rank>::operator=(const base_tensor &other) {
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
    template <class U, class Tag>
    tensor_view<T, Rank>&
    tensor_view<T, Rank>::operator=(const base_tensor<U, Rank, Tag> &other) {
        if (this->shape() != other.shape()) {
            std::ostringstream error;
            error << "input shape " << other.shape() << " doesn't match the "
                  << "output shape " << this->shape();
            throw std::invalid_argument(error.str());
        }
        std::transform(
            other.begin(m_order), other.end(m_order), this->begin(),
            cast_to<U, T>()
        );
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
            m_shape = other.m_shape;
            m_size = other.m_size;
            m_offset = other.m_offset;
            m_stride = other.m_stride;
            m_order = other.m_order;
            other.m_data = NULL;
            other.m_shape = shape_t<Rank>();
            other.m_size = 0;
            other.m_offset = 0;
            other.m_stride = shape_t<Rank>();
            other.m_order = row_major;
        }
        return *this;
    }

    /// Public methods.

    template <class T, size_t Rank>
    tensor_view<T, 1> tensor_view<T, Rank>::diagonal(ptrdiff_t k) {
        static_assert(Rank == 2, "Input tensor must be 2-dimensional");
        size_t size = 0, stride = 0;
        ptrdiff_t offset = 0;
        index_t<2> index = (k >= 0) ? make_index(0, k) : make_index(-k, 0);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            offset = m_offset + index[0] * m_stride[0] + index[1] * m_stride[1];
            stride = m_stride[0] + m_stride[1];
        }
        return tensor_view<T, 1>(m_data, size, offset, stride);
    }

    template <class T, size_t Rank>
    tensor_view<const T, 1> tensor_view<T, Rank>::diagonal(ptrdiff_t k)
    const {
        static_assert(Rank == 2, "Input tensor must be 2-dimensional");
        size_t size = 0, stride = 0;
        ptrdiff_t offset = 0;
        index_t<2> index = (k >= 0) ? make_index(0, k) : make_index(-k, 0);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            offset = m_offset + index[0] * m_stride[0] + index[1] * m_stride[1];
            stride = m_stride[0] + m_stride[1];
        }
        return tensor_view<const T, 1>(m_data, size, offset, stride);
    }

    template <class T, size_t Rank>
    inline tensor_view<T, 1> tensor_view<T, Rank>::flatten() {
        if (!this->is_contiguous()) {
            throw std::runtime_error("cannot flatten a non-contiguous view");
        }
        return tensor_view<T, 1>(m_data, m_size, m_offset, 1);
    }

    template <class T, size_t Rank>
    inline tensor_view<const T, 1> tensor_view<T, Rank>::flatten() const {
        if (!this->is_contiguous()) {
            throw std::runtime_error("cannot flatten a non-contiguous view");
        }
        return tensor_view<const T, 1>(m_data, m_size, m_offset, 1);
    }

    template <class T, size_t Rank>
    template <class... Sizes, detail::RequiresIntegral<Sizes...> >
    inline tensor_view<T, sizeof...(Sizes)>
    tensor_view<T, Rank>::reshape(Sizes... sizes) {
        return this->reshape(make_shape(sizes...));
    }

    template <class T, size_t Rank>
    template <class... Sizes, detail::RequiresIntegral<Sizes...> >
    inline tensor_view<const T, sizeof...(Sizes)>
    tensor_view<T, Rank>::reshape(Sizes... sizes) const {
        return this->reshape(make_shape(sizes...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline tensor_view<T, N>
    tensor_view<T, Rank>::reshape(const shape_t<N> &shape) {
        return this->reshape(shape, m_order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline tensor_view<const T, N>
    tensor_view<T, Rank>::reshape(const shape_t<N> &shape) const {
        return this->reshape(shape, m_order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<T, N> tensor_view<T, Rank>::reshape(
        const shape_t<N> &shape, layout_t order
    ) {
        if (this->size() != shape.prod()) {
            std::ostringstream error;
            error << "cannot reshape tensor of shape " << this->shape()
                  << " into shape " << shape;
            throw std::invalid_argument(error.str());
        }
        if (!this->is_contiguous()) {
            throw std::runtime_error("cannot reshape a non-contiguous view");
        }
        shape_t<N> strides = make_strides(shape, m_order);
        return tensor_view<T, N>(m_data, shape, m_offset, strides, order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<const T, N> tensor_view<T, Rank>::reshape(
        const shape_t<N> &shape, layout_t order
    ) const {
        if (this->size() != shape.prod()) {
            std::ostringstream error;
            error << "cannot reshape tensor of shape " << this->shape()
                  << " into shape " << shape;
            throw std::invalid_argument(error.str());
        }
        if (!this->is_contiguous()) {
            throw std::runtime_error("cannot reshape a non-contiguous view");
        }
        shape_t<N> strides = make_strides(shape, m_order);
        return tensor_view<const T, N>(m_data, shape, m_offset, strides, order);
    }

    template <class T, size_t Rank>
    template <class... Axes, detail::RequiresIntegral<Axes...> >
    inline tensor_view<T, Rank - sizeof...(Axes)>
    tensor_view<T, Rank>::squeeze(Axes... axes) {
        return this->squeeze(make_shape(axes...));
    }

    template <class T, size_t Rank>
    template <class... Axes, detail::RequiresIntegral<Axes...> >
    inline tensor_view<const T, Rank - sizeof...(Axes)>
    tensor_view<T, Rank>::squeeze(Axes... axes) const {
        return this->squeeze(make_shape(axes...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<T, Rank - N>
    tensor_view<T, Rank>::squeeze(const shape_t<N> &axes) {
        static_assert(N < Rank, "squeeze cannot take more arguments than the"
                      " tensor dimension");
        shape_t<Rank - N> shape, strides;
        bool keep_axis[Rank];
        std::fill_n(keep_axis, Rank, true);
        for (size_t i = 0; i < N; ++i) {
            keep_axis[axes[i]] = false;
        }
        size_t n = 0;
        for (size_t i = 0; i < Rank; ++i) {
            if (keep_axis[i]) {
                shape[n] = m_shape[i];
                strides[n++] = m_stride[i];
            }
            else if (m_shape[i] != 1) {
                char error[] = "cannot select an axis to squeeze out which has"
                    " size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<T, Rank - N>(
            m_data, shape, m_offset, strides, m_order
        );
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<const T, Rank - N>
    tensor_view<T, Rank>::squeeze(const shape_t<N> &axes) const {
        static_assert(N < Rank, "squeeze cannot take more arguments than the"
                      " tensor dimension");
        shape_t<Rank - N> shape, strides;
        bool keep_axis[Rank];
        std::fill_n(keep_axis, Rank, true);
        for (size_t i = 0; i < N; ++i) {
            keep_axis[axes[i]] = false;
        }
        size_t n = 0;
        for (size_t i = 0; i < Rank; ++i) {
            if (keep_axis[i]) {
                shape[n] = m_shape[i];
                strides[n++] = m_stride[i];
            }
            else if (m_shape[i] != 1) {
                char error[] = "cannot select an axis to squeeze out which has"
                    " size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<const T, Rank - N>(
            m_data, shape, m_offset, strides, m_order
        );
    }

    template <class T, size_t Rank>
    inline void tensor_view<T, Rank>::swapaxes(size_t axis1, size_t axis2) {
        std::swap(m_shape[axis1], m_shape[axis2]);
        std::swap(m_stride[axis1], m_stride[axis2]);
    }

    template <class T, size_t Rank>
    inline tensor_view<T, Rank> tensor_view<T, Rank>::t() {
        shape_t<Rank> shape = m_shape;
        shape_t<Rank> strides = m_stride;
        layout_t order = (m_order == row_major) ? column_major : row_major;
        std::reverse(shape.data(), shape.data() + Rank);
        std::reverse(strides.data(), strides.data() + Rank);
        return tensor_view<T, Rank>(m_data, shape, m_offset, strides, order);
    }

    template <class T, size_t Rank>
    inline tensor_view<const T, Rank> tensor_view<T, Rank>::t() const {
        shape_t<Rank> shape = m_shape;
        shape_t<Rank> strides = m_stride;
        layout_t order = (m_order == row_major) ? column_major : row_major;
        std::reverse(shape.data(), shape.data() + Rank);
        std::reverse(strides.data(), strides.data() + Rank);
        return tensor_view<const T, Rank>(
            m_data, shape, m_offset, strides, order
        );
    }
}

#endif // NUMCPP_TENSOR_VIEW_TCC_INCLUDED
