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
#include <tuple>

namespace numcpp {
    template <class... Matrices>
    struct vstack_tag;

    template <class... Matrices>
    struct hstack_tag;

    /**
     * @brief A matrix_vstack is a light-weight object which stores the 
     * vertical concatenation of one or more matrices (row-wise). A 
     * matrix_vstack is a readonly matrix which is convertible to a matrix 
     * object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Matrices... Matrix containers to stack.
     */
    template <class T, class... Matrices>
    class base_matrix< T, vstack_tag<Matrices...> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, vstack_tag<Matrices...> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Row stack constructor. Constructs a matrix_vstack.
         * 
         * @param matrices Matrices to stack.
         */
        base_matrix(const Matrices&... matrices) : m_matrices(matrices...) {
            this->m_shape2 = __matrix_cols(std::get<0>(this->m_matrices));
            this->m_shape1 = __sum_rows();
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
         *     matrix_vstack. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            return __at(i, j);
        }

        /** 
         * @brief Returns the number of rows in the matrix_vstack. 
         * 
         * @return The number of rows in the matrix_vstack.
         */
        size_t rows() const {
            return this->m_shape1;
        }

        /** 
         * @brief Returns the number of columns in the matrix_vstack.
         * 
         * @return The number of columns in the matrix_vstack.
         */
        size_t cols() const {
            return this->m_shape2;
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

    private:
        /// Helper function. Return the sum of the columns of the matrices in 
        /// the concatenation.
        template <size_t I = 0>
        typename std::enable_if<I < sizeof...(Matrices), size_t>::type 
        __sum_rows() const {
            size_t m = __matrix_rows(std::get<I>(this->m_matrices));
            size_t n = __matrix_cols(std::get<I>(this->m_matrices));
            if (n != this->m_shape2) {
                char error[100];
                sprintf(
                    error, "Number of columns is not the same in row stack: "
                    "(?,%zu) (?,%zu)", this->m_shape2, n
                );
                throw std::invalid_argument(error);
            }
            return m + __sum_rows<I + 1>();
        }

        template <size_t I = 0>
        typename std::enable_if<I == sizeof...(Matrices), size_t>::type 
        __sum_rows() const {
            return 0;
        }

        /// Helper function. Return the element at row i and column j in the 
        /// concatenation.
        template <size_t I = 0>
        typename std::enable_if<I < sizeof...(Matrices), T>::type 
        __at(size_t i, size_t j) const {
            if (i < __matrix_rows(std::get<I>(this->m_matrices))) {
                return __matrix_element(std::get<I>(this->m_matrices), i, j);
            }
            else {
                i -= __matrix_rows(std::get<I>(this->m_matrices));
                return __at<I + 1>(i, j);
            }
        }

        template <size_t I = 0>
        typename std::enable_if<I == sizeof...(Matrices), T>::type 
        __at(size_t, size_t) const {
            return T();
        }

        /// Helper function. Return the number of rows of a matrix.
        template <class Tag>
        size_t __matrix_rows(const base_matrix<T, Tag> &mat) const {
            return mat.rows();
        }

        template <class Tag>
        size_t __matrix_rows(const base_array<T, Tag> &) const {
            return 1;
        }

        /// Helper function. Return the number of columns of a matrix.
        template <class Tag>
        size_t __matrix_cols(const base_matrix<T, Tag> &mat) const {
            return mat.cols();
        }

        template <class Tag>
        size_t __matrix_cols(const base_array<T, Tag> &arr) const {
            return arr.size();
        }

        /// Helper function. Return an element from a matrix.
        template <class Tag>
        T __matrix_element(
            const base_matrix<T, Tag> &mat, size_t i, size_t j
        ) const {
            return mat(i, j);
        }

        template <class Tag>
        T __matrix_element(
            const base_array<T, Tag> &arr, size_t, size_t j
        ) const {
            return arr[j];
        }

    protected:
        /// Number of rows and columns in the concatenation.
        size_t m_shape1, m_shape2;

        /// Matrix arguments.
        std::tuple<const Matrices&...> m_matrices;
    };

