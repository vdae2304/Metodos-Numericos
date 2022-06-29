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

 /** @file include/numcpp/array/array_view.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/array.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_ARRAY_VIEW_H_INCLUDED
#define NUMCPP_ARRAY_VIEW_H_INCLUDED

namespace numcpp {    
    /**
     * @brief An array_view is just a view of an array object. It references 
     * the elements in the original array. The view itself does not own the 
     * data and any changes made to the view will affect the original array, 
     * and any changes made to the original array will affect the view.
     *
     * @tparam T Type of the elements contained in the array_view. This shall 
     *     be an arithmetic type or a class that behaves like one (such as 
     *     std::complex).
     */
    template <class T>
    class base_array<T, array_view_tag> {
    public:
        /// Member types:
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef base_array_iterator<T, array_view_tag> iterator;
        typedef base_array_const_iterator<T, array_view_tag> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        /// Constructors.

        /**
         * @brief Default constructor. Constructs an array_view that does not 
         * reference any object.
         */
        base_array();

        /**
         * @brief Array constructor. Constructs an array_view that references 
         * an array.
         *
         * @param n The number of elements in the array_view.
         * @param data The pointer to the memory array used by the array_view.
         */
        base_array(size_t n, T *data);

        /**
         * @brief Slice array constructor. Constructs an array_view that 
         * references a subset of array elements.
         *
         * @param n The number of elements in the array_view.
         * @param data The pointer to the memory array used by the array_view.
         * @param offset Index of the first element selected into the 
         *     array_view.
         * @param stride Span that separates the elements selected into the 
         *     array_view.
         */
        base_array(size_t n, T *data, size_t offset, size_t stride);

        /**
         * @brief Copy constructor. Constructs an array_view as a copy of 
         * other.
         *
         * @param other An array_view of the same type.
         */
        base_array(const base_array &other);

        /**
         * @brief Move constructor. Constructs an array_view with the contents 
         * of other.
         *
         * @param other An array_view of the same type. other is left in an 
         *     empty state.
         */
        base_array(base_array &&other);

        /// Destructor.
        ~base_array();

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the 
         * array_view.
         *
         * @return A random access iterator to the beginning of the array_view. 
         *     If the array_view is const-qualified, the function returns a 
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator begin();
        const_iterator begin() const;

        /**
         * @brief Returns an iterator pointing to the past-the-end element in 
         * the array_view. It does not point to any element, and thus shall not 
         * be dereferenced.
         *
         * @return A random access iterator to the element past the end of the 
         *     array_view. If the array_view is const-qualified, the function 
         *     returns a const_iterator. Otherwise, it returns an iterator.
         */
        iterator end();
        const_iterator end() const;

        /**
         * @brief Returns a reverse iterator pointing to the last element in 
         * the array_view (i.e., its reverse beginning). Reverse iterators 
         * iterate backwards.
         *
         * @return A reverse random access iterator to the reverse beginning of 
         *     the array_view. If the array_view is const-qualified, the 
         *     function returns a const_reverse_iterator. Otherwise, it returns 
         *     a reverse_iterator.
         */
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;

        /**
         * @brief Returns a reverse iterator pointing to the element preceding 
         * the first element in the array_view (i.e., its reverse end).
         *
         * @return A reverse random access iterator to the reverse end of the 
         *     array_view. If the array_view is const-qualified, the function 
         *     returns a const_reverse_iterator. Otherwise, it returns a 
         *     reverse_iterator.
         */
        reverse_iterator rend();
        const_reverse_iterator rend() const;

        /**
         * @brief Returns a const_iterator pointing to the first element in the 
         * array_view.
         *
         * @return A const_iterator to the beginning of the array_view.
         */
        const_iterator cbegin() const;

        /**
         * @brief Returns a const_iterator pointing to the past-the-end element 
         * in the array_view.
         *
         * @return A const_iterator to the element past the end of the 
         *     array_view.
         */
        const_iterator cend() const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the last element 
         * in the array_view (i.e., its reverse beginning).
         *
         * @return A const_reverse_iterator to the reverse beginning of the 
         *     array_view.
         */
        const_reverse_iterator crbegin() const;

        /**
         * @brief Returns a const_reverse_iterator pointing to the element 
         * preceding the first element in the array_view (i.e., its reverse 
         * end).
         *
         * @return A const_reverse_iterator to the reverse end of the 
         *     array_view.
         */
        const_reverse_iterator crend() const;

        /// Array indexing.

        /**
         * @brief Subscript operator. Returns a reference to the element at 
         * position i in the array_view. The element at position i corresponds 
         * to the element at position offset + i*stride in the original array.
         *
         * @param i Position of an element in the array_view. Must be between 
         *     0 and size() - 1.
         *
         * @return The element at the specified position in the array_view. 
         *     If the array_view is const-qualified, the function returns a 
         *     reference to const T. Otherwise, it returns a reference to T.
         *
         * @throw std::out_of_range Thrown if i is out of bounds.
         */
        T& operator[](size_t i);
        const T& operator[](size_t i) const;

        /**
         * @brief Returns the number of elements in the array_view.
         *
         * @return The number of elements in the array_view.
         */
        size_t size() const;

        /**
         * @brief Returns whether the array_view is empty (i.e., whether its 
         * size is 0).
         *
         * @return true if the array_view size is 0, false otherwise.
         */
        bool empty() const;

        /**
         * @brief Returns a pointer to the memory array used internally by the 
         * array_view.
         *
         * @return A pointer to the array used internally by the array_view. 
         *     If the array_view is const-qualified, the function returns a 
         *     pointer to const T. Otherwise, it returns a pointer to T.
         */
        T* data();
        const T* data() const;

        /// Assignment operator.

        /**
         * @brief Copy assignment. Assigns to each element the value of the 
         * corresponding element in other.
         *
         * @param other An array-like object of the same type and size.
         * 
         * @return *this
         *
         * @throw std::invalid_argument Thrown if array sizes do not match.
         */
        base_array& operator=(const base_array &other);
        template <class Tag>
        base_array& operator=(const base_array<T, Tag> &other);

        /**
         * @brief Fill assignment. Assigns val to every element.
         *
         * @param val Value assigned to all the elements in the array_view.
         * 
         * @return *this
         */
        base_array& operator=(const T &val);

        /**
         * @brief Move assignment. Acquires the contents of other, leaving 
         * other in an empty state.
         *
         * @param other An array_view of the same type.
         * 
         * @return *this
         */
        base_array& operator=(base_array &&other);

        /// Compound assignment operator.

        /**
         * @brief Assigns to *this the result of performing the respective 
         * operation on all the elements in the array_view.
         *
         * When the right-hand side argument is an array object, the operation 
         * is performed between the corresponding elements in each object (the 
         * first element of the left one with the right one, the second with 
         * the second, and so on...)
         *
         * When the right-hand side argument is a value, the operation is 
         * applied to all the elements in the array_view against that value.
         *
         * @param rhs Right-hand side array_view object.
         * @param val Value to use as right-hand operand.
         *
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the right-hand side argument 
         *     is an array_view object with different size to *this.
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
         * @brief Return the index of the maximum value in the array_view.
         *
         * @return The index of the maximum value in the array_view.
         */
        size_t argmax() const;

        /**
         * @brief Return the index of the minimum value in the array_view.
         *
         * @return The index of the minimum value in the array_view.
         */
        size_t argmin() const;

        /**
         * @brief Cast each element to a specified type.
         * 
         * @return A light-weight object with the elements in the array_view 
         *     casted to the specified type. Convertible to an array object.
         */
        template <class U>
        base_array< U, lazy_unary_tag<__identity, T, array_view_tag> > 
        astype() const;

        /**
         * @brief Clamp the values in the array_view. Given an interval 
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
         *     element in the array_view. Convertible to an array object.
         */
        base_array< T, lazy_unary_tag<__conjugate, T, array_view_tag> > 
        conj() const;

        /**
         * @brief Return or set the imaginary part, element-wise.
         * 
         * @param arr An array-like object with the values to set the imaginary 
         *     part to.
         * 
         * @return A light-weight object with the imaginary part of each 
         *     element in the array_view. Convertible to an array object.
         */
        base_array< 
            typename complex_traits<T>::value_type, 
            lazy_unary_tag<__imag_part, T, array_view_tag> 
        > 
        imag() const;

        template <class Tag>
        void imag(
            const base_array<typename complex_traits<T>::value_type, Tag> &arr
        );

        /**
         * @brief Return the maximum value contained in the array_view.
         *
         * @return The maximum value in the array_view.
         */
        T max() const;

        /**
         * @brief Return the average of the array_view elements.
         *
         * @return The average of the array_view elements.
         */
        T mean() const;

        /**
         * @brief Return the minimum value contained in the array_view.
         *
         * @return The minimum value in the array_view.
         */
        T min() const;

        /**
         * @brief Partition an array_view in-place.
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
         * @brief Return the product of the array_view elements.
         *
         * @return The product of the array_view elements.
         */
        T prod() const;

        /**
         * @brief Return or set the real part, element-wise.
         * 
         * @param arr An array-like object with the values to set the real part 
         *     to.
         * 
         * @return A light-weight object with the real part of each element in 
         *     the array_view. Convertible to an array object.
         */
        base_array< 
            typename complex_traits<T>::value_type, 
            lazy_unary_tag<__real_part, T, array_view_tag> 
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
         * @brief Sort an array_view in-place.
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
         * @brief Return the standard deviation of the array_view elements. 
         *
         * @param ddof Delta degrees of freedom. [See numcpp::stddev for full  
         * documentation.]
         *
         * @return The standard deviation of the array_view elements.
         */
        T stddev(size_t ddof = 0) const;

        /**
         * @brief Return the sum of the array_view elements.
         *
         * @return The sum of the array_view elements.
         */
        T sum() const;

        /**
         * @brief Return the variance of the array_view elements. 
         *
         * @param ddof Delta degrees of freedom. [See numcpp::var for full 
         * documentation.]
         *
         * @return The variance of the array_view elements.
         */
        T var(size_t ddof = 0) const;

    protected:
        /// Pointer to data.
        T *m_data;

        /// Number of elements in the array_view.
        size_t m_size;

        /// Offset and stride in the array_view with respect to the original 
        /// array.
        size_t m_offset, m_stride;
    };
}

#include "numcpp/array/array_view.tcc"

#endif // NUMCPP_ARRAY_VIEW_H_INCLUDED
