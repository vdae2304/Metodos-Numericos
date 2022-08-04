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

#include "numcpp/shape.h"
#include "numcpp/slice.h"
#include "numcpp/broadcasting/operators.h"
#include "numcpp/routines/ranges.h"

#include "numcpp/tensor/tensor_interface.h"
#include "numcpp/broadcasting/lazy_tensor.h"
#include "numcpp/tensor/tensor_view.h"
#include "numcpp/tensor/indirect_tensor.h"

#include <initializer_list>

namespace numcpp {
/// Namespace for implementation details.
namespace detail {
    /// Type constraint to request input iterator.
    template <class Iterator>
    using RequiresInputIterator = typename std::enable_if<
        std::is_convertible<
            typename std::iterator_traits<Iterator>::iterator_category,
            std::input_iterator_tag
        >::value
    >::type;

    /// Create a depth-level initializer_list.
    template <class T, size_t Depth>
    struct nested_initializer_list {
        typedef std::initializer_list<
            typename nested_initializer_list<T, Depth - 1>::type
        > type;
    };

    template <class T>
    struct nested_initializer_list<T, 0> {
        typedef T type;
    };

    template <class T, size_t Depth>
    using RequiresDepthInitializerList
        = typename nested_initializer_list<T, Depth>::type;

    /// Return the number of slice arguments.
    template <class... Args>
    struct slice_index_rank {};

    template <>
    struct slice_index_rank<> {
        constexpr static size_t value = 0;
    };

    template <class IntegralType, class... Args>
    struct slice_index_rank<IntegralType, Args...> {
        constexpr static typename std::enable_if<
            std::is_integral<IntegralType>::value, size_t
        >::type value = slice_index_rank<Args...>::value;
    };

    template <class... Args>
    struct slice_index_rank<slice, Args...> {
        constexpr static size_t value = 1 + slice_index_rank<Args...>::value;
    };

    /// Type constraint to request at least one slice argument.
    template <class... Args>
    using RequiresSlice = typename std::enable_if<
        (slice_index_rank<Args...>::value > 0), bool
    >::type;
}

