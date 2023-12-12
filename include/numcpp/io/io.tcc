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

/** @file include/numcpp/io/io.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{numcpp/io.h}
 */

// Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_IO_TCC_INCLUDED
#define NUMCPP_IO_TCC_INCLUDED

#include <algorithm>
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
 * @brief Find the endiannes of scalar types.
 *
 * @return '<' for little endian, '>' for big endian.
 */
inline char endian() {
  std::uint16_t word = 0x0001;
  std::uint8_t *byte = reinterpret_cast<std::uint8_t *>(&word);
  return byte[0] ? '<' : '>';
}

/**
 * @brief Reverse the bytes of a given integer.
 */
template <class T> inline T byteswap(T n) {
  std::uint8_t *bytes = reinterpret_cast<std::uint8_t *>(&n);
  std::reverse(bytes, bytes + sizeof(T));
  T *ptr = reinterpret_cast<T *>(bytes);
  return *ptr;
}

/**
 * @brief Return a serializable descriptor from the data type.
 *
 * @note Many of these have platform-dependent definitions.
 *
 * Full documentation: @link
 * https://numpy.org/doc/stable/reference/arrays.dtypes.html @endlink
 */
template <class T> inline std::string dtype_to_descr();

inline std::string dtype_to_descr_impl(char byteorder, char kind,
                                       size_t itemsize) {
  std::ostringstream buffer;
  buffer << '\'' << byteorder << kind << itemsize << '\'';
  return buffer.str();
}

/// Specialization for fundamental types.

template <> inline std::string dtype_to_descr<bool>() {
  return dtype_to_descr_impl('|', 'b', sizeof(bool));
}

template <> inline std::string dtype_to_descr<signed char>() {
  return dtype_to_descr_impl('|', 'i', sizeof(signed char));
}

template <> inline std::string dtype_to_descr<unsigned char>() {
  return dtype_to_descr_impl('|', 'u', sizeof(unsigned char));
}

template <> inline std::string dtype_to_descr<short>() {
  return dtype_to_descr_impl(endian(), 'i', sizeof(short));
}

template <> inline std::string dtype_to_descr<unsigned short>() {
  return dtype_to_descr_impl(endian(), 'u', sizeof(unsigned short));
}

template <> inline std::string dtype_to_descr<int>() {
  return dtype_to_descr_impl(endian(), 'i', sizeof(int));
}

template <> inline std::string dtype_to_descr<unsigned int>() {
  return dtype_to_descr_impl(endian(), 'u', sizeof(unsigned int));
}

template <> inline std::string dtype_to_descr<long>() {
  return dtype_to_descr_impl(endian(), 'i', sizeof(long));
}

template <> inline std::string dtype_to_descr<unsigned long>() {
  return dtype_to_descr_impl(endian(), 'u', sizeof(unsigned long));
}

template <> inline std::string dtype_to_descr<long long>() {
  return dtype_to_descr_impl(endian(), 'i', sizeof(long long));
}

template <> inline std::string dtype_to_descr<unsigned long long>() {
  return dtype_to_descr_impl(endian(), 'u', sizeof(unsigned long long));
}

template <> inline std::string dtype_to_descr<float>() {
  return dtype_to_descr_impl(endian(), 'f', sizeof(float));
}

template <> inline std::string dtype_to_descr<double>() {
  return dtype_to_descr_impl(endian(), 'f', sizeof(double));
}

template <> inline std::string dtype_to_descr<long double>() {
  return dtype_to_descr_impl(endian(), 'f', sizeof(long double));
}

template <> inline std::string dtype_to_descr<std::complex<float>>() {
  return dtype_to_descr_impl(endian(), 'c', sizeof(std::complex<float>));
}

template <> inline std::string dtype_to_descr<std::complex<double>>() {
  return dtype_to_descr_impl(endian(), 'c', sizeof(std::complex<double>));
}

