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

 /** @file include/numcpp/tensor/tensor.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_TCC_INCLUDED
#define NUMCPP_TENSOR_TCC_INCLUDED

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace numcpp {
    /// Constructors.

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor()
     : m_data(NULL), m_shape(), m_size(0), m_order(row_major) {}

    template <class T, size_t Rank>
    template <class... Sizes,
              detail::RequiresNArguments<Rank, Sizes...>,
              detail::RequiresIntegral<Sizes...> >
    tensor<T, Rank>::base_tensor(Sizes... sizes)
     : m_shape(sizes...), m_size(m_shape.prod()), m_order(row_major)
    {
        m_data = new T[m_size];
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(const shape_t<Rank> &shape, layout_t order)
     : m_shape(shape), m_size(shape.prod()), m_order(order)
    {
        m_data = new T[m_size];
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(
        const shape_t<Rank> &shape, const T &val, layout_t order
    ) : m_shape(shape), m_size(shape.prod()), m_order(order)
    {
        m_data = new T[m_size];
        std::fill_n(m_data, m_size, val);
    }

    template <class T, size_t Rank>
    template <class InputIterator, class... Sizes,
              detail::RequiresInputIterator<InputIterator>,
              detail::RequiresNArguments<Rank, Sizes...>,
              detail::RequiresIntegral<Sizes...> >
    tensor<T, Rank>::base_tensor(InputIterator first, Sizes... sizes)
     : m_shape(sizes...), m_size(m_shape.prod()), m_order(row_major)
    {
        m_data = new T[m_size];
        std::copy_n(first, m_size, m_data);
    }

    template <class T, size_t Rank>
    template <class InputIterator,
              detail::RequiresInputIterator<InputIterator> >
    tensor<T, Rank>::base_tensor(
        InputIterator first, const shape_t<Rank> &shape, layout_t order
    ) : m_shape(shape), m_size(shape.prod()), m_order(order)
    {
        m_data = new T[m_size];
        std::copy_n(first, m_size, m_data);
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(const base_tensor &other)
     : m_shape(other.m_shape), m_size(other.m_size), m_order(other.m_order)
    {
        m_data = new T[m_size];
        std::copy_n(other.m_data, m_size, m_data);
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(const base_tensor &other, layout_t order)
     : m_shape(other.m_shape), m_size(other.m_size), m_order(order)
    {
        m_data = new T[m_size];
        std::copy_n(other.begin(m_order), m_size, m_data);
    }

    template <class T, size_t Rank>
    template <class U, class Tag>
    tensor<T, Rank>::base_tensor(const base_tensor<U, Rank, Tag> &other)
     : m_shape(other.shape()), m_size(other.size()), m_order(other.layout())
    {
        m_data = new T[m_size];
        std::transform(other.begin(), other.end(), m_data, cast_to<U, T>());
    }

    template <class T, size_t Rank>
    template <class U, class Tag>
    tensor<T, Rank>::base_tensor(
        const base_tensor<U, Rank, Tag> &other, layout_t order
    ) : m_shape(other.shape()), m_size(other.size()), m_order(order)
    {
        m_data = new T[m_size];
        std::transform(
            other.begin(m_order), other.end(m_order), m_data, cast_to<U, T>()
        );
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(base_tensor &&other)
     : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
       m_order(other.m_order)
    {
        other.m_data = NULL;
        other.m_shape = shape_t<Rank>();
        other.m_size = 0;
        other.m_order = row_major;
    }

    template <class T, size_t Rank>
    template <size_t Depth>
    typename std::enable_if<(Depth == 1)>::type
    tensor<T, Rank>::__initializer_list_shape(
        shape_t<Rank> &shape, std::initializer_list<T> il
    ) {
        shape[Rank - 1] = std::max(shape[Rank - 1], il.size());
    }

    template <class T, size_t Rank>
    template <size_t Depth>
    typename std::enable_if<(Depth > 1)>::type
    tensor<T, Rank>::__initializer_list_shape(
        shape_t<Rank> &shape, detail::nested_initializer_list_t<T, Depth> il
    ) {
        shape[Rank - Depth] = std::max(shape[Rank - Depth], il.size());
        for (auto it = il.begin(); it != il.end(); ++it) {
            __initializer_list_shape<Depth - 1>(shape, *it);
        }
    }

    template <class T, size_t Rank>
    template <size_t Depth>
    typename std::enable_if<(Depth == 1)>::type
    tensor<T, Rank>::__fill_from_initializer_list(
        index_t<Rank> &index, std::initializer_list<T> il
    ) {
        index[Rank - 1] = 0;
        for (const T *it = il.begin(); it != il.end(); ++it) {
            this->operator[](index) = *it;
            ++index[Rank - 1];
        }
    }

    template <class T, size_t Rank>
    template <size_t Depth>
    typename std::enable_if<(Depth > 1)>::type
    tensor<T, Rank>::__fill_from_initializer_list(
        index_t<Rank> &index, detail::nested_initializer_list_t<T, Depth> il
    ) {
        index[Rank - Depth] = 0;
        for (auto it = il.begin(); it != il.end(); ++it) {
            __fill_from_initializer_list<Depth - 1>(index, *it);
            ++index[Rank - Depth];
        }
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(detail::nested_initializer_list_t<T, Rank> il)
     : m_shape(), m_size(0), m_order(row_major) {
        __initializer_list_shape<Rank>(m_shape, il);
        m_size = m_shape.prod();
        m_data = new T[m_size]();
        index_t<Rank> index;
        __fill_from_initializer_list<Rank>(index, il);
    }

    /// Destructor.

    template <class T, size_t Rank>
    tensor<T, Rank>::~base_tensor() {
        delete[] m_data;
    }

    /// Indexing.

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline T& tensor<T, Rank>::operator()(Index... index) {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline const T& tensor<T, Rank>::operator()(Index... index) const {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    inline T& tensor<T, Rank>::operator[](const index_t<Rank> &index) {
        detail::assert_within_bounds(m_shape, index);
        return m_data[ravel_index(index, m_shape, m_order)];
    }

    template <class T, size_t Rank>
    inline const T& tensor<T, Rank>::operator[](const index_t<Rank> &index)
    const {
        detail::assert_within_bounds(m_shape, index);
        return m_data[ravel_index(index, m_shape, m_order)];
    }

#ifdef __cpp_multidimensional_subscript
    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline T& tensor<T, Rank>::operator[](Index... index) {
        return this->operator[](make_index(index...));
    }

    template <class T, size_t Rank>
    template <class... Index,
              detail::RequiresNArguments<Rank, Index...>,
              detail::RequiresIntegral<Index...> >
    inline const T& tensor<T, Rank>::operator[](Index... index) const {
        return this->operator[](make_index(index...));
    }
#endif // C++23

    template <class T, size_t Rank>
    template <size_t N>
    void tensor<T, Rank>::__unpack_slices(
        size_t&, shape_t<N>&, ptrdiff_t&, shape_t<N>&
    ) const {}

    template <class T, size_t Rank>
    template <size_t N, class... Indices>
    void tensor<T, Rank>::__unpack_slices(
        size_t &size, shape_t<N> &shape, ptrdiff_t &offset, shape_t<N> &strides,
        size_t i, Indices... indices
    ) const {
        size_t axis = Rank - 1 - sizeof...(Indices);
        detail::assert_within_bounds(m_shape, i, axis);
        if (m_order == row_major) {
            __unpack_slices(size, shape, offset, strides, indices...);
        }
        offset += i * size;
        size *= m_shape[axis];
        if (m_order == column_major) {
            __unpack_slices(size, shape, offset, strides, indices...);
        }
    }

    template <class T, size_t Rank>
    template <size_t N, class... Indices>
    void tensor<T, Rank>::__unpack_slices(
        size_t &size, shape_t<N> &shape, ptrdiff_t &offset, shape_t<N> &strides,
        slice slc, Indices... indices
    ) const {
        size_t axis = Rank - 1 - sizeof...(Indices);
        if (slc == slice()) {
            slc = slice(m_shape[axis]);
        }
        else if (slc.size() > 0 && slc.last() >= m_shape[axis]) {
            slc = slice(slc.start(), m_shape[axis], slc.stride());
        }
        size_t I = N - 1 - detail::slicing_rank<Indices...>::value;
        if (m_order == row_major) {
            __unpack_slices(size, shape, offset, strides, indices...);
        }
        shape[I] = slc.size();
        offset += slc.start() * size;
        strides[I] = slc.stride() * size;
        size *= m_shape[axis];
        if (m_order == column_major) {
            __unpack_slices(size, shape, offset, strides, indices...);
        }
    }

    template <class T, size_t Rank>
    template <class... Indices,
              detail::RequiresNArguments<Rank, Indices...>,
              detail::RequiresSlicing<Indices...> >
    tensor_view<T, detail::slicing_rank<Indices...>::value>
    tensor<T, Rank>::operator()(Indices... indices) {
        constexpr size_t N = detail::slicing_rank<Indices...>::value;
        size_t size = 1;
        ptrdiff_t offset = 0;
        shape_t<N> shape, strides;
        __unpack_slices(size, shape, offset, strides, indices...);
        return tensor_view<T, N>(m_data, shape, offset, strides, m_order);
    }

    template <class T, size_t Rank>
    template <class... Indices,
              detail::RequiresNArguments<Rank, Indices...>,
              detail::RequiresSlicing<Indices...> >
    tensor_view<const T, detail::slicing_rank<Indices...>::value>
    tensor<T, Rank>::operator()(Indices... indices) const {
        constexpr size_t N = detail::slicing_rank<Indices...>::value;
        size_t size = 1;
        ptrdiff_t offset = 0;
        shape_t<N> shape, strides;
        __unpack_slices(size, shape, offset, strides, indices...);
        return tensor_view<const T, N>(m_data, shape, offset, strides, m_order);
    }

#ifdef __cpp_multidimensional_subscript
    template <class T, size_t Rank>
    template <class... Indices,
              detail::RequiresNArguments<Rank, Indices...>,
              detail::RequiresSlicing<Indices...> >
    inline tensor_view<T, detail::slicing_rank<Indices...>::value>
    tensor<T, Rank>::operator[](Indices... indices) {
        return this->operator()(indices...);
    }

    template <class T, size_t Rank>
    template <class... Indices,
              detail::RequiresNArguments<Rank, Indices...>,
              detail::RequiresSlicing<Indices...> >
    inline tensor_view<const T, detail::slicing_rank<Indices...>::value>
    tensor<T, Rank>::operator[](Indices... indices) const {
        return this->operator()(indices...);
    }
#endif // C++23

    template <class T, size_t Rank>
    template <size_t N, class Tag>
    indirect_tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<index_t<Rank>, N, Tag> &indices
    ) {
        size_t *indptr = new size_t[indices.size()];
        size_t n = 0;
        for (index_t<N> i : make_index_sequence(indices.shape())) {
            detail::assert_within_bounds(m_shape, indices[i]);
            indptr[n++] = ravel_index(indices[i], m_shape, m_order);
        }
        return indirect_tensor<T, N>(
            m_data, indices.shape(), indptr, row_major, -1
        );
    }

    template <class T, size_t Rank>
    template <size_t N, class Tag>
    tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<index_t<Rank>, N, Tag> &indices
    ) const {
        tensor<T, N> subset(indices.shape());
        for (index_t<N> i : make_index_sequence(indices.shape())) {
            subset[i] = this->operator[](indices[i]);
        }
        return subset;
    }

    template <class T, size_t Rank>
    template <class IntegralType, size_t N, class Tag,
              detail::RequiresIntegral<IntegralType> >
    indirect_tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<IntegralType, N, Tag> &indices
    ) {
        static_assert(Rank == 1, "Input tensor must be 1-dimensional");
        size_t *indptr = new size_t[indices.size()];
        size_t n = 0;
        for (index_t<N> i : make_index_sequence(indices.shape())) {
            detail::assert_within_bounds(m_size, indices[i]);
            indptr[n++] = indices[i];
        }
        return indirect_tensor<T, N>(
            m_data, indices.shape(), indptr, row_major, -1
        );
    }

    template <class T, size_t Rank>
    template <class IntegralType, size_t N, class Tag,
              detail::RequiresIntegral<IntegralType> >
    tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<IntegralType, N, Tag> &indices
    ) const {
        static_assert(Rank == 1, "Input tensor must be 1-dimensional");
        tensor<T, N> subset(indices.shape());
        for (index_t<N> i : make_index_sequence(indices.shape())) {
            subset[i] = this->operator[](indices[i]);
        }
        return subset;
    }

    template <class T, size_t Rank>
    template <class Tag>
    indirect_tensor<T, 1> tensor<T, Rank>::operator[](
        const base_tensor<bool, Rank, Tag> &mask
    ) {
        if (m_shape != mask.shape()) {
            std::ostringstream error;
            error << "boolean index did not match indexed tensor; shape is "
                  << m_shape << " but corresponding boolean shape is "
                  << mask.shape();
            throw std::invalid_argument(error.str());
        }
        size_t size = std::count(mask.begin(), mask.end(), true);
        size_t *indptr = new size_t[size];
        size_t n = 0;
        for (index_t<Rank> i : make_index_sequence(mask.shape())) {
            if (mask[i]) {
                indptr[n++] = ravel_index(i, m_shape, m_order);
            }
        }
        return indirect_tensor<T, 1>(m_data, size, indptr, row_major, -1);
    }

    template <class T, size_t Rank>
    template <class Tag>
    tensor<T, 1> tensor<T, Rank>::operator[](
        const base_tensor<bool, Rank, Tag> &mask
    ) const {
        if (m_shape != mask.shape()) {
            std::ostringstream error;
            error << "boolean index did not match indexed tensor; shape is "
                  << m_shape << " but corresponding boolean shape is "
                  << mask.shape();
            throw std::invalid_argument(error.str());
        }
        size_t size = std::count(mask.begin(), mask.end(), true);
        tensor<T, 1> subset(size);
        size_t n = 0;
        for (index_t<Rank> i : make_index_sequence(mask.shape())) {
            if (mask[i]) {
                subset[n++] = this->operator[](i);
            }
        }
        return subset;
    }

    template <class T, size_t Rank>
    constexpr size_t tensor<T, Rank>::ndim() {
        return Rank;
    }

    template <class T, size_t Rank>
    inline const shape_t<Rank>& tensor<T, Rank>::shape() const {
        return m_shape;
    }

    template <class T, size_t Rank>
    inline size_t tensor<T, Rank>::shape(size_t axis) const {
        return m_shape[axis];
    }

    template <class T, size_t Rank>
    inline size_t tensor<T, Rank>::size() const {
        return m_size;
    }

    template <class T, size_t Rank>
    inline bool tensor<T, Rank>::empty() const {
        return (m_size == 0);
    }

    template <class T, size_t Rank>
    inline T* tensor<T, Rank>::data() {
        return m_data;
    }

    template <class T, size_t Rank>
    inline const T* tensor<T, Rank>::data() const {
        return m_data;
    }

    template <class T, size_t Rank>
    inline layout_t tensor<T, Rank>::layout() const {
        return m_order;
    }

    template <class T, size_t Rank>
    inline bool tensor<T, Rank>::is_contiguous() const {
        return true;
    }

    /// Assignment operator.

    template <class T, size_t Rank>
    tensor<T, Rank>& tensor<T, Rank>::operator=(const base_tensor &other) {
        this->resize(other.m_shape);
        std::copy_n(other.begin(m_order), m_size, m_data);
        return *this;
    }

    template <class T, size_t Rank>
    template <class U, class Tag>
    tensor<T, Rank>&
    tensor<T, Rank>::operator=(const base_tensor<U, Rank, Tag> &other) {
        this->resize(other.shape());
        std::transform(
            other.begin(m_order), other.end(m_order), m_data,
            cast_to<U, T>()
        );
        return *this;
    }

    template <class T, size_t Rank>
    tensor<T, Rank>& tensor<T, Rank>::operator=(const T &val) {
        std::fill_n(m_data, m_size, val);
        return *this;
    }

    template <class T, size_t Rank>
    tensor<T, Rank>& tensor<T, Rank>::operator=(base_tensor &&other) {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_size = other.m_size;
            m_shape = other.m_shape;
            m_order = other.m_order;
            other.m_data = NULL;
            other.m_size = 0;
            other.m_shape = shape_t<Rank>();
            other.m_order = row_major;
        }
        return *this;
    }

    template <class T, size_t Rank>
    tensor<T, Rank>& tensor<T, Rank>::operator=(
        detail::nested_initializer_list_t<T, Rank> il
    ) {
        shape_t<Rank> shape;
        __initializer_list_shape<Rank>(shape, il);
        this->resize(shape);
        index_t<Rank> index;
        __fill_from_initializer_list<Rank>(index, il);
        return *this;
    }

    /// Public methods.

    template <class T, size_t Rank>
    tensor_view<T, 1> tensor<T, Rank>::diagonal(ptrdiff_t k) {
        static_assert(Rank == 2, "Input tensor must be 2-dimensional");
        size_t size = 0, stride = 0;
        ptrdiff_t offset = 0;
        index_t<2> index = (k >= 0) ? make_index(0, k) : make_index(-k, 0);
        shape_t<2> m_stride = make_strides(m_shape, m_order);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            offset = index[0] * m_stride[0] + index[1] * m_stride[1];
            stride = m_stride[0] + m_stride[1];
        }
        return tensor_view<T, 1>(m_data, size, offset, stride);
    }

    template <class T, size_t Rank>
    tensor_view<const T, 1> tensor<T, Rank>::diagonal(ptrdiff_t k) const {
        static_assert(Rank == 2, "Input tensor must be 2-dimensional");
        size_t size = 0, stride = 0;
        ptrdiff_t offset = 0;
        index_t<2> index = (k >= 0) ? make_index(0, k) : make_index(-k, 0);
        shape_t<2> m_stride = make_strides(m_shape, m_order);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            offset = index[0] * m_stride[0] + index[1] * m_stride[1];
            stride = m_stride[0] + m_stride[1];
        }
        return tensor_view<const T, 1>(m_data, size, offset, stride);
    }

    template <class T, size_t Rank>
    inline tensor_view<T, 1> tensor<T, Rank>::flatten() {
        return tensor_view<T, 1>(m_data, m_size);
    }

    template <class T, size_t Rank>
    inline tensor_view<const T, 1> tensor<T, Rank>::flatten() const {
        return tensor_view<const T, 1>(m_data, m_size);
    }

    template <class T, size_t Rank>
    template <class... Sizes, detail::RequiresIntegral<Sizes...> >
    inline tensor_view<T, sizeof...(Sizes)>
    tensor<T, Rank>::reshape(Sizes... sizes) {
        return this->reshape(make_shape(sizes...));
    }

    template <class T, size_t Rank>
    template <class... Sizes, detail::RequiresIntegral<Sizes...> >
    inline tensor_view<const T, sizeof...(Sizes)>
    tensor<T, Rank>::reshape(Sizes... sizes) const {
        return this->reshape(make_shape(sizes...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline tensor_view<T, N>
    tensor<T, Rank>::reshape(const shape_t<N> &shape) {
        return this->reshape(shape, m_order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline tensor_view<const T, N>
    tensor<T, Rank>::reshape(const shape_t<N> &shape) const {
        return this->reshape(shape, m_order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<T, N> tensor<T, Rank>::reshape(
        const shape_t<N> &shape, layout_t order
    ) {
        if (m_size != shape.prod()) {
            std::ostringstream error;
            error << "cannot reshape tensor of shape " << m_shape
                  << " into shape " << shape;
            throw std::invalid_argument(error.str());
        }
        return tensor_view<T, N>(m_data, shape, order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<const T, N> tensor<T, Rank>::reshape(
        const shape_t<N> &shape, layout_t order
    ) const {
        if (m_size != shape.prod()) {
            std::ostringstream error;
            error << "cannot reshape tensor of shape " << m_shape
                  << " into shape " << shape;
            throw std::invalid_argument(error.str());
        }
        return tensor_view<const T, N>(m_data, shape, order);
    }

    template <class T, size_t Rank>
    template <class... Sizes,
              detail::RequiresNArguments<Rank, Sizes...>,
              detail::RequiresIntegral<Sizes...> >
    inline void tensor<T, Rank>::resize(Sizes... sizes) {
        this->resize(make_shape(sizes...));
    }

    template <class T, size_t Rank>
    inline void tensor<T, Rank>::resize(const shape_t<Rank> &shape) {
        if (m_size != shape.prod()) {
            delete[] m_data;
            m_size = shape.prod();
            m_data = new T[m_size];
        }
        m_shape = shape;
    }

    template <class T, size_t Rank>
    template <class... Axes, detail::RequiresIntegral<Axes...> >
    inline tensor_view<T, Rank - sizeof...(Axes)>
    tensor<T, Rank>::squeeze(Axes... axes) {
        return this->squeeze(make_shape(axes...));
    }

    template <class T, size_t Rank>
    template <class... Axes, detail::RequiresIntegral<Axes...> >
    inline tensor_view<const T, Rank - sizeof...(Axes)>
    tensor<T, Rank>::squeeze(Axes... axes) const {
        return this->squeeze(make_shape(axes...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<T, Rank - N> tensor<T, Rank>::squeeze(const shape_t<N> &axes) {
        static_assert(N < Rank, "squeeze cannot take more arguments than the"
                      " tensor dimension");
        shape_t<Rank - N> shape;
        bool keep_axis[Rank];
        std::fill_n(keep_axis, Rank, true);
        for (size_t i = 0; i < N; ++i) {
            keep_axis[axes[i]] = false;
        }
        size_t n = 0;
        for (size_t i = 0; i < Rank; ++i) {
            if (keep_axis[i]) {
                shape[n++] = m_shape[i];
            }
            else if (m_shape[i] != 1) {
                char error[] = "cannot select an axis to squeeze out which has"
                    " size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<T, Rank - N>(m_data, shape, m_order);
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<const T, Rank - N>
    tensor<T, Rank>::squeeze(const shape_t<N> &axes) const {
        static_assert(N < Rank, "squeeze cannot take more arguments than the"
                      " tensor dimension");
        shape_t<Rank - N> shape;
        bool keep_axis[Rank];
        std::fill_n(keep_axis, Rank, true);
        for (size_t i = 0; i < N; ++i) {
            keep_axis[axes[i]] = false;
        }
        size_t n = 0;
        for (size_t i = 0; i < Rank; ++i) {
            if (keep_axis[i]) {
                shape[n++] = m_shape[i];
            }
            else if (m_shape[i] != 1) {
                char error[] = "cannot select an axis to squeeze out which has"
                    " size not equal to one";
                throw std::invalid_argument(error);
            }
        }
        return tensor_view<const T, Rank - N>(m_data, shape, m_order);
    }

    template <class T, size_t Rank>
    inline tensor_view<T, Rank> tensor<T, Rank>::t() {
        shape_t<Rank> shape = m_shape;
        layout_t order = (m_order == row_major) ? column_major : row_major;
        std::reverse(shape.data(), shape.data() + Rank);
        return tensor_view<T, Rank>(m_data, shape, order);
    }

    template <class T, size_t Rank>
    inline tensor_view<const T, Rank> tensor<T, Rank>::t() const {
        shape_t<Rank> shape = m_shape;
        layout_t order = (m_order == row_major) ? column_major : row_major;
        std::reverse(shape.data(), shape.data() + Rank);
        return tensor_view<const T, Rank>(m_data, shape, order);
    }

    template <class T, size_t Rank>
    inline tensor_view<T, Rank> tensor<T, Rank>::view() {
        return tensor_view<T, Rank>(m_data, m_shape, m_order);
    }

    template <class T, size_t Rank>
    inline tensor_view<const T, Rank> tensor<T, Rank>::view() const {
        return tensor_view<const T, Rank>(m_data, m_shape, m_order);
    }
}

#endif // NUMCPP_TENSOR_TCC_INCLUDED
