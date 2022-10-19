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

 /** @file include/numcpp/slice.h
  *  This header defines routines for linear algebra.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LINALG_H_INCLUDED
#define NUMCPP_LINALG_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/routines/ranges.h"
#include "numcpp/linalg/transpose_view.h"
#include "numcpp/linalg/lu_factor.h"

namespace numcpp {
    /// Basic linear algebra.

    /**
     * @brief Return the dot product of two vectors.
     *
     * @details The dot product of two vectors @f$a = [a_1, a_2, \ldots, a_n]@f$
     * and @f$b = [b_1, b_2, \ldots, b_n]@f$ is defined as
     * @f[
     *     a \cdot b = a_1b_1 + a_2b_2 + \ldots + a_nb_n
     * @f]
     *
     * @note Unlike Numpy's dot, dot only supports computing the dot product of
     * two 1-dimensional tensors with the same number of elements.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The dot product of the inputs.
     *
     * @throw std::invalid_argument Thrown if the tensor arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T dot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b);

    /**
     * @brief Return the dot product of two vectors.
     *
     * @details The vdot function handles complex numbers differently than dot.
     * For complex numbers, the complex conjugate of the first argument is used
     * for the calcualtion of the dot product.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The dot product of the inputs.
     *
     * @throw std::invalid_argument Thrown if the tensor arguments have
     *     different sizes.
     */
    template <class T, class Tag1, class Tag2>
    T vdot(const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b);

    template <class T, class Tag1, class Tag2>
    std::complex<T> vdot(
        const base_tensor<std::complex<T>, 1, Tag1> &a,
        const base_tensor<std::complex<T>, 1, Tag2> &b
    );

    /**
     * @brief Return the cross product of two vectors.
     *
     * @details The cross product of two vectors a and b (in R3) is a vector
     * perpendicular to both a and b.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     * @param axis Axis along which to take cross product. Default is Rank - 1.
     *     Input tensors need to be broadcastable to some shape along the other
     *     axes.
     *
     * @return The cross product of the inputs.
     *
     * @throw std::invalid_argument Thrown if the size of the vectors is not 3
     *     along the given axis.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> cross(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        size_t axis = Rank - 1
    );

    /**
     * @brief Return the matrix multiplication of two matrices.
     *
     * @details The behaviour depends on the arguments in the following way
     * - If both arguments are 1-dimensional, it is the dot product of vectors
     *   (without complex conjugation).
     * - If both arguments are 2-dimensional, it is the conventional matrix
     *   multiplication.
     * - If the first argument is 1-dimensional and the second argument is
     *   2-dimensional, it is the matrix multiplication with the first
     *   argument treated as a row-vector.
     * - If the first argument is 2-dimensional and the second argument is
     *   1-dimensional, it is the matrix multiplication with the second
     *   argument treated as a column-vector.
     * - If both arguments are n-dimensional, n > 2, it is treated as a stack
     *   of matrices residing in the last 2 dimensions and broadcast
     *   accordingly.
     *
     * The matrix multiplication of a @f$m \times p@f$ matrix
     * @f$A = (a_{ij})@f$ and a @f$p \times n@f$ matrix @f$B = (b_{ij})@f$ is
     * the @f$m \times n@f$ matrix @f$C = (c_{ij})@f$ such that
     * @f[
     *     c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \ldots + a_{ip}b_{pj}
     * @f]
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return The matrix multiplication of the inputs.
     *
     * @throw std::invalid_argument Thrown if the last dimension of a is not
     *     the same as the second-to-last dimension of b.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag1, class Tag2>
    T matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 2> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 1, Tag1> &a, const base_tensor<T, 2, Tag2> &b
    );

    template <class T, class Tag1, class Tag2>
    tensor<T, 1> matmul(
        const base_tensor<T, 2, Tag1> &a, const base_tensor<T, 1, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, 2, Tag2> &b
    );

    template <class T, size_t Rank, class Tag1, class Tag2>
    tensor<T, Rank> matmul(
        const base_tensor<T, 2, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b
    );

    /**
     * @brief Return a contraction of two tensors over multiple axes.
     *
     * @details Given two tensors a and b, and two tuples of axes a_axes and
     * b_axes, sum the products of a's and b's elements over the axes specified
     * by a_axes and b_axes.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     * @param a_axes List of axes to sum over a.
     * @param b_axes List of axes to sum over b.
     *
     * @return The contraction of the inputs. The shape of the result consists
     *     of the non-contracted axes of the first tensor, followed by the
     *     non-contracted axes of the second.
     *
     * @throw std::invalid_argument Thrown if the shape of a is not equal to
     *     the shape of b over the contracted axes.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag1, class Tag2>
    T tensordot(
        const base_tensor<T, Rank, Tag1> &a,
        const base_tensor<T, Rank, Tag2> &b,
        const shape_t<Rank> &a_axes,
        const shape_t<Rank> &b_axes
    );

    template <class T,
              size_t Rank1, class Tag1, size_t Rank2, class Tag2,
              size_t N>
    tensor<T, (Rank1 - N) + (Rank2 - N)> tensordot(
        const base_tensor<T, Rank1, Tag1> &a,
        const base_tensor<T, Rank2, Tag2> &b,
        const shape_t<N> &a_axes,
        const shape_t<N> &b_axes
    );

    /**
     * @brief Reverse or permute the axes of a tensor.
     *
     * @param a Input tensor.
     * @param axes If specified, it must be a permutation of (0, 1, ...,
     *     Rank - 1). The i-th axis of the returned tensor will correspond to
     *     the axis numbered axes[i] of the input. If not specified, reverses
     *     the order of the axes.
     *
     * @return A light-weight object with the axes of a permuted. This function
     *     does not create a new tensor, instead, it returns a readonly view
     *     of a with its axes reversed or permuted.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    );

    /**
     * @brief Return the (complex) conjugate transpose of a tensor. For
     * non-complex types, conj_transpose is equivalent to transpose.
     *
     * @param a Input tensor.
     * @param axes If specified, it must be a permutation of (0, 1, ...,
     *     Rank - 1). The i-th axis of the returned tensor will correspond to
     *     the axis numbered axes[i] of the input. If not specified, reverses
     *     the order of the axes.
     *
     * @return A light-weight object with the conjugate transpose of a. This
     *     function does not create a new tensor, instead, it returns a
     *     readonly view of a with its elements complex conjugated and its axes
     *     reversed or permuted.
     */
    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<std::complex<T>, Rank, conj_transpose_tag<Tag> >
    conj_transpose(
        const base_tensor<std::complex<T>, Rank, Tag> &a,
        const shape_t<Rank> &axes
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a
    );

    template <class T, size_t Rank, class Tag>
    base_tensor<T, Rank, transpose_tag<Tag> > conj_transpose(
        const base_tensor<T, Rank, Tag> &a, const shape_t<Rank> &axes
    );

