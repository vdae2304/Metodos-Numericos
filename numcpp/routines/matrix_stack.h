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

 /** @file include/numcpp/routines/matrix_stack.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_STACK_H_INCLUDED
#define NUMCPP_MATRIX_STACK_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"
#include <cstdio>
#include <stdexcept>

namespace numcpp {
    template <
        template<class, class> class Container1, class Tag1, 
        template<class, class> class Container2, class Tag2
    >
    struct hstack_tag;

    template <
        template<class, class> class Container1, class Tag1, 
        template<class, class> class Container2, class Tag2
    >
    struct vstack_tag;

    /**
     * @brief A matrix_hstack is a light-weight object which stores the 
     * horizontal concatenation of two matrices (column-wise). A matrix_hstack 
     * is a readonly matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag1 Type of the first matrix container.
     * @tparam Tag2 Type of the second matrix container.
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, hstack_tag<base_matrix, Tag1, base_matrix, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, hstack_tag<base_matrix, Tag1, base_matrix, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Column stack constructor. Constructs a matrix_hstack.
         * 
         * @param mat1 First matrix argument.
         * @param mat2 Second matrix argument.
         */
        base_matrix(
            const base_matrix<T, Tag1> &mat1, const base_matrix<T, Tag2> &mat2
        ) : m_mat1(mat1), m_mat2(mat2) {
            if (mat1.rows() != mat2.rows()) {
                char error[140];
                sprintf(
                    error, "Number of rows do not match for column stack: "
                    "(%zu,%zu) (%zu,%zu)", mat1.rows(), mat1.cols(), 
                    mat2.rows(), mat2.cols()
                );
                throw std::invalid_argument(error);
            }
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * matrix_hstack.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     matrix_hstack. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the matrix_hstack. It does not point to any element, and thus 
         * shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     matrix_hstack.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the matrix_hstack (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the matrix_hstack.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the matrix_hstack (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     matrix_hstack.
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
         * the matrix_hstack.
         *
         * @param i Row position of an element in the matrix_hstack. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_hstack. Must 
         *     be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     matrix_hstack. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (j < this->m_mat1.cols()) {
                return this->m_mat1(i, j);
            }
            else {
                return this->m_mat2(i, j - this->m_mat1.cols());
            }
        }

        /** 
         * @brief Returns the number of rows in the matrix_hstack. 
         * 
         * @return The number of rows in the matrix_hstack.
         */
        size_t rows() const {
            return this->m_mat1.rows();
        }

        /** 
         * @brief Returns the number of columns in the matrix_hstack.
         * 
         * @return The number of columns in the matrix_hstack.
         */
        size_t cols() const {
            return this->m_mat1.cols() + this->m_mat2.cols();
        }

        /**
         * @brief Returns the number of elements in the matrix_hstack, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the matrix_hstack.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_hstack is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the matrix_hstack size is 0, false otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First matrix argument.
        const base_matrix<T, Tag1> &m_mat1;

        /// Second matrix argument.
        const base_matrix<T, Tag2> &m_mat2;
    };

    /**
     * @brief Partial specialization of matrix_hstack which stores the 
     * horizontal concatenation of a matrix and an array (column-wise).
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, hstack_tag<base_matrix, Tag1, base_array, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, hstack_tag<base_matrix, Tag1, base_array, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_matrix<T, Tag1> &mat1, const base_array<T, Tag2> &arr2
        ) : m_mat1(mat1), m_arr2(arr2) {
            if (mat1.rows() != arr2.size()) {
                char error[120];
                sprintf(
                    error, "Number of rows do not match for column stack: "
                    "(%zu,%zu) (%zu,)", mat1.rows(), mat1.cols(), arr2.size()
                );
                throw std::invalid_argument(error);
            }
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
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (j < this->m_mat1.cols()) {
                return this->m_mat1(i, j);
            }
            else {
                return this->m_arr2[i];
            }
        }

        size_t rows() const {
            return this->m_mat1.rows();
        }

        size_t cols() const {
            return this->m_mat1.cols() + 1;
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First matrix argument.
        const base_matrix<T, Tag1> &m_mat1;

        /// Second array argument.
        const base_array<T, Tag2> &m_arr2;
    };

    /**
     * @brief Partial specialization of matrix_hstack which stores the 
     * horizontal concatenation of an array and a matrix (column-wise).
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, hstack_tag<base_array, Tag1, base_matrix, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, hstack_tag<base_array, Tag1, base_matrix, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_array<T, Tag1> &arr1, const base_matrix<T, Tag2> &mat2
        ) : m_arr1(arr1), m_mat2(mat2) {
            if (arr1.size() != mat2.rows()) {
                char error[120];
                sprintf(
                    error, "Number of rows do not match for column stack: "
                    "(%zu,) (%zu,%zu)", arr1.size(), mat2.rows(), mat2.cols()
                );
                throw std::invalid_argument(error);
            }
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
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (j == 0) {
                return this->m_arr1[i];
            }
            else {
                return this->m_mat2(i, j - 1);
            }
        }

        size_t rows() const {
            return this->m_mat2.rows();
        }

        size_t cols() const {
            return 1 + this->m_mat2.cols();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First array argument.
        const base_array<T, Tag1> &m_arr1;

        /// Second matrix argument.
        const base_matrix<T, Tag2> &m_mat2;
    };

    /**
     * @brief Partial specialization of matrix_hstack which stores the 
     * horizontal concatenation of two arrays (column-wise).
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, hstack_tag<base_array, Tag1, base_array, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, hstack_tag<base_array, Tag1, base_array, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
        ) : m_arr1(arr1), m_arr2(arr2) {
            if (arr1.size() != arr2.size()) {
                char error[120];
                sprintf(
                    error, "Number of rows do not match for column stack: "
                    "(%zu,) (%zu,)", arr1.size(), arr2.size()
                );
                throw std::invalid_argument(error);
            }
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
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (j == 0) {
                return this->m_arr1[i];
            }
            else {
                return this->m_arr2[i];
            }
        }

        size_t rows() const {
            return this->m_arr1.size();
        }

        size_t cols() const {
            return 2;
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First array argument.
        const base_array<T, Tag1> &m_arr1;

        /// Second array argument.
        const base_array<T, Tag2> &m_arr2;
    };

    /**
     * @brief A matrix_vstack is a light-weight object which stores the 
     * vertical concatenation of two matrices (row-wise). A matrix_vstack is a 
     * readonly matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag1 Type of the first matrix container.
     * @tparam Tag2 Type of the second matrix container.
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, vstack_tag<base_matrix, Tag1, base_matrix, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, vstack_tag<base_matrix, Tag1, base_matrix, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Row stack constructor. Constructs a matrix_vstack.
         * 
         * @param mat1 First matrix argument.
         * @param mat2 Second matrix argument.
         */
        base_matrix(
            const base_matrix<T, Tag1> &mat1, const base_matrix<T, Tag2> &mat2
        ) : m_mat1(mat1), m_mat2(mat2) {
            if (mat1.cols() != mat2.cols()) {
                char error[140];
                sprintf(
                    error, "Number of columns do not match for row stack: "
                    "(%zu,%zu) (%zu,%zu)", mat1.rows(), mat1.cols(), 
                    mat2.rows(), mat2.cols()
                );
                throw std::invalid_argument(error);
            }
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * matrix_vstack.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     matrix_vstack. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the matrix_vstack. It does not point to any element, and thus 
         * shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     matrix_vstack.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the matrix_vstack (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the matrix_vstack.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the matrix_vstack (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     matrix_vstack.
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
         * the matrix_vstack.
         *
         * @param i Row position of an element in the matrix_vstack. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_vstack. Must 
         *     be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     matrix_hstack. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (i < this->m_mat1.rows()) {
                return this->m_mat1(i, j);
            }
            else {
                return this->m_mat2(i - this->m_mat1.rows(), j);
            }
        }

        /** 
         * @brief Returns the number of rows in the matrix_vstack. 
         * 
         * @return The number of rows in the matrix_vstack.
         */
        size_t rows() const {
            return this->m_mat1.rows() + this->m_mat2.rows();
        }

        /** 
         * @brief Returns the number of columns in the matrix_vstack.
         * 
         * @return The number of columns in the matrix_vstack.
         */
        size_t cols() const {
            return this->m_mat1.cols();
        }

        /**
         * @brief Returns the number of elements in the matrix_vstack, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the matrix_vstack.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_vstack is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the matrix_vstack size is 0, false otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First matrix argument.
        const base_matrix<T, Tag1> &m_mat1;

        /// Second matrix argument.
        const base_matrix<T, Tag2> &m_mat2;
    };

    /**
     * @brief Partial specialization of matrix_vstack which stores the 
     * vertical concatenation of a matrix and an array (row-wise).
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, vstack_tag<base_matrix, Tag1, base_array, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, vstack_tag<base_matrix, Tag1, base_array, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_matrix<T, Tag1> &mat1, const base_array<T, Tag2> &arr2
        ) : m_mat1(mat1), m_arr2(arr2) {
            if (mat1.cols() != arr2.size()) {
                char error[120];
                sprintf(
                    error, "Number of columns do not match for row stack: "
                    "(%zu,%zu) (%zu,)", mat1.rows(), mat1.cols(), arr2.size()
                );
                throw std::invalid_argument(error);
            }
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
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (i < this->m_mat1.rows()) {
                return this->m_mat1(i, j);
            }
            else {
                return this->m_arr2[j];
            }
        }

        size_t rows() const {
            return this->m_mat1.rows() + 1;
        }

        size_t cols() const {
            return this->m_mat1.cols();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First matrix argument.
        const base_matrix<T, Tag1> &m_mat1;

        /// Second array argument.
        const base_array<T, Tag2> &m_arr2;
    };

    /**
     * @brief Partial specialization of matrix_vstack which stores the 
     * vertical concatenation of an array and a matrix (row-wise).
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, vstack_tag<base_array, Tag1, base_matrix, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, vstack_tag<base_array, Tag1, base_matrix, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_array<T, Tag1> &arr1, const base_matrix<T, Tag2> &mat2
        ) : m_arr1(arr1), m_mat2(mat2) {
            if (arr1.size() != mat2.cols()) {
                char error[120];
                sprintf(
                    error, "Number of columns do not match for row stack: "
                    "(%zu,) (%zu,%zu)", arr1.size(), mat2.rows(), mat2.cols()
                );
                throw std::invalid_argument(error);
            }
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
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (i == 0) {
                return this->m_arr1[j];
            }
            else {
                return this->m_mat2(i - 1, j);
            }
        }

        size_t rows() const {
            return 1 + this->m_mat2.rows();
        }

        size_t cols() const {
            return this->m_mat2.cols();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First array argument.
        const base_array<T, Tag1> &m_arr1;

        /// Second matrix argument.
        const base_matrix<T, Tag2> &m_mat2;
    };

    /**
     * @brief Partial specialization of matrix_vstack which stores the 
     * vertical concatenation of two arrays (row-wise).
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, vstack_tag<base_array, Tag1, base_array, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, vstack_tag<base_array, Tag1, base_array, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(
            const base_array<T, Tag1> &arr1, const base_array<T, Tag2> &arr2
        ) : m_arr1(arr1), m_arr2(arr2) {
            if (arr1.size() != arr2.size()) {
                char error[120];
                sprintf(
                    error, "Number of columns do not match for row stack: "
                    "(%zu,) (%zu,)", arr1.size(), arr2.size()
                );
                throw std::invalid_argument(error);
            }
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
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            if (i == 0) {
                return this->m_arr1[j];
            }
            else {
                return this->m_arr2[j];
            }
        }

        size_t rows() const {
            return 2;
        }

        size_t cols() const {
            return this->m_arr1.size();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// First array argument.
        const base_array<T, Tag1> &m_arr1;

        /// Second array argument.
        const base_array<T, Tag2> &m_arr2;
    };
}

#endif // NUMCPP_MATRIX_STACK_H_INCLUDED
