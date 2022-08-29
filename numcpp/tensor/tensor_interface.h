/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/tensor/tensor_interface.h
  *  This header defines a generic interface for tensor subclasses.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TENSOR_INTERFACE_H_INCLUDED
#define NUMCPP_TENSOR_INTERFACE_H_INCLUDED

#if __cplusplus < 201103L
#error This file requires compiler and library support for the ISO C++ 2011 \
standard. This support must be enabled with the -std=c++11 or -std=gnu++11 \
compiler options.
#else

#include "numcpp/shape.h"
#include "numcpp/functional/operators.h"
#include <type_traits>

namespace numcpp {
    /// Interfaces.
    template <class T, size_t Rank, class Tag>
    class tensor_interface;

    template <class T, size_t Rank, class Tag>
    class base_tensor;

    /// Iterators.
    template <class T, size_t Rank, class Tag>
    class base_tensor_iterator;

    template <class T, size_t Rank, class Tag>
    class base_tensor_const_iterator;

    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor_reduce_iterator;

    template <class T, size_t Rank, class Tag, size_t N>
    class base_tensor_const_reduce_iterator;

    /// Subclases.
    struct scalar_tag;
    struct tensor_tag;
    struct view_tag;
    struct indirect_tag;

    template <class T, size_t Rank>
    using tensor = base_tensor<T, Rank, tensor_tag>;

    template <class T, size_t Rank>
    using tensor_view = base_tensor<T, Rank, view_tag>;

    template <class T, size_t Rank>
    using indirect_tensor = base_tensor<T, Rank, indirect_tag>;

    template <class Function, class T, class Tag>
    struct lazy_unary_tag;

    template <class Function, class T, class TagT, class U, class TagU>
    struct lazy_binary_tag;

    /**
     * @brief A tensor_interface is a generic interface for tensor subclasses.
     * It implements an assortment of methods which will be inherited to all
     * the subclasses. A tensor_interface works by using the Curiously
     * Recurring Template Pattern
     * @link https://en.cppreference.com/w/cpp/language/crtp @endlink
     *
     * @tparam T Type of the elements contained in the base_tensor.
     * @tparam Rank Dimension of the base_tensor. It must be a positive
     *     integer.
     * @tparam Tag Type indicating which specialization of base_tensor refers
     *     to.
     */
    template <class T, size_t Rank, class Tag>
    class tensor_interface {
        static_assert(Rank > 0, "Rank must be positive");

    public:
        /// Public member types.
        typedef base_tensor_iterator<T, Rank, Tag> iterator;
        typedef base_tensor_const_iterator<T, Rank, Tag> const_iterator;

        /// Iterators.

        /**
         * @brief Returns an iterator pointing to the first element in the
         * tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A random access iterator to the beginning of the tensor. If
         *     the tensor is const-qualified, the function returns a
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator begin();
        const_iterator begin() const;

        iterator begin(bool row_major);
        const_iterator begin(bool row_major) const;

        /**
         * @brief Returns an iterator pointing to the past-the-end element in
         * the tensor. It does not point to any element, and thus shall not be
         * dereferenced.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A random access iterator to the element past the end of the
         *     tensor. If the tensor is const-qualified, the function returns a
         *     const_iterator. Otherwise, it returns an iterator.
         */
        iterator end();
        const_iterator end() const;

        iterator end(bool row_major);
        const_iterator end(bool row_major) const;

        /**
         * @brief Returns a const_iterator pointing to the first element in the
         * tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A const_iterator to the beginning of the tensor.
         */
        const_iterator cbegin() const;
        const_iterator cbegin(bool row_major) const;

        /**
         * @brief Returns a const_iterator pointing to the past-the-end element
         * in the tensor.
         *
         * @param row_major It is an optional parameter that changes the order
         *     in which elements are iterated. If provided, the elements are
         *     iterated in row-major order (i.e., from first axis to last axis)
         *     or column-major order (i.e., from last axis to first axis) as
         *     specified by row_major. Otherwise, the elements are iterated in
         *     the same order as stored in memory.
         *
         * @return A const_iterator to the element past the end of the tensor.
         */
        const_iterator cend() const;
        const_iterator cend(bool row_major) const;