template <> inline std::string dtype_to_descr<std::complex<long double>>() {
  return dtype_to_descr_impl(endian(), 'c', sizeof(std::complex<long double>));
}

/**
 * @brief Read the magic string to get the version of the file format.
 */
void read_magic(std::istream &file, std::uint8_t &major, std::uint8_t &minor) {
  std::string magic(6, ' ');
  file.read(&magic[0], 6);
  if (magic != "\x93NUMPY") {
    throw std::ios_base::failure("File is not a valid .npy file");
  }
  file.read(reinterpret_cast<char *>(&major), 1);
  file.read(reinterpret_cast<char *>(&minor), 1);
}

/**
 * @brief Parse a Python dictionary from a string. Return whether the operation
 * was successful.
 */
bool parse_pydict(const std::string &str,
                  std::vector<std::pair<std::string, std::string>> &dict);
bool parse_pydict_key(std::istringstream &parser, std::string &key);
bool parse_pydict_value(std::istringstream &parser, std::string &value);

/**
 * @brief Read the array header from a .npy file.
 *
 * @note Full documentation about the format of .npy files at @link
 * https://numpy.org/doc/stable/reference/generated/numpy.lib.format.html
 * @endlink
 */
template <class T, size_t Rank>
void read_array_header(std::istream &file, std::uint8_t version,
                       shape_t<Rank> &shape, layout_t &order) {
  std::string header;
  // Version 1.0 uses 2 bytes for the length while version 2.0 uses 4.
  if (version < 2) {
    std::uint16_t header_len;
    file.read(reinterpret_cast<char *>(&header_len), 2);
    if (endian() != '<') {
      header_len = byteswap(header_len);
    }
    header.resize(header_len);
  } else {
    std::uint32_t header_len;
    file.read(reinterpret_cast<char *>(&header_len), 4);
    if (endian() != '<') {
      header_len = byteswap(header_len);
    }
    header.resize(header_len);
  }
  file.read(&header[0], header.size());

  // Check whether header is a valid Pydict with the required fields.
  std::vector<std::pair<std::string, std::string>> dict;
  if (!parse_pydict(header, dict)) {
    throw std::ios_base::failure("File is corrupted or malformed");
  }
  std::sort(dict.begin(), dict.end());
  if (dict.size() != 3 || dict[0].first != "descr" ||
      dict[1].first != "fortran_order" || dict[2].first != "shape") {
    throw std::ios_base::failure("File is corrupted or malformed");
  }

  // Parse "descr" field.
  std::string descr = dict[0].second;
  if (descr != dtype_to_descr<T>()) {
    std::ostringstream error;
    error << "input file dtype " << descr << " doesn't match output dtype "
          << dtype_to_descr<T>();
    throw std::invalid_argument(error.str());
  }

  // Parse "fortran_order" field.
  std::string f_order = dict[1].second;
  if (f_order == "True") {
    order = column_major;
  } else if (f_order == "False") {
    order = row_major;
  } else {
    throw std::ios_base::failure("fortran_order must be True or False");
  }

  // Parse "shape" field.
  std::string a_shape = dict[2].second;
  std::istringstream parser(a_shape);
  if (!(parser >> shape)) {
    std::ostringstream error;
    error << "input file shape " << a_shape << " is not a valid shape of rank "
          << Rank;
    throw std::invalid_argument(error.str());
  }
}

bool parse_pydict(const std::string &str,
                  std::vector<std::pair<std::string, std::string>> &dict) {
  std::istringstream parser(str);
  std::string key, value;
  char delim;
  dict.clear();
  if (parser >> delim) {
    if (delim == '{') {
      while (parse_pydict_key(parser, key)) {
        if (parser >> delim) {
          if (delim == ':') {
            if (parse_pydict_value(parser, value)) {
              dict.emplace_back(key, value);
              if (parser >> delim) {
                if (delim == ',') {
                  continue;
                } else if (delim == '}') {
                  return true;
                }
              }
            }
          }
        }
        return false;
      }
      if (parser >> delim) {
        if (delim == '}') {
          return true;
        }
      }
    }
  }
  return false;
}

