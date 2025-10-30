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

/** @file include/numcpp/tensor/tensor_interface.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/tensor.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_DENSE_TENSOR_TCC_INCLUDED
#define NUMCPP_DENSE_TENSOR_TCC_INCLUDED

#include <algorithm>
#include "numcpp/broadcasting/assert.h"
#include "numcpp/iterators/axes_iterator.h"
#include "numcpp/iterators/index_sequence.h"
#include "numcpp/routines/ranges.h"

namespace numcpp {
/// Indexing.

template <class Derived, class T, size_t Rank>
inline T &
dense_tensor<Derived, T, Rank>::operator[](const index_t<Rank> &index) {
  return static_cast<Derived &>(*this)[index];
}

template <class Derived, class T, size_t Rank>
inline const T &
dense_tensor<Derived, T, Rank>::operator[](const index_t<Rank> &index) const {
  return static_cast<const Derived &>(*this)[index];
}

template <class Derived, class T, size_t Rank>
inline shape_t<Rank> dense_tensor<Derived, T, Rank>::shape() const {
  return static_cast<const Derived &>(*this).shape();
}

template <class Derived, class T, size_t Rank>
inline size_t dense_tensor<Derived, T, Rank>::shape(size_t axis) const {
  return static_cast<const Derived &>(*this).shape(axis);
}

template <class Derived, class T, size_t Rank>
inline size_t dense_tensor<Derived, T, Rank>::size() const {
  return static_cast<const Derived &>(*this).size();
}

template <class Derived, class T, size_t Rank>
inline layout_t dense_tensor<Derived, T, Rank>::layout() const {
  return static_cast<const Derived &>(*this).layout();
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::self() {
  return static_cast<Derived &>(*this);
}

template <class Derived, class T, size_t Rank>
inline const Derived &dense_tensor<Derived, T, Rank>::self() const {
  return static_cast<const Derived &>(*this);
}

/// Iterators.

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::iterator
dense_tensor<Derived, T, Rank>::begin() {
  return this->begin(this->layout());
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::begin() const {
  return this->begin(this->layout());
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::iterator
dense_tensor<Derived, T, Rank>::begin(layout_t order) {
  return iterator(static_cast<Derived *>(this), 0, order);
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::begin(layout_t order) const {
  return const_iterator(static_cast<const Derived *>(this), 0, order);
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::iterator
dense_tensor<Derived, T, Rank>::end() {
  return this->end(this->layout());
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::end() const {
  return this->end(this->layout());
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::iterator
dense_tensor<Derived, T, Rank>::end(layout_t order) {
  return iterator(static_cast<Derived *>(this), this->size(), order);
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::end(layout_t order) const {
  return const_iterator(static_cast<const Derived *>(this), this->size(),
                        order);
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::cbegin() const {
  return this->begin();
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::cbegin(layout_t order) const {
  return this->begin(order);
};

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::cend() const {
  return this->end();
}

template <class Derived, class T, size_t Rank>
inline typename dense_tensor<Derived, T, Rank>::const_iterator
dense_tensor<Derived, T, Rank>::cend(layout_t order) const {
  return this->end(order);
}

/// Assignment operator.

template <class Derived, class T, size_t Rank>
template <class Expr, class U>
Derived &dense_tensor<Derived, T, Rank>::operator=(
    const abstract_tensor<Expr, U, Rank> &other) {
  Derived &self = this->self();
  detail::assert_output_shape(self.shape(),
                              broadcast_shapes(self.shape(), other.shape()));
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i] = other[detail::broadcast_index(i, other.shape())];
  }
  return self;
}

template <class Derived, class T, size_t Rank>
Derived &dense_tensor<Derived, T, Rank>::operator=(const T &val) {
  Derived &self = this->self();
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i] = val;
  }
  return self;
}

/// Compound assignment operator.

template <class Derived, class T, size_t Rank>
template <class Function, class Expr>
Derived &dense_tensor<Derived, T, Rank>::apply2(
    Function f, const abstract_tensor<Expr, T, Rank> &rhs) {
  Derived &self = this->self();
  detail::assert_output_shape(self.shape(),
                              broadcast_shapes(self.shape(), rhs.shape()));
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i] = f(self[i], rhs[detail::broadcast_index(i, rhs.shape())]);
  }
  return self;
}

template <class Derived, class T, size_t Rank>
template <class Function>
Derived &dense_tensor<Derived, T, Rank>::apply2(Function f, const T &val) {
  Derived &self = this->self();
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i] = f(self[i], val);
  }
  return self;
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator+=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(plus(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator-=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(minus(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator*=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(multiplies(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator/=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(divides(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator%=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(modulus(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator&=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(bit_and(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator|=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(bit_or(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator^=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(bit_xor(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator<<=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(left_shift(), rhs);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
inline Derived &dense_tensor<Derived, T, Rank>::operator>>=(
    const abstract_tensor<Expr, T, Rank> &rhs) {
  return this->apply2(right_shift(), rhs);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator+=(const T &val) {
  return this->apply2(plus(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator-=(const T &val) {
  return this->apply2(minus(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator*=(const T &val) {
  return this->apply2(multiplies(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator/=(const T &val) {
  return this->apply2(divides(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator%=(const T &val) {
  return this->apply2(modulus(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator&=(const T &val) {
  return this->apply2(bit_and(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator|=(const T &val) {
  return this->apply2(bit_or(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator^=(const T &val) {
  return this->apply2(bit_xor(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator<<=(const T &val) {
  return this->apply2(left_shift(), val);
}

template <class Derived, class T, size_t Rank>
inline Derived &dense_tensor<Derived, T, Rank>::operator>>=(const T &val) {
  return this->apply2(right_shift(), val);
}

/// Public methods.

template <class Derived, class T, size_t Rank>
inline void dense_tensor<Derived, T, Rank>::apply(T f(T)) {
  std::transform(this->begin(), this->end(), this->begin(), f);
}

template <class Derived, class T, size_t Rank>
inline void dense_tensor<Derived, T, Rank>::apply(T f(const T &)) {
  std::transform(this->begin(), this->end(), this->begin(), f);
}

template <class Derived, class T, size_t Rank>
template <class Function>
inline void dense_tensor<Derived, T, Rank>::apply(Function &&f) {
  std::transform(this->begin(), this->end(), this->begin(),
                 std::forward<Function>(f));
}

template <class Derived, class T, size_t Rank>
template <class U>
inline unary_expr<cast_to<T, U>, Derived>
dense_tensor<Derived, T, Rank>::astype() const {
  return unary_expr<cast_to<T, U>, Derived>(this->self());
}

template <class Derived, class T, size_t Rank>
inline void dense_tensor<Derived, T, Rank>::clamp(const T &a_min,
                                                  const T &a_max) {
  this->apply(ranges::clamp<T>(a_min, a_max));
}

template <class Derived, class T, size_t Rank>
inline void dense_tensor<Derived, T, Rank>::partition(size_t kth, size_t axis) {
  this->partition(kth, axis, less());
}

template <class Derived, class T, size_t Rank>
template <class Compare, detail::RequiresCallable<Compare, T, T>>
void dense_tensor<Derived, T, Rank>::partition(size_t kth, size_t axis,
                                               Compare comp) {
  Derived &self = this->self();
  shape_t<Rank> shape = self.shape();
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    axes_iterator<Derived, T, Rank, 1> first(&self, index, axis, 0);
    axes_iterator<Derived, T, Rank, 1> last(&self, index, axis, size);
    std::nth_element(first, first + kth, last, comp);
  }
}

template <class Derived, class T, size_t Rank>
void dense_tensor<Derived, T, Rank>::reverse(size_t axis) {
  Derived &self = this->self();
  shape_t<Rank> shape = self.shape();
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    axes_iterator<Derived, T, Rank, 1> first(&self, index, axis, 0);
    axes_iterator<Derived, T, Rank, 1> last(&self, index, axis, size);
    std::reverse(first, last);
  }
}

template <class Derived, class T, size_t Rank>
void dense_tensor<Derived, T, Rank>::rotate(size_t shift, size_t axis) {
  Derived &self = this->self();
  shape_t<Rank> shape = self.shape();
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    axes_iterator<Derived, T, Rank, 1> first(&self, index, axis, 0);
    axes_iterator<Derived, T, Rank, 1> last(&self, index, axis, size);
    std::rotate(first, first + shift, last);
  }
}

template <class Derived, class T, size_t Rank>
inline void dense_tensor<Derived, T, Rank>::sort(size_t axis) {
  this->sort(axis, less());
}

template <class Derived, class T, size_t Rank>
template <class Compare, detail::RequiresCallable<Compare, T, T>>
void dense_tensor<Derived, T, Rank>::sort(size_t axis, Compare comp,
                                          bool stable) {
  Derived &self = this->self();
  shape_t<Rank> shape = self.shape();
  size_t size = shape[axis];
  shape[axis] = 1;
  for (index_t<Rank> index : make_index_sequence(shape)) {
    axes_iterator<Derived, T, Rank, 1> first(&self, index, axis, 0);
    axes_iterator<Derived, T, Rank, 1> last(&self, index, axis, size);
    if (stable) {
      std::stable_sort(first, last, comp);
    } else {
      std::sort(first, last, comp);
    }
  }
}

/// Reductions.

template <class Derived, class T, size_t Rank>
inline bool dense_tensor<Derived, T, Rank>::all() const {
  ranges::all pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline bool dense_tensor<Derived, T, Rank>::any() const {
  ranges::any pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline index_t<Rank> dense_tensor<Derived, T, Rank>::argmax() const {
  ranges::argmax pred;
  size_t index = pred(this->begin(), this->end());
  return unravel_index(index, this->shape(), this->layout());
}

template <class Derived, class T, size_t Rank>
inline index_t<Rank> dense_tensor<Derived, T, Rank>::argmin() const {
  ranges::argmin pred;
  size_t index = pred(this->begin(), this->end());
  return unravel_index(index, this->shape(), this->layout());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::max() const {
  ranges::max pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::mean() const {
  ranges::mean pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::min() const {
  ranges::min pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::prod() const {
  ranges::prod pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::stddev(bool bias) const {
  ranges::stddev pred(bias);
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::sum() const {
  ranges::sum pred;
  return pred(this->begin(), this->end());
}

template <class Derived, class T, size_t Rank>
inline T dense_tensor<Derived, T, Rank>::var(bool bias) const {
  ranges::var pred(bias);
  return pred(this->begin(), this->end());
}

/// Complex numbers.

template <class Derived, class T, size_t Rank>
inline unary_expr<math::real, Derived>
tensor_specialization<Derived, std::complex<T>, Rank>::real() const {
  const Derived &self = static_cast<const Derived &>(*this);
  return unary_expr<math::real, Derived>(self);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
void tensor_specialization<Derived, std::complex<T>, Rank>::real(
    const abstract_tensor<Expr, T, Rank> &x) {
  Derived &self = static_cast<Derived &>(*this);
  detail::assert_output_shape(self.shape(),
                              broadcast_shapes(self.shape(), x.shape()));
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i].real(x[detail::broadcast_index(i, x.shape())]);
  }
}

template <class Derived, class T, size_t Rank>
void tensor_specialization<Derived, std::complex<T>, Rank>::real(const T &val) {
  Derived &self = static_cast<Derived &>(*this);
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i].real(val);
  }
}

template <class Derived, class T, size_t Rank>
inline unary_expr<math::imag, Derived>
tensor_specialization<Derived, std::complex<T>, Rank>::imag() const {
  const Derived &self = static_cast<const Derived &>(*this);
  return unary_expr<math::imag, Derived>(self);
}

template <class Derived, class T, size_t Rank>
template <class Expr>
void tensor_specialization<Derived, std::complex<T>, Rank>::imag(
    const abstract_tensor<Expr, T, Rank> &y) {
  Derived &self = static_cast<Derived &>(*this);
  detail::assert_output_shape(self.shape(),
                              broadcast_shapes(self.shape(), y.shape()));
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i].imag(y[detail::broadcast_index(i, y.shape())]);
  }
}

template <class Derived, class T, size_t Rank>
void tensor_specialization<Derived, std::complex<T>, Rank>::imag(const T &val) {
  Derived &self = static_cast<Derived &>(*this);
  for (index_t<Rank> i : make_index_sequence_for(self)) {
    self[i].imag(val);
  }
}

template <class Derived, class T, size_t Rank>
inline unary_expr<math::conj, Derived>
tensor_specialization<Derived, std::complex<T>, Rank>::conj() const {
  const Derived &self = static_cast<const Derived &>(*this);
  return unary_expr<math::conj, Derived>(self);
}
} // namespace numcpp

#endif // NUMCPP_DENSE_TENSOR_TCC_INCLUDED
