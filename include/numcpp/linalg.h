/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

/** @file include/numcpp/linalg.h
 *  This header defines routines for linear algebra.
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_LINALG_H_INCLUDED
#define NUMCPP_LINALG_H_INCLUDED

#include "numcpp/config.h"
#include "numcpp/linalg/transpose_view.h"

namespace numcpp {
/// Basic linear algebra.

/**
 * @brief Return the dot product of two vectors.
 *
 * @details The dot product of two vectors @f$a = [a_1, a_2, \ldots, a_n]@f$ and
 * @f$b = [b_1, b_2, \ldots, b_n]@f$ is defined as
 * @f[
 *     a \cdot b = a_1b_1 + a_2b_2 + \ldots + a_nb_n
 * @f]
 *
 * @param a First tensor-like argument.
 * @param b Second tensor-like argument.
 *
 * @return The dot product of the inputs.
 *
 * @throw std::invalid_argument Thrown if the tensor arguments have different
 *                              sizes.
 */
template <class Container1, class Container2, class T>
T dot(const expression<Container1, T, 1> &a,
      const expression<Container2, T, 1> &b);

/**
 * @brief Return the dot product of two vectors.
 *
 * @details The @c vdot function handles complex numbers differently than
 * @c dot. For complex numbers, the complex conjugate of the first argument is
 * used for the calcualtion of the dot product.
 *
 * @param a First tensor-like argument.
 * @param b Second tensor-like argument.
 *
 * @return The dot product of the inputs.
 *
 * @throw std::invalid_argument Thrown if the tensor arguments have different
 *                              sizes.
 */
template <class Container1, class Container2, class T>
T vdot(const expression<Container1, T, 1> &a,
       const expression<Container2, T, 1> &b);

template <class Container1, class Container2, class T>
std::complex<T> vdot(const expression<Container1, std::complex<T>, 1> &a,
                     const expression<Container2, std::complex<T>, 1> &b);

