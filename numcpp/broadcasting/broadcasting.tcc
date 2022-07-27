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

 /** @file include/numcpp/broadcasting/broadcasting.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_BROADCASTING_TCC_INCLUDED
#define NUMCPP_BROADCASTING_TCC_INCLUDED

namespace numcpp {
    /// Broadcasting

    template <class T>
    const array_view<T> broadcast_to(T &val, size_t n) {
        size_t offset = 0, stride = 0;
        return array_view<T>(n, &val, offset, stride);
    }

    template <class T>
    const matrix_view<T> broadcast_to(T &val, size_t m, size_t n) {
        size_t offset = 0, stride1 = 0, stride2 = 0;
        return matrix_view<T>(m, n, &val, offset, stride1, stride2);
    }

    template <class T>
    const matrix_view<T> broadcast_to(
        array<T> &arr, size_t reps, bool rowwise
    ) {
        size_t m, n;
        size_t offset = 0, stride1 = 0, stride2 = 0;
        if (rowwise) {
            m = reps;
            n = arr.size();
            stride2 = 1;
        }
        else {
            m = arr.size();
            n = reps;
            stride1 = 1;
        }
        return matrix_view<T>(m, n, arr.data(), offset, stride1, stride2);
    }

    template <class T>
    const matrix_view<T> broadcast_to(
        array_view<T> &arr, size_t reps, bool rowwise
    ) {
        size_t m, n;
        size_t offset = arr.offset(), stride1 = 0, stride2 = 0;
        if (rowwise) {
            m = reps;
            n = arr.size();
            stride2 = arr.stride();
        }
        else {
            m = arr.size();
            n = reps;
            stride1 = arr.stride();
        }
        return matrix_view<T>(m, n, arr.data(), offset, stride1, stride2);
    }

    /// Creation routines from existing data

    template <class Container, class T>
    array<T> asarray(const Container &data) {
        return array<T>(data.begin(), data.end());
    }

    template <class T>
    array<T> asarray(std::initializer_list<T> data) {
        return array<T>(data);
    }

    template <class T, size_t N>
    array<T> asarray(const T (&data)[N]) {
        return array<T>(data, data + N);
    }

    template <class T>
    array<T>& asarray(array<T> &data) {
        return data;
    }

    template <class Container, class T>
    matrix<T> asmatrix(const Container &data, size_t n) {
        return matrix<T>(data.begin(), data.end(), n);
    }

    template <class T>
    matrix<T> asmatrix(
        std::initializer_list< std::initializer_list<T> > data, size_t
    ) {
        return matrix<T>(data);
    }

    template <class T, size_t M, size_t N>
    matrix<T> asmatrix(const T (&data)[M][N], size_t) {
        return matrix<T>(*data, *data + M * N, N);
    }

    template <class T>
    matrix<T>& asmatrix(matrix<T> &data, size_t) {
        return data;
    }

    template <class T, class Tag>
    array<T> copy(const base_array<T, Tag> &arr) {
        return array<T>(arr);
    }

    template <class T, class Tag>
    matrix<T> copy(const base_matrix<T, Tag> &mat) {
        return matrix<T>(mat);
    }

    template <class T, class Tag>
    array<T> flatten(const base_matrix<T, Tag> &mat) {
        return array<T>(mat.begin(), mat.end());
    }

    template <class T, class Tag>
    array<T> flatten(const base_matrix<T, Tag> &mat, bool row_major) {
        return array<T>(mat.begin(row_major), mat.end(row_major));
    }

    /// Basic manipulation routines

    template <class U, class T, class Tag>
    base_array< U, lazy_unary_tag<__identity, T, Tag> >
    astype(const base_array<T, Tag> &arr) {
        typedef lazy_unary_tag<__identity, T, Tag> Closure;
        return base_array<U, Closure>(__identity(), arr);
    }

    template <class U, class T, class Tag>
    base_matrix< U, lazy_unary_tag<__identity, T, Tag> >
    astype(const base_matrix<T, Tag> &mat) {
        typedef lazy_unary_tag<__identity, T, Tag> Closure;
        return base_matrix<U, Closure>(__identity(), mat);
    }

    template <class T, class Tag1, class U, class Tag2>
    base_array< std::pair<T, U>, lazy_binary_tag<__zip, T, Tag1, U, Tag2> >
    zip(const base_array<T, Tag1> &arr1, const base_array<U, Tag2> &arr2) {
        typedef std::pair<T, U> Rt;
        typedef lazy_binary_tag<__zip, T, Tag1, U, Tag2> Closure;
        return base_array<Rt, Closure>(__zip(), arr1, arr2);
    }

    template <class T, class Tag1, class U, class Tag2>
    base_matrix< std::pair<T, U>, lazy_binary_tag<__zip, T, Tag1, U, Tag2> >
    zip(const base_matrix<T, Tag1> &mat1, const base_matrix<U, Tag2> &mat2) {
        typedef std::pair<T, U> Rt;
        typedef lazy_binary_tag<__zip, T, Tag1, U, Tag2> Closure;
        return base_matrix<Rt, Closure>(__zip(), mat1, mat2);
    }

    template <size_t I, class T, class U, class Tag>
    base_array<
        typename std::tuple_element< I, std::pair<T, U> >::type,
        lazy_unary_tag<__unzip<I>, std::pair<T, U>, Tag>
    >
    unzip(const base_array<std::pair<T, U>, Tag> &arr) {
        typedef typename std::tuple_element< I, std::pair<T, U> >::type Rt;
        typedef lazy_unary_tag<__unzip<I>, std::pair<T, U>, Tag> Closure;
        return base_array<Rt, Closure>(__unzip<I>(), arr);
    }

    template <size_t I, class T, class U, class Tag>
    base_matrix<
        typename std::tuple_element< I, std::pair<T, U> >::type,
        lazy_unary_tag<__unzip<I>, std::pair<T, U>, Tag>
    >
    unzip(const base_matrix<std::pair<T, U>, Tag> &mat) {
        typedef typename std::tuple_element< I, std::pair<T, U> >::type Rt;
        typedef lazy_unary_tag<__unzip<I>, std::pair<T, U>, Tag> Closure;
        return base_matrix<Rt, Closure>(__unzip<I>(), mat);
    }

    template <class T, class Tag>
    base_array< T, reverse_tag<Tag> > reverse(const base_array<T, Tag> &arr) {
        typedef reverse_tag<Tag> Closure;
        return base_array<T, Closure>(arr);
    }

    template <class T, class Tag>
    base_matrix< T, reverse_tag<Tag> > reverse(
        const base_matrix<T, Tag> &mat, bool rowwise
    ) {
        typedef reverse_tag<Tag> Closure;
        return base_matrix<T, Closure>(mat, rowwise);
    }

    template <class T, class Tag>
    base_matrix< T, rotate_tag<Tag> > rot90(
        const base_matrix<T, Tag> &mat, int times
    ) {
        typedef rotate_tag<Tag> Closure;
        return base_matrix<T, Closure>(mat, times);
    }

    /// Indexing routines

    size_t ravel_multi_index(
        size_t i, size_t j,
        size_t m, size_t n, bool row_major
    ) {
        __ravel_multi_index pred(m, n, row_major);
        return pred(i, j);
    }

    size_t ravel_multi_index(
        const index_t &multi_index,
        size_t m, size_t n, bool row_major
    ) {
        __ravel_multi_index pred(m, n, row_major);
        return pred(multi_index);
    }

    template <class Tag>
    base_array< size_t, lazy_unary_tag<__ravel_multi_index, index_t, Tag> >
    ravel_multi_index(
        const base_array<index_t, Tag> &multi_index,
        size_t m, size_t n, bool row_major
    ) {
        typedef lazy_unary_tag<__ravel_multi_index, index_t, Tag> Closure;
        return base_array<size_t, Closure>(
            __ravel_multi_index(m, n, row_major), multi_index
        );
    }

    index_t unravel_index(
        size_t index,
        size_t m, size_t n, bool row_major
    ) {
        __unravel_index pred(m, n, row_major);
        return pred(index);
    }

    template <class Tag>
    base_array< index_t, lazy_unary_tag<__unravel_index, size_t, Tag> >
    unravel_index(
        const base_array<size_t, Tag> &index,
        size_t m, size_t n, bool row_major
    ) {
        typedef lazy_unary_tag<__unravel_index, size_t, Tag> Closure;
        return base_array<index_t, Closure>(
            __unravel_index(m, n, row_major), index
        );
    }

    template <class T, class Tag, class TagIndex>
    array<T> take(
        const base_array<T, Tag> &arr,
        const base_array<size_t, TagIndex> &indices
    ) {
        array<T> out(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            out[i] = arr[indices[i]];
        }
        return out;
    }

    template <class T, class Tag, class TagIndex>
    array<T> take(
        const base_matrix<T, Tag> &mat,
        const base_array<index_t, TagIndex> &indices
    ) {
        array<T> out(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            out[i] = mat(indices[i].first, indices[i].second);
        }
        return out;
    }

    template <class T, class Tag, class TagIndex>
    matrix<T> take(
        const base_matrix<T, Tag> &mat,
        const base_array<size_t, TagIndex> &indices, bool rowwise
    ) {
        if (rowwise) {
            matrix<T> out(mat.rows(), indices.size());
            for (size_t i = 0; i < mat.rows(); ++i) {
                for (size_t j = 0; j < indices.size(); ++j) {
                    out(i, j) = mat(i, indices[j]);
                }
            }
            return out;
        }
        else {
            matrix<T> out(indices.size(), mat.cols());
            for (size_t i = 0; i < indices.size(); ++i) {
                for (size_t j = 0; j < mat.cols(); ++j) {
                    out(i, j) = mat(indices[i], j);
                }
            }
            return out;
        }
    }

    template <class T, class Tag, class TagIndex>
    matrix<T> take_along_axis(
        const base_matrix<T, Tag> &mat,
        const base_matrix<size_t, TagIndex> &indices, bool rowwise
    ) {
        if (rowwise) {
            __assert_equal_length(mat.rows(), indices.rows());
            matrix<T> out(indices.rows(), indices.cols());
            for (size_t i = 0; i < indices.rows(); ++i) {
                for (size_t j = 0; j < indices.cols(); ++j) {
                    out(i, j) = mat(i, indices(i, j));
                }
            }
            return out;
        }
        else {
            __assert_equal_length(mat.cols(), indices.cols());
            matrix<T> out(indices.rows(), indices.cols());
            for (size_t i = 0; i < indices.rows(); ++i) {
                for (size_t j = 0; j < indices.cols(); ++j) {
                    out(i, j) = mat(indices(i, j), j);
                }
            }
            return out;
        }
    }

    template <class T, class Tag, class TagIndex, class TagValue>
    void put(
        base_array<T, Tag> &arr,
        const base_array<size_t, TagIndex> &indices,
        const base_array<T, TagValue> &values
    ) {
        __assert_equal_length(indices.size(), values.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            arr[indices[i]] = values[i];
        }
    }

    template <class T, class Tag, class TagIndex>
    void put(
        base_array<T, Tag> &arr,
        const base_array<size_t, TagIndex> &indices,
        const typename base_array<T, Tag>::value_type &value
    ) {
        for (size_t i = 0; i < indices.size(); ++i) {
            arr[indices[i]] = value;
        }
    }

    template <class T, class Tag, class TagIndex, class TagValue>
    void put(
        base_matrix<T, Tag> &mat,
        const base_array<index_t, TagIndex> &indices,
        const base_array<T, TagValue> &values
    ) {
        __assert_equal_length(indices.size(), values.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            mat(indices[i].first, indices[i].second) = values[i];
        }
    }

    template <class T, class Tag, class TagIndex>
    void put(
        base_matrix<T, Tag> &mat,
        const base_array<index_t, TagIndex> &indices,
        const typename base_matrix<T, Tag>::value_type &value
    ) {
        for (size_t i = 0; i < indices.size(); ++i) {
            mat(indices[i].first, indices[i].second) = value;
        }
    }

    template <class T, class Tag, class TagIndex, class TagValue>
    void put_along_axis(
        base_matrix<T, Tag> &mat,
        const base_matrix<size_t, TagIndex> &indices,
        const base_matrix<T, TagValue> &values,
        bool rowwise
    ) {
        __assert_equal_shape(
            indices.rows(), indices.cols(), values.rows(), values.cols()
        );
        if (rowwise) {
            __assert_equal_length(mat.rows(), indices.rows());
            for (size_t i = 0; i < indices.rows(); ++i) {
                for (size_t j = 0; j < indices.cols(); ++j) {
                    mat(i, indices(i, j)) = values(i, j);
                }
            }
        }
        else {
            __assert_equal_length(mat.cols(), indices.cols());
            for (size_t i = 0; i < indices.rows(); ++i) {
                for (size_t j = 0; j < indices.cols(); ++j) {
                    mat(indices(i, j), j) = values(i, j);
                }
            }
        }
    }

    template <class T, class Tag, class TagIndex>
    void put_along_axis(
        base_matrix<T, Tag> &mat,
        const base_matrix<size_t, TagIndex> &indices,
        const typename base_matrix<T, Tag>::value_type &value,
        bool rowwise
    ) {
        if (rowwise) {
            __assert_equal_length(mat.rows(), indices.rows());
            for (size_t i = 0; i < indices.rows(); ++i) {
                for (size_t j = 0; j < indices.cols(); ++j) {
                    mat(i, indices(i, j)) = value;
                }
            }
        }
        else {
            __assert_equal_length(mat.cols(), indices.cols());
            for (size_t i = 0; i < indices.rows(); ++i) {
                for (size_t j = 0; j < indices.cols(); ++j) {
                    mat(indices(i, j), j) = value;
                }
            }
        }
    }

    template <class T, class Tag, class TagCond>
    array<T> extract(
        const base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition
    ) {
        array<T> out(std::count(condition.begin(), condition.end(), true));
        size_t n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                out[n++] = arr[i];
            }
        }
        return out;
    }

    template <class T, class Tag, class TagCond>
    array<T> extract(
        const base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition
    ) {
        array<T> out(std::count(condition.begin(), condition.end(), true));
        size_t n = 0;
        for (size_t i = 0; i < condition.rows(); ++i) {
            for (size_t j = 0; j < condition.cols(); ++j) {
                if (condition(i, j)) {
                    out[n++] = mat(i, j);
                }
            }
        }
        return out;
    }

    template <class T, class Tag, class TagCond>
    matrix<T> extract(
        const base_matrix<T, Tag> &mat,
        const base_array<bool, TagCond> &condition, bool rowwise
    ) {
        size_t size = std::count(condition.begin(), condition.end(), true);
        if (rowwise) {
            matrix<T> out(mat.rows(), size);
            size_t n = 0;
            for (size_t j = 0; j < condition.size(); ++j) {
                if (condition[j]) {
                    for (size_t i = 0; i < mat.rows(); ++i) {
                        out(i, n) = mat(i, j);
                    }
                    ++n;
                }
            }
            return out;
        }
        else {
            matrix<T> out(size, mat.cols());
            size_t n = 0;
            for (size_t i = 0; i < condition.size(); ++i) {
                if (condition[i]) {
                    for (size_t j = 0; j < mat.cols(); ++j) {
                        out(n, j) = mat(i, j);
                    }
                    ++n;
                }
            }
            return out;
        }
    }

    template <class T, class Tag, class TagCond, class TagValue>
    void place(
        base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagValue> &values
    ) {
        size_t n = 0;
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                arr[i] = values[n++];
            }
        }
    }

    template <class T, class Tag, class TagCond>
    void place(
        base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition,
        const typename base_array<T, Tag>::value_type &value
    ) {
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                arr[i] = value;
            }
        }
    }

    template <class T, class Tag, class TagCond, class TagValue>
    void place(
        base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition,
        const base_array<T, TagValue> &values
    ) {
        size_t n = 0;
        for (size_t i = 0; i < condition.rows(); ++i) {
            for (size_t j = 0; j < condition.cols(); ++j) {
                if (condition(i, j)) {
                    mat(i, j) = values[n++];
                }
            }
        }
    }

    template <class T, class Tag, class TagCond>
    void place(
        base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition,
        const typename base_matrix<T, Tag>::value_type &value
    ) {
        for (size_t i = 0; i < condition.rows(); ++i) {
            for (size_t j = 0; j < condition.cols(); ++j) {
                if (condition(i, j)) {
                    mat(i, j) = value;
                }
            }
        }
    }

    template <class T, class Tag, class TagCond, class TagValue>
    void putmask(
        base_array<T, Tag> &arr,
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagValue> &values
    ) {
        for (size_t i = 0; i < condition.size(); ++i) {
            if (condition[i]) {
                arr[i] = values[i];
            }
        }
    }

    template <class T, class Tag, class TagCond, class TagValue>
    void putmask(
        base_matrix<T, Tag> &mat,
        const base_matrix<bool, TagCond> &condition,
        const base_matrix<T, TagValue> &values
    ) {
        for (size_t i = 0; i < condition.rows(); ++i) {
            for (size_t j = 0; j < condition.cols(); ++j) {
                if (condition(i, j)) {
                    mat(i, j) = values(i, j);
                }
            }
        }
    }

    template <class T, class TagCond, class TagTrue, class TagFalse>
    base_array< T, select_tag<TagCond, TagTrue, TagFalse> > select(
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagTrue> &arr1,
        const base_array<T, TagFalse> &arr2
    ) {
        typedef select_tag<TagCond, TagTrue, TagFalse> Closure;
        return base_array<T, Closure>(condition, arr1, arr2);
    }

    template <class T, class TagCond, class TagTrue>
    base_array< T, select_tag<TagCond, TagTrue, scalar_tag> > select(
        const base_array<bool, TagCond> &condition,
        const base_array<T, TagTrue> &arr1,
        const typename base_array<T, TagTrue>::value_type &val2
    ) {
        typedef select_tag<TagCond, TagTrue, scalar_tag> Closure;
        return base_array<T, Closure>(condition, arr1, val2);
    }

    template <class T, class TagCond, class TagFalse>
    base_array< T, select_tag<TagCond, scalar_tag, TagFalse> > select(
        const base_array<bool, TagCond> &condition,
        const typename base_array<T, TagFalse>::value_type &val1,
        const base_array<T, TagFalse> &arr2
    ) {
        typedef select_tag<TagCond, scalar_tag, TagFalse> Closure;
        return base_array<T, Closure>(condition, val1, arr2);
    }

    template <class T, class TagCond>
    base_array< T, select_tag<TagCond, scalar_tag, scalar_tag> > select(
        const base_array<bool, TagCond> &condition, const T &val1, const T &val2
    ) {
        typedef select_tag<TagCond, scalar_tag, scalar_tag> Closure;
        return base_array<T, Closure>(condition, val1, val2);
    }

    template <class T, class TagCond, class TagTrue, class TagFalse>
    base_matrix< T, select_tag<TagCond, TagTrue, TagFalse> > select(
        const base_matrix<bool, TagCond> &condition,
        const base_matrix<T, TagTrue> &mat1,
        const base_matrix<T, TagFalse> &mat2
    ) {
        typedef select_tag<TagCond, TagTrue, TagFalse> Closure;
        return base_matrix<T, Closure>(condition, mat1, mat2);
    }

    template <class T, class TagCond, class TagTrue>
    base_matrix< T, select_tag<TagCond, TagTrue, scalar_tag> > select(
        const base_matrix<bool, TagCond> &condition,
        const base_matrix<T, TagTrue> &mat1,
        const typename base_matrix<T, TagTrue>::value_type &val2
    ) {
        typedef select_tag<TagCond, TagTrue, scalar_tag> Closure;
        return base_matrix<T, Closure>(condition, mat1, val2);
    }

    template <class T, class TagCond, class TagFalse>
    base_matrix< T, select_tag<TagCond, scalar_tag, TagFalse> > select(
        const base_matrix<bool, TagCond> &condition,
        const typename base_matrix<T, TagFalse>::value_type &val1,
        const base_matrix<T, TagFalse> &mat2
    ) {
        typedef select_tag<TagCond, scalar_tag, TagFalse> Closure;
        return base_matrix<T, Closure>(condition, val1, mat2);
    }

    template <class T, class TagCond>
    base_matrix< T, select_tag<TagCond, scalar_tag, scalar_tag> > select(
        const base_matrix<bool, TagCond> &condition,
        const T &val1, const T &val2
    ) {
        typedef select_tag<TagCond, scalar_tag, scalar_tag> Closure;
        return base_matrix<T, Closure>(condition, val1, val2);
    }

    /// Functional programming

    template <class Function, class T, class Tag>
    T reduce(Function f, const base_array<T, Tag> &arr) {
        __range_reduce<Function> pred(f);
        return pred(arr.begin(), arr.end());
    }

    template <class Function, class T, class Tag>
    T reduce(Function f, const base_matrix<T, Tag> &mat) {
        __range_reduce<Function> pred(f);
        return pred(mat.begin(), mat.end());
    }

    template <class Function, class T, class Tag>
    base_array< T, lazy_axis_tag<__range_reduce<Function>, T, Tag> >
    reduce(Function f, const base_matrix<T, Tag> &mat, bool rowwise) {
        typedef lazy_axis_tag<__range_reduce<Function>, T, Tag> Closure;
        return base_array<T, Closure>(__range_reduce<Function>(f),mat,rowwise);
    }

    template <class Function, class T, class Tag>
    array<T> accumulate(Function f, const base_array<T, Tag> &arr) {
        array<T> out(arr.size());
        __accumulate(arr.begin(), arr.end(), out.begin(), f);
        return out;
    }

    template <class Function, class T, class Tag>
    matrix<T> accumulate(
        Function f, const base_matrix<T, Tag> &mat, bool rowwise
    ) {
        matrix<T> out(mat.rows(), mat.cols());
        size_t size = rowwise ? mat.rows() : mat.cols();
        size_t tda = rowwise ? mat.cols() : mat.rows();
        for (size_t i = 0; i < size; ++i) {
            __accumulate(
                mat.begin(rowwise) + i*tda, mat.begin(rowwise) + (i + 1)*tda,
                out.begin(rowwise) + i*tda,
                f
            );
        }
        return out;
    }
}

#endif // NUMCPP_BROADCASTING_TCC_INCLUDED
