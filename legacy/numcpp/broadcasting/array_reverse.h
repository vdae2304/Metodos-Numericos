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

 /** @file include/numcpp/broadcasting/array_reverse.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_REVERSE_H_INCLUDED
#define NUMCPP_ARRAY_REVERSE_H_INCLUDED

#include "numcpp/array/array_iterator.h"
#include "numcpp/matrix/matrix_iterator.h"

namespace numcpp {
    template <class Tag>
    struct reverse_tag;

    /**
     * @brief An array_reverse is a light-weight object which stores the
     * elements of an array in reversed order. An array_reverse is a readonly
     * array which is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     * @tparam Tag Type of the array container.
     */
    template <class T, class Tag>
    class base_array< T, reverse_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_array_const_iterator< T, reverse_tag<Tag> > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Array reverse constructor. Constructs an array_reverse.
         *
         * @param arr Array to reverse.
         */
        base_array(const base_array<T, Tag> &arr) : m_arr(arr) {}

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * array_reverse.
         *
         * @return A random access iterator to the beginning of the
         *     array_reverse.
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the array_reverse. It does not point to any element, and thus shall
         * not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     array_reverse.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the array_reverse (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the array_reverse.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the array_reverse (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     array_reverse.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the element at position i in the
         * reversed array.
         *
         * @param i Position of an element in the array_reverse. Must be
         *     between 0 and size() - 1.
         *
         * @return The element in the array at the specified position.
         */
        T operator[](size_t i) const {
            return m_arr[m_arr.size() - 1 - i];
        }

        /**
         * @brief Returns the number of elements in the array_reverse.
         *
         * @return The number of elements in the array_reverse.
         */
        size_t size() const {
            return m_arr.size();
        }

        /**
         * @brief Returns whether the array_reverse is empty (i.e., whether
         * its size is 0).
         *
         * @return true if the array_reverse size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the array argument.
        const base_array<T, Tag> &m_arr;
    };

    /**
     * @brief A matrix_reverse is a light-weight object which stores the
     * elements of a matrix in reversed order along an axis. A matrix_reverse
     * is a readonly matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class T, class Tag>
    class base_matrix< T, reverse_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator< T, reverse_tag<Tag> > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Matrix reverse constructor. Constructs a matrix_reverse.
         *
         * @param mat Matrix to reverse.
         * @param rowwise Whether to reverse along rows or columns.
         */
        base_matrix(const base_matrix<T, Tag> &mat, bool rowwise)
         : m_mat(mat), m_order(rowwise) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * matrix_reverse.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the
         *     matrix_reverse.
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the matrix_reverse. It does not point to any element, and thus
         * shall not be dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     matrix_reverse.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the matrix_reverse (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the matrix_reverse.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the matrix_reverse (i.e., its reverse end).
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the
         *     matrix_reverse.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        /**
         * @brief Call operator. Returns the element at row i and column j in
         * the reversed matrix.
         *
         * @param i Row position of an element in the matrix_reverse. Must be
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_reverse. Must
         *     be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the
         *     matrix_reverse.
         */
        T operator()(size_t i, size_t j) const {
            if (m_order) {
                return m_mat(i, m_mat.cols() - 1 - j);
            }
            else {
                return m_mat(m_mat.rows() - 1 - i, j);
            }
        }

        /**
         * @brief Subscript operator. Returns the element at a given row and
         * column in the reversed matrix.
         *
         * @param index An index_t object with the row and column position of
         *     an element in the matrix_reverse.
         *
         * @return The element at the specified row and column in the
         *     matrix_reverse.
         */
        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        /**
         * @brief Returns the number of rows in the matrix_reverse.
         *
         * @return The number of rows in the matrix_reverse.
         */
        size_t rows() const {
            return m_mat.rows();
        }

        /**
         * @brief Returns the number of columns in the matrix_reverse.
         *
         * @return The number of columns in the matrix_reverse.
         */
        size_t cols() const {
            return m_mat.cols();
        }

        /**
         * @brief Returns the number of elements in the matrix_reverse, i.e.,
         * rows()*cols().
         *
         * @return The number of elements in the matrix_reverse.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_reverse is empty (i.e., whether
         * its size is 0).
         *
         * @return true if the matrix_reverse size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the matrix argument.
        const base_matrix<T, Tag> &m_mat;

        /// Whether to reverse along rows or columns.
        bool m_order;
    };
}

#endif // NUMCPP_ARRAY_REVERSE_H_INCLUDED