namespace linalg {
    /**
     * @brief Return the vector norm.
     *
     * @details This function is able to return one of the following norms:
     *        0 sum(x != 0)
     *      inf amax(abs(x))
     *     -inf amin(abs(x))
     *        p sum(abs(a)**p)**(1./p)
     *
     * @param a A 1-dimensional tensor-like object.
     * @param ord Order of the norm. The default is 2 (Euclidean norm). For
     *     values of ord < 1, the result is, strictly speaking, not a
     *     mathematical norm, but it may still be useful for various numerical
     *     purposes.
     *
     * @return Norm of the vector.
     */
    template <class T, class Tag>
    T norm(const base_tensor<T, 1, Tag> &a, double ord = 2);

    template <class T, class Tag>
    T norm(const base_tensor<std::complex<T>, 1, Tag> &a, double ord = 2);

    /**
     * @brief Return the vector norm along the given axis.
     *
     * @param a A tensor-like object.
     * @param ord Order of the norm.
     * @param axis Axis along which to compute the vector norm.
     *
     * @return A new tensor with the norm of the vectors along an axis. The
     *     output tensor will have the same dimension and the same shape,
     *     except that the axis which is reduced is left as a dimension of size
     *     one.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> norm(
        const base_tensor<T, Rank, Tag> &a, double ord, size_t axis
    );

    template <class T, size_t Rank, class Tag>
    tensor<T, Rank> norm(
        const base_tensor<std::complex<T>, Rank, Tag> &a, double ord,
        size_t axis
    );

    /**
     * @brief Compute the determinant of a matrix via LU decomposition.
     *
     * @param a Input matrix to compute determinant for.
     *
     * @return Determinant of a.
     *
     * @throw std::invalid_argument Thrown if input matrix is not square.
     */
    template <class T, class Tag>
    T det(const base_tensor<T, 2, Tag> &a);
}

    /**
     * @brief Return the sum along a diagonal of the matrix, i.e., the sum of
     * the elements of the form a(i, i + k).
     *
     * @param a Matrix from which the diagonal is taken.
     * @param k Offset of the diagonal from the main diagonal. A positive value
     *     refers to an upper diagonal and a negative value refers to a lower
     *     diagonal. Defaults to main diagonal (0).
     *
     * @return The sum along the diagonal.
     */
    template <class T, class Tag>
    T trace(const base_tensor<T, 2, Tag> &a, ptrdiff_t k = 0);

    /// Exceptions.

