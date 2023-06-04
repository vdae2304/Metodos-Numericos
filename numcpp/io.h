/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library that
 * provides support for multidimensional arrays, and defines an assortment of
 * routines for fast operations on them, including mathematical, logical,
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

#include <cstdint>
#include <iosfwd>
#include "numcpp/config.h"

namespace numcpp {
/// Context manager for setting print options.
namespace printoptions {
/**
 * @brief Number of digits of precision for floating-point output. To always use
 * the maximum precision of each floating-point type, set to @a fullprecision.
 */
size_t precision = 8;
constexpr size_t fullprecision = SIZE_MAX;

/**
 * @brief Total number of array elements which trigger summarization rather than
 * full representation. To always use the full representation without
 * summarization, set to @a nothreshold.
 */
size_t threshold = 1000;
constexpr size_t nothreshold = SIZE_MAX;

/**
 * @brief Number of array items in summary at beginning and end of each
 * dimension.
 */
size_t edgeitems = 3;

/**
 * @brief Number of characters per line for the purpose of inserting line
 * breaks.
 */
size_t linewidth = 80;

enum floatmode_t {
  /**
   * @brief Write floating-point values in default notation.
   */
  defaultfloat,
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
 * @brief Controls the interpretation of the @a precision option for
 * floating-point types.
 */
floatmode_t floatmode = defaultfloat;

enum complexmode_t {
  /**
   * @brief Read and print complex values in default notation, i.e., as a pair
   * @a (real,imag).
   */
  defaultcomplex,
  /**
   * @brief Read and print complex values in arithmetic notation, i.e., as
   * @a real+imag*i.
   */
  arithmetic
};

/**
 * @brief Controls the input and output format of complex types.
 */
complexmode_t complexmode = defaultcomplex;
} // namespace printoptions

/// Binary data.

/**
 * @brief Load tensor contents from a binary file in NumPy @c .npy format.
 *
 * @tparam T Type of the elements contained in the tensor. Must be an arithmetic
 *     type (either integer or floating-point) or a complex type.
 * @tparam Rank Dimension of the tensor.
 *
 * @param filename A string representing the name of the file to load.
 *
 * @return A new tensor with its contents loaded from the file.
 *
 * @throw std::ios_base::failure Thrown if the input file doesn't exist or
 *                               cannot be read.
 * @throw std::invalid_argument Thrown if either T or Rank doesn't match the
 *                              data type and dimension stored in the input
 *                              file. Be aware that some data types might have
 *                              platform dependent definitions.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank>
tensor<T, Rank> load(const std::string &filename);

/**
 * @brief Save tensor contents to a binary file in NumPy @c .npy format.
 *
 * @param filename A string representing the name of the file destination.
 * @param data Tensor data to be saved. Only arithmetic types (either integer or
 *             floating-point) and complex types are supported.
 *
 * @throw std::ios_base::failure Thrown if the output file cannot be written.
 */
template <class Container, class T, size_t Rank>
void save(const std::string &filename,
          const expression<Container, T, Rank> &data);

/// Text files.

/**
 * @brief Load tensor data from a text file.
 *
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. Must be 1 or 2.
 *
 * @param filename A string representing the name of the file to load.
 * @param delimiter Character used to separate values. Defaults to whitespace.
 * @param newline Character used to separate lines. Defaults to end of line.
 * @param skiprows Skip the first @a skiprows lines. Default is 0.
 * @param max_rows Read @a max_rows lines of content after @a skiprows lines.
 *                 The default is to read all the lines.
 * @param usecols An initializer_list indicating which columns to read, with 0
 *                being the first. If not provided, read all the columns.
 *
 * @return A new tensor with its contents loaded from the file.
 *
 * @throw std::ios_base::failure Thrown if the input file doesn't exist or
 *                               cannot be read.
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class T, size_t Rank>
tensor<T, Rank> loadtxt(const std::string &filename, char delimiter = ' ',
                        char newline = '\n', size_t skiprows = 0,
                        size_t max_rows = SIZE_MAX,
                        std::initializer_list<size_t> usecols = {});

/**
 * @brief Save a tensor to a text file.
 *
 * @param filename A string representing the name of the file destination.
 * @param data A 1-dimensional or 2-dimensional tensor with the data to be saved
 *             to a text file.
 * @param delimiter Character used to separate values. Defaults to whitespace.
 * @param newline Character used to separate lines. Defaults to end of line.
 * @param header String that will be written at the beginning of the file.
 * @param footer String that will be written at the end of the file.
 *
 * @throw std::ios_base::failure Thrown if the output file cannot be written.
 */
template <class Container, class T, size_t Rank>
void savetxt(const std::string &filename,
             const expression<Container, T, Rank> &data, char delimiter = ' ',
             char newline = '\n', const std::string &header = "",
             const std::string &footer = "");

/// Input/output streams.

/**
 * @brief Extracts a tensor from an input stream.
 *
 * @details The format for input extraction is a list of values separated by
 * commas and delimited by brackets:
 *     [arg_1, arg_2, arg_3 ... , arg_n]
 * Each value in the list might be a list as well. However, the number of nested
 * lists must be equal to the dimension of the tensor to extract. If extraction
 * fails, an empty tensor is written and failbit flag is set.
 *
 * @param istr Input stream object.
 * @param arg Tensor to be extracted from the input stream.
 *
 * @return istr
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, tensor<T, Rank> &arg);

/**
 * @brief Inserts a tensor into the output stream.
 *
 * @details The format for output insertion is the same as input extraction,
 * i.e., a list of values separated by commas and delimited by brackets.
 *
 * @param ostr Output stream object.
 * @param arg A tensor-like object to be inserted into the output stream.
 *
 * @return ostr
 */
template <class charT, class traits, class Container, class T, size_t Rank>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr,
           const expression<Container, T, Rank> &arg);
} // namespace numcpp

#include "numcpp/io/io.tcc"

#endif // NUMCPP_IO_H_INCLUDED
