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

 /** @file include/numcpp/linalg/linalg.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/linalg.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LINALG_TCC_INCLUDED
#define NUMCPP_LINALG_TCC_INCLUDED

namespace numcpp {
    /// Basic linear algebra.

namespace detail {
    /**
     * @brief Asserts that the shapes are aligned along the given axis.
     */
    template <size_t Rank1, size_t Rank2>
    void assert_aligned_shapes(
        const shape_t<Rank1> &shape1, size_t axis1,
        const shape_t<Rank2> &shape2, size_t axis2
    ) {
        if (shape1[axis1] != shape2[axis2]) {
            std::ostringstream error;
            error << "shapes " << shape1 << " and " << shape2
                  << " not aligned: " << shape1[axis1] << " (dim " << axis1
                  << ") != " << shape2[axis2] << " (dim " << axis2 << ")";
            throw std::invalid_argument(error.str());
        }
    }

    /**
     * @brief Broadcast two shapes for cross product.
     */
    template <size_t Rank>
    shape_t<Rank> broadcast_cross(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2, size_t axis
    ) {
        if (shape1[axis] != 3 || shape2[axis] != 3) {
            char error[] = "incompatible dimensions for cross product"
                " (dimension must be 3)";
            throw std::invalid_argument(error);
        }
        shape_t<Rank> out_shape = shape1;
        for (size_t i = 0; i < shape1.ndim(); ++i) {
            if (i == axis) {
                out_shape[i] = 3;
            }
            else if (shape1[i] == 1) {
                out_shape[i] = shape2[i];
            }
            else if (shape2[i] != shape1[i] && shape2[i] != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shapes "
                      << shape1 << " " << shape2;
                throw std::invalid_argument(error.str());
            }
        }
        return out_shape;
    }

    /**
     * @brief Broadcast two shapes for matrix multiplication.
     */
    template <size_t Rank>
    shape_t<Rank> broadcast_matmul(
        const shape_t<Rank> &shape1, const shape_t<Rank> &shape2
    ) {
        shape_t<Rank> out_shape = shape1;
        for (size_t i = 0; i < shape1.ndim() - 2; ++i) {
            if (shape1[i] == 1) {
                out_shape[i] = shape2[i];
            }
            else if (shape2[i] != shape1[i] && shape2[i] != 1) {
                std::ostringstream error;
                error << "operands could not be broadcast together with shapes "
                      << shape1 << " " << shape2;
                throw std::invalid_argument(error.str());
            }
        }
        out_shape[shape1.ndim() - 2] = shape1[shape1.ndim() - 2];
        out_shape[shape2.ndim() - 1] = shape2[shape2.ndim() - 1];
        return out_shape;
    }
}

    template <class T, class Tag1, class Tag2>
    T dot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t n = a.size();
        T val = T(0);
        for (size_t i = 0; i < n; ++i) {
            val += a[i] * b[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    T vdot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t n = a.size();
        T val = T(0);
        for (size_t i = 0; i < n; ++i) {
            val += a[i] * b[i];
        }
        return val;
    }

    template <class T, class Tag1, class Tag2>
    std::complex<T> vdot(
        const base_tensor<std::complex<T>, 1, Tag1> &a,
        const base_tensor<std::complex<T>, 1, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t n = a.size();
        std::complex<T> val = T(0);
        for (size_t i = 0; i < n; ++i) {
            val += std::conj(a[i]) * b[i];
        }
        return val;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> cross(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        size_t axis
    ) {
        shape_t<Rank> shape = detail::broadcast_cross(a.shape(),b.shape(),axis);
        tensor<T, Rank> out(shape);
        shape[axis] = 1;
        for (index_t<Rank> index : make_indices(shape)) {
            T u[3], v[3];
            for (index[axis] = 0; index[axis] < 3; ++index[axis]) {
                u[index[axis]] = a[detail::broadcast_index(index, a.shape())];
                v[index[axis]] = b[detail::broadcast_index(index, b.shape())];
            }
            T v_out[3] = {u[1] * v[2] - v[1] * u[2],
                          v[0] * u[2] - u[0] * v[2],
                          u[0] * v[1] - v[0] * u[1]};
            for (index[axis] = 0; index[axis] < 3; ++index[axis]) {
                out[index] = v_out[index[axis]];
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    inline T matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    ) {
        return dot(a, b);
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 2> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 1, b.shape(), 0);
        size_t m = a.shape(0), p = a.shape(1), n = b.shape(1);
        tensor<T, 2> out(m, n);
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                T val = T(0);
                for (size_t k = 0; k < p; ++k) {
                    val += a(i, k) * b(k, j);
                }
                out(i, j) = val;
            }
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 0, b.shape(), 0);
        size_t m = b.shape(0), n = b.shape(1);
        tensor<T, 1> out(n);
        for (size_t j = 0; j < n; ++j) {
            T val = T(0);
            for (size_t i = 0; i < m; ++i) {
                val += a[i] * b(i, j);
            }
            out[j] = val;
        }
        return out;
    }

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    ) {
        detail::assert_aligned_shapes(a.shape(), 1, b.shape(), 0);
        size_t m = a.shape(0), n = a.shape(1);
        tensor<T, 1> out(m);
        for (size_t i = 0; i < m; ++i) {
            T val = T(0);
            for (size_t j = 0; j < n; ++j) {
                val += a(i, j) * b[j];
            }
            out[i] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
        shape_t<Rank> shape = detail::broadcast_matmul(a.shape(), b.shape());
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index =
                detail::broadcast_index(out_index, a.shape());
            index_t<Rank> b_index =
                detail::broadcast_index(out_index, b.shape());
            T val = T(0);
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, 2, Tag2> &b
    ) {
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
        shape_t<Rank> shape = a.shape();
        shape[axis1] = b.shape(1);
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank> a_index = out_index;
            index_t<2> b_index(0, out_index[out_index.ndim() - 1]);
            T val = T(0);
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, 2, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    ) {
        size_t axis1 = a.ndim() - 1, axis2 = b.ndim() - 2;
        detail::assert_aligned_shapes(a.shape(), axis1, b.shape(), axis2);
        shape_t<Rank> shape = b.shape();
        shape[axis2] = a.shape(0);
        size_t n = a.shape(axis1);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<2> a_index(out_index[out_index.ndim() - 2], 0);
            index_t<Rank> b_index = out_index;
            T val = T(0);
            for (a_index[axis1] = 0; a_index[axis1] < n; ++a_index[axis1]) {
                b_index[axis2] = a_index[axis1];
                val += a[a_index] * b[b_index];
            }
            out[out_index] = val;
        }
        return out;
    }

