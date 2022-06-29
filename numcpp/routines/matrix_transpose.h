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

 /** @file include/numcpp/routines/matrix_transpose.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_TRANSPOSE_H_INCLUDED
#define NUMCPP_MATRIX_TRANSPOSE_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"
#include <complex>

namespace numcpp {
    template <class Tag>
    struct transpose_tag;

    template <class Tag>
    struct conj_transpose_tag;

    /**
     * @brief A matrix_transpose is a light-weight object which stores the 
     * transpose of a matrix. A matrix_transpose is a readonly matrix which is 
     * convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class T, class Tag>
    class base_matrix< T, transpose_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< T, transpose_tag<Tag> > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Matrix transpose constructor. Constructs a matrix_transpose.
         * 
         * @param mat Matrix-like object to transpose.
         */
        base_matrix(const base_matrix<T, Tag> &mat) : m_mat(mat) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * matrix_transpose.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     matrix_transpose. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the matrix_transpose. It does not point to any element, and thus 
         * shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     matrix_transpose.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the matrix_transpose (i.e., its reverse beginning). Reverse 
         * iterators iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the matrix_transpose.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the matrix_transpose (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     matrix_transpose.
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
         * the matrix_transpose.
         *
         * @param i Row position of an element in the matrix_transpose. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_transpose. Must 
         *     be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     matrix_transpose. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            return this->m_mat(j, i);
        }

        /** 
         * @brief Returns the number of rows in the matrix_transpose. 
         * 
         * @return The number of rows in the matrix_transpose.
         */
        size_t rows() const {
            return this->m_mat.cols();
        }

        /** 
         * @brief Returns the number of columns in the matrix_transpose.
         * 
         * @return The number of columns in the matrix_transpose.
         */
        size_t cols() const {
            return this->m_mat.rows();
        }

        /**
         * @brief Returns the number of elements in the matrix_transpose, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the matrix_transpose.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_transpose is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the matrix_transpose size is 0, false otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// Reference to the matrix argument.
        const base_matrix<T, Tag> &m_mat;
    };

    /**
     * @brief A matrix_conj_transpose is a light-weight object which stores the 
     * conjugate transpose of a matrix. A matrix_conj_transpose is a readonly 
     * matrix which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the matrix container.
     */
    template <class T, class Tag>
    class base_matrix< T, conj_transpose_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            T, conj_transpose_tag<Tag> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Matrix conjugate transpose constructor. Constructs a 
         * matrix_conj_transpose.
         * 
         * @param mat Matrix-like object to transpose.
         */
        base_matrix(const base_matrix<T, Tag> &mat) : m_mat(mat) {}

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * matrix_conj_transpose.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     matrix_conj_transpose. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the matrix_conj_transpose. It does not point to any element, and 
         * thus shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     matrix_conj_transpose.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the matrix_conj_transpose (i.e., its reverse beginning). Reverse 
         * iterators iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the matrix_conj_transpose.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the matrix_conj_transpose (i.e., its reverse 
         * end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     matrix_conj_transpose.
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
         * the matrix_conj_transpose.
         *
         * @param i Row position of an element in the matrix_conj_transpose. 
         *     Must be between 0 and rows() - 1.
         * @param i Column position of an element in the matrix_conj_transpose. 
         *     Must be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     matrix_conj_transpose. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            return this->m_mat(j, i);
        }

        /** 
         * @brief Returns the number of rows in the matrix_conj_transpose. 
         * 
         * @return The number of rows in the matrix_conj_transpose.
         */
        size_t rows() const {
            return this->m_mat.cols();
        }

        /** 
         * @brief Returns the number of columns in the matrix_conj_transpose.
         * 
         * @return The number of columns in the matrix_conj_transpose.
         */
        size_t cols() const {
            return this->m_mat.rows();
        }

        /**
         * @brief Returns the number of elements in the matrix_conj_transpose, 
         * i.e., rows()*cols().
         *
         * @return The number of elements in the matrix_conj_transpose.
         */
        size_t size() const {
            return this->rows() * this->cols();
        }

        /**
         * @brief Returns whether the matrix_conj_transpose is empty 
         * (i.e., whether its size is 0).
         *
         * @return true if the matrix_conj_transpose size is 0, false 
         * otherwise.
         */
        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// Reference to the matrix argument.
        const base_matrix<T, Tag> &m_mat;
    };

    /**
     * @brief Partial specialization of matrix_conj_transpose for complex
     * types.
     */
    template <class T, class Tag>
    class base_matrix< std::complex<T>, conj_transpose_tag<Tag> > {
    public:
        /// Member types:
        typedef std::complex<T> value_type;
        typedef std::complex<T> reference;
        typedef const std::complex<T> const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< 
            std::complex<T>, conj_transpose_tag<Tag> 
        > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        base_matrix(const base_matrix<std::complex<T>, Tag> &mat)
         : m_mat(mat) {}

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

        std::complex<T> operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->rows(), this->cols(), i, j);
            return std::conj(this->m_mat(j, i));
        }

        size_t rows() const {
            return this->m_mat.cols();
        }

        size_t cols() const {
            return this->m_mat.rows();
        }

        size_t size() const {
            return this->rows() * this->cols();
        }

        bool empty() const {
            return this->size() == 0;
        }

    protected:
        /// Reference to the matrix argument.
        const base_matrix<std::complex<T>, Tag> &m_mat;
    };
}

#endif // NUMCPP_MATRIX_TRANSPOSE_H_INCLUDED
