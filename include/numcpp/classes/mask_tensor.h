/*
 * File: include/numcpp/classes/mask_tensor.h
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

#ifndef NUMCPP_MASK_TENSOR_H_INCLUDED
#define NUMCPP_MASK_TENSOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/abstract_tensor.h"
#include "numcpp/iterators/index_sequence.h"
#include "numcpp/utilities/operators.h"

namespace numcpp {
/**
 * @brief A @ref mask_tensor is a view of a subset of elements from a
 * multidimensional array. It references the elements in the original array
 * through a boolean mask. The view itself does not own the data and any changes
 * made to the view will affect the original array, and any changes made to the
 * original array will affect the view.
 *
 * @tparam Tensor The tensor subclass whose elements are referenced.
 * @tparam BooleanMask The tensor subclass used for masking.
 */
template <class Tensor, class BooleanMask>
class mask_tensor {
  /// Member types.
  typedef typename Tensor::value_type value_type;
  typedef typename Tensor::reference reference;
  typedef typename Tensor::pointer pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 private:
  // Tensor elements being referenced.
  Tensor& m_data;

  // Boolean mask.
  const BooleanMask& m_mask;

 public:
  /// Constructors.

  /**
   * @brief View constructor. Constructs a @ref mask_tensor that references the
   * elements of a multidimensional array.
   *
   * @param data Tensor whose elements are referenced.
   * @param indices A boolean abstract tensor with the same shape as input
   * tensor and with its elements identifying whether each element is taken or
   * not.
   */
  mask_tensor(Tensor& data, const BooleanMask& mask)
      : m_data(data), m_mask(mask) {}

  /// Destructor.
  ~mask_tensor() = default;

  /// Public methods.

  /**
   * @brief Return the number of true values in the mask.
   */
  size_type count() const {
    size_type n = 0;
    for (const index_t<Tensor::rank>& i : make_index_sequence(m_data.shape())) {
      if (m_mask[i]) ++n;
    }
    return n;
  }

  /// Conversion operator.

  /**
   * @brief Return a new tensor with a copy of the selection.
   */
  template <class T>
  operator tensor<T, 1>() const {
    // Allocate memory for n elements and fill elements in order.
    tensor<T, 1> vec(this->count());
    T* ptr = vec.data();
    for (const index_t<Tensor::rank>& i : make_index_sequence(m_data.shape())) {
      if (m_mask[i]) {
        *ptr = m_data[i];
        ++ptr;
      }
    }
    return vec;
  }

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in other. Elements are iterated using
   * @ref default_layout order.
   *
   * @param other A tensor-like object of rank 1.
   *
   * @return *this
   */
  template <abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator=(const TensorLike& other) {
    index_t<1> j{0};
    for (const index_t<Tensor::rank>& i : make_index_sequence(m_data.shape())) {
      if (m_mask[i]) {
        m_data[i] = other[j];
        ++j[0];
      }
    }
    return *this;
  }

  /**
   * @brief Fill assignment. Assigns val to every element.
   *
   * @param val Value assigned to all the elements in the mask_tensor.
   *
   * @return *this
   */
  mask_tensor& operator=(const value_type& val) {
    for (const index_t<Tensor::rank>& i : make_index_sequence(m_data.shape())) {
      if (m_mask[i]) {
        m_data[i] = val;
      }
    }
    return *this;
  }

  /// Compound assignment operator.

  /**
   * @brief Assigns to *this the result of performing the respective operation
   * on all the elements in the tensor.
   *
   * When the right-hand side argument is a tensor object, the operation is
   * performed between the corresponding elements in each object (the first
   * element of the left one with the right one, the second with the second, and
   * so on...)
   *
   * When the right-hand side argument is a value, the operation is applied to
   * all the elements in the tensor against that value.
   *
   * @param rhs Right-hand side abstract tensor.
   * @param val Value to use as right-hand operand.
   *
   * @return *this
   */
  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator+=(const TensorLike& rhs) {
    return this->assign(plus(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator-=(const TensorLike& rhs) {
    return this->assign(minus(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator*=(const TensorLike& rhs) {
    return this->assign(multiplies(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator/=(const TensorLike& rhs) {
    return this->assign(divides(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator%=(const TensorLike& rhs) {
    return this->assign(modulus(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator&=(const TensorLike& rhs) {
    return this->assign(bit_and(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator|=(const TensorLike& rhs) {
    return this->assign(bit_or(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator^=(const TensorLike& rhs) {
    return this->assign(bit_xor(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator<<=(const TensorLike& rhs) {
    return this->assign(left_shift(), rhs);
  }

  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& operator>>=(const TensorLike& rhs) {
    return this->assign(right_shift(), rhs);
  }

  mask_tensor& operator+=(const value_type& val) {
    return this->assign(plus(), val);
  }

  mask_tensor& operator-=(const value_type& val) {
    return this->assign(minus(), val);
  }

  mask_tensor& operator*=(const value_type& val) {
    return this->assign(multiplies(), val);
  }

  mask_tensor& operator/=(const value_type& val) {
    return this->assign(divides(), val);
  }

  mask_tensor& operator%=(const value_type& val) {
    return this->assign(modulus(), val);
  }

  mask_tensor& operator&=(const value_type& val) {
    return this->assign(bit_and(), val);
  }

  mask_tensor& operator|=(const value_type& val) {
    return this->assign(bit_or(), val);
  }

  mask_tensor& operator^=(const value_type& val) {
    return this->assign(bit_xor(), val);
  }

  mask_tensor& operator<<=(const value_type& val) {
    return this->assign(left_shift(), val);
  }

  mask_tensor& operator>>=(const value_type& val) {
    return this->assign(right_shift(), val);
  }

 private:
  /**
   * @brief Compound assignment operator implementation.
   */
  template <class Operator, abstract_tensor TensorLike>
    requires(TensorLike::rank == 1)
  mask_tensor& assign(Operator op, const TensorLike& rhs) {
    index_t<1> j{0};
    for (const index_t<Tensor::rank>& i : make_index_sequence(m_data.shape())) {
      if (m_mask[i]) {
        m_data[i] = op(m_data[i], rhs[j]);
        ++j[0];
      }
    }
    return *this;
  }

  /**
   * @brief Compound assignment operator implementation.
   */
  template <class Operator>
  mask_tensor& assign(Operator op, const value_type& val) {
    for (const index_t<Tensor::rank>& i : make_index_sequence(m_data.shape())) {
      if (m_mask[i]) {
        m_data[i] = op(m_data[i], val);
      }
    }
    return *this;
  }
};
} // namespace numcpp

#endif // NUMCPP_MASK_TENSOR_H_INCLUDED
