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

/** @file include/numcpp/broadcasting/assert.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ASSERT_H_INCLUDED
#define NUMCPP_ASSERT_H_INCLUDED

#include <sstream>
#include <stdexcept>

namespace numcpp {
/// Namespace for implementation details.
namespace detail {
/**
 * @brief Asserts whether an index is within the bounds of a tensor. Throws a
 * std::out_of_range exception if assertion fails.
 */
inline void assert_within_bounds(size_t size, size_t i) {
  if (i >= size) {
    std::ostringstream error;
    error << "index " << i << " is out of bounds with size " << size;
    throw std::out_of_range(error.str());
  }
}

template <size_t Rank>
inline void assert_within_bounds(const shape_t<Rank> &shape,
                                 const index_t<Rank> &index) {
  for (size_t i = 0; i < Rank; ++i) {
    if (index[i] >= shape[i]) {
      std::ostringstream error;
      error << "index " << index << " is out of bounds with size " << shape;
      throw std::out_of_range(error.str());
    }
  }
}

template <size_t Rank>
inline void assert_within_bounds(const shape_t<Rank> &shape, size_t index,
                                 size_t axis) {
  if (index >= shape[axis]) {
    std::ostringstream error;
    error << "index " << index << " is out of bounds for axis " << axis
          << " with size " << shape[axis];
    throw std::out_of_range(error.str());
  }
}

/**
 * @brief Asserts whether the output shape matches the input shape. Throws a
 * std::invalid_argument exception if assertion fails.
 */
template <size_t Rank>
void assert_output_shape(const shape_t<Rank> &output,
                         const shape_t<Rank> &input) {
  if (output != input) {
    std::ostringstream error;
    error << "non-broadcastable output operand with shape " << output
          << " doesn't match the broadcast shape " << input;
    throw std::invalid_argument(error.str());
  }
}

void assert_output_shape(size_t output, size_t input) {
  if (output != input) {
    std::ostringstream error;
    error << "non-broadcastable output operand with shape " << output
          << " doesn't match the broadcast shape " << input;
    throw std::invalid_argument(error.str());
  }
}

/**
 * @brief Asserts whether the shape of the boolean mask matches the shape of the
 * indexed tensor. Throws a std::invalid_argument exception if assertion fails.
 */
template <size_t Rank>
void assert_mask_shape(const shape_t<Rank> &shape,
                       const shape_t<Rank> &mask_shape) {
  if (shape != mask_shape) {
    std::ostringstream error;
    error << "boolean index did not match indexed tensor; shape is " << shape
          << " but corresponding boolean shape is " << mask_shape;
    throw std::invalid_argument(error.str());
  }
}

void assert_mask_shape(size_t size, size_t mask_size, size_t axis) {
  if (size != mask_size) {
    std::ostringstream error;
    error << "boolean index did not match indexed tensor along dimension "
          << axis << "; dimension is " << size
          << " but corresponding boolean dimension is " << mask_size;
    throw std::invalid_argument(error.str());
  }
}

/**
 * @brief Asserts whether the shape of the tensor of indices matches the shape
 * of the indexed tensor for all dimensions other than axis. Throws a
 * std::invalid_argument exception if assertion fails.
 */
template <size_t Rank>
void assert_index_along_axis_shape(const shape_t<Rank> &shape,
                                   const shape_t<Rank> &indices_shape,
                                   size_t axis) {
  for (size_t i = 0; i < Rank; ++i) {
    if (indices_shape[i] != shape[i] && i != axis) {
      std::ostringstream error;
      error << "index did not match indexed tensor along dimension " << i
            << "; dimension is " << shape[i]
            << " but corresponding index dimension is " << indices_shape[i];
      throw std::invalid_argument(error.str());
    }
  }
}
} // namespace detail
} // namespace numcpp

#endif // NUMCPP_ASSERT_H_INCLUDED
