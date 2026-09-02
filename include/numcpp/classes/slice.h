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

/** @file include/numcpp/tensor/slice.h
 *  This header defines slice class.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_SLICE_H_INCLUDED
#define NUMCPP_SLICE_H_INCLUDED

#include <concepts>
#include <cstdint>
#include <optional>

namespace numcpp {
/**
 * @brief A slice is a class that identifies a subset of elements in an array.
 * It holds three values: the starting index, the stop index and the stride.
 */
class slice {
 public:
  /// Constructors.

  /**
   * @brief Default constructor. Constructs an empty slice that includes all
   * the elements in an array.
   */
  slice() : m_start(0), m_stop(), m_stride(1) {}

  /**
   * @brief Slice constructor. Constructs a slice with values within the
   * half-open interval [0, stop).
   *
   * @param stop The position at which the slice ends. The slice does not
   * include this position.
   */
  explicit slice(ptrdiff_t stop) : m_start(0), m_stop(stop), m_stride(1) {}

  /**
   * @brief Slice constructor. Constructs a slice with values within the
   * half-open interval [start, stop).
   *
   * @param start The position of the first element selected by the slice.
   * @param stop The position at which the slice ends. The slice does not
   * include this position.
   */
  explicit slice(std::optional<ptrdiff_t> start, std::optional<ptrdiff_t> stop)
      : m_start(start), m_stop(stop), m_stride(1) {}

  /**
   * @brief Slice constructor. Constructs a slice with values within the
   * half-open interval [start, stop) and given step.
   *
   * @param start The position of the first element selected by the slice.
   * @param stop The position at which the slice ends. The slice does not
   * include this position.
   * @param stride The span that separates the elements selected by the slice.
   */
  explicit slice(std::optional<ptrdiff_t> start, std::optional<ptrdiff_t> stop,
                 ptrdiff_t stride)
      : m_start(start), m_stop(stop), m_stride(stride) {}

  /// Public methods.

  /**
   * @brief Return whether the slice has a start value.
   */
  bool has_start() const { return m_start.has_value(); }

  /**
   * @brief Return the first element in the slice. Throws a std::bad_optional
   * exception if the slice doesn't have a start value.
   */
  ptrdiff_t start() const { return *m_start; }

  /**
   * @brief Return whether the slice has a stop value.
   */
  bool has_stop() const { return m_stop.has_value(); }

  /**
   * @brief Return the past-the-end element in the slice. The slice does not
   * include this position. Throws a std::bad_optional exception if the slice
   * doesn't have a stop value.
   */
  ptrdiff_t stop() const { return *m_stop; }

  /**
   * @brief Return the separation of the elements in the slice.
   */
  ptrdiff_t stride() const { return m_stride; }

  /**
   * @brief Return a slice with start and stop values computed from the size
   * of an array.
   *
   * @details If the slice does not have start and stop values, they are
   * computed from the size of the array as follows:
   * - slice(_, _, stride) converts to slice(0, size, stride) if stride > 0
   * and to slice(size - 1, -1, stride) if stride < 0.
   * - slice(_, stop, stride) converts to slice(0, stop, stride) if stride > 0
   * and to slice(size - 1, stop, stride) if stride < 0.
   * - slice(start, _, stride) converts to slice(start, size, stride) if
   * stride > 0 and to slice(start, -1, stride) if stride < 0.
   *
   * @param size Size of the array to slice.
   */
  slice bind_array(size_t size) const {
    ptrdiff_t start, stop;
    if (has_start()) {
      start = *m_start;
      if (start < 0) {
        start += size;
      }
    } else {
      start = (m_stride >= 0) ? 0 : size - 1;
    }

    if (has_stop()) {
      stop = *m_stop;
      if (stop + size < 0) {
        stop = 0;
      } else if (stop < 0) {
        stop += size;
      } else if (stop > size) {
        stop = size;
      }
    } else {
      stop = (m_stride >= 0) ? size : -1;
    }

    return slice(start, stop, m_stride);
  }

 private:
  // Start.
  std::optional<ptrdiff_t> m_start;

  // Stop
  std::optional<ptrdiff_t> m_stop;

  // Stride.
  ptrdiff_t m_stride;
};

/**
 * Concept for integer or slice.
 */
template <class T>
concept integer_or_slice = std::is_same_v<T, slice> || std::integral<T>;
}  // namespace numcpp

#endif // NUMCPP_SLICE_H_INCLUDED
