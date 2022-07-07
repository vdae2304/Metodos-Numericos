/*
 * This file is part of the NumCpp project.
 *
 * NumCpp is a package for scientific computing in C++. It is a C++ library
 * that provides an array and a matrix object, and an assortment of routines
 * for fast operations on arrays and matrices, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * The NumCpp package is inspired by the NumPy package for Python, although it
 * is not related to it or any of its parts.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/array/array_iterator.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/array.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_ITERATOR_H_INCLUDED
#define NUMCPP_ARRAY_ITERATOR_H_INCLUDED

namespace numcpp {
    /// Forward declarations.
    template <class T, class Tag>
    class base_array_iterator;

    template <class T, class Tag>
    class base_array_const_iterator;

    /**
     * @brief A random access iterator for base_array class.
     *
     * @tparam T Type of the elements contained in the base_array.
     * @tparam Tag Type indicating which specialization of base_array refers to.
     */
    template <class T, class Tag>
    class base_array_iterator {
    public:
        /// Member types.

        typedef ptrdiff_t difference_type;
        typedef typename base_array<T, Tag>::value_type value_type;
        typedef typename base_array<T, Tag>::pointer pointer;
        typedef typename base_array<T, Tag>::reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        /// Constructors.

        /// Default constructor.
        base_array_iterator();

        /// Index constructor.
        base_array_iterator(base_array<T, Tag> *arr, size_t i);

        /// Copy constructor.
        base_array_iterator(const base_array_iterator &other);

        /// Assignment operator.

        base_array_iterator& operator=(const base_array_iterator &other);

        /// Operator overloading.

        base_array_iterator& operator++();
        base_array_iterator& operator--();
        base_array_iterator operator++(int);
        base_array_iterator operator--(int);
        base_array_iterator& operator+=(difference_type rhs);
        base_array_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /// Accesses the underlying array.
        base_array<T, Tag>* base() const;

        /// Returns the underlying index.
        size_t index() const;

    protected:
        /// Pointer to the array associated to the iterator.
        base_array<T, Tag> *m_ptr;

        /// Index associated to the iterator.
        size_t m_index;
    };

    /// Arithmetic operators for base_array_iterator (non member functions).

    template <class T, class Tag>
    base_array_iterator<T, Tag> operator+(
        const base_array_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    base_array_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_array_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    base_array_iterator<T, Tag> operator-(
        const base_array_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    ptrdiff_t operator-(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    /// Relational operators for base_array_iterator (non member functions).

    template <class T, class Tag>
    bool operator==(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator!=(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<=(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>=(
        const base_array_iterator<T, Tag> &lhs,
        const base_array_iterator<T, Tag> &rhs
    );

    /**
     * @brief A random access iterator for const-qualified base_array class.
     *
     * @tparam T Type of the elements contained in the base_array.
     * @tparam Tag Type indicating which specialization of base_array refers to.
     */
    template <class T, class Tag>
    class base_array_const_iterator {
    public:
        /// Member types.
    
        typedef ptrdiff_t difference_type;
        typedef typename base_array<T, Tag>::value_type value_type;
        typedef typename base_array<T, Tag>::const_pointer pointer;
        typedef typename base_array<T, Tag>::const_reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        /// Constructors.

        /// Default constructor.
        base_array_const_iterator();

        /// Index constructor.
        base_array_const_iterator(const base_array<T, Tag> *arr, size_t i);

        /// Copy constructor.
        base_array_const_iterator(const base_array_iterator<T, Tag> &other);
        base_array_const_iterator(const base_array_const_iterator &other);

        /// Assignment operator.

        base_array_const_iterator&
        operator=(const base_array_iterator<T, Tag> &other);
        base_array_const_iterator&
        operator=(const base_array_const_iterator &other);

        /// Operator overloading.

        base_array_const_iterator& operator++();
        base_array_const_iterator& operator--();
        base_array_const_iterator operator++(int);
        base_array_const_iterator operator--(int);
        base_array_const_iterator& operator+=(difference_type rhs);
        base_array_const_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /// Accesses the underlying array.
        const base_array<T, Tag>* base() const;

        /// Returns the underlying index.
        size_t index() const;

    protected:
        /// Pointer to the base_array associated to the iterator.
        const base_array<T, Tag> *m_ptr;

        /// Index associated to the iterator.
        size_t m_index;
    };

    /// Arithmetic operators for base_array_const_iterator (non member
    /// functions).

    template <class T, class Tag>
    base_array_const_iterator<T, Tag> operator+(
        const base_array_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    base_array_const_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_array_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    base_array_const_iterator<T, Tag> operator-(
        const base_array_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    ptrdiff_t operator-(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );

    /// Relational operators for base_array_const_iterator (non member
    /// functions).

    template <class T, class Tag>
    bool operator==(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator!=(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<=(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>=(
        const base_array_const_iterator<T, Tag> &lhs,
        const base_array_const_iterator<T, Tag> &rhs
    );
}

#include "numcpp/array/array_iterator.tcc"

#endif // NUMCPP_ARRAY_ITERATOR_H_INCLUDED
