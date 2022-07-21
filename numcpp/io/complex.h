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

 /** @file include/numcpp/io/complex.h
  *  This header overrides input and output stream for array and matrix of
  *  std::complex.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_COMPLEX_IO_H_INCLUDED
#define NUMCPP_COMPLEX_IO_H_INCLUDED

#include <iosfwd>
#include <complex>

namespace numcpp {
    /**
     * @brief Overrides input extraction of arrays and matrices of complex
     * types. The supported formats to read complex values are: x, yi, x+yi.
     */
    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::complex<T> &rhs
    );

    /**
     * @brief Overrides output insertion of arrays and matrices of complex
     * types. Each complex value is printed in the form x+yi.
     */
    template <class T, class charT, class traits>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::complex<T> &rhs
    );
}

#include "numcpp/io/complex.tcc"

#endif // NUMCPP_COMPLEX_IO_H_INCLUDED
