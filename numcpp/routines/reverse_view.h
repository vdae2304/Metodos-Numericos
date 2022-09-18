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

 /** @file include/numcpp/routines/reverse_view.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_REVERSE_VIEW_H_INCLUDED
#define NUMCPP_REVERSE_VIEW_H_INCLUDED

namespace numcpp {
    /// Forward declarations.
    template <class Tag, size_t N>
    struct reverse_tag;

    template <class Tag, size_t N>
    struct shift_tag;

    /**
     * @brief A light-weight object which stores the elements of a tensor in
     * reversed order over multiple axes. Convertible to a tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam Tag Type of the base_tensor container.
     * @tparam N Number of axes to reverse.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor<T, Rank, reverse_tag<Tag, N> > {
    public:
        /// Member types.
        typedef typename base_tensor<T, Rank, Tag>::value_type
            value_type;
        typedef typename base_tensor<T, Rank, Tag>::const_reference
            reference;
        typedef typename base_tensor<T, Rank, Tag>::const_reference
            const_reference;
        typedef typename base_tensor<T, Rank, Tag>::const_pointer
            pointer;
        typedef typename base_tensor<T, Rank, Tag>::const_pointer
            const_pointer;
        typedef base_tensor_const_iterator<T, Rank, reverse_tag<Tag, N> >
            iterator;
        typedef base_tensor_const_iterator<T, Rank, reverse_tag<Tag, N> >
            const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

    private:
        // Tensor object to reverse.
        const base_tensor<T, Rank, Tag> &m_arg;

        // Axes along which to reverse over.
        shape_t<N> m_axes;

    public:
        /// Constructors.

        /**
         * @brief Constructs a view which stores the elements of a tensor in
         * reversed order.
         *
         * @param arg Tensor to reverse.
         * @param axes Axes along which to reverse over.
         */
        base_tensor(
            const base_tensor<T, Rank, Tag> &arg, const shape_t<N> &axes
        ) : m_arg(arg), m_axes(axes) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin() const {
            return const_iterator(this, 0, this->layout());
        }

        const_iterator begin(layout_t order) const {
            return const_iterator(this, 0, order);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor.
         */
        const_iterator end() const {
            return const_iterator(this, this->size(), this->layout());
        }

        const_iterator end(layout_t order) const {
            return const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param index... Position of an element along each axis.
         *
         * @return The element at the specified position.
         */
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        const_reference operator()(Index... index) const {
            return this->operator[](make_index(index...));
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor. If the tensor is one dimensional, an integer can be
         *     used instead.
         *
         * @return The element at the specified position.
         */
        const_reference operator[](index_type index) const {
            for (size_t i = 0; i < m_axes.ndim(); ++i) {
                size_t axis = m_axes[i];
                index[axis] = m_arg.shape(axis) - 1 - index[axis];
            }
            return m_arg[index];
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
            return Rank;
        }

        /**
         * @brief Return the shape of the tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        auto shape() const -> decltype(m_arg.shape()) {
            return m_arg.shape();
        }

        size_type shape(size_type axis) const {
            return m_arg.shape(axis);
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         */
        size_type size() const {
            return m_arg.size();
        }

        /**
         * @brief Returns the memory layout in which elements are stored.
         */
        layout_t layout() const {
            return m_arg.layout();
        }

        /// Public methods.

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(*this);
        }
    };

    /**
     * @brief A light-weight object which stores the elements of a tensor
     * circularly shifted over multiple axes. Convertible to a tensor object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam Tag Type of the base_tensor container.
     * @tparam N Number of axes to shift.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor<T, Rank, shift_tag<Tag, N> > {
    public:
        /// Member types.
        typedef typename base_tensor<T, Rank, Tag>::value_type
            value_type;
        typedef typename base_tensor<T, Rank, Tag>::const_reference
            reference;
        typedef typename base_tensor<T, Rank, Tag>::const_reference
            const_reference;
        typedef typename base_tensor<T, Rank, Tag>::const_pointer
            pointer;
        typedef typename base_tensor<T, Rank, Tag>::const_pointer
            const_pointer;
        typedef base_tensor_const_iterator<T, Rank, shift_tag<Tag, N> >
            iterator;
        typedef base_tensor_const_iterator<T, Rank, shift_tag<Tag, N> >
            const_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef shape_t<Rank> shape_type;
        typedef index_t<Rank> index_type;

    private:
        // Tensor object to shift.
        const base_tensor<T, Rank, Tag> &m_arg;

        // Number of positions to shift along each axis.
        index_t<N> m_count;

        // Axes along which to shift over.
        shape_t<N> m_axes;

    public:
        /// Constructors.

        /**
         * @brief Constructs a view which stores the elements of a tensor
         * circularly shifted.
         *
         * @param arg Tensor to shift.
         * @param count Number of positions to shift along each axis.
         * @param axes Axes along which to shift over.
         */
        base_tensor(
            const base_tensor<T, Rank, Tag> &arg,
            const index_t<N> &count,
            const shape_t<N> &axes
        ) : m_arg(arg), m_count(count), m_axes(axes) {}

        /// Destructor.
        ~base_tensor() = default;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin() const {
            return const_iterator(this, 0, this->layout());
        }

        const_iterator begin(layout_t order) const {
            return const_iterator(this, 0, order);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param order It is an optional parameter that changes the order
         *     in which elements are iterated. In row-major order, the last
         *     index is varying the fastest. In column-major order, the first
         *     index is varying the fastest. The default is to use the same
         *     layout as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor.
         */
        const_iterator end() const {
            return const_iterator(this, this->size(), this->layout());
        }

        const_iterator end(layout_t order) const {
            return const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param index... Position of an element along each axis.
         *
         * @return The element at the specified position.
         */
        template <class... Index,
                  detail::RequiresNArguments<Rank, Index...> = 0,
                  detail::RequiresIntegral<Index...> = 0>
        const_reference operator()(Index... index) const {
            return this->operator[](make_index(index...));
        }

        /**
         * @brief Subscript operator. Returns the element at the given
         * position.
         *
         * @param index An index_t object with the position of an element in
         *     the tensor. If the tensor is one dimensional, an integer can be
         *     used instead.
         *
         * @return The element at the specified position.
         */
        const_reference operator[](index_type index) const {
            for (size_t i = 0; i < m_axes.ndim(); ++i) {
                size_t axis = m_axes[i];
                index[axis] = (index[axis] + m_count[i]) % m_arg.shape(axis);
            }
            return m_arg[index];
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        static constexpr size_type ndim() {
            return Rank;
        }

        /**
         * @brief Return the shape of the tensor.
         *
         * @param axis It is an optional parameter that changes the return
         *     value. If provided, returns the size along the given axis.
         *     Otherwise, return a shape_t object with the shape of the tensor
         *     along all axes.
         */
        auto shape() const -> decltype(m_arg.shape()) {
            return m_arg.shape();
        }

        size_type shape(size_type axis) const {
            return m_arg.shape(axis);
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         */
        size_type size() const {
            return m_arg.size();
        }

        /**
         * @brief Returns the memory layout in which elements are stored.
         */
        layout_t layout() const {
            return m_arg.layout();
        }

        /// Public methods.

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(*this);
        }
    };
}

#endif // NUMCPP_REVERSE_VIEW_H_INCLUDED
