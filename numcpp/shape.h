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

/** @file include/numcpp/shape.h
 *  This header defines the shape_t and index_t classes.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_SHAPE_H_INCLUDED
#define NUMCPP_SHAPE_H_INCLUDED

#include "numcpp/config.h"

namespace numcpp {
/**
 * @brief A shape_t is a class that identifies the size of a tensor along each
 * dimension.
 *
 * @tparam Rank Dimension of the shape. It must be a positive integer.
 */
template <size_t Rank> class shape_t {
public:
  static_assert(Rank > 0, "Rank must be positive");
  static_assert(Rank <= 32, "Maximum supported Rank is 32");

  /// Member types.
  typedef size_t size_type;

  /// Constructors.

  /**
   * @brief Default constructor. Constructs a shape_t object with its elements
   * initialized to zero.
   */
  shape_t();

  /**
   * @brief Initialization constructor. Initializes each element with the
   * corresponding element in @a sizes.
   *
   * @param sizes... Size along each axis.
   */
  template <class... Sizes, detail::RequiresNArguments<Rank, Sizes...> = 0,
            detail::RequiresIntegral<Sizes...> = 0>
  shape_t(Sizes... sizes);

  /**
   * @brief Copy constructor. The object is initialized with the contents of
   * @a other.
   *
   * @param other A shape_t object with the same dimension to copy.
   */
  shape_t(const shape_t &other);

  /// Assignment operator.

  /**
   * @brief Copy assignment.
   *
   * @param other A shape_t object with the same dimension to copy.
   *
   * @return *this
   */
  shape_t &operator=(const shape_t &other);

  /// Public methods.

  /**
   * @brief Return the dimension of the shape.
   */
  static constexpr size_t ndim();

  /**
   * @brief Return the product of the sizes along all the axes.
   */
  size_t prod() const;

  /**
   * @brief Return a pointer to the block of memory containing the elements of
   * the shape.
   */
  size_t *data();
  const size_t *data() const;

  /// Operator overloading.

  /**
   * @brief Return the size of the i-th axis.
   *
   * @param i Axis index.
   *
   * @return The size along the i-th axis. If the shape_t is const-qualified,
   *         the function returns a reference to const size_t. Otherwise, it
   *         returns a reference to size_t.
   */
  size_t &operator[](size_t i);
  const size_t &operator[](size_t i) const;

  /**
   * @brief Integer conversion. Dimension must be one.
   */
  template <class IntegralType, detail::RequiresIntegral<IntegralType> = 0>
  explicit operator IntegralType() const;

private:
  // Shape elements.
  size_t m_shape[Rank];
};

/**
 * @brief Create a shape_t object deducing its dimension from the number of
 * arguments.
 *
 * @param sizes... Size along each axis.
 *
 * @return A shape with the given values.
 */
template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
inline shape_t<sizeof...(Sizes)> make_shape(Sizes... sizes);

/**
 * @brief Create an index_t object deducing its dimension from the number of
 * arguments.
 *
 * @note index_t is just an alias of shape_t defined to distinguish between
 * shapes and indices, improving readability.
 *
 * @param indices... Index along each axis.
 *
 * @return An index with the given values.
 */
template <class... Indices, detail::RequiresIntegral<Indices...> = 0>
inline index_t<sizeof...(Indices)> make_index(Indices... indices);

/**
 * @brief Return a tuple of strides to offset a contiguous memory array as a
 * multidimensional array. The elements in the array can be offset by
 *     data[index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
 * where data is the memory array.
 *
 * @param shape The shape of the tensor.
 * @param order Determines whether the strides should be computed for row-major
 *              or column-major order. Defaults to row-major order.
 *
 * @return The strides for each dimension.
 */
template <size_t Rank>
shape_t<Rank> make_strides(const shape_t<Rank> &shape,
                           layout_t order = default_layout);

/**
 * @brief Converts a tuple of indices into a flat index.
 *
 * @param index A tuple of indices to flatten.
 * @param shape The shape of the tensor used for raveling.
 * @param order Determines whether the indices should be viewed as indexing in
 *              row-major or column-major order. Defaults to row-major order.
 *
 * @return The flattened index.
 */
template <size_t Rank>
size_t ravel_index(const index_t<Rank> &index, const shape_t<Rank> &shape,
                   layout_t order = default_layout);

/**
 * @brief Converts a flat index into a tuple of indices.
 *
 * @param index Index to unravel.
 * @param shape The shape of the tensor used for unraveling.
 * @param order Determines whether the indices should be viewed as indexing in
 *              row-major or column-major order. Defaults to row-major order.
 *
 * @return The unraveled index.
 */
template <size_t Rank>
index_t<Rank> unravel_index(size_t index, const shape_t<Rank> &shape,
                            layout_t order = default_layout);

/**
 * @brief Broadcast input shapes into a common shape.
 *
 * @details Two dimensions are said to be compatible if
 *   - they are equal or,
 *   - one of them is 1
 * The size of the resulting broadcasting is the size that is not 1 along each
 * axis of the shapes.
 *
 * @param shape1, shape2... The shapes to be broadcast against each other. The
 *                          shapes must have the same dimension.
 *
 * @return Broadcasted shape.
 *
 * @throw std::invalid_argument Thrown if the shapes are not compatible and
 *                              cannot be broadcasted according to broadcasting
 *                              rules.
 */
template <size_t Rank, class... Shapes>
shape_t<Rank> broadcast_shapes(const shape_t<Rank> &shape1,
                               const Shapes &...shape2);

/**
 * @brief Constructs a shape that is the concatenation of one or more shapes.
 *
 * @param shape1, shape2... The shapes to concatenate.
 *
 * @return The concatenated shape.
 */
template <size_t Rank, class... Shapes>
shape_t<detail::concatenation_rank<shape_t<Rank>, Shapes...>::value>
shape_cat(const shape_t<Rank> &shape1, const Shapes &...shape2);

/**
 * @brief Compares if two shapes are equal. Returns true if they have the same
 * dimension and the same size along each axis.
 */
template <size_t Rank1, size_t Rank2>
inline bool operator==(const shape_t<Rank1> &shape1,
                       const shape_t<Rank2> &shape2);

/**
 * @brief Compares if two shapes are not equal. Returns true if they have
 *  different dimensions or if they have different sizes along an axis.
 */
template <size_t Rank1, size_t Rank2>
inline bool operator!=(const shape_t<Rank1> &shape1,
                       const shape_t<Rank2> &shape2);
} // namespace numcpp

#include "numcpp/tensor/shape.tcc"

#endif // NUMCPP_SHAPE_H_INCLUDED
