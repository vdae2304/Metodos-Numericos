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

/** @file include/numcpp/tensor/dense_tensor.h
 *  This header defines the base class for non-expression dense tensors.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_DENSE_TENSOR_H_INCLUDED
#define NUMCPP_DENSE_TENSOR_H_INCLUDED

#include "numcpp/shape.h"
#include "numcpp/functional/operators.h"
#include "numcpp/iterators/flat_iterator.h"
#include "numcpp/math/mathfwd.h"

namespace numcpp {
/**
 * @brief Base class for dense tensors such as tensor, tensor_view and
 * indirect_tensor. Unlike plain expressions, dense tensors allows to modify its
 * elements, and implements an assortment of methods which will be inherited to
 * all the subclasses.
 *
 * @tparam Container Tensor subclass.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Container, class T, size_t Rank>
class dense_tensor : public expression<Container, T, Rank> {
public:
  static_assert(0 < Rank  && Rank <= 10, "Rank must be between 1 and 10");

  /// Member types.
  typedef flat_iterator<Container, T, Rank> iterator;
  typedef flat_iterator<const Container, T, Rank, const T *, const T &>
      const_iterator;

  /// Indexing.

  /**
   * @brief Return a reference to the element at the given position.
   */
  T &operator[](const index_t<Rank> &index);
  const T &operator[](const index_t<Rank> &index) const;

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  shape_t<Rank> shape() const;
  size_t shape(size_t axis) const;

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_t size() const;

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const;

  /**
   * @brief Return the derived subclass.
   */
  Container &self();
  const Container &self() const;

  /// Iterators.

  /**
   * @brief Return an iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the beginning of the tensor. If the
   *         tensor is const-qualified, the function returns a const_iterator.
   *         Otherwise, it returns an iterator.
   */
  iterator begin();
  const_iterator begin() const;

  iterator begin(layout_t order);
  const_iterator begin(layout_t order) const;

  /**
   * @brief Return an iterator pointing to the past-the-end element in the
   * tensor. It does not point to any element, and thus shall not be
   * dereferenced.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A random access iterator to the element past the end of the tensor.
   *         If the tensor is const-qualified, the function returns a
   *         const_iterator. Otherwise, it returns an iterator.
   */
  iterator end();
  const_iterator end() const;

  iterator end(layout_t order);
  const_iterator end(layout_t order) const;

  /**
   * @brief Return a const_iterator pointing to the first element in the tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A const_iterator to the beginning of the tensor.
   */
  const_iterator cbegin() const;
  const_iterator cbegin(layout_t order) const;

  /**
   * @brief Return a const_iterator pointing to the past-the-end element in the
   * tensor.
   *
   * @param order It is an optional parameter that changes the order in which
   *              elements are iterated. In row-major order, the last index is
   *              varying the fastest. In column-major order, the first index is
   *              varying the fastest. The default is to use the same layout as
   *              stored in memory.
   *
   * @return A const_iterator to the element past the end of the tensor.
   */
  const_iterator cend() const;
  const_iterator cend(layout_t order) const;

  /// Assignment operator.

  /**
   * @brief Copy assignment. Assigns to each element the value of the
   * corresponding element in @a other.
   *
   * @param other A tensor-like object of the same shape.
   *
   * @return *this
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   */
  template <class ContainerOp, class U>
  Container &operator=(const expression<ContainerOp, U, Rank> &other);

  /**
   * @brief Fill assignment. Assigns @a val to every element.
   *
   * @param val Value assigned to all the elements in the tensor.
   *
   * @return *this
   */
  Container &operator=(const T &val);

  /// Compound assignment operator.

  /**
   * @brief Assigns to @a *this the result of performing the respective
   * operation on all the elements in the tensor.
   *
   * When the right-hand side argument is a tensor object, the operation is
   * performed between the corresponding elements in each object (the first
   * element of the left one with the right one, the second with the second, and
   * so on...)
   *
   * When the right-hand side argument is a value, the operation is applied to
   * all the elements in the tensor against that value.
   *
   * @param rhs Right-hand side tensor-like object.
   * @param val Value to use as right-hand operand.
   *
   * @return *this
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   */
  template <class ContainerOp>
  Container &operator+=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator-=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator*=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator/=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator%=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator&=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator|=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator^=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator<<=(const expression<ContainerOp, T, Rank> &rhs);
  template <class ContainerOp>
  Container &operator>>=(const expression<ContainerOp, T, Rank> &rhs);

  Container &operator+=(const T &val);
  Container &operator-=(const T &val);
  Container &operator*=(const T &val);
  Container &operator/=(const T &val);
  Container &operator%=(const T &val);
  Container &operator&=(const T &val);
  Container &operator|=(const T &val);
  Container &operator^=(const T &val);
  Container &operator<<=(const T &val);
  Container &operator>>=(const T &val);

  /// Public methods.

  /**
   * @brief Test whether all tensor elements evaluate to true.
   *
   * @return true if all the elements evaluate to true and false otherwise.
   */
  bool all() const;

  /**
   * @brief Test whether any tensor element evaluate to true.
   *
   * @return true if any element evaluate to true and false otherwise.
   */
  bool any() const;

  /**
   * @brief Assigns to each element the result of applying a function to the
   * corresponding elements in @a *this.
   *
   * @param f A function that accepts one element of type T as argument, and
   *          returns a value convertible to T.
   */
  void apply(T f(T));
  void apply(T f(const T &));
  template <class Function> void apply(Function &&f);

  /**
   * @brief Return the index of the maximum value in the tensor.
   *
   * @return The index of the maximum value in the tensor.
   */
  index_t<Rank> argmax() const;

  /**
   * @brief Return the index of the minimum value in the tensor.
   *
   * @return The index of the minimum value in the tensor.
   */
  index_t<Rank> argmin() const;

  /**
   * @brief Cast each element to a specified type.
   *
   * @tparam U Data type to which the elements are casted.
   *
   * @return A light-weight object with the elements in the tensor casted to the
   *         specified type. This function does not create a new tensor,
   *         instead, it returns a readonly view of the tensor with its elements
   *         casted to the specified type.
   */
  template <class U>
  unary_expr<cast_to<T, U>, Container, T, Rank> astype() const;

  /**
   * @brief Clamp the values in the tensor. Given an interval @a [a_min,a_max],
   * values smaller than @a a_min become @a a_min, and values larger than
   * @a a_max become @a a_max. For complex types, the real and imaginary parts
   * are clamped separately.
   *
   * @param a_min The lower boundary to clamp.
   * @param a_max The upper boundary to clamp.
   *
   * @note The behavior is undefined if @a a_min is greater than @a a_max.
   */
  void clamp(const T &a_min, const T &a_max);

  /**
   * @brief Return the maximum value contained in the tensor.
   *
   * @return The maximum value in the tensor.
   */
  T max() const;

  /**
   * @brief Return the average of the tensor elements.
   *
   * @return The average of the tensor elements.
   */
  T mean() const;

  /**
   * @brief Return the minimum value contained in the tensor.
   *
   * @return The minimum value in the tensor.
   */
  T min() const;

  /**
   * @brief Partition the elements in-place.
   *
   * @param kth Element index to partition by. The element at the @a kth
   *            position is the element that would be in that position in the
   *            sorted tensor. The other elements are left without any specific
   *            order, except that none of the elements preceding @a kth are
   *            greater than it, and none of the elements following it are less.
   * @param axis Axis along which to partition. Defaults to Rank - 1, which
   *             means partition along the last axis.
   * @param comp Custom comparator. A binary function that accepts two elements
   *             of type T as arguments, and returns a value convertible to
   *             bool. The value returned indicates whether the element passed
   *             as first argument is considered to go before the second.
   */
  void partition(size_t kth, size_t axis = Rank - 1);
  template <class Compare, detail::RequiresCallable<Compare, T, T> = 0>
  void partition(size_t kth, size_t axis, Compare comp);

  /**
   * @brief Return the product of the tensor elements.
   *
   * @return The product of the tensor elements.
   */
  T prod() const;

  /**
   * @brief Reverse the order of the elements in-place.
   *
   * @param axis Axis along which to reverse. Defaults to Rank - 1, which means
   *             reverse along the last axis.
   */
  void reverse(size_t axis = Rank - 1);

  /**
   * @brief Rotate the elements in-place. The elements are shifted circularly in
   * such a way that the element at position @a shift becomes the first element
   * and the element at position @a shift - 1 becomes the last element.
   *
   * @param shift Number of positions to shift the elements by.
   * @param axis Axis along which to rotate. Defaults to Rank - 1, which means
   *             rotate along the last axis.
   */
  void rotate(size_t shift, size_t axis = Rank - 1);

  /**
   * @brief Sort the elements in-place.
   *
   * @param axis Axis along which to sort. Defaults to Rank - 1, which means
   *             sort along the last axis.
   * @param comp Custom comparator. A binary function that accepts two elements
   *             of type T as arguments, and returns a value convertible to
   *             bool. The value returned indicates whether the element passed
   *             as first argument is considered to go before the second.
   * @param stable If true, preserve the relative order of the elements with
   *               equivalent values. Otherwise, equivalent elements are not
   *               guaranteed to keep their original relative order.
   */
  void sort(size_t axis = Rank - 1);
  template <class Compare, detail::RequiresCallable<Compare, T, T> = 0>
  void sort(size_t axis, Compare comp, bool stable = false);

  /**
   * @brief Return the standard deviation of the tensor elements.
   *
   * @details The standard deviation is defined as the square root of the
   * average of the squared deviations from the mean
   *     stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
   * The mean is calculated as sum(x)/n, where n = x.size(). However, if @a bias
   * is false, the divisor n - 1 is used instead of n. In statistics, n - 1
   * provides an unbiased estimator of the sample variance; while n provides the
   * maximum likelihood estimator of the variance for normally distributed
   * variables.
   *
   * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
   *             normalization is by @a n - 1.
   *
   * @return The standard deviation of the tensor elements.
   */
  T stddev(bool bias) const;

  /**
   * @brief Return the sum of the tensor elements.
   *
   * @return The sum of the tensor elements.
   */
  T sum() const;

  /**
   * @brief Return the variance of the tensor elements.
   *
   * @details The variance is defined as the average of the squared deviations
   * from the mean
   *     var(a) = mean(x),    x = abs(a - mean(a))**2
   * The mean is calculated as sum(x)/n, where n = x.size(). However, if @a bias
   * is false, the divisor n - 1 is used instead of n. In statistics, n - 1
   * provides an unbiased estimator of the sample variance; while n provides the
   * maximum likelihood estimator of the variance for normally distributed
   * variables.
   *
   * @param bias If @a bias is true, then normalization is by @a n. Otherwise,
   *             normalization is by @a n - 1.
   *
   * @return The variance of the tensor elements.
   */
  T var(bool bias) const;