/**
 * @brief Return the cross product of two vectors.
 *
 * @details The cross product of two vectors @a a and @a b (in R3) is a vector
 * perpendicular to both @a a and @a b.
 *
 * @param a First tensor-like argument.
 * @param b Second tensor-like argument.
 * @param axis Axis along which to take cross product. Default is Rank - 1.
 *             Input tensors need to be broadcastable to some shape along the
 *             other axes.
 *
 * @return The cross product of the inputs.
 *
 * @throw std::invalid_argument Thrown if the size of the vectors is not 3 along
 *                              the given axis.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> cross(const expression<Container1, T, Rank> &a,
                      const expression<Container2, T, Rank> &b,
                      size_t axis = Rank - 1);

/**
 * @brief Return the matrix multiplication of two matrices.
 *
 * @details The behaviour depends on the arguments in the following way
 * - If both arguments are 1-dimensional, it is the dot product of vectors
 *   (without complex conjugation).
 * - If both arguments are 2-dimensional, it is the conventional matrix
 *   multiplication.
 * - If the first argument is 1-dimensional and the second argument is
 *   2-dimensional, it is the matrix multiplication with the first argument
 *   treated as a row-vector.
 * - If the first argument is 2-dimensional and the second argument is
 *   1-dimensional, it is the matrix multiplication with the second argument
 *   treated as a column-vector.
 * - If both arguments are n-dimensional, n > 2, it is treated as a stack of
 *   matrices residing in the last 2 dimensions and broadcast accordingly.
 *
 * The matrix multiplication of a @f$m \times p@f$ matrix @f$A = (a_{ij})@f$ and
 * a @f$p \times n@f$ matrix @f$B = (b_{ij})@f$ is the @f$m \times n@f$ matrix
 * @f$C = (c_{ij})@f$ such that
 * @f[
 *     c_{ij} = a_{i1}b_{1j} + a_{i2}b_{2j} + \ldots + a_{ip}b_{pj}
 * @f]
 *
 * @param a First tensor-like argument.
 * @param b Second tensor-like argument.
 *
 * @return The matrix multiplication of the inputs.
 *
 * @throw std::invalid_argument Thrown if the last dimension of @a a is not the
 *                              same as the second-to-last dimension of @a b.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class Container2, class T>
T matmul(const expression<Container1, T, 1> &a,
         const expression<Container2, T, 1> &b);

template <class Container1, class Container2, class T>
tensor<T, 2> matmul(const expression<Container1, T, 2> &a,
                    const expression<Container2, T, 2> &b);

template <class Container1, class Container2, class T>
tensor<T, 1> matmul(const expression<Container1, T, 1> &a,
                    const expression<Container2, T, 2> &b);

template <class Container1, class Container2, class T>
tensor<T, 1> matmul(const expression<Container1, T, 2> &a,
                    const expression<Container2, T, 1> &b);

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> matmul(const expression<Container1, T, Rank> &a,
                       const expression<Container2, T, Rank> &b);

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> matmul(const expression<Container1, T, Rank> &a,
                       const expression<Container2, T, 2> &b);

template <class Container1, class Container2, class T, size_t Rank>
tensor<T, Rank> matmul(const expression<Container1, T, 2> &a,
                       const expression<Container2, T, Rank> &b);

/**
 * @brief Return a contraction of two tensors over multiple axes.
 *
 * @details Given two tensors @a a and @a b, and two tuples of axes @a a_axes
 * and @a b_axes, sum the products of @a a 's and @a b 's elements over the axes
 * specified by @a a_axes and @a b_axes.
 *
 * @param a First tensor-like argument.
 * @param b Second tensor-like argument.
 * @param a_axes List of axes to sum over @a a.
 * @param b_axes List of axes to sum over @a b.
 *
 * @return The contraction of the inputs. The shape of the result consists of
 *         the non-contracted axes of the first tensor, followed by the
 *         non-contracted axes of the second.
 *
 * @throw std::invalid_argument Thrown if the shape of @a a is not equal to the
 *                              shape of @a b over the contracted axes.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class Container1, class T, size_t Rank1, class Container2,
          size_t Rank2, size_t N>
tensor<T, (Rank1 - N) + (Rank2 - N)>
tensordot(const expression<Container1, T, Rank1> &a,
          const expression<Container2, T, Rank2> &b, const shape_t<N> &a_axes,
          const shape_t<N> &b_axes);

template <class Container1, class T, size_t Rank1, class Container2,
          size_t Rank2>
tensor<T, Rank1 - Rank2> tensordot(const expression<Container1, T, Rank1> &a,
                                   const expression<Container2, T, Rank2> &b,
                                   const shape_t<Rank2> &a_axes,
                                   const shape_t<Rank2> &b_axes);

template <class Container1, class T, size_t Rank1, class Container2,
          size_t Rank2>
tensor<T, Rank2 - Rank1> tensordot(const expression<Container1, T, Rank1> &a,
                                   const expression<Container2, T, Rank2> &b,
                                   const shape_t<Rank1> &a_axes,
                                   const shape_t<Rank1> &b_axes);

template <class Container1, class T, size_t Rank, class Container2>
T tensordot(const expression<Container1, T, Rank> &a,
            const expression<Container2, T, Rank> &b,
            const shape_t<Rank> &a_axes, const shape_t<Rank> &b_axes);

/**
 * @brief Reverse or permute the axes of a tensor.
 *
 * @param a Input tensor.
 * @param axes If specified, it must be a permutation of (0, 1, ..., Rank - 1).
 *             The @a i -th axis of the returned tensor will correspond to the
 *             axis numbered @a axes[i] of the input. If not specified, reverses
 *             the order of the axes.
 *
 * @return A light-weight object with the axes of @a a permuted. This function
 *         does not create a new tensor, instead, it returns a readonly view of
 *         @a a with its axes reversed or permuted.
 */
