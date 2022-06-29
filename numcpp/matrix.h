/*
 * This file is part of the NumCpp project.
 *
 * NumCpp is a package for scientific computing in C++. It is a C++ library 
 * that provides an matay and a matrix object, and an assortment of routines 
 * for fast operations on matays and matrices, including mathematical, logical, 
 * sorting, selecting, I/O and much more.
 *
 * The NumCpp package is inspired by the NumPy package for Python, although it 
 * is not related to it or any of its parts.
 *
 * This program is free software: you can redistribute it and/or modify it by 
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/matrix.h
  *  This header defines matrix and matrix_view classes.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_H_INCLUDED
#define NUMCPP_MATRIX_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/slice.h"
#include "numcpp/routines/algo.h"

#include "numcpp/matrix/matrix_iterator.h"
#include "numcpp/matrix/lazy_matrix.h"
#include "numcpp/matrix/lazy_axis.h"
#include "numcpp/matrix/matrix_view.h"

#include <initializer_list>
#include <utility>

namespace numcpp {
    /**
     * @brief Matrices are contiguous two dimensional sequence containers: they 
     * hold a specific number of elements arranged in rows and columns. Unlike 
     * a matrix_view, a matrix is always owner of its own data and the elements 
     * are always stored in row-major order.
     * 
     * Matrices are designed to easily perform mathematical operations on them. 
     * Most mathematical operations can be applied directly to matrix objects, 
     * affecting all its elements.
     * 
     * @tparam T Type of the elements contained in the matrix. This shall be an 
     *     arithmetic type or a class that behaves like one (such as 
     *     std::complex).
     */
    template <class T>
    class base_matrix<T, matrix_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef base_matrix_iterator<T, matrix_tag> iterator;
        typedef base_matrix_const_iterator<T, matrix_tag> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /** 
         * @brief Default constructor. Constructs an empty matrix with no 
         * elements. 
         */
        base_matrix();

        /** 
         * @brief Size constructor. Constructs a matrix with m rows and n 
         * columns, each element is left uninitialized. 
         * 
         * @param m Number of rows.
         * @param n Number of columns.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        base_matrix(size_t m, size_t n);

        /** 
         * @brief Fill constructor. Constructs a matrix with m rows and n 
         * columns, each element initialized to val. 
         * 
         * @param m Number of rows.
         * @param n Number of columns.
         * @param val Value to which each of the elements is initialized.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        base_matrix(size_t m, size_t n, const T &val);

        /** 
         * @brief Range constructor. Constructs a matrix with at least as many 
         * elements as the range [first, last), with each element constructed 
         * from its corresponding element in that range, in the same order. 
         * 
         * @param first Input iterator to the initial position in a range.
         * @param last Input iterator to the final position in a range. The 
         *     range used is [first, last), which includes all the elements 
         *     between first and last, including the element pointed by first 
         *     but not the element pointed by last.
         * @param n Number of columns. The number of rows is inferred from the 
         *     length of the range.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <
            class InputIterator,
            typename = RequiresInputIterator<InputIterator>
        >
        base_matrix(InputIterator first, InputIterator last, size_t n);

        /** 
         * @brief Copy constructor. Constructs a matrix with a copy of each of 
         * the elements in other, in the same order. 
         * 
         * @param other A matrix-like object of the same type.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        base_matrix(const base_matrix &other);
        template <class Tag>
        base_matrix(const base_matrix<T, Tag> &other);

        /** 
         * @brief Move constructor. Constructs a matrix that acquires the 
         * elements of other. 
         * 
         * @param other A matrix of the same type. The ownership is directly 
         *     transferred from other. other is left in an empty state.
         */
        base_matrix(base_matrix &&other);

        /** 
         * @brief Nested initializer list constructor. Constructs a matrix with 
         * a copy of each of the elements in il, in the same order. 
         * 
         * @param il An initializer_list object.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        base_matrix(std::initializer_list< std::initializer_list<T> > il);

        /// Destructor.
        ~base_matrix();

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the matrix. If 
         *     the matrix is const-qualified, the function returns a 
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator begin();
        const_iterator begin() const;

        iterator begin(bool row_major);
        const_iterator begin(bool row_major) const;

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the matrix. It does not point to any element, and thus shall not be 
         * dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     matrix. If the matrix is const-qualified, the function returns a 
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator end();
        const_iterator end() const;

        iterator end(bool row_major);
        const_iterator end(bool row_major) const;

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the matrix (i.e., its reverse beginning). Reverse iterators iterate 
         * backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the matrix. If the matrix is const-qualified, the function 
         *     returns a const_reverse_iterator. Otherwise, it returns a 
         *     reverse_iterator.
         */
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;

        reverse_iterator rbegin(bool row_major);
        const_reverse_iterator rbegin(bool row_major) const;

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     matrix. If the matrix is const-qualified, the function returns a 
         *     const_reverse_iterator. Otherwise, it returns a reverse_iterator.
         */
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        
        reverse_iterator rend(bool row_major);
        const_reverse_iterator rend(bool row_major) const;

        /**
         * @brief Returns a const_iterator pointing to the first element in the 
         * matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_iterator to the beginning of the matrix.
         */
        const_iterator cbegin() const;
        const_iterator cbegin(bool row_major) const;

        /**
         * @brief Returns a const_iterator pointing to the past-the-end element 
         * in the matrix.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_iterator to the element past the end of the matrix.
         */
        const_iterator cend() const;
        const_iterator cend(bool row_major) const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the last element 
         * in the matrix (i.e., its reverse beginning).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_reverse_iterator to the reverse beginning of the 
         *     matrix.
         */
        const_reverse_iterator crbegin() const;
        const_reverse_iterator crbegin(bool row_major) const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the element 
         * preceding the first element in the matrix (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_reverse_iterator to the reverse end of the matrix.
         */
        const_reverse_iterator crend() const;
        const_reverse_iterator crend(bool row_major) const;

        /// Matrix indexing.
        
        /** 
         * @brief Call operator. Returns a reference to the element at row i 
         * and column j in the matrix.
         * 
         * @param i Row position of an element in the matrix. Must be between 0 
         *     and rows() - 1.
         * @param j Column position of an element in the matrix. Must be 
         *     between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the matrix. 
         *     If the matrix is const-qualified, the function returns a 
         *     reference to const T. Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T& operator()(size_t i, size_t j);
        const T& operator()(size_t i, size_t j) const;

        /**
         * @brief Slice indexing: Returns a matrix_view object that selects the 
         * elements specified by the slices.
         *
         * @param slc1 A slice object specifying which rows of the matrix are 
         *     selected.
         * @param slc2 A slice object specifying which columns of the matrix 
         *     are selected.
         *
         * @return If the matrix is const-qualified, the function returns a 
         *     const matrix_view object, which is convertible to a matrix 
         *     object. Otherwise, the function returns a matrix_view object, 
         *     which has reference semantics to the original matrix.
         *
         * @throw std::out_of_range Thrown if the slices references to an 
         *     element out of bounds.
         */
        matrix_view<T> operator()(slice slc1, slice slc2);
        const matrix_view<T> operator()(slice slc1, slice slc2) const;

        /**
         * @brief Row indexing: Returns an array_view object that selects the 
         * elements specified by the slice along the given row.
         * 
         * @param i Row position in the matrix. Must be between 0 and 
         *     rows() - 1.
         * @param slc A slice object specifying which columns of the matrix are 
         *     selected.
         * 
         * @return If the matrix is const-qualified, the function returns a 
         *     const array_view object, which is convertible to an array 
         *     object. Otherwise, the function returns an array_view object, 
         *     which has reference semantics to the original matrix.
         * 
         * @throw std::out_of_range Thrown if the slice references to an 
         *     element out of bounds.
         */
        array_view<T> operator()(size_t i, slice slc);
        const array_view<T> operator()(size_t i, slice slc) const;

        /**
         * @brief Column indexing: Returns an array_view object that selects 
         * the elements specified by the slice along the given column.
         * 
         * @param slc A slice object specifying which rows of the matrix are 
         *     selected.
         * @param j Column position in the matrix. Must be between 0 and 
         *     cols() - 1.
         * 
         * @return If the matrix is const-qualified, the function returns a 
         *     const array_view object, which is convertible to an array 
         *     object. Otherwise, the function returns an array_view object, 
         *     which has reference semantics to the original matrix.
         * 
         * @throw std::out_of_range Thrown if the slice references to an 
         *     element out of bounds.
         */
        array_view<T> operator()(slice slc, size_t j);
        const array_view<T> operator()(slice slc, size_t j) const;

        /** 
         * @brief Coordinate array indexing: Returns an index_view object that 
         * selects the elements specified by the array of pairs of indices. 
         * 
         * @param index An array< std::pair<size_t, size_t> > object with its 
         *     elements identifying which elements of the matrix are selected.
         *
         * @return If the matrix is const-qualified, the function returns a new 
         *     array object with a copy of the selection. Otherwise, the 
         *     function returns an index_view object, which has reference 
         *     semantics to the original matrix.
         *
         * @throw std::out_of_range Thrown if the array of indices references 
         *     to an element out of bounds.
         * @throw std::bad_alloc If the function needs to allocate storage and 
         *     fails, it may throw an exception.
         */
        template <class Tag>
        index_view<T> operator[](
            const base_array<std::pair<size_t, size_t>, Tag> &index
        );

        template <class Tag>
        array<T> operator[](
            const base_array<std::pair<size_t, size_t>, Tag> &index
        ) const;

        /** 
         * @brief Boolean matrix indexing: Returns an index_view object that 
         * selects the elements specified by the boolean mask.
         * 
         * @param mask A matrix<bool> object with its elements identifying 
         *     whether each element of the matrix is selected or not.
         *
         * @return If the matrix is const-qualified, the function returns a new 
         *     array object with a copy of the selection. Otherwise, the 
         *     function returns an index_view object, which has reference 
         *     semantics to the original matrix.
         *
         * @throw std::bad_alloc If the function needs to allocate storage and 
         *     fails, it may throw an exception. 
         */
        template <class Tag>
        index_view<T> operator[](const base_matrix<bool, Tag> &mask);
        
        template <class Tag>
        array<T> operator[](const base_matrix<bool, Tag> &mask) const;

        /** 
         * @brief Returns the number of rows in the matrix. 
         * 
         * @return The number of rows in the matrix.
         */
        size_t rows() const;

        /** 
         * @brief Returns the number of columns in the matrix.
         * 
         * @return The number of columns in the matrix.
         */
        size_t cols() const;

        /**
         * @brief Returns the number of elements in the matrix, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the matrix.
         */
        size_t size() const;

        /**
         * @brief Resizes the matrix so that it contains m rows and n columns. 
         * After resizing, if the new size is different from the total number 
         * of elements in the matrix, the previous contents are lost. 
         * Otherwise, the contents are preserved, but stored in a different 
         * order.
         * 
         * @param m New number of rows of the matrix.
         * @param n New number of columns of the matrix.
         * 
         * @warning Invalidates all iterators, references and views to elements 
         *     of the matrix.
         */
        void resize(size_t m, size_t n);

        /** 
         * @brief Returns whether the matrix is empty (i.e., whether its size 
         * is 0).
         *
         * @return true if the matrix size is 0, false otherwise.
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the 
         * matrix. Because elements in the matrix are stored contiguously and 
         * in row-major order, the pointer retrieved can be offset to access 
         * any element in the matrix.
         *
         * @return A pointer to the array used internally by the matrix. If the 
         *     matrix is const-qualified, the function returns a pointer to 
         *     const T. Otherwise, it returns a pointer to T.
         */
        T* data();
        const T* data() const;

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns to each element the value of the 
         * corresponding element in other after resizing the object (if 
         * necessary).
         *
         * @param other A matrix-like object of the same type.
         * 
         * @return *this
         *
         * @throw std::bad_alloc If the function needs to allocate storage and 
         *     fails, it may throw an exception.
         *
         * @warning When the number of rows and columns do not mach, 
         *     invalidates all iterators, references and views to elements of 
         *     the matrix. Otherwise, valid iterators, references and views 
         *     keep their validity.
         */
        base_matrix& operator=(const base_matrix &other);
        template <class Tag>
        base_matrix& operator=(const base_matrix<T, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element. The size of 
         * the matrix is preserved.
         *
         * @param val Value assigned to all the elements in the matrix.
         * 
         * @return *this
         */
        base_matrix& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving 
         * other in an empty state.
         *
         * @param other A matrix of the same type.
         * 
         * @return *this
         *
         * @warning All iterators, references and views to elements of the 
         *     matrix are invalidated.
         */
        base_matrix& operator=(base_matrix &&other);

        /** 
         * @brief Nested initializer list assignment. Assigns to each element 
         * the value of the corresponding element in il after resizing the 
         * object (if necessary).
         * 
         * @param il An initializer_list object.
         * 
         * @return *this
         *
         * @throw std::bad_alloc If the function needs to allocate storage and 
         *     fails, it may throw an exception.
         *
         * @warning When the number of rows and columns do not mach, 
         *     invalidates all iterators, references and views to elements of 
         *     the matrix. Otherwise, valid iterators, references and views 
         *     keep their validity.
         */
        base_matrix& 
        operator=(std::initializer_list< std::initializer_list<T> > il);

        /// Compound assignment operator.

        /**
         * @brief Assigns to *this the result of performing the respective 
         * operation on all the elements in the matrix.
         *
         * When the right-hand side argument is a matrix object, the operation 
         * is performed between the corresponding elements in each object (the 
         * first element of the left one with the right one, the second with 
         * the second, and so on...)
         *
         * When the right-hand side argument is a value, the operation is 
         * applied to all the elements in the matrix against that value.
         *
         * @param rhs Right-hand side matrix-like object.
         * @param val Value to use as right-hand operand.
         *
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the right-hand side argument 
         *     is a matrix object with different number of rows and columns to 
         *     *this.
         */
        base_matrix& operator+=(const base_matrix &rhs);
        base_matrix& operator-=(const base_matrix &rhs);
        base_matrix& operator*=(const base_matrix &rhs);
        base_matrix& operator/=(const base_matrix &rhs);
        base_matrix& operator%=(const base_matrix &rhs);
        base_matrix& operator&=(const base_matrix &rhs);
        base_matrix& operator|=(const base_matrix &rhs);
        base_matrix& operator^=(const base_matrix &rhs);
        base_matrix& operator<<=(const base_matrix &rhs);
        base_matrix& operator>>=(const base_matrix &rhs);
        
        template <class Tag>
        base_matrix& operator+=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator-=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator*=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator/=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator%=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator&=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator|=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator^=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator<<=(const base_matrix<T, Tag> &rhs);
        template <class Tag>
        base_matrix& operator>>=(const base_matrix<T, Tag> &rhs);

        base_matrix& operator+=(const T &val);
        base_matrix& operator-=(const T &val);
        base_matrix& operator*=(const T &val);
        base_matrix& operator/=(const T &val);
        base_matrix& operator%=(const T &val);
        base_matrix& operator&=(const T &val);
        base_matrix& operator|=(const T &val);
        base_matrix& operator^=(const T &val);
        base_matrix& operator<<=(const T &val);
        base_matrix& operator>>=(const T &val);

        /// Public methods.

        /**
         * @brief Assigns to each element the result of applying a function to 
         * the corresponding elements in *this.
         *
         * @param f A function that accepts one element of type T as argument, 
         *     and returns a value convertible to T.
         */
        void apply(T f(T));
        void apply(T f(const T&));
        template <class Function>
        void apply(Function f);

        /**
         * @brief Return the pair of indices of the maximum value in the 
         * matrix.
         * 
         * @param i Reference to an index where the row location will be 
         *     stored.
         * @param j Reference to an index where the column location will be 
         *     stored.
         */
        void argmax(size_t &i, size_t &j) const;

        /** 
         * @brief Return the index of the maximum value in the matrix along the 
         * specified axis. 
         * 
         * @param rowwise If true, return the index of the maximum value along 
         *     each row. Otherwise, return the index of the maximum value along 
         *     each column.
         * 
         * @return A light-weight object with the index of the maximum value 
         *     along an axis. Convertible to an array object.
         */
        base_array< size_t, lazy_axis_tag<__range_argmax, T, matrix_tag> >
        argmax(bool rowwise) const;

        /**
         * @brief Return the pair of indices of the minimum value in the 
         * matrix.
         * 
         * @param i Reference to an index where the row location will be 
         *     stored.
         * @param j Reference to an index where the column location will be 
         *     stored.
         */
        void argmin(size_t &i, size_t &j) const;

        /** 
         * @brief Return the index of the minimum value in the matrix along the 
         * specified axis. 
         * 
         * @param rowwise If true, return the index of the minimum value along 
         *     each row. Otherwise, return the index of the minimum value along 
         *     each column.
         * 
         * @return A light-weight object with the index of the minimum value 
         *     along an axis. Convertible to an array object.
         */
        base_array< size_t, lazy_axis_tag<__range_argmin, T, matrix_tag> >
        argmin(bool rowwise) const;

        /**
         * @brief Cast each element to a specified type.
         * 
         * @return A light-weight object with the elements in the matrix casted 
         *     to the specified type. Convertible to a matrix object.
         */
        template <class U>
        base_matrix< U, lazy_unary_tag<__identity, T, matrix_tag> > 
        astype() const;

        /**
         * @brief Clamp the values in the matrix. Given an interval 
         * [a_min, a_max], values smaller than a_min become a_min, and values 
         * larger than a_max become a_max.
         * 
         * If T is a complex type, then real and imaginary parts are clamped 
         * independently.
         *
         * @param a_min The lower boundary to clamp.
         * @param a_max The upper boundary to clamp.
         *
         * @note The behavior is undefined if a_min is greater than a_max.
         */
        void clamp(const T &a_min, const T &a_max);

        /**
         * @brief Return the complex conjugate, element-wise. 
         * 
         * @return A light-weight object with the complex conjugate of each 
         *     element in the matrix. Convertible to a matrix object.
         */
        base_matrix< T, lazy_unary_tag<__conjugate, T, matrix_tag> > 
        conj() const;

        /**
         * @brief Return a view of the diagonal of *this. 
         * 
         * @param offset Offset of the diagonal from the main diagonal. A 
         *    positive value refers to an upper diagonal and a negative value 
         *    refers to a lower diagonal. Defaults to 0 (main diagonal).
         * 
         * @return If the matrix is const-qualified, the function returns a 
         *     const array_view object, which is convertible to an array 
         *     object. Otherwise, the function returns an array_view object, 
         *     which has reference semantics to the original matrix.
         */
        array_view<T> diagonal(ptrdiff_t offset = 0);
        const array_view<T> diagonal(ptrdiff_t offset = 0) const;

        /**
         * @brief Return the matrix multiplication of a matrix and a 
         * column-vector.
         *
         * @param rhs Right-hand side array-like object.
         *
         * @return The matrix-vector multiplication with rhs.
         *
         * @throw std::invalid_argument Thrown if the number of columns of 
         *     *this and the size of rhs do not match.
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class Tag>
        array<T> dot(const base_array<T, Tag> &rhs) const;

        /**
         * @brief Return the matrix multiplication of two matrices.
         *
         * @param rhs Right-hand side matrix-like object.
         *
         * @return The matrix multiplication with rhs.
         *
         * @throw std::invalid_argument Thrown if the number of columns of 
         *     *this and the number of rows of rhs do not match.
         * @throw std::bad_alloc If the function fails to allocate storage it 
         *     may throw an exception.
         */
        template <class Tag>
        base_matrix dot(const base_matrix<T, Tag> &rhs) const;

        /** 
         * @brief Return a view of the matrix collapsed into an array. 
         * 
         * @return If the matrix is const-qualified, the function returns a 
         *     const array_view object, which is convertible to an array 
         *     object. Otherwise, the function returns an array_view object, 
         *     which has reference semantics to the original matrix.
         */
        array_view<T> flatten();
        const array_view<T> flatten() const;

        /**
         * @brief Return the imaginary part, element-wise.
         * 
         * @return A light-weight object with the imaginary part of each 
         *     element in the matrix. Convertible to a matrix object.
         */
        base_matrix< 
            typename complex_traits<T>::value_type, 
            lazy_unary_tag<__imag_part, T, matrix_tag> 
        > 
        imag() const;

        /**
         * @brief Return the maximum value contained in the matrix.
         *
         * @return The maximum value in the matrix.
         */
        T max() const;

        /** 
         * @brief Return the maximum value contained in the matrix along the 
         * specified axis. 
         * 
         * @param rowwise If true, return the maximum value along each row. 
         *     Otherwise, return the maximum value along each column.
         * 
         * @return A light-weight object with the maximum value along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_max, T, matrix_tag> >
        max(bool rowwise) const;

        /** 
         * @brief Return the average of the matrix elements.
         * 
         * @return The average of the matrix elements.
         */
        T mean() const;

        /**
         * @brief Return the average of the matrix elements along the specified 
         * axis.
         * 
         * @param rowwise If true, return the average of the elements along 
         *     each row. Otherwise, return the average of the elements along 
         *     each column.
         *
         * @return A light-weight object with the average along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_mean, T, matrix_tag> >
        mean(bool rowwise) const;

        /**
         * @brief Return the minimum value contained in the matrix.
         *
         * @return The minimum value in the matrix.
         */
        T min() const;

        /** 
         * @brief Return the minimum value contained in the matrix along the 
         * specified axis. 
         * 
         * @param rowwise If true, return the minimum value along each row. 
         *     Otherwise, return the minimum value along each column.
         * 
         * @return A light-weight object with the minimum value along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_min, T, matrix_tag> > 
        min(bool rowwise) const;

        /**
         * @brief Partition a matrix in-place.
         * 
         * @param kth Element index to partition by. The element at the kth 
         *     position is the element that would be in that position in the 
         *     sorted array. The other elements are left without any specific 
         *     order, except that none of the elements preceding kth are 
         *     greater than it, and none of the elements following it are less.
         * @param rowwise If true, partition the elements along each row. 
         *     Otherwise, partition the elements along each column.
         * @param comp Custom comparator. A binary function that accepts two 
         *     elements of type T as arguments, and returns a value convertible 
         *     to bool. The value returned indicates whether the element passed 
         *     as first argument is considered to go before the second.
         */
        void partition(size_t kth, bool rowwise);
        template <class Compare>
        void partition(size_t kth, bool rowwise, Compare comp);

        /**
         * @brief Return the product of the matrix elements.
         * 
         * @return The product of the matrix elements.
         */
        T prod() const;

        /**
         * @brief Return the product of the matrix elements along the specified 
         * axis.
         * 
         * @param rowwise If true, return the product of the elements along 
         *     each row. Otherwise, return the product of the elements along 
         *     each column.
         *
         * @return A light-weight object with the product along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_prod, T, matrix_tag> >
        prod(bool rowwise) const;

        /**
         * @brief Return the real part, element-wise.
         * 
         * @return A light-weight object with the real part of each element in 
         *     the matrix. Convertible to a matrix object.
         */
        base_matrix< 
            typename complex_traits<T>::value_type, 
            lazy_unary_tag<__real_part, T, matrix_tag> 
        > 
        real() const;

        /**
         * @brief Reverse the order of the elements in-place along an axis.
         * 
         * @param rowwise If true, reverse the elements along each row. 
         *     Otherwise, reverse the elements along each column.
         */
        void reverse(bool rowwise);

        /**
         * @brief Sort a matrix in-place along an axis.
         * 
         * @param rowwise If true, sort the elements along each row. Otherwise, 
         *     sort the elements along each column.
         * @param comp Custom comparator. A binary function that accepts two 
         *     elements of type T as arguments, and returns a value convertible 
         *     to bool. The value returned indicates whether the element passed 
         *     as first argument is considered to go before the second.
         * @param stable If true, preserve the relative order of the elements 
         *     with equivalent values. Otherwise, equivalent elements are not 
         *     guaranteed to keep their original relative order.
         */
        void sort(bool rowwise);
        template <class Compare>
        void sort(bool rowwise, Compare comp, bool stable = false);

        /** 
         * @brief Return the standard deviation of the matrix elements. 
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::stddev for full 
         *     documentation.]
         * 
         * @return The standard deviation of the matrix elements.
         */
        T stddev(size_t ddof = 0) const;

        /**
         * @brief Return the standard deviation of the matrix elements along 
         * the specified axis.
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::stddev for full 
         *     documentation.]
         * @param rowwise If true, return the standard deviation of the 
         *     elements along each row. Otherwise, return the standard 
         *     deviation of the elements along each column.
         *
         * @return A light-weight object with the standard deviation along an 
         *     axis. Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_stddev, T, matrix_tag> >
        stddev(size_t ddof, bool rowwise) const;

        /**
         * @brief Return the sum of the matrix elements.
         * 
         * @return The sum of the matrix elements.
         */
        T sum() const;

        /**
         * @brief Return the sum of the matrix elements along the specified 
         * axis.
         * 
         * @param rowwise If true, return the sum of the elements along each 
         *     row. Otherwise, return the sum of the elements along each 
         *     column.
         *
         * @return A light-weight object with the sum along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_sum, T, matrix_tag> >
        sum(bool rowwise) const;

        /**
         * @brief Exchanges the content of the matrix by the content of other. 
         * Implemented to run in constant time.
         *
         * @param other A matrix of the same type.
         *
         * @note All valid iterators, references and views of both *this and 
         *     other keep their validity, and are now referring to the same 
         *     elements they referred to before the call, but in the other 
         *     matrix.
         */
        void swap(base_matrix &other);

        /** 
         * @brief Return a view of the matrix transposed. 
         * 
         * @return If the matrix is const-qualified, the function returns a 
         *     const matrix_view object, which is convertible to a matrix 
         *     object. Otherwise, the function returns a matrix_view object, 
         *     which has reference semantics to the original matrix.
         */
        matrix_view<T> t();
        const matrix_view<T> t() const;

        /** 
         * @brief Return the variance of the matrix elements. 
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::var for full 
         *     documentation.]
         * 
         * @return The variance of the matrix elements.
         */
        T var(size_t ddof = 0) const;

        /**
         * @brief Return the variance of the matrix elements along the 
         * specified axis.
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::var for full 
         *     documentation.]
         * @param rowwise If true, return the variance of the elements along 
         *     each row. Otherwise, return the variance of the elements along 
         *     each column.
         *
         * @return A light-weight object with the variance along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_var, T, matrix_tag> >
        var(size_t ddof, bool rowwise) const;

        /**
         * @brief Return a view of the matrix with the same data.
         *
         * @return A matrix_view object, which has reference semantics to the 
         *     original matrix.
         */
        matrix_view<T> view();

    protected:
        /// Pointer to data.
        T *m_data;

        /// Number of rows and columns in the matrix.
        size_t m_shape1, m_shape2;
    };

    /**
     * Each of this functions performs their respective operation on all the 
     * elements in the matrix.
     *
     * When both the left-hand side and right-hand side arguments are matrix 
     * objects, the operation is performed between the corresponding elements 
     * in each object (the first element of the left one with the right one, 
     * the second with the second, and so on...)
     *
     * When one of the arguments is a value, the operation is applied to all 
     * the elements in the matrix against that value.
     *
     * @param lhs Left-hand side matrix-like object.
     * @param rhs Right-hand side matrix-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object which stores the result of performing the 
     *     operation on each element. Each of these operators uses 
     *     lazy-evaluation, meaning that the result of each operation is 
     *     computed only at the end, when the whole expression is evaluated or 
     *     assigned to a matrix object.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side 
     *     arguments are matrix objects with different number of rows and 
     *     columns.
     */

    /// Unary operators.

    template <class T, class Tag>
    inline base_matrix< T, lazy_unary_tag<__unary_plus, T, Tag> > 
    operator+(const base_matrix<T, Tag> &mat) {
        typedef lazy_unary_tag<__unary_plus, T, Tag> Closure;
        return base_matrix<T, Closure>(__unary_plus(), mat);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_unary_tag<__negate, T, Tag> > 
    operator-(const base_matrix<T, Tag> &mat) {
        typedef lazy_unary_tag<__negate, T, Tag> Closure;
        return base_matrix<T, Closure>(__negate(), mat);
    }    

    template <class T, class Tag>
    inline base_matrix< T, lazy_unary_tag<__bit_not, T, Tag> > 
    operator~(const base_matrix<T, Tag> &mat) {
        typedef lazy_unary_tag<__bit_not, T, Tag> Closure;
        return base_matrix<T, Closure>(__bit_not(), mat);
    }
    
    template <class T, class Tag>
    inline base_matrix< bool, lazy_unary_tag<__logical_not, T, Tag> > 
    operator!(const base_matrix<T, Tag> &mat) {
        typedef lazy_unary_tag<__logical_not, T, Tag> Closure;
        return base_matrix<bool, Closure>(__logical_not(), mat);
    }
    
    /// Arithmetic operators.
    
    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__plus, T, Tag1, T, Tag2> >
    operator+(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__plus, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__plus(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__plus, T, Tag, T, scalar_tag> >
    operator+(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__plus, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__plus(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__plus, T, scalar_tag, T, Tag> >
    operator+(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__plus, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__plus(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__minus, T, Tag1, T, Tag2> >
    operator-(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__minus, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__minus(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__minus, T, Tag, T, scalar_tag> >
    operator-(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__minus, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__minus(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__minus, T, scalar_tag, T, Tag> >
    operator-(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__minus, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__minus(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__multiplies, T, Tag1, T, Tag2> >
    operator*(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__multiplies, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__multiplies(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< T, lazy_binary_tag<__multiplies, T, Tag, T, scalar_tag> >
    operator*(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__multiplies, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__multiplies(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< T, lazy_binary_tag<__multiplies, T, scalar_tag, T, Tag> >
    operator*(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__multiplies, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__multiplies(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__divides, T, Tag1, T, Tag2> >
    operator/(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__divides, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__divides(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__divides, T, Tag, T, scalar_tag> >
    operator/(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__divides, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__divides(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__divides, T, scalar_tag, T, Tag> >
    operator/(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__divides, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__divides(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__modulus, T, Tag1, T, Tag2> >
    operator%(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__modulus, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__modulus(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__modulus, T, Tag, T, scalar_tag> >
    operator%(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__modulus, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__modulus(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__modulus, T, scalar_tag, T, Tag> >
    operator%(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__modulus, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__modulus(), val, rhs);
    }

    /// Bitwise operators.

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__bit_and, T, Tag1, T, Tag2> >
    operator&(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__bit_and, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__bit_and(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__bit_and, T, Tag, T, scalar_tag> >
    operator&(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__bit_and, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__bit_and(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__bit_and, T, scalar_tag, T, Tag> >
    operator&(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__bit_and, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__bit_and(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__bit_or, T, Tag1, T, Tag2> >
    operator|(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__bit_or, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__bit_or(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__bit_or, T, Tag, T, scalar_tag> >
    operator|(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__bit_or, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__bit_or(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__bit_or, T, scalar_tag, T, Tag> >
    operator|(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__bit_or, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__bit_or(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__bit_xor, T, Tag1, T, Tag2> >
    operator^(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__bit_xor, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__bit_xor(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__bit_xor, T, Tag, T, scalar_tag> >
    operator^(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__bit_xor, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__bit_xor(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< T, lazy_binary_tag<__bit_xor, T, scalar_tag, T, Tag> >
    operator^(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__bit_xor, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__bit_xor(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__left_shift, T, Tag1, T, Tag2> >
    operator<<(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__left_shift, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__left_shift(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< T, lazy_binary_tag<__left_shift, T, Tag, T, scalar_tag> >
    operator<<(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__left_shift, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__left_shift(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< T, lazy_binary_tag<__left_shift, T, scalar_tag, T, Tag> >
    operator<<(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__left_shift, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__left_shift(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< T, lazy_binary_tag<__right_shift, T, Tag1, T, Tag2> >
    operator>>(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__right_shift, T, Tag1, T, Tag2> Closure;
        return base_matrix<T, Closure>(__right_shift(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< T, lazy_binary_tag<__right_shift, T, Tag, T, scalar_tag> >
    operator>>(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__right_shift, T, Tag, T, scalar_tag> Closure;
        return base_matrix<T, Closure>(__right_shift(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< T, lazy_binary_tag<__right_shift, T, scalar_tag, T, Tag> >
    operator>>(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__right_shift, T, scalar_tag, T, Tag> Closure;
        return base_matrix<T, Closure>(__right_shift(), val, rhs);
    }

    /// Logical operators.

    template <class T, class Tag1, class Tag2>
    inline base_matrix< bool, lazy_binary_tag<__logical_and, T, Tag1, T, Tag2> >
    operator&&(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__logical_and, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__logical_and(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__logical_and, T, Tag, T, scalar_tag> > 
    operator&&(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__logical_and, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__logical_and(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__logical_and, T, scalar_tag, T, Tag> > 
    operator&&(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__logical_and, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__logical_and(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< bool, lazy_binary_tag<__logical_or, T, Tag1, T, Tag2> >
    operator||(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__logical_or, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__logical_or(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__logical_or, T, Tag, T, scalar_tag> > 
    operator||(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__logical_or, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__logical_or(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__logical_or, T, scalar_tag, T, Tag> > 
    operator||(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__logical_or, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__logical_or(), val, rhs);
    }

    /// Relational operators.

    template <class T, class Tag1, class Tag2>
    inline base_matrix< bool, lazy_binary_tag<__equal_to, T, Tag1, T, Tag2> >
    operator==(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__equal_to, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__equal_to(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__equal_to, T, Tag, T, scalar_tag> > 
    operator==(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__equal_to, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__equal_to(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__equal_to, T, scalar_tag, T, Tag> > 
    operator==(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__equal_to, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__equal_to(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline 
    base_matrix< bool, lazy_binary_tag<__not_equal_to, T, Tag1, T, Tag2> >
    operator!=(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__not_equal_to, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__not_equal_to(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__not_equal_to, T, Tag, T, scalar_tag> > 
    operator!=(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__not_equal_to, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__not_equal_to(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__not_equal_to, T, scalar_tag, T, Tag> > 
    operator!=(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__not_equal_to, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__not_equal_to(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< bool, lazy_binary_tag<__less, T, Tag1, T, Tag2> >
    operator<(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__less, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__less(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_matrix< bool, lazy_binary_tag<__less, T, Tag, T, scalar_tag> > 
    operator<(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__less, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__less(), lhs, val);
    }
    
    template <class T, class Tag>
    inline base_matrix< bool, lazy_binary_tag<__less, T, scalar_tag, T, Tag> > 
    operator<(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__less, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__less(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_matrix< bool, lazy_binary_tag<__greater, T, Tag1, T, Tag2> >
    operator>(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__greater, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__greater(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__greater, T, Tag, T, scalar_tag> > 
    operator>(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__greater, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__greater(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__greater, T, scalar_tag, T, Tag> > 
    operator>(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__greater, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__greater(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline 
    base_matrix< bool, lazy_binary_tag<__less_equal, T, Tag1, T, Tag2> >
    operator<=(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__less_equal, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__less_equal(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__less_equal, T, Tag, T, scalar_tag> > 
    operator<=(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__less_equal, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__less_equal(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__less_equal, T, scalar_tag, T, Tag> > 
    operator<=(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__less_equal, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__less_equal(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline 
    base_matrix< bool, lazy_binary_tag<__greater_equal, T, Tag1, T, Tag2> >
    operator>=(
        const base_matrix<T, Tag1> &lhs, const base_matrix<T, Tag2> &rhs
    ) {
        typedef lazy_binary_tag<__greater_equal, T, Tag1, T, Tag2> Closure;
        return base_matrix<bool, Closure>(__greater_equal(), lhs, rhs);
    }

    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__greater_equal, T, Tag, T, scalar_tag> > 
    operator>=(
        const base_matrix<T, Tag> &lhs, 
        const typename base_matrix<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__greater_equal, T, Tag, T, scalar_tag> Closure;
        return base_matrix<bool, Closure>(__greater_equal(), lhs, val);
    }
    
    template <class T, class Tag>
    inline 
    base_matrix< bool, lazy_binary_tag<__greater_equal, T, scalar_tag, T, Tag> > 
    operator>=(
        const typename base_matrix<T, Tag>::value_type &val, 
        const base_matrix<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__greater_equal, T, scalar_tag, T, Tag> Closure;
        return base_matrix<bool, Closure>(__greater_equal(), val, rhs);
    }
}

#include "numcpp/matrix/matrix.tcc"

#endif // NUMCPP_MATRIX_H_INCLUDED
