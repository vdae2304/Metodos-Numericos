/*
 * File: include/numcpp/tensor.h
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

#ifndef NUMCPP_TENSOR_H_INCLUDED
#define NUMCPP_TENSOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/classes/slice.h"
#include "numcpp/classes/dense_tensor.h"
#include "numcpp/tensor_view.h"
#include "numcpp/classes/indirect_tensor.h"
#include "numcpp/classes/mask_tensor.h"
#include "numcpp/expressions/unary_expr.h"
#include "numcpp/expressions/binary_expr.h"

#include <algorithm>

namespace numcpp {
/**
 * @brief Tensors are contiguous multidimensional sequence containers: they
 * hold a variable number of elements arranged in multiple axes. Unlike
 * @ref tensor_view, @ref tensor is always owner of its own data, which means
 * that the storage of the tensor is handled automatically.
 *
 * Tensors are designed to easily perform mathematical operations on them. Most
 * mathematical operations can be applied directly to tensor objects, including
 * arithmetic and comparison operators, affecting all its elements. It also
 * supports various forms of generalized subscript operators, slicing and
 * indirect access.
 *
 * @tparam T Type of the elements contained in the tensor. This shall be an
 * arithmetic type or a class that behaves like one (such as @ref std::complex).
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class T, size_t Rank>
class tensor : public dense_tensor<tensor<T, Rank>, T> {
 public:
  /// Member types.
  typedef T value_type;
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
   * @brief Default constructor. Constructs an empty tensor with no elements.
   */
  tensor() : m_shape{}, m_size(0), m_data(nullptr), m_layout(default_layout) {}

  /**
   * @brief Size constructor. Constructs a tensor with given shape, each element
   * is left uninitialized.
   *
   * @param shape Number of elements along each axis. It can be a @ref shape_t
   * object or the elements of the shape passed as separate arguments.
   * @param layout Memory layout in which elements are stored. If set to
   * @ref layout_right, the last dimension is contiguous. If set to
   * @ref layout_left, the first dimension is contiguous. Defaults to
   * @ref default_layout.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   * throw an exception.
   */
  template <std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  explicit tensor(Sizes... sizes)
      : m_shape{static_cast<size_type>(sizes)...},
        m_size(m_shape.prod()),
        m_data(new T[m_size]),
        m_layout(default_layout) {}

  explicit tensor(const shape_type& shape, layout_t layout = default_layout)
      : m_shape(shape),
        m_size(shape.prod()),
        m_data(new T[m_size]),
        m_layout(layout) {}

  /**
   * @brief Fill constructor. Constructs a tensor with given shape, each element
   * initialized to val.
   *
   * @param shape Number of elements along each axis.
   * @param val Value to which each of the elements is initialized.
   * @param layout Memory layout in which elements are stored. If set to
   * @ref layout_right, the last dimension is contiguous. If set to
   * @ref layout_left, the first dimension is contiguous. Defaults to
   * @ref default_layout.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   * throw an exception.
   */
  tensor(const shape_type& shape, const T& val,
         layout_t layout = default_layout)
      : tensor(shape, layout) {
    std::fill_n(m_data, m_size, val);
  }

  /**
   * @brief Range constructor. Constructs a tensor with given shape, with each
   * element constructed from its corresponding element in the range starting at
   * first, in the same order.
   *
   * @param first Input iterator to the initial position in a range.
   * @param shape Number of elements along each axis. It can be a @ref shape_t
   * object or the elements of the shape passed as separate arguments.
   * @param layout Memory layout in which elements are stored. If set to
   * @ref layout_right, the last dimension is contiguous. If set to
   * @ref layout_left, the first dimension is contiguous. Defaults to
   * @ref default_layout.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   * throw an exception.
   */
  template <std::input_iterator Iterator, std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  tensor(Iterator first, Sizes... sizes) : tensor(sizes...) {
    std::copy_n(first, m_size, m_data);
  }

  template <std::input_iterator Iterator>
  tensor(Iterator first, const shape_type& shape,
         layout_t layout = default_layout)
      : tensor(shape, layout) {
    std::copy_n(first, m_size, m_data);
  }

  /**
   * @brief Copy constructor. Constructs a tensor with a copy of each of the
   * elements in other, in the same order.
   *
   * @param other An abstract tensor of the same rank.
   * @param layout Memory layout in which elements are stored. If set to
   * @ref layout_right, the last dimension is contiguous. If set to
   * @ref layout_left, the first dimension is contiguous. Defaults to the same
   * layout as *this.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   * throw an exception.
   */
  tensor(const tensor& other) : tensor(other.m_shape, other.m_layout) {
    std::copy_n(other.m_data, m_size, m_data);
  }

  template <abstract_tensor TensorLike>
    requires(TensorLike::rank == Rank)
  tensor(const TensorLike& other) : tensor(other, other.layout()) {}

  template <abstract_tensor TensorLike>
    requires(TensorLike::rank == Rank)
  tensor(const TensorLike& other, layout_t layout)
      : tensor(other.shape(), layout) {
    dense_tensor<tensor, T>::operator=(other);
  }

  /**
   * @brief Move constructor. Constructs a tensor that acquires the elements of
   * other.
   *
   * @param other A tensor of the same type and rank. The ownership is directly
   * transferred from other. other is left in an empty state.
   */
  tensor(tensor&& other)
      : m_shape(other.m_shape),
        m_size(other.m_size),
        m_data(other.m_data),
        m_layout(other.m_layout) {
    other.m_shape = shape_type();
    other.m_size = 0;
    other.m_data = nullptr;
    other.m_layout = default_layout;
  }

  /**
   * @brief Initializer list constructor. Constructs a tensor with a copy of
   * each of the elements in @a il, in the same order.
   *
   * @param il An initializer_list object.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   * throw an exception.
   */
  tensor(typename detail::nested_initializer_list<T, Rank>::type il);

  /// Destructor.
  ~tensor() { delete[] m_data; }

  /// Indexing.

  /**
   * @brief Call operator. Return a reference to the element at the given
   * position.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The element at the specified position. If the tensor is
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
   * tensor. Since C++23, the elements of the index can be passed as separate
   * arguments.
   *
   * @return The element at the specified position. If the tensor is
   * const-qualified, the function returns a reference to const T. Otherwise, it
   * returns a reference to T.
   *
   * @note Undefined behaviour if index is out of bounds.
   */
  T& operator[](const index_type& index) {
    return m_data[ravel_index(index, m_shape, m_layout)];
  }

  const T& operator[](const index_type& index) const {
    return m_data[ravel_index(index, m_shape, m_layout)];
  }

