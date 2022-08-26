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

 /** @file include/numcpp.h
  *  This is the NumCpp Library header.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_H_INCLUDED
#define NUMCPP_H_INCLUDED

#include "numcpp/tensor.h"
#include "numcpp/math.h"
#include "numcpp/broadcasting.h"
#include "numcpp/functional.h"
// #include "numcpp/routines.h"
#include "numcpp/random.h"
#include "numcpp/io.h"

namespace numcpp {
    template <class T>
    using array = tensor<T, 1>;

    template <class T>
    using array_view = tensor_view<T, 1>;

    template <class T>
    using indirect_array = indirect_tensor<T, 1>;

    template <class T>
    using matrix = tensor<T, 2>;

    template <class T>
    using matrix_view = tensor_view<T, 2>;

    template <class T>
    using indirect_matrix = indirect_tensor<T, 2>;
}

#endif // NUMCPP_H_INCLUDED
