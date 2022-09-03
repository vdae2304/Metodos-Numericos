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

 /** @file include/numcpp/io/io.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/io.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_IO_TCC_INCLUDED
#define NUMCPP_IO_TCC_INCLUDED

#include <complex>
#include <ios>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace numcpp {
    /// Input/output streams

    template <class charT, class traits, size_t Rank>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape
    ) {
        charT ch;
        bool fail = true;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('('))) {
                for (size_t i = 0; i < shape.ndim(); ++i) {
                    if (istr >> shape[i] >> ch) {
                        if (i < shape.ndim() - 1) {
                            if (traits::eq(ch, istr.widen(','))) {
                                continue;
                            }
                            else {
                                istr.putback(ch);
                            }
                        }
                        else {
                            if (traits::eq(ch, istr.widen(')'))) {
                                fail = false;
                            }
                            else {
                                istr.putback(ch);
                            }
                        }
                    }
                    break;
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

    template <class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, shape_t<1> &shape
    ) {
        charT ch;
        bool fail = true;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('('))) {
                if (istr >> shape[0] >> ch) {
                    if (traits::eq(ch, istr.widen(','))) {
                        if (istr >> ch) {
                            if (traits::eq(ch, istr.widen(')'))) {
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
                if (istr >> shape[0]) {
                    fail = false;
                }
            }
        }
        if (fail) {
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

    template <class charT, class traits, size_t Rank>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const shape_t<Rank> &shape
    ) {
        std::basic_stringstream<charT, traits> sstr;
        sstr.flags(ostr.flags());
        sstr.imbue(ostr.getloc());
        sstr << "(" << shape[0];
        for (size_t i = 1; i < shape.ndim(); ++i) {
            sstr << ", " << shape[i];
        }
        sstr << ")";
        return ostr << sstr.str();
    }

    template <class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const shape_t<1> &shape
    ) {
        std::basic_stringstream<charT, traits> sstr;
        sstr.flags(ostr.flags());
        sstr.imbue(ostr.getloc());
        sstr << "(" << shape[0] << ",)";
        return ostr << sstr.str();
    }

    template <class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, slice &slc
    ) {
        size_t start, stop, stride;
        charT ch;
        bool fail = true;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('('))) {
                if (istr >> stop >> ch) {
                    if (traits::eq(ch, istr.widen(','))) {
                        start = stop;
                        if (istr >> stop >> ch) {
                            if (traits::eq(ch, istr.widen(','))) {
                                if (istr >> stride >> ch) {
                                    if (traits::eq(ch, istr.widen(')'))) {
                                        slc = slice(start, stop, stride);
                                        fail = false;
                                    }
                                    else {
                                        istr.putback(ch);
                                    }
                                }
                            }
                            else if (traits::eq(ch, istr.widen(')'))) {
                                slc = slice(start, stop);
                                fail = false;
                            }
                            else {
                                istr.putback(ch);
                            }
                        }
                    }
                    else if (traits::eq(ch, istr.widen(')'))) {
                        slc = slice(stop);
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
        if (fail) {
            slc = slice();
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

    template <class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const slice &slc
    ) {
        std::basic_ostringstream<charT, traits> sstr;
        sstr.flags(ostr.flags());
        sstr.imbue(ostr.getloc());
        sstr << "(" << slc.start() << "," << slc.size() << "," << slc.stride()
             << ")";
        return ostr << sstr.str();
    }

namespace detail {
    /**
     * @brief Helper function: Read the elements of a tensor recursively from
     * the first axis to the last axis. Returns whether the operation was
     * successfull.
     */
    template <class charT, class traits, class T, size_t Rank>
    bool read_tensor(
        std::basic_istream<charT, traits> &istr,
        shape_t<Rank> &shape, std::vector<T> &buffer,
        size_t axis
    );

    /**
     * @brief Helper function: Read a value. Returns whether the operation was
     * successful.
     */
    template <class charT, class traits, class T>
    inline bool read_value(std::basic_istream<charT, traits> &istr, T &rhs) {
        return bool(istr >> rhs);
    }

    /**
     * @brief Helper function: Read a value. Partial specialization for complex
     * numbers.
     */
    template <class charT, class traits, class T>
    bool read_value(
        std::basic_istream<charT, traits> &istr, std::complex<T> &rhs
    ) {
        using namespace printoptions;
        switch (complexmode) {
        case complexmode_t::defaultmode:
            return bool(istr >> rhs);
        case complexmode_t::algebraic:
            T x, y;
            charT ch;
            bool fail = true;
            if (istr >> x) {
                if (!istr.eof()) {
                    charT plus = istr.widen('+'), minus = istr.widen('-');
                    istr >> ch;
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
        }
        return bool(istr);
    }

    /**
     * @brief Helper function: Read a list of values and append them at the end
     * of a std::vector. Save in size the number of extracted values. Return
     * whether the operation was sucessfull.
     */
    template <class charT, class traits, class T>
    bool read_tensor_slice(
        std::basic_istream<charT, traits> &istr, size_t &size,
        std::vector<T> &buffer
    ) {
        charT ch;
        T val;
        size = 0;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('['))) {
                while (read_value(istr, val) && istr >> ch) {
                    ++size;
                    buffer.push_back(val);
                    if (traits::eq(ch, istr.widen(','))) {
                        continue;
                    }
                    else if (traits::eq(ch, istr.widen(']'))) {
                        return true;
                    }
                    else {
                        istr.putback(ch);
                        break;
                    }
                }
            }
            else {
                istr.putback(ch);
            }
        }
        return false;
    }

    /**
     * @brief Helper function: Read a list of values and append them at the end
     * of a std::vector. Partial specialization for strings. Strings need to be
     * handled separately to deal with whitespaces.
     */
    template <class charT, class traits>
    bool read_tensor_slice(
        std::basic_istream<charT, traits> &istr, size_t &size,
        std::vector<std::basic_string<charT, traits> > &buffer
    ) {
        charT ch;
        size = 0;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('['))) {
                std::basic_string<charT, traits> line, token;
                std::getline(istr, line, istr.widen(']'));
                std::basic_istringstream<charT, traits> tokenizer(line);
                while (std::getline(tokenizer, token, istr.widen(','))) {
                    ++size;
                    token.erase(0, token.find_first_not_of(" \f\n\r\t\v"));
                    token.erase(token.find_last_not_of(" \f\n\r\t\v") + 1);
                    buffer.push_back(token);
                }
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Helper function: Read a block of values and append them at the
     * end of a std::vector. Save the size of the block in shape. Return
     * whether the operation was sucessfull.
     */
    template <class charT, class traits, class T, size_t Rank>
    bool read_tensor_block(
        std::basic_istream<charT, traits> &istr,
        shape_t<Rank> &shape, std::vector<T> &buffer,
        size_t axis
    ) {
        charT ch;
        shape[axis] = 0;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('['))) {
                size_t size = 0;
                while (read_tensor(istr, shape, buffer, axis + 1)
                       && istr >> ch) {
                    if (shape[axis] > 0 && size != shape[axis + 1]) {
                        break;
                    }
                    size = shape[axis + 1];
                    ++shape[axis];
                    if (traits::eq(ch, istr.widen(','))) {
                        continue;
                    }
                    else if (traits::eq(ch, istr.widen(']'))) {
                        return true;
                    }
                    else {
                        istr.putback(ch);
                        break;
                    }
                }
            }
            else {
                istr.putback(ch);
            }
        }
        return false;
    }

    template <class charT, class traits, class T, size_t Rank>
    bool read_tensor(
        std::basic_istream<charT, traits> &istr,
        shape_t<Rank> &shape, std::vector<T> &buffer,
        size_t axis
    ) {
        if (axis == shape.ndim() - 1) {
            return read_tensor_slice(istr, shape[axis], buffer);
        }
        else {
            return read_tensor_block(istr, shape, buffer, axis);
        }
    }
}

    template <class charT, class traits, class T, size_t Rank>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, tensor<T, Rank> &arr
    ) {
        shape_t<Rank> shape;
        std::vector<T> buffer;
        if (detail::read_tensor(istr, shape, buffer, 0)) {
            arr = std::move(tensor<T, Rank>(shape, buffer.begin()));
        }
        else {
            arr = std::move(tensor<T, Rank>());
            istr.setstate(std::ios_base::failbit);
        }
        return istr;
    }

