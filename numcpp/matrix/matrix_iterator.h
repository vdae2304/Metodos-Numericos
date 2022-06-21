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

 /** @file include/numcpp/matrix/matrix_iterator.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_ITERATOR_H_INCLUDED
#define NUMCPP_MATRIX_ITERATOR_H_INCLUDED

namespace numcpp {
    /// Forward declarations.
    template <class T, class Tag>
    class base_matrix_iterator;

    template <class T, class Tag>
    class base_matrix_const_iterator;

    /**
     * @brief A random access iterator for base_matrix class.
     *
     * @tparam T Type of the elements contained in the base_matrix.
     * @tparam Tag Type indicating which specialization of base_matrix refers 
     *     to.
     */
    template <class T, class Tag>
    class base_matrix_iterator {
    public:
        /// Member types.

        typedef ptrdiff_t difference_type;
        typedef typename base_matrix<T, Tag>::value_type value_type;
        typedef typename base_matrix<T, Tag>::pointer pointer;
        typedef typename base_matrix<T, Tag>::reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        /// Constructors.

        /// Default constructor.
        base_matrix_iterator();

        /// Flat index constructor.
        base_matrix_iterator(
            base_matrix<T, Tag> *mat, size_t index, bool order
        );

        /// Coordinate index constructor.
        base_matrix_iterator(
            base_matrix<T, Tag> *mat, size_t i, size_t j, bool order
        );

        /// Copy constructor.
        base_matrix_iterator(const base_matrix_iterator &other);

        /// Assignment operator.

        base_matrix_iterator& operator=(const base_matrix_iterator &other);

        /// Operator overloading.

        base_matrix_iterator& operator++();
        base_matrix_iterator& operator--();
        base_matrix_iterator operator++(int);
        base_matrix_iterator operator--(int);
        base_matrix_iterator& operator+=(difference_type rhs);
        base_matrix_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /// Accesses the underlying matrix.
        base_matrix<T, Tag>* base() const;

        /// Returns the underlying flat index.
        size_t index() const;

        /// Returns the underlying row position.
        size_t row() const;

        /// Returns the underlying column position.
        size_t col() const;

        /// Returns whether the matrix elements are iterated in row-major order.
        bool rowmajor() const;

        /// Returns whether the matrix elements are iterated in column-major 
        /// order.
        bool colmajor() const;

    protected:
        /// Pointer to the matrix associated to the iterator.
        base_matrix<T, Tag> *m_ptr;

        /// Flat index associated to the iterator.
        size_t m_index;

        /// Whether the elements are iterated in row-major or column-major 
        /// order.
        bool m_order;
    };

    /// Arithmetic operators for base_matrix_iterator (non member functions).

    template <class T, class Tag>
    base_matrix_iterator<T, Tag> operator+(
        const base_matrix_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    base_matrix_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_matrix_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    base_matrix_iterator<T, Tag> operator-(
        const base_matrix_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    ptrdiff_t operator-(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    /// Relational operators for base_matrix_iterator (non member functions).
    template <class T, class Tag>
    bool operator==(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator!=(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<=(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>=(
        const base_matrix_iterator<T, Tag> &lhs, 
        const base_matrix_iterator<T, Tag> &rhs
    );

    /**
     * @brief A random access iterator for const-qualified base_matrix class.
     *
     * @tparam T Type of the elements contained in the base_matrix.
     * @tparam Tag Type indicating which specialization of base_matrix refers 
     *     to.
     */
    template <class T, class Tag>
    class base_matrix_const_iterator {
    public:
        /// Member types.

        typedef ptrdiff_t difference_type;
        typedef typename base_matrix<T, Tag>::value_type value_type;
        typedef typename base_matrix<T, Tag>::const_pointer pointer;
        typedef typename base_matrix<T, Tag>::const_reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        /// Constructors.

        /// Default constructor.
        base_matrix_const_iterator();

        /// Flat index constructor.
        base_matrix_const_iterator(
            const base_matrix<T, Tag> *mat, size_t index, bool order
        );

        /// Coordinate index constructor.
        base_matrix_const_iterator(
            const base_matrix<T, Tag> *mat, size_t i, size_t j, bool order
        );

        /// Copy constructor.
        base_matrix_const_iterator(const base_matrix_iterator<T, Tag> &other);
        base_matrix_const_iterator(const base_matrix_const_iterator &other);

        /// Assignment operator.
        
        base_matrix_const_iterator& 
        operator=(const base_matrix_iterator<T, Tag> &other);
        base_matrix_const_iterator& 
        operator=(const base_matrix_const_iterator &other);
        
        /// Operator overloading.

        base_matrix_const_iterator& operator++();
        base_matrix_const_iterator& operator--();
        base_matrix_const_iterator operator++(int);
        base_matrix_const_iterator operator--(int);
        base_matrix_const_iterator& operator+=(difference_type rhs);
        base_matrix_const_iterator& operator-=(difference_type rhs);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        /// Public methods.

        /// Accesses the underlying matrix.
        const base_matrix<T, Tag>* base() const;

        /// Returns the underlying flat index.
        size_t index() const;

        /// Returns the underlying row position.
        size_t row() const;

        /// Returns the underlying column position.
        size_t col() const;

        /// Returns whether the matrix elements are iterated in row-major order.
        bool rowmajor() const;

        /// Returns whether the matrix elements are iterated in column-major 
        /// order.
        bool colmajor() const;

    protected:
        /// Pointer to the base_matrix associated to the iterator.
        const base_matrix<T, Tag> *m_ptr;

        /// Flat index associated to the iterator.
        size_t m_index;

        /// Whether the elements are iterated in row-major or column-major 
        /// order.
        bool m_order;
    };

    /// Arithmetic operators for base_matrix_const_iterator (non member 
    /// functions).

    template <class T, class Tag>
    base_matrix_const_iterator<T, Tag> operator+(
        const base_matrix_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    base_matrix_const_iterator<T, Tag> operator+(
        ptrdiff_t lhs, const base_matrix_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    base_matrix_const_iterator<T, Tag> operator-(
        const base_matrix_const_iterator<T, Tag> &lhs, ptrdiff_t rhs
    );

    template <class T, class Tag>
    ptrdiff_t operator-(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );

    /// Relational operators for base_matrix_const_iterator (non member 
    /// functions).

    template <class T, class Tag>
    bool operator==(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator!=(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator>(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );

    template <class T, class Tag>
    bool operator<=(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );
    
    template <class T, class Tag>
    bool operator>=(
        const base_matrix_const_iterator<T, Tag> &lhs, 
        const base_matrix_const_iterator<T, Tag> &rhs
    );
}

#include "numcpp/matrix/matrix_iterator.tcc"

#endif // NUMCPP_MATRIX_ITERATOR_H_INCLUDED
