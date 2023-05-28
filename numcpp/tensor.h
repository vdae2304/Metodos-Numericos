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

/** @file include/numcpp/tensor.h
 *  This header defines the tensor class.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_H_INCLUDED
#define NUMCPP_TENSOR_H_INCLUDED

#if __cplusplus < 201103L
#error This file requires compiler and library support for the ISO C++ 2011 \
standard. This support must be enabled with the -std=c++11 or -std=gnu++11 \
compiler options.
#else

#include "numcpp/tensor/shape.h"
#include "numcpp/tensor/slice.h"
#include "numcpp/functional/lazy_expression.h"
#include "numcpp/tensor/dense_tensor.h"
#include "numcpp/tensor/tensor_view.h"
#include "numcpp/tensor/indirect_tensor.h"

namespace numcpp {
/**
 * @brief Tensors are contiguous multidimensional sequence containers: they hold
 * a variable number of elements arranged in multiple axis. Unlike tensor_view,
 * tensor is always owner of its own data, which means that the storage of the
 * tensor is handled automatically.
 *
 * Tensors are designed to easily perform mathematical operations on them. Most
 * mathematical operations can be applied directly to tensor objects, including
 * arithmetic and comparison operators, affecting all its elements. It also
 * supports various forms of generalized subscript operators, slicing and
 * indirect access.
 *
 * @tparam T Type of the elements contained in the tensor. This shall be an
 *           arithmetic type or a class that behaves like one (such as
 *           std::complex).
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class T, size_t Rank>
class tensor : public dense_tensor<tensor<T, Rank>, T, Rank>,
               public complex_expr<tensor<T, Rank>, T, Rank> {
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
  tensor();

  /**
   * @brief Size constructor. Constructs a tensor with given shape, each element
   * is left uninitialized.
   *
   * @param shape Number of elements along each axis. It can be a shape_t object
   *              or the elements of the shape passed as separate arguments.
   * @param order Memory layout in which elements are stored. In row-major
   *              order, the last dimension is contiguous. In column-major
   *              order, the first dimension is contiguous. Defaults to
   *              row-major order.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...> = 0,
            detail::RequiresIntegral<Sizes...> = 0>
  explicit tensor(Sizes... sizes);

  explicit tensor(const shape_type &shape, layout_t order = default_layout);

  /**
   * @brief Fill constructor. Constructs a tensor with given shape, each element
   * initialized to @a val.
   *
   * @param shape Number of elements along each axis.
   * @param val Value to which each of the elements is initialized.
   * @param order Memory layout in which elements are stored. In row-major
   *              order, the last dimension is contiguous. In column-major
   *              order, the first dimension is contiguous. Defaults to
   *              row-major order.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  tensor(const shape_type &shape, const T &val,
         layout_t order = default_layout);

  /**
   * @brief Range constructor. Constructs a tensor with given shape, with each
   * element constructed from its corresponding element in the range starting at
   * @a first, in the same order.
   *
   * @param first Input iterator to the initial position in a range.
   * @param shape Number of elements along each axis. It can be a shape_t object
   *              or the elements of the shape passed as separate arguments.
   * @param order Memory layout in which elements are stored. In row-major
   *              order, the last dimension is contiguous. In column-major
   *              order, the first dimension is contiguous. Defaults to
   *              row-major order.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  template <class InputIterator, class... Sizes,
            detail::RequiresInputIterator<InputIterator> = 0,
            detail::RequiresNArguments<Rank, Sizes...> = 0,
            detail::RequiresIntegral<Sizes...> = 0>
  tensor(InputIterator first, Sizes... sizes);

  template <class InputIterator,
            detail::RequiresInputIterator<InputIterator> = 0>
  tensor(InputIterator first, const shape_type &shape,
         layout_t order = default_layout);

  /**
   * @brief Copy constructor. Constructs a tensor with a copy of each of the
   * elements in @a other, in the same order.
   *
   * @param other A tensor-like object of the same rank.
   * @param order Memory layout in which elements are stored. In row-major
   *              order, the last dimension is contiguous. In column-major
   *              order, the first dimension is contiguous. The default is to
   *              use the same layout as other.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  tensor(const tensor &other);

  template <class Container, class U>
  tensor(const expression<Container, U, Rank> &other);

  template <class Container, class U>
  tensor(const expression<Container, U, Rank> &other, layout_t order);

  /**
   * @brief Move constructor. Constructs a tensor that acquires the elements of
   * @a other.
   *
   * @param other A tensor of the same type and rank. The ownership is directly
   *              transferred from @a other. @a other is left in an empty state.
   */
  tensor(tensor &&other);

  /**
   * @brief Initializer list constructor. Constructs a tensor with a copy of
   * each of the elements in @a il, in the same order.
   *
   * @param il An initializer_list object.
   *
   * @throw std::bad_alloc If the function fails to allocate storage it may
   *                       throw an exception.
   */
  tensor(detail::nested_initializer_list_t<T, Rank> il);

  /// Destructor.
  ~tensor();

  /// Indexing.

  /**
   * @brief Call operator. Return a reference to the element at the given
   * position.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The element at the specified position. If the tensor is
   *         const-qualified, the function returns a reference to const T.
   *         Otherwise, it returns a reference to T.
   *
   * @throw std::out_of_range Thrown if index is out of bounds.
   */
  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  T &operator()(Indices... indices);

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  const T &operator()(Indices... indices) const;

  /**
   * @brief Subscript operator. Return a reference to the element at the given
   * position.
   *
   * @param index An @c index_t object with the position of an element in the
   *              tensor. Since C++23, the elements of the index can be passed
   *              as separate arguments.
   *
   * @return The element at the specified position. If the tensor is
   *         const-qualified, the function returns a reference to const T.
   *         Otherwise, it returns a reference to T.
   *
   * @throw std::out_of_range Thrown if index is out of bounds.
   */
  T &operator[](const index_type &index);
  const T &operator[](const index_type &index) const;

