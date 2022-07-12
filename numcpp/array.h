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

 /** @file include/numcpp/array.h
  *  This header defines array and array_view classes.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_H_INCLUDED
#define NUMCPP_ARRAY_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/slice.h"
#include "numcpp/routines/algo.h"
#include "numcpp/math/mathfwd.h"

#include "numcpp/array/array_iterator.h"
#include "numcpp/array/lazy_array.h"
#include "numcpp/array/array_view.h"
#include "numcpp/array/index_view.h"

#include <initializer_list>

namespace numcpp {
    /**
     * @brief Arrays are contiguous sequence containers: they hold a specific
     * number of elements ordered in a strict linear sequence. Unlike an
     * array_view, an array is always owner of its own data.
     *
     * Arrays are designed to easily perform mathematical operations on them.
     * Most mathematical operations can be applied directly to array objects,
     * affecting all its elements.
     *
     * @tparam T Type of the elements contained in the array. This shall be an
     *     arithmetic type or a class that behaves like one (such as
     *     std::complex).
     */
    template <class T>
    class base_array<T, array_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef base_array_iterator<T, array_tag> iterator;
        typedef base_array_const_iterator<T, array_tag> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Default constructor. Constructs an empty array with no
         * elements.
         */
        base_array();

        /**
         * @brief Size constructor. Constructs an array with n elements, each
         * element is left uninitialized.
         *
         * @param n Length of array.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_array(size_t n);

        /**
         * @brief Fill constructor. Constructs an array with n elements, each
         * element initialized to val.
         *
         * @param n Length of array.
         * @param val Value to which each of the elements is initialized.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_array(size_t n, const T &val);

        /**
         * @brief Range constructor. Constructs an array with as many elements
         * as the range [first, last), with each element constructed from its
         * corresponding element in that range, in the same order.
         *
         * @param first Input iterator to the initial position in a range.
         * @param last Input iterator to the final position in a range. The
         *     range used is [first, last), which includes all the elements
         *     between first and last, including the element pointed by first
         *     but not the element pointed by last.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <
            class InputIterator,
            typename = RequiresInputIterator<InputIterator>
        >
        base_array(InputIterator first, InputIterator last);

        /**
         * @brief Copy constructor. Constructs an array with a copy of each of
         * the elements in other, in the same order.
         *
         * @param other An array-like object of the same type.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_array(const base_array &other);
        template <class Tag>
        base_array(const base_array<T, Tag> &other);

        /**
         * @brief Move constructor. Constructs an array that acquires the
         * elements of other.
         *
         * @param other An array of the same type. The ownership is directly
         *     transferred from other. other is left in an empty state.
         */
        base_array(base_array &&other);

        /**
         * @brief Initializer list constructor. Constructs an array with a copy
         * of each of the elements in il, in the same order.
         *
         * @param il An initializer_list object.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        base_array(std::initializer_list<T> il);

        /// Destructor.
        ~base_array();

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * array.
         *
         * @return A random access iterator to the beginning of the array. If
         *     the array is const-qualified, the function returns a
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator begin();
        const_iterator begin() const;

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the array. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @return A random access iterator to the element past the end of the
         *     array. If the array is const-qualified, the function returns a
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator end();
        const_iterator end() const;

        /**
         * @brief Returns a reverse iterator pointing to the last element in
         * the array (i.e., its reverse beginning). Reverse iterators iterate
         * backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of
         *     the array. If the array is const-qualified, the function returns
         *     a const_reverse_iterator. Otherwise, it returns a
         *     reverse_iterator.
         */
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;

        /**
         * @brief Returns a reverse iterator pointing to the element preceding
         * the first element in the array (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the
         *     array. If the array is const-qualified, the function returns a
         *     const_reverse_iterator. Otherwise, it returns a reverse_iterator.
         */
        reverse_iterator rend();
        const_reverse_iterator rend() const;

        /**
         * @brief Returns a const_iterator pointing to the first element in the
         * array.
         *
         * @return A const_iterator to the beginning of the array.
         */
        const_iterator cbegin() const;

        /**
         * @brief Returns a const_iterator pointing to the past-the-end element
         * in the array.
         *
         * @return A const_iterator to the element past the end of the array.
         */
        const_iterator cend() const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the last element
         * in the array (i.e., its reverse beginning).
         *
         * @return A const_reverse_iterator to the reverse beginning of the
         *     array.
         */
        const_reverse_iterator crbegin() const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the element
         * preceding the first element in the array (i.e., its reverse end).
         *
         * @return A const_reverse_iterator to the reverse end of the array.
         */
        const_reverse_iterator crend() const;

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns a reference to the element at
         * position i in the array.
         *
         * @param i Position of an element in the array. Must be between 0 and
         *     size() - 1.
         *
         * @return The element at the specified position in the array. If the
         *     array is const-qualified, the function returns a reference to
         *     const T. Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        T& operator[](size_t i);
        const T& operator[](size_t i) const;

        /**
         * @brief Slice indexing: Returns an array_view object that selects
         * the elements specified by the slice.
         *
         * @param slc A slice object specifying which elements of the array are
         *     selected.
         *
         * @return If the array is const-qualified, the function returns a
         *     const array_view object, which is convertible to an array
         *     object. Otherwise, the function returns an array_view object,
         *     which has reference semantics to the original array.
         *
         * @throw std::out_of_range Thrown if the slice references to an
         *     element out of bounds.
         */
        array_view<T> operator[](slice slc);
        const array_view<T> operator[](slice slc) const;

        /**
         * @brief Integer array indexing: Returns an index_view object that
         * selects the elements specified by the array of indices.
         *
         * @param index An array<size_t> object with its elements identifying
         *     which elements of the array are selected.
         *
         * @return If the array is const-qualified, the function returns a new
         *     array object with a copy of the selection. Otherwise, the
         *     function returns an index_view object, which has reference
         *     semantics to the original array.
         *
         * @throw std::out_of_range Thrown if the array of indices references
         *     to an element out of bounds.
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         */
        template <class Tag>
        index_view<T> operator[](const base_array<size_t, Tag> &index);

        template <class Tag>
        base_array operator[](const base_array<size_t, Tag> &index) const;

        /**
         * @brief Boolean array indexing: Returns an index_view object that
         * selects the elements specified by the boolean mask.
         *
         * @param mask An array<bool> object with its elements identifying
         *     whether each element of the array is selected or not.
         *
         * @return If the array is const-qualified, the function returns a new
         *     array object with a copy of the selection. Otherwise, the
         *     function returns an index_view object, which has reference
         *     semantics to the original array.
         *
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         */
        template <class Tag>
        index_view<T> operator[](const base_array<bool, Tag> &mask);

        template <class Tag>
        base_array operator[](const base_array<bool, Tag> &mask) const;

        /**
         * @brief Returns the number of elements in the array.
         *
         * @return The number of elements in the array.
         */
        size_t size() const;

        /**
         * @brief Resizes the array so that it contains n elements. After
         * resizing, the previous contents are lost.
         *
         * @param n New size of the array.
         *
         * @warning Invalidates all iterators, references and views to elements
         *     of the array.
         */
        void resize(size_t n);

        /**
         * @brief Returns whether the array is empty (i.e., whether its size is
         * 0).
         *
         * @return true if the array size is 0, false otherwise.
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the
         * array. Because elements in the array are stored contiguously, the
         * pointer retrieved can be offset to access any element in the array.
         *
         * @return A pointer to the array used internally by the array. If the
         *     array is const-qualified, the function returns a pointer to
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
         * @param other An array-like object of the same type.
         *
         * @return *this
         *
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         *
         * @warning When the sizes do not mach, invalidates all iterators,
         *     references and views to elements of the array. Otherwise,
         *     valid iterators, references and views keep their validity.
         */
        base_array& operator=(const base_array &other);
        template <class Tag>
        base_array& operator=(const base_array<T, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element. The size of
         * the array is preserved.
         *
         * @param val Value assigned to all the elements in the array.
         *
         * @return *this
         */
        base_array& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving
         * other in an empty state.
         *
         * @param other An array of the same type.
         *
         * @return *this
         *
         * @warning All iterators, references and views to elements of the
         *     array are invalidated.
         */
        base_array& operator=(base_array &&other);

        /**
         * @brief Initializer list assignment. Assigns to each element the
         * value of the corresponding element in il after resizing the object
         * (if necessary).
         *
         * @param il An initializer_list object.
         *
         * @return *this
         *
         * @throw std::bad_alloc If the function needs to allocate storage and
         *     fails, it may throw an exception.
         *
         * @warning When the sizes do not mach, invalidates all iterators,
         *     references and views to elements of the array. Otherwise,
         *     valid iterators, references and views keep their validity.
         */
        base_array& operator=(std::initializer_list<T> il);

        /// Compound assignment operator.

        /**
         * @brief Assigns to *this the result of performing the respective
         * operation on all the elements in the array.
         *
         * When the right-hand side argument is an array object, the operation
         * is performed between the corresponding elements in each object (the
         * first element of the left one with the right one, the second with
         * the second, and so on...)
         *
         * When the right-hand side argument is a value, the operation is
         * applied to all the elements in the array against that value.
         *
         * @param rhs Right-hand side array-like object.
         * @param val Value to use as right-hand operand.
         *
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the right-hand side argument
         *     is an array object with different size to *this.
         */
        base_array& operator+=(const base_array &rhs);
        base_array& operator-=(const base_array &rhs);
        base_array& operator*=(const base_array &rhs);
        base_array& operator/=(const base_array &rhs);
        base_array& operator%=(const base_array &rhs);
        base_array& operator&=(const base_array &rhs);
        base_array& operator|=(const base_array &rhs);
        base_array& operator^=(const base_array &rhs);
        base_array& operator<<=(const base_array &rhs);
        base_array& operator>>=(const base_array &rhs);

        template <class Tag>
        base_array& operator+=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator-=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator*=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator/=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator%=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator&=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator|=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator^=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator<<=(const base_array<T, Tag> &rhs);
        template <class Tag>
        base_array& operator>>=(const base_array<T, Tag> &rhs);

        base_array& operator+=(const T &val);
        base_array& operator-=(const T &val);
        base_array& operator*=(const T &val);
        base_array& operator/=(const T &val);
        base_array& operator%=(const T &val);
        base_array& operator&=(const T &val);
        base_array& operator|=(const T &val);
        base_array& operator^=(const T &val);
        base_array& operator<<=(const T &val);
        base_array& operator>>=(const T &val);

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
         * @brief Return the index of the maximum value in the array.
         *
         * @return The index of the maximum value in the array.
         */
        size_t argmax() const;

        /**
         * @brief Return the index of the minimum value in the array.
         *
         * @return The index of the minimum value in the array.
         */
        size_t argmin() const;

        /**
         * @brief Return the indices that would partition the array.
         *
         * @param kth Element index to partition by. The element at the kth
         *     position is the element that would be in that position in the
         *     sorted array. The other elements are left without any specific
         *     order, except that none of the elements preceding kth are
         *     greater than it, and none of the elements following it are less.
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         *
         * @return An array of indices that partitions the array. If a is an
         *     array, then a[a.argpartition()] yields a partitioned a.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        array<size_t> argpartition(size_t kth) const;
        template <class Compare>
        array<size_t> argpartition(size_t kth, Compare comp) const;

        /**
         * @brief Return the indices that would sort the array.
         *
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         * @param stable If true, preserve the relative order of the elements
         *     with equivalent values. Otherwise, equivalent elements are not
         *     guaranteed to keep their original relative order.
         *
         * @return An array of indices that sort the array. If a is an array,
         *     then a[a.argsort()] yields a sorted a.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        array<size_t> argsort() const;
        template <class Compare>
        array<size_t> argsort(Compare comp, bool stable = false) const;

        /**
         * @brief Cast each element to a specified type.
         *
         * @return A light-weight object with the elements in the array casted
         *     to the specified type. Convertible to an array object.
         */
        template <class U>
        base_array< U, lazy_unary_tag<__identity, T, array_tag> >
        astype() const;

        /**
         * @brief Clamp the values in the array. Given an interval
         * [a_min, a_max], values smaller than a_min become a_min, and values
         * larger than a_max become a_max.
         *
         * If T is a complex type, then real and imaginary parts are clamped
         * separately.
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
         *     element in the array. Convertible to an array object.
         */
        base_array< T, lazy_unary_tag<__math_conj, T, array_tag> >
        conj() const;

        /**
         * @brief Return the inner product of two arrays (without complex
         * conjugation).
         *
         * @param rhs Right-hand side array-like object.
         *
         * @return The inner product with rhs.
         *
         * @throw std::invalid_argument Thrown if the sizes do not match.
         */
        template <class Tag>
        T dot(const base_array<T, Tag> &rhs) const;

        /**
         * @brief Return the matrix multiplication of a row-vector and a
         * matrix.
         *
         * @param rhs Right-hand side matrix-like object.
         *
         * @return The vector-matrix multiplication with rhs.
         *
         * @throw std::invalid_argument Thrown if the size of *this and the
         *     number of rows of rhs do not match.
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        template <class Tag>
        base_array dot(const base_matrix<T, Tag> &rhs) const;

        /**
         * @brief Return or set the imaginary part, element-wise.
         *
         * @param arr An array-like object with the values to set the imaginary
         *     part to.
         *
         * @return A light-weight object with the imaginary part of each
         *     element in the array. Convertible to an array object.
         */
        base_array<
            typename complex_traits<T>::value_type,
            lazy_unary_tag<__math_imag, T, array_tag>
        >
        imag() const;

        template <class Tag>
        void imag(
            const base_array<typename complex_traits<T>::value_type, Tag> &arr
        );

        /**
         * @brief Return the maximum value contained in the array.
         *
         * @return The maximum value in the array.
         */
        T max() const;

        /**
         * @brief Return the average of the array elements.
         *
         * @return The average of the array elements.
         */
        T mean() const;

        /**
         * @brief Return the minimum value contained in the array.
         *
         * @return The minimum value in the array.
         */
        T min() const;

        /**
         * @brief Partition an array in-place.
         *
         * @param kth Element index to partition by. The element at the kth
         *     position is the element that would be in that position in the
         *     sorted array. The other elements are left without any specific
         *     order, except that none of the elements preceding kth are
         *     greater than it, and none of the elements following it are less.
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         */
        void partition(size_t kth);
        template <class Compare>
        void partition(size_t kth, Compare comp);

        /**
         * @brief Return the product of the array elements.
         *
         * @return The product of the array elements.
         */
        T prod() const;

        /**
         * @brief Return or set the real part, element-wise.
         *
         * @param arr An array-like object with the values to set the real part
         *     to.
         *
         * @return A light-weight object with the real part of each element in
         *     the array. Convertible to an array object.
         */
        base_array<
            typename complex_traits<T>::value_type,
            lazy_unary_tag<__math_real, T, array_tag>
        >
        real() const;

        template <class Tag>
        void real(
            const base_array<typename complex_traits<T>::value_type, Tag> &arr
        );

        /**
         * @brief Reverse the order of the elements in-place.
         */
        void reverse();

        /**
         * @brief Sort an array in-place.
         *
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         * @param stable If true, preserve the relative order of the elements
         *     with equivalent values. Otherwise, equivalent elements are not
         *     guaranteed to keep their original relative order.
         */
        void sort();
        template <class Compare>
        void sort(Compare comp, bool stable = false);

        /**
         * @brief Return the standard deviation of the array elements.
         *
         * @param ddof Delta degrees of freedom. [See numcpp::stddev for full
         * documentation.]
         *
         * @return The standard deviation of the array elements.
         */
        T stddev(size_t ddof = 0) const;

        /**
         * @brief Return the sum of the array elements.
         *
         * @return The sum of the array elements.
         */
        T sum() const;

        /**
         * @brief Exchanges the content of the array by the content of other.
         * Implemented to run in constant time.
         *
         * @param other An array of the same type.
         *
         * @note All valid iterators, references and views of both *this and
         *     other keep their validity, and are now referring to the same
         *     elements they referred to before the call, but in the other
         *     array.
         */
        void swap(base_array &other);

        /**
         * @brief Return the variance of the array elements.
         *
         * @param ddof Delta degrees of freedom. [See numcpp::var for full
         * documentation.]
         *
         * @return The variance of the array elements.
         */
        T var(size_t ddof = 0) const;

        /**
         * @brief Return a view of the array with the same data.
         *
         * @returns An array_view object, which has reference semantics to the
         *     original array.
         */
        array_view<T> view();

    protected:
        /// Pointer to data.
        T *m_data;

        /// Number of elements in the array.
        size_t m_size;
    };

    /**
     * @brief Each of this functions performs their respective operation on all
     * the elements in the array.
     *
     * When both the left-hand side and right-hand side arguments are array
     * objects, the operation is performed between the corresponding elements
     * in each object (the first element of the left one with the right one,
     * the second with the second, and so on...)
     *
     * When one of the arguments is a value, the operation is applied to all
     * the elements in the array against that value.
     *
     * @param lhs Left-hand side array-like object.
     * @param rhs Right-hand side array-like object.
     * @param val Value to use either as left-hand or right-hand operand.
     *
     * @return A light-weight object which stores the result of performing the
     *     operation on each element. Each of these operators uses
     *     lazy-evaluation, meaning that the result of each operation is
     *     computed only at the end, when the whole expression is evaluated or
     *     assigned to an array object.
     *
     * @throw std::invalid_argument Thrown if the left-hand and right-hand side
     *     arguments are array objects with different sizes.
     */

    /// Unary operators.

    template <class T, class Tag>
    inline base_array< T, lazy_unary_tag<__unary_plus, T, Tag> >
    operator+(const base_array<T, Tag> &arr) {
        typedef lazy_unary_tag<__unary_plus, T, Tag> Closure;
        return base_array<T, Closure>(__unary_plus(), arr);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_unary_tag<__negate, T, Tag> >
    operator-(const base_array<T, Tag> &arr) {
        typedef lazy_unary_tag<__negate, T, Tag> Closure;
        return base_array<T, Closure>(__negate(), arr);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_unary_tag<__bit_not, T, Tag> >
    operator~(const base_array<T, Tag> &arr) {
        typedef lazy_unary_tag<__bit_not, T, Tag> Closure;
        return base_array<T, Closure>(__bit_not(), arr);
    }

    template <class T, class Tag>
    inline base_array< bool, lazy_unary_tag<__logical_not, T, Tag> >
    operator!(const base_array<T, Tag> &arr) {
        typedef lazy_unary_tag<__logical_not, T, Tag> Closure;
        return base_array<bool, Closure>(__logical_not(), arr);
    }

    /// Arithmetic operators.

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__plus, T, Tag1, T, Tag2> >
    operator+(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__plus, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__plus(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__plus, T, Tag, T, scalar_tag> >
    operator+(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__plus, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__plus(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__plus, T, scalar_tag, T, Tag> >
    operator+(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__plus, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__plus(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__minus, T, Tag1, T, Tag2> >
    operator-(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__minus, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__minus(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__minus, T, Tag, T, scalar_tag> >
    operator-(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__minus, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__minus(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__minus, T, scalar_tag, T, Tag> >
    operator-(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__minus, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__minus(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__multiplies, T, Tag1, T, Tag2> >
    operator*(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__multiplies, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__multiplies(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__multiplies, T, Tag, T, scalar_tag> >
    operator*(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__multiplies, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__multiplies(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__multiplies, T, scalar_tag, T, Tag> >
    operator*(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__multiplies, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__multiplies(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__divides, T, Tag1, T, Tag2> >
    operator/(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__divides, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__divides(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__divides, T, Tag, T, scalar_tag> >
    operator/(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__divides, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__divides(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__divides, T, scalar_tag, T, Tag> >
    operator/(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__divides, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__divides(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__modulus, T, Tag1, T, Tag2> >
    operator%(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__modulus, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__modulus(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__modulus, T, Tag, T, scalar_tag> >
    operator%(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__modulus, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__modulus(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__modulus, T, scalar_tag, T, Tag> >
    operator%(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__modulus, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__modulus(), val, rhs);
    }

    /// Bitwise operators.

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__bit_and, T, Tag1, T, Tag2> >
    operator&(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__bit_and, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__bit_and(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__bit_and, T, Tag, T, scalar_tag> >
    operator&(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__bit_and, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__bit_and(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__bit_and, T, scalar_tag, T, Tag> >
    operator&(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__bit_and, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__bit_and(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__bit_or, T, Tag1, T, Tag2> >
    operator|(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__bit_or, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__bit_or(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__bit_or, T, Tag, T, scalar_tag> >
    operator|(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__bit_or, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__bit_or(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__bit_or, T, scalar_tag, T, Tag> >
    operator|(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__bit_or, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__bit_or(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__bit_xor, T, Tag1, T, Tag2> >
    operator^(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__bit_xor, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__bit_xor(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__bit_xor, T, Tag, T, scalar_tag> >
    operator^(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__bit_xor, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__bit_xor(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__bit_xor, T, scalar_tag, T, Tag> >
    operator^(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__bit_xor, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__bit_xor(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__left_shift, T, Tag1, T, Tag2> >
    operator<<(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__left_shift, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__left_shift(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__left_shift, T, Tag, T, scalar_tag> >
    operator<<(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__left_shift, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__left_shift(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< T, lazy_binary_tag<__left_shift, T, scalar_tag, T, Tag> >
    operator<<(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__left_shift, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__left_shift(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< T, lazy_binary_tag<__right_shift, T, Tag1, T, Tag2> >
    operator>>(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__right_shift, T, Tag1, T, Tag2> Closure;
        return base_array<T, Closure>(__right_shift(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< T, lazy_binary_tag<__right_shift, T, Tag, T, scalar_tag> >
    operator>>(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__right_shift, T, Tag, T, scalar_tag> Closure;
        return base_array<T, Closure>(__right_shift(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< T, lazy_binary_tag<__right_shift, T, scalar_tag, T, Tag> >
    operator>>(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__right_shift, T, scalar_tag, T, Tag> Closure;
        return base_array<T, Closure>(__right_shift(), val, rhs);
    }

    /// Logical operators.

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__logical_and, T, Tag1, T, Tag2> >
    operator&&(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__logical_and, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__logical_and(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__logical_and, T, Tag, T, scalar_tag> >
    operator&&(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__logical_and, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__logical_and(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__logical_and, T, scalar_tag, T, Tag> >
    operator&&(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__logical_and, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__logical_and(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__logical_or, T, Tag1, T, Tag2> >
    operator||(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__logical_or, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__logical_or(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__logical_or, T, Tag, T, scalar_tag> >
    operator||(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__logical_or, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__logical_or(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__logical_or, T, scalar_tag, T, Tag> >
    operator||(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__logical_or, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__logical_or(), val, rhs);
    }

    /// Relational operators.

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__equal_to, T, Tag1, T, Tag2> >
    operator==(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__equal_to, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__equal_to(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__equal_to, T, Tag, T, scalar_tag> >
    operator==(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__equal_to, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__equal_to(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__equal_to, T, scalar_tag, T, Tag> >
    operator==(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__equal_to, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__equal_to(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__not_equal_to, T, Tag1, T, Tag2> >
    operator!=(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__not_equal_to, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__not_equal_to(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__not_equal_to, T, Tag, T, scalar_tag> >
    operator!=(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__not_equal_to, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__not_equal_to(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__not_equal_to, T, scalar_tag, T, Tag> >
    operator!=(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__not_equal_to, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__not_equal_to(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__less, T, Tag1, T, Tag2> >
    operator<(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__less, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__less(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< bool, lazy_binary_tag<__less, T, Tag, T, scalar_tag> >
    operator<(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__less, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__less(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< bool, lazy_binary_tag<__less, T, scalar_tag, T, Tag> >
    operator<(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__less, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__less(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__greater, T, Tag1, T, Tag2> >
    operator>(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__greater, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__greater(), lhs, rhs);
    }

    template <class T, class Tag>
    inline base_array< bool, lazy_binary_tag<__greater, T, Tag, T, scalar_tag> >
    operator>(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__greater, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__greater(), lhs, val);
    }

    template <class T, class Tag>
    inline base_array< bool, lazy_binary_tag<__greater, T, scalar_tag, T, Tag> >
    operator>(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__greater, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__greater(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline base_array< bool, lazy_binary_tag<__less_equal, T, Tag1, T, Tag2> >
    operator<=(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__less_equal, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__less_equal(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__less_equal, T, Tag, T, scalar_tag> >
    operator<=(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__less_equal, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__less_equal(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__less_equal, T, scalar_tag, T, Tag> >
    operator<=(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__less_equal, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__less_equal(), val, rhs);
    }

    template <class T, class Tag1, class Tag2>
    inline
    base_array< bool, lazy_binary_tag<__greater_equal, T, Tag1, T, Tag2> >
    operator>=(const base_array<T, Tag1> &lhs, const base_array<T, Tag2> &rhs) {
        typedef lazy_binary_tag<__greater_equal, T, Tag1, T, Tag2> Closure;
        return base_array<bool, Closure>(__greater_equal(), lhs, rhs);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__greater_equal, T, Tag, T, scalar_tag> >
    operator>=(
        const base_array<T, Tag> &lhs,
        const typename base_array<T, Tag>::value_type &val
    ) {
        typedef lazy_binary_tag<__greater_equal, T, Tag, T, scalar_tag> Closure;
        return base_array<bool, Closure>(__greater_equal(), lhs, val);
    }

    template <class T, class Tag>
    inline
    base_array< bool, lazy_binary_tag<__greater_equal, T, scalar_tag, T, Tag> >
    operator>=(
        const typename base_array<T, Tag>::value_type &val,
        const base_array<T, Tag> &rhs
    ) {
        typedef lazy_binary_tag<__greater_equal, T, scalar_tag, T, Tag> Closure;
        return base_array<bool, Closure>(__greater_equal(), val, rhs);
    }
}

#include "numcpp/array/array.tcc"

#endif // NUMCPP_ARRAY_H_INCLUDED
