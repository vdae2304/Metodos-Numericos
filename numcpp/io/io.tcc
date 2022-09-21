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

#include <cstdint>
#include <complex>
#include <ios>
#include <fstream>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace numcpp {
    /// Binary data.

namespace detail {
    /**
     * @brief Return a serializable descriptor from the data type.
     *
     * @note Since many of these have platform-dependent definitions, the use
     * of fixed width data types is suggested (<cstding>) for cross-platform
     * compatibility.
     *
     * Full documentation:
     * @link https://numpy.org/doc/stable/user/basics.types.html @endlink
     */
    template <class T>
    inline const char* dtype_to_descr(); // Not defined for non-arithmetic or
                                         // complex types.

    template <>
    inline const char* dtype_to_descr<bool>() {
        return "|b1";
    }

    template <>
    inline const char* dtype_to_descr<signed char>() {
        return "|i1";
    }

    template <>
    inline const char* dtype_to_descr<unsigned char>() {
        return "|u1";
    }

    template <>
    inline const char* dtype_to_descr<short>() {
        return "<i2";
    }

    template <>
    inline const char* dtype_to_descr<unsigned short>() {
        return "<u2";
    }

    template <>
    inline const char* dtype_to_descr<int>() {
        return (sizeof(int) == 4) ? "<i4" : "<i2";
    }

    template <>
    inline const char* dtype_to_descr<unsigned int>() {
        return (sizeof(unsigned int) == 4) ? "<u4" : "<u2";
    }

    template <>
    inline const char* dtype_to_descr<long>() {
        return (sizeof(long) == 8) ? "<i8" : "<i4";
    }

    template <>
    inline const char* dtype_to_descr<unsigned long>() {
        return (sizeof(unsigned long) == 8) ? "<u8" : "<u4";
    }

    template <>
    inline const char* dtype_to_descr<long long>() {
        return "<i8";
    }

    template <>
    inline const char* dtype_to_descr<unsigned long long>() {
        return "<u8";
    }

    template <>
    inline const char* dtype_to_descr<float>() {
        return "<f4";
    }

    template <>
    inline const char* dtype_to_descr<double>() {
        return "<f8";
    }

    template <>
    inline const char* dtype_to_descr<long double>() {
        return (sizeof(long double) == 16) ? "<f16" : "<f12";
    }

    template <>
    inline const char* dtype_to_descr< std::complex<float> >() {
        return "<c8";
    }

    template <>
    inline const char* dtype_to_descr< std::complex<double> >() {
        return "<c16";
    }

    template <>
    inline const char* dtype_to_descr< std::complex<long double> >() {
        return (sizeof(long double) == 16) ? "<c32" : "<c24";
    }

    /**
     * @brief Parse a field from a tensor header.
     */
    std::string parse_array_header(
        const std::string &header, const std::string &field
    ) {
        size_t start, end, match = header.find(field);
        if (match == std::string::npos) {
            throw std::runtime_error("File is corrupted or malformed");
        }
        char prefix = header[match - 1];
        char suffix = header[match + field.size()];
        if (prefix != '\"' && prefix != '\'' && suffix != prefix) {
            throw std::runtime_error("File is corrupted or malformed");
        }
        start = header.find_first_not_of(": ", match + field.size() + 1);
        if (header[start] == '\"' || header[start] == '\'') {
            ++start;
            end = header.find_first_of(header[start - 1], start);
        }
        else if (header[start] == '(') {
            end = header.find_first_of(')', start) + 1;
        }
        else {
            end = header.find_first_of(",}", start);
        }
        return header.substr(start, end - start);
    }

    /**
     * @brief Read the tensor header from a .npy file.
     * Full documentation about the format of .npy files can be fount in @link
     * https://numpy.org/doc/stable/reference/generated/numpy.lib.format.html
     * @endlink
     */
    template <class T, size_t Rank>
    void read_array_header(
        std::ifstream &file, int version, shape_t<Rank> &shape, layout_t &order
    ) {
        std::string header;
        // Version 1.0 uses 2 bytes for the length while version 2.0 uses 4.
        if (version < 2) {
            std::uint16_t header_len;
            file.read(reinterpret_cast<char*>(&header_len), 2);
            header.resize(header_len);
        }
        else {
            std::uint32_t header_len;
            file.read(reinterpret_cast<char*>(&header_len), 4);
            header.resize(header_len);
        }
        file.read(&header[0], header.size());
        // Check it is a valid header.
        if (header.front() != '{' || header.back() != '}') {
            throw std::runtime_error("File is corrupted or malformed");
        }
        // Parse "descr" field.
        std::string descr = parse_array_header(header, "descr");
        if (descr != dtype_to_descr<T>()) {
            std::ostringstream error;
            error << "Stored data type \"" << descr << "\" doesn't match the"
                  << " desired data type \"" << dtype_to_descr<T>() << "\"";
            throw std::runtime_error(error.str());
        }
        // Parse "fortran_order" field.
        std::string f_order = parse_array_header(header, "fortran_order");
        if (f_order == "True") {
            order = col_major;
        }
        else if (f_order == "False") {
            order = row_major;
        }
        else {
            throw std::runtime_error("File is corrupted or malformed");
        }
        // Parse "shape" field.
        std::string a_shape = parse_array_header(header, "shape");
        std::istringstream parser(a_shape);
        if (!(parser >> shape)) {
            std::ostringstream error;
            error << "Shape " << a_shape << " is not a valid shape of rank "
                  << Rank;
            throw std::runtime_error(error.str());
        }
    }