bool parse_pydict_key(std::istringstream &parser, std::string &key) {
  char delim;
  if (parser >> delim) {
    if (delim == '\"' || delim == '\'') {
      if (std::getline(parser, key, delim)) {
        return true;
      }
    } else {
      parser.putback(delim);
    }
  }
  return false;
}

bool parse_pydict_value(std::istringstream &parser, std::string &value) {
  char delim;
  if (parser >> delim) {
    switch (delim) {
    case '\"':
    case '\'':
      if (std::getline(parser, value, delim)) {
        value = '\'' + value + '\'';
        return true;
      }
      break;
    case '(':
      if (std::getline(parser, value, ')')) {
        value = '(' + value + ')';
        return true;
      }
      break;
    case '[':
      if (std::getline(parser, value, ']')) {
        value = '[' + value + ']';
        return true;
      }
      break;
    case '{':
      if (std::getline(parser, value, '}')) {
        value = '{' + value + '}';
        return true;
      }
      break;
    default:
      value = "";
      parser.putback(delim);
      while (parser.get(delim)) {
        if (!std::isalnum(delim) && delim != '_') {
          parser.putback(delim);
          return !value.empty();
        }
        value.push_back(delim);
      }
    }
  }
  return false;
}

/**
 * @brief Read the array's content from a .npy file.
 */
template <class T> void read_array(std::istream &file, T *data, size_t n) {
  std::streampos offset = file.tellg();
  file.seekg(0, std::ios::end);
  size_t bytesize = file.tellg() - offset;
  file.seekg(offset, std::ios::beg);
  if (bytesize < n * sizeof(T)) {
    throw std::ios_base::failure("File is corrupted or malformed");
  }
  file.read(reinterpret_cast<char *>(data), bytesize);
}
} // namespace detail

template <class T, size_t Rank>
tensor<T, Rank> load(const std::string &filename) {
  std::ifstream file(filename, std::ifstream::binary);
  if (!file) {
    std::ostringstream error;
    error << "Input file " << filename << " does not exist or cannot be read";
    throw std::ios_base::failure(error.str());
  }
  return load<T, Rank>(file);
}

template <class T, size_t Rank> tensor<T, Rank> load(std::istream &file) {
  std::uint8_t major, minor;
  detail::read_magic(file, major, minor);
  shape_t<Rank> shape;
  layout_t order;
  detail::read_array_header<T>(file, major, shape, order);
  tensor<T, Rank> out(shape, order);
  detail::read_array<T>(file, out.data(), out.size());
  return out;
}

namespace detail {
/**
 * @brief Write the magic string to a .npy file.
 */
void write_magic(std::ostream &file, std::uint8_t major, std::uint8_t minor) {
  file.write("\x93NUMPY", 6);
  file.write(reinterpret_cast<char *>(&major), 1);
  file.write(reinterpret_cast<char *>(&minor), 1);
}

/**
 * @brief Writes the array header to a .npy file.
 */
template <class T, size_t Rank>
void write_array_header(std::ostream &file, const shape_t<Rank> &shape,
                        layout_t order) {
  std::ostringstream buffer;
  std::string descr = dtype_to_descr<T>();
  std::string f_order = (order == column_major) ? "True" : "False";
  buffer << "{'descr': " << descr << ", 'fortran_order': " << f_order
         << ", 'shape': " << shape << "}";
  std::string header = buffer.str();
  std::uint16_t header_len = header.size();
  if (endian() != '<') {
    header_len = byteswap(header_len);
  }
  file.write(reinterpret_cast<char *>(&header_len), 2);
  file.write(header.data(), header_len);
}

/**
 * @brief Write the array's contents to a .npy file.
 */
template <class T, class InputIterator>
void write_array(std::ostream &file, InputIterator first, InputIterator last) {
  while (first != last) {
    T val = *first;
    file.write(reinterpret_cast<char *>(&val), sizeof(T));
    ++first;
  }
}
} // namespace detail