namespace detail {
    /**
     * @brief Asserts that the shapes are aligned along the given axes.
     * Return the size of the flattened dimensions if assertion doesn't fail.
     */
    template <size_t Rank1, size_t Rank2, size_t N>
    size_t assert_aligned_shapes(
        const shape_t<Rank1> &shape1, const shape_t<N> axes1,
        const shape_t<Rank2> &shape2, const shape_t<N> axes2
    ) {
        size_t size = 1;
        for (size_t i = 0; i < axes1.ndim(); ++i) {
            assert_aligned_shapes(shape1, axes1[i], shape2, axes2[i]);
            size *= shape1[axes1[i]];
        }
        return size;
    }

    /**
     * @brief Constructs the output shape of tensordot. Extract the
     * non-contracted axes of a shape and append them at the end of another
     * shape.
     */
    template <size_t OutRank, size_t Rank, size_t N>
    size_t tensordot_concat_shape(
        shape_t<OutRank> &out_shape,
        const shape_t<Rank> &shape, const shape_t<N> &axes,
        size_t offset = 0
    ) {
        bool keep_axis[Rank];
        std::fill_n(keep_axis, shape.ndim(), true);
        for (size_t i = 0; i < axes.ndim(); ++i) {
            keep_axis[axes[i]] = false;
        }
        for (size_t i = 0; i < shape.ndim(); ++i) {
            if (keep_axis[i]) {
                out_shape[offset++] = shape[i];
            }
        }
        return offset;
    }

