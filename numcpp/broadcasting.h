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

 /** @file include/numcpp/broadcasting.h
  *  This header defines routines for broadcasting, functional programming and
  *  indexing.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_BROADCASTING_H_INCLUDED
#define NUMCPP_BROADCASTING_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/routines/algo.h"
#include "numcpp/broadcasting/utilities.h"

#include "numcpp/broadcasting/array_reverse.h"
#include "numcpp/broadcasting/matrix_rotate.h"
#include "numcpp/broadcasting/array_select.h"
#include "numcpp/broadcasting/matrix_select.h"
#include "numcpp/routines/outer_matrix.h"

namespace numcpp {
    /// Broadcasting

    /**
     * @brief Broadcast a value to an array.
     *
     * @details Return a readonly array_view of given size filled with val.
     * This function does not create a new array, instead, it broadcasts the
     * given value as an array of given size. All the elements in the
     * broadcasted array refer to the same memory location.
     *
     * @param val A reference to the fill value.
     * @param n Length of array.
     *
     * @return A readonly view of the original value with the given size.
     */
    template <class T>
    const array_view<T> broadcast_to(T &val, size_t n);

    /**
     * @brief Broadcast a value to a matrix.
     *
     * @details Return a readonly matrix_view of given size filled with val.
     * This function does not create a new matrix, instead, it broadcasts the
     * given value as a matrix of given size. All the elements in the
     * broadcasted matrix refer to the same memory location.
     *
     * @param val A reference to the fill value.
     * @param m Number of rows.
     * @param n Number of columns.
     *
     * @return A readonly view of the original value with the given number of
     *     rows and columns.
     */
    template <class T>
    const matrix_view<T> broadcast_to(T &val, size_t m, size_t n);

    /**
     * @brief Broadcast an array to a matrix.
     *
     * @details Return a readonly matrix_view constructed by repeating an array
     * a given number of times. This function does not create a new matrix,
     * instead, it broadcasts the given array as a matrix of given size. Some
     * elements in the broadcasted matrix may refer to the same memory
     * location.
     *
     * @param arr A reference to an array or array_view object.
     * @param reps The number of repetitions of arr along the new axis.
     * @param row_major If true (default), the array is tiled across each row.
     *     Otherwise, the array is tiled across each column.
     *
     * @return A read only view of the original array with the given size.
     */
    template <class T>
    const matrix_view<T> broadcast_to(
        array<T> &arr, size_t reps, bool row_major = true
    );

    template <class T>
    const matrix_view<T> broadcast_to(
        array_view<T> &arr, size_t reps, bool row_major = true
    );

    /// Creation routines from existing data

    /**
     * @brief Convert the input to an array.
     *
     * @param data Input data, in any container form. This includes static
     *     arrays, std::vector, std::list, std::set, etc.
     *
     * @return Array interpretation of the data. No copy is performed if the
     *     input is already an array.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Container, class T = typename Container::value_type>
    array<T> asarray(const Container &data);

    template <class T, size_t N>
    array<T> asarray(const T (&data)[N]);

    template <class T>
    array<T> asarray(std::initializer_list<T> data);

    template <class T>
    array<T>& asarray(array<T> &data);

    /**
     * @brief Convert the input to a matrix.
     *
     * @param data Input data, in any container form. This includes static
     *     arrays, std::vector, std::list, std::set, etc. The input must be one
     *     dimensional, except for static arrays, matrix and
     *     std::initializer_list.
     * @param n Number of columns. The number of rows is inferred from the
     *     length of the data. Ignored if the input is already two dimensional.
     *     Defaults to 1.
     *
     * @return Matrix interpretation of the data. No copy is performed if the
     *     input is already a matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Container, class T = typename Container::value_type>
    matrix<T> asmatrix(const Container &data, size_t n = 1);

    template <class T, size_t M, size_t N>
    matrix<T> asmatrix(const T (&data)[M][N], size_t n = 1);

    template <class T>
    matrix<T> asmatrix(
        std::initializer_list< std::initializer_list<T> > data, size_t n = 1
    );

    template <class T>
    matrix<T>& asmatrix(matrix<T> &data, size_t n = 1);

    /**
     * @brief Return a copy of an array.
     *
     * @param arr Array-like object to copy.
     *
     * @return A copy of arr.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> copy(const base_array<T, Tag> &arr);

    /**
     * @brief Return a copy of a matrix.
     *
     * @param mat Matrix-like object to copy.
     *
     * @return A copy of mat.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    matrix<T> copy(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return a copy of the matrix collapsed into an array.
     *
     * @param mat Matrix-like object to flatten.
     * @param row_major Whether to flatten the elements in row-major order or
     *     column-major order. Defaults to the same order as stored in memory.
     *
     * @return The flattened matrix.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> flatten(const base_matrix<T, Tag> &mat);

    template <class T, class Tag>
    array<T> flatten(const base_matrix<T, Tag> &mat, bool row_major);

    /// Basic manipulation routines

    /**
     * @brief Cast each element in an array to a specified type.
     *
     * @tparam U Data type to which the array is cast.
     * @param arr Array-like object to cast.
     *
     * @return A light-weight object with the elements in the array casted
     *     to the specified type. Convertible to an array object.
     */
    template <class U, class T, class Tag>
    base_array< U, lazy_unary_tag<__identity, T, Tag> >
    astype(const base_array<T, Tag> &arr);

    /**
     * @brief Cast each element in a matrix to a specified type.
     *
     * @tparam U Data type to which the matrix is cast.
     * @param mat Matrix-like object to cast.
     *
     * @return A light-weight object with the elements in the matrix casted
     *     to the specified type. Convertible to a matrix object.
     */
    template <class U, class T, class Tag>
    base_matrix< U, lazy_unary_tag<__identity, T, Tag> >
    astype(const base_matrix<T, Tag> &mat);

    /**
     * @brief Return an array of pairs with each element constructed from the
     * corresponding elements in two array objects.
     *
     * @param arr1 First array-like argument.
     * @param arr2 Second array-like argument.
     *
     * @return A light-weight object with the elements in two array arguments
     *     zipped as a pair. Convertible to an array object.
     */
    template <class T, class Tag1, class U, class Tag2>
    base_array< std::pair<T, U>, lazy_binary_tag<__zip, T, Tag1, U, Tag2> >
    zip(const base_array<T, Tag1> &arr1, const base_array<U, Tag2> &arr2);

    /**
     * @brief Return a matrix of pairs with each element constructed from the
     * corresponding elements in two matrix objects.
     *
     * @param mat1 First matrix-like argument.
     * @param mat2 Second matrix-like argument.
     *
     * @return A light-weight object with the elements in two matrix arguments
     *     zipped as a pair. Convertible to a matrix object.
     */
    template <class T, class Tag1, class U, class Tag2>
    base_matrix< std::pair<T, U>, lazy_binary_tag<__zip, T, Tag1, U, Tag2> >
    zip(const base_matrix<T, Tag1> &mat1, const base_matrix<U, Tag2> &mat2);

    /**
     * @brief Return an array with each element constructed from taking the
     * I-th element of the corresponding pair in an array object.
     *
     * @tparam I Element index.
     * @param arr An array-like object of pairs.
     *
     * @return A light-weight object with the I-th element of each pair in the
     *     array. Convertible to an array object.
     */
    template <size_t I, class T, class U, class Tag>
    base_array<
        typename std::tuple_element< I, std::pair<T, U> >::type,
        lazy_unary_tag<__unzip<I>, std::pair<T, U>, Tag>
    >
    unzip(const base_array<std::pair<T, U>, Tag> &arr);

    /**
     * @brief Return a matrix with each element constructed from taking the
     * I-th element of the corresponding pair in a matrix object.
     *
     * @tparam I Element index.
     * @param mat A matrix-like object of pairs.
     *
     * @return A light-weight object with the I-th element of each pair in the
     *     matrix. Convertible to a matrix object.
     */
    template <size_t I, class T, class U, class Tag>
    base_matrix<
        typename std::tuple_element< I, std::pair<T, U> >::type,
        lazy_unary_tag<__unzip<I>, std::pair<T, U>, Tag>
    >
    unzip(const base_matrix<std::pair<T, U>, Tag> &mat);

    /**
     * @brief Return the elements of an array in reversed order.
     *
     * @param arr An array-like object.
     *
     * @return A light-weight object with the elements in reversed order.
     *     Convertible to an array object.
     */
    template <class T, class Tag>
    base_array< T, reverse_tag<Tag> > reverse(const base_array<T, Tag> &arr);

    /**
     * @brief Return the elements of a matrix in reversed order along an axis.
     *
     * @param mat A matrix-like object.
     * @param rowwise If true, reverse the elements along each row. Otherwise,
     *     reverse the elements along each column.
     *
     * @return A light-weight object with the elements in reversed order.
     *     Convertible to a matrix object.
     */
    template <class T, class Tag>
    base_matrix< T, reverse_tag<Tag> > reverse(
        const base_matrix<T, Tag> &mat, bool rowwise
    );

    /**
     * @brief Rotate a matrix by 90 degrees.
     *
     * @param mat A matrix-like object.
     * @param times Number of times the matrix is rotated by 90 degrees. A
     *     positive value indicates the matrix is rotated clockwise and a
     *     negative value indicates the matrix is rotated counter clockwise.
     *     Defaults to 1.
     *
     * @return The rotated matrix.
     */
    template <class T, class Tag>
    base_matrix< T, rotate_tag<Tag> > rot90(
        const base_matrix<T, Tag> &mat, int times = 1
    );

    /// Indexing routines

    /**
     * @brief Converts a pair of indices into a flat index.
     *
     * @param multi_index The indices to flatten. It can be either an index_t
     *     object or two size_t arguments.
     * @param m Number of rows.
     * @param n Number of columns.
     * @param row_major Determines whether the indices should be viewed as
     *     indexing in row-major order or column-major order. Defaults to
     *     row-major order.
     *
     * @return The flattened index.
     */
    size_t ravel_multi_index(
        size_t i, size_t j, size_t m, size_t n, bool row_major = true
    );

    size_t ravel_multi_index(
        const index_t &multi_index, size_t m, size_t n, bool row_major = true
    );

    /**
     * @brief Converts a pair of indices into a flat index.
     *
     * @param multi_index An array-like object of index_t with the indices to
     *     flatten.
     * @param m Number of rows.
     * @param n Number of columns.
     * @param row_major Determines whether the indices should be viewed as
     *     indexing in row-major order or column-major order. Defaults to
     *     row-major order.
     *
     * @return A light-weight object with the flattened indices. Convertible to
     *     an array object.
     */
    template <class Tag>
    base_array< size_t, lazy_unary_tag<__ravel_multi_index, index_t, Tag> >
    ravel_multi_index(
        const base_array<index_t, Tag> &multi_index,
        size_t m, size_t n, bool row_major = true
    );

    /**
     * @brief Converts a flat index into a pair of indices.
     *
     * @param index A flat index.
     * @param m Number of rows.
     * @param n Number of columns.
     * @param row_major Determines whether the indices should be viewed as
     *     indexing in row-major order or column-major order. Defaults to
     *     row-major order.
     *
     * @return The unraveled index.
     */
    index_t unravel_index(
        size_t index, size_t m, size_t n, bool row_major = true
    );

    /**
     * @brief Converts a flat index into a pair of indices.
     *
     * @param index An array-like object of size_t with the flat indices.
     * @param m Number of rows.
     * @param n Number of columns.
     * @param row_major Determines whether the indices should be viewed as
     *     indexing in row-major order or column-major order. Defaults to
     *     row-major order.
     *
     * @return A light-weight object with the unraveled indices. Convertible to
     *     an array object.
     */
    template <class Tag>
    base_array< index_t, lazy_unary_tag<__unravel_index, size_t, Tag> >
    unravel_index(
        const base_array<size_t, Tag> &index,
        size_t m, size_t n, bool row_major = true
    );

    /**
     * @brief Take elements from an array. When an array object is given, a
     * call such as np::take(arr, indices) is equivalent to arr[indices],
     * except that a copy is always returned.
     *
     * @param arr The source array.
     * @param indices An array-like object of size_t with the indices of the
     *     values to take.
     *
     * @return A new array with the elements from arr at the given positions.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class TagIndex>
    array<T> take(
        const base_array<T, Tag> &arr,
        const base_array<size_t, TagIndex> &indices
    );

    /**
     * @brief Take elements from a matrix. When a matrix object is given, a
     * call such as np::take(mat, indices) is equivalent to mat[indices],
     * except that a copy is always returned.
     *
     * @param mat The source matrix.
     * @param indices An array-like object of index_t with the indices of the
     *     values to take.
     *
     * @return A new array with the elements from mat at the given positions.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class TagIndex>
    array<T> take(
        const base_matrix<T, Tag> &mat,
        const base_array<index_t, TagIndex> &indices
    );

    /**
     * @brief Take elements from a matrix along an axis.
     *
     * @param mat The source matrix.
     * @param indices A single index or an array-like object of size_t with the
     *     indices of the values to take.
     * @param rowwise If true, take elements along each row. Otherwise, take
     *     elements along each column.
     *
     * @return A new array if a single index is given, or a new matrix if an
     *     array of indices is given, with the elements from mat at the given
     *     positions.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    array<T> take(const base_matrix<T, Tag> &mat, size_t index, bool rowwise);

    template <class T, class Tag, class TagIndex>
    matrix<T> take(
        const base_matrix<T, Tag> &mat,
        const base_array<size_t, TagIndex> &indices, bool rowwise
    );

    /**
     * @brief Take values from the input matrix by matching 1d index and data
     * slices. Functions returning an index along an axis, like argsort and
     * argpartition, produce suitable indices for this function.
     *
     * @param mat The source matrix.
     * @param indices A matrix-like object of size_t with the indices to take
     *     along each 1d slice of mat.
     * @param rowwise If true, match indices along each row. Otherwise, match
     *     indices along each column.
     *
     * @return A new matrix with the elements from mat at the given positions.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class TagIndex>
    matrix<T> take_along_axis(
        const base_matrix<T, Tag> &mat,
        const base_matrix<size_t, TagIndex> &indices, bool rowwise
    );

    /**
     * @brief Replaces specified elements of an array with given values. When
     * an array object is given, a call such as np::put(arr, indices, values)
     * is equivalent to arr[indices] = values.
     *
     * @param arr Target array.
     * @param indices Target indices.
     * @param values A single value or an array of values to put into arr at
     *     target indices.
     *
     * @throw std::invalid_argument Thrown if indices and values have different
     *     sizes.
     */
    template <class T, class Tag, class TagIndex, class TagValue>
    void put(
        base_array<T, Tag> &arr,
        const base_array<size_t, TagIndex> &indices,
        const base_array<T, TagValue> &values
    );

    template <class T, class Tag, class TagIndex>
    void put(
        base_array<T, Tag> &arr,
        const base_array<size_t, TagIndex> &indices,
        const typename base_array<T, Tag>::value_type &value
    );

    /**
     * @brief Replaces specified elements of a matrix with given values. When a
     * matrix object is given, a call such as np::put(mat, indices, values) is
     * equivalent to mat[indices] = values.
     *
     * @param mat Target matrix.
     * @param indices Target indices.
     * @param values A single value or an array of values to put into mat at
     *     target indices.
     *
     * @throw std::invalid_argument Thrown if indices and values have different
     *     sizes.
     */
    template <class T, class Tag, class TagIndex, class TagValue>
    void put(
        base_matrix<T, Tag> &mat,
        const base_array<index_t, TagIndex> &indices,
        const base_array<T, TagValue> &values
    );

    template <class T, class Tag, class TagIndex>
    void put(
        base_matrix<T, Tag> &mat,
        const base_array<index_t, TagIndex> &indices,
        const typename base_matrix<T, Tag>::value_type &value
    );

    /**
     * @brief Put values into the destination matrix by matching 1d index and
     * data slices. Functions returning an index along an axis, like argsort
     * and argpartition, produce suitable indices for this function.
     *
     * @param mat Destination matrix.
     * @param indices A matrix-like object of size_t with the indices to change
     *     along each 1d slice of mat.
     * @param values A single value or a matrix of values to insert at those
     *     indices.
     * @param rowwise If true, match indices along each row. Otherwise, match
     *     indices along each column.
     *
     * @throw std::invalid_argument Thrown if indices and values have different
     *     number of rows and columns.
     */
    template <class T, class Tag, class TagIndex, class TagValue>
    void put_along_axis(
        base_matrix<T, Tag> &mat,
        const base_matrix<size_t, TagIndex> &indices,
        const base_matrix<T, TagValue> &values,
        bool rowwise
    );

    template <class T, class Tag, class TagIndex>
    void put_along_axis(
        base_matrix<T, Tag> &mat,
        const base_matrix<size_t, TagIndex> &indices,
        const typename base_matrix<T, Tag>::value_type &value,
        bool rowwise
    );

    /**
     * @brief Return the elements of an array that satisfy some condition. When
     * an array object is given, a call such as np::extract(arr, condition) is
     * equivalent to arr[condition], except that a copy is always returned.
     *
     * @param arr An array-like object.
     * @param condition An array-like object of bool whose true entries
     *     indicate the elements of arr to extract.
     *
     * @return A new array with the elements from arr where condition is true.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class TagCond>
    array<T> extract(
        const base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition
    );

    /**
     * @brief Return the elements of a matrix that satisfy some condition. When
     * a matrix object is given, a call such as np::extract(mat, condition) is
     * equivalent to mat[condition], except that a copy is always returned.
     *
     * @param mat A matrix-like object.
     * @param condition A matrix-like object of bool whose true entries
     *     indicate the elements of mat to extract.
     *
     * @return A new array with the elements from mat where condition is true.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class TagCond>
    array<T> extract(
        const base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition
    );

    /**
     * @brief Return the elements of a matrix that satisfy some condition along
     * given axis.
     *
     * @param mat A matrix-like object.
     * @param condition An array-like object of bool whose true entries
     *     indicate the elements of mat to extract.
     * @param rowwise If true, extract elements along each row. Otherwise,
     *     extract elements along each column.
     *
     * @return A new matrix with the elements from mat where condition is true.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag, class TagCond>
    matrix<T> extract(
        const base_matrix<T, Tag> &mat,
        const base_array<bool, TagCond> &condition, bool rowwise
    );

    /**
     * @brief Change elements of an array based on conditional and input
     * values.
     *
     * @param arr Array to put data into.
     * @param condition Boolean mask array.
     * @param values A single value or an array of values to place in arr. Only
     *     the first N elements are used, where N is the number of true values
     *     in condition.
     */
    template <class T, class Tag, class TagCond, class TagValue>
    void place(
        base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagValue> &values
    );

    template <class T, class Tag, class TagCond>
    void place(
        base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition,
        const typename base_array<T, Tag>::value_type &value
    );

    /**
     * @brief Change elements of a matrix based on conditional and input
     * values.
     *
     * @param mat Matrix to put data into.
     * @param condition Boolean mask matrix.
     * @param values A single value or an array of values to place in mat. Only
     *     the first N elements are used, where N is the number of true values
     *     in condition.
     */
    template <class T, class Tag, class TagCond, class TagValue>
    void place(
        base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition,
        const base_array<T, TagValue> &values
    );

    template <class T, class Tag, class TagCond>
    void place(
        base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition,
        const typename base_matrix<T, Tag>::value_type &value
    );

    /**
     * @brief Change elements of an array based on conditional and input
     * values. Similar to place, the difference is that place uses the first N
     * elements of values, where N is the number of true values in condition,
     * while putmask uses the elements where condition is true.
     *
     * @param arr Array to put data into.
     * @param condition Boolean mask array.
     * @param values Values to put into arr where condition is true.
     */
    template <class T, class Tag, class TagCond, class TagValue>
    void putmask(
        base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagValue> &values
    );

    /**
     * @brief Change elements of a matrix based on conditional and input
     * values. Similar to place, the difference is that place uses the first N
     * elements of values, where N is the number of true values in condition,
     * while putmask uses the elements where condition is true.
     *
     * @param mat Matrix to put data into.
     * @param condition Boolean mask matrix.
     * @param values Values to put into mat where condition is true.
     */
    template <class T, class Tag, class TagCond, class TagValue>
    void putmask(
        base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition,
        const base_matrix<T, TagValue> &values
    );

    /**
     * @brief Return an array of elements choosen from two arrays depending on
     * condition.
     *
     * @param condition An array-like object of bool, where true, yield arr1,
     *     otherwise yield arr2.
     * @param arr1 An array-like object with the values from which to choose
     *     where condition is true.
     * @param arr2 An array-like object with the values from which to choose
     *     where condition is false.
     * @param val1 Value to use as true argument. Values are broadcasted to an
     *     appropriate size.
     * @param val2 Value to use as false argument. Values are broadcasted to
     *     an appropriate size.
     *
     * @return A light-weight object with the elements choosen from arr1 where
     *     condition is true and from arr2 where condition is false.
     *     Convertible to an array object.
     *
     * @throw std::invalid_argument Thrown if condition and array arguments
     *     have different sizes.
     */
    template <class T, class TagCond, class TagTrue, class TagFalse>
    base_array< T, select_tag<TagCond, TagTrue, TagFalse> > select(
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagTrue> &arr1,
        const base_array<T, TagFalse> &arr2
    );

    template <class T, class TagCond, class TagTrue>
    base_array< T, select_tag<TagCond, TagTrue, scalar_tag> > select(
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagTrue> &arr1,
        const typename base_array<T, TagTrue>::value_type &val2
    );

    template <class T, class TagCond, class TagFalse>
    base_array< T, select_tag<TagCond, scalar_tag, TagFalse> > select(
        const base_array<bool, TagCond> &condition,
        const typename base_array<T, TagFalse>::value_type &val1,
        const base_array<T, TagFalse> &arr2
    );

    template <class T, class TagCond>
    base_array< T, select_tag<TagCond, scalar_tag, scalar_tag> > select(
        const base_array<bool, TagCond> &condition,
        const typename base_array<T, TagCond>::value_type &val1,
        const typename base_array<T, TagCond>::value_type &val2
    );

    /**
     * @brief Return a matrix of elements choosen from two matrices depending
     * on condition.
     *
     * @param condition A matrix-like object of bool, where true, yield mat1,
     *     otherwise yield mat2.
     * @param mat1 A matrix-like object with the values from which to choose
     *     where condition is true.
     * @param mat2 A matrix-like object with the values from which to choose
     *     where condition is false.
     * @param val1 Value to use as true argument. Values are broadcasted to an
     *     appropriate size.
     * @param val2 Value to use as false argument. Values are broadcasted to
     *     an appropriate size.
     *
     * @return A light-weight object with the elements choosen from mat1 where
     *     condition is true and from mat2 where condition is false.
     *     Convertible to a matrix object.
     *
     * @throw std::invalid_argument Thrown if condition and matrix arguments
     *     have different number of rows and columns.
     */
    template <class T, class TagCond, class TagTrue, class TagFalse>
    base_matrix< T, select_tag<TagCond, TagTrue, TagFalse> > select(
        const base_matrix<bool, TagCond> &condition,
        const base_matrix<T, TagTrue> &mat1,
        const base_matrix<T, TagFalse> &mat2
    );

    template <class T, class TagCond, class TagTrue>
    base_matrix< T, select_tag<TagCond, TagTrue, scalar_tag> > select(
        const base_matrix<bool, TagCond> &condition,
        const base_matrix<T, TagTrue> &mat1,
        const typename base_matrix<T, TagTrue>::value_type &val2
    );

    template <class T, class TagCond, class TagFalse>
    base_matrix< T, select_tag<TagCond, scalar_tag, TagFalse> > select(
        const base_matrix<bool, TagCond> &condition,
        const typename base_matrix<T, TagFalse>::value_type &val1,
        const base_matrix<T, TagFalse> &mat2
    );

    template <class T, class TagCond>
    base_matrix< T, select_tag<TagCond, scalar_tag, scalar_tag> > select(
        const base_matrix<bool, TagCond> &condition,
        const typename base_array<T, TagCond>::value_type &val1,
        const typename base_array<T, TagCond>::value_type &val2
    );

    /// Functional programming

    /**
     * @brief Apply a function element-wise.
     *
     * @param f The function to apply.
     * @param arr An array-like object with the values where the function will
     *     be invoked.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function on each element. This function uses lazy-evaluation,
     *     meaning that the result of each call is computed only at the end,
     *     when the whole expression is evaluated or assigned to an array
     *     object.
     */
    template <class Function, class T, class Tag>
    base_array<
        typename std::result_of<Function(T)>::type,
        lazy_unary_tag<Function, T, Tag>
    > apply(Function f, const base_array<T, Tag> &arr) {
        typedef typename std::result_of<Function(T)>::type Rt;
        typedef lazy_unary_tag<Function, T, Tag> Closure;
        return base_array<Rt, Closure>(f, arr);
    }

    /**
     * @brief Apply a function element-wise.
     *
     * @param f The function to apply.
     * @param mat A matrix-like object with the values where the function will
     *     be invoked.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function on each element. This function uses lazy-evaluation,
     *     meaning that the result of each call is computed only at the end,
     *     when the whole expression is evaluated or assigned to a matrix
     *     object.
     */
    template <class Function, class T, class Tag>
    base_matrix<
        typename std::result_of<Function(T)>::type,
        lazy_unary_tag<Function, T, Tag>
    > apply(Function f, const base_matrix<T, Tag> &mat) {
        typedef typename std::result_of<Function(T)>::type Rt;
        typedef lazy_unary_tag<Function, T, Tag> Closure;
        return base_matrix<Rt, Closure>(f, mat);
    }

    /**
     * @brief Apply a function element-wise.
     *
     * @param f The function to apply.
     * @param arr1 First array-like object.
     * @param arr2 Second array-like object.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function on each element. This function uses lazy-evaluation,
     *     meaning that the result of each call is computed only at the end,
     *     when the whole expression is evaluated or assigned to an array
     *     object.
     *
     * @throw std::invalid_argument Thrown if the array arguments have
     *     different sizes.
     */
    template <class Function, class T1, class Tag1, class T2, class Tag2>
    base_array<
        typename std::result_of<Function(T1, T2)>::type,
        lazy_binary_tag<Function, T1, Tag1, T2, Tag2>
    > apply(
        Function f,
        const base_array<T1, Tag1> &arr1,
        const base_array<T2, Tag2> &arr2
    ) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef lazy_binary_tag<Function, T1, Tag1, T2, Tag2> Closure;
        return base_array<Rt, Closure>(f, arr1, arr2);
    }

    template <class Function, class T1, class Tag, class T2>
    base_array<
        typename std::result_of<Function(T1, T2)>::type,
        lazy_binary_tag<Function, T1, Tag, T2, scalar_tag>
    > apply(Function f, const base_array<T1, Tag> &arr1, const T2 &val) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef lazy_binary_tag<Function, T1, Tag, T2, scalar_tag> Closure;
        return base_array<Rt, Closure>(f, arr1, val);
    }

    template <class Function, class T1, class T2, class Tag>
    base_array<
        typename std::result_of<Function(T1, T2)>::type,
        lazy_binary_tag<Function, T1, scalar_tag, T2, Tag>
    > apply(Function f, const T1 &val, const base_array<T2, Tag> &arr2) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef lazy_binary_tag<Function, T1, scalar_tag, T2, Tag> Closure;
        return base_array<Rt, Closure>(f, val, arr2);
    }

    /**
     * @brief Apply a function element-wise.
     *
     * @param f The function to apply.
     * @param mat1 First matrix-like object.
     * @param mat2 Second matrix-like object.
     * @param val Value to use either as first argument or second argument.
     *     Values are broadcasted to an appropriate size.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function on each element. This function uses lazy-evaluation,
     *     meaning that the result of each call is computed only at the end,
     *     when the whole expression is evaluated or assigned to a matrix
     *     object.
     *
     * @throw std::invalid_argument Thrown if the matrix arguments have
     *     different number of rows and columns.
     */
    template <class Function, class T1, class Tag1, class T2, class Tag2>
    base_matrix<
        typename std::result_of<Function(T1, T2)>::type,
        lazy_binary_tag<Function, T1, Tag1, T2, Tag2>
    > apply(
        Function f,
        const base_matrix<T1, Tag1> &mat1,
        const base_matrix<T2, Tag2> &mat2
    ) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef lazy_binary_tag<Function, T1, Tag1, T2, Tag2> Closure;
        return base_matrix<Rt, Closure>(f, mat1, mat2);
    }

    template <class Function, class T1, class Tag, class T2>
    base_matrix<
        typename std::result_of<Function(T1, T2)>::type,
        lazy_binary_tag<Function, T1, Tag, T2, scalar_tag>
    > apply(Function f, const base_matrix<T1, Tag> &mat1, const T2 &val) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef lazy_binary_tag<Function, T1, Tag, T2, scalar_tag> Closure;
        return base_matrix<Rt, Closure>(f, mat1, val);
    }

    template <class Function, class T1, class T2, class Tag>
    base_matrix<
        typename std::result_of<Function(T1, T2)>::type,
        lazy_binary_tag<Function, T1, scalar_tag, T2, Tag>
    > apply(Function f, const T1 &val, const base_matrix<T2, Tag> &mat2) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef lazy_binary_tag<Function, T1, scalar_tag, T2, Tag> Closure;
        return base_matrix<Rt, Closure>(f, val, mat2);
    }

    /**
     * @brief Apply a function along an axis.
     *
     * @param f The function to apply. A function that accepts two iterators as
     *     arguments and returns a value. The iterators indicates the initial
     *     position and final position of a range. The range used is
     *     [first, last), which includes all the elements between first and
     *     last, including the element pointed by first but not the element
     *     pointed by last.
     * @param mat A matrix-like object with the values where the function will
     *     be invoked.
     * @param rowwise If true, invoke the function along each row. Otherwise,
     *     invoke the function along each column.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function along an axis. This function uses lazy-evaluation,
     *     meaning that the result of each call is computed only at the end,
     *     when the whole expression is evaluated or assigned to an array
     *     object.
     */
    template <class Function, class T, class Tag>
    base_array<
        typename std::result_of<Function(
            typename base_matrix<T, Tag>::const_iterator,
            typename base_matrix<T, Tag>::const_iterator
        )>::type,
        lazy_axis_tag<Function, T, Tag>
    > apply_along_axis(
        Function f, const base_matrix<T, Tag> &mat, bool rowwise
    ) {
        typedef typename std::result_of<Function(
            typename base_matrix<T, Tag>::const_iterator,
            typename base_matrix<T, Tag>::const_iterator
        )>::type Rt;
        typedef lazy_axis_tag<Function, T, Tag> Closure;
        return base_array<Rt, Closure>(f, mat, rowwise);
    }

    /**
     * @brief Reduce the array's dimension by applying a function to all
     * elements.
     *
     * @param f Binary function taking two elements as arguments and returning
     *     the result of the accumulated operation.
     * @param arr An array-like object.
     *
     * @return The result of accumulating all the elements in the array.
     *
     * @throw std::invalid_argument Thrown if the array is empty and f does not
     *     have an identity property.
     */
    template <class Function, class T, class Tag>
    T reduce(Function f, const base_array<T, Tag> &arr);

    /**
     * @brief Reduce the matrix's dimension by applying a function to all
     * elements.
     *
     * @param f Binary function taking two elements as arguments and returning
     *     the result of the accumulated operation.
     * @param mat A matrix-like object.
     *
     * @return The result of accumulating all the elements in the matrix.
     *
     * @throw std::invalid_argument Thrown if the matrix is empty and f does
     *     not have an identity property.
     */
    template <class Function, class T, class Tag>
    T reduce(Function f, const base_matrix<T, Tag> &mat);

    /**
     * @brief Reduce the matrix's dimension by one, by applying a function
     * along an axis.
     *
     * @param f Binary function taking two elements as arguments and returning
     *     the result of the accumulated operation.
     * @param mat A matrix-like object.
     * @param rowwise If true, accumulate the function along each row.
     *     Otherwise, accumulate the function along each column.
     *
     * @return The result of accumulating the elements along an axis.
     *
     * @throw std::invalid_argument Thrown if the axis is empty and f does not
     *     have an identity property.
     */
    template <class Function, class T, class Tag>
    base_array< T, lazy_axis_tag<__range_reduce<Function>, T, Tag> >
    reduce(Function f, const base_matrix<T, Tag> &mat, bool rowwise);

    /**
     * @brief Accumulate the result of applying a function to all elements.
     *
     * @param f Binary function taking two elements as arguments and returning
     *     the result of the accumulated operation.
     * @param arr An array-like object.
     *
     * @return A new array with the accumulated results of applying a function
     *     to the array elements.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Function, class T, class Tag>
    array<T> accumulate(Function f, const base_array<T, Tag> &arr);

    /**
     * @brief Accumulate the result of applying a function to all elements.
     *
     * @param f Binary function taking two elements as arguments and returning
     *     the result of the accumulated operation.
     * @param mat A matrix-like object.
     * @param rowwise If true, accumulate the result along each row. Otherwise,
     *     accumulate the result along each column.
     *
     * @return A new matrix with the accumulated results of applying a function
     *     along an axis.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class Function, class T, class Tag>
    matrix<T> accumulate(
        Function f, const base_matrix<T, Tag> &mat, bool rowwise
    );

    /**
     * @brief Apply a function to all pairs (a, b) with a in arr1 and b in
     * arr2.
     *
     * @param f The function to apply.
     * @param arr1 First array-like object.
     * @param arr2 Second array-like object.
     *
     * @return A light-weight object which stores the result of invoking the
     *     function to all pairs of elements. This function uses
     *     lazy-evaluation, meaning that the result of each call is computed
     *     only at the end, when the whole expression is evaluated or assigned
     *     to a matrix object.
     * >
     */
    template <class Function, class T1, class Tag1, class T2, class Tag2>
    base_matrix<
        typename std::result_of<Function(T1, T2)>::type,
        outer_tag<Function, T1, Tag1, T2, Tag2>
    > outer(
        Function f,
        const base_array<T1, Tag1> &arr1,
        const base_array<T2, Tag2> &arr2
    ) {
        typedef typename std::result_of<Function(T1, T2)>::type Rt;
        typedef outer_tag<Function, T1, Tag1, T2, Tag2> Closure;
        return base_matrix<Rt, Closure>(f, arr1, arr2);
    }
}

#include "numcpp/broadcasting/broadcasting.tcc"

#endif // NUMCPP_BROADCASTING_H_INCLUDED