#ifdef __cpp_multidimensional_subscript
  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  T &operator[](Indices... indices);

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  const T &operator[](Indices... indices) const;
#endif // C++23

  /**
   * @brief Slice indexing. Return a @c tensor_view object that selects the
   * elements specified by the slices.
   *
   * @param indices... Each argument can be either an integer or a slice. If an
   *                   integer is given, an unique position is selected for the
   *                   axis and the dimensionality of the returned view is
   *                   reduced by 1. If a slice is given, a subset of positions
   *                   is selected along the axis. An empty slice can be used as
   *                   a placeholder to select all the positions along the axis.
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor. The dimension of the
   *         returned view will equal the number of slice arguments.
   *
   * @throw std::out_of_range Thrown if an integral index is out of bounds.
   */
  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresSlicing<Indices...> = 0>
  tensor_view<T, detail::slicing_rank<Indices...>::value>
  operator()(Indices... indices);

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresSlicing<Indices...> = 0>
  tensor_view<const T, detail::slicing_rank<Indices...>::value>
  operator()(Indices... indices) const;

#ifdef __cpp_multidimensional_subscript
  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresSlicing<Indices...> = 0>
  tensor_view<T, detail::slicing_rank<Indices...>::value>
  operator[](Indices... indices);

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresSlicing<Indices...> = 0>
  tensor_view<const T, detail::slicing_rank<Indices...>::value>
  operator[](Indices... indices) const;
