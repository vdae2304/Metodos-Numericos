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

/** @file include/numcpp/tensor/indirect_tensor.h
 *  This header defines the indirect_tensor class.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_INDIRECT_TENSOR_H_INCLUDED
#define NUMCPP_INDIRECT_TENSOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/tensor/dense_tensor.h"

namespace numcpp {
/**
 * @brief An indirect_tensor is a view of a subset of elements from a
 * multidimensional array. It references the elements in the original array
 * through an array of indices. The view itself does not own the data and any
 * changes made to the view will affect the original array, and any changes made
 * to the original array will affect the view.
 *
 * @tparam T Type of the elements contained in the indirect_tensor. This shall
 *           be an arithmetic type or a class that behaves like one (such as
 *           std::complex).
 * @tparam Rank Dimension of the indirect_tensor. It must be a positive integer.
 */
template <class T, size_t Rank>
class indirect_tensor
    : public dense_tensor<indirect_tensor<T, Rank>,
                          typename std::remove_cv<T>::type, Rank>,
      public complex_expr<indirect_tensor<T, Rank>,
                          typename std::remove_cv<T>::type, Rank> {
public:
  /// Member types.
  typedef typename std::remove_cv<T>::type value_type;
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
   * @brief Default constructor. Constructs an indirect_tensor that does not
   * reference any object.
   */
  indirect_tensor();

  /**
   * @brief View constructor. Constructs an indirect_tensor that references the
   * elements of a multidimensional array.
   *
   * @param data Pointer to the memory array used by the indirect_tensor.
   * @param shape Number of elements along each axis. It can be a shape_t object
   *              or the elements of the shape passed as separate arguments.
   * @param order Memory layout in which elements are stored. In row-major
   *              order, the last dimension is contiguous. In column-major
   *              order, the first dimension is contiguous. Defaults to
   *              row-major order.
   */
  template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...> = 0,
            detail::RequiresIntegral<Sizes...> = 0>
  indirect_tensor(T *data, Sizes... sizes);

  indirect_tensor(T *data, const shape_type &shape,
                  layout_t order = default_layout);

  /**
   * @brief Index array constructor. Constructs an indirect_tensor that
   * references a subset of elements from a multidimensional array.
   *
   * @param data Pointer to the memory array used by the indirect_tensor.
   * @param shape Number of elements along each axis.
   * @param indptr Pointer to the array of indices with its elements identifying
   *               which elements of the memory array are selected.
   * @param order Order in which elements shall be iterated. In row-major order,
   *              the last index is varying the fastest. In column-major order,
   *              the first index is varying the fastest. Defaults to
   *              row-major order.
   */
  template <class IntegralType, detail::RequiresIntegral<IntegralType> = 0>
  indirect_tensor(T *data, const shape_type &shape, IntegralType *indptr,
                  layout_t order = default_layout);

  /**
   * @brief Copy constructor. Constructs an indirect_tensor as a copy of
   * @a other.
   *
   * @param other An indirect_tensor of the same type and rank.
   */
  indirect_tensor(const indirect_tensor &other);

  /**
   * @brief Move constructor. Constructs an indirect_tensor that acquires the
   * elements of @a other.
   *
   * @param other An indirect_tensor of the same type and rank. The ownership
   *              of the array of indices is directly transferred from @a other.
   *              @a other is left in an empty state.
   */
  indirect_tensor(indirect_tensor &&other);

  /// Destructor.
  ~indirect_tensor();

  /// Indexing.

  /**
   * @brief Call operator. Return a reference to the element at the given
   * position. The elements in an indirect_tensor are given by
   *     data[indptr[ravel_index(index, shape, order)]]
   * where @a data is the memory array and @a indptr is the array of indices.
   *
   * @param indices... Position of an element along each axis.
   *
   * @return The element at the specified position. If the indirect_tensor is
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
   *              indirect_tensor. Since C++23, the elements of the index can be
   *              passed as separate arguments.
   *
   * @return The element at the specified position. If the indirect_tensor is
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
   * @brief Return a reference to the element at the given position in the array
   * of indices. The elements in an indirect_tensor can be accessed as
   *     data[iat(index)]
   * where @a data is the memory array.
   *
   * @param index The position of an element in the array of indices. It can be
   *              an @c index_t object or the elements of the index passed as
   *              separate arguments.
   *
   * @return The element at the specified position. If the indirect_tensor is
   *         const-qualified, the function returns a reference to const
   *         size_type. Otherwise, it returns a reference to size_type.
   *
   * @throw std::out_of_range Thrown if index is out of bounds.
   */
  size_type &iat(const index_type &index);
  const size_type &iat(const index_type &index) const;

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  size_type &iat(Indices... indices);

  template <class... Indices, detail::RequiresNArguments<Rank, Indices...> = 0,
            detail::RequiresIntegral<Indices...> = 0>
  size_type &iat(Indices... indices) const;

  /**
   * @brief Return the shape of the indirect_tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  const shape_type &shape() const;
  size_type shape(size_type axis) const;

  /**
   * @brief Return the number of elements in the indirect_tensor (i.e., the
   * product of the sizes along all the axes).
   */
  size_type size() const;

  /**
   * @brief Return whether the indirect_tensor is empty (i.e., whether its size
   * is 0).
   */
  bool empty() const;

  /**
   * @brief Return a pointer to the memory array used internally by the
   * indirect_tensor.
   *
   * @return A pointer to the memory array used internally by the
   *         indirect_tensor. If the indirect_tensor is const-qualified, the
   *         function returns a pointer to const T. Otherwise, it returns a
   *         pointer to T.
   */
  T *data();
  const T *data() const;

  /**
   * @brief Return a pointer to the array of indices used internally by the
   * indirect_tensor.
   *
   * @return A pointer to the array of indices used internally by the
   *         indirect_tensor. If the indirect_tensor is const-qualified, the
   *         function returns a pointer to const size_type. Otherwise, it
   *         returns a pointer to size_type.
   */
  size_type *indptr();
  const size_type *indptr() const;

  /**
   * @brief Return the order in which elements are iterated. It is not
   * necessarily the memory layout in which elements are stored as the elements
   * might not be contiguous in memory.
   */
  layout_t layout() const;

  /**
   * @brief Return whether the elements in the indirect_tensor are stored
   * contiguously. For @c indirect_tensor class, always returns false.
   */
  bool is_contiguous() const;

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in @a other.
   *
   * @param other A tensor-like object of the same shape.
   *
   * @return *this
   *
   * @throw std::invalid_argument Thrown if the shapes are different.
   */
  indirect_tensor &operator=(const indirect_tensor &other);
  template <class Container, class U>
  indirect_tensor &operator=(const expression<Container, U, Rank> &other);

  /**
   * @brief Fill assignment. Assigns @a val to every element.
   *
   * @param val Value assigned to all the elements in the indirect_tensor.
   *
   * @return *this
   */
  indirect_tensor &operator=(const T &val);

  /**
   * @brief Move assignment. Acquires the contents of @a other, leaving @a other
   * in an empty state.
   *
   * @param other An indirect_tensor of the same type and rank. The ownership
   *              of the array of indices is directly transferred from @a other.
   *              @a other is left in an empty state.
   *
   * @return *this
   */
  indirect_tensor &operator=(indirect_tensor &&other);

private:
  // Pointer to data.
  T *m_data;

  // Number of elements along each axis.
  shape_type m_shape;

  // Number of elements.
  size_type m_size;

  // Pointer to the array of indices.
  size_type *m_indptr;

  // Memory layout.
  layout_t m_order;
};
} // namespace numcpp

#include "numcpp/tensor/indirect_tensor.tcc"

#endif // NUMCPP_INDIRECT_TENSOR_H_INCLUDED
