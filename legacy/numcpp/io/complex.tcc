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

 /** @file include/numcpp/io/complex.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/io/complex.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_COMPLEX_IO_TCC_INCLUDED
#define NUMCPP_COMPLEX_IO_TCC_INCLUDED

namespace numcpp {
    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::complex<T> &rhs
    ) {
        T x, y;
        charT ch;
        bool fail = true;
        if (istr >> x) {
            if (istr >> ch) {
                charT plus = istr.widen('+'), minus = istr.widen('-');
                if (traits::eq(ch, plus) || traits::eq(ch, minus)) {
                    istr.putback(ch);
                    if (istr >> y >> ch) {
                        if (traits::eq(ch, istr.widen('i'))) {
                            rhs = std::complex<T>(x, y);
                            fail = false;
                        }
                        else {
                            istr.putback(ch);
                        }
                    }
                }
                else if (traits::eq(ch, istr.widen('i'))) {
                    rhs = std::complex<T>(T(), x);
                    fail = false;
                }
                else {
                    istr.putback(ch);
                    rhs = x;
                    fail = false;
                }
            }
            else {
                rhs = x;
                fail = false;
            }
        }
        if (fail) {
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

    template <class T, class charT, class traits>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::complex<T> &rhs
    ) {
        std::basic_ostringstream<charT, traits> buffer;
        buffer.flags(ostr.flags());
        buffer.imbue(ostr.getloc());
        buffer.precision(ostr.precision());
        buffer << rhs.real() << std::showpos << rhs.imag() << 'i';
        return ostr << buffer.str();
    }
}

#endif // NUMCPP_COMPLEX_IO_TCC_INCLUDED
