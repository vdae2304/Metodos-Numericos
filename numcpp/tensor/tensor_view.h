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

 /** @file include/numcpp/tensor/tensor_view.h
  *  This header defines the tensor_view class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_VIEW_H_INCLUDED
#define NUMCPP_TENSOR_VIEW_H_INCLUDED

namespace numcpp {
    /**
     * @brief A tensor_view is just a view of a multidimensional array. It
     * references the elements in the original array. The view itself does not
     * own the data and any changes made to the view will affect the original
     * array, and any changes made to the original array will affect the view.
     *
     * @tparam T Type of the elements contained in the tensor_view. This shall
     *     be an arithmetic type or a class that behaves like one (such as
     *     std::complex).
     * @tparam Rank Dimension of the tensor_view. It must be a positive integer.
     */
    template <class T, size_t Rank>
    class base_tensor<T, Rank, view_tag>
     : public tensor_interface<T, Rank, view_tag>,
       public complex_interface<T, Rank, view_tag> {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Default constructor. Constructs a tensor_view that does not
         * reference any object.
         */
        base_tensor();

        /**
         * @brief Tensor constructor. Constructs a tensor_view that references
         * a multidimensional array.
         *
         * @param shape Number of elements along each axis.
         * @param data Pointer to the memory array used by the tensor_view.
         * @param order If true (default), the elements are stored in row-major
         *     order (from first axis to last axis). Otherwise, the elements
         *     are stored in column-major order (from last axis to first axis).
         */
        base_tensor(const shape_t<Rank> &shape, T *data, bool order = true);

        /**
         * @brief Slice constructor. Constructs a tensor_view that references a
         * subset of elements from a multidimensional array.
         *
         * @param shape Number of elements along each axis.
         * @param data Pointer to the memory array used by the tensor_view.
         * @param offset Index of the first element selected by the tensor_view.
         * @param strides Span that separates the selected elements along each
         *     axis.
         * @param order If true (default), the elements are stored in row-major
         *     order (from first axis to last axis). Otherwise, the elements
         *     are stored in column-major order (from last axis to first axis).
         */
        base_tensor(
            const shape_t<Rank> &shape, T *data,
            size_t offset, const shape_t<Rank> &strides, bool order = true
        );

        /**
         * @brief Copy constructor. Constructs a tensor_view as a copy of other.
         *
         * @param other A tensor_view of the same type and rank.
         */
        base_tensor(const base_tensor &other);

        /**
         * @brief Move constructor. Constructs a tensor_view that acquires the
         * elements of other.
         *
         * @param other A tensor_view of the same type and rank. other is left
         *     in an empty state.
         */
        base_tensor(base_tensor &&other);

        /// Destructor.
        ~base_tensor();

        /// Indexing.

        /**
         * @brief Call operator. Returns a reference to the element at the
         * given position. The elements in a tensor_view are given by
         *     data[offset + index[0]*stride[0] + ... + index[N-1]*stride[N-1]]
         * where data is the memory array.
         *
         * @param args... Index arguments.
         *
         * @return The element at the specified position. If the tensor_view is
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
         *     the tensor_view. If the tensor is one dimensional, an integer
         *     can be used instead.
         *
         * @return The element at the specified position. If the tensor_view is
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
         * @brief Return the dimension of the tensor_view.
         */
        constexpr size_t ndim() const;

        /**
         * @brief Return the shape of the tensor_view.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        const shape_t<Rank>& shape() const;
        size_t shape(size_t axis) const;

        /**
         * @brief Returns the number of elements in the tensor_view (i.e.,
         * the product of the sizes along all the axes).
         *
         * @note Time complexity: O(1)
         */
        size_t size() const;

        /**
         * @brief Returns whether the tensor_view is empty (i.e., whether its
         * size is 0).
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the
         * tensor_view.
         *
         * @return A pointer to the memory array used internally by the
         *     tensor_view. If the tensor_view is const-qualified, the function
         *     returns a pointer to const T. Otherwise, it returns a pointer to
         *     T.
         */
        T* data();
        const T* data() const;

        /**
         * @brief Returns the position in the memory array of the first
         * element.
         */
        size_t offset() const;

        /**
         * @brief Returns the span that separates the elements in the memory
         * array.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the stride along the given axis.
         *     Otherwise, return a shape_t object with the strides of the
         *     tensor along all axes.
         */
        const shape_t<Rank>& strides() const;
        size_t strides(size_t axis) const;

        /**
         * @brief Returns whether the elements are stored in row-major order.
         */
        bool rowmajor() const;

        /**
         * @brief Returns whether the elements are stored in column-major
         * order.
         */
        bool colmajor() const;

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns to each element the value of the
         * corresponding element in other.
         *
         * @param other A tensor-like object of the same shape.
         *
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the shapes are different.
         */
        template <class U, class Tag>
        base_tensor& operator=(const base_tensor<U, Rank, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element.
         *
         * @param val Value assigned to all the elements in the tensor_view.
         *
         * @return *this
         */
        base_tensor& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving
         * other in an empty state.
         *
         * @param other A tensor_view of the same type and rank. other is left
         *     in an empty state.
         *
         * @return *this
         */
        base_tensor& operator=(base_tensor &&other);

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
         * @brief Removes axes of length one.
         *
         * @param axes Selects a subset of the entries of length one in the
         *     shape. It can be a shape_t object or the elements of the shape
         *     passed as separate arguments.
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

        template <class... Args, detail::RequiresIntegral<Args...> = true>
        tensor_view<T, Rank - sizeof...(Args)> squeeze(Args... args);

        template <size_t N>
        tensor_view<const T, Rank - N> squeeze(const shape_t<N> &axes) const;

        template <class... Args, detail::RequiresIntegral<Args...> = true>
        tensor_view<const T,Rank - sizeof...(Args)> squeeze(Args... args) const;

        /**
         * @brief Interchanges two axes of a tensor_view in-place.
         *
         * @param axis1 First axis.
         * @param axis2 Second axis.
         */
        void swapaxes(size_t axis1, size_t axis2);

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

    private:
        // Pointer to data.
        T *m_data;

        // Number of elements.
        size_t m_size;

        // Number of elements along each axis.
        shape_t<Rank> m_shape;

        // Offset of array data in memory.
        size_t m_offset;

        // Strides of data in memory.
        shape_t<Rank> m_stride;

        /// Row-major (true) or column-major (false) order.
        bool m_order;
    };
}

#include "numcpp/tensor/tensor_view.tcc"

#endif // NUMCPP_TENSOR_VIEW_H_INCLUDED