template <class Container, class T, size_t Rank>
void save(const std::string &filename,
          const expression<Container, T, Rank> &data) {
  std::ofstream file(filename, std::ofstream::binary);
  if (!file) {
    std::ostringstream error;
    error << "Ouput file " << filename << " cannot be written";
    throw std::ios_base::failure(error.str());
  }
  save(file, data);
  file.close();
}

template <class Container, class T, size_t Rank>
void save(std::ostream &file, const expression<Container, T, Rank> &data) {
  detail::write_magic(file, 1, 0);
  detail::write_array_header<T>(file, data.shape(), data.layout());
  detail::write_array<T>(file, data.self().begin(), data.self().end());
}

/// Text files.

namespace detail {
/**
 * @brief Fill a tensor with data from an istream object.
 */
template <class T>
void load_file_data(std::istream &file, tensor<T, 2> &data, char delimiter,
                    char newline, size_t max_rows) {
  std::string line, token;
  shape_t<2> shape;
  std::vector<T> buffer;
  T val;
  while (shape[0] < max_rows && std::getline(file, line, newline)) {
    std::istringstream tokenizer(line);
    size_t ntokens = 0;
    while (std::getline(tokenizer, token, delimiter)) {
      std::istringstream parser(token);
      parser >> val;
      buffer.push_back(val);
      ++ntokens;
    }
    if (shape[0] == 0) {
      shape[1] = ntokens;
    } else if (ntokens != shape[1]) {
      throw std::ios_base::failure(
          "Number of columns is not the same for all rows");
    }
    ++shape[0];
  }
  data = std::move(tensor<T, 2>(buffer.begin(), shape, row_major));
}

template <class T>
void load_file_data(std::istream &file, tensor<T, 2> &data, char delimiter,
                    char newline, size_t max_rows,
                    std::initializer_list<std::size_t> usecols) {
  std::string line, token;
  shape_t<2> shape(0, usecols.size());
  std::vector<T> buffer;
  T val;
  while (shape[0] < max_rows && std::getline(file, line, newline)) {
    std::istringstream tokenizer(line);
    size_t ntokens = 0;
    std::vector<T> row_values;
    while (std::getline(tokenizer, token, delimiter)) {
      std::istringstream parser(token);
      parser >> val;
      row_values.push_back(val);
      ++ntokens;
    }
    for (size_t i : usecols) {
      buffer.push_back(row_values[i]);
    }
    ++shape[0];
  }
  data = std::move(tensor<T, 2>(buffer.begin(), shape, row_major));
}

template <class T>
void load_file_data(std::istream &file, tensor<T, 1> &data, char, char newline,
                    size_t max_rows) {
  std::string line;
  size_t size = 0;
  std::vector<T> buffer;
  T val;
  while (size < max_rows && std::getline(file, line, newline)) {
    std::istringstream parser(line);
    parser >> val;
    buffer.push_back(val);
    ++size;
  }
  data = std::move(tensor<T, 1>(buffer.begin(), size));
}

template <class T>
void load_file_data(std::istream &file, tensor<T, 1> &data, char delimiter,
                    char newline, size_t max_rows,
                    std::initializer_list<std::size_t> usecols) {
  std::string line, token;
  size_t size = 0;
  std::vector<T> buffer;
  T val;
  while (size < max_rows && std::getline(file, line, newline)) {
    std::istringstream tokenizer(line);
    size_t ntokens = 0;
    while (std::getline(tokenizer, token, delimiter)) {
      if (ntokens == *usecols.begin()) {
        std::istringstream parser(token);
        parser >> val;
        buffer.push_back(val);
      }
      ++ntokens;
    }
    ++size;
  }
  data = std::move(tensor<T, 1>(buffer.begin(), size));
}
} // namespace detail

