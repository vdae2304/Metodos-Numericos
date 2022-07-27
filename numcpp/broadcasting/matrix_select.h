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

 /** @file include/numcpp/broadcasting/matrix_select.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_SELECT_H_INCLUDED
#define NUMCPP_MATRIX_SELECT_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"

namespace numcpp {
    /**
     * @brief A matrix_select is a light-weight object which selects elements
     * from two matrices depending on condition. A matrix_select is a readonly
     * matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class T, class TagCond, class TagTrue, class TagFalse>
    class base_matrix< T, select_tag<TagCond, TagTrue, TagFalse> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator<
            T, base_matrix< T, select_tag<TagCond, TagTrue, TagFalse> >
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Matrix select constructor. Constructs a matrix_select.
         *
         * @param condition Where true, yield x, otherwise yield y.
         * @param x Values from which to choose where condition is true.
         * @param y Values from which to choose where condition is false.
         */
        base_matrix(
            const base_matrix<bool, TagCond> &condition,
            const base_matrix<T, TagTrue> &x,
            const base_matrix<T, TagFalse> &y
        ) : m_cond(condition), m_true(x), m_false(y) {
            __assert_equal_shape(
                condition.rows(), condition.cols(), x.rows(), x.cols()
            );
            __assert_equal_shape(
                condition.rows(), condition.cols(), y.rows(), y.cols()
            );
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * matrix_select.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the
         *     matrix_select.
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the matrix_select. It does not point to any element, and thus shall
         * not be dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     matrix_select.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the matrix_select (i.e., its reverse beginning). Reverse iterators
         * iterate backwards.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the matrix_select.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the matrix_select (i.e., its reverse end).
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major or column-major order as specified by
         *     row_major. Otherwise, the elements are iterated in the same
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the
         *     matrix_select.
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
         * the matrix_select.
         *
         * @param i Row position of an element in the matrix_select. Must be
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_select. Must be
         *     between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the
         *     matrix_select.
         */
        T operator()(size_t i, size_t j) const {
            if (m_cond(i, j)) {
                return m_true(i, j);
            }
            else {
                return m_false(i, j);
            }
        }

        /**
         * @brief Subscript operator. Returns the element at a given row and
         * column in the matrix_select.
         *
         * @param index An index_t object with the row and column position of
         *     an element in the matrix_select.
         *
         * @return The element at the specified row and column in the
         *     matrix_select.
         */
        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        /**
         * @brief Returns the number of rows in the matrix_select.
         *
         * @return The number of rows in the matrix_select.
         */
        size_t rows() const {
            return m_cond.rows();
        }

        /**
         * @brief Returns the number of columns in the matrix_select.
         *
         * @return The number of columns in the matrix_select.
         */
        size_t cols() const {
            return m_cond.cols();
        }

        /**
         * @brief Returns the number of elements in the matrix_select, i.e.,
         * rows()*cols().
         *
         * @return The number of elements in the matrix_select.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_select is empty (i.e., whether its
         * size is 0).
         *
         * @return true if the matrix_select size is 0, false otherwise.
         */
        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_matrix<bool, TagCond> &m_cond;

        /// Reference to the true argument.
        const base_matrix<T, TagTrue> &m_true;

        /// Reference to the false argument.
        const base_matrix<T, TagFalse> &m_false;
    };

    /**
     * @brief Partial specialization of matrix_select when the second argument
     * is a single value.
     */
    template <class T, class TagCond, class TagTrue>
    class base_matrix< T, select_tag<TagCond, TagTrue, scalar_tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator<
            T, base_matrix< T, select_tag<TagCond, TagTrue, scalar_tag> >
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_matrix<bool, TagCond> &condition,
            const base_matrix<T, TagTrue> &x,
            const T &y
        ) : m_cond(condition), m_true(x), m_false(y) {
            __assert_equal_shape(
                condition.rows(), condition.cols(), x.rows(), x.cols()
            );
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        T operator()(size_t i, size_t j) const {
            if (m_cond(i, j)) {
                return m_true(i, j);
            }
            else {
                return m_false;
            }
        }

        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        size_t rows() const {
            return m_cond.rows();
        }

        size_t cols() const {
            return m_cond.cols();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_matrix<bool, TagCond> &m_cond;

        /// Reference to the true argument.
        const base_matrix<T, TagTrue> &m_true;

        /// False argument.
        T m_false;
    };

    /**
     * @brief Partial specialization of matrix_select when the first argument
     * is a single value.
     */
    template <class T, class TagCond, class TagFalse>
    class base_matrix< T, select_tag<TagCond, scalar_tag, TagFalse> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator<
            T, base_matrix< T, select_tag<TagCond, scalar_tag, TagFalse> >
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_matrix<bool, TagCond> &condition,
            const T &x,
            const base_matrix<T, TagFalse> &y
        ) : m_cond(condition), m_true(x), m_false(y) {
            __assert_equal_shape(
                condition.rows(), condition.cols(), y.rows(), y.cols()
            );
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        T operator()(size_t i, size_t j) const {
            if (m_cond(i, j)) {
                return m_true;
            }
            else {
                return m_false(i, j);
            }
        }

        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        size_t rows() const {
            return m_cond.rows();
        }

        size_t cols() const {
            return m_cond.cols();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_matrix<bool, TagCond> &m_cond;

        /// True argument.
        T m_true;

        /// Reference to the false argument.
        const base_matrix<T, TagFalse> &m_false;
    };

    /**
     * @brief Partial specialization of matrix_select when both argument are
     * single values.
     */
    template <class T, class TagCond>
    class base_matrix< T, select_tag<TagCond, scalar_tag, scalar_tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef nullptr_t pointer;
        typedef nullptr_t const_pointer;
        typedef base_matrix_const_iterator<
            T, base_matrix< T, select_tag<TagCond, scalar_tag, scalar_tag> >
        > iterator;
        typedef iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_matrix<bool, TagCond> &condition,
            const T &x,
            const T &y
        ) : m_cond(condition), m_true(x), m_false(y) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        reverse_iterator rend(bool row_major) const {
            return reverse_iterator(this->begin(row_major));
        }

        /// Matrix indexing.

        T operator()(size_t i, size_t j) const {
            if (m_cond(i, j)) {
                return m_true;
            }
            else {
                return m_false;
            }
        }

        T operator[](const index_t &index) const {
            return (*this)(index.first, index.second);
        }

        size_t rows() const {
            return m_cond.rows();
        }

        size_t cols() const {
            return m_cond.cols();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return (this->size() == 0);
        }

    protected:
        /// Reference to the condition argument.
        const base_matrix<bool, TagCond> &m_cond;

        /// True argument.
        T m_true;

        /// False argument.
        T m_false;
    };
}

#endif // NUMCPP_ARRAY_SELECT_H_INCLUDED