        /// Compound assignment operator.

        /**
         * @brief Assigns to *this the result of performing the respective
         * operation on all the elements in the tensor.
         *
         * When the right-hand side argument is a tensor object, the operation
         * is performed between the corresponding elements in each object (the
         * first element of the left one with the right one, the second with
         * the second, and so on...)
         *
         * When the right-hand side argument is a value, the operation is
         * applied to all the elements in the tensor against that value.
         *
         * @param rhs Right-hand side tensor-like object.
         * @param val Value to use as right-hand operand.
         *
         * @return *this
         *
         * @throw std::invalid_argument Thrown if the shapes are not compatible
         *     and cannot be broadcasted according to broadcasting rules.
         */
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator+=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator-=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator*=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator/=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator%=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator&=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator|=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator^=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator<<=(
            const base_tensor<U, Rank, TagU> &rhs
        );
        template <class U, class TagU>
        base_tensor<T, Rank, Tag>& operator>>=(
            const base_tensor<U, Rank, TagU> &rhs
        );

        base_tensor<T, Rank, Tag>& operator+=(const T &val);
        base_tensor<T, Rank, Tag>& operator-=(const T &val);
        base_tensor<T, Rank, Tag>& operator*=(const T &val);
        base_tensor<T, Rank, Tag>& operator/=(const T &val);
        base_tensor<T, Rank, Tag>& operator%=(const T &val);
        base_tensor<T, Rank, Tag>& operator&=(const T &val);
        base_tensor<T, Rank, Tag>& operator|=(const T &val);
        base_tensor<T, Rank, Tag>& operator^=(const T &val);
        base_tensor<T, Rank, Tag>& operator<<=(const T &val);
        base_tensor<T, Rank, Tag>& operator>>=(const T &val);

        /// Public methods.

        /**
         * @brief Test whether all tensor elements evaluates to true.
         *
         * @return true if all the elements evaluates to true and false
         *     otherwise.
         */
        bool all() const;

        /**
         * @brief Test whether all tensor elements over the given axes
         * evaluates to true.
         *
         * @param axes A shape_t object with the axes along which the logical
         *     AND reduction is performed.
         *
         * @return A new tensor with the results of the test over the axes. The
         *     output tensor will have the same dimension and the same shape,
         *     except that the axes which are reduced are left as dimensions of
         *     size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<bool, Rank> all(size_t axis) const;
        template <size_t N>
        tensor<bool, Rank> all(const shape_t<N> &axes) const;

        /**
         * @brief Test whether any tensor element evaluates to true.
         *
         * @return true if any element evaluates to true and false otherwise.
         */
        bool any() const;

        /**
         * @brief Test whether any tensor element over the given axes evaluates
         * to true.
         *
         * @param axes A shape_t object with the axes along which the logical
         *     OR reduction is performed.
         *
         * @return A new tensor with the results of the test over the axes. The
         *     output tensor will have the same dimension and the same shape,
         *     except that the axes which are reduced are left as dimensions of
         *     size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<bool, Rank> any(size_t axis) const;
        template <size_t N>
        tensor<bool, Rank> any(const shape_t<N> &axes) const;

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
        void apply(Function &&f);

        /**
         * @brief Return the indices of the maximum value in the tensor.
         *
         * @return The indices of the maximum value in the tensor.
         */
        index_t<Rank> argmax() const;

