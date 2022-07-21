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

 /** @file include/numcpp/io/tuple.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/io/tuple.tcc}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_TUPLE_IO_TCC_INCLUDED
#define NUMCPP_TUPLE_IO_TCC_INCLUDED

namespace numcpp {
    /// Helper function: Reads a tuple. Returns whether the operation was
    /// successful or not.
    template <class charT, class traits, class Tuple, size_t I = 0>
    typename std::enable_if<I == std::tuple_size<Tuple>::value, bool>::type
    __read_tuple(
        std::basic_istream<charT, traits> &istr, Tuple&, char
    ) {
        charT ch;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen(')'))) {
                return true;
            }
            else {
                istr.putback(ch);
            }
        }
        return false;
    }

    /// Helper function: Reads a tuple. Returns whether the operation was
    /// successful or not.
    template <class charT, class traits, class Tuple, size_t I = 0>
    typename std::enable_if<I < std::tuple_size<Tuple>::value, bool>::type
    __read_tuple(
        std::basic_istream<charT, traits> &istr, Tuple &rhs,
        char sep = '('
    ) {
        charT ch;
        typename std::tuple_element<I, Tuple>::type val;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen(sep))) {
                if (istr >> val) {
                    std::get<I>(rhs) = std::move(val);
                    return
                    __read_tuple<charT, traits, Tuple, I + 1>(istr, rhs, ',');
                }
            }
            else {
                istr.putback(ch);
            }
        }
        return false;
    }

    template <class charT, class traits, class T>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::tuple<T> &rhs
    ) {
        charT ch;
        T val;
        bool fail = true;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('('))) {
                if (istr >> val >> ch) {
                    if (traits::eq(ch, istr.widen(','))) {
                        if (istr >> ch) {
                            if (traits::eq(ch, istr.widen(')'))) {
                                std::get<0>(rhs) = std::move(val);
                                fail = false;
                            }
                            else {
                                istr.putback(ch);
                            }
                        }
                    }
                    else {
                        istr.putback(ch);
                    }
                }
            }
            else {
                istr.putback(ch);
            }
        }
        if (fail) {
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

    template <class charT, class traits, class T, class U>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::pair<T, U> &rhs
    ) {
        charT ch;
        T first;
        U second;
        bool fail = true;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('('))) {
                if (istr >> first >> ch) {
                    if (traits::eq(ch, istr.widen(','))) {
                        if (istr >> second >> ch) {
                            if (traits::eq(ch, istr.widen(')'))) {
                                rhs.first = std::move(first);
                                rhs.second = std::move(second);
                                fail = false;
                            }
                            else {
                                istr.putback(ch);
                            }
                        }
                    }
                    else {
                        istr.putback(ch);
                    }
                }
            }
            else {
                istr.putback(ch);
            }
        }
        if (fail) {
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

    template <class charT, class traits, class... Args>
    std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, std::tuple<Args...> &rhs
    ) {
        if (!__read_tuple(istr, rhs)) {
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

    /// Helper function: Prints a tuple.
    template <class charT, class traits, class Tuple, size_t I = 0>
    typename std::enable_if<
        I == std::tuple_size<Tuple>::value,
        std::basic_ostream<charT, traits>&
    >::type __print_tuple(
        std::basic_ostream<charT, traits> &ostr, const Tuple&, char
    ) {
        return ostr << ')';
    }

    /// Helper function: Prints a tuple.
    template <class charT, class traits, class Tuple, size_t I = 0>
    typename std::enable_if<
        I < std::tuple_size<Tuple>::value,
        std::basic_ostream<charT, traits>&
    >::type __print_tuple(
        std::basic_ostream<charT, traits> &ostr, const Tuple &rhs,
        char sep = '('
    ) {
        ostr << sep << std::get<I>(rhs);
        return __print_tuple<charT, traits, Tuple, I + 1>(ostr, rhs, ',');
    }

    template <class charT, class traits, class T>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::tuple<T> &rhs
    ) {
        std::basic_ostringstream<charT, traits> buffer;
        buffer.flags(ostr.flags());
        buffer.imbue(ostr.getloc());
        buffer.precision(ostr.precision());
        ostr << '(' << std::get<0>(rhs) << ',' << ')';
        return ostr << buffer.str();
    }

    template <class charT, class traits, class T, class U>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::pair<T, U> &rhs
    ) {
        std::basic_ostringstream<charT, traits> buffer;
        buffer.flags(ostr.flags());
        buffer.imbue(ostr.getloc());
        buffer.precision(ostr.precision());
        buffer << '(' << rhs.first << ',' << rhs.second << ')';
        return ostr << buffer.str();
    }

    template <class charT, class traits, class... Args>
    std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const std::tuple<Args...> &rhs
    ) {
        std::basic_ostringstream<charT, traits> buffer;
        buffer.flags(ostr.flags());
        buffer.imbue(ostr.getloc());
        buffer.precision(ostr.precision());
        __print_tuple(buffer, rhs);
        return ostr << buffer.str();
    }
}

#endif // NUMCPP_TUPLE_IO_TCC_INCLUDED
