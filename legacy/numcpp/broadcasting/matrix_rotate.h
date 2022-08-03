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

 /** @file include/numcpp/broadcasting/array_rotate.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_ROTATE_H_INCLUDED
#define NUMCPP_MATRIX_ROTATE_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"

namespace numcpp {
    template <class Tag>
    struct rotate_tag;

    /**
     * @brief A matrix_rotate is a light-weight object which stores the
     * elements of a matrix rotated 90 degrees. A matrix_rot90 is a readonly
     * matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class T, class Tag>
    class base_matrix< T, rotate_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator< T, rotate_tag<Tag> > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Matrix rotate constructor. Constructs a matrix_rotate.
         *
         * @param mat Matrix to rotate.
         * @param times Number of times the matrix is rotated by 90 degrees.
         */
        base_matrix(const base_matrix<T, Tag> &mat, int times)
         : m_mat(mat) {
            m_times = times % 4;
            if (m_times < 0) {
                m_times += 4;
            }
         }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * matrix_rotate.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the
         *     matrix_rotate.
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the matrix_rotate. It does not point to any element, and thus
         * shall not be dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     matrix_rotate.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the matrix_rotate (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the matrix_rotate.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the matrix_rotate (i.e., its reverse end).
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the
         *     matrix_rotate.
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
         * the rotated matrix.
         *
         * @param i Row position of an element in the matrix_rotate. Must be
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_rotate. Must be
         *     between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the
         *     matrix_rotate.
         */
        T operator()(size_t i, size_t j) const {
            switch (m_times) {
            case 1:
                return m_mat(m_mat.rows() - 1 - j, i);
            case 2:
                return m_mat(m_mat.rows() - 1 - i, m_mat.cols() - 1 - j);
            case 3:
                return m_mat(j, m_mat.cols() - 1 - i);
            default:
                return m_mat(i, j);
            }
        }

        /**
         * @brief Subscript operator. Returns the element at a given row and
         * column in the rotated matrix.
         *
         * @param index An index_t object with the row and column position of
         *     an element in the matrix_rotate.
         *
         * @return The element at the specified row and column in the
         *     matrix_rotate.
         */
        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        /**
         * @brief Returns the number of rows in the matrix_rotate.
         *
         * @return The number of rows in the matrix_rotate.
         */
        size_t rows() const {
            if (m_times % 2 == 0) {
                return m_mat.rows();
            }
            else {
                return m_mat.cols();
            }
        }

        /**
         * @brief Returns the number of columns in the matrix_rotate.
         *
         * @return The number of columns in the matrix_rotate.
         */
        size_t cols() const {
            if (m_times % 2 == 0) {
                return m_mat.cols();
            }
            else {
                return m_mat.rows();
            }
        }

        /**
         * @brief Returns the number of elements in the matrix_rotate, i.e.,
         * rows()*cols().
         *
         * @return The number of elements in the matrix_rotate.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_rotate is empty (i.e., whether its
         * size is 0).
         *
         * @return true if the matrix_rotate size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the matrix argument.
        const base_matrix<T, Tag> &m_mat;

        /// Number of times the matrix is rotated.
        int m_times;
    };
}

#endif // NUMCPP_MATRIX_ROTATE_H_INCLUDED