        /**
         * @brief Return the indices of the maximum value in the tensor along
         * the given axis.
         *
         * @param axis Axis along which the maximum value is selected.
         *
         * @return A new tensor with the indices of the maximum value along an
         *     axis. The output tensor will have the same dimension and the
         *     same shape, except that the axis which is reduced is left as a
         *     dimension of size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<size_t, Rank> argmax(size_t axis) const;

        /**
         * @brief Return the indices of the minimum value in the tensor.
         *
         * @return The indices of the minimum value in the tensor.
         */
        index_t<Rank> argmin() const;

        /**
         * @brief Return the indices of the minimum value in the tensor along
         * the given axis.
         *
         * @param axis Axis along which the minimum value is selected.
         *
         * @return A new tensor with the indices of the minimum value along an
         *     axis. The output tensor will have the same dimension and the
         *     same shape, except that the axis which is reduced is left as a
         *     dimension of size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<size_t, Rank> argmin(size_t axis) const;

        /**
         * @brief Return the indices that would partition the tensor.
         *
         * @param kth Element index to partition by. The element at the kth
         *     position is the element that would be in that position in the
         *     sorted tensor. The other elements are left without any specific
         *     order, except that none of the elements preceding kth are
         *     greater than it, and none of the elements following it are less.
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         *
         * @return A one-dimensional tensor of indices that partitions the
         *     tensor. If a is a tensor, then a[a.argpartition(kth)] yields a
         *     partitioned a.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<index_t<Rank>, 1> argpartition(size_t kth) const;
        template <class Compare>
        tensor<index_t<Rank>, 1> argpartition(size_t kth, Compare comp) const;

         /**
         * @brief Return the indices that would sort the tensor.
         *
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         * @param stable If true, preserve the relative order of the elements
         *     with equivalent values. Otherwise, equivalent elements are not
         *     guaranteed to keep their original relative order.
         *
         * @return A one-dimensional tensor of indices that sort the tensor. If
         *     a is a tensor, then a[a.argsort()] yields a sorted a.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<index_t<Rank>, 1> argsort(bool stable = false) const;
        template <class Compare>
        tensor<index_t<Rank>, 1> argsort(Compare comp, bool stable=false) const;

        /**
         * @brief Cast each element to a specified type.
         *
         * @tparam U Data type to which the elements are casted.
         *
         * @return A light-weight object with the elements in the tensor casted
         *     to the specified type. This function does not create a new
         *     tensor, instead, it returns a readonly view of the tensor with
         *     its elements casted to the specified type.
         */
        template <class U>
        base_tensor<U, Rank, lazy_unary_tag<cast_to<U>, T, Tag> >
        astype() const;

        /**
         * @brief Clamp the values in the tensor. Given an interval
         * [a_min, a_max], values smaller than a_min become a_min, and values
         * larger than a_max become a_max. For complex types, the real and
         * imaginary parts are clamped separately.
         *
         * @param a_min The lower boundary to clamp.
         * @param a_max The upper boundary to clamp.
         *
         * @note The behavior is undefined if a_min is greater than a_max.
         */
        void clamp(const T &a_min, const T &a_max);

        /**
         * @brief Return a copy of the tensor.
         *
         * @return A new tensor with a copy of each of the elements in the
         *     tensor, in the same order.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank> copy() const;

        /**
         * @brief Return the maximum value contained in the tensor.
         *
         * @return The maximum value in the tensor.
         */
        typename std::remove_cv<T>::type max() const;

        /**
         * @brief Return the maximum value contained in the tensor over the
         * given axes.
         *
         * @param axes A shape_t object with the axes along which the maximum
         *     value is selected.
         *
         * @return A new tensor with the maximum value over the axes. The
         *     output tensor will have the same dimension and the same shape,
         *     except that the axes which are reduced are left as dimensions of
         *     size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        max(size_t axis) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        max(const shape_t<N> &axes) const;

        /**
         * @brief Return the average of the tensor elements.
         *
         * @return The average of the tensor elements.
         */
        typename std::remove_cv<T>::type mean() const;

