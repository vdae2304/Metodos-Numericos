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
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

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
         * @param data Pointer to the memory array used by the tensor_view.
         * @param shape Number of elements along each axis. It can be a shape_t
         *     object or the elements of the shape passed as separate arguments.
         * @param order Memory layout in which elements are stored. In
         *     row-major order, the last dimension is contiguous. In
         *     column-major order, the first dimension is contiguous. Defaults
         *     to row-major order.
         */
        template <class... Sizes,
                  detail::RequiresNArguments<Rank, Sizes...> = 0,
                  detail::RequiresIntegral<Sizes...> = 0>
        base_tensor(T *data, Sizes... sizes);

        base_tensor(
            T *data, const shape_t<Rank> &shape,
            layout_t order = row_major
        );

        /**
         * @brief Slice constructor. Constructs a tensor_view that references a
         * subset of elements from a multidimensional array.
         *
         * @param data Pointer to the memory array used by the tensor_view.
         * @param shape Number of elements along each axis.
         * @param offset Relative position of the first element selected by the
         *     tensor_view.
         * @param strides Span that separates the selected elements along each
         *     axis.
         * @param order Order in which elements shall be iterated. In row-major
         *     order, the last index is varying the fastest. In column-major
         *     order, the first index is varying the fastest. Defaults to
         *     row-major order.
         */
        base_tensor(
            T *data, const shape_t<Rank> &shape,
            ptrdiff_t offset, const shape_t<Rank> &strides,
            layout_t order = row_major
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
         * @param index... Position of an element along each axis.
         *
         * @return The element at the specified position. If the tensor_view is
         *     const-qualified, the function returns a reference to const T.
         *     Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if index is out of bounds.
         */
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        T& operator()(Index... index);

        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        const T& operator()(Index... index) const;

        /**
         * @brief Subscript operator. Returns a reference to the element at the
         * given position.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor_view. Since C++23, the elements of the index can be
         *     passed as separate arguments.
         *
         * @return The element at the specified position. If the tensor_view is
         *     const-qualified, the function returns a reference to const T.
         *     Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if index is out of bounds.
         */
        T& operator[](const index_t<Rank> &index);
        const T& operator[](const index_t<Rank> &index) const;

#ifdef __cpp_multidimensional_subscript
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        T& operator[](Index... index);

        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        const T& operator[](Index... index) const;
#endif // C++23

        /**
         * @brief Return the dimension of the tensor_view.
         */
        static constexpr size_t ndim();

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
        ptrdiff_t offset() const;

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
         * @brief Returns the order in which elements are iterated. It is not
         * necessarily the memory layout in which elements are stored as the
         * elements might not be contiguous in memory.
         */
        layout_t layout() const;

        /**
         * @brief Returns whether the elements in the tensor_view are stored
         * contiguously.
         */
        bool is_contiguous() const;

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
        base_tensor& operator=(const base_tensor &other);
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
         * @param k Offset of the diagonal from the main diagonal. A positive
         *     value refers to an upper diagonal and a negative value refers to
         *     a lower diagonal. Defaults to 0 (main diagonal).
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         */
        tensor_view<T, 1> diagonal(ptrdiff_t k = 0);
        tensor_view<const T, 1> diagonal(ptrdiff_t k = 0) const;

        /**
         * @brief Return a view of the tensor collapsed into one dimension.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         *
         * @throw std::runtime_error Thrown if the elements in the view are
         *     non-contiguous.
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
         * @param order Memory layout in which elements are read. In row-major
         *     order, the last dimension is contiguous. In column-major order,
         *     the first dimension is contiguous. The default is to use the
         *     same layout as *this.
         *
         * @return If the tensor is const-qualified, the function returns a
         *     tensor_view to const T, which is convertible to a tensor
         *     object. Otherwise, the function returns a tensor_view to T,
         *     which has reference semantics to the original tensor.
         *
         * @throw std::invalid_argument Thrown if the tensor could not reshaped.
         * @throw std::runtime_error Thrown if the elements in the view are
         *     non-contiguous.
         */
        template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
        tensor_view<T, sizeof...(Sizes)> reshape(Sizes... sizes);
        template <class... Sizes, detail::RequiresIntegral<Sizes...> = 0>
        tensor_view<const T, sizeof...(Sizes)> reshape(Sizes... sizes) const;

        template <size_t N>
        tensor_view<T, N> reshape(const shape_t<N> &shape);
        template <size_t N>
        tensor_view<const T, N> reshape(const shape_t<N> &shape) const;

        template <size_t N>
        tensor_view<T, N> reshape(const shape_t<N> &shape, layout_t order);
        template <size_t N>
        tensor_view<const T, N> reshape(
            const shape_t<N> &shape, layout_t order
        ) const;

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
        template <class... Axes, detail::RequiresIntegral<Axes...> = 0>
        tensor_view<T, Rank - sizeof...(Axes)> squeeze(Axes... axes);
        template <class... Axes, detail::RequiresIntegral<Axes...> = 0>
        tensor_view<const T,Rank - sizeof...(Axes)> squeeze(Axes... axes) const;

        template <size_t N>
        tensor_view<T, Rank - N> squeeze(const shape_t<N> &axes);
        template <size_t N>
        tensor_view<const T, Rank - N> squeeze(const shape_t<N> &axes) const;

        /**
         * @brief Interchanges two axes of a tensor_view in-place. The internal
         * layout order is preserved.
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

        // Number of elements along each axis.
        shape_t<Rank> m_shape;

        // Number of elements.
        size_t m_size;

        // Offset of array data in memory.
        ptrdiff_t m_offset;

        // Strides of data in memory.
        shape_t<Rank> m_stride;

        // Memory layout.
        layout_t m_order;
    };
}

#include "numcpp/tensor/tensor_view.tcc"

#endif // NUMCPP_TENSOR_VIEW_H_INCLUDED
