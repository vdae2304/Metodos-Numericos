/*
 * File: include/numcpp/tensor_view.h
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

#ifndef NUMCPP_TENSOR_VIEW_H_INCLUDED
#define NUMCPP_TENSOR_VIEW_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/dense_tensor.h"
#include "numcpp/classes/indirect_tensor.h"
#include "numcpp/classes/mask_tensor.h"
#include "numcpp/expressions/unary_expr.h"
#include "numcpp/expressions/binary_expr.h"

namespace numcpp {
/**
 * @brief A @ref tensor_view is a view of a multidimensional array. It
 * references the elements in the original array. The view itself does not own
 * the data and any changes made to the view will affect the original array, and
 * any changes made to the original array will affect the view.
 *
 * @tparam T Type of the elements contained in the tensor. This shall be an
 * arithmetic type or a class that behaves like one (such as @ref std::complex).
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class T, size_t Rank>
class tensor_view
    : public dense_tensor<tensor_view<T, Rank>, std::remove_cv_t<T>, Rank> {
 public:
  /// Member types.
  typedef std::remove_cv_t<T> value_type;
  static constexpr size_t rank = Rank;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef shape_t<Rank> shape_type;
  typedef index_t<Rank> index_type;

  /// Constructors.

  /**
   * @brief Default constructor. Constructs a tensor_view that does not
   * reference any object.
   */
  tensor_view() : m_shape{}, m_size(0), m_data(nullptr), m_stride{} {}

  /**
   * @brief View constructor. Constructs a @ref tensor_view that references the
   * elements of a multidimensional array.
   *
   * @param data Pointer to the memory array used by the @ref tensor_view.
   * @param shape Number of elements along each axis. It can be a @ref shape_t
   * object or the elements of the shape passed as separate arguments.
   * @param layout Memory layout in which elements are stored. If set to
   * @ref layout_right, the last dimension is contiguous. If set to
   * @ref layout_left, the first dimension is contiguous. Defaults to
   * @ref default_layout.
   */
  template <std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  tensor_view(T* data, Sizes... sizes)
      : m_shape{static_cast<size_type>(sizes)...},
        m_size(m_shape.prod()),
        m_data(data),
        m_stride(make_strides(m_shape)) {}

  tensor_view(T* data, const shape_type& shape,
              layout_t layout = default_layout)
      : m_shape(shape),
        m_size(shape.prod()),
        m_data(data),
        m_stride(make_strides(shape, layout)) {}

  /**
   * @brief Stride constructor. Constructs a @ref tensor_view that references a
   * subset of elements from a multidimensional array.
   *
   * @param data Pointer to the memory array used by the @ref tensor_view.
   * @param shape Number of elements along each axis.
   * @param strides Span that separates the selected elements along each axis.
   */
  tensor_view(T* data, const shape_type& shape, const index_type& strides)
      : m_shape(shape),
        m_size(shape.prod()),
        m_data(data),
        m_stride(strides) {}

  /**
   * @brief Copy constructor. Constructs a tensor_view as a copy of @ref other.
   *
   * @param other A @ref tensor_view of the same type and rank.
   */
  tensor_view(const tensor_view& other) = default;
  tensor_view(tensor_view&& other) = default;

  /// Destructor.
  ~tensor_view() = default;

  /// Indexing.

  /**
   * @brief Call operator. Return a reference to the element at the given
   * position. The elements in a @ref tensor_view are given by
   *     data[index[0]*stride[0] + ... + index[rank-1]*stride[rank-1]]
   * where data is the memory array.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The element at the specified position. If the @ref tensor_view is
   * const-qualified, the function returns a reference to const T. Otherwise, it
   * returns a reference to T.
   *
   * @note Undefined behaviour if index is out of bounds.
   */
  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  T& operator()(Indices... indices) {
    return (*this)[index_type{static_cast<difference_type>(indices)...}];
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  const T& operator()(Indices... indices) const {
    return (*this)[index_type{static_cast<difference_type>(indices)...}];
  }

  /**
   * @brief Subscript operator. Return a reference to the element at the given
   * position.
   *
   * @param index An @ref index_t object with the position of an element in the
   * @ref tensor_view. Since C++23, the elements of the index can be passed as
   * separate arguments.
   *
   * @return The element at the specified position. If the @ref tensor_view is
   * const-qualified, the function returns a reference to const T. Otherwise, it
   * returns a reference to T.
   *
   * @note Undefined behaviour if index is out of bounds.
   */
  T& operator[](const index_type& index) {
    difference_type offset = 0;
    for (size_t i = 0; i < Rank; ++i) {
      offset += index[i] * m_stride[i];
    }
    return m_data[offset];
  }

  const T& operator[](const index_type& index) const {
    difference_type offset = 0;
    for (size_t i = 0; i < Rank; ++i) {
      offset += index[i] * m_stride[i];
    }
    return m_data[offset];
  }

#if __cplusplus >= 202302L
  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  T& operator[](Indices... indices) {
    return this->operator()(indices...);
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  const T& operator[](Indices... indices) const {
    return this->operator()(indices...);
  }
#endif  // C++23

  /**
   * @brief Slice indexing. Return a tensor_view object that selects the
   * elements specified by the slices.
   *
   * @param indices... Each argument can be either an integer or a slice. If an
   * integer is given, an unique position is selected for the axis and the
   * dimensionality of the returned view is reduced by 1. If a slice is given,
   * a subset of positions is selected along the axis
   *
   * @return If the tensor is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor. The dimension of
   * the returned view will equal the number of slice arguments.
   *
   * @throw std::out_of_range Thrown if an integral index is out of bounds.
   */
  template <class... Indices,
            detail::requires_all<detail::n_arguments<Rank, Indices...>,
                                 detail::has_slicing<Indices...> > = 0>
  tensor_view<T, detail::slicing_rank<Indices...>::value> operator()(
      const Indices&... indices);

  template <class... Indices,
            detail::requires_all<detail::n_arguments<Rank, Indices...>,
                                 detail::has_slicing<Indices...> > = 0>
  tensor_view<const T, detail::slicing_rank<Indices...>::value> operator()(
      const Indices&... indices) const;

#ifdef __cpp_multidimensional_subscript
  template <class... Indices,
            detail::requires_all<detail::n_arguments<Rank, Indices...>,
                                 detail::has_slicing<Indices...> > = 0>
  tensor_view<T, detail::slicing_rank<Indices...>::value> operator[](
      const Indices&... indices) {
    return this->operator()(indices...);
  }

  template <class... Indices,
            detail::requires_all<detail::n_arguments<Rank, Indices...>,
                                 detail::has_slicing<Indices...> > = 0>
  tensor_view<const T, detail::slicing_rank<Indices...>::value> operator[](
      const Indices&... indices) const {
    return this->operator()(indices...);
  }
#endif  // C++23

  /**
   * @brief Coordinate tensor indexing. Return an @ref indirect_tensor that
   * selects the elements specified by the tensor of indices.
   *
   * @param indices An abstract tensor of @ref index_t with its elements
   * identifying which elements of the tensor are selected.
   *
   * @return An @ref indirect_tensor, which has reference semantics to the
   * original tensor.
   */
  template <abstract_tensor TensorLike>
    requires(std::same_as<typename TensorLike::value_type, index_type>)
  indirect_tensor<tensor_view, TensorLike> operator[](
      const TensorLike& indices) {
    return indirect_tensor<tensor_view, TensorLike>(*this, indices);
  }

  template <abstract_tensor TensorLike>
    requires(std::same_as<typename TensorLike::value_type, index_type>)
  indirect_tensor<const tensor_view, TensorLike> operator[](
      const TensorLike& indices) const {
    return indirect_tensor<const tensor_view, TensorLike>(*this, indices);
  }

  /**
   * @brief Boolean tensor indexing. Return a @ref mask_tensor that selects
   * the elements specified by the boolean mask.
   *
   * @param mask An abstract tensor of @ref bool with its elements identifying
   * whether each element of the tensor is selected or not.
   *
   * @return If the tensor is const-qualified, the function returns a new tensor
   * object with a copy of the selection. Otherwise, the function returns a
   * @ref mask_tensor, which has reference semantics to the original tensor.
   *
   * @throw std::bad_alloc If the function needs to allocate storage and fails,
   * it may throw an exception.
   */
  template <abstract_tensor TensorLike>
    requires(std::same_as<typename TensorLike::value_type, bool> &&
             TensorLike::rank == rank)
  mask_tensor<tensor_view, TensorLike> operator[](const TensorLike& mask) {
    return mask_tensor<tensor_view, TensorLike>(*this, mask);
  }

  template <abstract_tensor TensorLike>
    requires(std::same_as<typename TensorLike::value_type, bool> &&
             TensorLike::rank == rank)
  tensor<T, 1> operator[](const TensorLike& mask) const {
    return mask_tensor<const tensor_view, TensorLike>(*this, mask);
  }

  /**
   * @brief Return the shape of the @ref tensor_view.
   */
  const shape_type& shape() const { return m_shape; }

  /**
   * @brief Return the size of the @ref tensor_view along the given axis.
   */
  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the @ref tensor_view.
   */
  size_type size() const { return m_size; }

  /**
   * @brief Return whether the @ref tensor_view is empty.
   */
  bool empty() const { return (m_size == 0); }

  /**
   * @brief Return a pointer to the memory array used internally by the
   * @ref tensor_view.
   *
   * @return A pointer to the memory array used internally by the
   * @ref tensor_view. If the @ref tensor_view is const-qualified, the function
   * returns a pointer to const T. Otherwise, it returns a pointer to T.
   */
  T* data() { return m_data; }
  const T* data() const { return m_data; }

  /**
   * @brief Return the span that separates the elements in the memory array.
   */
  const index_type& strides() const { return m_stride; }

  /**
   * @brief Return the span that separates the elements in the memory array
   * along the given axis.
   */
  difference_type strides(size_type axis) const { return m_stride[axis]; }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const;

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in @a other.
   *
   * @param other An abstract tensor of the same rank.
   *
   * @return *this
   */
  template <abstract_tensor TensorLike>
    requires(TensorLike::rank == Rank)
  tensor_view& operator=(const TensorLike& other) {
    dense_tensor<tensor_view, value_type, Rank>::operator=(other);
    return *this;
  }

  /**
   * @brief Fill assignment. Assigns @a val to every element.
   *
   * @param val Value assigned to all the elements in the tensor_view.
   *
   * @return *this
   */
  tensor_view &operator=(const T &val) {
    dense_tensor<tensor_view, value_type, Rank>::operator=(val);
    return *this;
  }

  /**
   * @brief Move assignment. Acquires the contents of @a other, leaving @a other
   * in an empty state.
   *
   * @param other A tensor_view of the same type and rank. @a other is left in
   * an empty state.
   *
   * @return *this
   */
  tensor_view& operator=(tensor_view&& other) = default;

  /// Public methods.

  /**
   * @brief Broadcast a tensor to a new shape.
   *
   * @details Return a readonly tensor_view of given shape constructed by
   * repeating elements along each axis of size one. Some of the elements in
   * the broadcasted tensor may refer to the same memory location.
   *
   * @param shape The shape of the desired tensor. It can be a shape_t object
   * or the elements of the shape passed as separate arguments.
   *
   * @return A tensor_view to const T.
   *
   * @throw std::invalid_argument Thrown if the tensor is not compatible with
   * the new shape according to broadcasting rules.
   */
  template <std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  tensor_view<const T, Rank> broadcast_to(Sizes... sizes) const {
    return this->broadcast_to(shape_type{static_cast<size_type>(sizes)...});
  }

  tensor_view<const T, Rank> broadcast_to(const shape_type& shape) const;

  /**
   * @brief Return a view of the tensor collapsed into one dimension.
   *
   * @return If the tensor_view is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor.
   *
   * @throw std::runtime_error Thrown if the elements in the view cannot be
   * flattened.
   */
  tensor_view<T, 1> flatten();
  tensor_view<const T, 1> flatten() const;

  /**
   * @brief Return a tensor_view containing the same data with a new shape.
   *
   * @param shape The new shape should be compatible with the original shape. It
   * can be a shape_t object or the elements of the shape passed as separate
   * arguments.
   * @param layout Memory layout in which elements are stored. If set to
   * @ref layout_right, the last dimension is contiguous. If set to
   * @ref layout_left, the first dimension is contiguous. Defaults to the same
   * layout as *this.
   *
   * @return If the tensor_view is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor.
   *
   * @throw std::invalid_argument Thrown if the tensor could not reshaped.
   * @throw std::runtime_error Thrown if the elements in the view are
   * non-contiguous.
   */
  template <std::integral... Sizes>
  tensor_view<T, sizeof...(Sizes)> reshape(Sizes... sizes) {
    return this->reshape(make_shape(sizes...));
  }

  template <std::integral... Sizes>
  tensor_view<const T, sizeof...(Sizes)> reshape(Sizes... sizes) const {
    return this->reshape(make_shape(sizes...));
  }

  template <size_t N>
  tensor_view<T, N> reshape(const shape_t<N>& shape,
                            layout_t layout = no_layout);

  template <size_t N>
  tensor_view<const T, N> reshape(const shape_t<N>& shape,
                                  layout_t layout = no_layout) const;

  /**
   * @brief Return a view of the tensor with its axes transposed.
   *
   * @param axes If specified, it must be a permutation of
   * {0, 1, ..., Rank - 1}. The @a i -th axis of the returned view corresponds
   * to the axis numbered @a axes[i] of the tensor. If not specified, reverses
   * the order of the axes.
   *
   * @return If the tensor_view is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor.
   */
  tensor_view<T, Rank> t();
  tensor_view<const T, Rank> t() const;

  template <std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  tensor_view<T, Rank> t(Sizes... axes) {
    return this->t(shape_type{static_cast<size_type>(axes)...});
  }

  template <std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  tensor_view<const T, Rank> t(Sizes... axes) const {
    return this->t(shape_type{static_cast<size_type>(axes)...});
  }

  tensor_view<T, Rank> t(const shape_type& axes);
  tensor_view<const T, Rank> t(const shape_type& axes) const;

 private:
  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Pointer to data.
  T *m_data;

  // Strides of data in memory.
  index_type m_stride;
};
} // namespace numcpp

#include "numcpp/classes/tensor_view.tcc"

#endif // NUMCPP_TENSOR_VIEW_H_INCLUDED
