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

    /// Type constraint to request N arguments.
    template <size_t N, class... Args>
    using RequiresNArguments =
        typename std::enable_if<sizeof...(Args) == N, bool>::type;

    /// Type constraint to request integer arguments.
    template <class... Args>
    using RequiresIntegral =
        typename std::enable_if<are_integral<Args...>::value, bool>::type;
}

    /// Forward declarations.
    using std::size_t;
    using std::ptrdiff_t;
    using std::nullptr_t;

    template <size_t Rank> class shape_t;

    template <size_t Rank> using index_t = shape_t<Rank>;

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
         * @brief Integer assignment.
         *
         * @param val An integer to copy. Dimension must be one.
         *
         * @return *this
         */
        shape_t& operator=(size_t val);

        /**
         * @brief Integer conversion. Dimension must be one.
         */
        operator size_t();

        /**
         * @brief Return the dimension of the shape.
         */
        constexpr size_t ndim() const;

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
        /**
         * @brief Fill a shape_t from variadic arguments.
         */
        void __initialize(size_t arg0);

        template <class... Args>
        void __initialize(size_t arg0, Args... args);

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
     * @details index_t is just an alias of shape_t defined to distinguish
     * between shapes and indices, improving readability.
     *
     * @param args... Index arguments.
     *
     * @return An index with the given values.
     */
    template <class... Args, detail::RequiresIntegral<Args...> = true>
    inline index_t<sizeof...(Args)> make_index(Args... args);

    /**
     * @brief Converts a tuple of indices into a flat index.
     *
     * @param index A tuple of indices to flatten.
     * @param shape The shape of the tensor used for raveling.
     * @param order Determines whether the indices should be viewed as indexing
     *     in row-major order (true) or column-major order (false). Defaults to
     *     row-major order.
     *
     * @return The flattened index.
     */
    template <size_t Rank>
    size_t ravel_index(
        const index_t<Rank> &index, const shape_t<Rank> &shape,
        bool order = true
    );

    /**
     * @brief Converts a flat index into a tuple of indices.
     *
     * @param index Index to unravel.
     * @param shape The shape of the tensor used for unraveling.
     * @param order Determines whether the indices should be viewed as indexing
     *     in row-major order (true) or column-major order (false). Defaults to
     *     row-major order.
     *
     * @return The unraveled index.
     */
    template <size_t Rank>
    index_t<Rank> unravel_index(
        size_t index, const shape_t<Rank> &shape, bool order = true
    );

    /**
     * @brief Broadcast two shapes into a common shape. Throws a
     * std::invalid_argument exception if the shapes are not compatible and
     * cannot be broadcasted according to broadcasting rules.
     *
     * @details Two dimensions are said to be compatible if
     *   - they are equal or,
     *   - one of them is 1
     * The size of the resulting broadcasting is the size that is not 1 along
     * each axis of the shapes.
     */
    template <size_t Rank>
    shape_t<Rank> broadcast_shapes(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2
    );

    /**
     * @brief Asserts whether an index is within the bounds of a tensor. Throws
     * an std::out_of_range exception if assertion fails.
     */
    inline void assert_within_bounds(size_t size, size_t i);

    template <size_t Rank>
    inline void assert_within_bounds(
        const shape_t<Rank> &shape, const index_t<Rank> &index
    );

    template <size_t Rank>
    inline void assert_within_bounds(
        const shape_t<Rank> &shape, size_t index, size_t axis
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
