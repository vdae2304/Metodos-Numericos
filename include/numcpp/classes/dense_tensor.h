/*
 * File: include/numcpp/classes/dense_tensor.h
 * Repository: https://github.com/vdae2304/Metodos-Numericos
 * 
 * Copyright (C) 2026 vdae2304
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NUMCPP_DENSE_TENSOR_H_INCLUDED
#define NUMCPP_DENSE_TENSOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"
#include "numcpp/iterators/index_sequence.h"
#include "numcpp/iterators/flat_iterator.h"
#include "numcpp/utilities/operators.h"

namespace numcpp {
/**
 * @brief Base class for dense tensors such as tensor, tensor_view and
 * indirect_tensor. Unlike plain expressions, dense tensors allows to modify its
 * elements, and implements an assortment of methods which will be inherited to
 * all the subclasses.
 *
 * @tparam Derived Derived tensor subclass.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Derived, class T, size_t Rank>
class dense_tensor : public abstract_tensor<Derived, T, Rank> {
public:
  /// Member types.
  typedef flat_iterator<Derived, T&, T*> iterator;
  typedef flat_iterator<const Derived, const T&, const T*> const_iterator;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   * 
   * @param layout Memory layout in which elements are iterated. If set to
   * @ref layout_right, the last dimension is varying the fastest. If set to
   * @ref layout_left, the first dimension is varying the fastest. Defaults to
   * @ref default_layout.
   *
   * @return A random access iterator to the beginning of the tensor. If the
   * tensor is const-qualified, the function returns a const_iterator.
   * Otherwise, it returns an iterator.
   */
  iterator begin(layout_t layout = default_layout) {
    Derived* ptr = static_cast<Derived*>(this);
    return iterator(ptr, 0, layout);
  }

  const_iterator begin(layout_t layout = default_layout) const {
    return cbegin(layout);
  }

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   * 
   * @param layout Memory layout in which elements are iterated. If set to
   * @ref layout_right, the last dimension is varying the fastest. If set to
   * @ref layout_left, the first dimension is varying the fastest. Defaults to
   * @ref default_layout.
   *
   * @return A random access iterator to the beginning of the tensor. If the
   * tensor is const-qualified, the function returns a const_iterator.
   * Otherwise, it returns an iterator.
   */
  iterator end(layout_t layout = default_layout) {
    Derived* ptr = static_cast<Derived*>(this);
    return iterator(ptr, ptr->size(), layout);
  }

  const_iterator end(layout_t layout = default_layout) const {
    return cend(layout);
  }

  /**
   * @brief Return a const_iterator pointing to the first element in the tensor.
   * 
   * @param layout Memory layout in which elements are iterated. If set to
   * @ref layout_right, the last dimension is varying the fastest. If set to
   * @ref layout_left, the first dimension is varying the fastest. Defaults to
   * @ref default_layout.
   *
   * @return A const_iterator to the beginning of the tensor.
   */
  const_iterator cbegin(layout_t layout = default_layout) const {
    const Derived* ptr = static_cast<const Derived*>(this);
    return const_iterator(ptr, 0, layout);
  }

  /**
   * @brief Return a const_iterator pointing to the past-the-end element in the
   * 
   * @param layout Memory layout in which elements are iterated. If set to
   * @ref layout_right, the last dimension is varying the fastest. If set to
   * @ref layout_left, the first dimension is varying the fastest. Defaults to
   * @ref default_layout.
   *
   * @return A const_iterator to the element past the end of the tensor.
   */
  const_iterator cend(layout_t layout = default_layout) const {
    const Derived* ptr = static_cast<const Derived*>(this);
    return const_iterator(ptr, ptr->size(), layout);
  }

  /// Indexing.

  /**
   * @brief Return a reference to the element at the given position.
   */
  T& operator[](const index_t<Rank>& index) { return self()[index]; }

  const T& operator[](const index_t<Rank>& index) const {
    return self()[index];
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_t<Rank> shape() const { return self().shape(); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_t shape(size_t axis) const { return self().shape(axis); }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_t size() const { return self().size(); }

  /**
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return self().empty(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return self().layout(); }

  /**
   * @brief Return the derived subclass.
   */
  Derived& self() { return static_cast<Derived&>(*this); }
  const Derived& self() const { return static_cast<const Derived&>(*this); }

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in @a other.
   *
   * @param other An abstract tensor of the same rank.
   *
   * @return *this
   */
  template <class Expr>
  Derived& operator=(const abstract_tensor<Expr, T, Rank>& other) {
    Derived& self = this->self();
    const Expr& a = other.self();
    for (const index_t<Rank>& i : make_index_sequence_for(self)) {
      self[i] = a[i];
    }
    return self;
  }

  template <class Expr, class U>
  Derived& operator=(const abstract_tensor<Expr, U, Rank>& other) {
    Derived& self = this->self();
    const Expr& a = other.self();
    for (const index_t<Rank>& i : make_index_sequence_for(self)) {
      self[i] = static_cast<T>(a[i]);
    }
    return self;
  }

  /**
   * @brief Fill assignment. Assigns @a val to every element.
   *
   * @param val Value assigned to all the elements in the tensor.
   *
   * @return *this
   */
  Derived& operator=(const T& val) {
    Derived& self = this->self();
    for (const index_t<Rank>& i : make_index_sequence_for(self)) {
      self[i] = val;
    }
    return self;
  }

  /// Compound assignment operator.

  /**
   * @brief Assigns to *this the result of performing the respective operation
   * on all the elements in the tensor.
   *
   * When the right-hand side argument is a tensor, the operation is performed
   * between the corresponding elements in each object (the first element of
   * the left one with the right one, the second with the second, and so on...)
   *
   * When the right-hand side argument is a value, the operation is applied to
   * all the elements in the tensor against that value.
   *
   * @param rhs Right-hand side abstract tensor.
   * @param val Value to use as right-hand operand.
   *
   * @return *this
   */
  template <class Expr>
  Derived& operator+=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(plus(), rhs);
  }

  template <class Expr>
  Derived& operator-=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(minus(), rhs);
  }

  template <class Expr>
  Derived& operator*=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(multiplies(), rhs);
  }

  template <class Expr>
  Derived& operator/=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(divides(), rhs);
  }

  template <class Expr>
  Derived& operator%=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(modulus(), rhs);
  }

  template <class Expr>
  Derived& operator&=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(bit_and(), rhs);
  }

  template <class Expr>
  Derived& operator|=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(bit_or(), rhs);
  }

  template <class Expr>
  Derived& operator^=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(bit_xor(), rhs);
  }

  template <class Expr>
  Derived& operator<<=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(left_shift(), rhs);
  }

  template <class Expr>
  Derived& operator>>=(const abstract_tensor<Expr, T, Rank>& rhs) {
    return this->assign(right_shift(), rhs);
  }

  Derived& operator+=(const T& val) { return this->assign(plus(), val); }

  Derived& operator-=(const T& val) { return this->assign(minus(), val); }

  Derived& operator*=(const T& val) { return this->assign(multiplies(), val); }

  Derived& operator/=(const T& val) { return this->assign(divides(), val); }

  Derived& operator%=(const T& val) { return this->assign(modulus(), val); }

  Derived& operator&=(const T& val) { return this->assign(bit_and(), val); }

  Derived& operator|=(const T& val) { return this->assign(bit_or(), val); }

  Derived& operator^=(const T& val) { return this->assign(bit_xor(), val); }

  Derived& operator<<=(const T& val) { return this->assign(left_shift(), val); }

  Derived& operator>>=(const T& val) {
    return this->assign(right_shift(), val);
  }

 private:
  /**
   * @brief Compound assignment operator implementation.
   */
  template <class Operator, class Expr>
  Derived& assign(Operator op, const abstract_tensor<Expr, T, Rank>& rhs) {
    Derived& self = this->self();
    const Expr& a = rhs.self();
    for (const index_t<Rank>& i : make_index_sequence_for(self)) {
      self[i] = op(self[i], a[i]);
    }
    return self;
  }

  /**
   * @brief Compound assignment operator implementation.
   */
  template <class Operator>
  Derived& assign(Operator op, const T& val) {
    Derived& self = this->self();
    for (const index_t<Rank>& i : make_index_sequence_for(self)) {
      self[i] = op(self[i], val);
    }
    return self;
  }
};
} // namespace numcpp

#endif // NUMCPP_DENSE_TENSOR_H_INCLUDED