#endif // C++23

  /**
   * @brief Coordinate tensor indexing. Return an @c indirect_tensor that
   * selects the elements specified by the tensor of indices.
   *
   * @param indices A tensor-like object of @c index_t with its elements
   *                identifying which elements of the tensor are selected. If
   *                the tensor is 1-dimensional, a tensor-like object of
   *                integers can be used instead.
   *
   * @return If the tensor is const-qualified, the function returns a new tensor
   *         object with a copy of the selection. Otherwise, the function
   *         returns an indirect_tensor, which has reference semantics to the
   *         original tensor.
   *
   * @throw std::out_of_range Thrown if the indices references to an element
   *                          out of bounds.
   * @throw std::bad_alloc If the function needs to allocate storage and fails,
   *                       it may throw an exception.
   */
  template <class Container, size_t N>
  indirect_tensor<T, N>
  operator[](const expression<Container, index_type, N> &indices);

  template <class Container, size_t N>
  tensor<T, N>
  operator[](const expression<Container, index_type, N> &indices) const;

  template <class Container, class IntegralType, size_t N,
            detail::RequiresIntegral<IntegralType> = 0>
  indirect_tensor<T, N>
  operator[](const expression<Container, IntegralType, N> &indices);

  template <class Container, class IntegralType, size_t N,
            detail::RequiresIntegral<IntegralType> = 0>
  tensor<T, N>
  operator[](const expression<Container, IntegralType, N> &indices) const;

  /**
   * @brief Boolean tensor indexing. Return an @c indirect_tensor that selects
   * the elements specified by the boolean mask.
   *
   * @param mask A tensor-like object of @c bool with its elements identifying
   *             whether each element of the tensor is selected or not.
   *
   * @return If the tensor is const-qualified, the function returns a new tensor
   *         object with a copy of the selection. Otherwise, the function
   *         returns an indirect_tensor, which has reference semantics to the
   *         original tensor.
   *
   * @throw std::invalid_argument Thrown if the tensor and the boolean mask
   *                              have different shapes.
   * @throw std::bad_alloc If the function needs to allocate storage and fails,
   *                       it may throw an exception.
   */
  template <class Container>
  indirect_tensor<T, 1>
  operator[](const expression<Container, bool, Rank> &mask);

  template <class Container>
  tensor<T, 1> operator[](const expression<Container, bool, Rank> &mask) const;

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const;
  size_type shape(size_type axis) const;

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_type size() const;

  /**
   * @brief Return whether the tensor is empty (i.e., whether its size is 0).
   */
  bool empty() const;

  /**
   * @brief Return a pointer to the memory array used internally by the tensor.
   * Because elements in the tensor are stored contiguously, the pointer
   * retrieved can be offset to access any element in the tensor.
   *
   * @return A pointer to the memory array used internally by the tensor. If the
   *         tensor is const-qualified, the function returns a pointer to
   *         const T. Otherwise, it returns a pointer to T.
   */
  T *data();
  const T *data() const;

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const;

  /**
   * @brief Return whether the elements in the tensor are stored contiguously.
   * For @c tensor class, always returns true.
   */
  bool is_contiguous() const;

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in @a other, after resizing the object (if
   * necessary).
   *
   * @param other A tensor-like object of the same rank.
   *
   * @return *this
   *
   * @throw std::bad_alloc If the function needs to allocate storage and fails,
   *                       it may throw an exception.
   *
   * @warning When the shapes do not mach, invalidates all iterators, references
   * and views to elements of the tensor. Otherwise, valid iterators, references
   * and views keep their validity.
   */
  tensor &operator=(const tensor &other);
  template <class Container, class U>
  tensor &operator=(const expression<Container, U, Rank> &other);

  /**
   * @brief Fill assignment. Assigns @a val to every element. The size of the
   * tensor is preserved.
   *
   * @param val Value assigned to all the elements in the tensor.
   *
   * @return *this
   */
  tensor &operator=(const T &val);

  /**
   * @brief Move assignment. Acquires the contents of @a other, leaving @a other
   * in an empty state.
   *
   * @param other A tensor of the same type and rank. The ownership is directly
   *              transferred from @a other. @a other is left in an empty state.
   *
   * @return *this
   *
   * @warning Invalidates all iterators, references and views to elements of the
   * tensor.
   */
  tensor &operator=(tensor &&other);

  /**
   * @brief Initializer list assignment. Assigns to each element the value of
   * the corresponding element in @a il after resizing the object (if
   * necessary).
   *
   * @param il An initializer_list object.
   *
   * @return *this
   *
   * @throw std::bad_alloc If the function needs to allocate storage and fails,
   *                       it may throw an exception.
   *
   * @warning When the shapes do not mach, invalidates all iterators, references
   * and views to elements of the tensor. Otherwise, valid iterators, references
   * and views keep their validity.
   */
  tensor &operator=(detail::nested_initializer_list_t<T, Rank> il);

  /// Public methods.

  /**
   * @brief Return a view of the diagonal.
   *
   * @param k Offset of the diagonal from the main diagonal. A positive value
   *          refers to an upper diagonal and a negative value refers to a lower
   *          diagonal. Defaults to 0 (main diagonal).
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor.
   */
  tensor_view<T, 1> diagonal(difference_type k = 0);
  tensor_view<const T, 1> diagonal(difference_type k = 0) const;

  /**
   * @brief Return a view of the tensor collapsed into one dimension.
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor.
   */
  tensor_view<T, 1> flatten();
  tensor_view<const T, 1> flatten() const;

  /**
   * @brief Return a tensor_view containing the same data with a new shape.
   *
   * @param shape The new shape should be compatible with the original shape. It
   *              can be a shape_t object or the elements of the shape passed as
   *              separate arguments.
   * @param order Memory layout in which elements are read. In row-major order,
   *              the last dimension is contiguous. In column-major order, the
   *              first dimension is contiguous. The default is to use the same
   *              layout as *this.
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor.
   *
   * @throw std::invalid_argument Thrown if the tensor could not reshaped.
   */
  template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
  tensor_view<T, sizeof...(Sizes)> reshape(Sizes... sizes);

  template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
  tensor_view<const T, sizeof...(Sizes)> reshape(Sizes... sizes) const;

  template <size_t N> tensor_view<T, N> reshape(const shape_t<N> &shape);

  template <size_t N>
  tensor_view<const T, N> reshape(const shape_t<N> &shape) const;

  template <size_t N>
  tensor_view<T, N> reshape(const shape_t<N> &shape, layout_t order);

  template <size_t N>
  tensor_view<const T, N> reshape(const shape_t<N> &shape,
                                  layout_t order) const;

  /**
   * @brief Resizes the tensor in-place to a given shape. If the new size is
   * different from the number of elements stored in the tensor, a reallocation
   * takes place to fit the new shape, losing the previous contents in the
   * process. Otherwise, the contents of the tensor are preserved, but arranged
   * to match the new shape.
   *
   * @param shape New shape of the tensor. It can be a shape_t object or the
   *              elements of the shape passed as separate arguments.
   *
   * @warning Invalidates all iterators, references and views to elements of the
   * tensor.
   */
  template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...> = 0,
            detail::RequiresIntegral<Sizes...> = 0>
  void resize(Sizes... sizes);

  void resize(const shape_type &shape);

  /**
   * @brief Removes axes of length one.
   *
   * @param axes Selects a subset of the entries of length one in the shape. It
   *             can be a shape_t object or the elements of the shape passed as
   *             separate arguments.
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor.
   *
   * @throw std::invalid_argument Thrown if an axis with shape entry greater
   *                              than one is selected.
   */
  template <class... Axes, detail::RequiresIntegral<Axes...> = 0>
  tensor_view<T, Rank - sizeof...(Axes)> squeeze(Axes... axes);

  template <class... Axes, detail::RequiresIntegral<Axes...> = 0>
  tensor_view<const T, Rank - sizeof...(Axes)> squeeze(Axes... axes) const;

  template <size_t N> tensor_view<T, Rank - N> squeeze(const shape_t<N> &axes);

  template <size_t N>
  tensor_view<const T, Rank - N> squeeze(const shape_t<N> &axes) const;

  /**
   * @brief Return a view of the tensor with its axes in reversed order.
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor.
   */
  tensor_view<T, Rank> t();
  tensor_view<const T, Rank> t() const;

  /**
   * @brief Return a view of the tensor with the same data.
   *
   * @return If the tensor is const-qualified, the function returns a
   *         tensor_view to const T, which is convertible to a tensor object.
   *         Otherwise, the function returns a tensor_view to T, which has
   *         reference semantics to the original tensor.
   */
  tensor_view<T, Rank> view();
  tensor_view<const T, Rank> view() const;

