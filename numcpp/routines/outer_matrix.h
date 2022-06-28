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

 /** @file include/numcpp/routines/outer_matrix.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_OUTER_MATRIX_H_INCLUDED
#define NUMCPP_OUTER_MATRIX_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"

namespace numcpp {
    template <class Function, class T1, class Tag1, class T2, class Tag2>
    struct outer_tag;

    template <class Tag1, class Tag2>
    struct kronecker_tag;

    /**
     * @brief An outer_matrix is a light-weight object which stores the result 
     * of applying a binary function to all pairs of elements from two arrays. 
     * An outer_matrix is a readonly matrix which is convertible to a matrix 
     * object.
     *
     * @tparam R Result type of the function.
     * @tparam Function Type of the applied function.
     * @tparam T1 Type of the elements contained in the first array argument.
     * @tparam Tag1 Type of the first array container.
     * @tparam T2 Type of the elements contained in the second array argument.
     * @tparam Tag2 Type of the second array container.
     */
    template <
        class R, class Function, class T1, class Tag1, class T2, class Tag2
    >
    class base_matrix< R, outer_tag<Function, T1, Tag1, T2, Tag2> > {
    public:
        /// Member types:
        typedef R value_type;
        typedef R reference;
        typedef const R const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            R, outer_tag<Function, T1, Tag1, T2, Tag2> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Outer matrix constructor. Constructs an outer_matrix which 
         * stores the result of applying a function to all pairs of elements.
         * 
         * @param f The function to apply.
         * @param arr1 First array-like argument.
         * @param arr2 Second array-like argument.
         */
        base_matrix(
            Function f, 
            const base_array<T1, Tag1> &arr1, const base_array<T2, Tag2> &arr2
        ) : m_fun(f), m_arr1(arr1), m_arr2(arr2) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * outer_matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     outer_matrix. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the outer_matrix. It does not point to any element, and thus 
         * shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     outer_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the outer_matrix (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the outer_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the outer_matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     outer_matrix.
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
         * the outer_matrix.
         *
         * @param i Row position of an element in the outer_matrix. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the outer_matrix. Must be 
         *     between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     outer_matrix. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        R operator()(size_t i, size_t j) const {
            return this->m_fun(this->m_arr1[i], this->m_arr2[j]);
        }

        /** 
         * @brief Returns the number of rows in the outer_matrix. 
         * 
         * @return The number of rows in the outer_matrix.
         */
        size_t rows() const {
            return this->m_arr1.size();
        }

        /** 
         * @brief Returns the number of columns in the outer_matrix.
         * 
         * @return The number of columns in the outer_matrix.
         */
        size_t cols() const {
            return this->m_arr2.size();
        }

        /**
         * @brief Returns the number of elements in the outer_matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the outer_matrix.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the outer_matrix is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the outer_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// Underlying function.
        Function m_fun;

        /// Reference to the first argument.
        const base_array<T1, Tag1> &m_arr1;

        /// Reference to the second argument.
        const base_array<T2, Tag2> &m_arr2;
    };

    /**
     * @brief A kronecker_matrix is a light-weight object which stores the 
     * Kronecker product of two matrices. A kronecker_matrix is a readonly 
     * matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag1 Type of the first matrix container.
     * @tparam Tag2 Type of the second matrix container.
     */
    template <class T, class Tag1, class Tag2>
    class base_matrix< T, kronecker_tag<Tag1, Tag2> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, kronecker_tag<Tag1, Tag2>
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Kronecker product constructor.
         * 
         * @param mat1 First matrix-like argument.
         * @param mat2 Second matrix-like argument.
         */
        base_matrix(
            const base_matrix<T, Tag1> &mat1, const base_matrix<T, Tag2> &mat2
        ) : m_mat1(mat1), m_mat2(mat2) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * kronecker_matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     kronecker_matrix. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the kronecker_matrix. It does not point to any element, and thus 
         * shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     kronecker_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the kronecker_matrix (i.e., its reverse beginning). Reverse 
         * iterators iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the kronecker_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the kronecker_matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     kronecker_matrix.
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
         * the kronecker_matrix.
         *
         * @param i Row position of an element in the kronecker_matrix. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the kronecker_matrix. Must 
         *     be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     kronecker_matrix. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            size_t i1 = i / this->m_mat2.rows();
            size_t j1 = j / this->m_mat2.cols();
            size_t i2 = i % this->m_mat2.rows();
            size_t j2 = j % this->m_mat2.cols();
            return this->m_mat1(i1, j1) * this->m_mat2(i2, j2);
        }

        /** 
         * @brief Returns the number of rows in the kronecker_matrix. 
         * 
         * @return The number of rows in the kronecker_matrix.
         */
        size_t rows() const {
            return this->m_mat1.rows() * this->m_mat2.rows();
        }

        /** 
         * @brief Returns the number of columns in the kronecker_matrix.
         * 
         * @return The number of columns in the kronecker_matrix.
         */
        size_t cols() const {
            return this->m_mat1.cols() * this->m_mat2.cols();
        }

        /**
         * @brief Returns the number of elements in the kronecker_matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the kronecker_matrix.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the kronecker_matrix is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the kronecker_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// Reference to the first argument.
        const base_matrix<T, Tag1> &m_mat1;

        /// Reference to the second argument.
        const base_matrix<T, Tag2> &m_mat2;
    };
}

#endif // NUMCPP_OUTER_MATRIX_H_INCLUDED