#if __cplusplus >= 202302L
  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  T& operator[](Indices... indices) {
    return (*this)(indices...);
  }

  template <std::integral... Indices>
    requires(sizeof...(Indices) == Rank)
  const T& operator[](Indices... indices) const {
    return (*this)(indices...);
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
  template <integer_or_slice... Indices>
    requires((sizeof...(Indices) == Rank) &&
             (std::is_same_v<Indices, slice> || ...))
  tensor_view<T, detail::slicing_rank<Indices...>> operator()(
      const Indices&... indices) {
    constexpr size_t N = detail::slicing_rank<Indices...>;
    index_type m_stride = make_strides(m_shape, m_layout);
    shape_t<N> shape;
    index_t<N> strides;
    ptrdiff_t offset = __unpack_slices(m_shape, m_stride, shape.data(),
                                       strides.data(), indices...);
    return tensor_view<T, N>(m_data + offset, shape, strides);
  }

  template <integer_or_slice... Indices>
    requires((sizeof...(Indices) == Rank) &&
             (std::is_same_v<Indices, slice> || ...))
  tensor_view<const T, detail::slicing_rank<Indices...>> operator()(
      const Indices&... indices) const {
    constexpr size_t N = detail::slicing_rank<Indices...>;
    index_type m_stride = make_strides(m_shape, m_layout);
    shape_t<N> shape;
    index_t<N> strides;
    ptrdiff_t offset = __unpack_slices(m_shape, m_stride, shape.data(),
                                       strides.data(), indices...);
    return tensor_view<const T, N>(m_data + offset, shape, strides);
  }

#if __cplusplus >= 202302L
  template <integer_or_slice... Indices>
    requires((sizeof...(Indices) == Rank) &&
             (std::is_same_v<Indices, slice> || ...))
  tensor_view<T, detail::slicing_rank<Indices...>> operator[](
      const Indices&... indices) {
    return (*this)(indices...);
  }

  template <integer_or_slice... Indices>
    requires((sizeof...(Indices) == Rank) &&
             (std::is_same_v<Indices, slice> || ...))
  tensor_view<const T, detail::slicing_rank<Indices...>> operator[](
      const Indices&... indices) const {
    return (*this)(indices...);
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
  indirect_tensor<tensor, TensorLike> operator[](const TensorLike& indices) {
    return indirect_tensor<tensor, TensorLike>(*this, indices);
  }

  template <abstract_tensor TensorLike>
    requires(std::same_as<typename TensorLike::value_type, index_type>)
  indirect_tensor<const tensor, TensorLike> operator[](
      const TensorLike& indices) const {
    return indirect_tensor<const tensor, TensorLike>(*this, indices);
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
             TensorLike::rank == Rank)
  mask_tensor<tensor, TensorLike> operator[](const TensorLike& mask) {
    return mask_tensor<tensor, TensorLike>(*this, mask);
  }

  template <abstract_tensor TensorLike>
    requires(std::same_as<typename TensorLike::value_type, bool> &&
             TensorLike::rank == rank)
  tensor<T, 1> operator[](const TensorLike& mask) const {
    return mask_tensor<const tensor, TensorLike>(*this, mask);
  }

  /**
   * @brief Return the shape of the tensor.
   */
  const shape_type& shape() const { return m_shape; }

  /**
   * @brief Return the size of the tensor along the given axis.
   */
  size_type shape(size_type axis) const { return m_shape[axis]; }

  /**
   * @brief Return the number of elements in the tensor.
   */
  size_type size() const { return m_size; }

  /**
   * @brief Return whether the tensor is empty.
   */
  bool empty() const { return (m_size == 0); }

  /**
   * @brief Return a pointer to the memory array used internally by the tensor.
   * Because elements in the tensor are stored contiguously, the pointer
   * retrieved can be offset to access any element in the tensor.
   *
   * @return A pointer to the memory array used internally by the tensor. If the
   * tensor is const-qualified, the function returns a pointer to const T.
   * Otherwise, it returns a pointer to T.
   */
  T* data() { return m_data; }
  const T* data() const { return m_data; }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const { return m_layout; }

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in @a other, after resizing the object (if
   * necessary).
   *
   * @param other An abstract tensor of the same rank.
   *
   * @return *this
   *
   * @throw std::bad_alloc If the function needs to allocate storage and fails,
   * it may throw an exception.
   *
   * @warning When the shapes do not mach, invalidates all iterators, references
   * and views to elements of the tensor. Otherwise, valid iterators, references
   * and views keep their validity.
   */
  template <abstract_tensor TensorLike>
    requires(TensorLike::rank == Rank)
  tensor& operator=(const TensorLike& other) {
    this->resize(other.shape());
    dense_tensor<tensor, T>::operator=(other);
    return *this;
  }

  /**
   * @brief Fill assignment. Assigns @a val to every element. The size of the
   * tensor is preserved.
   *
   * @param val Value assigned to all the elements in the tensor.
   *
   * @return *this
   */
  tensor& operator=(const T& val) {
    std::fill_n(m_data, m_size, val);
    return *this;
  }

  /**
   * @brief Move assignment. Acquires the contents of @a other, leaving @a other
   * in an empty state.
   *
   * @param other A tensor of the same type and rank. The ownership is directly
   * transferred from @a other. @a other is left in an empty state.
   *
   * @return *this
   *
   * @warning Invalidates all iterators, references and views to elements of the
   * tensor.
   */
  tensor& operator=(tensor&& other) {
    if (this != &other) {
      delete[] m_data;
      m_shape = other.m_shape;
      m_size = other.m_size;
      m_data = other.m_data;
      m_layout = other.m_layout;
      other.m_shape = shape_type();
      other.m_size = 0;
      other.m_data = nullptr;
      other.m_layout = default_layout;
    }
    return *this;
  }

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
   * @return If the tensor is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor.
   */
  tensor_view<T, 1> flatten() { return tensor_view<T, 1>(m_data, m_size); }

  tensor_view<const T, 1> flatten() const {
    return tensor_view<const T, 1>(m_data, m_size);
  }

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
   * @return If the tensor is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor.
   *
   * @throw std::invalid_argument Thrown if the tensor could not reshaped.
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
   * @brief Resizes the tensor in-place to a given shape. If the new size is
   * different from the number of elements stored in the tensor, a reallocation
   * takes place to fit the new shape, losing the previous contents in the
   * process. Otherwise, the contents of the tensor are preserved, but arranged
   * to match the new shape.
   *
   * @param shape New shape of the tensor. It can be a shape_t object or the
   * elements of the shape passed as separate arguments.
   *
   * @warning Invalidates all iterators, references and views to elements of the
   * tensor.
   */
  template <std::integral... Sizes>
    requires(sizeof...(Sizes) == Rank)
  void resize(Sizes... sizes) {
    this->resize(shape_type{static_cast<size_type>(sizes)...});
  }

  void resize(const shape_type &shape);

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

  /**
   * @brief Return a view of the tensor with the same data.
   *
   * @return If the tensor is const-qualified, the function returns a
   * tensor_view to const T. Otherwise, the function returns a tensor_view to
   * T, which has reference semantics to the original tensor.
   */
  tensor_view<T, Rank> view() {
    return tensor_view<T, Rank>(m_data, m_shape, m_layout);
  }

  tensor_view<const T, Rank> view() const {
    return tensor_view<const T, Rank>(m_data, m_shape, m_layout);
  }

private:
  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Pointer to data.
  T *m_data;

  // Memory layout.
  layout_t m_layout;
};
} // namespace numcpp

#include "numcpp/classes/tensor.tcc"

#endif // NUMCPP_TENSOR_H_INCLUDED