private:
  /**
   * @brief Get the shape of a nested initializer list recursively.
   */
  template <size_t Depth>
  typename std::enable_if<(Depth == 1)>::type
  __initializer_list_shape(shape_type &shape, std::initializer_list<T> il);

  template <size_t Depth>
  typename std::enable_if<(Depth > 1)>::type
  __initializer_list_shape(shape_type &shape,
                           detail::nested_initializer_list_t<T, Depth> il);

  /**
   * @brief Fill the tensor with elements from a nested initializer_list
   * recursively.
   */
  template <size_t Depth>
  typename std::enable_if<(Depth == 1)>::type
  __fill_from_initializer_list(index_type &index, std::initializer_list<T> il);

  template <size_t Depth>
  typename std::enable_if<(Depth > 1)>::type
  __fill_from_initializer_list(index_type &index,
                               detail::nested_initializer_list_t<T, Depth> il);

  /**
   * @brief Constructs the shape, offset and strides of the slice indexing.
   * Removes out of bound elements when possible, otherwise throws a
   * std::out_of_range error.
   */
  template <size_t N>
  void __unpack_slices(size_t &size, shape_t<N> &shape, ptrdiff_t &offset,
                       shape_t<N> &strides) const;

  template <size_t N, class... Indices>
  void __unpack_slices(size_t &size, shape_t<N> &shape, ptrdiff_t &offset,
                       shape_t<N> &strides, size_t i, Indices... indices) const;

  template <size_t N, class... Indices>
  void __unpack_slices(size_t &size, shape_t<N> &shape, ptrdiff_t &offset,
                       shape_t<N> &strides, slice slc,
                       Indices... indices) const;

