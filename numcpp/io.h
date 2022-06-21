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

 /** @file include/numcpp/io.h
  *  This header defines input and output operations for array and matrix 
  *  classes.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_IO_H_INCLUDED
#define NUMCPP_IO_H_INCLUDED

#include "numcpp/config.h"
#include <iosfwd>

namespace numcpp {
    /// Context manager for setting print options.
    namespace printoptions {
        /// Number of digits of precision for floating point output. To always 
        /// use the maximum precission of each floating point type, set to 
        /// fullprecision.
        size_t precision = 8;
        enum { fullprecision = -1 };

        /// Total number of array elements which trigger summarization rather 
        /// than full representation. To always use the full representation 
        /// without summarization, set to nothreshold.
        size_t threshold = 1000;
        enum { nothreshold = -1 };

        /// Number of array items in summary at beginning and end of each 
        /// dimension.
        size_t edgeitems = 3;

        /// Controls printing of the sign of numeric types. If true, always 
        /// print the sign of positive values. If false, omit the sign 
        /// character of positive values.
        bool sign = false;

        enum floatmode_t {
            /// Write floating-point values in default notation.
            defaultmode, 
            /// Write floating-point values in fixed-point notation.
            fixed, 
            /// Write floating-point values in scientific notation.
            scientific
        };

        /// Controls the interpretation of the precision option for 
        /// floating-point types.
        floatmode_t floatmode = defaultmode;
    }

    /// Binary files

    /** 
     * @brief Return a new array from a binary file.
     * 
     * @param filename A string representing the name of the file to load.
     * 
     * @return A new array with its contents loaded from the file.
     * 
     * @throw std::runtime_error Thrown if the file could not be opened.
     * @throw std::bad_alloc If the function fails to allocate storage it may 
     *     throw an exception.
     */
    template <class T>
    array<T> load(const std::string &filename);

    /** 
     * @brief Save an array to a binary file.
     * 
     * @param filename A string representing the name of the file destination.
     * @param arr Array data to be saved.
     * 
     * @throw std::runtime_error Thrown if the file could not be created or 
     *     overwritten.
     */
    template <class T>
    void save(const std::string &filename, const array<T> &arr);

    /** 
     * @brief Return a new matrix from a binary file.
     * 
     * @param filename A string representing the name of the file to load.
     * 
     * @return A new matrix with its contents loaded from the file.
     * 
     * @throw std::runtime_error Thrown if the file could not be opened.
     * @throw std::bad_alloc If the function fails to allocate storage it may 
     *     throw an exception.
     */
    template <class T>
    matrix<T> load_matrix(const std::string &filename);

    /** 
     * @brief Save a matrix to a binary file.
     * 
     * @param filename A string representing the name of the file destination.
     * @param mat Matrix data to be saved.
     * 
     * @throw std::runtime_error Thrown if the file could not be created or 
     *     overwritten.
     */
    template <class T>
    void save_matrix(const std::string &filename, const matrix<T> &mat);

    /// Text files

    /** 
     * @brief Return a new matrix from a text file.
     * 
     * @param filename A string representing the name of the file to load.
     * @param delimiter The character used to separate values.
     * @param header If true, the first row in the text file is interpreted as 
     *     the column names, and the start of the data is set at the second 
     *     row. If false, the start of the data is set at the first row.
     * @param names An array of string. If given and header is true, then the 
     *     column names are saved to names.
     * 
     * @return A new matrix with its contents loaded from the file.
     * 
     * @throw std::runtime_error Thrown if the file could not be opened.
     * @throw std::bad_alloc If the function fails to allocate storage it may 
     *     throw an exception.
     */
    template <class T>
    matrix<T> load_txt(
        const std::string &filename, char delimiter, bool header
    );
    
    template <class T>
    matrix<T> load_txt(
        const std::string &filename, char delimiter, bool header, 
        array<std::string> &names
    );

    /** 
     * @brief Save a matrix to a text file.
     * 
     * @param filename A string representing the name of the file destination.
     * @param mat Matrix data to be saved.
     * @param delimiter The character used to separate values.
     * @param names An array of string. If given, then the first row in the 
     *     text file will contain the column names as given by names.
     * 
     * @throw std::runtime_error Thrown if the file could not be opened.
     */
    template <class T, class Tag>
    void save_txt(
        const std::string &filename, const base_matrix<T, Tag> &mat, 
        char delimiter
    );

    template <class T, class Tag1, class Tag2>
    void save_txt(
        const std::string &filename, const base_matrix<T, Tag1> &mat, 
        char delimiter,
        const base_array<std::string, Tag2> &names
    );

    /// Input/output streams

    /**
     * @brief Extracts an array from an input stream. The format for input 
     * extraction is a list of values separated by commas and delimited by 
     * brackets: 
     *     [rhs[0], rhs[1], rhs[2], ... , rhs[n - 1]]
     * If extraction fails, an empty array is written and failbit flag is set.
     * 
     * @param istr Input stream object.
     * @param rhs Reference to an array object to store the extracted values.
     * 
     * @return The input stream object.
     * 
     * @throw std::bad_alloc If the function fails to allocate storage it may 
     *     throw an exception.
     */
    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, array<T> &arr
    );

    /**
     * @brief Extracts a matrix from an input stream. The format for input 
     * extraction is a list of lists separated by commas and delimited by 
     * brackets. The inner lists are formatted with the same format. All inner 
     * lists must be of equal length.
     *     [[rhs(0, 0), rhs(0, 1), ... , rhs(0, n - 1)],
     *      [rhs(1, 0), rhs(1, 1), ... , rhs(1, n - 1)],
     *      ... ,
     *      [rhs(m - 1, 0), rhs(m - 1, 1), ... , rhs(m - 1, n - 1)]]
     * If extraction fails, an empty matrix is written and failbit flag is set.
     * 
     * @param istr Input stream object.
     * @param rhs Reference to a matrix object to store the extracted values.
     * 
     * @return The input stream object.
     * 
     * @throw std::bad_alloc If the function fails to allocate storage it may 
     *     throw an exception.
     */
    template <class T, class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, matrix<T> &mat
    );
    
    /**
     * @brief Inserts an array into the output stream. The format for output 
     * insertion is the same as input extraction.
     * 
     * @param ostr Output stream object.
     * @param arr An array or array_view object to insert.
     * 
     * @return The output stream object.
     */
    template <class T, class Tag, class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const base_array<T, Tag> &arr
    );

    /**
     * @brief Inserts a matrix into the output stream. The format for output 
     * insertion is the same as input extraction.
     * 
     * @param ostr Output stream object.
     * @param mat A matrix or matrix_view object to insert.
     * 
     * @return The output stream object.
     */
    template <class T, class Tag, class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const base_matrix<T, Tag> &mat
    );
}

#include "numcpp/io/io.tcc"

#endif // NUMCPP_IO_H_INCLUDED
