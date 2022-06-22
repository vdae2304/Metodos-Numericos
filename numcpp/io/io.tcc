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
    void __assert_file_is_open(
        std::ifstream &file, const std::string &filename
    ) {
        if (!file) {
            std::ostringstream error;
            error << "No such file or directory: " << filename;
            throw std::runtime_error(error.str());
        }
    }

    /// Helper function: Throws a std::runtime_error if the output file could
    /// not be opened.
    void __assert_file_is_open(
        std::ofstream &file, const std::string &filename
    ) {
        if (!file) {
            std::ostringstream error;
            error << "Unable to create or modify file: " << filename;
            throw std::runtime_error(error.str());
        }
    }

    /// Binary files

    template <class T>
    array<T> load(const std::string &filename) {
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
        std::ofstream file(filename, std::ofstream::binary);
        __assert_file_is_open(file, filename);
        size_t n = arr.size();
        file.write(reinterpret_cast<const char *>(&n), sizeof(size_t));
        file.write(reinterpret_cast<const char *>(arr.data()), n*sizeof(T));
    }

    template <class T>
    matrix<T> load_matrix(const std::string &filename) {
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
        std::ofstream file(filename, std::ofstream::binary);
        __assert_file_is_open(file, filename);
        size_t m = mat.rows(), n = mat.cols();
        file.write(reinterpret_cast<const char *>(&m), sizeof(size_t));
        file.write(reinterpret_cast<const char *>(&n), sizeof(size_t));
        file.write(reinterpret_cast<const char *>(mat.data()), m*n*sizeof(T));
    }

    /// Text files

    /// Helper function: Find the number of rows and columns of the data
    /// stored in the text file. This function does not read any data, it only
    /// returns the shape of the data stored.
    void __scan_data_shape_in_file(
        std::ifstream &file, size_t &nrows, size_t &ncols, char delimiter,
        bool header
    ) {
        std::string line, token;
        if (header) {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        for (nrows = 0, ncols = 0; std::getline(file, line); ++nrows) {
            std::istringstream tokenizer(line);
            size_t width;
            for (width = 0; std::getline(tokenizer, token, delimiter); ++width)
                {}
            ncols = std::max(ncols, width);
        }
        file.clear();
        file.seekg(0, std::ios::beg);
    }

    /// Helper function: Return the column names (header) from an ifstream
    /// object.
    template <class Tag>
    void __load_header_from_fstream(
        std::ifstream &file, size_t ncols, char delimiter, 
        base_array<std::string, Tag> &names
    ) {
        names.resize(ncols);
        std::string line, token;
        std::getline(file, line);
        std::istringstream tokenizer(line);
        for (size_t j = 0; std::getline(tokenizer, token, delimiter); ++j) {
            names[j] = token;
        }
    }

    /// Helper function: Return a new matrix with data from an ifstream object.
    template <class T>
    matrix<T> __load_data_from_fstream(
        std::ifstream &file, size_t nrows, size_t ncols, char delimiter
    ) {
        matrix<T> mat(nrows, ncols);
        std::string line, token;
        for (size_t i = 0; std::getline(file, line); ++i) {
            std::istringstream tokenizer(line);
            for (size_t j = 0; std::getline(tokenizer, token, delimiter); ++j) {
                std::istringstream in(token);
                in >> mat(i, j);
            }
        }
        return mat;
    }

    template <class T>
    matrix<T> load_txt(
        const std::string &filename, char delimiter, bool header
    ) {
        std::ifstream file(filename);
        __assert_file_is_open(file, filename);
        size_t nrows, ncols;
        __scan_data_shape_in_file(file, nrows, ncols, delimiter, header);
        if (header) {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return __load_data_from_fstream<T>(file, nrows, ncols, delimiter);
    }

    template <class T>
    matrix<T> load_txt(
        const std::string &filename, char delimiter, bool header,
        array<std::string> &names
    ) {
        std::ifstream file(filename);
        __assert_file_is_open(file, filename);
        size_t nrows, ncols;
        __scan_data_shape_in_file(file, nrows, ncols, delimiter, header);
        if (header) {
            __load_header_from_fstream(file, ncols, delimiter, names);
        }
        return __load_data_from_fstream<T>(file, nrows, ncols, delimiter);
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

    /// Helper function: Save the column names (header) to an ofstream object.
    template <class Tag>
    void __save_header_to_fstream(
        std::ofstream &file, char delimiter, 
        const base_array<std::string, Tag> &names
    ) {
        if (!names.empty()) {
            file << names[0];
            for (size_t i = 1; i < names.size(); ++i) {
                file << delimiter << names[i];
            }
            file << "\n";
        }
    }

    /// Helper function: Save the matrix data to an ofstream object.
    template <class T, class Tag>
    void __save_data_to_fstream(
        std::ofstream &file, const base_matrix<T, Tag> &mat, char delimiter
    ) {
        if (!mat.empty()) {
            for (size_t i = 0; i < mat.rows(); ++i) {
                file << mat(i, 0);
                for (size_t j = 1; j < mat.cols(); ++j) {
                    file << delimiter << mat(i, j);
                }
                file << "\n";
            }
        }
    }

    template <class T, class Tag>
    void save_txt(
        const std::string &filename, const base_matrix<T, Tag> &mat, 
        char delimiter
    ) {
        std::ofstream file(filename);
        __assert_file_is_open(file, filename);
        __set_printoptions_flags<T>(file);
        __save_data_to_fstream(file, mat, delimiter);
    }

    template <class T, class Tag1, class Tag2>
    void save_txt(
        const std::string &filename, const base_matrix<T, Tag1> &mat, 
        char delimiter,
        const base_array<std::string, Tag2> &names
    ) {
        std::ofstream file(filename);
        __assert_file_is_open(file, filename);
        __set_printoptions_flags<T>(file);
        __save_header_to_fstream(file, delimiter, names);
        __save_data_to_fstream(file, mat, delimiter);
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
                while (istr >> val >> ch) {
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

    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, array<T> &arr
    ) {
        size_t size;
        std::vector<T> buffer;
        if (__read_array(istr, size, buffer)) {
            arr = array<T>(buffer.begin(), buffer.end());
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
            mat = matrix<T>(buffer.begin(), buffer.end(), ncols);
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
            ostr << *first;
            while (++first != last) {
                ostr << ", ";
                ostr.width(width);
                ostr << *first;
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
                        buffer << mat(i, j);
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