    /**
     * @brief Split the output index of tensordot into suitable indices for its
     * arguments.
     */
    template <size_t OutRank, size_t Rank, size_t N>
    size_t tensordot_split_index(
        const index_t<OutRank> &out_index,
        index_t<Rank> &index, const shape_t<N> &axes,
        size_t offset = 0
    ) {
        bool keep_axis[Rank];
        std::fill_n(keep_axis, index.ndim(), true);
        for (size_t i = 0; i < axes.ndim(); ++i) {
            keep_axis[axes[i]] = false;
        }
        for (size_t i = 0; i < index.ndim(); ++i) {
            if (keep_axis[i]) {
                index[i] = out_index[offset++];
            }
        }
        return offset;
    }
}

    template <class T, size_t Rank, class Tag1, class Tag2>
    T tensordot(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const shape_t<Rank> &a_axes,
        const shape_t<Rank> &b_axes
    ) {
        detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
        size_t size = a.size();
        index_t<Rank> index;
        auto first1 = make_const_axes_iterator(&a, index, a_axes, 0);
        auto last1 = make_const_axes_iterator(&a, index, a_axes, size);
        auto first2 = make_const_axes_iterator(&b, index, b_axes, 0);
        return std::inner_product(first1, last1, first2, T(0));
    }

    template <class T,
              size_t Rank1, class Tag1, size_t Rank2, class Tag2,
              size_t N>
    tensor<T, (Rank1 - N) + (Rank2 - N)> tensordot(
        const base_tensor<T, Rank1, Tag1> &a,
        const base_tensor<T, Rank2, Tag2> &b,
        const shape_t<N> &a_axes,
        const shape_t<N> &b_axes
    ) {
        static_assert(N <= Rank1 && N <= Rank2, "Contraction dimension must be"
                      " less or equal to tensor dimensions");
        size_t size =
        detail::assert_aligned_shapes(a.shape(), a_axes, b.shape(), b_axes);
        constexpr size_t Rank = (Rank1 - N) + (Rank2 - N);
        shape_t<Rank> shape;
        size_t n = detail::tensordot_concat_shape(shape, a.shape(), a_axes);
        n = detail::tensordot_concat_shape(shape, b.shape(), b_axes, n);
        tensor<T, Rank> out(shape);
        for (index_t<Rank> out_index : make_indices(shape)) {
            index_t<Rank1> a_index;
            n = detail::tensordot_split_index(out_index, a_index, a_axes);
            index_t<Rank2> b_index;
            n = detail::tensordot_split_index(out_index, b_index, b_axes, n);
            auto first1 = make_const_axes_iterator(&a, a_index, a_axes, 0);
            auto last1 = make_const_axes_iterator(&a, a_index, a_axes, size);
            auto first2 = make_const_axes_iterator(&b, b_index, b_axes, 0);
            out[out_index] = std::inner_product(first1, last1, first2, T(0));
        }
        return out;
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a, axes);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a
    ) {
        typedef conj_transpose_tag<Tag> Closure;
        return base_tensor<std::complex<T>, Rank, Closure>(a);
    }

    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const shape_t<Rank> &axes
    ) {
        typedef conj_transpose_tag<Tag> Closure;
        return base_tensor<std::complex<T>, Rank, Closure>(a, axes);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a);
    }

    template <class T, size_t Rank, class Tag>
    inline base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    ) {
        typedef transpose_tag<Tag> Closure;
        return base_tensor<T, Rank, Closure>(a, axes);
    }

namespace linalg {
    template <class T, class Tag>
    T norm(const base_tensor<T, 1, Tag> &a, double ord) {
        ranges::norm pred(ord);
        return pred(a.begin(), a.end());
    }

    template <class T, class Tag>
    T norm(const base_tensor<std::complex<T>, 1, Tag> &a, double ord) {
        ranges::norm pred(ord);
        return pred(a.begin(), a.end());
    }

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> norm(
        const base_tensor<T, Rank, Tag> &a, double ord, size_t axis
    ) {
        tensor<T, Rank> out;
        apply_along_axis(out, ranges::norm(ord), a, axis);
        return out;
    }

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> norm(
        const base_tensor<std::complex<T>, Rank, Tag> &a, double ord,
        size_t axis
    ) {
        tensor<T, Rank> out;
        apply_along_axis(out, ranges::norm(ord), a, axis);
        return out;
    }
}

    template <class T, class Tag>
    T trace(const base_tensor<T, 2, Tag> &a, ptrdiff_t k) {
        T val = T(0);
        if (k >= 0) {
            for (size_t i = 0; i < a.shape(0) && i + k < a.shape(1); ++i) {
                val += a(i, i + k);
            }
        }
        else {
            for (size_t i = 0; i - k < a.shape(0) && i < a.shape(1); ++i) {
                val += a(i - k, i);
            }
        }
        return val;
    }
}

#endif // NUMCPP_LINALG_TCC_INCLUDED