namespace detail {
    /**
     * @brief Helper function: Set all the print options flags on the output
     * stream object.
     */
    template <class T, class charT, class traits>
    void set_printoptions_flags(std::basic_ostream<charT, traits> &ostr) {
        using namespace printoptions;
        if (precision == (size_t)fullprecision) {
            ostr.precision(std::numeric_limits<T>::max_digits10);
        }
        else {
            ostr.precision(precision);
        }
        switch (floatmode) {
        case floatmode_t::defaultmode:
            ostr.unsetf(std::ios::floatfield);
            break;
        case floatmode_t::fixed:
            ostr.setf(std::ios::floatfield, std::ios::fixed);
            break;
        case floatmode_t::scientific:
            ostr.setf(std::ios::floatfield, std::ios::scientific);
            break;
        }
    }

    /**
     * @brief Helper function: Print the elements of a tensor recursively from
     * the first axis to the last axis.
     */
    template <class charT, class traits, class T, size_t Rank, class Tag>
    void print_tensor(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr,
        index_t<Rank> &index, size_t axis,
        size_t width
    );

    /**
     * @brief Helper function. Print a value.
     */
    template <class charT, class traits, class T>
    inline std::basic_ostream<charT, traits>& print_value(
        std::basic_ostream<charT, traits> &ostr, const T &rhs
    ) {
        return ostr << rhs;
    }

