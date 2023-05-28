/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
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
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace numcpp {
/// Constructors.

template <class T, size_t Rank>
indirect_tensor<T, Rank>::indirect_tensor()
    : m_data(NULL), m_shape(), m_size(0), m_indptr(NULL),
      m_order(default_layout) {}

template <class T, size_t Rank>
template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...>,
          detail::RequiresIntegral<Sizes...>>
indirect_tensor<T, Rank>::indirect_tensor(T *data, Sizes... sizes)
    : m_data(data), m_shape(sizes...), m_size(m_shape.prod()),
      m_order(default_layout) {
  m_indptr = new size_type[m_size];
  std::iota(m_indptr, m_indptr + m_size, size_type(0));
}

template <class T, size_t Rank>
indirect_tensor<T, Rank>::indirect_tensor(T *data, const shape_type &shape,
                                          layout_t order)
    : m_data(data), m_shape(shape), m_size(shape.prod()), m_order(order) {
  m_indptr = new size_type[m_size];
  std::iota(m_indptr, m_indptr + m_size, size_type(0));
};

template <class T, size_t Rank>
template <class IntegralType, detail::RequiresIntegral<IntegralType>>
indirect_tensor<T, Rank>::indirect_tensor(T *data, const shape_type &shape,
                                          IntegralType *indptr, layout_t order)
    : m_data(data), m_shape(shape), m_size(shape.prod()), m_order(order) {
  m_indptr = new size_type[m_size];
  std::copy_n(indptr, m_size, m_indptr);
}

template <class T, size_t Rank>
indirect_tensor<T, Rank>::indirect_tensor(const indirect_tensor &other)
    : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
      m_order(other.m_order) {
  m_indptr = new size_type[m_size];
  std::copy_n(other.m_indptr, m_size, m_indptr);
}

template <class T, size_t Rank>
indirect_tensor<T, Rank>::indirect_tensor(indirect_tensor &&other)
    : m_data(other.m_data), m_shape(other.m_shape), m_size(other.m_size),
      m_indptr(other.m_indptr), m_order(other.m_order) {
  other.m_data = NULL;
  other.m_shape = shape_type();
  other.m_size = 0;
  other.m_indptr = NULL;
  other.m_order = default_layout;
}

/// Destructor.

template <class T, size_t Rank> indirect_tensor<T, Rank>::~indirect_tensor() {
  delete[] m_indptr;
}

/// Indexing.

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline T &indirect_tensor<T, Rank>::operator()(Indices... indices) {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline const T &indirect_tensor<T, Rank>::operator()(Indices... indices) const {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
inline T &indirect_tensor<T, Rank>::operator[](const index_type &index) {
  return m_data[this->iat(index)];
}

template <class T, size_t Rank>
inline const T &
indirect_tensor<T, Rank>::operator[](const index_type &index) const {
  return m_data[this->iat(index)];
}

#ifdef __cpp_multidimensional_subscript
template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
T &indirect_tensor<T, Rank>::operator[](Indices... indices) {
  return this->operator[](index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
const T &indirect_tensor<T, Rank>::operator[](Indices... indices) const {
  return this->operator[](index_type(indices...));
}
#endif // C++23

template <class T, size_t Rank>
inline size_t &indirect_tensor<T, Rank>::iat(const index_type &index) {
  detail::assert_within_bounds(m_shape, index);
  return m_indptr[ravel_index(index, m_shape, m_order)];
}

template <class T, size_t Rank>
inline const size_t &
indirect_tensor<T, Rank>::iat(const index_type &index) const {
  detail::assert_within_bounds(m_shape, index);
  return m_indptr[ravel_index(index, m_shape, m_order)];
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline size_t &indirect_tensor<T, Rank>::iat(Indices... indices) {
  return this->iat(index_type(indices...));
}

template <class T, size_t Rank>
template <class... Indices, detail::RequiresNArguments<Rank, Indices...>,
          detail::RequiresIntegral<Indices...>>
inline size_t &indirect_tensor<T, Rank>::iat(Indices... indices) const {
  return this->iat(index_type(indices...));
}

template <class T, size_t Rank>
inline const shape_t<Rank> &indirect_tensor<T, Rank>::shape() const {
  return m_shape;
}

template <class T, size_t Rank>
inline size_t indirect_tensor<T, Rank>::shape(size_type axis) const {
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

template <class T, size_t Rank> inline T *indirect_tensor<T, Rank>::data() {
  return m_data;
}

template <class T, size_t Rank>
inline const T *indirect_tensor<T, Rank>::data() const {
  return m_data;
}

template <class T, size_t Rank>
inline size_t *indirect_tensor<T, Rank>::indptr() {
  return m_indptr;
}

template <class T, size_t Rank>
inline const size_t *indirect_tensor<T, Rank>::indptr() const {
  return m_indptr;
}

template <class T, size_t Rank>
inline layout_t indirect_tensor<T, Rank>::layout() const {
  return m_order;
}

template <class T, size_t Rank>
inline bool indirect_tensor<T, Rank>::is_contiguous() const {
  return false;
}

/// Assignment operator.

template <class T, size_t Rank>
indirect_tensor<T, Rank> &
indirect_tensor<T, Rank>::operator=(const indirect_tensor &other) {
  if (this->shape() != other.shape()) {
    std::ostringstream error;
    error << "input shape " << other.shape()
          << " doesn't match the output shape " << this->shape();
    throw std::invalid_argument(error.str());
  }
  std::copy(other.begin(m_order), other.end(m_order), this->begin());
  return *this;
}

template <class T, size_t Rank>
template <class Container, class U>
indirect_tensor<T, Rank> &indirect_tensor<T, Rank>::operator=(
    const expression<Container, U, Rank> &other) {
  if (this->shape() != other.shape()) {
    std::ostringstream error;
    error << "input shape " << other.shape()
          << " doesn't match the output shape " << this->shape();
    throw std::invalid_argument(error.str());
  }
  std::transform(other.self().begin(m_order), other.self().end(m_order),
                 this->begin(), cast_to<U, T>());
  return *this;
}

template <class T, size_t Rank>
indirect_tensor<T, Rank> &indirect_tensor<T, Rank>::operator=(const T &val) {
  std::fill_n(this->begin(), this->size(), val);
  return *this;
}

template <class T, size_t Rank>
indirect_tensor<T, Rank> &
indirect_tensor<T, Rank>::operator=(indirect_tensor &&other) {
  if (this != &other) {
    delete[] m_indptr;
    m_data = other.m_data;
    m_shape = other.m_shape;
    m_size = other.m_size;
    m_indptr = other.m_indptr;
    m_order = other.m_order;
    other.m_data = NULL;
    other.m_shape = shape_type();
    other.m_size = 0;
    other.m_indptr = NULL;
    other.m_order = default_layout;
  }
  return *this;
}
} // namespace numcpp

#endif // NUMCPP_INDIRECT_TENSOR_TCC_INCLUDED
