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

 /** @file include/numcpp/broadcasting.h
  *  This header defines routines for broadcasting, functional programming and
  *  indexing.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_BROADCASTING_H_INCLUDED
#define NUMCPP_BROADCASTING_H_INCLUDED

#include "numcpp/tensor.h"
#include "numcpp/broadcasting/utilities.h"

namespace numcpp {
    /**
     * @brief Return a tensor of pairs with each element constructed from the
     * corresponding elements in two tensor objects.
     *
     * @param a First tensor-like argument.
     * @param b Second tensor-like argument.
     *
     * @return A light-weight object with the elements in two tensor arguments
     *     zipped as a pair. This function does not create a new tensor,
     *     instead, it returns a readonly view with the pairs constructed from
     *     both tensors.
     */
    template <size_t Rank, class T, class TagT, class U, class TagU>
    base_tensor<
        std::pair<T, U>, Rank, lazy_binary_tag<detail::zip, T, TagT, U, TagU>
    > zip(
        const base_tensor<T, Rank, TagT> &a,
        const base_tensor<U, Rank, TagU> &b
    );

    /**
     * @brief Return a tensor with each element constructed from taking the
     * I-th element of the corresponding pair in a tensor object.
     *
     * @tparam I Element index. It shall be 0 or 1.
     * @param a A tensor-like object of pairs.
     *
     * @return A light-weight object with the I-th element of each pair in the
     *     tensor. This function does not create a new tensor, instead, it
     *     returns a readonly view with the I-th element of each pair.
     */
    template <size_t I, size_t Rank, class T, class U, class Tag>
    base_tensor<
        typename std::tuple_element<I, std::pair<T, U> >::type, Rank,
        lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag>
    >
    unzip(const base_tensor<std::pair<T, U>, Rank, Tag> &a);
}

#include "broadcasting/broadcasting.tcc"

#endif // NUMCPP_BROADCASTING_H_INCLUDED
