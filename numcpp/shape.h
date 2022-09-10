/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
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

#if __cplusplus < 201103L
#error This file requires compiler and library support for the ISO C++ 2011 \
standard. This support must be enabled with the -std=c++11 or -std=gnu++11 \
compiler options.
#else

#include <cstddef>
#include <type_traits>

namespace numcpp {
    /// Forward declarations.
    using std::size_t;
    using std::ptrdiff_t;
    using std::nullptr_t;

    template <size_t Rank> class shape_t;

    template <size_t Rank> using index_t = shape_t<Rank>;

/// Namespace for implementation details.
namespace detail {
    /// Check whether a parameter pack consists of integers.
    template <class... Args> struct are_integral;

    template <class T>
    struct are_integral<T> : std::is_integral<T> {};

    template <class T, class... Args>
    struct are_integral<T, Args...>
     : std::integral_constant<
        bool, std::is_integral<T>::value && are_integral<Args...>::value
      > {};

    /// Rank of concatenation.
    template <class... Args>
    struct concatenation_rank;

    template <size_t Rank>
    struct concatenation_rank<shape_t<Rank> >
     : std::integral_constant<size_t, Rank> {};

    template <size_t Rank, class... Args>
    struct concatenation_rank<shape_t<Rank>, Args...>
     : std::integral_constant<size_t, Rank + concatenation_rank<Args...>::value>
     {};

    /// Type constraint to request N arguments.
    template <size_t N, class... Args>
    using RequiresNArguments =
        typename std::enable_if<sizeof...(Args) == N, bool>::type;

    /// Type constraint to request integer arguments.
    template <class... Args>
    using RequiresIntegral =
        typename std::enable_if<are_integral<Args...>::value, bool>::type;
}

    /**
     * @brief A shape_t is a class that identifies the size of a tensor along
     * each dimension.
     *
     * @tparam Rank Dimension of the shape. It must be a positive integer.
     */
    template <size_t Rank>
    class shape_t {
        static_assert(Rank > 0, "Rank must be positive");

    public:
        /**
         * @brief Default constructor. Set all sizes to zero.
         */
        shape_t();

        /**
         * @brief Construct a new shape_t object from a list of sizes.
         *
         * @param args... Size arguments.
         */
        template <class... Args,
                  typename = detail::RequiresNArguments<Rank, Args...>,
                  typename = detail::RequiresIntegral<Args...> >
        shape_t(Args... args);

        /**
         * @brief Copy constructor.
         *
         * @param other A shape_t object with the same dimension to copy.
         */
        shape_t(const shape_t &other);

        /**
         * @brief Copy assignment.
         *
         * @param other A shape_t object with the same dimension to copy.
         *
         * @return *this
         */
        shape_t& operator=(const shape_t &other);

        /**
         * @brief Integer conversion. Dimension must be one.
         */
        template <class IntegralType,
                  detail::RequiresIntegral<IntegralType> = true>
        explicit operator IntegralType() const;

        /**
         * @brief Pointer to size_t conversion.
         */
        explicit operator size_t*();
        explicit operator const size_t*() const;

        /**
         * @brief Return the dimension of the shape.
         */
        static constexpr size_t ndim();

        /**
         * @brief Return the product of the sizes along all the axes.
         *
         * @note Time complexity: O(Rank)
         */
        size_t size() const;

        /**
         * @brief Return the size of the i-th axis.
         *
         * @param i Axis index.
         *
         * @return If the shape_t is const-qualified, return a copy of the size.
         *     Otherwise, return a reference to the size along the i-th axis.
         */
        size_t& operator[](size_t i);
        size_t operator[](size_t i) const;

        /**
         * @brief Return a copy with the axes in reversed order.
         */
        shape_t transpose() const;

    private:
        // Shape elements.
        size_t m_shape[Rank];
    };

    /**
     * @brief Create a shape_t object deducing its dimension from the number of
     * arguments.
     *
     * @param args... Size arguments.
     *
     * @return A shape with the given values.
     */
    template <class... Args, detail::RequiresIntegral<Args...> = true>
    inline shape_t<sizeof...(Args)> make_shape(Args... args);

