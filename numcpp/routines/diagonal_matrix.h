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

 /** @file include/numcpp/routines/diagonal_matrix.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/routines.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_DIAGONAL_MATRIX_H_INCLUDED
#define NUMCPP_DIAGONAL_MATRIX_H_INCLUDED

#include "numcpp/matrix/matrix_iterator.h"

namespace numcpp {
    template <class Tag>
    struct diagonal_tag;

    struct eye_tag;

    /**
     * @brief A diagonal_matrix is a light-weight object with given values on 
     * the diagonal and zeros elsewhere. A diagonal_matrix is a readonly matrix 
     * which is convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     * @tparam Tag Type of the array container with the elements on the 
     *     diagonal.
     */
    template <class T, class Tag>
    class base_matrix< T, diagonal_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator< T, diagonal_tag<Tag> > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Diagonal matrix constructor. Constructs a diagonal_matrix 
         * with given values on the diagonal and zeros elsewhere.
         * 
         * @param arr An array-like object with the elements on the diagonal.
         * @param offset Offset of the diagonal from the main diagonal. A 
         *     positive value refers to an upper diagonal and a negative value 
         *     refers to a lower diagonal. Defaults to main diagonal (0).
         */
        base_matrix(const base_array<T, Tag> &arr, ptrdiff_t offset = 0)
         : m_arr(arr), m_offset(offset) {
            if (offset >= 0) {
                this->m_size = arr.size() + offset;
            }
            else {
                this->m_size = arr.size() - offset;
            }
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * diagonal_matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     diagonal_matrix. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the diagonal_matrix. It does not point to any element, and thus 
         * shall not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     diagonal_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the diagonal_matrix (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the diagonal_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the diagonal_matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     diagonal_matrix.
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
         * the diagonal_matrix.
         *
         * @param i Row position of an element in the diagonal_matrix. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the diagonal_matrix. Must 
         *     be between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     diagonal_matrix. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->m_size, this->m_size, i, j);
            if (this->m_offset >= 0) {
                return i + this->m_offset == j ? this->m_arr[i] : T(0);
            }
            else {
                return i == j - this->m_offset ? this->m_arr[j] : T(0);
            }
        }

        /** 
         * @brief Returns the number of rows in the diagonal_matrix. 
         * 
         * @return The number of rows in the diagonal_matrix.
         */
        size_t rows() const {
            return this->m_size;
        }

        /** 
         * @brief Returns the number of columns in the diagonal_matrix.
         * 
         * @return The number of columns in the diagonal_matrix.
         */
        size_t cols() const {
            return this->m_size;
        }

        /**
         * @brief Returns the number of elements in the diagonal_matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the diagonal_matrix.
         */
        size_t size() const {
            return this->m_size * this->m_size;
        }

        /**
         * @brief Returns whether the diagonal_matrix is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the diagonal_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return this->m_size == 0;
        }

    protected:
        /// Number of rows and columns in the matrix.
        size_t m_size;

        /// Elements on the diagonal.
        const base_array<T, Tag> &m_arr;

        /// Offset from the main diagonal.
        ptrdiff_t m_offset;
    };

    /**
     * @brief A diagonal_array is a light-weight object with the elements on 
     * the diagonal of a given matrix. A diagonal_array is a readonly array 
     * which is convertible to an array object.
     *
     * @tparam T Type of the elements contained in the array.
     * @tparam Tag Type of the matrix container with the elements on the 
     *     diagonal.
     */
    template <class T, class Tag>
    class base_array< T, diagonal_tag<Tag> > {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_array_const_iterator< T, diagonal_tag<Tag> > iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Diagonal array constructor. Constructs a diagonal_array.
         * 
         * @param mat A matrix-like object from which the diagonal is taken.
         * @param offset Offset of the diagonal from the main diagonal. A 
         *     positive value refers to an upper diagonal and a negative value 
         *     refers to a lower diagonal. Defaults to main diagonal (0).
         */
        base_array(const base_matrix<T, Tag> &mat, ptrdiff_t offset = 0)
         : m_mat(mat), m_offset(offset) {
            this->m_size = 0;
            if (offset >= 0) {
                if (mat.cols() > (size_t)offset) {
                    this->m_size = std::min(mat.rows(), mat.cols() - offset);
                }
            }
            else {
                if (mat.rows() > (size_t)-offset) {
                    this->m_size = std::min(mat.rows() + offset, mat.cols());
                }
            }
        }

        /// Destructor.
        ~base_array() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * diagonal_array.
         *
         * @return A random access iterator to the beginning of the 
         *     diagonal_array. 
         */
        iterator begin() const {
            return iterator(this, 0);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the diagonal_array. It does not point to any element, and thus shall 
         * not be dereferenced.
         *
         * @return A random access iterator to the element past the end of the 
         *     diagonal_array.
         */
        iterator end() const {
            return iterator(this, this->size());
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the diagonal_array (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the diagonal_array.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the diagonal_array (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     diagonal_array.
         */
        reverse_iterator rend() const {
            return reverse_iterator(this->begin());
        }

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns the element at position i in the 
         * diagonal_array.
         *
         * @param i Position of an element in the diagonal_array. Must be 
         *     between 0 and size() - 1.
         *
         * @return The element in the sequence at the specified position. 
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        T operator[](size_t i) const {
            __assert_within_bounds(this->m_size, i);
            if (this->m_offset >= 0) {
                return this->m_mat(i, i + this->m_offset);
            }
            else {
                return this->m_mat(i - this->m_offset, i);
            }
        }

        /**
         * @brief Returns the number of elements in the diagonal_array.
         *
         * @return The number of elements in the diagonal_array.
         */
        size_t size() const {
            return this->m_size;
        }

        /**
         * @brief Returns whether the diagonal_array is empty (i.e., whether 
         * its size is 0).
         *
         * @return true if the diagonal_array size is 0, false otherwise.
         */
        bool empty() const {
            return this->m_size == 0;
        }

    protected:
        /// Number of elements in the diagonal.
        size_t m_size;

        /// Matrix from which the diagonal is taken.
        const base_matrix<T, Tag> &m_mat;

        /// Offset from the main diagonal.
        ptrdiff_t m_offset;
    };

    /**
     * @brief An eye_matrix is a light-weight object with ones on the diagonal 
     * and zeros elsewhere. An eye_matrix is a readonly matrix which is 
     * convertible to a matrix object.
     *
     * @tparam T Type of the elements contained in the matrix.
     */
    template <class T>
    class base_matrix<T, eye_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T reference;
        typedef const T const_reference;
        typedef void pointer;
        typedef void const_pointer;
        typedef base_matrix_const_iterator<T, eye_tag> iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Eye matrix constructor. Constructs an eye_matrix with ones on 
         * the diagonal and zeros elsewhere.
         * 
         * @param m Number of rows.
         * @param n Number of columns.
         * @param offset Offset of the diagonal from the main diagonal. A 
         *     positive value refers to an upper diagonal and a negative value 
         *     refers to a lower diagonal. Defaults to main diagonal (0).
         */
        base_matrix(size_t m, size_t n, ptrdiff_t offset = 0) {
            this->m_shape1 = m;
            this->m_shape2 = n;
            this->m_offset = offset;
        }

        /// Destructor.
        ~base_matrix() {}

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * eye_matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the eye_matrix. 
         */
        iterator begin() const {
            return iterator(this, 0, true);
        }

        iterator begin(bool row_major) const {
            return iterator(this, 0, row_major);
        }

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the eye_matrix. It does not point to any element, and thus shall not 
         * be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     eye_matrix.
         */
        iterator end() const {
            return iterator(this, this->size(), true);
        }

        iterator end(bool row_major) const {
            return iterator(this, this->size(), row_major);
        }

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the eye_matrix (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the eye_matrix.
         */
        reverse_iterator rbegin() const {
            return reverse_iterator(this->end());
        }
        
        reverse_iterator rbegin(bool row_major) const {
            return reverse_iterator(this->end(row_major));
        }

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the eye_matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     eye_matrix.
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
         * the eye_matrix.
         *
         * @param i Row position of an element in the eye_matrix. Must be 
         *     between 0 and rows() - 1.
         * @param i Column position of an element in the eye_matrix. Must be 
         *     between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     eye_matrix. 
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T operator()(size_t i, size_t j) const {
            __assert_within_bounds(this->m_shape1, this->m_shape2, i, j);
            if (this->m_offset >= 0) {
                return i + this->m_offset == j ? T(1) : T(0);
            }
            else {
                return i == j - this->m_offset ? T(1) : T(0);
            }
        }

        /** 
         * @brief Returns the number of rows in the eye_matrix. 
         * 
         * @return The number of rows in the eye_matrix.
         */
        size_t rows() const {
            return this->m_shape1;
        }

        /** 
         * @brief Returns the number of columns in the eye_matrix.
         * 
         * @return The number of columns in the eye_matrix.
         */
        size_t cols() const {
            return this->m_shape2;
        }

        /**
         * @brief Returns the number of elements in the eye_matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the eye_matrix.
         */
        size_t size() const {
            return this->m_shape1 * this->m_shape2;
        }

        /**
         * @brief Returns whether the eye_matrix is empty (i.e., whether its 
         * size is 0).
         *
         * @return true if the eye_matrix size is 0, false otherwise.
         */
        bool empty() const {
            return this->m_shape1 == 0 || this->m_shape2 == 0;
        }

    protected:
        /// Number of rows and columns in the eye_matrix.
        size_t m_shape1, m_shape2;

        /// Offset from the main diagonal.
        ptrdiff_t m_offset;
    };
}

#endif // NUMCPP_DIAGONAL_MATRIX_H_INCLUDED
