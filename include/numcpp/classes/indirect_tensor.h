/*
 * File: include/numcpp/classes/indirect_tensor.h
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

#ifndef NUMCPP_INDIRECT_TENSOR_H_INCLUDED
#define NUMCPP_INDIRECT_TENSOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/dense_tensor.h"

namespace numcpp {
/**
 * @brief An @ref indirect_tensor is a view of a subset of elements from a
 * multidimensional array. It references the elements in the original array
 * through an array of indices. The view itself does not own the data and any
 * changes made to the view will affect the original array, and any changes made
 * to the original array will affect the view.
 *
 * @tparam Tensor The tensor subclass whose elements are referenced.
 * @tparam IndexTensor The tensor subclass used for indexing.
 */
template <class Tensor, class IndexTensor>
class indirect_tensor
    : public dense_tensor<indirect_tensor<Tensor, IndexTensor>,
                          typename Tensor::value_type> {
 public:
  /// Member types.
  typedef typename Tensor::value_type value_type;
  static constexpr size_t rank = IndexTensor::rank;
  typedef typename Tensor::reference reference;
  typedef typename Tensor::pointer pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<rank> shape_type;
  typedef index_t<rank> index_type;

 private:
  // Tensor elements being referenced.
  Tensor& m_data;

  // Array of indices.
  const IndexTensor& m_index;

 public:
  /// Constructors.

  /**
   * @brief View constructor. Constructs an @ref indirect_tensor that references
   * the elements of a multidimensional array.
   *
   * @param data Tensor whose elements are referenced.
   * @param index An abstract tensor of @ref index_t with its elements
   * identifying which elements of the input tensor are selected.
   */
  indirect_tensor(Tensor& data, const IndexTensor& index)
      : m_data(data), m_index(index) {}

  /// Destructor.
  ~indirect_tensor() = default;

  /// Indexing.

  /**
   * @brief Call operator. Return a reference to the element at the given
   * position.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The element at the specified position.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) {
    return m_data[m_index(indices...)];
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == rank)
  decltype(auto) operator()(Indices... indices) const {
    return m_data[m_index(indices...)];
  }

  /**
   * @brief Subscript operator. Return a reference to the element at the given
   * position.
   *
   * @param index An @ref index_t object with the position of an element in the
   * @ref indirect_tensor.
   *
   * @return The element at the specified position.
   */
  decltype(auto) operator[](const index_type& index) {
    return m_data[m_index[index]];
  }

  decltype(auto) operator[](const index_type& index) const {
    return m_data[m_index[index]];
  }

  /**
   * @brief Return the shape of the tensor.
   */
  shape_type shape() const { return m_index.shape(); }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const { return m_index.shape(axis); }

  /**
   * @brief Return the number of elements in the @ref indirect_tensor.
   */
  size_type size() const { return m_index.size(); }

  /**
   * @brief Return whether the @ref indirect_tensor is empty.
   */
  bool empty() const { return m_index.empty(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_data.layout(); }

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in other.
   *
   * @param other A tensor-like object of the same rank.
   *
   * @return *this
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   * cannot be broadcasted according to broadcasting rules.
   */
  template <abstract_tensor TensorLike>
    requires(TensorLike::rank == rank)
  indirect_tensor& operator=(const TensorLike& other) {
    dense_tensor<indirect_tensor, value_type>::operator=(other);
    return *this;
  }

  /**
   * @brief Fill assignment. Assigns val to every element.
   *
   * @param val Value assigned to all the elements in the indirect_tensor.
   *
   * @return *this
   */
  indirect_tensor& operator=(const value_type& val) {
    dense_tensor<indirect_tensor, value_type>::operator=(val);
    return *this;
  }
};
} // namespace numcpp

#endif // NUMCPP_INDIRECT_TENSOR_H_INCLUDED