        /**
         * @brief Return the average of the tensor elements over the given
         * axes.
         *
         * @param axes A shape_t object with the axes along which the average
         *     is computed.
         *
         * @return A new tensor with the average over the axes. The output
         *     tensor will have the same dimension and the same shape, except
         *     that the axes which are reduced are left as dimensions of size
         *     one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        mean(size_t axis) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        mean(const shape_t<N> &axes) const;

        /**
         * @brief Return the minimum value contained in the tensor.
         *
         * @return The minimum value in the tensor.
         */
        typename std::remove_cv<T>::type min() const;

        /**
         * @brief Return the minimum value contained in the tensor over the
         * given axes.
         *
         * @param axes A shape_t object with the axes along which the minimum
         *     value is selected.
         *
         * @return A new tensor with the minimum value over the axes. The
         *     output tensor will have the same dimension and the same shape,
         *     except that the axes which are reduced are left as dimensions of
         *     size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        min(size_t axis) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        min(const shape_t<N> &axes) const;

        /**
         * @brief Return the indices of the elements that are non-zero.
         *
         * @return A new tensor with the indices of the elements that are
         * non-zero.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<index_t<Rank>, 1> nonzero() const;

        /**
         * @brief Partition the elements in-place.
         *
         * @param kth Element index to partition by. The element at the kth
         *     position is the element that would be in that position in the
         *     sorted tensor. The other elements are left without any specific
         *     order, except that none of the elements preceding kth are
         *     greater than it, and none of the elements following it are less.
         * @param axis Axis along which to partition. Defaults to Rank - 1,
         *     which means partition along the last axis.
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         */
        void partition(size_t kth, size_t axis = Rank - 1);
        template <class Compare>
        void partition(size_t kth, size_t axis, Compare comp);

        /**
         * @brief Return the product of the tensor elements.
         *
         * @return The product of the tensor elements.
         */
        typename std::remove_cv<T>::type prod() const;

        /**
         * @brief Return the product of the tensor elements over the given
         * axes.
         *
         * @param axes A shape_t object with the axes along which the product
         *     is performed.
         *
         * @return A new tensor with the product over the axes. The output
         *     tensor will have the same dimension and the same shape, except
         *     that the axes which are reduced are left as dimensions of size
         *     one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        prod(size_t axis) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        prod(const shape_t<N> &axes) const;

        /**
         * @brief Reverse the order of the elements in-place.
         *
         * @param axis Axis along which to reverse. Defaults to Rank - 1, which
         *     means reverse along the last axis.
         */
        void reverse(size_t axis = Rank - 1);

        /**
         * @brief Sort the elements in-place.
         *
         * @param axis Axis along which to sort. Defaults to Rank - 1, which
         *     means sort along the last axis.
         * @param comp Custom comparator. A binary function that accepts two
         *     elements of type T as arguments, and returns a value convertible
         *     to bool. The value returned indicates whether the element passed
         *     as first argument is considered to go before the second.
         * @param stable If true, preserve the relative order of the elements
         *     with equivalent values. Otherwise, equivalent elements are not
         *     guaranteed to keep their original relative order.
         */
        void sort(size_t axis = Rank - 1, bool stable = false);
        template <class Compare>
        void sort(size_t axis, Compare comp, bool stable = false);

        /**
         * @brief Return the standard deviation of the tensor elements.
         *
         * @details The standard deviation is defined as the square root of the
         * average of the squared deviations from the mean
         *     stddev(a) = sqrt(mean(x)),    x = abs(a - mean(a))**2
         * The mean is calculated as sum(x)/n, where n = x.size(). However, if
         * bias is false, the divisor n - 1 is used instead of n. In
         * statistics, n - 1 provides an unbiased estimator of the sample
         * variance; while n provides the maximum likelihood estimator of the
         * variance for normally distributed variables.
         *
         * @param bias If bias is true, then normalization is by n. Otherwise,
         *     normalization is by n - 1.
         *
         * @return The standard deviation of the tensor elements.
         */
        typename std::remove_cv<T>::type stddev(bool bias) const;

