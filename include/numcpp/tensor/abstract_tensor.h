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

/** @file include/numcpp/tensor/abstract_tensor.h
 *  This header defines the base class for all tensors.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ABSTRACT_TENSOR_H_INCLUDED
#define NUMCPP_ABSTRACT_TENSOR_H_INCLUDED

#include "numcpp/shape.h"

namespace numcpp {
/**
 * @brief Base class for all expressions and tensors. All subclasses inherits
 * from this class using the Curiously Recurring Template Pattern.
 *
 * @tparam Derived Derived tensor subclass.
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. It must be a positive integer.
 */
template <class Derived, class T, size_t Rank> class abstract_tensor {
public:
  /// Member types.
  typedef T value_type;
  static constexpr size_t rank = Rank;

  /**
   * @brief Return the element at the given position.
   */
  T operator[](const index_t<Rank> &index) const {
    return static_cast<const Derived &>(*this)[index];
  }

  /**
   * @brief Return the shape of the tensor.
   *
   * @param axis It is an optional parameter that changes the return value. If
   *             provided, returns the size along the given axis. Otherwise,
   *             returns a shape_t object with the shape of the tensor along all
   *             axes.
   */
  shape_t<Rank> shape() const {
    return static_cast<const Derived &>(*this).shape();
  }

  size_t shape(size_t axis) const {
    return static_cast<const Derived &>(*this).shape(axis);
  }

  /**
   * @brief Return the number of elements in the tensor (i.e., the product of
   * the sizes along all the axes).
   */
  size_t size() const { return static_cast<const Derived &>(*this).size(); }

  /**
   * @brief Return the memory layout in which elements are stored.
   */
  layout_t layout() const {
    return static_cast<const Derived &>(*this).layout();
  }

  /**
   * @brief Return the derived subclass.
   */
  Derived &self() { return static_cast<Derived &>(*this); }

  const Derived &self() const { return static_cast<const Derived &>(*this); }
};
} // namespace numcpp

#endif // NUMCPP_ABSTRACT_TENSOR_H_INCLUDED
