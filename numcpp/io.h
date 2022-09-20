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

 /** @file include/numcpp/io.h
  *  This header defines input and output operations for tensor classes.
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_IO_H_INCLUDED
#define NUMCPP_IO_H_INCLUDED

#include "numcpp/config.h"
#include <iosfwd>

namespace numcpp {
    /// Context manager for setting print options.
    namespace printoptions {
        /**
         * @brief Number of digits of precision for floating point output. To
         * always use the maximum precission of each floating point type, set
         * to fullprecision.
         */
        size_t precision = 8;
        enum { fullprecision = -1 };

        /**
         * @brief Total number of array elements which trigger summarization
         * rather than full representation. To always use the full
         * representation without summarization, set to nothreshold.
         */
        size_t threshold = 1000;
        enum { nothreshold = -1 };

        /**
         * @brief Number of array items in summary at beginning and end of each
         * dimension.
         */
        size_t edgeitems = 3;

        /**
         * @brief Number of characters per line for the purpose of inserting
         * line breaks.
         */
        size_t linewidth = 80;

        enum class floatmode_t {
            /**
             * @brief Write floating-point values in default notation.
             */
            defaultmode,
            /**
             * @brief Write floating-point values in fixed-point notation.
             */
            fixed,
            /**
             * @brief Write floating-point values in scientific notation.
             */
            scientific
        };

        /**
         * @brief Controls the interpretation of the precision option for
         * floating-point types.
         */
        floatmode_t floatmode = floatmode_t::defaultmode;

        enum class complexmode_t {
            /**
             * @brief Read and write complex values in default notation, i.e.,
             * as a pair (real,imag).
             */
            defaultmode,
            /**
             * @brief Read and write complex values in algebraic notation,
             * i.e., as real + imag i.
             */
            algebraic
        };

        /**
         * @brief Controls the input and output format of complex types.
         */
        complexmode_t complexmode = complexmode_t::defaultmode;
    }

    /// Binary data.

    /**
     * @brief Load tensor contents from a binary file in NumPy .npy format.
     *
     * @tparam T Type of the elements contained in the tensor. Must be an
     *     arithmetic (either integer or floating-point type) or complex type.
     * @tparam Rank Dimension of the tensor.
     *
     * @param filename The file to read.
     *
     * @return A new tensor with its contents loaded from the file.
     *
     * @throw std::runtime_error If the input file does not exist or cannot be
     *     read; or if the tensor's data type and rank does not match the ones
     *     stored in the file.
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class T, size_t Rank>
    tensor<T, Rank> load(const std::string &filename);

    /**
     * @brief Save tensor contents to a binary file in NumPy .npy format.
     *
     * @param filename Filename to which the data is saved.
     * @param data Tensor data to be saved. Only arithmetic and complex types
     *     are supported.
     *
     * @throw std::runtime_error If the output file cannot be written.
     */
    template <class T, size_t Rank, class Tag>
    void save(
        const std::string &filename, const base_tensor<T, Rank, Tag> &data
    );

    /// Input/output streams

    /**
     * @brief Overloads input stream for shape_t objects. The supported format
     * for input extraction is (elem1, elem2, ..., elemN) if N > 1 and (elem1,)
     * or elem1 if N == 1.
     */
    template <class charT, class traits, size_t Rank>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape
    );

    template <class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, shape_t<1> &shape
    );

    /**
     * @brief Overloads output stream for shape_t objects. The format for
     * output extraction is (elem1, elem2, ..., elemN) if N > 1 and (elem1,)
     * if N == 1.
     */
    template <class charT, class traits, size_t Rank>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const shape_t<Rank> &shape
    );

    template <class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const shape_t<1> &shape
    );

    /**
     * @brief Overloads input stream for slice objects. The supported formats
     * for input extraction are (size), (start,size) and (start,size,stride).
     */
    template <class charT, class traits>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, slice &slc
    );

    /**
     * @brief Overloads output stream for slice objects. The format for
     * output extraction is (start,size,stride).
     */
    template <class charT, class traits>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr, const slice &slc
    );

    /**
     * @brief Extracts a tensor from an input stream. The format for input
     * extraction is a list of values separated by commas and delimited by
     * brackets:
     *     [rhs[0], rhs[1], rhs[2], ... , rhs[n - 1]]
     * Each value in the list might be a list as well. If extraction fails, an
     * empty tensor is written and failbit flag is set.
     *
     * @param istr Input stream object.
     * @param rhs Reference to a tensor object to store the extracted values.
     *
     * @return The input stream object.
     *
     * @throw std::bad_alloc If the function fails to allocate storage it may
     *     throw an exception.
     */
    template <class charT, class traits, class T, size_t Rank>
    std::basic_istream<charT, traits>& operator>>(
        std::basic_istream<charT, traits> &istr, tensor<T, Rank> &arr
    );

    /**
     * @brief Inserts a tensor into the output stream. The format for output
     * insertion is the same as input extraction.
     *
     * @param ostr Output stream object.
     * @param arr A tensor-like object to insert.
     *
     * @return The output stream object.
     */
    template <class charT, class traits, class T, size_t Rank, class Tag>
    std::basic_ostream<charT, traits>& operator<<(
        std::basic_ostream<charT, traits> &ostr,
        const base_tensor<T, Rank, Tag> &arr
    );
}

#include "numcpp/io/io.tcc"

#endif // NUMCPP_IO_H_INCLUDED