    /**
     * @brief Tensors are contiguous multidimensional sequence containers: they
     * hold a specific number of elements arranged in multiple axis. Unlike
     * a tensor_view, a tensor is always owner of its own data and the elements
     * are always stored in row-major order.
     *
     * Tensors are designed to easily perform mathematical operations on them.
     * Most mathematical operations can be applied directly to tensor objects,
     * affecting all its elements.
     *
     * @tparam T Type of the elements contained in the tensor. This shall be an
     *     arithmetic type or a class that behaves like one (such as
     *     std::complex).
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     */
    template <class T, size_t Rank>
    class base_tensor<T, Rank, tensor_tag>
     : public tensor_interface<T, Rank, tensor_tag> {
    public:
        /// Member types.
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Default constructor. Constructs an empty tensor with no
         * elements.
         */
        base_tensor();

        /**
         * @brief Size constructor. Constructs a tensor with given shape, each
         * element is left uninitialized.
         *
         * @param shape Number of elements along each axis. It can be a shape_t
         *     object or the elements of the shape passed as separate arguments.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_tensor(const shape_t<Rank> &shape);

        template <class... Args,
                  typename = detail::RequiresNArguments<Rank, Args...>,
                  typename = detail::RequiresIntegral<Args...> >
        base_tensor(Args... args);

        /**
         * @brief Fill constructor. Constructs a tensor with given shape, each
         * element initialized to val.
         *
         * @param shape Number of elements along each axis.
         * @param val Value to which each of the elements is initialized.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_tensor(const shape_t<Rank> &shape, const T &val);

        /**
         * @brief Range constructor. Constructs a tensor with given shape, with
         * each element constructed from its corresponding element in the range
         * starting at first, in the same order.
         *
         * @param shape Number of elements along each axis.
         * @param first Input iterator to the initial position in a range.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class InputIterator,
                  typename = detail::RequiresInputIterator<InputIterator> >
        base_tensor(const shape_t<Rank> &shape, InputIterator first);

        /**
         * @brief Copy constructor. Constructs a tensor with a copy of each of
         * the elements in other, in the same order.
         *
         * @param other A tensor-like object of the same rank.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_tensor(const base_tensor &other);
        template <class U, class Tag>
        base_tensor(const base_tensor<U, Rank, Tag> &other);

        /**
         * @brief Move constructor. Constructs a tensor that acquires the
         * elements of other.
         *
         * @param other A tensor of the same type and rank. The ownership is
         *     directly transferred from other. other is left in an empty
         *     state.
         */
        base_tensor(base_tensor &&other);

        /**
         * @brief Initializer list constructor. Constructs a tensor with a copy
         * of each of the elements in il, in the same order.
         *
         * @param il An initializer_list object.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_tensor(detail::RequiresDepthInitializerList<T, Rank> il);

        /// Destructor.
        ~base_tensor();

        /// Indexing.

        /**
         * @brief Call operator. Returns a reference to the element at the
         * given position.
         *
         * @param args... Index arguments.
         *
         * @return The element at the specified position. If the tensor is
         *     const-qualified, the function returns a reference to const T.
         *     Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if index is out of bounds.
         */
        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        T& operator()(Args... args);

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        const T& operator()(Args... args) const;

        /**
         * @brief Subscript operator. Returns a reference to the element at the
         * given position.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor. If the tensor is one dimensional, an integer can be
         *     used instead.
         *
         * @return The element at the specified position. If the tensor is
         *     const-qualified, the function returns a reference to const T.
         *     Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if index is out of bounds.
         */
        T& operator[](const index_t<Rank> &index);
        const T& operator[](const index_t<Rank> &index) const;

        T& operator[](size_t i);
        const T& operator[](size_t i) const;

        /**
         * @brief Slice indexing. Returns a tensor_view object that selects the
         * elements given by the slices.
         *
         * @param args... Index arguments. Each argument can be either an
         *     integer or a slice. If a slice is given, select multiple
         *     positions along the corresponding axis. An empty slice can be
         *     used to select all the positions along the axis.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor. The
         *     dimension of the tensor_view is the number of slices in the
         *     index arguments.
         *
         * @throw std::out_of_range Thrown if an integral index is out of
         *     bounds.
         */
        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresSlice<Args...> = true>
        tensor_view<T, detail::slice_index_rank<Args...>::value>
        operator()(Args... args);

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresSlice<Args...> = true>
        tensor_view<const T, detail::slice_index_rank<Args...>::value>
        operator()(Args... args) const;

        /**
         * @brief Coordinate tensor indexing. Returns an indirect_tensor that
         * selects the elements specified by the tensor of indices.
         *
         * @param index A tensor-like object of index_t with its elements
         *     identifying which elements of the tensor are selected. If the
         *     tensor is one dimensional, a tensor-like object of integers can
         *     be used instead.
         *
         * @return If the tensor is const-qualified, the function returns a new
         *     tensor object with a copy of the selection. Otherwise, the
         *     function returns an indirect_tensor, which has reference
         *     semantics to the original tensor.
         *
         * @throw std::out_of_range Thrown if the indices references to an
         *     element out of bounds.
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         */
        template <size_t N, class Tag>
        indirect_tensor<T, N> operator[](
            const base_tensor<index_t<Rank>, N, Tag> &index
        );
        template <size_t N, class Tag>
        tensor<T, N> operator[](
            const base_tensor<index_t<Rank>, N, Tag> &index
        ) const;

        template <class IntegralType, size_t N, class Tag,
                  detail::RequiresIntegral<IntegralType> = true>
        indirect_tensor<T, N> operator[](
            const base_tensor<IntegralType, N, Tag> &index
        );
        template <class IntegralType, size_t N, class Tag,
                  detail::RequiresIntegral<IntegralType> = true>
        tensor<T, N> operator[](
            const base_tensor<IntegralType, N, Tag> &index
        ) const;

        /**
         * @brief Boolean tensor indexing. Returns an indirect_tensor that
         * selects the elements specified by the boolean mask.
         *
         * @param mask A tensor-like object of bool with its elements
         *     identifying whether each element of the tensor is selected or
         *     not.
         *
         * @return If the tensor is const-qualified, the function returns a new
         *     tensor object with a copy of the selection. Otherwise, the
         *     function returns an indirect_tensor, which has reference
         *     semantics to the original tensor.
         *
         * @throw std::invalid_argument Thrown if the tensor and the boolean
         *     mask have different shapes.
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         */
        template <class Tag>
        indirect_tensor<T, 1> operator[](
            const base_tensor<bool, Rank, Tag> &mask
        );
        template <class Tag>
        tensor<T, 1> operator[](
            const base_tensor<bool, Rank, Tag> &mask
        ) const;

        /**
         * @brief Return the dimension of the tensor.
         */
        constexpr size_t ndim() const;

        /**
         * @brief Return the shape of the tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        const shape_t<Rank>& shape() const;
        size_t shape(size_t axis) const;

        /**
         * @brief Return the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         *
         * @note Time complexity: O(1)
         */
        size_t size() const;

        /**
         * @brief Returns whether the tensor is empty (i.e., whether its size
         * is 0).
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the
         * tensor. Because elements in the tensor are stored contiguously and
         * in row-major order, the pointer retrieved can be offset to access
         * any element in the tensor.
         *
         * @return A pointer to the memory array used internally by the tensor.
         *     If the tensor is const-qualified, the function returns a pointer
         *     to const T. Otherwise, it returns a pointer to T.
         */
        T* data();
        const T* data() const;

        /**
         * @brief Returns whether the elements are stored in row-major order.
         * For tensor objects, always returns true.
         */
        bool rowmajor() const;

        /**
         * @brief Returns whether the elements are stored in column-major
         * order. For tensor objects, always returns false.
         */
        bool colmajor() const;

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns to each element the value of the
         * corresponding element in other, after resizing the object (if
         * necessary).
         *
         * @param other A tensor-like object.
         *
         * @return *this
         *
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         *
         * @warning When the shapes do not mach, invalidates all iterators,
         *     references and views to elements of the tensor. Otherwise, valid
         *     iterators, references and views keep their validity.
         */
        base_tensor& operator=(const base_tensor &other);
        template <class U, class Tag>
        base_tensor& operator=(const base_tensor<U, Rank, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element. The size of
         * the tensor is preserved.
         *
         * @param val Value assigned to all the elements in the tensor.
         *
         * @return *this
         */
        base_tensor& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving
         * other in an empty state.
         *
         * @param other A tensor of the same type and rank. The ownership is
         *     directly transferred from other. other is left in an empty
         *     state.
         *
         * @return *this
         */
        base_tensor& operator=(base_tensor &&other);

        /**
         * @brief Initializer list assignment. Assigns to each element the
         * value of the corresponding element in il after resizing the object
         * (if necessary).
         *
         * @param il An initializer_list object.
         *
         * @return *this
         *
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         *
         * @warning When the shapes do not mach, invalidates all iterators,
         *     references and views to elements of the tensor. Otherwise, valid
         *     iterators, references and views keep their validity.
         */
        base_tensor&
        operator=(detail::RequiresDepthInitializerList<T, Rank> il);

        /// Public methods.

        /**
         * @brief Return a view of the diagonal.
         *
         * @param offset Offset of the diagonal from the main diagonal. A
         *    positive value refers to an upper diagonal and a negative value
         *    refers to a lower diagonal. Defaults to 0 (main diagonal).
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         */
        tensor_view<T, 1> diagonal(ptrdiff_t offset = 0);
        tensor_view<const T, 1> diagonal(ptrdiff_t offset = 0) const;

        /**
         * @brief Return a view of the tensor collapsed into one dimension.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         */
        tensor_view<T, 1> flatten();
        tensor_view<const T, 1> flatten() const;

        /**
         * @brief Return a tensor_view containing the same data with a new
         * shape.
         *
         * @param shape The new shape should be compatible with the original
         *     shape. It can be a shape_t object or the elements of the shape
         *     passed as separate arguments.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         *
         * @throw std::invalid_argument Thrown if the tensor could not reshaped.
         */
        template <size_t N>
        tensor_view<T, N> reshape(const shape_t<N> &shape);

        template <class... Args, detail::RequiresIntegral<Args...> = true>
        tensor_view<T, sizeof...(Args)> reshape(Args... args);

        template <size_t N>
        tensor_view<const T, N> reshape(const shape_t<N> &shape) const;

        template <class... Args, detail::RequiresIntegral<Args...> = true>
        tensor_view<const T, sizeof...(Args)> reshape(Args... args) const;

        /**
         * @brief Resizes the tensor in-place to a given shape. After resizing,
         * if the new size is different from the total number of elements in
         * the tensor, allocates memory for the new size losing the previous
         * contents. Otherwise, the contents are preserved, but possibly
         * aranged in a different order.
         *
         * @param shape New shape of the tensor. It can be a shape_t object or
         *     the elements of the shape passed as separate arguments.
         *
         * @warning Invalidates all iterators, references and views to elements
         *     of the tensor.
         */
        void resize(const shape_t<Rank> &shape);

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        void resize(Args... args);

        /**
         * @brief Removes axes of length one.
         *
         * @param axes Selects a subset of the entries of length one in the
         *     shape.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         *
         * @throw std::invalid_argument Thrown if an axis with shape entry
         *     greater than one is selected.
         */
        template <size_t N>
        tensor_view<T, Rank - N> squeeze(const shape_t<N> &axes);

        template <size_t N>
        tensor_view<const T, Rank - N> squeeze(const shape_t<N> &axes) const;

        /**
         * @brief Return a view of the tensor with its axes in reversed order.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         */
        tensor_view<T, Rank> t();
        tensor_view<const T, Rank> t() const;

        /**
         * @brief Return a view of the tensor with the same data.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         */
        tensor_view<T, Rank> view();
        tensor_view<const T, Rank> view() const;

    private:
        /**
         * @brief Get the shape of a nested initializer list recursively.
         */
        template <size_t Depth>
        typename std::enable_if<(Depth == 1)>::type
        __initializer_list_shape(
            shape_t<Rank> &shape, const std::initializer_list<T> &il
        );

        template <size_t Depth>
        typename std::enable_if<(Depth > 1)>::type
        __initializer_list_shape(
            shape_t<Rank> &shape,
            const detail::RequiresDepthInitializerList<T, Depth> &il
        );

        /**
         * @brief Fill the tensor with the elements from a nested
         * initializer_list recursively.
         */
        template <size_t Depth>
        typename std::enable_if<(Depth == 1)>::type
        __fill_from_initializer_list(
            index_t<Rank> &index, const std::initializer_list<T> &il
        );

        template <size_t Depth>
        typename std::enable_if<(Depth > 1)>::type
        __fill_from_initializer_list(
            index_t<Rank> &index,
            const detail::RequiresDepthInitializerList<T, Depth> &il
        );

        // Pointer to data.
        T *m_data;

        // Number of elements.
        size_t m_size;

        // Number of elements along each axis.
        shape_t<Rank> m_shape;
    };

    /**
     * @brief Each of this functions performs their respective operation on all
     * the elements in the tensor.
     *
     * When both the left-hand side and right-hand side arguments are tensor
     * objects, the operation is performed between the corresponding elements
     * in each object (the first element of the left one with the right one,
     * the second with the second, and so on...)
     *
     * When one of the arguments is a value, the operation is applied to all
     * the elements in the tensor against that value.
     *
     * @param lhs Left-hand side tensor-like object.
     * @param rhs Right-hand side tensor-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object which stores the result of performing the
     *     operation on each element. Each of these operators uses
     *     lazy-evaluation, meaning that the result of each operation is
     *     computed only at the end, when the whole expression is evaluated or
     *     assigned to a tensor object.
     *
     * @throw std::invalid_argument Thrown if the shapes are not compatible and
     *     cannot be broadcasted according to broadcasting rules.
     */

    /// Unary operators.

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<unary_plus, T, Tag> >
    operator+(const base_tensor<T, Rank, Tag> &arg) {
        typedef lazy_unary_tag<unary_plus, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(unary_plus(), arg);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<negate, T, Tag> >
    operator-(const base_tensor<T, Rank, Tag> &arg) {
        typedef lazy_unary_tag<negate, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(negate(), arg);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_unary_tag<bit_not, T, Tag> >
    operator~(const base_tensor<T, Rank, Tag> &arg) {
        typedef lazy_unary_tag<bit_not, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_not(), arg);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<bool, Rank, lazy_unary_tag<logical_not, T, Tag> >
    operator!(const base_tensor<T, Rank, Tag> &arg) {
        typedef lazy_unary_tag<logical_not, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(logical_not(), arg);
    }

    /// Arithmetic operators.

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<plus, T, Tag1, T, Tag2> >
    operator+(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<plus, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(plus(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_binary_tag<plus, T, Tag, T, scalar_tag> >
    operator+(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<plus, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(plus(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_binary_tag<plus, T, scalar_tag, T, Tag> >
    operator+(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<plus, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(plus(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<minus, T, Tag1, T, Tag2> >
    operator-(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<minus, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(minus(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_binary_tag<minus, T, Tag, T, scalar_tag> >
    operator-(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<minus, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(minus(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_binary_tag<minus, T, scalar_tag, T, Tag> >
    operator-(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<minus, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(minus(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<multiplies, T, Tag1, T, Tag2> >
    operator*(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<multiplies, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(multiplies(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<multiplies, T, Tag, T, scalar_tag>
    > operator*(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<multiplies, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(multiplies(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<multiplies, T, scalar_tag, T, Tag>
    > operator*(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<multiplies, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(multiplies(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<divides, T, Tag1, T, Tag2> >
    operator/(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<divides, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(divides(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<divides, T, Tag, T, scalar_tag>
    > operator/(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<divides, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(divides(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<divides, T, scalar_tag, T, Tag>
    > operator/(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<divides, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(divides(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<modulus, T, Tag1, T, Tag2> >
    operator%(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<modulus, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(modulus(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<modulus, T, Tag, T, scalar_tag>
    > operator%(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<modulus, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(modulus(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<modulus, T, scalar_tag, T, Tag>
    > operator%(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<modulus, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(modulus(), val, rhs);
    }

    /// Bitwise operators.

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<bit_and, T, Tag1, T, Tag2> >
    operator&(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<bit_and, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(bit_and(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<bit_and, T, Tag, T, scalar_tag>
    > operator&(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<bit_and, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_and(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<bit_and, T, scalar_tag, T, Tag>
    > operator&(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<bit_and, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_and(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<bit_or, T, Tag1, T, Tag2> >
    operator|(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<bit_or, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(bit_or(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_binary_tag<bit_or, T, Tag, T, scalar_tag> >
    operator|(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<bit_or, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_or(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, lazy_binary_tag<bit_or, T, scalar_tag, T, Tag> >
    operator|(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<bit_or, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_or(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<bit_xor, T, Tag1, T, Tag2> >
    operator^(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<bit_xor, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(bit_xor(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<bit_xor, T, Tag, T, scalar_tag>
    > operator^(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<bit_xor, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_xor(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<bit_xor, T, scalar_tag, T, Tag>
    > operator^(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<bit_xor, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(bit_xor(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<T, Rank, lazy_binary_tag<left_shift, T, Tag1, T, Tag2> >
    operator<<(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<left_shift, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(left_shift(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<left_shift, T, Tag, T, scalar_tag>
    > operator<<(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<left_shift, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(left_shift(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<left_shift, T, scalar_tag, T, Tag>
    > operator<<(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<left_shift, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(left_shift(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        T, Rank, lazy_binary_tag<right_shift, T, Tag1, T, Tag2>
    > operator>>(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<right_shift, T, Tag1, T, Tag2> Closure;
        return base_tensor<T, Rank, Closure>(right_shift(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<right_shift, T, Tag, T, scalar_tag>
    > operator>>(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<right_shift, T, Tag, T, scalar_tag> Closure;
        return base_tensor<T, Rank, Closure>(right_shift(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        T, Rank, lazy_binary_tag<right_shift, T, scalar_tag, T, Tag>
    > operator>>(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<right_shift, T, scalar_tag, T, Tag> Closure;
        return base_tensor<T, Rank, Closure>(right_shift(), val, rhs);
    }

    /// Logical operators.

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<logical_and, T, Tag1, T, Tag2>
    > operator&&(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<logical_and, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(logical_and(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<logical_and, T, Tag, T, scalar_tag>
    > operator&&(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<logical_and, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(logical_and(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<logical_and, T, scalar_tag, T, Tag>
    > operator&&(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<logical_and, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(logical_and(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<logical_or, T, Tag1, T, Tag2>
    > operator||(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<logical_or, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(logical_or(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<logical_or, T, Tag, T, scalar_tag>
    > operator||(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<logical_or, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(logical_or(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<logical_or, T, scalar_tag, T, Tag>
    > operator||(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<logical_or, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(logical_or(), val, rhs);
    }

    /// Relational operators.

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<bool, Rank, lazy_binary_tag<equal_to, T, Tag1, T, Tag2> >
    operator==(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<equal_to, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(equal_to(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<equal_to, T, Tag, T, scalar_tag>
    > operator==(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<equal_to, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(equal_to(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<equal_to, T, scalar_tag, T, Tag>
    > operator==(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<equal_to, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(equal_to(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<not_equal_to, T, Tag1, T, Tag2>
    > operator!=(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<not_equal_to, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(not_equal_to(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<not_equal_to, T, Tag, T, scalar_tag>
    > operator!=(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<not_equal_to, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(not_equal_to(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<not_equal_to, T, scalar_tag, T, Tag>
    > operator!=(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<not_equal_to, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(not_equal_to(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<bool, Rank, lazy_binary_tag<less, T, Tag1, T, Tag2> >
    operator<(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<less, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(less(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<less, T, Tag, T, scalar_tag>
    > operator<(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<less, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(less(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<less, T, scalar_tag, T, Tag>
    > operator<(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<less, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(less(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<bool, Rank, lazy_binary_tag<greater, T, Tag1, T, Tag2> >
    operator>(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<greater, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(greater(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<greater, T, Tag, T, scalar_tag>
    > operator>(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<greater, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(greater(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<greater, T, scalar_tag, T, Tag>
    > operator>(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<greater, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(greater(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<less_equal, T, Tag1, T, Tag2>
    > operator<=(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<less_equal, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(less_equal(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<less_equal, T, Tag, T, scalar_tag>
    > operator<=(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<less_equal, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(less_equal(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<less_equal, T, scalar_tag, T, Tag>
    > operator<=(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<less_equal, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(less_equal(), val, rhs);
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<greater_equal, T, Tag1, T, Tag2>
    > operator>=(
        const base_tensor<T, Rank, Tag1> &lhs,
        const base_tensor<T, Rank, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<greater_equal, T, Tag1, T, Tag2> Closure;
        return base_tensor<bool, Rank, Closure>(greater_equal(), lhs, rhs);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<greater_equal, T, Tag, T, scalar_tag>
    > operator>=(
        const base_tensor<T, Rank, Tag> &lhs,
        const typename tensor<T, Rank>::value_type &val
    ) {
        typedef lazy_binary_tag<greater_equal, T, Tag, T, scalar_tag> Closure;
        return base_tensor<bool, Rank, Closure>(greater_equal(), lhs, val);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<
        bool, Rank, lazy_binary_tag<greater_equal, T, scalar_tag, T, Tag>
    > operator>=(
        const typename tensor<T, Rank>::value_type &val,
        const base_tensor<T, Rank, Tag> &rhs
    ) {
        typedef lazy_binary_tag<greater_equal, T, scalar_tag, T, Tag> Closure;
        return base_tensor<bool, Rank, Closure>(greater_equal(), val, rhs);
    }

    /**
     * @brief Return the matrix multiplication of two matrices.
     *
     * @details The behaviour depends on the arguments in the following way.
     * - If both arguments are one-dimensional, it is the inner product of
     *   vectors (without complex conjugation).
     * - If both arguments are two-dimensional, it is the conventional matrix
     *   multiplication.
     * - If the first argument is one-dimensional, it is promoted to a matrix
     *   by prepending a 1 to its dimensions. After matrix multiplication the
     *   prepended 1 is removed.
     * - If the second argument is one-dimensional, it is promoted to a matrix
     *   by appending a 1 to its dimensions. After matrix multiplication the
     *   appended 1 is removed.
     * - If both arguments are n-dimensional, n > 2, it is treated as a stack
     *   of matrices residing in the last two indexes and broadcast
     *   accordingly.
     *
     * The inner product of two vectors @f$a = [a_1, a_2, \ldots, a_n]@f$ and
     * @f$b = [b_1, b_2, \ldots, b_n]@f$ is defined as
     * @f[
     *     a \cdot b = a_1b_1 + a_2b_2 + \ldots + a_nb_n
     * @f]
     * The matrix multiplication of a @f$m \times p@f$ matrix
     * @f$A = (a_{ij})@f$ and a @f$p \times n@f$ matrix @f$B = (b_{ij})@f$ is
     * the @f$m \times n@f$ matrix @f$C = (c_{ij})@f$ such that
     * @f[
     *     c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \ldots + a_{ip}b_{pj}
     * @f]
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The matrix multiplication of the inputs.
     *
     * @throw std::invalid_argument Thrown if the last dimension of a is not
     *     the same as the second-to-last dimension of b.
     */
    template <class T, class Tag1, class Tag2>
    T matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 2> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank - 1> matmul(
        const base_tensor<T, Rank, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank - 1> matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, Rank, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );
}

#include "numcpp/tensor/tensor.tcc"

#endif // C++11
#endif // NUMCPP_TENSOR_H_INCLUDED
