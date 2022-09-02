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

 /** @file include/numcpp/broadcasting/lazy_reverse.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LAZY_REVERSE_H_INCLUDED
#define NUMCPP_LAZY_REVERSE_H_INCLUDED

#include <type_traits>

namespace numcpp {
    template <class Tag, size_t N>
    struct lazy_reverse_tag;

/// Namespace for implementation details.
namespace detail {
    /// Checks whether a tensor subclass is an expression object.
    template <class T, size_t Rank, class Tag, size_t N>
    struct is_expression<base_tensor<T, Rank, lazy_reverse_tag<Tag, N> > >
     : std::true_type {};
}

    /**
     * @brief A lazy_reverse is a light-weight object which stores the elements
     * of a tensor in reversed order. A lazy_reverse is convertible to a tensor
     * object.
     *
     * @tparam T Type of the elements contained in the tensor.
     * @tparam Rank Dimension of the tensor. It must be a positive integer.
     * @tparam Tag Type of the base_tensor container.
     * @tparam N Number of axes to reverse.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor<T, Rank, lazy_reverse_tag<Tag, N> > {
    public:
        /// Member types.
        typedef typename std::remove_cv<T>::type value_type;
        typedef T reference;
        typedef T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_reverse_tag<Tag, N>
        > iterator;
        typedef base_tensor_const_iterator<
            T, Rank, lazy_reverse_tag<Tag, N>
        > const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Constructs a lazy_reverse which stores the elements of a
         * tensor in reversed order.
         *
         * @param arg Tensor to reverse.
         * @param axes Axes along which to reverse over.
         */
        base_tensor(
            const base_tensor<T, Rank, Tag> &arg, const shape_t<N> &axes
        ) : m_arg(arg), m_axes(axes) {}

        /// Destructor.
        ~base_tensor() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor.
         */
        const_iterator begin() const {
            return this->begin(this->rowmajor());
        }

        const_iterator begin(bool row_major) const {
            return make_tensor_const_iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor.
         */
        const_iterator end() const {
            return this->end(this->rowmajor());
        }

        const_iterator end(bool row_major) const {
            return make_tensor_const_iterator(this, this->size(), row_major);
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
        T operator[](const index_t<Rank> &index) const {
            index_t<Rank> a_index = index;
            for (size_t i = 0; i < N; ++i) {
                a_index[m_axes[i]] =
                    m_arg.shape(m_axes[i]) - 1 - index[m_axes[i]];
            }
            return m_arg[a_index];
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
         * @brief Returns whether the tensor is empty (i.e., whether its size
         * is 0).
         */
        bool empty() const {
            return m_arg.empty();
        }

        /**
         * @brief Returns whether the elements are stored in row-major order.
         */
        bool rowmajor() const {
            return m_arg.rowmajor();
        }

        /**
         * @brief Returns whether the elements are stored in column-major
         * order.
         */
        bool colmajor() const {
            return m_arg.colmajor();
        }

        /**
         * @brief Return a copy of the tensor.
         */
        tensor<value_type, Rank> copy() const {
            return tensor<value_type, Rank>(this->shape(), this->begin());
        }

    private:
        // Tensor object to reverse.
        detail::ConstRefIfNotExpression<base_tensor<T, Rank, Tag> > m_arg;

        // Axes along which to reverse over.
        shape_t<N> m_axes;
    };
}

#endif // NUMCPP_LAZY_REVERSE_H_INCLUDED