template <class T, size_t Rank>
tensor<T, Rank> loadtxt(const std::string &filename, char delimiter,
                        char newline, size_t skiprows, size_t max_rows,
                        std::initializer_list<size_t> usecols) {
  std::ifstream file(filename);
  if (!file) {
    std::ostringstream error;
    error << "Input file " << filename << " does not exist or cannot be read";
    throw std::ios_base::failure(error.str());
  }
  return loadtxt<T, Rank>(file, delimiter, newline, skiprows, max_rows,
                          usecols);
}

template <class T, size_t Rank>
tensor<T, Rank> loadtxt(std::istream &file, char delimiter, char newline,
                        size_t skiprows, size_t max_rows,
                        std::initializer_list<size_t> usecols) {
  static_assert(Rank == 1 || Rank == 2,
                "Output tensor must be 1-dimensional or 2-dimensional");
  for (size_t i = 0; i < skiprows; ++i) {
    file.ignore(std::numeric_limits<std::streamsize>::max(), newline);
  }
  tensor<T, Rank> data;
  if (usecols.size() == 0) {
    detail::load_file_data(file, data, delimiter, newline, max_rows);
  } else {
    detail::load_file_data(file, data, delimiter, newline, max_rows, usecols);
  }
  return data;
}

namespace detail {
/**
 * @brief Set all print options flags on the output stream object.
 */
template <class T, class charT, class traits>
void set_printoptions_flags(
    std::basic_ostream<charT, traits> &ostr,
    size_t precision = printoptions::precision, bool sign = printoptions::sign,
    printoptions::floatmode_t floatmode = printoptions::floatmode) {
  if (precision == printoptions::fullprecision) {
    ostr.precision(std::numeric_limits<T>::max_digits10);
  } else {
    ostr.precision(precision);
  }
  if (sign) {
    ostr.setf(std::ios::showpos);
  } else {
    ostr.unsetf(std::ios::showpos);
  }
  switch (floatmode) {
  case printoptions::defaultfloat:
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

/**
 * @brief Save the tensor's contents to a text file.
 */
template <class Container, class T>
void save_file_data(std::ostream &file, const expression<Container, T, 2> &data,
                    char delimiter, char newline) {
  size_t rows = data.shape(0), cols = data.shape(1);
  if (cols > 0) {
    for (size_t i = 0; i < rows; ++i) {
      file << data[{i, 0}];
      for (size_t j = 1; j < cols; ++j) {
        file << delimiter << data[{i, j}];
      }
      file << newline;
    }
  }
}

template <class Container, class T>
void save_file_data(std::ostream &file, const expression<Container, T, 1> &data,
                    char, char newline) {
  size_t size = data.size();
  for (size_t i = 0; i < size; ++i) {
    file << data[i] << newline;
  }
}
} // namespace detail

template <class Container, class T, size_t Rank>
void savetxt(const std::string &filename,
             const expression<Container, T, Rank> &data, char delimiter,
             char newline, const std::string &header,
             const std::string &footer) {
  std::ofstream file(filename);
  if (!file) {
    std::ostringstream error;
    error << "Ouput file " << filename << " cannot be written";
    throw std::ios_base::failure(error.str());
  }
  savetxt(file, data, delimiter, newline, header, footer);
}

template <class Container, class T, size_t Rank>
void savetxt(std::ostream &file, const expression<Container, T, Rank> &data,
             char delimiter, char newline, const std::string &header,
             const std::string &footer) {
  static_assert(Rank == 1 || Rank == 2,
                "Input tensor must be 1-dimensional or 2-dimensional");
  if (!header.empty()) {
    file << header << newline;
  }
  detail::set_printoptions_flags<T>(file);
  detail::save_file_data(file, data, delimiter, newline);
  if (!footer.empty()) {
    file << footer << newline;
  }
}

/// Input/output streams.

namespace detail {
/**
 * @brief Calls to operator>> on the input stream object.
 */
template <class charT, class traits, class T>
inline std::basic_istream<charT, traits> &
input_value(std::basic_istream<charT, traits> &istr, T &val) {
  return istr >> val;
}

/**
 * @brief Partial specialization for strings. Strings need to be delimited by
 * quotation marks when reading arrays of strings.
 */
template <class charT, class traits>
std::basic_istream<charT, traits> &
input_value(std::basic_istream<charT, traits> &istr,
            std::basic_string<charT, traits> &str) {
  charT ch;
  bool fail = true;
  if (istr >> ch) {
    if (traits::eq(ch, '\"') || traits::eq(ch, '\'')) {
      if (std::getline(istr, str, ch)) {
        fail = false;
      }
    } else {
      istr.putback(ch);
    }
  }
  if (fail) {
    istr.setstate(std::ios_base::failbit);
  }
  return istr;
}

/**
 * @brief Read the last axis in the tensor and append each value at the end of
 * a std::vector. Store the size of the axis and return whether the operation
 * was sucessfull.
 */
template <class charT, class traits, class T>
bool input_last_axis(std::basic_istream<charT, traits> &istr, size_t &size,
                     std::vector<T> &buffer) {
  charT ch;
  T val;
  size = 0;
  if (istr >> ch) {
    if (traits::eq(ch, istr.widen('['))) {
      while (input_value(istr, val)) {
        buffer.push_back(val);
        ++size;
        if (istr >> ch) {
          if (traits::eq(ch, istr.widen(','))) {
            continue;
          } else if (traits::eq(ch, istr.widen(']'))) {
            return true;
          } else {
            istr.putback(ch);
          }
        }
        break;
      }
    } else {
      istr.putback(ch);
    }
  }
  return false;
}

/**
 * @brief Read an axis in the tensor and append each value at the end of a
 * std::vector. Store the size of the axis and return whether the operation was
 * sucessfull.
 */
template <class charT, class traits, class T, size_t Rank>
bool input_axis(std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape,
                std::vector<T> &buffer, size_t axis = 0) {
  if (axis == Rank - 1) {
    return input_last_axis(istr, shape[axis], buffer);
  }

  charT ch;
  size_t n = 0;
  shape[axis] = 0;
  if (istr >> ch) {
    if (traits::eq(ch, istr.widen('['))) {
      while (input_axis(istr, shape, buffer, axis + 1)) {
        if (shape[axis] == 0) {
          n = shape[axis + 1];
        } else if (n != shape[axis + 1]) {
          break;
        }
        ++shape[axis];
        if (istr >> ch) {
          if (traits::eq(ch, istr.widen(','))) {
            continue;
          } else if (traits::eq(ch, istr.widen(']'))) {
            return true;
          } else {
            istr.putback(ch);
          }
        }
        break;
      }
    } else {
      istr.putback(ch);
    }
  }
  return false;
}
} // namespace detail

template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, tensor<T, Rank> &a) {
  shape_t<Rank> shape;
  std::vector<T> buffer;
  if (detail::input_axis(istr, shape, buffer)) {
    a = std::move(tensor<T, Rank>(buffer.begin(), shape, row_major));
  } else {
    a = std::move(tensor<T, Rank>());
    istr.setstate(std::ios_base::failbit);
  }
  return istr;
}

namespace detail {
/**
 * @brief Default formatter.
 */
template <class charT = char, class traits = std::char_traits<charT>>
struct default_formatter {
  /// Number of digits of precision for floating-point output.
  size_t precision;