    /**
     * @brief Create an index_t object deducing its dimension from the number of
     * arguments.
     *
     * @note index_t is just an alias of shape_t defined to distinguish between
     * shapes and indices, improving readability.
     *
     * @param args... Index arguments.
     *
     * @return An index with the given values.
     */
    template <class... Args, detail::RequiresIntegral<Args...> = true>
    inline index_t<sizeof...(Args)> make_index(Args... args);

    /**
     * @brief Layout in which elements are stored or iterated.
     */
    enum layout_t {
        /**
         * @brief Row-major order (C/C++ style).
         * In row-major order, the last dimension is contiguous, so that the
         * memory offset of each axis is a constant multiple of the following
         * axis.
         * In row-major iteration, the last index is varying the fastest.
         */
        row_major = 1,

        /**
         * @brief Column-major order (Fortran/Matlab style).
         * In column-major order, the first dimension is contiguous, so that
         * the memory offset of each axis is a constant multiple of the
         * previous axis.
         * In column-major iteration, the first index is varying the fastest.
         */
        col_major = 0
    };

    /**
     * @brief Return a tuple of strides to offset a contiguous memory array as
     * a multidimensional array. The elements in the array can be offset by
     *     data[index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
     * where data is the memory array.
     *
     * @param shape The shape of the tensor.
     * @param order Determines whether the strides should be computed for
     *     row-major or column-major order. Defaults to row-major order.
     *
     * @return The strides for each dimension.
     */
    template <size_t Rank>
    shape_t<Rank> make_strides(
        const shape_t<Rank> &shape, layout_t order = row_major
    );

    /**
     * @brief Converts a tuple of indices into a flat index.
     *
     * @param index A tuple of indices to flatten.
     * @param shape The shape of the tensor used for raveling.
     * @param order Determines whether the indices should be viewed as indexing
     *     in row-major or column-major order. Defaults to row-major order.
     *
     * @return The flattened index.
     */
    template <size_t Rank>
    size_t ravel_index(
        const index_t<Rank> &index, const shape_t<Rank> &shape,
        layout_t order = row_major
    );

    /**
     * @brief Converts a flat index into a tuple of indices.
     *
     * @param index Index to unravel.
     * @param shape The shape of the tensor used for unraveling.
     * @param order Determines whether the indices should be viewed as indexing
     *     in row-major or column-major order. Defaults to row-major order.
     *
     * @return The unraveled index.
     */
    template <size_t Rank>
    index_t<Rank> unravel_index(
        size_t index, const shape_t<Rank> &shape, layout_t order = row_major
    );

    /**
     * @brief Broadcast input shapes into a common shape. Throws a
     * std::invalid_argument exception if the shapes are not compatible and
     * cannot be broadcasted according to broadcasting rules.
     *
     * @details Two dimensions are said to be compatible if
     *   - they are equal or,
     *   - one of them is 1
     * The size of the resulting broadcasting is the size that is not 1 along
     * each axis of the shapes.
     */
    template <size_t Rank, class... Args>
    shape_t<Rank> broadcast_shapes(
        const shape_t<Rank> &shape1, const Args&... shapes
    );

    /**
     * @brief Concatenates one or more shape_t objects.
     */
    template <size_t Rank, class... Args>
    shape_t<detail::concatenation_rank<shape_t<Rank>, Args...>::value>
    shape_cat(
        const shape_t<Rank> &shape1, const Args&... shapes
    );

    /**
     * @brief Compares two shape_t objects. Returns true if they have the same
     * dimension and the same sizes along each axis.
     */
    template <size_t Rank1, size_t Rank2>
    inline bool operator==(
        const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2
    );

    /**
     * @brief Compares two shape_t objects. Returns true if they have different
     * dimensions or they are different in at least one axis.
     */
    template <size_t Rank1, size_t Rank2>
    inline bool operator!=(
        const shape_t<Rank1> &shape1, const shape_t<Rank2> &shape2
    );
}

#include "numcpp/tensor/shape.tcc"

#endif // C++11
#endif // NUMCPP_SHAPE_H_INCLUDED