        /**
         * @brief Return the standard deviation of the tensor elements over the
         * given axes.
         *
         * @param axes A shape_t object with the axes along which the standard
         *     deviation is computed.
         * @param bias If bias is true, then normalization is by n. Otherwise,
         *     normalization is by n - 1.
         *
         * @return A new tensor with the standard deviation over the axes. The
         *     output tensor will have the same dimension and the same shape,
         *     except that the axes which are reduced are left as dimensions of
         *     size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        stddev(size_t axis, bool bias) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        stddev(const shape_t<N> &axes, bool bias) const;

        /**
         * @brief Return the sum of the tensor elements.
         *
         * @return The sum of the tensor elements.
         */
        typename std::remove_cv<T>::type sum() const;

        /**
         * @brief Return the sum of the tensor elements over the given axes.
         *
         * @param axes A shape_t object with the axes along which the sum is
         *     performed.
         *
         * @return A new tensor with the sum over the axes. The output tensor
         *     will have the same dimension and the same shape, except that the
         *     axes which are reduced are left as dimensions of size one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        sum(size_t axis) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        sum(const shape_t<N> &axes) const;

        /**
         * @brief Return the variance of the tensor elements.
         *
         * @details The variance is defined as the average of the squared
         * deviations from the mean
         *     var(a) = mean(x),    x = abs(a - mean(a))**2
         * The mean is calculated as sum(x)/n, where n = x.size(). However, if
         * bias is false, the divisor n - 1 is used instead of n. In
         * statistics, n - 1 provides an unbiased estimator of the sample
         * variance; while n provides the maximum likelihood estimator of the
         * variance for normally distributed variables.
         *
         * @param bias If bias is true, then normalization is by n. Otherwise,
         *     normalization is by n - 1.
         *
         * @return The variance of the tensor elements.
         */
        typename std::remove_cv<T>::type var(bool bias) const;

        /**
         * @brief Return the variance of the tensor elements over the given
         * axes.
         *
         * @param axes A shape_t object with the axes along which the variance
         *     is computed.
         * @param bias If bias is true, then normalization is by n. Otherwise,
         *     normalization is by n - 1.
         *
         * @return A new tensor with the variance over the axes. The output
         *     tensor will have the same dimension and the same shape, except
         *     that the axes which are reduced are left as dimensions of size
         *     one.
         *
         * @throw std::bad_alloc If the function fails to allocate storage it
         *     may throw an exception.
         */
        tensor<typename std::remove_cv<T>::type, Rank>
        var(size_t axis, bool bias) const;

        template <size_t N>
        tensor<typename std::remove_cv<T>::type, Rank>
        var(const shape_t<N> &axes, bool bias) const;

    private:
        /**
         * @brief Returns a pointer to the (derived) tensor class.
         */
        base_tensor<T, Rank, Tag>* base();
        const base_tensor<T, Rank, Tag>* base() const;

        /**
         * @brief Helper function. Apply a binary function with another tensor
         * or with a value.
         */
        template <class Function, class U, class TagU>
        base_tensor<T, Rank, Tag>& apply_binary_function(
            Function f, const base_tensor<U, Rank, TagU> &rhs
        );

        template <class Function>
        base_tensor<T, Rank, Tag>& apply_binary_function(
            Function f, const T &val
        );

        /**
         * @brief Helper function. Apply a function over multiple axes.
         */
        template <class R, class Function>
        tensor<R, Rank> apply_along_axis(Function f, size_t axis) const;

        template <class R, class Function, size_t N>
        tensor<R, Rank> apply_over_axes(
            Function f, const shape_t<N> &axes
        ) const;
    };
}

#include "numcpp/iterators/tensor_iterator.h"
#include "numcpp/iterators/reduce_iterator.h"
#include "numcpp/tensor/tensor_interface.tcc"

#endif // C++11
#endif // NUMCPP_TENSOR_INTERFACE_H_INCLUDED