    /**
     * @brief Read the tensor's content from a .npy file.
     */
    template <class T, class OutputIterator>
    void read_array(
        std::ifstream &file, OutputIterator first, OutputIterator last
    ) {
        while (first != last) {
            file.read(reinterpret_cast<char*>(&*first), sizeof(T));
            ++first;
        }
    }
}

    template <class T, size_t Rank>
    tensor<T, Rank> load(const std::string &filename) {
        std::ifstream file(filename, std::ifstream::binary);
        if (!file) {
            std::ostringstream error;
            error << "Input file " << filename << " does not exist or cannot"
                  << " be read";
            throw std::runtime_error(error.str());
        }
        uint8_t version[2];
        file.ignore(6);
        file.read(reinterpret_cast<char*>(version), 2);
        shape_t<Rank> shape;
        layout_t order;
        detail::read_array_header<T>(file, version[0], shape, order);
        tensor<T, Rank> out(shape);
        detail::read_array<T>(file, out.begin(order), out.end(order));
        return out;
    }

namespace detail {
    /**
     * @brief Writes the tensor header to a .npy file.
     */
    template <class T, size_t Rank>
    void write_array_header(
        std::ofstream &file, const shape_t<Rank> &shape, layout_t order
    ) {
        std::ostringstream buffer;
        std::string descr = dtype_to_descr<T>();
        std::string f_order = (order == col_major) ? "True" : "False";
        buffer << "{\"descr\": \"" << descr << "\", \"fortran_order\": "
               << f_order << ", \"shape\": " << shape << "}";
        std::string header = buffer.str();
        std::uint16_t header_len = header.size();
        file.write(reinterpret_cast<char*>(&header_len), 2);
        file.write(header.data(), header_len);
    }

    /**
     * @brief Write the tensor's contents to a .npy file.
     */
    template <class T, class InputIterator>
    void write_array(
        std::ofstream &file, InputIterator first, InputIterator last
    ) {
        while (first != last) {
            T val = *first;
            file.write(reinterpret_cast<char*>(&val), sizeof(T));
            ++first;
        }
    }
}

    template <class T, size_t Rank, class Tag>
    void save(
        const std::string &filename, const base_tensor<T, Rank, Tag> &data
    ) {
        std::ofstream file(filename, std::ofstream::binary);
        if (!file) {
            std::ostringstream error;
            error << "Ouput file " << filename << " cannot be written";
            throw std::runtime_error(error.str());
        }
        file.write("\x93NUMPY", 6);
        file.write("\x01", 1);
        file.write("\x00", 1);
        detail::write_array_header<T>(file, data.shape(), data.layout());
        detail::write_array<T>(file, data.begin(), data.end());
        file.close();
    }

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
    inline std::basic_istream<charT, traits>& read_value(
        std::basic_istream<charT, traits> &istr, T &rhs
    ) {
        return istr >> rhs;
    }

    /**
     * @brief Helper function: Read a value. Partial specialization for complex
     * numbers.
     */
    template <class charT, class traits, class T>
    std::basic_istream<charT, traits>& read_value(
        std::basic_istream<charT, traits> &istr, std::complex<T> &rhs
    ) {
        using namespace printoptions;
        switch (complexmode) {
        case complexmode_t::defaultmode:
            return istr >> rhs;
        case complexmode_t::algebraic:
            T x, y;
            charT ch;
            bool fail = true;
            if (istr >> x) {
                if (!istr.eof()) {
                    charT plus = istr.widen('+'), minus = istr.widen('-');
                    istr >> ch;
                    if (traits::eq(ch, plus) || traits::eq(ch, minus)) {
                        bool sign = traits::eq(ch, minus);
                        if (istr >> y >> ch) {
                            if (traits::eq(ch, istr.widen('i'))) {
                                rhs = std::complex<T>(x, sign ? -y : y);
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
        return istr;
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
            arr = std::move(tensor<T, Rank>(buffer.begin(), shape));
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