template <class Container, class T, size_t Rank>
transpose_expr<Container, T, Rank>
transpose(const expression<Container, T, Rank> &a) {
  return transpose_expr<Container, T, Rank>(a);
}

template <class Container, class T, size_t Rank>
transpose_expr<Container, T, Rank>
transpose(const expression<Container, T, Rank> &a, const shape_t<Rank> &axes) {
  return transpose_expr<Container, T, Rank>(a, axes);
}

/**
 * @brief Return the (complex) conjugate transpose of a tensor. For non-complex
 * types, @c conj_transpose is equivalent to @c transpose.
 *
 * @param a Input tensor.
 * @param axes If specified, it must be a permutation of (0, 1, ..., Rank - 1).
 *             The @a i -th axis of the returned tensor will correspond to the
 *             axis numbered @a axes[i] of the input. If not specified, reverses
 *             the order of the axes.
 *
 * @return A light-weight object with the conjugate transpose of @a a. This
 *         function does not create a new tensor, instead, it returns a readonly
 *         view of @a a with the complex conjugate of its elements and its axes
 *         reversed or permuted.
 */
template <class Container, class T, size_t Rank>
conj_transpose_expr<Container, std::complex<T>, Rank>
conj_transpose(const expression<Container, std::complex<T>, Rank> &a) {
  return conj_transpose_expr<Container, std::complex<T>, Rank>(a);
}

template <class Container, class T, size_t Rank>
conj_transpose_expr<Container, std::complex<T>, Rank>
conj_transpose(const expression<Container, std::complex<T>, Rank> &a,
               const shape_t<Rank> &axes) {
  return conj_transpose_expr<Container, std::complex<T>, Rank>(a, axes);
}

template <class Container, class T, size_t Rank>
transpose_expr<Container, T, Rank>
conj_transpose(const expression<Container, T, Rank> &a) {
  return transpose_expr<Container, T, Rank>(a);
}

template <class Container, class T, size_t Rank>
transpose_expr<Container, T, Rank>
conj_transpose(const expression<Container, T, Rank> &a,
               const shape_t<Rank> &axes) {
  return transpose_expr<Container, T, Rank>(a, axes);
}

namespace linalg {
/**
 * @brief Return the vector norm.
 *
 * @details This function is able to return one of the following norms:
 *   0    sum(x != 0)
 *   inf  amax(abs(x))
 *   -inf amin(abs(x))
 *   p    sum(abs(a)**p)**(1./p)
 *
 * @param a A 1-dimensional tensor-like object.
 * @param ord Order of the norm. The default is 2 (Euclidean norm). For values
 *            of @a ord < 1, the result is, strictly speaking, not a
 *            mathematical norm, but it may still be useful for various
 *            numerical purposes.
 *
 * @return Norm of the vector.
 */
template <class Container, class T>
T norm(const expression<Container, T, 1> &a, double ord = 2);

template <class Container, class T>
T norm(const expression<Container, std::complex<T>, 1> &a, double ord = 2);

/**
 * @brief Compute the determinant of a matrix via LU decomposition.
 *
 * @param a Input matrix to compute determinant for.
 *
 * @return Determinant of @a a.
 *
 * @throw std::invalid_argument Thrown if input matrix is not square.
 */
template <class Container, class T> T det(const expression<Container, T, 2> &a);
} // namespace linalg

/**
 * @brief Return the sum along a diagonal of the matrix, i.e., the sum of the
 * elements of the form @a a(i,i+k).
 *
 * @param a Matrix from which the diagonal is taken.
 * @param k Offset of the diagonal from the main diagonal. A positive value
 *          refers to an upper diagonal and a negative value refers to a lower
 *          diagonal. Defaults to main diagonal (0).
 *
 * @return The sum along the diagonal.
 */
template <class Container, class T>
T trace(const expression<Container, T, 2> &a, ptrdiff_t k = 0);
} // namespace numcpp

#include "numcpp/linalg/linalg.tcc"

#endif // NUMCPP_LINALG_H_INCLUDED