    /**
     * @brief A matrix_hstack is a light-weight object which stores the 
     * horizontal concatenation of one or more matrices (column-wise). A 
     * matrix_hstack is a readonly matrix which is convertible to a matrix 
     * object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Matrices... Matrix containers to stack.
     */
    template <class T, class... Matrices>
    class base_matrix< T, hstack_tag<Matrices...> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, hstack_tag<Matrices...> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Column stack constructor. Constructs a matrix_hstack.
         * 
         * @param matrices Matrices to stack.
         */
        base_matrix(const Matrices&... matrices) : m_matrices(matrices...) {
            this->m_shape1 = __matrix_rows(std::get<0>(this->m_matrices));
            this->m_shape2 = __sum_columns();
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
            return __at(i, j);
        }

        /** 
         * @brief Returns the number of rows in the matrix_hstack. 
         * 
         * @return The number of rows in the matrix_hstack.
         */
        size_t rows() const {
            return this->m_shape1;
        }

        /** 
         * @brief Returns the number of columns in the matrix_hstack.
         * 
         * @return The number of columns in the matrix_hstack.
         */
        size_t cols() const {
            return this->m_shape2;
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

    private:
        /// Helper function. Return the sum of the columns of the matrices in 
        /// the concatenation.
        template <size_t I = 0>
        typename std::enable_if<I < sizeof...(Matrices), size_t>::type 
        __sum_columns() const {
            size_t m = __matrix_rows(std::get<I>(this->m_matrices));
            size_t n = __matrix_cols(std::get<I>(this->m_matrices));
            if (m != this->m_shape1) {
                char error[100];
                sprintf(
                    error, "Number of rows is not the same in column stack: "
                    "(%zu,?) (%zu,?)", this->m_shape1, m
                );
                throw std::invalid_argument(error);
            }
            return n + __sum_columns<I + 1>();
        }

        template <size_t I = 0>
        typename std::enable_if<I == sizeof...(Matrices), size_t>::type 
        __sum_columns() const {
            return 0;
        }

        /// Helper function. Return the element at row i and column j in the 
        /// concatenation.
        template <size_t I = 0>
        typename std::enable_if<I < sizeof...(Matrices), T>::type 
        __at(size_t i, size_t j) const {
            if (j < __matrix_cols(std::get<I>(this->m_matrices))) {
                return __matrix_element(std::get<I>(this->m_matrices), i, j);
            }
            else {
                j -= __matrix_cols(std::get<I>(this->m_matrices));
                return __at<I + 1>(i, j);
            }
        }

        template <size_t I = 0>
        typename std::enable_if<I == sizeof...(Matrices), T>::type 
        __at(size_t, size_t) const {
            return T();
        }

        /// Helper function. Return the number of rows of a matrix.
        template <class Tag>
        size_t __matrix_rows(const base_matrix<T, Tag> &mat) const {
            return mat.rows();
        }

        template <class Tag>
        size_t __matrix_rows(const base_array<T, Tag> &arr) const {
            return arr.size();
        }

        /// Helper function. Return the number of columns of a matrix.
        template <class Tag>
        size_t __matrix_cols(const base_matrix<T, Tag> &mat) const {
            return mat.cols();
        }

        template <class Tag>
        size_t __matrix_cols(const base_array<T, Tag> &) const {
            return 1;
        }

        /// Helper function. Return an element from a matrix.
        template <class Tag>
        T __matrix_element(
            const base_matrix<T, Tag> &mat, size_t i, size_t j
        ) const {
            return mat(i, j);
        }

        template <class Tag>
        T __matrix_element(
            const base_array<T, Tag> &arr, size_t i, size_t
        ) const {
            return arr[i];
        }

    protected:
        /// Number of rows and columns in the concatenation.
        size_t m_shape1, m_shape2;

        /// Matrix arguments.
        std::tuple<const Matrices&...> m_matrices;
    };
}

#endif // NUMCPP_MATRIX_STACK_H_INCLUDED