  /// Controls printing of the sign of numeric values.
  bool sign;

  /// Controls the interpretation of the precision option for floating-point
  /// types.
  printoptions::floatmode_t floatmode;

  /**
   * @brief Constructor.
   */
  default_formatter(size_t precision, bool sign,
                    printoptions::floatmode_t floatmode)
      : precision(precision), sign(sign), floatmode(floatmode) {}

  /**
   * @brief Calls to operator<< on the output stream object.
   */
  template <class T>
  std::basic_string<charT, traits> operator()(const T &value) const {
    std::basic_ostringstream<charT, traits> ostr;
    set_printoptions_flags<T>(ostr, precision, sign, floatmode);
    ostr << value;
    return ostr.str();
  }

  /**
   * @brief Specialization for string types.
   */
  std::basic_string<charT, traits>
  operator()(const std::basic_string<charT, traits> &value) const {
    return "\"" + value + "\"";
  }
};

/**
 * @brief Print a value in the tensor. Insert a line break whenever the maximum
 * line width has been reached.
 */
template <class charT, class traits, class T, class Function>
void print_value(std::basic_ostream<charT, traits> &ostr, const T &value,
                 Function &formatter, size_t linewidth, size_t padding,
                 size_t &offset, size_t width) {
  if (offset + width >= linewidth) {
    ostr << "\n";
    if (padding > 0) {
      ostr.width(padding);
      ostr << " ";
      offset = padding;
    }
  }
  ostr.width(width);
  ostr << formatter(value);
  offset += width;
}

/**
 * @brief Print the last axis in the tensor.
 */
template <class charT, class traits, class Container, class T, size_t Rank,
          class Function>
void print_last_axis(std::basic_ostream<charT, traits> &ostr,
                     const expression<Container, T, Rank> &a,
                     Function &formatter, size_t threshold, size_t edgeitems,
                     size_t linewidth,
                     const std::basic_string<charT, traits> &prefix,
                     const std::basic_string<charT, traits> &suffix,
                     const std::basic_string<charT, traits> &separator,
                     size_t width, index_t<Rank> &index) {
  size_t axis = Rank - 1;
  size_t size = a.shape(axis);
  size_t padding = (axis + 1) * prefix.size();
  size_t offset = padding;

  ostr << prefix;
  if (size < threshold || size <= 2 * edgeitems) {
    for (index[axis] = 0; index[axis] < size; ++index[axis]) {
      if (index[axis] > 0) {
        ostr << separator;
        offset += separator.size();
      }
      print_value(ostr, a[index], formatter, linewidth, padding, offset, width);
    }
  } else {
    for (index[axis] = 0; index[axis] < edgeitems; ++index[axis]) {
      print_value(ostr, a[index], formatter, linewidth, padding, offset, width);
      ostr << separator;
      offset += separator.size();
    }
    ostr << "...";
    offset += 3;
    for (index[axis] = size - edgeitems; index[axis] < size; ++index[axis]) {
      ostr << separator;
      offset += separator.size();
      print_value(ostr, a[index], formatter, linewidth, padding, offset, width);
    }
  }
  ostr << suffix;
};

/**
 * @brief Print an axis in the tensor. Each axis is printed recursively.
 */
template <class charT, class traits, class Container, class T, size_t Rank,
          class Function>
void print_axis(std::basic_ostream<charT, traits> &ostr,
                const expression<Container, T, Rank> &a, Function &formatter,
                size_t threshold, size_t edgeitems, size_t linewidth,
                const std::basic_string<charT, traits> &prefix,
                const std::basic_string<charT, traits> &suffix,
                const std::basic_string<charT, traits> &separator, size_t width,
                index_t<Rank> &index, size_t axis = 0) {
  if (axis == Rank - 1) {
    print_last_axis(ostr, a, formatter, threshold, edgeitems, linewidth, prefix,
                    suffix, separator, width, index);
    return;
  }

  size_t size = a.shape(axis);
  std::basic_string<charT, traits> linebreak = separator;
  linebreak.append(Rank - 1 - axis, '\n');
  linebreak.append((axis + 1) * prefix.size(), ' ');

  ostr << prefix;
  if (size < threshold || size <= 2 * edgeitems) {
    for (index[axis] = 0; index[axis] < size; ++index[axis]) {
      if (index[axis] > 0) {
        ostr << linebreak;
      }
      print_axis(ostr, a, formatter, threshold, edgeitems, linewidth, prefix,
                 suffix, separator, width, index, axis + 1);
    }
  } else {
    for (index[axis] = 0; index[axis] < edgeitems; ++index[axis]) {
      print_axis(ostr, a, formatter, threshold, edgeitems, linewidth, prefix,
                 suffix, separator, width, index, axis + 1);
      ostr << linebreak;
    }
    ostr << "...";
    for (index[axis] = size - edgeitems; index[axis] < size; ++index[axis]) {
      ostr << linebreak;
      print_axis(ostr, a, formatter, threshold, edgeitems, linewidth, prefix,
                 suffix, separator, width, index, axis + 1);
    }
  }
  ostr << suffix;
}

/**
 * @brief Return the number of characters required to print each value in the
 * tensor.
 */
template <class Container, class T, size_t Rank, class Function>
size_t print_width(const expression<Container, T, Rank> &a, Function &formatter,
                   size_t threshold, size_t edgeitems, index_t<Rank> &index,
                   size_t axis = 0) {
  if (axis == Rank) {
    return formatter(a[index]).size();
  }
  size_t size = a.shape(axis);
  size_t width = 0;
  if (size < threshold || size <= 2 * edgeitems) {
    for (index[axis] = 0; index[axis] < size; ++index[axis]) {
      width = std::max(width, print_width(a, formatter, threshold, edgeitems,
                                          index, axis + 1));
    }
  } else {
    for (index[axis] = 0; index[axis] < edgeitems; ++index[axis]) {
      width = std::max(width, print_width(a, formatter, threshold, edgeitems,
                                          index, axis + 1));
    }
    for (index[axis] = size - edgeitems; index[axis] < size; ++index[axis]) {
      width = std::max(width, print_width(a, formatter, threshold, edgeitems,
                                          index, axis + 1));
    }
  }
  return width;
}
} // namespace detail

template <class charT, class traits, class Container, class T, size_t Rank>
inline std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr,
           const expression<Container, T, Rank> &a) {
  using namespace printoptions;
  detail::default_formatter<> formatter(precision, sign, floatmode);
  index_t<Rank> index;
  std::basic_string<charT, traits> prefix = "[", suffix = "]", separator = ", ";
  size_t width = detail::print_width(a, formatter, threshold, edgeitems, index);
  detail::print_axis(ostr, a, formatter, threshold, edgeitems, linewidth,
                     prefix, suffix, separator, width, index);
  return ostr;
}

template <class Container, class T, size_t Rank>
inline std::string
to_string(const expression<Container, T, Rank> &a, size_t precision,
          size_t threshold, size_t edgeitems, size_t linewidth, bool sign,
          printoptions::floatmode_t floatmode, const std::string &prefix,
          const std::string &suffix, const std::string &separator) {
  detail::default_formatter<> formatter(precision, sign, floatmode);
  return to_string(a, formatter, threshold, edgeitems, linewidth, prefix,
                   suffix, separator);
}

template <class Container, class T, size_t Rank, class Function,
          detail::RequiresCallable<Function, T>>
inline std::string
to_string(const expression<Container, T, Rank> &a, Function formatter,
          size_t threshold, size_t edgeitems, size_t linewidth,
          const std::string &prefix, const std::string &suffix,
          const std::string &separator) {
  std::ostringstream ostr;
  index_t<Rank> index;
  size_t width = detail::print_width(a, formatter, threshold, edgeitems, index);
  detail::print_axis(ostr, a, formatter, threshold, edgeitems, linewidth,
                     prefix, suffix, separator, width, index);
  return ostr.str();
}
} // namespace numcpp

#endif // NUMCPP_IO_TCC_INCLUDED