    /**
     * @brief Helper function. Print a value. Partial specialization for
     * complex numbers.
     */
    template <class charT, class traits, class T>
    inline std::basic_ostream<charT, traits>& print_value(
        std::basic_ostream<charT, traits> &ostr, const std::complex<T> &rhs
    ) {
        using namespace printoptions;
        switch (complexmode) {
        case complexmode_t::defaultmode:
            return ostr << rhs;
        case complexmode_t::algebraic:
            std::basic_ostringstream<charT, traits> buffer;
            buffer.flags(ostr.flags());
            buffer.imbue(ostr.getloc());
            buffer.precision(ostr.precision());
            buffer << rhs.real() << std::showpos << rhs.imag() << "i";
            return ostr << buffer.str();
        }
        return ostr;
    }

    /**
     * @brief Helper function: Print the elements of a tensor along a given
     * axis. The remaining axes are fixed with a given index.
     */
    template <class charT, class traits, class T, size_t Rank, class Tag>
    void print_tensor_slice(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr,
        index_t<Rank> &index, size_t axis,
        size_t width
    ) {
        using namespace printoptions;
        size_t size = arr.shape(axis);
        size_t linesize = axis + 1;
        std::string delim = "";
        index[axis] = 0;
        while (index[axis] < size) {
            ostr << delim;
            linesize += delim.size();
            delim = ", ";
            if (size >= threshold && size > 2*edgeitems
                && edgeitems <= index[axis] && index[axis] < size - edgeitems) {
                delim = "..., ";
                if (linesize + delim.size() >= linewidth) {
                    ostr << "\n" << std::string(axis + 1, ' ');
                    linesize = axis + 1;
                }
                index[axis] = size - edgeitems;
                continue;
            }
            if (index[axis] > 0 && linesize + width >= linewidth) {
                ostr << "\n" << std::string(axis + 1, ' ');
                linesize = axis + 1;
            }
            ostr.width(width);
            print_value(ostr, arr[index]);
            linesize += width;
            ++index[axis];
        }
    }

    /**
     * @brief Helper function: Print a block of tensor elements. The first
     * axes are fixed with a given index while the later axes are printed
     * recursively.
     */
    template <class charT, class traits, class T, size_t Rank, class Tag>
    void print_tensor_block(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr,
        index_t<Rank> &index, size_t axis,
        size_t width
    ) {
        using namespace printoptions;
        size_t size = arr.shape(axis);
        std::string delim = "";
        index[axis] = 0;
        while (index[axis] < size) {
            ostr << delim;
            delim = ",";
            delim.append(arr.ndim() - axis - 1, '\n');
            delim.append(axis + 1, ' ');
            if (size >= threshold && size > 2*edgeitems
                && edgeitems <= index[axis] && index[axis] < size - edgeitems) {
                delim = "...,";
                delim.append(arr.ndim() - axis - 1, '\n');
                delim.append(axis + 1, ' ');
                index[axis] = size - edgeitems;
                continue;
            }
            print_tensor(ostr, arr, index, axis + 1, width);
            ++index[axis];
        }
    }

    /**
     * @brief Helper function: Get the common number of characters required
     * to print each value along the last axis.
     */
    template <class charT, class traits, class T, size_t Rank, class Tag>
    size_t print_tensor_width(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr,
        index_t<Rank> &index, size_t axis
    ) {
        using namespace printoptions;
        size_t size = arr.shape(axis);
        size_t max_width = 0;
        index[axis] = 0;
        while (index[axis] < size) {
            if (size >= threshold && size > 2*edgeitems
                && edgeitems <= index[axis] && index[axis] < size - edgeitems) {
                index[axis] = size - edgeitems;
                continue;
            }
            if (axis == arr.ndim() - 1) {
                std::basic_ostringstream<charT, traits> buffer;
                buffer.flags(ostr.flags());
                buffer.imbue(ostr.getloc());
                buffer.precision(ostr.precision());
                print_value(buffer, arr[index]);
                max_width = std::max(max_width, buffer.str().size());
            }
            else {
                max_width = std::max(
                    max_width, print_tensor_width(ostr, arr, index, axis + 1)
                );
            }
            ++index[axis];
        }
        return max_width;
    }

    template <class charT, class traits, class T, size_t Rank, class Tag>
    void print_tensor(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr,
        index_t<Rank> &index, size_t axis,
        size_t width
    ) {
        ostr << "[";
        if (axis == arr.ndim() - 1) {
            print_tensor_slice(ostr, arr, index, axis, width);
        }
        else {
            print_tensor_block(ostr, arr, index, axis, width);
        }
        ostr << "]";
    }
}

    template <class charT, class traits, class T, size_t Rank, class Tag>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr
    ) {
        std::ios_base::fmtflags default_flags = ostr.flags();
        size_t default_precision = ostr.precision();
        detail::set_printoptions_flags<T>(ostr);
        index_t<Rank> index;
        size_t width = detail::print_tensor_width(ostr, arr, index, 0);
        detail::print_tensor(ostr, arr, index, 0, width);
        ostr.flags(default_flags);
        ostr.precision(default_precision);
        return ostr;
    }
}

#endif // NUMCPP_IO_TCC_INCLUDED
