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
void read_magic(std::ifstream &file, std::uint8_t &major, std::uint8_t &minor) {
  std::string magic(6, ' ');
  file.read(&magic[0], 6);
  if (magic != "\x93NUMPY") {
    throw std::ifstream::failure("File is not a valid .npy file");
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
void read_array_header(std::ifstream &file, std::uint8_t version,
                       shape_t<Rank> &shape, layout_t &order) {
  std::string header;
  // Version 1.0 uses 2 bytes for the length while version 2.0 uses 4.
  if (version < 2) {
    std::uint16_t header_len;
    file.read(reinterpret_cast<char *>(&header_len), 2);
    header.resize(header_len);
  } else {
    std::uint32_t header_len;
    file.read(reinterpret_cast<char *>(&header_len), 4);
    header.resize(header_len);
  }
  file.read(&header[0], header.size());
  // Check whether header is a valid Pydict with the required fields.
  std::vector<std::pair<std::string, std::string>> dict;
  if (!parse_pydict(header, dict)) {
    throw std::ifstream::failure("File is corrupted or malformed");
  }
  std::sort(dict.begin(), dict.end());
  if (dict.size() != 3 || dict[0].first != "descr" ||
      dict[1].first != "fortran_order" || dict[2].first != "shape") {
    throw std::ifstream::failure("File is corrupted or malformed");
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
    throw std::ifstream::failure("fortran_order must be True or False");
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
template <class T> void read_array(std::ifstream &file, T *data, size_t n) {
  std::streampos offset = file.tellg();
  file.seekg(0, std::ios::end);
  size_t bytesize = file.tellg() - offset;
  file.seekg(offset, std::ios::beg);
  if (bytesize < n * sizeof(T)) {
    throw std::ifstream::failure("File is corrupted or malformed");
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
    throw std::ifstream::failure(error.str());
  }
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
void write_magic(std::ofstream &file, std::uint8_t major, std::uint8_t minor) {
  file.write("\x93NUMPY", 6);
  file.write(reinterpret_cast<char *>(&major), 1);
  file.write(reinterpret_cast<char *>(&minor), 1);
}

/**
 * @brief Writes the array header to a .npy file.
 */
template <class T, size_t Rank>
void write_array_header(std::ofstream &file, const shape_t<Rank> &shape,
                        layout_t order) {
  std::ostringstream buffer;
  std::string descr = dtype_to_descr<T>();
  std::string f_order = (order == column_major) ? "True" : "False";
  buffer << "{'descr': " << descr << ", 'fortran_order': " << f_order
         << ", 'shape': " << shape << "}";
  std::string header = buffer.str();
  std::uint16_t header_len = header.size();
  file.write(reinterpret_cast<char *>(&header_len), 2);
  file.write(header.data(), header_len);
}

/**
 * @brief Write the array's contents to a .npy file.
 */
template <class T, class InputIterator>
void write_array(std::ofstream &file, InputIterator first, InputIterator last) {
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
    throw std::ofstream::failure(error.str());
  }
  detail::write_magic(file, 1, 0);
  detail::write_array_header<T>(file, data.shape(), data.layout());
  detail::write_array<T>(file, data.self().begin(), data.self().end());
  file.close();
}

/// Text files.

namespace detail {
/**
 * @brief Fill a tensor with data from an ifstream object.
 */
template <class T>
void load_file_data(std::ifstream &file, tensor<T, 2> &data, char delimiter,
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
      throw std::ifstream::failure(
          "Number of columns is not the same for all the rows");
    }
    ++shape[0];
  }
  data = std::move(tensor<T, 2>(buffer.begin(), shape, row_major));
}

template <class T>
void load_file_data(std::ifstream &file, tensor<T, 2> &data, char delimiter,
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
void load_file_data(std::ifstream &file, tensor<T, 1> &data, char, char newline,
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
void load_file_data(std::ifstream &file, tensor<T, 1> &data, char delimiter,
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
  static_assert(Rank == 1 || Rank == 2,
                "Output tensor must be 1-dimensional or 2-dimensional");
  std::ifstream file(filename);
  if (!file) {
    std::ostringstream error;
    error << "Input file " << filename << " does not exist or cannot be read";
    throw std::ifstream::failure(error.str());
  }
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
void set_printoptions_flags(std::basic_ostream<charT, traits> &ostr) {
  using namespace printoptions;
  if (precision == fullprecision) {
    ostr.precision(std::numeric_limits<T>::max_digits10);
  } else {
    ostr.precision(precision);
  }
  switch (floatmode) {
  case defaultfloat:
    ostr.unsetf(std::ios::floatfield);
    break;
  case fixed:
    ostr.setf(std::ios::floatfield, std::ios::fixed);
    break;
  case scientific:
    ostr.setf(std::ios::floatfield, std::ios::scientific);
    break;
  }
}

/**
 * @brief Save the tensor's contents to a text file.
 */
template <class Container, class T>
void save_file_data(std::ofstream &file,
                    const expression<Container, T, 2> &data, char delimiter,
                    char newline) {
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
void save_file_data(std::ofstream &file,
                    const expression<Container, T, 1> &data, char,
                    char newline) {
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
  static_assert(Rank == 1 || Rank == 2,
                "Input tensor must be 1-dimensional or 2-dimensional");
  std::ofstream file(filename);
  if (!file) {
    std::ostringstream error;
    error << "Ouput file " << filename << " cannot be written";
    throw std::ofstream::failure(error.str());
  }
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

template <class charT, class traits, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape) {
  charT ch;
  bool fail = true;
  if (istr >> ch) {
    if (traits::eq(ch, istr.widen('('))) {
      for (size_t i = 0; i < Rank; ++i) {
        if (istr >> shape[i] >> ch) {
          if (traits::eq(ch, istr.widen(','))) {
            if (i < Rank - 1) {
              continue;
            }
          } else if (traits::eq(ch, istr.widen(')'))) {
            if (i == Rank - 1) {
              fail = false;
              break;
            }
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
  if (fail) {
    istr.setstate(std::ios_base::failbit);
  }
  return istr;
}

template <class charT, class traits>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, shape_t<1> &shape) {
  charT ch;
  bool fail = true;
  if (istr >> ch) {
    if (traits::eq(ch, istr.widen('('))) {
      if (istr >> shape[0] >> ch) {
        if (traits::eq(ch, istr.widen(','))) {
          if (istr >> ch) {
            if (traits::eq(ch, istr.widen(')'))) {
              fail = false;
            } else {
              istr.putback(ch);
            }
          }
        } else {
          istr.putback(ch);
        }
      }
    } else {
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
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr,
           const shape_t<Rank> &shape) {
  std::basic_stringstream<charT, traits> sstr;
  sstr.flags(ostr.flags());
  sstr.imbue(ostr.getloc());
  sstr << "(" << shape[0];
  for (size_t i = 1; i < Rank; ++i) {
    sstr << ", " << shape[i];
  }
  sstr << ")";
  return ostr << sstr.str();
}

template <class charT, class traits>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr, const shape_t<1> &shape) {
  std::basic_stringstream<charT, traits> sstr;
  sstr.flags(ostr.flags());
  sstr.imbue(ostr.getloc());
  sstr << "(" << shape[0] << ",)";
  return ostr << sstr.str();
}

namespace detail {
/**
 * @brief Calls to operator>> on the input stream object.
 */
template <class charT, class traits, class T>
inline std::basic_istream<charT, traits> &
input(std::basic_istream<charT, traits> &istr, T &val) {
  return istr >> val;
}

/**
 * @brief Partial specialization of read for strings. Strings need to be
 * delimited by quotation marks when reading arrays of strings.
 */
template <class charT, class traits>
std::basic_istream<charT, traits> &
input(std::basic_istream<charT, traits> &istr,
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
 * @brief  Partial specialization of read for complex numbers. Complex numbers
 * can be read using default format or using real + imag i format.
 */
template <class charT, class traits, class T>
std::basic_istream<charT, traits> &
input_complex(std::basic_istream<charT, traits> &istr, std::complex<T> &z) {
  T x, y;
  charT ch;
  bool fail = true;
  if (istr >> x) {
    if (istr >> ch) {
      charT plus = istr.widen('+'), minus = istr.widen('-');
      if (traits::eq(ch, plus) || traits::eq(ch, minus)) {
        bool sign = traits::eq(ch, minus);
        if (istr >> y >> ch) {
          if (traits::eq(ch, istr.widen('i'))) {
            z = std::complex<T>(x, sign ? -y : y);
            fail = false;
          } else {
            istr.putback(ch);
          }
        }
      } else if (traits::eq(ch, istr.widen('i'))) {
        z = std::complex<T>(T(), x);
        fail = false;
      } else {
        istr.putback(ch);
        z = x;
        fail = false;
      }
    } else {
      z = x;
      fail = false;
    }
  }
  if (fail) {
    istr.setstate(std::ios_base::failbit);
  }
  return istr;
}

template <class charT, class traits, class T>
std::basic_istream<charT, traits> &
input(std::basic_istream<charT, traits> &istr, std::complex<T> &z) {
  using namespace printoptions;
  switch (complexmode) {
  case defaultcomplex:
    return istr >> z;
  case arithmetic:
    return input_complex(istr, z);
  }
  return istr;
}

/**
 * @brief Read the elements of a tensor recursively from the first axis to the
 * last axis. Returns whether the operation was successfull.
 */
template <class charT, class traits, class T, size_t Rank>
bool input_tensor(std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape,
                  std::vector<T> &buffer, size_t axis = 0);

/**
 * @brief Read a list of values and append them at the end of a std::vector.
 * Save in size the number of extracted values. Return whether the operation was
 * sucessfull.
 */
template <class charT, class traits, class T>
bool input_list(std::basic_istream<charT, traits> &istr, size_t &size,
                std::vector<T> &buffer) {
  charT ch;
  T val;
  size = 0;
  if (istr >> ch) {
    if (traits::eq(ch, istr.widen('['))) {
      while (input(istr, val) && istr >> ch) {
        ++size;
        buffer.push_back(val);
        if (traits::eq(ch, istr.widen(','))) {
          continue;
        } else if (traits::eq(ch, istr.widen(']'))) {
          return true;
        } else {
          istr.putback(ch);
          break;
        }
      }
    } else {
      istr.putback(ch);
    }
  }
  return false;
}

/**
 * @brief Read a block of values and append them at the end of a std::vector.
 * Save the size of the block in shape. Return whether the operation was
 * sucessfull.
 */
template <class charT, class traits, class T, size_t Rank>
bool input_block(std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape,
                 std::vector<T> &buffer, size_t axis) {
  charT ch;
  shape[axis] = 0;
  if (istr >> ch) {
    if (traits::eq(ch, istr.widen('['))) {
      size_t n = 0;
      while (input_tensor(istr, shape, buffer, axis + 1) && istr >> ch) {
        if (shape[axis] == 0) {
          n = shape[axis + 1];
        } else if (n != shape[axis + 1]) {
          break;
        }
        ++shape[axis];
        if (traits::eq(ch, istr.widen(','))) {
          continue;
        } else if (traits::eq(ch, istr.widen(']'))) {
          return true;
        } else {
          istr.putback(ch);
          break;
        }
      }
    } else {
      istr.putback(ch);
    }
  }
  return false;
}

template <class charT, class traits, class T, size_t Rank>
bool input_tensor(std::basic_istream<charT, traits> &istr, shape_t<Rank> &shape,
                  std::vector<T> &buffer, size_t axis) {
  if (axis == Rank - 1) {
    return input_list(istr, shape[axis], buffer);
  } else {
    return input_block(istr, shape, buffer, axis);
  }
}
} // namespace detail

template <class charT, class traits, class T, size_t Rank>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &istr, tensor<T, Rank> &arg) {
  shape_t<Rank> shape;
  std::vector<T> buffer;
  if (detail::input_tensor(istr, shape, buffer)) {
    arg = std::move(tensor<T, Rank>(buffer.begin(), shape, row_major));
  } else {
    arg = std::move(tensor<T, Rank>());
    istr.setstate(std::ios_base::failbit);
  }
  return istr;
}

namespace detail {
/**
 * @brief Calls to operator<< on the output stream object.
 */
template <class charT, class traits, class T>
inline std::basic_ostream<charT, traits> &
print(std::basic_ostream<charT, traits> &ostr, const T &val) {
  return ostr << val;
}

/**
 * @brief Partial specialization of print for strings. Strings are surrounded by
 * quotation marks when printing arrays of strings.
 */
template <class charT, class traits>
inline std::basic_ostream<charT, traits> &
print(std::basic_ostream<charT, traits> &ostr,
      const std::basic_string<charT, traits> &str) {
  char quotes = (str.find('\"') >= str.size()) ? '\"' : '\'';
  return ostr << (quotes + str + quotes);
}

/**
 * @brief Partial specialization of print for complex numbers. Complex numbers
 * can be printed using default format or using real + imag i format.
 */
template <class charT, class traits, class T>
std::basic_ostream<charT, traits> &
print_complex(std::basic_ostream<charT, traits> &ostr,
              const std::complex<T> &z) {
  std::basic_ostringstream<charT, traits> buffer;
  buffer.flags(ostr.flags());
  buffer.imbue(ostr.getloc());
  buffer.precision(ostr.precision());
  buffer << z.real() << std::showpos << z.imag() << "i";
  return ostr << buffer.str();
}

template <class charT, class traits, class T>
std::basic_ostream<charT, traits> &
print(std::basic_ostream<charT, traits> &ostr, const std::complex<T> &z) {
  using namespace printoptions;
  switch (complexmode) {
  case defaultcomplex:
    return ostr << z;
  case arithmetic:
    return print_complex(ostr, z);
  }
  return ostr;
}

/**
 * @brief Print the elements of a tensor recursively from the first axis to the
 * last axis.
 */
template <class charT, class traits, class Container, class T, size_t Rank>
void print_tensor(std::basic_ostream<charT, traits> &ostr,
                  const expression<Container, T, Rank> &arg,
                  index_t<Rank> &index, size_t axis, size_t width);

/**
 * @brief Print the elements of a tensor along a given axis. The remaining axes
 * are fixed with a given index.
 */
template <class charT, class traits, class Container, class T, size_t Rank>
void print_slice(std::basic_ostream<charT, traits> &ostr,
                 const expression<Container, T, Rank> &arg,
                 index_t<Rank> &index, size_t axis, size_t width) {
  using namespace printoptions;
  size_t size = arg.shape(axis);
  size_t linesize = axis + 1;
  std::string delim = "";
  index[axis] = 0;
  while (index[axis] < size) {
    ostr << delim;
    linesize += delim.size();
    if (size >= threshold && size > 2 * edgeitems && edgeitems <= index[axis] &&
        index[axis] < size - edgeitems) {
      delim = "..., ";
      if (linesize + delim.size() >= linewidth) {
        ostr << "\n";
        ostr.width(axis + 1);
        ostr << " ";
        linesize = axis + 1;
      }
      index[axis] = size - edgeitems;
      continue;
    }
    if (linesize + width >= linewidth && index[axis] > 0) {
      ostr << "\n";
      ostr.width(axis + 1);
      ostr << " ";
      linesize = axis + 1;
    }
    ostr.width(width);
    print(ostr, arg[index]);
    delim = ", ";
    linesize += width;
    ++index[axis];
  }
}

/**
 * @brief Print a block of tensor elements. The first axes are fixed with a
 * given index while the later axes are printed recursively.
 */
template <class charT, class traits, class Container, class T, size_t Rank>
void print_block(std::basic_ostream<charT, traits> &ostr,
                 const expression<Container, T, Rank> &arg,
                 index_t<Rank> &index, size_t axis, size_t width) {
  using namespace printoptions;
  size_t size = arg.shape(axis);
  std::string delim = "";
  index[axis] = 0;
  while (index[axis] < size) {
    ostr << delim;
    if (size >= threshold && size > 2 * edgeitems && edgeitems <= index[axis] &&
        index[axis] < size - edgeitems) {
      delim = "...,";
      delim.append(Rank - axis - 1, '\n');
      delim.append(axis + 1, ' ');
      index[axis] = size - edgeitems;
      continue;
    }
    print_tensor(ostr, arg, index, axis + 1, width);
    delim = ",";
    delim.append(Rank - axis - 1, '\n');
    delim.append(axis + 1, ' ');
    ++index[axis];
  }
}

/**
 * @brief Return the number of characters required to print each value in the
 * tensor.
 */
template <class charT, class traits, class Container, class T, size_t Rank>
size_t get_print_width(std::basic_ostream<charT, traits> &ostr,
                       const expression<Container, T, Rank> &arg,
                       index_t<Rank> &index, size_t axis = 0) {
  using namespace printoptions;
  size_t size = arg.shape(axis);
  size_t max_width = 0;
  index[axis] = 0;
  while (index[axis] < size) {
    if (size >= threshold && size > 2 * edgeitems && edgeitems <= index[axis] &&
        index[axis] < size - edgeitems) {
      index[axis] = size - edgeitems;
      continue;
    }
    if (axis == Rank - 1) {
      std::basic_ostringstream<charT, traits> buffer;
      buffer.flags(ostr.flags());
      buffer.imbue(ostr.getloc());
      buffer.precision(ostr.precision());
      print(buffer, arg[index]);
      max_width = std::max(max_width, buffer.str().size());
    } else {
      max_width =
          std::max(max_width, get_print_width(ostr, arg, index, axis + 1));
    }
    ++index[axis];
  }
  return max_width;
}

template <class charT, class traits, class Container, class T, size_t Rank>
void print_tensor(std::basic_ostream<charT, traits> &ostr,
                  const expression<Container, T, Rank> &arg,
                  index_t<Rank> &index, size_t axis, size_t width) {
  ostr << "[";
  if (axis == Rank - 1) {
    print_slice(ostr, arg, index, axis, width);
  } else {
    print_block(ostr, arg, index, axis, width);
  }
  ostr << "]";
}
} // namespace detail

template <class charT, class traits, class Container, class T, size_t Rank>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &ostr,
           const expression<Container, T, Rank> &arg) {
  std::ios_base::fmtflags default_flags = ostr.flags();
  size_t default_precision = ostr.precision();
  detail::set_printoptions_flags<T>(ostr);
  index_t<Rank> index;
  size_t width = detail::get_print_width(ostr, arg, index);
  detail::print_tensor(ostr, arg, index, 0, width);
  ostr.flags(default_flags);
  ostr.precision(default_precision);
  return ostr;
}
} // namespace numcpp

#endif // NUMCPP_IO_TCC_INCLUDED