private:
  /**
   * @brief Apply a binary function in-place with another tensor or with a
   * value.
   */
  template <class Function, class ContainerOp>
  Container &apply2(Function f, const expression<ContainerOp, T, Rank> &rhs);

  template <class Function> Container &apply2(Function f, const T &val);
};

template <class Container, class T, size_t Rank> class complex_expr {};

/**
 * @brief A complex_expr is an interface with additional methods for
 * complex-valued tensor subclasses.
 *
 * @tparam Container Tensor subclass.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Container, class T, size_t Rank>
class complex_expr<Container, std::complex<T>, Rank> {
public:
  /// Public methods.

  /**
   * @brief Return the real part, element-wise.
   *
   * @return A light-weight object with the real part of each element in the
   *         tensor. This function does not create a new tensor, instead, it
   *         returns a readonly view with the real part of each element.
   */
  unary_expr<math::real, Container, std::complex<T>, Rank> real() const;

  /**
   * @brief Set the real part, element-wise.
   *
   * @param x A tensor-like object with the values to set the real part to.
   * @param val Value to set the real part to.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   */
  template <class ContainerOp>
  void real(const expression<ContainerOp, T, Rank> &x);

  void real(const T &val);

  /**
   * @brief Return the imaginary part, element-wise.
   *
   * @return A light-weight object with the imaginary part of each element in
   *         the tensor. This function does not create a new tensor, instead, it
   *         returns a readonly view with the imaginary part of each element.
   */
  unary_expr<math::imag, Container, std::complex<T>, Rank> imag() const;

  /**
   * @brief Set the imaginary part, element-wise.
   *
   * @param y A tensor-like object with the values to set the imaginary part
   *            to.
   * @param val Value to set the imaginary part to.
   *
   * @throw std::invalid_argument Thrown if the shapes are not compatible and
   *                              cannot be broadcasted according to
   *                              broadcasting rules.
   */
  template <class ContainerOp>
  void imag(const expression<ContainerOp, T, Rank> &y);

  void imag(const T &val);

  /**
   * @brief Return the complex conjugate, element-wise.
   *
   * @return A light-weight object with the complex conjugate of each element in
   *         the tensor. This function does not create a new tensor, instead, it
   *         returns an expression object with the complex conjugate of each
   *         element.
   */
  unary_expr<math::conj, Container, std::complex<T>, Rank> conj() const;
};
} // namespace numcpp

#include "numcpp/tensor/dense_tensor.tcc"

#endif // NUMCPP_DENSE_TENSOR_H_INCLUDED