private:
  // Pointer to data.
  T *m_data;

  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Memory layout.
  layout_t m_order;
};

/**
 * @brief Each of this functions performs their respective operation on all the
 * elements in the tensor.
 *
 * When both the left-hand side and right-hand side arguments are tensor
 * objects, the operation is performed between the corresponding elements in
 * each object (the first element of the left one with the right one, the second
 * with the second, and so on...)
 *
 * When one of the arguments is a value, the operation is applied to all the
 * elements in the tensor against that value.
 *
 * @param lhs Left-hand side tensor-like object.
 * @param rhs Right-hand side tensor-like object.
 * @param val Value to use either as left-hand or right-hand operand.
 *
 * @return A light-weight object which stores the result of performing the
 *         operation on each element. None of these operators creates a new
 *         tensor object, instead, an expression object is returned. The
 *         returned object uses lazy-evaluation, which means that the result of
 *         each operation is computed only when required, i.e., when the whole
 *         expression is evaluated or assigned to a tensor object.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */

/// Unary operators.

template <class Container, class T, size_t Rank>
inline unary_expr<unary_plus, Container, T, Rank>
operator+(const expression<Container, T, Rank> &arg) {
  return unary_expr<unary_plus, Container, T, Rank>(arg);
}

template <class Container, class T, size_t Rank>
inline unary_expr<negate, Container, T, Rank>
operator-(const expression<Container, T, Rank> &arg) {
  return unary_expr<negate, Container, T, Rank>(arg);
}

template <class Container, class T, size_t Rank>
inline unary_expr<bit_not, Container, T, Rank>
operator~(const expression<Container, T, Rank> &arg) {
  return unary_expr<bit_not, Container, T, Rank>(arg);
}

template <class Container, class T, size_t Rank>
inline unary_expr<logical_not, Container, T, Rank>
operator!(const expression<Container, T, Rank> &arg) {
  return unary_expr<logical_not, Container, T, Rank>(arg);
}

