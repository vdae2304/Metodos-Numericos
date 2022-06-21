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

 /** @file include/numcpp/matrix/matrix_view.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/matrix.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_MATRIX_VIEW_H_INCLUDED
#define NUMCPP_MATRIX_VIEW_H_INCLUDED

namespace numcpp {
    /**
     * @brief A matrix_view is just a view of a matrix object. It references 
     * the elements in the original matrix. The view itself does not own the 
     * data and any changes made to the view will affect the original matrix, 
     * and any changes made to the original matrix will affect the view. 
     *
     * @tparam T Type of the elements contained in the matrix_view. This 
     *     shall be an arithmetic type or a class that behaves like one (such 
     *     as std::complex).
     */
    template <class T>
    class base_matrix<T, matrix_view_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef base_matrix_iterator<T, matrix_view_tag> iterator;
        typedef base_matrix_const_iterator<T, matrix_view_tag> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Default constructor. Constructs a matrix_view that does not 
         * reference any object.
         */
        base_matrix();

        /**
         * @brief Matrix constructor. Constructs a matrix_view that references 
         * a matrix.
         *
         * @param m The number of rows in the matrix_view. 
         * @param n The number of columns in the matrix_view. 
         * @param data The pointer to the memory array used by the matrix_view.
         * @param row_major If true (default), the elements are stored in 
         *     row-major order. Otherwise, the elements are stored in 
         *     column-major order.
         */
        base_matrix(size_t m, size_t n, T *data, bool row_major = true);

        /**
         * @brief Slice matrix constructor. Constructs a matrix_view that 
         * references a subset of matrix elements.
         *
         * @param m The number of rows in the matrix_view. 
         * @param n The number of columns in the matrix_view.
         * @param data The pointer to the memory array used by the matrix_view.
         * @param tda Trailing dimmension. For row-major order, it is the 
         *     number of columns as laid out in memory. For column-major order, 
         *     it is the number of rows as laid out in memory.
         * @param offset1 Index of the first row selected into the matrix_view.
         * @param stride1 Span that separates the rows selected into the 
         *     matrix_view.
         * @param offset2 Index of the first column selected into the 
         *     matrix_view.
         * @param stride2 Span that separates the columns selected into the 
         *     matrix_view.
         * @param row_major If true (default), the elements are stored in 
         *     row-major order. Otherwise, the elements are stored in 
         *     column-major order.
         */
        base_matrix(
            size_t m, size_t n, T *data, size_t tda,
            size_t offset1, size_t stride1, 
            size_t offset2, size_t stride2, 
            bool row_major = true
        );

        /** 
         * @brief Copy constructor. Constructs a matrix_view as a copy of 
         * other.
         *
         * @param other A matrix_view of the same type.
         */
        base_matrix(const base_matrix &other);

        /** 
         * @brief Move constructor. Constructs a matrix_view with the contents 
         * of other.
         * 
         * @param other A matrix_view of the same type. other is left in an 
         *     empty state.
         */
        base_matrix(base_matrix &&other);

        /// Destructor.
        ~base_matrix();

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * matrix_view.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the beginning of the 
         *     matrix_view. If the matrix_view is const-qualified, the function 
         *     returns a const_iterator. Otherwise, it returns an iterator.
         */
        iterator begin();
        const_iterator begin() const;

        iterator begin(bool row_major);
        const_iterator begin(bool row_major) const;

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the matrix_view. It does not point to any element, and thus shall 
         * not be dereferenced.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the 
         *     matrix_view. If the matrix_view is const-qualified, the function 
         *     returns a const_iterator. Otherwise, it returns an iterator.
         */
        iterator end();
        const_iterator end() const;

        iterator end(bool row_major);
        const_iterator end(bool row_major) const;

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the matrix_view (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the matrix_view. If the matrix_view is const-qualified, the 
         *     function returns a const_reverse_iterator. Otherwise, it returns 
         *     a reverse_iterator.
         */
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;

        reverse_iterator rbegin(bool row_major);
        const_reverse_iterator rbegin(bool row_major) const;

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the matrix_view (i.e., its reverse end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     matrix_view. If the matrix_view is const-qualified, the function 
         *     returns a const_reverse_iterator. Otherwise, it returns a 
         *     reverse_iterator.
         */
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        
        reverse_iterator rend(bool row_major);
        const_reverse_iterator rend(bool row_major) const;

        /**
         * @brief Returns a const_iterator pointing to the first element in the 
         * matrix_view.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_iterator to the beginning of the matrix_view.
         */
        const_iterator cbegin() const;
        const_iterator cbegin(bool row_major) const;

        /**
         * @brief Returns a const_iterator pointing to the past-the-end element 
         * in the matrix_view.
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_iterator to the element past the end of the 
         *     matrix_view.
         */
        const_iterator cend() const;
        const_iterator cend(bool row_major) const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the last element 
         * in the matrix_view (i.e., its reverse beginning).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_reverse_iterator to the reverse beginning of the 
         *     matrix_view.
         */
        const_reverse_iterator crbegin() const;
        const_reverse_iterator crbegin(bool row_major) const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the element 
         * preceding the first element in the matrix_view (i.e., its reverse 
         * end).
         * 
         * @param row_major It is an optional parameter that changes the order 
         *     in which elements are iterated. If provided, the elements are 
         *     iterated in row-major or column-major order as specified by 
         *     row_major. Otherwise, the elements are iterated in the same 
         *     order as stored in memory.
         *
         * @return A const_reverse_iterator to the reverse end of the 
         *     matrix_view.
         */
        const_reverse_iterator crend() const;
        const_reverse_iterator crend(bool row_major) const;

        /// Matrix indexing.

        /** 
         * @brief Call operator. Returns a reference to the element at row i 
         * and column j in the matrix_view. The element at row i and column j 
         * corresponds to the element at row offset1 + i*stride1 and column 
         * offset2 + j*stride2 in the original matrix.
         * 
         * @param i Row position of an element in the matrix_view. Must be 
         *     between 0 and rows() - 1.
         * @param j Column position of an element in the matrix_view. Must be 
         *     between 0 and cols() - 1.
         *
         * @return The element at the specified row and column in the 
         *     matrix_view. If the matrix_view is const-qualified, the function 
         *     returns a reference to const T. Otherwise, it returns a 
         *     reference to T.
         *
         * @throw std::out_of_range Thrown if either i or j is out of bounds.
         */
        T& operator()(size_t i, size_t j);
        const T& operator()(size_t i, size_t j) const;

        /** 
         * @brief Returns the number of rows in the matrix_view. 
         * 
         * @return The number of rows in the matrix_view.
         */
        size_t rows() const;

        /** 
         * @brief Returns the number of columns in the matrix_view.
         * 
         * @return The number of columns in the matrix_view.
         */
        size_t cols() const;

        /**
         * @brief Returns the number of elements in the matrix_view, i.e., 
         * rows()*cols().
         *
         * @return The number of elements in the matrix_view.
         */
        size_t size() const;

        /** 
         * @brief Returns whether the matrix_view is empty (i.e., whether its 
         * size is 0).
         *
         * @return true if the matrix_view size is 0, false otherwise.
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the 
         * matrix_view.
         *
         * @return A pointer to the array used internally by the matrix_view. 
         *     If the matrix_view is const-qualified, the function returns a 
         *     pointer to const T. Otherwise, it returns a pointer to T.
         */
        T* data();
        const T* data() const;

        /**
         * @brief Returns whether the elements are stored in row-major order.
         * 
         * @return true if the elements are stored in row-major order, 
         *     false otherwise.
         */
        bool rowmajor() const;

        /**
         * @brief Returns whether the elements are stored in column-major 
         * order.
         * 
         * @return true if the elements are stored in column-major order, 
         *     false otherwise.
         */
        bool colmajor() const;

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns to each element the value of the 
         * corresponding element in other.
         *
         * @param other A matrix-like object of the same type and size.
         * 
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the number of rows and 
         *     columns do not match.
         */
        base_matrix& operator=(const base_matrix &other);
        template <class Tag>
        base_matrix& operator=(const base_matrix<T, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element.
         *
         * @param val Value assigned to all the elements in the matrix_view.
         * 
         * @return *this
         */
        base_matrix& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving 
         * other in an empty state.
         *
         * @param other A matrix_view of the same type.
         * 
         * @return *this
         */
        base_matrix& operator=(base_matrix &&other);

        /// Compound assignment operator.

        /**
         * @brief Assigns to *this the result of performing the respective 
         * operation on all the elements in the matrix_view.
         *
         * When the right-hand side argument is a matrix object, the operation 
         * is performed between the corresponding elements in each object (the 
         * first element of the left one with the right one, the second with 
         * the second, and so on...)
         *
         * When the right-hand side argument is a value, the operation is 
         * applied to all the elements in the matrix_view against that value.
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
         * matrix_view.
         * 
         * @param i Reference to an index where the row location will be 
         *     stored.
         * @param j Reference to an index where the column location will be 
         *     stored.
         *
         * @return The indices of the maximum value in the matrix_view.
         */
        void argmax(size_t &i, size_t &j) const;

        /** 
         * @brief Return the index of the maximum value in the matrix_view 
         * along the specified axis. 
         * 
         * @param rowwise If true, return the index of the maximum value along 
         *     each row. Otherwise, return the index of the maximum value along 
         *     each column.
         * 
         * @return A light-weight object with the index of the maximum value 
         *     along an axis. Convertible to an array object.
         */
        base_array< size_t, lazy_axis_tag<__range_argmax, T, matrix_view_tag> >
        argmax(bool rowwise) const;

        /**
         * @brief Return the pair of indices of the minimum value in the 
         * matrix_view.
         * 
         * @param i Reference to an index where the row location will be 
         *     stored.
         * @param j Reference to an index where the column location will be 
         *     stored.
         *
         * @return The indices of the minimum value in the matrix_view.
         */
        void argmin(size_t &i, size_t &j) const;

        /** 
         * @brief Return the index of the minimum value in the matrix_view 
         * along the specified axis. 
         * 
         * @param rowwise If true, return the index of the minimum value along 
         *     each row. Otherwise, return the index of the minimum value along 
         *     each column.
         * 
         * @return A light-weight object with the index of the minimum value 
         *     along an axis. Convertible to an array object.
         */
        base_array< size_t, lazy_axis_tag<__range_argmin, T, matrix_view_tag> >
        argmin(bool rowwise) const;

        /**
         * @brief Cast each element to a specified type.
         * 
         * @return A light-weight object with the elements in the matrix_view 
         *     casted to the specified type. Convertible to a matrix object.
         */
        template <class U>
        base_matrix< U, lazy_unary_tag<__identity, T, matrix_view_tag> > 
        astype() const;

        /**
         * @brief Clamp the values in the matrix_view. Given an interval 
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
         *     element in the matrix_view. Convertible to a matrix object.
         */
        base_matrix< T, lazy_unary_tag<__conjugate, T, matrix_view_tag> > 
        conj() const;

        /**
         * @brief Return the imaginary part, element-wise.
         * 
         * @return A light-weight object with the imaginary part of each 
         *     element in the matrix_view. Convertible to a matrix object.
         */
        base_matrix< 
            typename complex_traits<T>::value_type, 
            lazy_unary_tag<__imag_part, T, matrix_view_tag> 
        > 
        imag() const;

        /**
         * @brief Return the maximum value contained in the matrix_view.
         *
         * @return The maximum value in the matrix_view.
         */
        T max() const;

        /** 
         * @brief Return the maximum value contained in the matrix_view along 
         * the specified axis. 
         * 
         * @param rowwise If true, return the maximum value along each row. 
         *     Otherwise, return the maximum value along each column.
         * 
         * @return A light-weight object with the maximum value along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_max, T, matrix_view_tag> >
        max(bool rowwise) const;

        /** 
         * @brief Return the average of the matrix_view elements.
         * 
         * @return The average of the matrix_view elements.
         */
        T mean() const;

        /**
         * @brief Return the average of the matrix_view elements along the 
         * specified axis.
         * 
         * @param rowwise If true, return the average of the elements along 
         *     each row. Otherwise, return the average of the elements along 
         *     each column.
         *
         * @return A light-weight object with the average along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_mean, T, matrix_view_tag> >
        mean(bool rowwise) const;

        /**
         * @brief Return the minimum value contained in the matrix_view.
         *
         * @return The minimum value in the matrix_view.
         */
        T min() const;

        /** 
         * @brief Return the minimum value contained in the matrix_view along 
         * the specified axis. 
         * 
         * @param rowwise If true, return the minimum value along each row. 
         *     Otherwise, return the minimum value along each column.
         * 
         * @return A light-weight object with the minimum value along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_min, T, matrix_view_tag> > 
        min(bool rowwise) const;

        /**
         * @brief Return the product of the matrix_view elements.
         * 
         * @return The product of the matrix_view elements.
         */
        T prod() const;

        /**
         * @brief Return the product of the matrix_view elements along the 
         * specified axis.
         * 
         * @param rowwise If true, return the product of the elements along 
         *     each row. Otherwise, return the product of the elements along 
         *     each column.
         *
         * @return A light-weight object with the product along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_prod, T, matrix_view_tag> >
        prod(bool rowwise) const;

        /**
         * @brief Return the real part, element-wise.
         * 
         * @return A light-weight object with the real part of each element in 
         *     the matrix_view. Convertible to a matrix object.
         */
        base_matrix< 
            typename complex_traits<T>::value_type, 
            lazy_unary_tag<__real_part, T, matrix_view_tag> 
        > 
        real() const;

        /** 
         * @brief Return the standard deviation of the matrix_view elements. 
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::stddev for full 
         *     documentation.]
         * 
         * @return The standard deviation of the matrix_view elements.
         */
        T stddev(size_t ddof = 0) const;

        /**
         * @brief Return the standard deviation of the matrix_view elements 
         * along the specified axis.
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
        base_array< T, lazy_axis_tag<__range_stddev, T, matrix_view_tag> >
        stddev(size_t ddof, bool rowwise) const;

        /**
         * @brief Return the sum of the matrix_view elements.
         * 
         * @return The sum of the matrix_view elements.
         */
        T sum() const;

        /**
         * @brief Return the sum of the matrix_view elements along the 
         * specified axis.
         * 
         * @param rowwise If true, return the sum of the elements along each 
         *     row. Otherwise, return the sum of the elements along each 
         *     column.
         *
         * @return A light-weight object with the sum along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_sum, T, matrix_view_tag> >
        sum(bool rowwise) const;

        /** 
         * @brief Return the variance of the matrix_view elements. 
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::var for full 
         *     documentation.]
         * 
         * @return The variance of the matrix_view elements.
         */
        T var(size_t ddof = 0) const;

        /**
         * @brief Return the variance of the matrix_view elements along the 
         * specified axis.
         * 
         * @param ddof Delta degrees of freedom. [See numcpp::var for full 
         * documentation.]
         * @param rowwise If true, return the variance of the elements along 
         *     each row. Otherwise, return the variance of the elements along 
         *     each column.
         *
         * @return A light-weight object with the variance along an axis. 
         *     Convertible to an array object.
         */
        base_array< T, lazy_axis_tag<__range_var, T, matrix_view_tag> >
        var(size_t ddof, bool rowwise) const;

    protected:
        /// Pointer to data.
        T *m_data;

        /// Number of rows and columns in the matrix_view.
        size_t m_shape1, m_shape2;

        /// Trailing dimmension. For row-major order, it is the size of a row 
        /// as laid out in memory. For column-major order, it is the size of a 
        /// column as laid out in memory.
        size_t m_tda;

        /// Offset and stride between rows in the matrix_view with respect to 
        /// the original matrix.
        size_t m_offset1, m_stride1;

        /// Offset and stride between columns in the matrix_view with respect 
        /// to the original matrix.
        size_t m_offset2, m_stride2;

        /// Whether the elements are stored in row-major or column-major order.
        bool m_order;
    };
}

#include "numcpp/matrix/matrix_view.tcc"

#endif // NUMCPP_MATRIX_VIEW_H_INCLUDED
