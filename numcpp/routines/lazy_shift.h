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

 /** @file include/numcpp/routines/lazy_shift.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_SHIFT_H_INCLUDED
#define NUMCPP_LAZY_SHIFT_H_INCLUDED

namespace numcpp {
    template <class Tag, size_t N>
    struct lazy_shift_tag;

    /**
     * @brief A lazy_shift is a light-weight object which stores the elements
     * of a tensor circularly shifted. A lazy_shift is convertible to a tensor
     * object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam Tag Type of the base_tensor container.
     * @tparam N Number of axes to shift.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor<T, Rank, lazy_shift_tag<Tag, N> > {
    public:
        /// Member types.
        typedef typename base_tensor<T, Rank, Tag>::value_type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_shift_tag<Tag, N>
        > iterator;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_shift_tag<Tag, N>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_shift which stores the elements of a tensor
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
            return this->begin(this->layout());
        }

        const_iterator begin(layout_t order) const {
            return make_tensor_const_iterator(this, 0, order);
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
            return this->end(this->layout());
        }

        const_iterator end(layout_t order) const {
            return make_tensor_const_iterator(this, this->size(), order);
        }

        /// Indexing.

        /**
         * @brief Call operator. Returns the element at the given position.
         *
         * @param args... Index arguments.
         *
         * @return The element at the specified position.
         */

        template <class... Args,
                  detail::RequiresNArguments<Rank, Args...> = true,
                  detail::RequiresIntegral<Args...> = true>
        T operator()(Args... args) const {
            return this->operator[](make_index(args...));
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
        T operator[](index_t<Rank> index) const {
            for (size_t i = 0; i < m_axes.ndim(); ++i) {
                index[m_axes[i]] += m_count[i];
                index[m_axes[i]] %= m_arg.shape(m_axes[i]);
            }
            return m_arg[index];
        }

        /**
         * @brief Return the dimension of the tensor.
         */
        constexpr size_t ndim() const {
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
        auto shape() const
         -> decltype(std::declval<base_tensor<T, Rank, Tag> >().shape()) {
            return m_arg.shape();
        }

        size_t shape(size_t axis) const {
            return m_arg.shape(axis);
        }

        /**
         * @brief Returns the number of elements in the tensor (i.e., the
         * product of the sizes along all the axes).
         */
        size_t size() const {
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
            return tensor<value_type, Rank>(this->begin(), this->shape());
        }

    private:
        // Tensor object to shift.
        const base_tensor<T, Rank, Tag> &m_arg;

        // Number of positions to shift along each axis.
        index_t<N> m_count;

        // Axes along which to shift over.
        shape_t<N> m_axes;
    };
}

#endif // NUMCPP_LAZY_SHIFT_H_INCLUDED