/// Arithmetic operators.

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<plus, Container1, T, Container2, T, Rank>
operator+(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<plus, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<plus, Container, T, void, T, Rank>
operator+(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<plus, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<plus, void, T, Container, T, Rank>
operator+(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<plus, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<minus, Container1, T, Container2, T, Rank>
operator-(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<minus, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<minus, Container, T, void, T, Rank>
operator-(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<minus, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<minus, void, T, Container, T, Rank>
operator-(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<minus, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<multiplies, Container1, T, Container2, T, Rank>
operator*(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<multiplies, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<multiplies, Container, T, void, T, Rank>
operator*(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<multiplies, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<multiplies, void, T, Container, T, Rank>
operator*(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<multiplies, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<divides, Container1, T, Container2, T, Rank>
operator/(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<divides, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<divides, Container, T, void, T, Rank>
operator/(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<divides, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<divides, void, T, Container, T, Rank>
operator/(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<divides, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<modulus, Container1, T, Container2, T, Rank>
operator%(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<modulus, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<modulus, Container, T, void, T, Rank>
operator%(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<modulus, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<modulus, void, T, Container, T, Rank>
operator%(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<modulus, void, T, Container, T, Rank>(val, rhs);
}

/// Bitwise operators.

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<bit_and, Container1, T, Container2, T, Rank>
operator&(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<bit_and, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<bit_and, Container, T, void, T, Rank>
operator&(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<bit_and, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<bit_and, void, T, Container, T, Rank>
operator&(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<bit_and, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<bit_or, Container1, T, Container2, T, Rank>
operator|(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<bit_or, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<bit_or, Container, T, void, T, Rank>
operator|(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<bit_or, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<bit_or, void, T, Container, T, Rank>
operator|(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<bit_or, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<bit_xor, Container1, T, Container2, T, Rank>
operator^(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<bit_xor, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<bit_xor, Container, T, void, T, Rank>
operator^(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<bit_xor, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<bit_xor, void, T, Container, T, Rank>
operator^(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<bit_xor, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<left_shift, Container1, T, Container2, T, Rank>
operator<<(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<left_shift, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<left_shift, Container, T, void, T, Rank>
operator<<(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<left_shift, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<left_shift, void, T, Container, T, Rank>
operator<<(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<left_shift, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<right_shift, Container1, T, Container2, T, Rank>
operator>>(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<right_shift, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<right_shift, Container, T, void, T, Rank>
operator>>(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<right_shift, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<right_shift, void, T, Container, T, Rank>
operator>>(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<right_shift, void, T, Container, T, Rank>(val, rhs);
}

/// Logical operators.

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<logical_and, Container1, T, Container2, T, Rank>
operator&&(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<logical_and, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<logical_and, Container, T, void, T, Rank>
operator&&(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<logical_and, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<logical_and, void, T, Container, T, Rank>
operator&&(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<logical_and, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<logical_or, Container1, T, Container2, T, Rank>
operator||(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<logical_or, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<logical_or, Container, T, void, T, Rank>
operator||(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<logical_or, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<logical_or, void, T, Container, T, Rank>
operator||(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<logical_or, void, T, Container, T, Rank>(val, rhs);
}

/// Relational operators.

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<equal_to, Container1, T, Container2, T, Rank>
operator==(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<equal_to, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<equal_to, Container, T, void, T, Rank>
operator==(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<equal_to, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<equal_to, void, T, Container, T, Rank>
operator==(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<equal_to, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<not_equal_to, Container1, T, Container2, T, Rank>
operator!=(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<not_equal_to, Container1, T, Container2, T, Rank>(lhs,
                                                                       rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<not_equal_to, Container, T, void, T, Rank>
operator!=(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<not_equal_to, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<not_equal_to, void, T, Container, T, Rank>
operator!=(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<not_equal_to, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<less, Container1, T, Container2, T, Rank>
operator<(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<less, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<less, Container, T, void, T, Rank>
operator<(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<less, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<less, void, T, Container, T, Rank>
operator<(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<less, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<greater, Container1, T, Container2, T, Rank>
operator>(const expression<Container1, T, Rank> &lhs,
          const expression<Container2, T, Rank> &rhs) {
  return binary_expr<greater, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<greater, Container, T, void, T, Rank>
operator>(const expression<Container, T, Rank> &lhs,
          const typename Container::value_type &val) {
  return binary_expr<greater, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<greater, void, T, Container, T, Rank>
operator>(const typename Container::value_type &val,
          const expression<Container, T, Rank> &rhs) {
  return binary_expr<greater, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<less_equal, Container1, T, Container2, T, Rank>
operator<=(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<less_equal, Container1, T, Container2, T, Rank>(lhs, rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<less_equal, Container, T, void, T, Rank>
operator<=(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<less_equal, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<less_equal, void, T, Container, T, Rank>
operator<=(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<less_equal, void, T, Container, T, Rank>(val, rhs);
}

template <class Container1, class T, class Container2, size_t Rank>
inline binary_expr<greater_equal, Container1, T, Container2, T, Rank>
operator>=(const expression<Container1, T, Rank> &lhs,
           const expression<Container2, T, Rank> &rhs) {
  return binary_expr<greater_equal, Container1, T, Container2, T, Rank>(lhs,
                                                                        rhs);
}

template <class Container, class T, size_t Rank>
inline binary_expr<greater_equal, Container, T, void, T, Rank>
operator>=(const expression<Container, T, Rank> &lhs,
           const typename Container::value_type &val) {
  return binary_expr<greater_equal, Container, T, void, T, Rank>(lhs, val);
}

template <class T, class Container, size_t Rank>
inline binary_expr<greater_equal, void, T, Container, T, Rank>
operator>=(const typename Container::value_type &val,
           const expression<Container, T, Rank> &rhs) {
  return binary_expr<greater_equal, void, T, Container, T, Rank>(val, rhs);
}
} // namespace numcpp

#include "numcpp/tensor/tensor.tcc"

#endif // C++11
#endif // NUMCPP_TENSOR_H_INCLUDED
