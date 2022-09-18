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

 /** @file include/numcpp/iterators/axes_iterator.h
  *  This header defines an iterator for reduction operations on tensor class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED
#define NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED

#include <iterator>

namespace numcpp {
    /**
     * @brief Constructs a base_tensor_axes_iterator with its templates
     * deduced from its arguments.
     *
     * @param ptr Pointer to the tensor to iterate over.
     * @param indices An index_t object with the indices to fix.
     * @param axes A shape_t object with the axes to iterate over.
     * @param flat Flat index over the iterated axes. Defaults to 0.
     *
     * @return A random access iterator pointing to an element in the tensor.
     *     The returned iterator iterates only over the given axes. The
     *     remaining axes are fixed at the positions given by indices.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_axes_iterator<T, Rank, Tag, N>
    make_axes_iterator(
        base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
        const shape_t<N> &axes, size_t flat = 0
    );

    template <class T, size_t Rank, class Tag>
    base_tensor_axes_iterator<T, Rank, Tag, 1>
    make_axes_iterator(
        base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
        size_t axis, size_t flat = 0
    );

    /**
     * @brief Constructs a base_tensor_const_axes_iterator with its templates
     * deduced from its arguments.
     *
     * @param ptr Pointer to the tensor to iterate over.
     * @param indices An index_t object with the indices to fix.
     * @param axes A shape_t object with the axes to iterate over.
     * @param flat Flat index over the iterated axes. Defaults to 0.
     *
     * @return A const random access iterator pointing to an element in the
     *     tensor. The returned iterator iterates only over the given axes. The
     *     remaining axes are fixed at the positions given by indices.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_axes_iterator<T, Rank, Tag, N>
    make_const_axes_iterator(
        const base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
        const shape_t<N> &axes, size_t flat = 0
    );

    template <class T, size_t Rank, class Tag>
    base_tensor_const_axes_iterator<T, Rank, Tag, 1>
    make_const_axes_iterator(
        const base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
        size_t axis, size_t flat = 0
    );

    /**
     * @brief A random access iterator for base_tensor class obtained by fixing
     * some axes and iterating over the remaining axes.
     *
     * @tparam T Type of the elements contained in the base_tensor.
     * @tparam Rank Dimension of the base_tensor. It must be a positive
     *     integer.
     * @tparam Tag Type indicating which specialization of base_tensor refers
     *     to.
     * @tparam N Reduction dimension. Must be less or equal than Rank.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor_axes_iterator {
    public:
        static_assert(N <= Rank, "Cannot fix more axes than the tensor"
                      " dimension");

        /// Member types.
        typedef ptrdiff_t difference_type;
        typedef typename base_tensor<T, Rank, Tag>::value_type value_type;
        typedef typename base_tensor<T, Rank, Tag>::pointer pointer;
        typedef typename base_tensor<T, Rank, Tag>::reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        /// Constructors.

        /**
         * @brief Default constructor.
         */
        base_tensor_axes_iterator();

        /**
         * @brief Flat index (over iterated axes) constructor.
         */
        base_tensor_axes_iterator(
            base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
            const shape_t<N> &axes, size_t flat = 0
        );

        /**
         * @brief Copy constructor.
         */
        base_tensor_axes_iterator(const base_tensor_axes_iterator &other);

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        base_tensor_axes_iterator&
        operator=(const base_tensor_axes_iterator &other);

        /// Operator overloading.

        base_tensor_axes_iterator& operator++();
        base_tensor_axes_iterator& operator--();
        base_tensor_axes_iterator operator++(int);
        base_tensor_axes_iterator operator--(int);
        base_tensor_axes_iterator& operator+=(difference_type rhs);
        base_tensor_axes_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /**
         * @brief Accesses the underlying tensor.
         */
        base_tensor<T, Rank, Tag>* base() const;

        /**
         * @brief Returns the flat index over the iterated axes.
         */
        size_t index() const;

        /**
         * @brief Returns an index_t object with the current coordinates.
         */
        index_t<Rank> coords() const;

        /**
         * @brief Returns a shape_t object with the axes to iterate over.
         */
        const shape_t<N>& axes() const;

    private:
        // Pointer to the base_tensor associated to the iterator.
        base_tensor<T, Rank, Tag> *m_ptr;

        // Indices to fix.
        index_t<Rank> m_indices;

        // Axes to iterate over.
        shape_t<N> m_axes;

        // Flat index over the iterated axes.
        size_t m_flat;
    };

    /// Arithmetic operators for base_tensor_axes_iterator (non member
    /// functions).

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_axes_iterator<T, Rank, Tag, N> operator+(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_axes_iterator<T, Rank, Tag, N> operator+(
        ptrdiff_t lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_axes_iterator<T, Rank, Tag, N> operator-(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    ptrdiff_t operator-(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    /// Relational operators for base_tensor_axes_iterator (non member
    /// functions).

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator==(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator!=(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator<(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator>(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator<=(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator>=(
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &lhs,
        const  base_tensor_axes_iterator<T, Rank, Tag, N> &rhs
    );

    /**
     * @brief A random access iterator for const-qualified base_tensor class
     * obtained by fixing some axes and iterating over the remaining axes.
     *
     * @tparam T Type of the elements contained in the base_tensor.
     * @tparam Rank Dimension of the base_tensor. It must be a positive
     *     integer.
     * @tparam Tag Type indicating which specialization of base_tensor refers
     *     to.
     * @tparam N Reduction dimension. Must be less or equal than Rank.
     */
    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor_const_axes_iterator {
    public:
        static_assert(N <= Rank, "Cannot fix more axes than the tensor"
                      " dimension");

        /// Member types.
        typedef ptrdiff_t difference_type;
        typedef typename base_tensor<T, Rank, Tag>::value_type value_type;
        typedef typename base_tensor<T, Rank, Tag>::const_pointer pointer;
        typedef typename base_tensor<T, Rank, Tag>::const_reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        /// Constructors.

        /**
         * @brief Default constructor.
         */
        base_tensor_const_axes_iterator();

        /**
         * @brief Flat index (over iterated axes) constructor.
         */
        base_tensor_const_axes_iterator(
            const base_tensor<T, Rank, Tag> *ptr, const index_t<Rank> &indices,
            const shape_t<N> &axes, size_t flat = 0
        );

        /**
         * @brief Copy constructor.
         */
        base_tensor_const_axes_iterator(
            const base_tensor_axes_iterator<T, Rank, Tag, N> &other
        );
        base_tensor_const_axes_iterator(
            const base_tensor_const_axes_iterator &other
        );

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        base_tensor_const_axes_iterator&
        operator=(const base_tensor_axes_iterator<T, Rank, Tag, N> &other);
        base_tensor_const_axes_iterator&
        operator=(const base_tensor_const_axes_iterator &other);

        /// Operator overloading.

        base_tensor_const_axes_iterator& operator++();
        base_tensor_const_axes_iterator& operator--();
        base_tensor_const_axes_iterator operator++(int);
        base_tensor_const_axes_iterator operator--(int);
        base_tensor_const_axes_iterator& operator+=(difference_type rhs);
        base_tensor_const_axes_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /**
         * @brief Accesses the underlying tensor.
         */
        const base_tensor<T, Rank, Tag>* base() const;

        /**
         * @brief Returns the flat index over the iterated axes.
         */
        size_t index() const;

        /**
         * @brief Returns an index_t object with the current coordinates.
         */
        index_t<Rank> coords() const;

        /**
         * @brief Returns a shape_t object with the axes to iterate over.
         */
        const shape_t<N>& axes() const;

    private:
        // Pointer to the base_tensor associated to the iterator.
        const base_tensor<T, Rank, Tag> *m_ptr;

        // Indices to fix.
        index_t<Rank> m_indices;

        // Axes to iterate over.
        shape_t<N> m_axes;

        // Flat index over the iterated axes.
        size_t m_flat;
    };

    /// Arithmetic operators for base_tensor_const_axes_iterator (non member
    /// functions).

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_axes_iterator<T, Rank, Tag, N> operator+(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_axes_iterator<T, Rank, Tag, N> operator+(
        ptrdiff_t lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    base_tensor_const_axes_iterator<T, Rank, Tag, N> operator-(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    ptrdiff_t operator-(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    /// Relational operators for base_tensor_const_axes_iterator (non member
    /// functions).

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator==(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator!=(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator<(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator>(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator<=(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );

    template <class T, size_t Rank, class Tag, size_t N>
    bool operator>=(
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &lhs,
        const base_tensor_const_axes_iterator<T, Rank, Tag, N> &rhs
    );
}

#include "numcpp/iterators/axes_iterator.tcc"

#endif // NUMCPP_TENSOR_AXES_ITERATOR_H_INCLUDED