namespace linalg {
    /**
     * @brief This class defines the type of objects thrown as exceptions by
     * linalg functions. It reports when a Linear Algebra-related condition
     * would prevent further correct execution of the function.
     */
    class linalg_error : public std::runtime_error {
    public:
        explicit linalg_error(const std::string &what_arg)
         : std::runtime_error(what_arg) {}

        explicit linalg_error(const char *what_arg)
         : std::runtime_error(what_arg) {}

        linalg_error(const linalg_error&) = default;
        linalg_error(linalg_error&&) = default;
        linalg_error& operator=(const linalg_error&) = default;
        linalg_error& operator=(linalg_error&&) = default;

        virtual ~linalg_error() = default;
    };
}

    /// Decompositions.

namespace linalg {
    /**
     * @brief Compute the pivoted LU decomposition of a matrix.
     *
     * @details Let @f$A@f$ be a @f$m \times n@f$ matrix. The decomposition is
     * @f[
     *     A = P L U
     * @f]
     * where
     * - @f$P@f$ is a @f$m \times m@f$ permutation matrix,
     * - @f$L@f$ is a @f$m \times k@f$, @f$k = \min(m, n)@f$, lower triangular
     *   or trapezoidal matrix with unit diagonal, and
     * - @f$U@f$ is a @f$k \times n@f$ upper triangular or trapezoidal matrix.
     *
     * @param A Matrix to decompose.
     *
     * @return A lu_result object with the pivoted LU decomposition of A.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    lu_result<typename base_tensor<T, 2, Tag>::value_type>
    lu(const base_tensor<T, 2, Tag> &A);

    /**
     * @brief Compute the LDL decomposition of a matrix.
     *
     * @details Let @f$A@f$ be a Hermitian matrix. The decomposition is
     * @f[
     *     A = L D L^*
     * @f]
     * where
     * - @f$L@f$ is a lower triangular matrix with unit diagonal,
     * - @f$D@f$ is a diagonal matrix, and
     * - @f$L^*@f$ denotes the conjugate transpose of @f$L@f$.
     * When @f$A@f$ is a real matrix (hence symmetric), the decomposition may
     * be written as
     * @f[
     *     A = L D L^T
     * @f]
     *
     * @param A Matrix to decompose. Only the lower triangle of A is used.
     *
     * @return A ldl_result object with the LDL decomposition of A.
     *
     * @throw std::invalid_argument Thrown if input matrix is not square.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, class Tag>
    ldl_result<typename base_tensor<T, 2, Tag>::value_type>
    ldl(const base_tensor<T, 2, Tag> &A);

    template <class T, class Tag>
    ldl_result< std::complex<T> >
    ldl(const base_tensor<std::complex<T>, 2, Tag> &A);

    /**
     * @brief Compute the Cholesky decomposition of a matrix.
     *
     * @details Let @f$A@f$ be a Hermitian positive-definite matrix. The
     * decomposition is
     * @f[
     *     A = L L^*
     * @f]
     * where
     * - @f$L@f$ is a lower triangular matrix, and
     * - @f$L^*@f$ denotes the conjugate transpose of @f$L@f$.
     * When @f$A@f$ is a real matrix (hence symmetric positive-definite), the
     * decomposition may be written as
     * @f[
     *     A = L L^T
     * @f]
     *
     * @param A Matrix to decompose. Only the lower triangle of A is used.
     *
     * @return A cho_result object with the Cholesky decomposition of A.
     *
     * @throw std::invalid_argument Thrown if input matrix is not square.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     * @throw linalg_error Thrown if decomposition fails.
     */
    template <class T, class Tag>
    cho_result<typename base_tensor<T, 2, Tag>::value_type>
    cholesky(const base_tensor<T, 2, Tag> &A);

    template <class T, class Tag>
    cho_result< std::complex<T> >
    cholesky(const base_tensor<std::complex<T>, 2, Tag> &A);
}
}

#include "numcpp/linalg/linalg.tcc"

#endif // NUMCPP_LINALG_H_INCLUDED
