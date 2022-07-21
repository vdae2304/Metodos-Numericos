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

 /** @file include/numcpp/io/io.tcc
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_IO_TCC_INCLUDED
#define NUMCPP_IO_TCC_INCLUDED

#include <fstream>
#include <ios>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

namespace numcpp {
    /// Helper function: Throws a std::runtime_error if the input file could
    /// not be opened.
    template <class charT, class traits>
    inline void __assert_file_is_open(
        std::basic_ifstream<charT, traits> &file, const std::string &filename
    ) {
        if (!file) {
            std::ostringstream error;
            error << "No such file or directory: " << filename;
            throw std::runtime_error(error.str());
        }
    }

    /// Helper function: Throws a std::runtime_error if the output file could
    /// not be opened.
    template <class charT, class traits>
    inline void __assert_file_is_open(
        std::basic_ofstream<charT, traits> &file, const std::string &filename
    ) {
        if (!file) {
            std::ostringstream error;
            error << "Unable to create or modify file: " << filename;
            throw std::runtime_error(error.str());
        }
    }

    /// Helper function: Reads an object using default input stream operator.
    template <class T, class charT, class traits>
    inline std::basic_istream<charT, traits>& __read(
        std::basic_istream<charT, traits> &istr, T &rhs
    ) {
        return istr >> rhs;
    }

    /// Helper function: Prints an object using default output stream operator.
    template <class T, class charT, class traits>
    inline std::basic_ostream<charT, traits>& __print(
        std::basic_ostream<charT, traits> &ostr, const T &rhs
    ) {
        return ostr << rhs;
    }

    /// Binary files

    template <class T>
    array<T> load(const std::string &filename) {
        static_assert(std::is_fundamental<T>::value,
                      "T must be a fundamental type");
        std::ifstream file(filename, std::ifstream::binary);
        __assert_file_is_open(file, filename);
        size_t n;
        file.read(reinterpret_cast<char *>(&n), sizeof(size_t));
        array<T> arr(n);
        file.read(reinterpret_cast<char *>(arr.data()), n*sizeof(T));
        return arr;
    }

    template <class T>
    void save(const std::string &filename, const array<T> &arr) {
        static_assert(std::is_fundamental<T>::value,
                      "T must be a fundamental type");
        std::ofstream file(filename, std::ofstream::binary);
        __assert_file_is_open(file, filename);
        size_t n = arr.size();
        file.write(reinterpret_cast<const char *>(&n), sizeof(size_t));
        file.write(reinterpret_cast<const char *>(arr.data()), n*sizeof(T));
    }

    template <class T>
    matrix<T> load_matrix(const std::string &filename) {
        static_assert(std::is_fundamental<T>::value,
                      "T must be a fundamental type");
        std::ifstream file(filename, std::ifstream::binary);
        __assert_file_is_open(file, filename);
        size_t m, n;
        file.read(reinterpret_cast<char *>(&m), sizeof(size_t));
        file.read(reinterpret_cast<char *>(&n), sizeof(size_t));
        matrix<T> mat(m, n);
        file.read(reinterpret_cast<char *>(mat.data()), m*n*sizeof(T));
        return mat;
    }

    template <class T>
    void save_matrix(const std::string &filename, const matrix<T> &mat) {
        static_assert(std::is_fundamental<T>::value,
                      "T must be a fundamental type");
        std::ofstream file(filename, std::ofstream::binary);
        __assert_file_is_open(file, filename);
        size_t m = mat.rows(), n = mat.cols();
        file.write(reinterpret_cast<const char *>(&m), sizeof(size_t));
        file.write(reinterpret_cast<const char *>(&n), sizeof(size_t));
        file.write(reinterpret_cast<const char *>(mat.data()), m*n*sizeof(T));
    }

    /// Text files

    /// Parse a value from a string.
    template <class T, class charT, class traits>
    inline void __parse(const std::basic_string<charT, traits> &str, T &val) {
        std::basic_istringstream<charT, traits> parser(str);
        __read(parser, val);
    }

    /// Parse a value from a string. If already a string, removes leading and
    /// trailing whitespaces.
    template <class charT, class traits>
    inline void __parse(
        const std::basic_string<charT, traits> &str,
        std::basic_string<charT, traits> &val
    ) {
        val = str;
        val.erase(0, val.find_first_not_of(" \f\n\r\t\v"));
        val.erase(val.find_last_not_of(" \f\n\r\t\v") + 1);
    }

    /// Helper function: Find the number of rows and columns of the data
    /// stored in the text file. This function does not read any data, it only
    /// returns the size of the data stored.
    template <class charT, class traits>
    void __scan_file_size(
        std::basic_ifstream<charT, traits> &file, size_t &nrows, size_t &ncols,
        charT delimiter, charT newline, size_t skiprows, size_t max_rows
    ) {
        for (size_t i = 0; i < skiprows; ++i) {
            file.ignore(std::numeric_limits<std::streamsize>::max(), newline);
        }
        std::streampos offset = file.tellg();
        std::basic_string<charT, traits> line, token;
        nrows = 0;
        ncols = 0;
        while (nrows < max_rows && std::getline(file, line, newline)) {
            std::basic_istringstream<charT, traits> tokenizer(line);
            size_t width = 0;
            while (std::getline(tokenizer, token, delimiter)) {
                ++width;
            }
            ++nrows;
            ncols = std::max(ncols, width);
        }
        file.clear();
        file.seekg(offset, std::ios::beg);
    }

    /// Helper function: Return the header from an ifstream object.
    template <class charT, class traits>
    void __load_file_header(
        std::basic_ifstream<charT, traits> &file,
        array< std::basic_string<charT, traits> > &header,
        charT delimiter, charT newline
    ) {
        std::basic_string<charT, traits> line, token;
        std::getline(file, line, newline);
        std::basic_istringstream<charT, traits> tokenizer(line);
        std::vector< std::basic_string<charT, traits> > buffer;
        while (std::getline(tokenizer, token, delimiter)) {
            __parse(token, token);
            buffer.push_back(token);
        }
        header.resize(buffer.size());
        for (size_t i = 0; i < buffer.size(); ++i) {
            header[i] = buffer[i];
        }
    }

    /// Helper function: Return the header from an ifstream object. Read only
    /// the specified columns.
    template <class charT, class traits>
    void __load_file_header(
        std::basic_ifstream<charT, traits> &file,
        array< std::basic_string<charT, traits> > &header,
        charT delimiter, charT newline,
        std::initializer_list<size_t> usecols
    ) {
        std::basic_string<charT, traits> line, token;
        std::getline(file, line, newline);
        std::basic_istringstream<charT, traits> tokenizer(line);
        std::vector< std::basic_string<charT, traits> > buffer;
        while (std::getline(tokenizer, token, delimiter)) {
            __parse(token, token);
            buffer.push_back(token);
        }
        header.resize(usecols.size());
        for (size_t i = 0; i < usecols.size(); ++i) {
            header[i] = buffer[*(usecols.begin() + i)];
        }
    }

    /// Helper function: Return a new matrix with data from an ifstream object.
    template <class T, class charT, class traits>
    matrix<T> __load_file_data(
        std::basic_ifstream<charT, traits> &file, size_t nrows, size_t ncols,
        charT delimiter, charT newline
    ) {
        matrix<T> mat(nrows, ncols);
        std::basic_string<charT, traits> line, token;
        for (size_t i = 0; i < nrows; ++i) {
            std::getline(file, line, newline);
            std::basic_istringstream<charT, traits> tokenizer(line);
            for (size_t j = 0; j < ncols; ++j) {
                std::getline(tokenizer, token, delimiter);
                __parse(token, mat(i, j));
            }
        }
        return mat;
    }

    /// Helper function: Return a new matrix with data from an ifstream object.
    /// Read only the specified columns.
    template <class T, class charT, class traits>
    matrix<T> __load_file_data(
        std::basic_ifstream<charT, traits> &file, size_t nrows, size_t ncols,
        charT delimiter, charT newline,
        std::initializer_list<size_t> usecols
    ) {
        matrix<T> mat(nrows, usecols.size());
        std::basic_string<charT, traits> line, token;
        for (size_t i = 0; i < nrows; ++i) {
            std::getline(file, line, newline);
            std::basic_istringstream<charT, traits> tokenizer(line);
            std::vector<T> buffer(ncols);
            for (size_t j = 0; j < ncols; ++j) {
                std::getline(tokenizer, token, delimiter);
                __parse(token, buffer[j]);
            }
            for (size_t j = 0; j < usecols.size(); ++j) {
                mat(i, j) = buffer[*(usecols.begin() + j)];
            }
        }
        return mat;
    }

    template <class T>
    matrix<T> load_txt(
        const std::string &filename, char delimiter, char newline,
        size_t skiprows, size_t max_rows,
        std::initializer_list<size_t> usecols
    ) {
        std::ifstream file(filename, std::ifstream::binary);
        __assert_file_is_open(file, filename);
        size_t m, n;
        __scan_file_size(file, m, n, delimiter, newline, skiprows, max_rows);
        if (usecols.size() == 0) {
            return __load_file_data<T>(file, m, n, delimiter, newline);
        }
        else {
            return __load_file_data<T>(file, m, n, delimiter, newline, usecols);
        }
    }

    template <class T>
    matrix<T> load_txt(
        const std::string &filename, array<std::string> &header,
        char delimiter, char newline,
        size_t skiprows, size_t max_rows,
        std::initializer_list<size_t> usecols
    ) {
        std::ifstream file(filename, std::ifstream::binary);
        __assert_file_is_open(file, filename);
        if (usecols.size() == 0) {
            __load_file_header(file, header, delimiter, newline);
        }
        else {
            __load_file_header(file, header, delimiter, newline, usecols);
        }
        size_t m, n;
        __scan_file_size(file, m, n, delimiter, newline, skiprows, max_rows);
        if (usecols.size() == 0) {
            return __load_file_data<T>(file, m, n, delimiter, newline);
        }
        else {
            return __load_file_data<T>(file, m, n, delimiter, newline, usecols);
        }
    }

    /// Helper function: Set all the print options flags on the output stream
    /// object.
    template <class T, class charT, class traits>
    void __set_printoptions_flags(std::basic_ostream<charT, traits> &ostr) {
        if (printoptions::precision == (size_t)printoptions::fullprecision) {
            ostr.precision(std::numeric_limits<T>::max_digits10);
        }
        else {
            ostr.precision(printoptions::precision);
        }
        if (printoptions::sign) {
            ostr.setf(std::ios_base::showpos);
        }
        else {
            ostr.unsetf(std::ios_base::showpos);
        }
        switch (printoptions::floatmode) {
            case printoptions::defaultmode:
                ostr.unsetf(std::ios::floatfield);
                break;
            case printoptions::fixed:
                ostr.setf(std::ios::floatfield, std::ios::fixed);
                break;
            case printoptions::scientific:
                ostr.setf(std::ios::floatfield, std::ios::scientific);
                break;
        }
    }

    /// Helper function: Save the header to an ofstream object.
    template <class charT, class traits, class Tag>
    void __save_file_header(
        std::basic_ofstream<charT, traits> &file,
        const base_array<std::basic_string<charT, traits>, Tag> &header,
        charT delimiter, charT newline
    ) {
        if (!header.empty()) {
            file << header[0];
            for (size_t i = 1; i < header.size(); ++i) {
                file << delimiter << header[i];
            }
            file << newline;
        }
    }

    /// Helper function: Save the matrix data to an ofstream object.
    template <class T, class Tag, class charT, class traits>
    void __save_file_data(
        std::basic_ofstream<charT, traits> &file,
        const base_matrix<T, Tag> &mat,
        charT delimiter, charT newline
    ) {
        if (!mat.empty()) {
            for (size_t i = 0; i < mat.rows(); ++i) {
                __print(file, mat(i, 0));
                for (size_t j = 1; j < mat.cols(); ++j) {
                    file << delimiter;
                    __print(file, mat(i, j));
                }
                file << newline;
            }
        }
    }

    template <class T, class Tag>
    void save_txt(
        const std::string &filename, const base_matrix<T, Tag> &mat,
        char delimiter, char newline
    ) {
        std::ofstream file(filename);
        __assert_file_is_open(file, filename);
        __set_printoptions_flags<T>(file);
        __save_file_data(file, mat, delimiter, newline);
    }

    template <class T, class Tag, class TagHeader>
    void save_txt(
        const std::string &filename, const base_matrix<T, Tag> &mat,
        const base_array<std::string, TagHeader> &header,
        char delimiter, char newline
    ) {
        std::ofstream file(filename);
        __assert_file_is_open(file, filename);
        __set_printoptions_flags<T>(file);
        __save_file_header(file, header, delimiter, newline);
        __save_file_data(file, mat, delimiter, newline);
    }

    /// Input stream

    /// Helper function: Read a list of values and append them at the end of a
    /// std::vector. Returns whether the operation was successful or not.
    template <class T, class charT, class traits>
    bool __read_array(
        std::basic_istream<charT, traits> &istr,
        size_t &size, std::vector<T> &buffer
    ) {
        charT ch;
        T val;
        size = 0;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('['))) {
                while (__read(istr, val) && istr >> ch) {
                    ++size;
                    buffer.push_back(val);
                    if (traits::eq(ch, istr.widen(']'))) {
                        return true;
                    }
                    else if (!traits::eq(ch, istr.widen(','))) {
                        istr.putback(ch);
                        return false;
                    }
                }
            }
            else {
                istr.putback(ch);
                return false;
            }
        }
        return false;
    }

    /// Helper function: Read a list of values and append them at the end of a
    /// std::vector. Returns whether the operation was successful or not.
    /// Specialization for string objects.
    template <class charT, class traits>
    bool __read_array(
        std::basic_istream<charT, traits> &istr,
        size_t &size, std::vector< std::basic_string<charT, traits> > &buffer
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
                    __parse(token, token);
                    buffer.push_back(token);
                }
                return true;
            }
            else {
                istr.putback(ch);
                return false;
            }
        }
        return false;
    }

    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, array<T> &arr
    ) {
        size_t size;
        std::vector<T> buffer;
        if (__read_array(istr, size, buffer)) {
            arr = std::move(array<T>(buffer.begin(), buffer.end()));
        }
        else {
            istr.setstate(std::ios_base::failbit);
            arr = array<T>();
        }
        return istr;
    }

    /// Helper function: Read a list of lists and append the values at the end
    /// of a std::vector. All inner lists must be of equal length. Returns
    /// whether the operation was successful or not.
    template <class T, class charT, class traits>
    bool __read_matrix(
        std::basic_istream<charT, traits> &istr,
        size_t &nrows, size_t &ncols, std::vector<T> &buffer
    ) {
        charT ch;
        size_t width;
        nrows = 0;
        ncols = 0;
        if (istr >> ch) {
            if (traits::eq(ch, istr.widen('['))) {
                while (__read_array(istr, width, buffer) && istr >> ch) {
                    if (nrows > 0 && width != ncols) {
                        return false;
                    }
                    ++nrows;
                    ncols = width;
                    if (traits::eq(ch, istr.widen(']'))) {
                        return true;
                    }
                    else if (!traits::eq(ch, istr.widen(','))) {
                        istr.putback(ch);
                        return false;
                    }
                }
            }
            else {
                istr.putback(ch);
                return false;
            }
        }
        return false;
    }

    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, matrix<T> &mat
    ) {
        size_t nrows, ncols;
        std::vector<T> buffer;
        if (__read_matrix(istr, nrows, ncols, buffer)) {
            mat = std::move(matrix<T>(buffer.begin(), buffer.end(), ncols));
        }
        else {
            istr.setstate(std::ios_base::failbit);
            mat = matrix<T>();
        }
        return istr;
    }

    /// Output stream

    /// Helper function: Print a subset of elements of an array-like object.
    template <class charT, class traits, class ArrayIterator>
    void __print_subarray(
        std::basic_ostream<charT, traits> &ostr,
        ArrayIterator first, ArrayIterator last,
        size_t width
    ) {
        if (first != last) {
            ostr.width(width);
            __print(ostr, *first);
            while (++first != last) {
                ostr << ", ";
                ostr.width(width);
                __print(ostr, *first);
            }
        }
    }

    /// Helper function: Print the elements of an array-like object.
    template <class charT, class traits, class ArrayIterator>
    void __print_array(
        std::basic_ostream<charT, traits> &ostr,
        ArrayIterator first, ArrayIterator last,
        size_t width
    ) {
        using namespace printoptions;
        size_t size = last - first;
        ostr << "[";
        if (size < threshold || size <= 2*edgeitems) {
            __print_subarray(ostr, first, last, width);
        }
        else {
            __print_subarray(ostr, first, first + edgeitems, width);
            ostr << ", ..., ";
            __print_subarray(ostr, last - edgeitems, last, width);
        }
        ostr << "]";
    }

    template <class T, class Tag, class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const base_array<T, Tag> &arr
    ) {
        std::ios_base::fmtflags default_flags = ostr.flags();
        size_t default_precision = ostr.precision();
        __set_printoptions_flags<T>(ostr);
        __print_array(ostr, arr.begin(), arr.end(), 0);
        ostr.flags(default_flags);
        ostr.precision(default_precision);
        return ostr;
    }

    /// Helper function: Print a subset of rows of a matrix-like object.
    template <class charT, class traits, class MatrixIterator>
    void __print_submatrix(
        std::basic_ostream<charT, traits> &ostr,
        size_t nrows, size_t ncols, MatrixIterator first,
        size_t width
    ) {
        if (nrows > 0) {
            __print_array(ostr, first, first + ncols, width);
            for (size_t i = 1; i < nrows; ++i) {
                first += ncols;
                ostr << ",\n ";
                __print_array(ostr, first, first + ncols, width);
            }
        }
    }

    /// Helper function: Print the elements of a matrix-like object.
    template <class charT, class traits, class MatrixIterator>
    void __print_matrix(
        std::basic_ostream<charT, traits> &ostr,
        size_t nrows, size_t ncols, MatrixIterator first,
        size_t width
    ) {
        using namespace printoptions;
        ostr << "[";
        if (nrows < threshold || nrows <= 2*edgeitems) {
            __print_submatrix(ostr, nrows, ncols, first, width);
        }
        else {
            __print_submatrix(ostr, edgeitems, ncols, first, width);
            ostr << "\n ...\n ";
            first += (nrows - edgeitems)*ncols;
            __print_submatrix(ostr, edgeitems, ncols, first, width);
        }
        ostr << "]";
    }

    /// Helper function: Return the maximum width of all columns.
    template <class T, class Tag, class charT, class traits>
    size_t __column_width(
        std::basic_ostream<charT, traits> &ostr, const base_matrix<T, Tag> &mat
    ) {
        using namespace printoptions;
        std::basic_ostringstream<charT, traits> buffer;
        buffer.flags(ostr.flags());
        buffer.imbue(ostr.getloc());
        buffer.precision(ostr.precision());
        size_t width = 0;
        for (size_t i = 0; i < mat.rows(); ++i) {
            if (
                mat.rows() < threshold ||
                mat.rows() <= 2*edgeitems ||
                i < edgeitems ||
                i >= mat.rows() - edgeitems
            ) {
                for (size_t j = 0; j < mat.cols(); ++j) {
                    if (
                        mat.cols() < threshold ||
                        mat.cols() <= 2*edgeitems ||
                        j < edgeitems ||
                        j >= mat.cols() - edgeitems
                    ) {
                        buffer.str("");
                        __print(buffer, mat(i, j));
                        width = std::max(width, buffer.str().size());
                    }
                }
            }
        }
        return width;
    }

    template <class T, class Tag, class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const base_matrix<T, Tag> &mat
    ) {
        std::ios_base::fmtflags default_flags = ostr.flags();
        size_t default_precision = ostr.precision();
        __set_printoptions_flags<T>(ostr);
        size_t width = __column_width(ostr, mat);
        __print_matrix(ostr, mat.rows(), mat.cols(), mat.begin(true), width);
        ostr.flags(default_flags);
        ostr.precision(default_precision);
        return ostr;
    }
}

#endif // NUMCPP_IO_TCC_INCLUDED
