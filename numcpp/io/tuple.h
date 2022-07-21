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

 /** @file include/numcpp/io/tuple.h
  *  This header overrides input and output stream for array and matrix of
  *  std::pair and std::tuple.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TUPLE_IO_H_INCLUDED
#define NUMCPP_TUPLE_IO_H_INCLUDED

#include <iosfwd>
#include <tuple>
#include <utility>

namespace numcpp {
    /**
     * @brief Overrides input extraction of arrays and matrices of pairs. The
     * supported format to read pairs is (x,y).
     */
    template <class charT, class traits, class T, class U>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::pair<T, U> &rhs
    );

    /**
     * @brief Overrides input extraction of arrays and matrices of tuples. The
     * supported format to read tuples is (elem1,elem2,...,elemN) for N > 1 and
     * (elem1,) for N = 1.
     */
    template <class charT, class traits, class... Args>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::tuple<Args...> &rhs
    );

    /**
     * @brief Overrides output insertion of arrays and matrices of pairs. Each
     * pair is printed in the form (x,y).
     */
    template <class charT, class traits, class T, class U>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::pair<T, U> &rhs
    );

    /**
     * @brief Overrides output insertion of arrays and matrices of tuples. Each
     * tuple is printed in the form (elem1,elem2,...,elemN) for N > 1 and
     * (elem1,) for N = 1.
     */
    template <class charT, class traits, class... Args>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::tuple<Args...> &rhs
    );
}

#include "numcpp/io/tuple.tcc"

#endif // NUMCPP_TUPLE_IO_H_INCLUDED
