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

 /** @file include/numcpp/iterators/tensor_iterator.h
  *  This header defines a generic iterator for tensor class.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_ITERATOR_H_INCLUDED
#define NUMCPP_TENSOR_ITERATOR_H_INCLUDED

#include <iterator>

namespace numcpp {
    /**
     * @brief Constructs a base_tensor_iterator with its templates deduced from
     * its arguments.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag> make_tensor_iterator(
        base_tensor<T, Rank, Tag> *ptr, size_t index = 0,
        layout_t order = row_major
    );

    /**
     * @brief Constructs a base_tensor_const_iterator with its templates
     * deduced from its arguments.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag> make_tensor_const_iterator(
        const base_tensor<T, Rank, Tag> *ptr, size_t index = 0,
        layout_t order = row_major
    );

    /**
     * @brief A generic random access iterator for base_tensor class.
     *
     * @tparam T Type of the elements contained in the base_tensor.
     * @tparam Rank Dimension of the base_tensor. It must be a positive
     *     integer.
     * @tparam Tag Type indicating which specialization of base_tensor refers
     *     to.
     */
    template <class T, size_t Rank, class Tag>
    class base_tensor_iterator {
    public:
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
        base_tensor_iterator();

        /**
         * @brief Flat index constructor.
         */
        base_tensor_iterator(
            base_tensor<T, Rank, Tag> *ptr, size_t index = 0,
            layout_t order = row_major
        );

        /**
         * @brief Copy constructor.
         */
        base_tensor_iterator(const base_tensor_iterator &other);

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        base_tensor_iterator& operator=(const base_tensor_iterator &other);

        /// Operator overloading.

        base_tensor_iterator& operator++();
        base_tensor_iterator& operator--();
        base_tensor_iterator operator++(int);
        base_tensor_iterator operator--(int);
        base_tensor_iterator& operator+=(difference_type rhs);
        base_tensor_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /**
         * @brief Accesses the underlying tensor.
         */
        base_tensor<T, Rank, Tag>* base() const;

        /**
         * @brief Returns the underlying flat index.
         */
        size_t index() const;

        /**
         * @brief Returns an index_t object with the current coordinates.
         */
        index_t<Rank> coords() const;

        /**
         * @brief Returns the order in which elements are iterated.
         */
        layout_t layout() const;

    private:
        // Pointer to the base_tensor associated to the iterator.
        base_tensor<T, Rank, Tag> *m_ptr;

        // Index associated to the iterator.
        size_t m_index;

        // Layout iteration.
        layout_t m_order;
    };

    /// Arithmetic operators for base_tensor_iterator (non member functions).

    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag> operator+(
        const base_tensor_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag> operator+(
        ptrdiff_t lhs, const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    base_tensor_iterator<T, Rank, Tag> operator-(
        const base_tensor_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag>
    ptrdiff_t operator-(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    /// Relational operators for base_tensor_iterator (non member functions).

    template <class T, size_t Rank, class Tag>
    bool operator==(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator!=(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator<(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator>(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator<=(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator>=(
        const base_tensor_iterator<T, Rank, Tag> &lhs,
        const base_tensor_iterator<T, Rank, Tag> &rhs
    );

    /**
     * @brief A generic random access iterator for const-qualified base_tensor
     * class.
     *
     * @tparam T Type of the elements contained in the base_tensor.
     * @tparam Rank Dimension of the base_tensor. It must be a positive
     *     integer.
     * @tparam Tag Type indicating which specialization of base_tensor refers
     *     to.
     */
    template <class T, size_t Rank, class Tag>
    class base_tensor_const_iterator {
    public:
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
        base_tensor_const_iterator();

        /**
         * @brief Flat index constructor.
         */
        base_tensor_const_iterator(
            const base_tensor<T, Rank, Tag> *ptr, size_t index = 0,
            layout_t order = row_major
        );

        /**
         * @brief Copy constructor.
         */
        base_tensor_const_iterator(
            const base_tensor_iterator<T, Rank, Tag> &other
        );
        base_tensor_const_iterator(
            const base_tensor_const_iterator &other
        );

        /// Assignment operator.

        /**
         * @brief Copy assignment.
         */
        base_tensor_const_iterator&
        operator=(const base_tensor_iterator<T, Rank, Tag> &other);
        base_tensor_const_iterator&
        operator=(const base_tensor_const_iterator &other);

        /// Operator overloading.

        base_tensor_const_iterator& operator++();
        base_tensor_const_iterator& operator--();
        base_tensor_const_iterator operator++(int);
        base_tensor_const_iterator operator--(int);
        base_tensor_const_iterator& operator+=(difference_type rhs);
        base_tensor_const_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /**
         * @brief Accesses the underlying tensor.
         */
        const base_tensor<T, Rank, Tag>* base() const;

        /**
         * @brief Returns the underlying flat index.
         */
        size_t index() const;

        /**
         * @brief Returns an index_t object with the current coordinates.
         */
        index_t<Rank> coords() const;

        /**
         * @brief Returns the order in which elements are iterated.
         */
        layout_t layout() const;

    private:
        // Pointer to the base_tensor associated to the iterator.
        const base_tensor<T, Rank, Tag> *m_ptr;

        // Index associated to the iterator.
        size_t m_index;

        // Layout iteration.
        layout_t m_order;
    };

    /// Arithmetic operators for base_tensor_const_iterator (non member
    /// functions).

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag> operator+(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag> operator+(
        ptrdiff_t lhs, const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    base_tensor_const_iterator<T, Rank, Tag> operator-(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, size_t Rank, class Tag>
    ptrdiff_t operator-(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    /// Relational operators for base_tensor_const_iterator (non member
    /// functions).

    template <class T, size_t Rank, class Tag>
    bool operator==(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator!=(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator<(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator>(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator<=(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );

    template <class T, size_t Rank, class Tag>
    bool operator>=(
        const base_tensor_const_iterator<T, Rank, Tag> &lhs,
        const base_tensor_const_iterator<T, Rank, Tag> &rhs
    );
}

#include "numcpp/iterators/tensor_iterator.tcc"

#endif // NUMCPP_TENSOR_ITERATOR_H_INCLUDED
