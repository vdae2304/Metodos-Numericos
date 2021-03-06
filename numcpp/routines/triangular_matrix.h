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

 /** @file include/numcpp/routines/triangular_matrix.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TRIANGULAR_MATRIX_H_INCLUDED
#define NUMCPP_TRIANGULAR_MATRIX_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"

namespace numcpp {
    template <class Tag>
    struct triangular_tag;

    /**
     * @brief A triangular_matrix is a light-weight object with values below
     * the diagonal and zeros elsewhere (lower triangular), or with values
     * above the diagonal and zeros elsewhere (upper triangular). A
     * triangular_matrix is a readonly matrix which is convertible to a matrix
     * object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container with the elements below/above
     *     the diagonal.
     */
    template <class T, class Tag>
    class base_matrix< T, triangular_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator< T, triangular_tag<Tag> > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Triangular matrix constructor.
         *
         * @param mat A matrix-like object from which to extract the
         *     lower/upper triangle.
         * @param lower Whether to extract the lower triangle or the upper
         *     triangle.
         * @param offset Diagonal above/below which to zero elements. A
         *     positive value refers to an upper diagonal and a negative value
         *     refers to a lower diagonal. Defaults to main diagonal (0).
         * @param unit_diagonal Whether to fill the diagonal with ones or with
         *     the elements from the matrix.
         */
        base_matrix(
            const base_matrix<T, Tag> &mat, bool lower,
            ptrdiff_t offset = 0, bool unit_diagonal = false
        ) : m_mat(mat), m_lower(lower), m_offset(offset),
            m_unit_diagonal(unit_diagonal) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * triangular_matrix.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the
         *     triangular_matrix.
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the triangular_matrix. It does not point to any element, and thus
         * shall not be dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     triangular_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the triangular_matrix (i.e., its reverse beginning). Reverse
         * iterators iterate backwards.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the triangular_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the triangular_matrix (i.e., its reverse end).
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the
         *     triangular_matrix.
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
         * the triangular_matrix.
         *
         * @param i Row position of an element in the triangular_matrix. Must
         *     be between 0 and rows() - 1.
         * @param i Column position of an element in the triangular_matrix.
         *     Must be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the
         *     triangular_matrix.
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (m_offset >= 0) {
                if (i + m_offset == j) {
                    return m_unit_diagonal ? T(1) : m_mat(i, j);
                }
                else if (i + m_offset > j) {
                    return m_lower ? m_mat(i, j) : T(0);
                }
                else {
                    return m_lower ? T(0) : m_mat(i, j);
                }
            }
            else {
                if (i == j - m_offset) {
                    return m_unit_diagonal ? T(1) : m_mat(i, j);
                }
                else if (i > j - m_offset) {
                    return m_lower ? m_mat(i, j) : T(0);
                }
                else {
                    return m_lower ? T(0) : m_mat(i, j);
                }
            }
        }

        /**
         * @brief Subscript operator. Returns the element at a given row and
         * column in the triangular_matrix.
         *
         * @param index An index_t object with the row and column position of
         *     an element in the triangular_matrix.
         *
         * @return The element at the specified row and column in the
         *     triangular_matrix.
         *
         * @throw std::out_of_range Thrown if either row or column index is out
         *     of bounds.
         */
        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        /**
         * @brief Returns the number of rows in the triangular_matrix.
         *
         * @return The number of rows in the triangular_matrix.
         */
        size_t rows() const {
            return m_mat.rows();
        }

        /**
         * @brief Returns the number of columns in the triangular_matrix.
         *
         * @return The number of columns in the triangular_matrix.
         */
        size_t cols() const {
            return m_mat.cols();
        }

        /**
         * @brief Returns the number of elements in the triangular_matrix,
         * i.e., rows()*cols().
         *
         * @return The number of elements in the triangular_matrix.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the triangular_matrix is empty (i.e., whether
         * its size is 0).
         *
         * @return true if the triangular_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Elements below/above the diagonal.
        const base_matrix<T, Tag> &m_mat;

        /// Whether to extract the lower or upper triangle.
        bool m_lower;

        /// Offset from the main diagonal.
        ptrdiff_t m_offset;

        /// Whether to fill the diagonal with ones or not.
        bool m_unit_diagonal;
    };
}

#endif // NUMCPP_TRIANGULAR_MATRIX_H_INCLUDED
