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

/**
 * @brief Controls printing of the sign of numeric values. If true, always print
 * the sign of positive values. If false, omit the sign of positive values.
 */
bool sign = false;

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
 * @param file File object to read.
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

template <class T, size_t Rank> tensor<T, Rank> load(std::istream &file);

/**
 * @brief Save tensor contents to a binary file in NumPy @c .npy format.
 *
 * @param filename A string representing the name of the file destination.
 * @param file File object to write.
 * @param data Tensor data to be saved. Only arithmetic types (either integer or
 *             floating-point) and complex types are supported.
 *
 * @throw std::ios_base::failure Thrown if the output file cannot be written.
 */
template <class Container, class T, size_t Rank>
void save(const std::string &filename,
          const expression<Container, T, Rank> &data);

template <class Container, class T, size_t Rank>
void save(std::ostream &file, const expression<Container, T, Rank> &data);

/// Text files.

/**
 * @brief Load tensor data from a text file.
 *
 * @tparam T Type of the elements contained in the tensor.
 * @tparam Rank Dimension of the tensor. Must be 1 or 2.
 *
 * @param filename A string representing the name of the file to load.
 * @param file File object to read.
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

template <class T, size_t Rank>
tensor<T, Rank> loadtxt(std::istream &file, char delimiter = ' ',
                        char newline = '\n', size_t skiprows = 0,
                        size_t max_rows = SIZE_MAX,
                        std::initializer_list<size_t> usecols = {});

/**
 * @brief Save a tensor to a text file.
 *
 * @param filename A string representing the name of the file destination.
 * @param file File object to write.
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

template <class Container, class T, size_t Rank>
void savetxt(std::ostream &file, const expression<Container, T, Rank> &data,
             char delimiter = ' ', char newline = '\n',
             const std::string &header = "", const std::string &footer = "");

/// Input/output streams.

/**
 * @brief Extracts a tensor from the input stream.
 *
 * @details The format for input extraction is a list of values separated by
 * commas and delimited by brackets:
 *     [a1, a2, a3, ..., an]
 * Each value in the list can be a list as well. However, the depth of nested
 * lists must be equal to the dimension of the tensor to extract. For example,
 * a 1-dimensional tensor is formatted as
 *     [a1, a2, a3, ..., an]
 * while a 2-dimensional is formatted as
 *     [[a11, a12, a13, ..., a1n],
 *      [a21, a22, a23, ..., a2n],
 *      ...,
 *      [am1, am2, am3, ..., amn]]
 * 
 * String values must be delimited by either single quotes (') or double quotes
 * (").
 * 
 * If extraction fails, an empty tensor is written and failbit flag is set.
 *
 * @param istr Input stream object.
 * @param a Tensor to be extracted from the input stream.
 *
 * @return istr
 *
 * @throw std::bad_alloc If the function fails to allocate storage it may throw
 *                       an exception.
 */
template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, tensor<T, Rank> &a);

/**
 * @brief Inserts a tensor into the output stream.
 *
 * @details The format for output insertion is the same as input extraction,
 * i.e., a list of values separated by commas and delimited by brackets.
 * 
 * Floating-point values are formatted according to @c precision and
 * @c floatmode options (see printoptions).
 * 
 * String values are enclosed with double quotes (").
 *
 * @param ostr Output stream object.
 * @param a A tensor-like object to be inserted into the output stream.
 *
 * @return ostr
 */
template <class charT, class traits, class Container, class T, size_t Rank>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr,
           const expression<Container, T, Rank> &a);

/**
 * @brief Return a string representation of a tensor.
 *
 * @param a Input tensor.
 * @param formatter Custom formatter. A function that accepts an element of type
 *                  T as argument, and returns a value convertible to string.
 * @param precision Floating point precision. Defaults to
 *                  @a printoptions::precision
 * @param threshold Total number of array elements which trigger summarization
 *                  rather than full representation. Defaults to
 *                  @a printoptions::threshold
 * @param edgeitems Number of array items in summary at beginning and end of
 *                  each dimension. Defaults to @a printoptions::edgeitems
 * @param linewidth Insert newlines if text is longer than @a linewidth.
 *                  Defaults to @a printoptions::linewidth
 * @param sign Controls the sign of numeric values. If true, always print the
 *             sign of positive values. If false, omit the sign of positive
 *             values. Default is @a printoptions::sign.
 * @param floatmode Controls the interpretation of the precision option for
 *                  floating-point types. Defaults to @a printoptions::floatmode
 * @param prefix String to insert at the beginning of each dimension.
 * @param suffix String to insert the end of each dimension.
 * @param separator String to insert bewtween elements.
 *
 * @return String representation of the tensor.
 */
template <class Container, class T, size_t Rank>
std::string
to_string(const expression<Container, T, Rank> &a,
          size_t precision = printoptions::precision,
          size_t threshold = printoptions::threshold,
          size_t edgeitems = printoptions::edgeitems,
          size_t linewidth = printoptions::linewidth,
          bool sign = printoptions::sign,
          printoptions::floatmode_t floatmode = printoptions::floatmode,
          const std::string &prefix = "[", const std::string &suffix = "]",
          const std::string &separator = ", ");

template <class Container, class T, size_t Rank, class Function,
          detail::RequiresCallable<Function, T> = 0>
std::string
to_string(const expression<Container, T, Rank> &a, Function formatter,
          size_t threshold = printoptions::threshold,
          size_t edgeitems = printoptions::edgeitems,
          size_t linewidth = printoptions::linewidth,
          const std::string &prefix = "[", const std::string &suffix = "]",
          const std::string &separator = ", ");
} // namespace numcpp

#include "numcpp/io/io.tcc"

#endif // NUMCPP_IO_H_INCLUDED
