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
 * @tparam MaskExpr The tensor subclass used for masking.
 */
template <class Tensor, class MaskExpr>
class mask_tensor {
 public:
  static_assert(Tensor::rank == MaskExpr::rank,
                "Expressions must have same dimension");

  /// Member types.
  typedef typename Tensor::value_type value_type;
  static constexpr size_t rank = 1;
  typedef typename Tensor::reference reference;
  typedef typename Tensor::pointer pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<1> shape_type;

 private:
  // Tensor elements being referenced.
  Tensor& m_data;

  // Boolean mask.
  const MaskExpr& m_mask;

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
  mask_tensor(Tensor& data, const MaskExpr& mask)
      : m_data(data), m_mask(mask) {}

  /// Destructor.
  ~mask_tensor() = default;

  /// Conversion operator.

  /**
   * @brief Return a new tensor with a copy of the selection.
   */
  template <class T>
  operator tensor<T, 1>() const {
    // Count true values in mask.
    size_t n = 0;
    for (auto i : make_index_sequence_for(m_data)) {
      if (m_mask[i]) {
        n++;
      }
    }
    // Allocate memory for n elements and fill elements in order.
    tensor<T, 1> vec(n);
    T* ptr = vec.data();
    for (auto i : make_index_sequence_for(m_data, default_layout)) {
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
  template <class Expr, class T>
  mask_tensor& operator=(const abstract_tensor<Expr, T, 1>& other) {
    const Expr& a = other.self();
    size_t n = 0;
    for (auto i : make_index_sequence_for(m_data, default_layout)) {
      if (m_mask[i]) {
        m_data[i] = a[{n++}];
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
    for (auto i : make_index_sequence_for(m_data)) {
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
  template <class Expr, class T>
  mask_tensor& operator+=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(plus(), rhs);
  }

  mask_tensor& operator+=(const value_type& val) {
    return __apply(plus(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator-=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(minus(), rhs);
  }

  mask_tensor& operator-=(const value_type& val) {
    return __apply(minus(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator*=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(multiplies(), rhs);
  }

  mask_tensor& operator*=(const value_type& val) {
    return __apply(multiplies(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator/=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(divides(), rhs);
  }

  mask_tensor& operator/=(const value_type& val) {
    return __apply(divides(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator%=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(modulus(), rhs);
  }

  mask_tensor& operator%=(const value_type& val) {
    return __apply(modulus(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator&=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(bit_and(), rhs);
  }

  mask_tensor& operator&=(const value_type& val) {
    return __apply(bit_and(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator|=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(bit_or(), rhs);
  }

  mask_tensor& operator|=(const value_type& val) {
    return __apply(bit_or(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator^=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(bit_xor(), rhs);
  }

  mask_tensor& operator^=(const value_type& val) {
    return __apply(bit_xor(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator<<=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(left_shift(), rhs);
  }

  mask_tensor& operator<<=(const value_type& val) {
    return __apply(left_shift(), val);
  }

  template <class Expr, class T>
  mask_tensor& operator>>=(const abstract_tensor<Expr, T, 1>& rhs) {
    return __apply(right_shift(), rhs);
  }

  mask_tensor& operator>>=(const value_type& val) {
    return __apply(right_shift(), val);
  }

 private:
  /**
   * @brief Applies op element-wise using *this as left hand operand and rhs
   * as right hand operand.
   */
  template <class Function, class Expr, class T>
  mask_tensor& __apply(Function op, const abstract_tensor<Expr, T, 1>& rhs) {
    const Expr& a = rhs.self();
    size_t n = 0;
    for (auto i : make_index_sequence_for(m_data, default_layout)) {
      if (m_mask[i]) {
        m_data[i] = op(m_data[i], a[{n++}]);
      }
    }
    return *this;
  }

  /**
   * @brief Applies op element-wise using *this as left hand operand and val
   * as right hand operand.
   */
  template <class Function>
  mask_tensor& __apply(Function op, const value_type& val) {
    for (auto i : make_index_sequence_for(m_data)) {
      if (m_mask[i]) {
        m_data[i] = op(m_data[i], val);
      }
    }
    return *this;
  }
};
} // namespace numcpp

#endif // NUMCPP_MASK_TENSOR_H_INCLUDED
