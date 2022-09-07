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
    tensor<T, Rank>::base_tensor() : m_data(NULL), m_size(0), m_shape() {}

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(const shape_t<Rank> &shape)
     : m_size(shape.size()), m_shape(shape)
    {
        m_data = new T[m_size];
    }

    template <class T, size_t Rank>
    template <class... Args, typename, typename>
    tensor<T, Rank>::base_tensor(Args... args)
     : m_shape(args...)
    {
        m_size = m_shape.size();
        m_data = new T[m_size];
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(const shape_t<Rank> &shape, const T &val)
     : m_size(shape.size()), m_shape(shape)
    {
        m_data = new T[m_size];
        std::fill_n(m_data, m_size, val);
    }

    template <class T, size_t Rank>
    template <class InputIterator, typename>
    tensor<T, Rank>::base_tensor(
        InputIterator first, const shape_t<Rank> &shape
    ) : m_size(shape.size()), m_shape(shape)
    {
        m_data = new T[m_size];
        std::copy_n(first, m_size, m_data);
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(const base_tensor &other)
     : m_size(other.m_size), m_shape(other.m_shape)
    {
        m_data = new T[m_size];
        std::copy_n(other.m_data, m_size, m_data);
    }

    template <class T, size_t Rank>
    template <class U, class Tag>
    tensor<T, Rank>::base_tensor(const base_tensor<U, Rank, Tag> &other)
     : m_size(other.size()), m_shape(other.shape())
    {
        m_data = new T[m_size];
        std::transform(
            other.begin(row_major), other.end(row_major), m_data,
            cast_to<T>()
        );
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(base_tensor &&other)
     : m_data(other.m_data), m_size(other.m_size), m_shape(other.m_shape)
    {
        other.m_data = NULL;
        other.m_size = 0;
        other.m_shape = shape_t<Rank>();
    }

    template <class T, size_t Rank>
    template <size_t Depth>
    typename std::enable_if<(Depth == 1)>::type
    tensor<T, Rank>::__initializer_list_shape(
        shape_t<Rank> &shape, const std::initializer_list<T> &il
    ) {
        shape[Rank - 1] = std::max(shape[Rank - 1], il.size());
    }

    template <class T, size_t Rank>
    template <size_t Depth>
    typename std::enable_if<(Depth > 1)>::type
    tensor<T, Rank>::__initializer_list_shape(
        shape_t<Rank> &shape,
        const detail::nested_initializer_list_t<T, Depth> &il
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
        index_t<Rank> &index, const std::initializer_list<T> &il
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
        index_t<Rank> &index,
        const detail::nested_initializer_list_t<T, Depth> &il
    ) {
        index[Rank - Depth] = 0;
        for (auto it = il.begin(); it != il.end(); ++it) {
            __fill_from_initializer_list<Depth - 1>(index, *it);
            ++index[Rank - Depth];
        }
    }

    template <class T, size_t Rank>
    tensor<T, Rank>::base_tensor(
        detail::nested_initializer_list_t<T, Rank> il
    ) : m_shape() {
        __initializer_list_shape<Rank>(m_shape, il);
        m_size = m_shape.size();
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
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...> >
    inline T& tensor<T, Rank>::operator()(Args... args) {
        return this->operator[](make_index(args...));
    }

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...> >
    inline const T& tensor<T, Rank>::operator()(Args... args) const {
        return this->operator[](make_index(args...));
    }

    template <class T, size_t Rank>
    inline T& tensor<T, Rank>::operator[](const index_t<Rank> &index) {
        detail::assert_within_bounds(m_shape, index);
        return m_data[ravel_index(index, m_shape)];
    }

    template <class T, size_t Rank>
    inline const T& tensor<T, Rank>::operator[](const index_t<Rank> &index)
    const {
        detail::assert_within_bounds(m_shape, index);
        return m_data[ravel_index(index, m_shape)];
    }

    template <class T, size_t Rank>
    inline T& tensor<T, Rank>::operator[](size_t i) {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        detail::assert_within_bounds(m_size, i);
        return m_data[i];
    }

    template <class T, size_t Rank>
    inline const T& tensor<T, Rank>::operator[](size_t i) const {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        detail::assert_within_bounds(m_size, i);
        return m_data[i];
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline size_t tensor<T, Rank>::__unpack_slices(
        shape_t<N>&, size_t &offset, shape_t<N>&
    ) const {
        offset = 0;
        return 1;
    }

    template <class T, size_t Rank>
    template <size_t N, class... Args>
    size_t tensor<T, Rank>::__unpack_slices(
        shape_t<N> &shape, size_t &offset, shape_t<N> &strides,
        size_t i, Args... args
    ) const {
        size_t axis = Rank - sizeof...(Args) - 1;
        detail::assert_within_bounds(m_shape, i, axis);
        size_t n = __unpack_slices(shape, offset, strides, args...);
        offset += i * n;
        return m_shape[axis] * n;
    }

    template <class T, size_t Rank>
    template <size_t N, class... Args>
    size_t tensor<T, Rank>::__unpack_slices(
        shape_t<N> &shape, size_t &offset, shape_t<N> &strides,
        slice slc, Args... args
    ) const {
        size_t axis = Rank - sizeof...(Args) - 1;
        if (slc == slice()) {
            slc = slice(m_shape[axis]);
        }
        else if (slc.size() > 0 && slc.last() >= m_shape[axis]) {
            slc = slice(slc.start(), m_shape[axis], slc.stride());
        }
        size_t I = N - detail::slicing_rank<Args...>::value - 1;
        size_t n = __unpack_slices(shape, offset, strides, args...);
        shape[I] = slc.size();
        offset += slc.start() * n;
        strides[I] = slc.stride() * n;
        return m_shape[axis] * n;
    }

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresSlicing<Args...> >
    tensor_view<T, detail::slicing_rank<Args...>::value>
    tensor<T, Rank>::operator()(Args... args) {
        constexpr size_t N = detail::slicing_rank<Args...>::value;
        shape_t<N> shape, strides;
        size_t offset;
        __unpack_slices(shape, offset, strides, args...);
        return tensor_view<T, N>(shape, m_data, offset, strides);
    }

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresSlicing<Args...> >
    tensor_view<const T, detail::slicing_rank<Args...>::value>
    tensor<T, Rank>::operator()(Args... args) const {
        constexpr size_t N = detail::slicing_rank<Args...>::value;
        shape_t<N> shape, strides;
        size_t offset;
        __unpack_slices(shape, offset, strides, args...);
        return tensor_view<const T, N>(shape, m_data, offset, strides);
    }

    template <class T, size_t Rank>
    template <size_t N, class Tag>
    indirect_tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<index_t<Rank>, N, Tag> &indices
    ) {
        size_t *indptr = new size_t[indices.size()];
        size_t n = 0;
        for (index_t<N> i : make_indices(indices.shape())) {
            detail::assert_within_bounds(m_shape, indices[i]);
            indptr[n++] = ravel_index(indices[i], m_shape);
        }
        return indirect_tensor<T, N>(
            indices.shape(), m_data, indptr, row_major, -1
        );
    }

    template <class T, size_t Rank>
    template <size_t N, class Tag>
    tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<index_t<Rank>, N, Tag> &indices
    ) const {
        tensor<T, N> subset(indices.shape());
        for (index_t<N> i : make_indices(indices.shape())) {
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
        static_assert(Rank == 1, "Unkown conversion from integral type");
        size_t *indptr = new size_t[indices.size()];
        size_t n = 0;
        for (index_t<N> i : make_indices(indices.shape())) {
            detail::assert_within_bounds(m_size, indices[i]);
            indptr[n++] = indices[i];
        }
        return indirect_tensor<T, N>(
            indices.shape(), m_data, indptr, row_major, -1
        );
    }

    template <class T, size_t Rank>
    template <class IntegralType, size_t N, class Tag,
              detail::RequiresIntegral<IntegralType> >
    tensor<T, N> tensor<T, Rank>::operator[](
        const base_tensor<IntegralType, N, Tag> &indices
    ) const {
        static_assert(Rank == 1, "Unkown conversion from integral type");
        tensor<T, N> subset(indices.shape());
        for (index_t<N> i : make_indices(indices.shape())) {
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
        for (index_t<Rank> i : make_indices(mask.shape())) {
            if (mask[i]) {
                indptr[n++] = ravel_index(i, m_shape);
            }
        }
        return indirect_tensor<T, 1>(size, m_data, indptr, row_major, -1);
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
        for (index_t<Rank> i : make_indices(mask.shape())) {
            if (mask[i]) {
                subset[n++] = this->operator[](i);
            }
        }
        return subset;
    }

    template <class T, size_t Rank>
    constexpr size_t tensor<T, Rank>::ndim() const {
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
        return row_major;
    }

    /// Assignment operator.

    template <class T, size_t Rank>
    tensor<T, Rank>& tensor<T, Rank>::operator=(const base_tensor &other) {
        this->resize(other.m_shape);
        std::copy_n(other.m_data, m_size, m_data);
        return *this;
    }

    template <class T, size_t Rank>
    template <class U, class Tag>
    tensor<T, Rank>&
    tensor<T, Rank>::operator=(const base_tensor<U, Rank, Tag> &other) {
        this->resize(other.shape());
        std::transform(
            other.begin(row_major), other.end(row_major), m_data,
            cast_to<T>()
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
            other.m_data = NULL;
            other.m_size = 0;
            other.m_shape = shape_t<Rank>();
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
        static_assert(Rank == 2, "Input must be 2 dimensional");
        size_t size = 0, offset = 0, stride = 0;
        index_t<2> index = (k >= 0) ? make_index(0, k) : make_index(-k, 0);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            offset = ravel_index(index, m_shape);
            stride = m_shape[1] + 1;
        }
        return tensor_view<T, 1>(size, m_data, offset, stride);
    }

    template <class T, size_t Rank>
    tensor_view<const T, 1> tensor<T, Rank>::diagonal(ptrdiff_t k) const {
        static_assert(Rank == 2, "Input must be 2 dimensional");
        size_t size = 0, offset = 0, stride = 0;
        index_t<2> index = (k >= 0) ? make_index(0, k) : make_index(-k, 0);
        if (index[0] < m_shape[0] && index[1] < m_shape[1]) {
            size = std::min(m_shape[0] - index[0], m_shape[1] - index[1]);
            offset = ravel_index(index, m_shape);
            stride = m_shape[1] + 1;
        }
        return tensor_view<const T, 1>(size, m_data, offset, stride);
    }

    template <class T, size_t Rank>
    tensor_view<T, 1> tensor<T, Rank>::flatten() {
        return tensor_view<T, 1>(m_size, m_data);
    }

    template <class T, size_t Rank>
    tensor_view<const T, 1> tensor<T, Rank>::flatten() const {
        return tensor_view<const T, 1>(m_size, m_data);
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline tensor_view<T, N> tensor<T, Rank>::reshape(const shape_t<N> &shape) {
        if (m_size != shape.size()) {
            std::ostringstream error;
            error << "cannot reshape tensor of shape " << m_shape
                  << " into shape " << shape;
            throw std::invalid_argument(error.str());
        }
        return tensor_view<T, N>(shape, m_data);
    }

    template <class T, size_t Rank>
    template <class... Args, detail::RequiresIntegral<Args...> >
    inline tensor_view<T, sizeof...(Args)>
    tensor<T, Rank>::reshape(Args... args) {
        return this->reshape(make_shape(args...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    inline tensor_view<const T, N>
    tensor<T, Rank>::reshape(const shape_t<N> &shape) const {
        if (m_size != shape.size()) {
            std::ostringstream error;
            error << "cannot reshape tensor of shape " << m_shape
                  << " into shape " << shape;
            throw std::invalid_argument(error.str());
        }
        return tensor_view<const T, N>(shape, m_data);
    }

    template <class T, size_t Rank>
    template <class... Args, detail::RequiresIntegral<Args...> >
    inline tensor_view<const T, sizeof...(Args)>
    tensor<T, Rank>::reshape(Args... args) const {
        return this->reshape(make_shape(args...));
    }

    template <class T, size_t Rank>
    inline void tensor<T, Rank>::resize(const shape_t<Rank> &shape) {
        if (m_size != shape.size()) {
            delete[] m_data;
            m_size = shape.size();
            m_data = new T[m_size];
        }
        m_shape = shape;
    }

    template <class T, size_t Rank>
    template <class... Args,
              detail::RequiresNArguments<Rank, Args...>,
              detail::RequiresIntegral<Args...> >
    inline void tensor<T, Rank>::resize(Args... args) {
        this->resize(make_shape(args...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<T, Rank - N> tensor<T, Rank>::squeeze(const shape_t<N> &axes) {
        static_assert(N < Rank, "Reduction dimension must be less than tensor"
                      " dimension");
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
        return tensor_view<T, Rank - N>(shape, m_data);
    }

    template <class T, size_t Rank>
    template <class... Args, detail::RequiresIntegral<Args...> >
    inline tensor_view<T, Rank - sizeof...(Args)>
    tensor<T, Rank>::squeeze(Args... args) {
        return this->squeeze(make_shape(args...));
    }

    template <class T, size_t Rank>
    template <size_t N>
    tensor_view<const T, Rank - N>
    tensor<T, Rank>::squeeze(const shape_t<N> &axes) const {
        static_assert(N < Rank, "Reduction dimension must be less than tensor"
                      " dimension");
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
        return tensor_view<const T, Rank - N>(shape, m_data);
    }

    template <class T, size_t Rank>
    template <class... Args, detail::RequiresIntegral<Args...> >
    inline tensor_view<const T, Rank - sizeof...(Args)>
    tensor<T, Rank>::squeeze(Args... args) const {
        return this->squeeze(make_shape(args...));
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank> tensor<T, Rank>::t() {
        return tensor_view<T, Rank>(m_shape.transpose(), m_data, col_major);
    }

    template <class T, size_t Rank>
    tensor_view<const T, Rank> tensor<T, Rank>::t() const {
        return tensor_view<const T, Rank>(
            m_shape.transpose(), m_data, col_major
        );
    }

    template <class T, size_t Rank>
    tensor_view<T, Rank> tensor<T, Rank>::view() {
        return tensor_view<T, Rank>(m_shape, m_data);
    }

    template <class T, size_t Rank>
    tensor_view<const T, Rank> tensor<T, Rank>::view() const {
        return tensor_view<const T, Rank>(m_shape, m_data);
    }

    /// Matrix multiplication.

namespace detail {
    /**
     * @brief Helper function. Asserts that the corresponding axes are aligned
     * for matrix multiplication.
     */
    template <size_t Rank1, size_t Rank2>
    void assert_matmul_shapes(
        const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2
    ) {
        size_t axis1 = shape1.ndim() - 1;
        size_t axis2 = (shape2.ndim() >= 2) ? shape2.ndim() - 2 : 0;
        if (shape1[axis1] != shape2[axis2]) {
            std::ostringstream error;
            error << "shapes " << shape1 << " and " << shape2
                  << " not aligned: " << shape1[axis1] << " (dim " << axis1
                  << ") != " << shape2[axis2] << " (dim " << axis2 << ")";
            throw std::invalid_argument(error.str());
        }
    }

    /**
     * @brief Helper function. Returns the common shape of the first Rank - 2
     * dimensions.
     */
    template <size_t Rank>
    shape_t<Rank> broadcast_matmul(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2
    ) {
        shape_t<Rank> out_shape = shape1;
        for (size_t i = 0; i < shape1.ndim() - 2; ++i) {
            if (shape1[i] == 1) {
                out_shape[i] = shape2[i];
            }
            else if (shape2[i] != shape1[i] && shape2[i] != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shapes "
                      << shape1 << " " << shape2;
                throw std::invalid_argument(error.str());
            }
        }
        out_shape[shape1.ndim() - 2] = shape1[shape1.ndim() - 2];
        out_shape[shape2.ndim() - 1] = shape2[shape2.ndim() - 1];
        return out_shape;
    }

    /**
     * @brief Helper function. Reduce the shape dimension by removing an axis.
     */
    template <size_t Rank>
    shape_t<Rank - 1> remove_axis(const shape_t<Rank> &shape, size_t axis) {
        shape_t<Rank - 1> new_shape;
        for (size_t i = 0; i < axis; ++i) {
            new_shape[i] = shape[i];
        }
        for (size_t i = axis + 1; i < shape.ndim(); ++i) {
            new_shape[i - 1] = shape[i];
        }
        return new_shape;
    }

    /**
     * @brief Helper function. Expand the shape dimension by inserting a new
     * axis.
     */
    template <size_t Rank>
    shape_t<Rank + 1> insert_axis(
        const shape_t<Rank> &shape, size_t axis, size_t size = 0
    ) {
        shape_t<Rank + 1> new_shape;
        for (size_t i = 0; i < axis; ++i) {
            new_shape[i] = shape[i];
        }
        new_shape[axis] = size;
        for (size_t i = axis; i < shape.ndim(); ++i) {
            new_shape[i + 1] = shape[i];
        }
        return new_shape;
    }
}

    template <class T, class Tag1, class Tag2>
    T matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    ) {
        detail::assert_matmul_shapes(a.shape(), b.shape());
        size_t n = a.size();
        T val = 0;
        for (size_t i = 0; i < n; ++i) {
            val += a[i] * b[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 2> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    ) {
        detail::assert_matmul_shapes(a.shape(), b.shape());
        size_t m = a.shape(0), p = a.shape(1), n = b.shape(1);
        tensor<T, 2> out(m, n);
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = 0;
                for (size_t k = 0; k < p; ++k) {
                    val += a(i, k) * b(k, j);
                }
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank - 1> matmul(
        const base_tensor<T, Rank, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    ) {
        detail::assert_matmul_shapes(a.shape(), b.shape());
        size_t axis = a.ndim() - 1;
        size_t n = a.shape(axis);
        tensor<T, Rank - 1> out(detail::remove_axis(a.shape(), axis));
        for (index_t<Rank - 1> out_index : make_indices(out.shape())) {
            index_t<Rank> index = detail::insert_axis(out_index, axis);
            T val = 0;
            for (index[axis] = 0; index[axis] < n; ++index[axis]) {
                val += a[index] * b[index[axis]];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank - 1> matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, Rank, Tag2> &b
    ) {
        detail::assert_matmul_shapes(a.shape(), b.shape());
        size_t axis = b.ndim() - 2;
        size_t n = b.shape(axis);
        tensor<T, Rank - 1> out(detail::remove_axis(b.shape(), axis));
        for (index_t<Rank - 1> out_index : make_indices(out.shape())) {
            index_t<Rank> index = detail::insert_axis(out_index, axis);
            T val = 0;
            for (index[axis] = 0; index[axis] < n; ++index[axis]) {
                val += a[index[axis]] * b[index];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        detail::assert_matmul_shapes(a.shape(), b.shape());
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(detail::broadcast_matmul(a.shape(), b.shape()));
        for (index_t<Rank> out_index : make_indices(out.shape())) {
            index_t<Rank> a_index =
                detail::broadcast_index(out_index, a.shape());
            index_t<Rank> b_index =
                detail::broadcast_index(out_index, b.shape());
            T val = 0;
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }
}

#endif // NUMCPP_TENSOR_TCC_INCLUDED
