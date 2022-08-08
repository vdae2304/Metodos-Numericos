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

 /** @file include/numcpp/broadcasting/broadcasting.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/broadcasting.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_BROADCASTING_TCC_INCLUDED
#define NUMCPP_BROADCASTING_TCC_INCLUDED

namespace numcpp {
    template <size_t Rank, class T, class TagT, class U, class TagU>
    base_tensor<
        std::pair<T, U>, Rank, lazy_binary_tag<detail::zip, T, TagT, U, TagU>
    > zip(
        const base_tensor<T, Rank, TagT> &a,
        const base_tensor<U, Rank, TagU> &b
    ) {
        typedef std::pair<T, U> Rt;
        typedef lazy_binary_tag<detail::zip, T, TagT, U, TagU> Closure;
        return base_tensor<Rt, Rank, Closure>(detail::zip(), a, b);
    }

    template <size_t I, size_t Rank, class T, class U, class Tag>
    base_tensor<
        typename std::tuple_element<I, std::pair<T, U> >::type, Rank,
        lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag>
    >
    unzip(const base_tensor<std::pair<T, U>, Rank, Tag> &a) {
        typedef typename std::tuple_element<I, std::pair<T, U> >::type Rt;
        typedef lazy_unary_tag<detail::unzip<I>, std::pair<T, U>, Tag> Closure;
        return base_tensor<Rt, Rank, Closure>(detail::unzip<I>(), a);
    }
}

#endif // NUMCPP_BROADCASTING_TCC_INCLUDED
